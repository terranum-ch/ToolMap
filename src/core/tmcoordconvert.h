/***************************************************************************
 tmcoordconvert
 -------------------
 copyright            : (C) 2013 Lucien Schreiber
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef _TMCOORDCONVERT_H_
#define _TMCOORDCONVERT_H_

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <gdalwarper.h>
#include <ogr_spatialref.h>
#include <wx/stdpaths.h>

#include "../components/geodesic/geodesic.h"
#include "projectdefmemory.h"

class tmRealRect;

class tmCoordConvert {
  private:
    PRJDEF_PROJ_TYPE m_ProjType;

    OGRSpatialReference* _CreateSpatialRef(PRJDEF_PROJ_TYPE proj);

    OGRSpatialReference* _CreateSpatialRefGoogle();

    wxRealPoint _Transform(OGRSpatialReference* refin, OGRSpatialReference* refout, const wxRealPoint& in);

    wxRealPoint _GetPointLocalFromWGS(const wxRealPoint& pt);

  public:
    tmCoordConvert(PRJDEF_PROJ_TYPE projtype);

    virtual ~tmCoordConvert();

    wxRealPoint GetPointWGS(const wxRealPoint& in);

    wxRealPoint GetPointGoogle(const wxRealPoint& in);

    wxBitmap* GetProjectGoogleRaster(wxBitmap* web_raster, tmRealRect* coord_web, tmRealRect* coord_local);

    double GetDistance(const wxRealPoint& p1, const wxRealPoint& p2);

    wxRealPoint GetPointAtDistance(const wxRealPoint& p1, double distance, double azimut);

    wxString GetDistanceHuman(double distanceM);

    char* GetWKTProjectionGoogle();

    char* GetWKTProjectionLocal();

    char* GetESRIWKTProjectionLocal();
};

#endif
