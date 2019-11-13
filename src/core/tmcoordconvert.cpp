/***************************************************************************
 tmcoordconvert
 -------------------
 copyright            : (C) 2013 Lucien Schreiber
 email                : lucien.schreiber at gmail dot com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "tmcoordconvert.h"
#include "gdal.h"
#include "../gis/tmgisscale.h"

// EPSG 3857, google and bing projection system
int projEPSGGoogle = 3857;

// EPSG 21781, CH1903 / LV03 Swiss projection
int projEPSGCH1903 = 21781;

// EPSG 2056, CH1903+ / LV95 Swiss projection
int projEPSGCH1903plus = 2056;

// EPSG 4326, WGS projection
int projEPSGWGS84 = 4326;


// WGS84 Geodesic definition see: http://geographiclib.sourceforge.net/html/C/
double geod_a = 6378137;
double geod_f = 1 / 298.257223563;


OGRSpatialReference *tmCoordConvert::_CreateSpatialRef(PRJDEF_PROJ_TYPE proj)
{
    OGRSpatialReference *mySpatRef = new OGRSpatialReference();
    mySpatRef->SetAxisMappingStrategy(OAMS_TRADITIONAL_GIS_ORDER);
    switch (proj) {
        case PROJ_SWISS_CH1903:
            mySpatRef->importFromEPSG(projEPSGCH1903);
            break;

        case PROJ_SWISS_CH1903PLUS:
            mySpatRef->importFromEPSG(projEPSGCH1903plus);
            break;

        case PROJ_WORLD_WGS84:
            mySpatRef->importFromEPSG(projEPSGWGS84);
            break;

        default:
            wxLogError(_("Unsupported projection!"));
            wxDELETE(mySpatRef);
            break;
    }
    return mySpatRef;
}


OGRSpatialReference *tmCoordConvert::_CreateSpatialRefGoogle()
{
    OGRSpatialReference *mySpatRef = new OGRSpatialReference();
    mySpatRef->SetAxisMappingStrategy(OAMS_TRADITIONAL_GIS_ORDER);
    mySpatRef->importFromEPSG(projEPSGGoogle);
    return mySpatRef;
}


wxRealPoint tmCoordConvert::_Transform(OGRSpatialReference *refin, OGRSpatialReference *refout, const wxRealPoint &in)
{
    if (refin == NULL || refout == NULL) {
        wxLogError(_("Empty projection reference found!"));
        return wxRealPoint(wxNOT_FOUND, wxNOT_FOUND);
    }

    double myPtX = in.x;
    double myPtY = in.y;

    OGRCoordinateTransformation *myTransform = OGRCreateCoordinateTransformation(refin, refout);
    if (myTransform == NULL) {
        wxLogError(_("Unable to init coordinate transformation! Proj library not found!"));
        return wxRealPoint(0, 0);
    }
    myTransform->Transform(1, &myPtX, &myPtY, NULL);
    wxRealPoint myOutPt(myPtX, myPtY);
    OCTDestroyCoordinateTransformation(myTransform);
    return myOutPt;
}


tmCoordConvert::tmCoordConvert(PRJDEF_PROJ_TYPE projtype)
{
    m_ProjType = projtype;
}


tmCoordConvert::~tmCoordConvert()
{
}


wxRealPoint tmCoordConvert::GetPointWGS(const wxRealPoint &in)
{
    if (m_ProjType == PROJ_WORLD_WGS84) {
        return in;
    }

    OGRSpatialReference *myInRef = _CreateSpatialRef(m_ProjType);
    wxASSERT(myInRef);
    OGRSpatialReference *myOutRef = _CreateSpatialRef(PROJ_WORLD_WGS84);
    wxASSERT(myOutRef);
    return _Transform(myInRef, myOutRef, in);
}


wxRealPoint tmCoordConvert::GetPointGoogle(const wxRealPoint &in)
{
    return _Transform(_CreateSpatialRef(m_ProjType), _CreateSpatialRefGoogle(), in);
}


double tmCoordConvert::GetDistance(const wxRealPoint &p1, const wxRealPoint &p2)
{
    if (m_ProjType == PROJ_SWISS_CH1903 || m_ProjType == PROJ_SWISS_CH1903PLUS) {
        if (p1.y == p2.y) {
            return fabs(p1.x - p2.x);
        } else if (p1.x == p2.x) {
            return fabs(p1.y - p2.y);
        }
    }

    struct geod_geodesic myGeodesic;
    geod_init(&myGeodesic, geod_a, geod_f);

    wxRealPoint p1wgs = GetPointWGS(p1);
    wxRealPoint p2wgs = GetPointWGS(p2);

    double dist = 0;
    double azi1 = 0;
    double azi2 = 0;
    geod_inverse(&myGeodesic, p1wgs.y, p1wgs.x, p2wgs.y, p2wgs.x, &dist, &azi1, &azi2);

    return dist;
}


wxRealPoint tmCoordConvert::_GetPointLocalFromWGS(const wxRealPoint &pt)
{
    if (m_ProjType == PROJ_WORLD_WGS84) {
        return pt;
    }

    OGRSpatialReference *myInRef = _CreateSpatialRef(PROJ_WORLD_WGS84);
    wxASSERT(myInRef);
    OGRSpatialReference *myOutRef = _CreateSpatialRef(m_ProjType);
    wxASSERT(myOutRef);
    return _Transform(myInRef, myOutRef, pt);
}


wxRealPoint tmCoordConvert::GetPointAtDistance(const wxRealPoint &p1, double distance, double azimut)
{
    if (m_ProjType == PROJ_SWISS_CH1903 || m_ProjType == PROJ_SWISS_CH1903PLUS) {
        wxRealPoint newPointCH(p1);
        if (azimut == 90) {
            newPointCH.x += distance;
            return newPointCH;
        } else if (azimut == 180) {
            newPointCH.y -= distance;
            return newPointCH;
        }
    }

    struct geod_geodesic myGeodesic;
    geod_init(&myGeodesic, geod_a, geod_f);

    wxRealPoint p1wgs = GetPointWGS(p1);
    wxRealPoint pDestWgs;

    geod_direct(&myGeodesic, p1wgs.y, p1wgs.x, azimut, distance, &pDestWgs.y, &pDestWgs.x, 0);
    return _GetPointLocalFromWGS(pDestWgs);
}


wxString tmCoordConvert::GetDistanceHuman(double distanceM)
{
    if (distanceM < 1.0) {
        return wxString::Format(_T("%f [cm]"), distanceM * 100);
    }

    if ((distanceM / 1000.0) < 1.0) {
        return wxString::Format(_T("%f [m]"), distanceM);
    }

    return wxString::Format(_T("%f [km]"), distanceM / 1000.0);
}


char *tmCoordConvert::GetWKTProjectionGoogle()
{
    OGRSpatialReference *mySpatRef = _CreateSpatialRefGoogle();
    if (mySpatRef == NULL) {
        wxLogError(_("Getting Google projection failed!"));
        return NULL;
    }

    char *myWktTxt = NULL;
    mySpatRef->exportToWkt(&myWktTxt);
    return myWktTxt;
}


char *tmCoordConvert::GetWKTProjectionLocal()
{
    OGRSpatialReference *mySpatRef = _CreateSpatialRef(m_ProjType);
    if (mySpatRef == NULL) {
        wxLogError(_("Getting Local projection failed!"));
        return NULL;
    }

    char *myWktTxt = NULL;
    mySpatRef->exportToWkt(&myWktTxt);
    return myWktTxt;
}

char *tmCoordConvert::GetESRIWKTProjectionLocal()
{
    OGRSpatialReference *mySpatRef = _CreateSpatialRef(m_ProjType);
    if (mySpatRef == NULL) {
        wxLogError(_("Getting Local projection failed!"));
        return NULL;
    }

    char *myWktTxt = NULL;
    mySpatRef->morphToESRI();
    mySpatRef->exportToWkt(&myWktTxt);
    return myWktTxt;
}

wxBitmap *tmCoordConvert::GetProjectGoogleRaster(wxBitmap *web_raster, tmRealRect *coord_web, tmRealRect *coord_local)
{
    // create source dataset
    //const char *pszFormat = "MEM";
    const char *pszFormat = "GTiff";
    GDALDriverH hDriver = GDALGetDriverByName(pszFormat);
    if (hDriver == NULL) {
        wxASSERT(hDriver);
        return NULL;
    }

    GDALDatasetH hOriginDS;
    char **papszOptions = NULL;
    wxFileName myOriginName(wxStandardPaths::Get().GetAppDocumentsDir(), _T("test_origin.tif"));
    hOriginDS = GDALCreate(hDriver, myOriginName.GetFullPath().mb_str(), web_raster->GetWidth(),
                           web_raster->GetHeight(), 3, GDT_Byte, papszOptions);

    double pxsize = (coord_web->x_max - coord_web->x_min) / web_raster->GetWidth();
    double pysize = (coord_web->y_max - coord_web->y_min) / web_raster->GetHeight() * -1.0;
    double adfGeoTransform[6] = {coord_web->x_min, pxsize, 0, coord_web->y_max, 0, pysize};
    GDALSetGeoTransform(hOriginDS, adfGeoTransform);

    OGRSpatialReference *myOriginSpatial = _CreateSpatialRefGoogle();
    wxASSERT(myOriginSpatial);

    char *pSpatialWKT = NULL;
    myOriginSpatial->exportToWkt(&pSpatialWKT);
    GDALSetProjection(hOriginDS, pSpatialWKT);


    wxImage myImage = web_raster->ConvertToImage();
    unsigned char *myImgData = myImage.GetData();

    for (int i = 1; i <= 3; i++) {
        int offs = i - 1;
        GDALRasterBandH hband = GDALGetRasterBand(hOriginDS, i);
        if (0 <= offs && offs < 3) {
            CPLErr ret = GDALRasterIO(hband, GF_Write, 0, 0, web_raster->GetWidth(), web_raster->GetHeight(),
                                      myImgData + offs, web_raster->GetWidth(), web_raster->GetHeight(), GDT_Byte, 3,
                                      0);

            if (ret == CE_Failure) {
                wxLogError(_T("An unknown error occured while writing band %i"), i);
                break;
            }
        }
    }


    OGRSpatialReference *myDestSpatial = _CreateSpatialRef(m_ProjType);
    wxASSERT(myDestSpatial);
    char *pSpatialDestWKT = NULL;
    myDestSpatial->exportToWkt(&pSpatialDestWKT);

    wxFileName myDestName(wxStandardPaths::Get().GetAppDocumentsDir(), _T("test_dest.tif"));
    CPLErr myErr = GDALCreateAndReprojectImage(hOriginDS, pSpatialWKT, myDestName.GetFullPath().mb_str(),
                                               pSpatialDestWKT, hDriver, NULL, GRA_Bilinear, 0.0, 0.0, NULL, NULL,
                                               NULL);
    if (myErr != CE_None) {
        wxLogError(_("Reprojecting web raster failed!"));
    }

    CPLFree(pSpatialWKT);
    CPLFree(pSpatialDestWKT);

    /* create destination dataset
    GDALDatasetH hDestDS;
    wxFileName myDestName (wxStandardPaths::Get().GetAppDocumentsDir(), _T("test_dest.tif"));
    hDestDS = GDALCreate( hDriver, myDestName.GetFullPath().mb_str(), web_raster->GetWidth(), web_raster->GetHeight(), 3, GDT_Byte, papszOptions );
    
    double pxdsize = (coord_local->x_max - coord_local->x_min) / web_raster->GetWidth();
    double pydsize = (coord_local->y_max - coord_local->y_min) / web_raster->GetHeight() * -1.0;
    double adfdGeoTransform[6] = { coord_local->x_min, pxdsize, 0, coord_local->y_max, 0, pydsize };
    GDALSetGeoTransform( hDestDS, adfdGeoTransform );
    
    OGRSpatialReference * myDestSpatial = _CreateSpatialRef(m_ProjType);
    wxASSERT(myDestSpatial);
    
    char * pSpatialdWKT = NULL;
    myDestSpatial->exportToWkt(&pSpatialdWKT);
    GDALSetProjection(hDestDS, pSpatialdWKT);
    CPLFree(pSpatialdWKT);


    CPLErr myErr = GDALReprojectImage(hOriginDS, NULL, hDestDS, NULL, GRA_NearestNeighbour, 0.0, 1, NULL, NULL, NULL);
    if (myErr != CE_None) {
        wxLogError(_("Reprojecting web raster failed!"));
    }*/

    GDALClose(hOriginDS);
    //GDALClose( hDestDS );

    return new wxBitmap(*web_raster);
}



