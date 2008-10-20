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


 

tmTOCCtrlMenu::tmTOCCtrlMenu(tmLayerProperties * item) :
	wxMenu(item->GetDisplayName(), 0)
{
	m_flags = (tmDRAWING_FLAGS) item->m_DrawFlags;
	m_spattypes = item->m_LayerSpatialType;
	if (item->m_LayerType < TOC_NAME_NOT_GENERIC)
		m_Generic = true;
	else
		m_Generic = false;

	
	// create menu based on item spatial type
	CreateTOCContextMenu();
	
}



tmTOCCtrlMenu::~tmTOCCtrlMenu()
{
	
}



void tmTOCCtrlMenu::CreateTOCContextMenu()
{
	CreateTOCBasic(); // REMOVE ITEM
	CreateTOCShowVertex(); // SHOW VERTEX (only if needed)
	CreateTOCProperties();	// menu properties
	
	
}


void tmTOCCtrlMenu::CreateTOCBasic ()
{
	if (!m_Generic)
	{
		Append(ID_TOCMENU_REMOVE, _("Remove layer"));
	}
}



void tmTOCCtrlMenu::CreateTOCShowVertex ()
{
	switch (m_spattypes)
	{
		case LAYER_SPATIAL_LINE:
		case LAYER_SPATIAL_POLYGON:
		{
			wxMenu * menushow = new wxMenu();			
			menushow->AppendCheckItem(ID_TOCMENU_SHOW_VERTEX_NONE, _("None"));			
			menushow->AppendCheckItem(ID_TOCMENU_SHOW_VERTEX_ALL, _("All"));
			menushow->AppendCheckItem(ID_TOCMENU_SHOW_VERTEX_BEGIN_END, _("Begin/End"));
			menushow->Check(ID_TOCMENU_SHOW_VERTEX_NONE + m_flags,
							true);
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
			wxLogDebug(_T("Unknown spattype = %d, case note supported"), m_spattypes);
			break;
	}
}



void tmTOCCtrlMenu::CreateTOCProperties ()
{
	Append(ID_TOCMENU_PROPERTIES, _("Properties..."));
	
}






