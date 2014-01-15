/***************************************************************************
								tmtocctrlmenu.cpp
                    Display contextual menu for item in the TOC Ctrl
                             -------------------
    copyright            : (C) 2007 CREALP Lucien Schreiber 
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

// comment doxygen

#include "tmtocctrlmenu.h"
#include "tmlayerproperties.h"
#include "../gui/tmwebframe.h"


 

tmTOCCtrlMenu::tmTOCCtrlMenu(tmLayerProperties * item, int pos, int numberitems) :
wxMenu(item->GetNameDisplay(), 0){
    m_LayerProperties = item;
	m_SelectedPos = pos;
	m_TotalLayers = numberitems;
	
	// create menu
	_CreateTOCBasic();
	_CreateTOCMoveMenu();
	_CreateTOCShowVertex();
	_CreateTOCProperties();
}



tmTOCCtrlMenu::~tmTOCCtrlMenu(){
	
}




void tmTOCCtrlMenu::_CreateTOCBasic (){
	if (m_LayerProperties->GetType() > TOC_NAME_NOT_GENERIC){
		Append(ID_TOCMENU_REMOVE, _("Remove layer"));
	}
	else {
		AppendCheckItem(ID_TOCMENU_EDIT_LAYER, _("Edit layer"));
        Check(ID_TOCMENU_EDIT_LAYER, m_LayerProperties->IsEditing());
	}
    
    if (m_LayerProperties->GetType() == TOC_NAME_WEB) {
        AppendCheckItem (ID_TOCMENU_SHOW_WEBFRAME, _("Show Web Frame"));
        Check(ID_TOCMENU_SHOW_WEBFRAME, m_LayerProperties->GetWebFrameRef()->IsVisible());
    }
}



void tmTOCCtrlMenu::_CreateTOCShowVertex (){
	int myflags = (tmDRAWING_FLAGS) m_LayerProperties->GetVertexFlags();
    switch (m_LayerProperties->GetSpatialType())
	{
		case LAYER_SPATIAL_LINE:
		case LAYER_SPATIAL_POLYGON:
		{
			wxMenu * menushow = new wxMenu();			
			menushow->AppendCheckItem(ID_TOCMENU_SHOW_VERTEX_NONE, _("None"));			
			menushow->AppendCheckItem(ID_TOCMENU_SHOW_VERTEX_ALL, _("All"));
			menushow->AppendCheckItem(ID_TOCMENU_SHOW_VERTEX_BEGIN_END, _("Begin/End"));
			menushow->Check(ID_TOCMENU_SHOW_VERTEX_NONE + myflags, true);
			Append(wxID_ANY, _("Show vertex"), menushow);
			break;
		}	
		case LAYER_SPATIAL_RASTER:
		case LAYER_SPATIAL_POINT:
		{
			// do nothing :-) nice no !
			break;
		}
			
		default:
			wxLogError(_T("Unknown spattype = %d, case note supported"), m_LayerProperties->GetSpatialType());
			break;
	}
}



void tmTOCCtrlMenu::_CreateTOCProperties (){
    if (m_LayerProperties->GetType() == TOC_NAME_SHP) {
        Append(ID_TOCMENU_LABELS, _("Labels..."));
    }
	Append(ID_TOCMENU_PROPERTIES, _("Symbology..."));
}



/***************************************************************************//**
 @brief Create "move" menu
 @details Append to the contextual menu entry for moving layers up, down, to the
 top and to the bottom.
 @author Lucien Schreiber (c) CREALP 2008
 @date 27 October 2008
 *******************************************************************************/
void tmTOCCtrlMenu::_CreateTOCMoveMenu ()
{
	wxMenu * menumove = new wxMenu();
	menumove->Append(ID_TOCMENU_MOVE_TOP, _("To the top\tCtrl+Home"));
	menumove->Append(ID_TOCMENU_MOVE_UP, _("Up\tCtrl+PgUp"));
	menumove->Append(ID_TOCMENU_MOVE_DOWN, _("Down\tCtrl+PgDn"));
	menumove->Append(ID_TOCMENU_MOVE_BOTTOM, _("To the bottom\tCtrl+End"));
	
	// gray menu based on position and number of layers
	if (m_SelectedPos == 0){
		menumove->Enable(ID_TOCMENU_MOVE_TOP, false);
		menumove->Enable(ID_TOCMENU_MOVE_UP, false);
	}
	
	if (m_SelectedPos == m_TotalLayers -1){
		menumove->Enable(ID_TOCMENU_MOVE_DOWN, false);
		menumove->Enable(ID_TOCMENU_MOVE_BOTTOM, false);
	}
	
	Append(wxID_ANY, _("Move layer"), menumove);
}


