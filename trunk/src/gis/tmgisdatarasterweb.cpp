/***************************************************************************
 tmgisdatarasterweb
 -------------------
 copyright            : (C) 2013 CREALP Lucien Schreiber
 email                : lucien.schreiber at crealp dot vs dot ch
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "tmgisdatarasterweb.h"
#include "../gui/tmwebframe.h"
#include "tmrenderer.h"


const double tmGISDataRasterWeb::m_AvaillableResolutions [] ={
    156543.03390625, 78271.516953125, 39135.7584765625,
    19567.87923828125, 9783.939619140625, 4891.9698095703125,
    2445.9849047851562, 1222.9924523925781, 611.4962261962891,
    305.74811309814453, 152.87405654907226, 76.43702827453613,
    38.218514137268066, 19.109257068634033, 9.554628534317017,
    4.777314267158508, 2.388657133579254, 1.194328566789627,
    0.5971642833948135, 0.29858214169740677, 0.14929107084870338,
    0.07464553542435169
};



tmGISDataRasterWeb::tmGISDataRasterWeb(){
	m_FileType = _T("Web Raster");
	m_ClassType = tmGIS_RASTER_WEB;
    m_WebFrameRef = NULL;
}

tmGISDataRasterWeb::~tmGISDataRasterWeb(){
}


wxString tmGISDataRasterWeb::GetDataSizeAsHtml (int iPrecision){
	wxString myResult = _("<U><B>Web Raster Size</B></U><BR>No Size!");
	return myResult;
}


bool tmGISDataRasterWeb::Open (const wxString & filename, bool bReadWrite){
    tmGISData::Open(filename, bReadWrite);
	wxASSERT(m_DataSet == NULL);
    
    if (m_WebFrameRef == NULL) {
        return false;
    }
    m_WebFrameRef->LoadPage(filename);
    return true;
}



tmRealRect tmGISDataRasterWeb::GetMinimalBoundingRectangle(){
    return tmRealRect(0,0,0,0);
}


bool tmGISDataRasterWeb::SetSpatialFilter (tmRealRect filter, int type){
    m_FilterCoordLocal = filter;
    
    // convert project coordinates into web coordinates and loads data into tmwebframe
    wxRealPoint xymin = GetCoordConvert()->GetPointGoogle(wxRealPoint(filter.x_min, filter.y_min));
    wxRealPoint xymax = GetCoordConvert()->GetPointGoogle(wxRealPoint(filter.x_max, filter.y_max));
    m_FilterCoordWeb = tmRealRect (xymin.x, xymin.y, xymax.x, xymax.y);
    
    wxLogMessage(_("web coordinates: %f, %f, %f, %f."), xymin.x, xymin.y, xymax.x, xymax.y);
    if (m_WebFrameRef == NULL) {
        return true;
    }
    
    m_WebFrameRef->ZoomToExtend(m_FilterCoordWeb);
    
    // Resolution
    double myResolution = _GetResolution(m_FilterCoordWeb);
    double myClosestResolution = _GetClosestAvaillableResolution(m_FilterCoordWeb, myResolution);
    wxLogMessage(_("Actual Resolution= %f, Closest Resolution= %f"), myResolution, myClosestResolution);
    
    double myResolutionDiff = myClosestResolution-myResolution;
    wxSize myClientSize = m_WebFrameRef->GetClientSize();
    double xdiff = myResolutionDiff * myClientSize.GetWidth();
    double ydiff = myResolutionDiff * myClientSize.GetHeight();
    
    tmRealRect myWebCoord (m_FilterCoordWeb);
    myWebCoord.x_max += xdiff / 2.0;
    myWebCoord.y_max += ydiff / 2.0;
    myWebCoord.x_min -= xdiff / 2.0;
    myWebCoord.y_min -= ydiff / 2.0;
    
    wxLogMessage(_("x difference= %f, y difference= %f"), xdiff , ydiff );
    wxLogMessage(_("web coord: %f, %f, %f, %f,"), myWebCoord.x_min, myWebCoord.y_min, myWebCoord.x_max, myWebCoord.y_max);
    wxLogMessage(_("x diffpx= %f, y diffpx= %f"), xdiff / myClosestResolution, ydiff / myClosestResolution);
    
    return true;
}



double tmGISDataRasterWeb::_GetClosestAvaillableResolution(tmRealRect bounds, double resolution){
    int iNbResolutions = sizeof(m_AvaillableResolutions) / sizeof(double);
    for (unsigned int i = 0; i< iNbResolutions; i++) {
        if (m_AvaillableResolutions[i] < resolution) {
            return m_AvaillableResolutions[i-1];
        }
    }
    return wxNOT_FOUND;
}


double tmGISDataRasterWeb::_GetResolution (tmRealRect bounds){
    wxSize myClientSize = m_WebFrameRef->GetClientSize();
    return std::max((bounds.x_max - bounds.x_min) / myClientSize.GetWidth(),
                    (bounds.y_max - bounds.y_min) / myClientSize.GetHeight());
}



bool tmGISDataRasterWeb::IsImageInsideVisibleArea (){
    // return always true. Web image covers the world.
    return true;
}


CPLErr tmGISDataRasterWeb::GetImageData(unsigned char **imgbuf, unsigned int   *imglen,
                                        unsigned char **maskbuf, unsigned int   *masklen,
                                        wxSize imgSize){
    wxBitmap * myBmp = m_WebFrameRef->GetPageAsBitmap(imgSize);
    if (myBmp == NULL) {
        return CE_Failure;
    }
    
    if (myBmp->GetWidth() != imgSize.GetWidth() ||
        myBmp->GetHeight() != imgSize.GetHeight()) {
        wxLogWarning(_("Web bitmap size incorrect. Expected: %d,%d received: %d,%d"),
                     imgSize.GetWidth(), imgSize.GetHeight(), myBmp->GetWidth(), myBmp->GetHeight());
        return CE_Failure;
    }
    
    
    *imglen = 3 * imgSize.GetWidth() * imgSize.GetHeight();
    *imgbuf = (unsigned char*)CPLMalloc(*imglen);
    if ( *imgbuf == NULL){
        wxLogError(_("The system does not have enough memory to project"));
        return CE_Failure;
    }
    
    // crop and stretch image
    double myResolution = _GetResolution(m_FilterCoordWeb);
    double myClosestResolution = _GetClosestAvaillableResolution(m_FilterCoordWeb, myResolution);
    double myResolutionDiff = myClosestResolution-myResolution;
    wxSize myClientSize = m_WebFrameRef->GetClientSize();
    double xpxdiff = myResolutionDiff * myClientSize.GetWidth() / myClosestResolution;
    double ypxdiff = myResolutionDiff * myClientSize.GetHeight() / myClosestResolution ;

    wxBitmap myCropBmp = myBmp->GetSubBitmap(wxRect(xpxdiff, ypxdiff, imgSize.GetWidth() - 2 * xpxdiff, imgSize.GetHeight() - 2 * ypxdiff));
    wxImage myImage = myCropBmp.ConvertToImage();
    myImage.Rescale(imgSize.GetWidth(), imgSize.GetHeight());
    unsigned char * myImgData = myImage.GetData();
    wxASSERT(myImgData);
    std::memcpy(*imgbuf, myImgData, *imglen * sizeof(unsigned char));
    wxDELETE(myBmp);
    return CE_None;
    
    
    /* TODO: reproject image from ESPG:3857 (Google, Bing) into local projection definition.
    wxBitmap * myProjBmp = GetCoordConvert()->GetProjectGoogleRaster(myBmp, &m_FilterCoordWeb, &m_FilterCoordLocal);
    wxDELETE(myBmp);
    if (myProjBmp == NULL) {
        wxLogError(_("Unable to project web raster"));
        return CE_Failure;
    }*/
}

