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


// Tree ctrl images (checked, unchecked)
#include "../img/tmimgfunc.h"	// for image processing
#include "../img/img_tree_unchecked.cpp"
#include "../img/img_tree_checked.cpp"



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

BEGIN_EVENT_TABLE(tmTOCCtrl, wxTreeCtrl)
	EVT_LEFT_DOWN(tmTOCCtrl::OnMouseClick)
END_EVENT_TABLE()



void tmTOCCtrl::InitMemberValues()
{
	
	
}


void tmTOCCtrl::LoadImageList()
{
	 wxImageList *images = new wxImageList(16, 16, true);
	
	wxBitmap img_treectrl1 (wxGetBitmapFromMemory(unchecked_tree_sml));
	images->Add(img_treectrl1);
	wxBitmap img_treectrl2 (wxGetBitmapFromMemory(checked_tree_sml));
	images->Add(img_treectrl2);
	
	AssignImageList(images);

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
	LoadImageList();
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



bool tmTOCCtrl::InsertLayer(tmLayerProperties * item, wxTreeItemId position)
{
	wxASSERT_MSG(item, _T("Error adding empty item to TOC array"));

	
	if (!m_root.IsOk())
	{
		wxLogDebug(_T("Root item not defined, define it first"));
		return FALSE;
	}
	
	wxTreeItemId itemid;
	// appending item
	if (!position.IsOk())
	{
		itemid = AppendItem(m_root, item->m_LayerNameExt, item->m_LayerVisible, -1, item);
		wxLogDebug(_T("Adding item"));
	}
	else
	{
		// inserting item
		itemid = InsertItem(m_root, position, item->m_LayerNameExt, item->m_LayerVisible, -1, item);
		wxLogDebug(_T("Inserting item"));
	}
	
	// setting item style
	SetItemStyle(itemid, item);
	
	return TRUE;
}



bool tmTOCCtrl::EditLayer (tmLayerProperties * newitemdata, wxTreeItemId position)
{
	// check item exists
	if (!position.IsOk())
	{
		wxLogDebug(_T("Position dosen't exist, unable to modify item"));
		return FALSE;
	}
	
	SetItemText(position, newitemdata->m_LayerNameExt);
	SetItemImage(position, newitemdata->m_LayerVisible);
	SetItemStyle(position, newitemdata);
	
	return TRUE;
}




void tmTOCCtrl::ClearAllLayers()
{
	// Delete all items and clear the root
	DeleteAllItems();
	m_root = 0;
	
	
}



void tmTOCCtrl::SetItemStyle (wxTreeItemId id, tmLayerProperties * item)
{
	// change style to bold if generic layer
	if (item->m_LayerIsGeneric < TOC_NAME_NOT_GENERIC)
	{
		SetItemBold(id, TRUE);
	}
}


unsigned int tmTOCCtrl::GetCountLayers()
{
	return GetChildrenCount(m_root, FALSE);
}



void tmTOCCtrl::OnMouseClick (wxMouseEvent & event)
{
	wxTreeItemId clickedid = 0;
	int flags = 0;
	// = NULL;
	
	clickedid = HitTest(event.GetPosition(), flags);
	if (flags & wxTREE_HITTEST_ONITEMICON)
	{
		tmLayerProperties * itemdata = (tmLayerProperties*) GetItemData(clickedid);
		itemdata->m_LayerVisible == TRUE ? itemdata->m_LayerVisible = FALSE : itemdata->m_LayerVisible = TRUE; 
				
		// change item image
		EditLayer(itemdata, clickedid);
	}
	
	event.Skip();
}





