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
    while ( m_WebFrameRef->IsLoaded() == false );
    return true;
}



tmRealRect tmGISDataRasterWeb::GetMinimalBoundingRectangle(){
    return tmRealRect(0,0,0,0);
}


bool tmGISDataRasterWeb::SetSpatialFilter (tmRealRect filter, int type){
	// check if view moved !
	if (m_FilterCoordLocal == filter){
		return true;
	}
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
	while (m_WebFrameRef->HasZoomed() == false){
        continue; // wait for the loading of the raster
    }
    
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
    
    //wxLogMessage(_("x difference= %f, y difference= %f"), xdiff , ydiff );
    //wxLogMessage(_("web coord: %f, %f, %f, %f,"), myWebCoord.x_min, myWebCoord.y_min, myWebCoord.x_max, myWebCoord.y_max);
    //wxLogMessage(_("x diffpx= %f, y diffpx= %f"), xdiff / myClosestResolution, ydiff / myClosestResolution);
    return true;
}



double tmGISDataRasterWeb::_GetClosestAvaillableResolution(tmRealRect bounds, double resolution){
    int iNbResolutions = sizeof(m_AvaillableResolutions) / sizeof(double);
    for (int i = 0; i< iNbResolutions; i++) {
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
    
    // get center
    wxRect2DDouble myLocalRect;
    myLocalRect.SetLeftBottom( wxPoint2DDouble( m_FilterCoordWeb.x_min, m_FilterCoordWeb.y_min) );
    myLocalRect.SetRightTop( wxPoint2DDouble(m_FilterCoordWeb.x_max, m_FilterCoordWeb.y_max) );
    wxPoint2DDouble myCenter = myLocalRect.GetCentre();
    
    // get closest resolution
    double myResolution = _GetResolution(m_FilterCoordWeb);
    double myClosestResolution = _GetClosestAvaillableResolution(m_FilterCoordWeb, myResolution);

    wxSize myClientSize = m_WebFrameRef->GetClientSize();
    double halfWDeg = (myClientSize.GetWidth() * myClosestResolution) / 2;
    double halfHDeg = (myClientSize.GetHeight() * myClosestResolution) / 2;
    
    tmRealRect myRealWebBounds (myCenter.m_x - halfWDeg,
                                myCenter.m_y - halfHDeg,
                                myCenter.m_x + halfWDeg,
                                myCenter.m_y + halfHDeg);
    wxLogMessage (_("Web raster extent: %f, %f, %f, %f"), myRealWebBounds.x_min, myRealWebBounds.y_min, myRealWebBounds.x_max, myRealWebBounds.y_max);
    
    
    // save web raster
    wxString myDriverName = _T("GTiff");
    wxString myExtension = _T("tif");
    if (GetWebFrameRef() && GetWebFrameRef()->IsUsingRAM() == true) {
        myDriverName = _T("MEM");
        myExtension = _T("memory");
    }
    
    GDALDriverH hDriver = GDALGetDriverByName( myDriverName.mb_str() );
    if (hDriver == NULL) {
        wxASSERT(hDriver);
        return CE_Failure;
    }
    
    GDALDatasetH hOriginDS;
    char **papszOptions = NULL;
    wxFileName myOriginName (wxStandardPaths::Get().GetAppDocumentsDir(), _T("web_origin.") + myExtension);
    hOriginDS = GDALCreate( hDriver, myOriginName.GetFullPath().mb_str(), imgSize.GetWidth(), imgSize.GetHeight(), 3, GDT_Byte, papszOptions );
    
    double adfGeoTransform[6] = {
        myRealWebBounds.x_min, myClosestResolution,
        0, myRealWebBounds.y_max,
        0, myClosestResolution * -1.0
    };
    GDALSetGeoTransform( hOriginDS, adfGeoTransform );
    char * pSpatialWKT = GetCoordConvert()->GetWKTProjectionGoogle();
    GDALSetProjection(hOriginDS, pSpatialWKT);
    
    wxImage myImage = myBmp->ConvertToImage();
    unsigned char * myImgData = myImage.GetData();
    
    for (int i=1; i <= 3; i++){
        int offs = i -1;
        GDALRasterBandH hband =  GDALGetRasterBand(hOriginDS, i);
        if (0 <= offs && offs < 3){
            CPLErr ret = GDALRasterIO(hband, GF_Write, 0, 0,imgSize.GetWidth(),imgSize.GetHeight(), myImgData+offs, imgSize.GetWidth(), imgSize.GetHeight(), GDT_Byte, 3, 0);
            
            if (ret == CE_Failure){
                wxLogError(_T( "An unknown error occured while writing band %i"),i);
                break;
            }
        }
    }
    
    // Create a transformation object from the source to
    // destination coordinate system.
    char * pSpatialDestWKT = GetCoordConvert()->GetWKTProjectionLocal();
    void * hTransformArg = GDALCreateGenImgProjTransformer( hOriginDS, pSpatialWKT, NULL, pSpatialDestWKT, true, 1000.0, 0 );
    if( hTransformArg == NULL )
        return CE_Failure;
    
    // Get approximate output definition.
    double adfDstGeoTransform[6];
    int nPixels;
    int nLines;
    
    if( GDALSuggestedWarpOutput( hOriginDS ,GDALGenImgProjTransform, hTransformArg,adfDstGeoTransform, &nPixels, &nLines ) != CE_None ){
        return CE_Failure;
    }
    GDALDestroyGenImgProjTransformer( hTransformArg );
    
    // Create the output file.
    wxFileName myDestName (wxStandardPaths::Get().GetAppDocumentsDir(), _T("web_projected.") + myExtension);
    GDALDatasetH hDstDS = GDALCreate( hDriver, myDestName.GetFullPath().mb_str(),
                                     nPixels, nLines, GDALGetRasterCount(hOriginDS),
                                     GDALGetRasterDataType(GDALGetRasterBand(hOriginDS,1)),
                                     papszOptions);
    if( hDstDS == NULL ){
        return CE_Failure;
    }
    
    // Write out the projection definition.
    GDALSetProjection( hDstDS, pSpatialDestWKT );
    GDALSetGeoTransform( hDstDS, adfDstGeoTransform );
    
    // Perform the reprojection.
    CPLErr eErr = GDALReprojectImage( hOriginDS, pSpatialWKT, hDstDS, pSpatialDestWKT,
                                     GRA_Bilinear, 0.0, 0.0,NULL, NULL, NULL );
    if (eErr != CE_None) {
        wxLogError(_("Reprojecting web raster failed!"));
    }
    
    //GDALDataset * myDataset = static_cast<GDALDataset *>( hOriginDS );
    OGRFree(pSpatialWKT);
    OGRFree(pSpatialDestWKT);
    GDALClose(hOriginDS);
    
    // crop
    tmGISDataRaster myRaster;
    myRaster.UseExisting(myDestName.GetFullPath(), hDstDS);
    myRaster.SetSpatialFilter(m_FilterCoordLocal, 0);
    return myRaster.GetImageData(imgbuf, imglen, maskbuf, masklen, imgSize);
     // GDALClose( hDstDS ); Don't close, will be closed by myRaster !
}

