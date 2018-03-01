/***************************************************************************
 tmstatsevents.h
 Definition of Statistics related events 
 -------------------
 copyright            : (C) 2011 CREALP Lucien Schreiber 
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

#ifndef _TMSTATS_EVENT_H
#define _TMSTATS_EVENT_H


#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif


DECLARE_EVENT_TYPE(tmEVT_STAT_CLICK, -1);
DECLARE_EVENT_TYPE(tmEVT_STAT_ATTRIB, -1);
DECLARE_EVENT_TYPE(tmEVT_STAT_INTERSECTION, -1);


#endif
