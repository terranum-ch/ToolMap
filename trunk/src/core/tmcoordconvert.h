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
#ifndef _TMCOORDCONVERT_H_
#define _TMCOORDCONVERT_H_

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <ogr_spatialref.h>
#include "projectdefmemory.h"


class tmCoordConvert {
private:
    static wxString m_ProjTextGoogle;
    static wxString m_ProjTextSwiss;
    static wxString m_ProjTextWGS84;
    
    PRJDEF_PROJ_TYPE m_ProjType;
    
    OGRSpatialReference * _CreateSpatialRef(PRJDEF_PROJ_TYPE proj);
    OGRSpatialReference * _CreateSpatialRefGoogle();
    wxRealPoint _Transform ( OGRSpatialReference * refin, OGRSpatialReference * refout, const wxRealPoint & in);
    
public:
    tmCoordConvert(PRJDEF_PROJ_TYPE projtype);
    virtual ~tmCoordConvert();
    
    wxRealPoint GetPointWGS(const wxRealPoint & in);
    wxRealPoint GetPointGoogle(const wxRealPoint & in);
    
};
#endif
