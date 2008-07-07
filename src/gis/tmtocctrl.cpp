/***************************************************************************
								tmtocctrl.cpp
                    Contain description of the GIS toc ctrl
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

#include "tmtocctrl.h"

#include <wx/arrimpl.cpp>
WX_DEFINE_OBJARRAY (tmLayerPropertiesArray);

/***************************************************************************//**
 @brief Default values for tmLayerProperties
 @author Lucien Schreiber (c) CREALP 2008
 @date 07 July 2008
 *******************************************************************************/
void tmLayerProperties::InitMemberValues()
{
	m_LayerID = 0;
	m_LayerType = LAYER_LINE;
	m_LayerPathOnly = wxEmptyString;
	m_LayerNameExt = wxEmptyString;
	m_LayerVisible = TRUE;
	m_LayerIsGeneric = TOC_NAME_NOT_GENERIC;
}



bool tmLayerProperties::InitFromArray(const wxArrayString & array)
{
	long temptype = 0;
	long tempstatus = 0;
	long tempgeneric = 0;
	
	array.Item(0).ToLong(&m_LayerID);
	array.Item(1).ToLong(&temptype);
	m_LayerType = (PRJDEF_LAYERS_TYPE) temptype;
	
	m_LayerPathOnly = array.Item(2);
	m_LayerNameExt = array.Item(3);
	
	array.Item(4).ToLong(&tempstatus);
	m_LayerVisible = (bool) tempstatus;
	
	array.Item(5).ToLong(&tempgeneric);
	m_LayerIsGeneric = (int) tempgeneric;
	
	return TRUE;
}


/******************************* TOC CONTROL *************************************/


void tmTOCCtrl::InitMemberValues()
{
	
	
}




/***************************************************************************//**
 @brief Constructor
 @author Lucien Schreiber (c) CREALP 2008
 @date 07 July 2008
 *******************************************************************************/
tmTOCCtrl::tmTOCCtrl(wxWindow * parent, wxWindowID id, wxSize size, long style) : 
		wxTreeCtrl (parent, id, wxDefaultPosition, size, style)
{
	InitMemberValues();

}



void tmTOCCtrl::InsertProjectName (const wxString & prjname)
{
	if (m_root.IsOk())
	{
		wxLogDebug(_T("Root allready defined, not able to init root twice"));
		return;
	}
	
	m_root = AddRoot(prjname);
}



bool tmTOCCtrl::InsertLayer(tmLayerProperties * item, long position)
{
	wxASSERT_MSG(item, _T("Error adding empty item to TOC array"));
	
	m_TOCArray.Add(item);
	
	if (!m_root.IsOk())
	{
		wxLogDebug(_T("Root item not defined, define it first"));
		return FALSE;
	}
	
		// appending item
	if (position == -1 || position >= (signed) GetCount())
	{
		AppendItem(m_root, item->m_LayerNameExt);
	}
	else
	{
		// inserting item
		InsertItem(m_root, position, item->m_LayerNameExt);
	}
	
	return TRUE;
}




void tmTOCCtrl::ClearAllLayers()
{
	// Delete all items and clear the root
	DeleteAllItems();
	m_root = 0;
	
	m_TOCArray.Clear();
	
}



