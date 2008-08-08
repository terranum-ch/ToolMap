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
	bool bIsGeneric = FALSE;
	if (item->m_LayerType < TOC_NAME_NOT_GENERIC)
		bIsGeneric =TRUE;
	
	// create menu based on item spatial type
	CreateTOCContextMenu(item->m_LayerSpatialType, bIsGeneric);
	
}



tmTOCCtrlMenu::~tmTOCCtrlMenu()
{
	
}


void tmTOCCtrlMenu::CreateTOCContextMenu(PRJDEF_LAYERS_TYPE spattype, bool bIsGeneric)
{
	CreateTOCBasic(bIsGeneric); // REMOVE ITEM
	CreateTOCShowVertex(spattype); // SHOW VERTEX (only if needed)
	CreateTOCProperties();	// menu properties
	
	
}





void tmTOCCtrlMenu::CreateTOCBasic (bool bIsGeneric)
{
	if (!bIsGeneric)
	{
		Append(ID_TOCMENU_REMOVE, _("Remove layer"));
		//AppendSeparator();
	}
}


void tmTOCCtrlMenu::CreateTOCShowVertex (PRJDEF_LAYERS_TYPE spattype)
{
	switch (spattype) {
		case LAYER_LINE:
		case LAYER_POLYGON:
		{
			wxMenu * menushow = new wxMenu();
			menushow->AppendRadioItem(ID_TOCMENU_SHOW_VERTEX_BEGIN_END, _("Begin/End"));
			menushow->AppendRadioItem(ID_TOCMENU_SHOW_VERTEX_ALL, _("All"));
			menushow->AppendRadioItem(ID_TOCMENU_SHOW_VERTEX_NONE, _("None"));
			Append(wxID_ANY, _("Show vertex"), menushow);
			//AppendSeparator();
			break;
		}	
		case LAYER_RASTER:
		case LAYER_POINT:
		{
			// do nothing :-) nice no !
			break;
		}
			
		default:
			wxLogDebug(_T("Unknown spattype = %d, case note supported"), spattype);
			break;
	}
}


void tmTOCCtrlMenu::CreateTOCProperties ()
{
	Append(ID_TOCMENU_PROPERTIES, _("Properties..."));
	
}




