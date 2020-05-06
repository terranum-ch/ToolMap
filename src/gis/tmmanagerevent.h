/***************************************************************************
 tmmanagerevent.h
 Definition of event for GIS manager (layer, attribution)
 -------------------
 copyright : (C) 2007 CREALP Lucien Schreiber
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

// comment doxygen

#ifndef _TM_MANAGER_EVENT_H_
#define _TM_MANAGER_EVENT_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

// Include wxWidgets' headers
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

DECLARE_EVENT_TYPE(tmEVT_SELECTION_DONE, -1)

DECLARE_EVENT_TYPE(tmEVT_QUERY_RUN, -1);

DECLARE_EVENT_TYPE(tmEVT_TOGGLE_FREQUENT, -1)

DECLARE_EVENT_TYPE(tmEVT_ENABLE_ACCELERATORS, -1)
DECLARE_EVENT_TYPE(tmEVT_DISABLE_ACCELERATORS, -1)

// DEFINE_EVENT_TYPE (tmEVT_SHORTCUT_REFRESH);
DECLARE_EVENT_TYPE(tmEVT_SHORTCUT_REFRESH, -1);

// DECLARE_EVENT_TYPE (tmEVT_VIEW_REFRESHED, -1);
DECLARE_EVENT_TYPE(tmEVT_AM_COPY_ATTRIBUTION, -1);

DECLARE_EVENT_TYPE(tmEVT_LM_ANGLE_CHANGED, -1);
DECLARE_EVENT_TYPE(tmEVT_FOCUS_RENDERER, -1);

DECLARE_EVENT_TYPE(tmEVT_LM_MOVE_TO_FEATURE, -1);
DECLARE_EVENT_TYPE(tmEVT_LM_ZOOM_TO_FEATURE, -1);

#endif
