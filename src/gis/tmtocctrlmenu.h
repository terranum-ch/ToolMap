/***************************************************************************
 tmtocctrlmenu.h
 Display contextual menu for item in the TOC Ctrl
 -------------------
 copyright : (C) 2007 CREALP Lucien Schreiber
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
#ifndef _TM_TOCCTRLMENU_H_
#define _TM_TOCCTRLMENU_H_

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/menu.h>

const int ID_TOCMENU_REMOVE = 20100;
const int ID_TOCMENU_SHOW_VERTEX_NONE = 20101;
const int ID_TOCMENU_SHOW_VERTEX_ALL = 20102;
const int ID_TOCMENU_SHOW_VERTEX_BEGIN_END = 20103;
const int ID_TOCMENU_PROPERTIES = 20104;
const int ID_TOCMENU_PROPERTIES_SAVE = 20105;
const int ID_TOCMENU_PROPERTIES_LOAD = 20106;
const int ID_TOCMENU_MOVE_TOP = 20107;
const int ID_TOCMENU_MOVE_UP = 20108;
const int ID_TOCMENU_MOVE_DOWN = 20109;
const int ID_TOCMENU_MOVE_BOTTOM = 20110;
const int ID_TOCMENU_EDIT_LAYER = 20111;
const int ID_TOCMENU_LABELS = 20112;

class tmLayerProperties;

/***************************************************************************//**
 @brief Display contextual menu for TOC
 @details Contextual menu is created mainly based on the spatial type ( See
 #TM_GIS_SPATIAL_TYPES).
 @note All events are processed in the tmTOCCtrl Class
 @author Lucien Schreiber (c) CREALP 2008
 @date 11 August 2008
 *******************************************************************************/
class tmTOCCtrlMenu : public wxMenu
{
private:
    int m_SelectedPos;
    int m_TotalLayers;
    tmLayerProperties *m_LayerProperties;

    void _CreateTOCBasic();

    void _CreateTOCShowVertex();

    void _CreateTOCProperties();

    void _CreateTOCMoveMenu();

public:
    tmTOCCtrlMenu()
    { ; }

    tmTOCCtrlMenu(tmLayerProperties *item, int pos, int numberitems);

    ~tmTOCCtrlMenu();
};


#endif
