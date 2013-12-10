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
    tmRealRect myFilterCoordWeb = tmRealRect (xymin.x, xymin.y, xymax.x, xymax.y);
    
    wxLogMessage(_("web coordintes: %f, %f, %f, %f"), xymin.x, xymin.y, xymax.x, xymax.y);
    if (m_WebFrameRef == NULL) {
        return true;
    }
    
    m_WebFrameRef->ZoomToExtend(myFilterCoordWeb);
    return true;
}


bool tmGISDataRasterWeb::IsImageInsideVisibleArea (){
    // return always true. Web image covers the world.
    return true;
}


CPLErr tmGISDataRasterWeb::GetImageData(unsigned char **imgbuf, unsigned int   *imglen,
                                        unsigned char **maskbuf, unsigned int   *masklen,
                                        wxSize imgSize){
    wxBitmap * myBmp = m_WebFrameRef->GetPageAsBitmap();
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

    wxImage myImage = myBmp->ConvertToImage();
    unsigned char * myImgData = myImage.GetData();
    wxASSERT(myImgData);
    std::memcpy(*imgbuf, myImgData, *imglen * sizeof(unsigned char *));
    wxDELETE(myBmp);
    
    return CE_None;
}

