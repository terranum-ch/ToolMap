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

// EPSG 3857, google and bing projection system
wxString tmCoordConvert::m_ProjTextGoogle = _T("+proj=merc +a=6378137 +b=6378137 +lat_ts=0.0 +lon_0=0.0 +x_0=0.0 +y_0=0 +k=1.0 +units=m +nadgrids=@null +wktext  +no_defs");

// ESPG 21781, CH1903 / LV03 Swiss projection
wxString tmCoordConvert::m_ProjTextSwiss = _T("+proj=somerc +lat_0=46.95240555555556 +lon_0=7.439583333333333 +k_0=1 +x_0=600000 +y_0=200000 +ellps=bessel +towgs84=674.4,15.1,405.3,0,0,0,0 +units=m +no_defs");

// ESPG 4326, WGS projection
wxString tmCoordConvert::m_ProjTextWGS84 = _T(" +proj=longlat +datum=WGS84 +no_defs  ");


// WGS84 Geodesic definition see: http://geographiclib.sourceforge.net/html/C/
double tmCoordConvert::m_Geod_a = 6378137;
double tmCoordConvert::m_Geod_f = 1/298.257223563;


OGRSpatialReference * tmCoordConvert::_CreateSpatialRef(PRJDEF_PROJ_TYPE proj) {
    OGRSpatialReference * mySpatRef = new OGRSpatialReference();
    switch (proj) {
        case PROJ_SWISSPROJ:
            mySpatRef->importFromProj4(m_ProjTextSwiss.mb_str(wxConvUTF8));
            break;
            
            case PROJ_WORLDWGS84:
            mySpatRef->importFromProj4(m_ProjTextWGS84.mb_str(wxConvUTF8));
            break;
            
        default:
            wxLogError(_("Unsupported projection!"));
            wxDELETE (mySpatRef);
            break;
    }
    return mySpatRef;
}


OGRSpatialReference * tmCoordConvert::_CreateSpatialRefGoogle() {
    OGRSpatialReference * mySpatRef = new OGRSpatialReference();
    mySpatRef->importFromProj4(m_ProjTextGoogle.mb_str(wxConvUTF8));
    return mySpatRef;
}


wxRealPoint tmCoordConvert::_Transform ( OGRSpatialReference * refin, OGRSpatialReference * refout, const wxRealPoint & in){
    if (refin == NULL || refout == NULL) {
        wxLogError(_("Empty projection reference found!"));
        return wxRealPoint (wxNOT_FOUND, wxNOT_FOUND);
    }
    
    double myPtX = in.x;
    double myPtY = in.y;
    
    OGRCoordinateTransformation * myTransform = OGRCreateCoordinateTransformation (refin , refout);
    if (myTransform == NULL) {
        wxLogError(_("Unable to init coordinate transformation! Proj library not found!"));
        return wxRealPoint(0,0);
    }
    myTransform->Transform(1, &myPtX, &myPtY, NULL);
    wxRealPoint myOutPt (myPtX, myPtY);
    OCTDestroyCoordinateTransformation(myTransform);
    return myOutPt;
}


tmCoordConvert::tmCoordConvert(PRJDEF_PROJ_TYPE projtype) {
    m_ProjType = projtype;
}



tmCoordConvert::~tmCoordConvert() {
}



wxRealPoint tmCoordConvert::GetPointWGS(const wxRealPoint & in) {
    if (m_ProjType == PROJ_WORLDWGS84) {
        return in;
    }
    
    OGRSpatialReference * myInRef = _CreateSpatialRef(m_ProjType);
    wxASSERT(myInRef);
    OGRSpatialReference * myOutRef = _CreateSpatialRef(PROJ_WORLDWGS84);
    wxASSERT(myOutRef);
    return _Transform(myInRef, myOutRef, in);
}



wxRealPoint tmCoordConvert::GetPointGoogle(const wxRealPoint & in) {
    return _Transform(_CreateSpatialRef(m_ProjType), _CreateSpatialRefGoogle(), in);
}



double tmCoordConvert::GetDistance (const wxRealPoint & p1, const wxRealPoint & p2 ){
    struct geod_geodesic myGeodesic;
    geod_init(&myGeodesic, m_Geod_a, m_Geod_f);
    
    wxRealPoint p1wgs = GetPointWGS(p1);
    wxRealPoint p2wgs = GetPointWGS(p2);
    
    double dist = 0;
    double azi1 = 0;
    double azi2 = 0;
    geod_inverse(&myGeodesic, p1wgs.y, p1wgs.x, p2wgs.y, p2wgs.x, &dist, &azi1, &azi2);
    
    return dist;
}


wxRealPoint tmCoordConvert::_GetPointLocalFromWGS( const wxRealPoint & pt){
    if (m_ProjType == PROJ_WORLDWGS84) {
        return pt;
    }
    
    OGRSpatialReference * myInRef = _CreateSpatialRef(PROJ_WORLDWGS84);
    wxASSERT(myInRef);
    OGRSpatialReference * myOutRef = _CreateSpatialRef(m_ProjType);
    wxASSERT(myOutRef);
    return _Transform(myInRef, myOutRef, pt);
}



wxRealPoint tmCoordConvert::GetPointAtDistance (const wxRealPoint & p1, double distance, double azimut){
    struct geod_geodesic myGeodesic;
    geod_init(&myGeodesic, m_Geod_a, m_Geod_f);
    
    wxRealPoint p1wgs = GetPointWGS(p1);
    wxRealPoint pDestWgs;
    
    geod_direct(&myGeodesic, p1wgs.y, p1wgs.x , azimut, distance, &pDestWgs.y, &pDestWgs.x, 0);
    return _GetPointLocalFromWGS(pDestWgs);
}



wxString tmCoordConvert::GetDistanceHuman (double distanceM){
    if (distanceM < 1.0) {
        return wxString::Format(_T("%f [cm]"), distanceM * 100);
    }
    
    if ( (distanceM / 1000.0) < 1.0) {
        return wxString::Format(_T("%f [m]"), distanceM);
    }
    
    return wxString::Format(_T("%f [km]"), distanceM / 1000.0);
}




