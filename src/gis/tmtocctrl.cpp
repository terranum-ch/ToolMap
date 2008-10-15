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


DEFINE_EVENT_TYPE(tmEVT_LM_REMOVE)
DEFINE_EVENT_TYPE(tmEVT_LM_ADD)
DEFINE_EVENT_TYPE(tmEVT_LM_SHOW_HIDE)




/******************************* TOC CONTROL *************************************/

BEGIN_EVENT_TABLE(tmTOCCtrl, wxTreeCtrl)
	EVT_LEFT_DOWN(tmTOCCtrl::OnMouseClick)
	EVT_TREE_ITEM_RIGHT_CLICK(wxID_ANY, tmTOCCtrl::OnMouseItemRightClick)
	EVT_MENU (ID_TOCMENU_REMOVE, tmTOCCtrl::OnRemoveItem)
	EVT_MENU (ID_TOCMENU_PROPERTIES, tmTOCCtrl::OnShowProperties)
END_EVENT_TABLE()


/***************************************************************************//**
 @brief Init member variables
 @author Lucien Schreiber (c) CREALP 2008
 @date 10 July 2008
 *******************************************************************************/
void tmTOCCtrl::InitMemberValues()
{
	m_ParentEvt = NULL;
	m_ContextMenu = NULL;
	m_ActualItemID = 0;
}



/***************************************************************************//**
 @brief Load the image list
 @details This loads the image list (checked, unchecked, ...) and assign the
 list to the tree ctrl (#tmTOCCtrl)
 @author Lucien Schreiber (c) CREALP 2008
 @date 10 July 2008
 *******************************************************************************/
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
	m_ParentEvt = parent;
	//m_ParentEvt->PushEventHandler(this)
}


tmTOCCtrl::~tmTOCCtrl()
{
	if(m_ContextMenu)
		delete m_ContextMenu;
	
}


/***************************************************************************//**
 @brief Add the first tree root (project name)
 @note If the first root exists, nothing is done and a Debug message is
 issued in debug mode
 @author Lucien Schreiber (c) CREALP 2008
 @date 10 July 2008
 *******************************************************************************/
void tmTOCCtrl::InsertProjectName (const wxString & prjname)
{
	if (m_root.IsOk())
	{
		wxLogDebug(_T("Root allready defined, not able to init root twice"));
		return;
	}
	
	m_root = AddRoot(prjname);
}



/***************************************************************************//**
 @brief Append or insert a layer entry
 @param item pointer to data to attach to every tree item
 @param position item position
 @return  true if item added or inserted sucessfully
 @author Lucien Schreiber (c) CREALP 2008
 @date 10 July 2008
 *******************************************************************************/
bool tmTOCCtrl::InsertLayer(tmLayerProperties * item, wxTreeItemId position)
{
	wxASSERT_MSG(item, _T("Error adding empty item to TOC array"));

	
	if (!m_root.IsOk())
	{
		wxLogDebug(_T("Root item not defined, define it first"));
		return FALSE;
	}
	
	wxString myDisplayName = item->GetDisplayName();
	
	wxTreeItemId itemid;
	// appending item
	if (!position.IsOk())
	{
		itemid = AppendItem(m_root, myDisplayName, item->m_LayerVisible, -1, item);
	}
	else
	{
		// inserting item
		itemid = InsertItem(m_root, position, myDisplayName, item->m_LayerVisible, -1, item);
		wxLogDebug(_T("Inserting item"));
	}
	
	// setting item style
	SetItemStyle(itemid, item);
	
	return TRUE;
}


/***************************************************************************//**
 @brief Delete item (and all its children)
 @param position the item we want to delete
 @param bRemoveChild TRUE if we want to delete all item's children (default)
 @return  TRUE if item deleted and FALSE if the position dosen't exists
 @note Only first level items are allowed to be deleted (verification is done
 internally)
 @author Lucien Schreiber (c) CREALP 2008
 @date 10 July 2008
 *******************************************************************************/
bool tmTOCCtrl::RemoveLayer (wxTreeItemId position, bool bRemoveChild)
{
	if (!position.IsOk())
	{
		wxLogDebug(_T("Tree position error, position unknown"));
		return FALSE;
	}
	
	// delete only if first level item
	if(!GetItemParent(position) == m_root || position == m_root)
	{
		wxLogDebug(_T("Only layers could be deleted, don't select legend or project name"));
		return FALSE;
		
	}
	// delete children first if required
	if (bRemoveChild)
		DeleteChildren(position);
	Delete(position);
	
	return TRUE;
}



/***************************************************************************//**
 @brief Modify an existing layer entry
 @param newitemdata tmLayerProperties object with new values to modify
 @param position the position of item
 @return TRUE if item was modified, FALSE if the item dosen't exists
 @author Lucien Schreiber (c) CREALP 2008
 @date 10 July 2008
 *******************************************************************************/
bool tmTOCCtrl::EditLayer (tmLayerProperties * newitemdata, wxTreeItemId position)
{
	// check item exists
	if (!position.IsOk())
	{
		wxLogDebug(_T("Position dosen't exist, unable to modify item"));
		return FALSE;
	}
	
	SetItemText(position, newitemdata->GetDisplayName());
	SetItemImage(position, newitemdata->m_LayerVisible);
	SetItemStyle(position, newitemdata);
	
	return TRUE;
}



/***************************************************************************//**
 @brief Iterate through all layers in the TOC
 @details Call this function recursively until there is no more layers to
 iterate. Use ResetToLast = TRUE during the first iteration and then set this value
 to false. To display layers in the correct order, we iterate starting from the end
 (see figure bellow).
 \image html iterate_orders.png
 @param ResetToLast TRUE for reseting iteration to the End and false to 
 continue iterating things up
 @return  a valid #tmLayerProperties or NULL if there is no more item to iterate
 @author Lucien Schreiber (c) CREALP 2008
 @date 11 July 2008
 *******************************************************************************/
tmLayerProperties * tmTOCCtrl::IterateLayers (bool ResetToLast)
{
	// check
	wxASSERT_MSG(m_root.IsOk(), _T("m_root not ok, error"));

	
	// if we are starting from the End
	if(ResetToLast)
	{
		m_ActualItemID = GetLastChild(m_root);
	}
	else 
	{
		m_ActualItemID = GetPrevSibling (m_ActualItemID);
	}
	
	if (!m_ActualItemID.IsOk())
		return NULL;
	
	return (tmLayerProperties*) GetItemData(m_ActualItemID);
}



/***************************************************************************//**
 @brief Remove all layers from tree ctrl
 @author Lucien Schreiber (c) CREALP 2008
 @date 10 July 2008
 *******************************************************************************/
void tmTOCCtrl::ClearAllLayers()
{
	// Delete all items and clear the root
	DeleteAllItems();
	m_root = 0;
}



/***************************************************************************//**
 @brief Set the layer style
 @details Change the style used for displaying the layer in the TOC
 @param id position of the item
 @param item value of the item is used for changing style, for exemple generic
 layers are displayed in bold
 @author Lucien Schreiber (c) CREALP 2008
 @date 10 July 2008
 *******************************************************************************/
void tmTOCCtrl::SetItemStyle (wxTreeItemId id, tmLayerProperties * item)
{
	// change style to bold if generic layer
	if (item->m_LayerType < TOC_NAME_NOT_GENERIC)
	{
		SetItemBold(id, TRUE);
	}
}



/***************************************************************************//**
 @brief Count the number of layers
 @todo Check that this function works correctly with sub-childs (like
 layer types or legend).
 @author Lucien Schreiber (c) CREALP 2008
 @date 10 July 2008
 *******************************************************************************/
unsigned int tmTOCCtrl::GetCountLayers()
{
	if (m_root.IsOk())
		return GetChildrenCount(m_root, FALSE);
	return 0;
}



/***************************************************************************//**
 @brief Called when mouse is clicked
 @details Change the picture value (checked, unchecked) if mouse click occur
 inside picture
 @author Lucien Schreiber (c) CREALP 2008
 @date 10 July 2008
 *******************************************************************************/
void tmTOCCtrl::OnMouseClick (wxMouseEvent & event)
{
	wxTreeItemId clickedid = 0;
	int flags = 0;
	
	clickedid = HitTest(event.GetPosition(), flags);
	if (flags & wxTREE_HITTEST_ONITEMICON)
	{
		tmLayerProperties * itemdata = (tmLayerProperties*) GetItemData(clickedid);
		itemdata->m_LayerVisible == TRUE ? itemdata->m_LayerVisible = FALSE : itemdata->m_LayerVisible = TRUE; 
				
		// change item image
		EditLayer(itemdata, clickedid);
		
		// Send message show/hide to layermanager
		wxCommandEvent evt(tmEVT_LM_SHOW_HIDE, wxID_ANY);
		evt.SetInt((int) itemdata->m_LayerVisible);
		GetEventHandler()->AddPendingEvent(evt);
	}
	
	event.Skip();
}



/***************************************************************************//**
 @brief Called when right mouse is clicked on item
 @details Check that some first level item (layers) where clicked and then call
 contextual menu
 @author Lucien Schreiber (c) CREALP 2008
 @date 08 August 2008
 *******************************************************************************/
void tmTOCCtrl::OnMouseItemRightClick (wxTreeEvent & event)
{
	wxTreeItemId itemid = event.GetItem();
	if (GetItemParent(itemid) != m_root)
	{
		wxLogDebug(_T("No menu for this item : Maybe the parent item ?"));
		return;
	}
	
	// only one item selected in the TOC ctrl otherwise no contextual menu
	wxArrayTreeItemIds selection;
	if (GetSelections(selection) != 1)
	{
		wxLogDebug(_T("No contextual menu allowed for more than one item"));
		return;
	}
	

	if(m_ContextMenu)
		delete m_ContextMenu;
	m_ContextMenu = new tmTOCCtrlMenu((tmLayerProperties*)GetItemData(itemid));
	PopupMenu(m_ContextMenu, event.GetPoint());
}



/***************************************************************************//**
 @brief Called when contextual menu "Properties" is pressed
 @details All events from tmTOCCtrlMenu are processed in tmTOCCtrl
 @author Lucien Schreiber (c) CREALP 2008
 @date 11 August 2008
 *******************************************************************************/
void tmTOCCtrl::OnShowProperties (wxCommandEvent & event)
{
	// get selected item
	wxArrayTreeItemIds selection;
	GetSelections(selection);
	tmLayerProperties * item = (tmLayerProperties*) GetItemData(selection.Item(0));
	
	wxASSERT(item->m_LayerSymbol);
	if(item->m_LayerSymbol->ShowSymbologyDialog(GetParent(),
												wxGetMousePosition())==wxID_OK)
	{
		// send event to the layer manager for updating display
		wxCommandEvent evt(tmEVT_LM_SHOW_HIDE, wxID_ANY);
		GetEventHandler()->AddPendingEvent(evt);
	}
}




/***************************************************************************//**
 @brief Called when user try to remove a layer
 @note Verifications are done internally to ensure that a valid layers
 was selected and that we don't try to remove a generic layer.
 @author Lucien Schreiber (c) CREALP 2008
 @date 10 July 2008
 *******************************************************************************/
void tmTOCCtrl::OnRemoveItem (wxCommandEvent & event)
{
	wxArrayTreeItemIds selectedarray;
	unsigned int iNbSelectedItems = 0;
	
	iNbSelectedItems = GetSelections(selectedarray);
	UnselectAll();
	
	// only one item actually, the first item from array
	if(iNbSelectedItems == 0)
	{
		wxLogMessage(_("No layer selected, select a layer first"));
		return;
	}
	
	// not able to remove generic layers
	tmLayerProperties * item = (tmLayerProperties*) GetItemData(selectedarray.Item(0));
	if (item->m_LayerType < TOC_NAME_NOT_GENERIC)
	{
		wxLogMessage(_("Not allowed to remove generic layers from project"));
		return;
	}
	
	wxCommandEvent evt(tmEVT_LM_REMOVE, wxID_ANY);
	evt.SetExtraLong(item->m_LayerID);
	
	// only delete the first selected layer 
	if (RemoveLayer(selectedarray.Item(0), TRUE))
	{
		GetEventHandler()->AddPendingEvent(evt);
	}
}



/***************************************************************************//**
 @brief Checks if the TOC is ready
 @details This function may be used to know if the TOC was correctely started
 and ready to be filled with others layers. Following checks are made :
 - Root item exists
 - Number of layers is not less than 4 (generic layers)
 @return  TRUE if both tests above pass, FALSE otherwise. In debug mode messages
 are sent for explaining problem
 @author Lucien Schreiber (c) CREALP 2008
 @date 11 July 2008
 *******************************************************************************/
bool tmTOCCtrl::IsTOCReady()
{
	if(!m_root.IsOk())
	{
		//wxLogDebug(_T("No root item defined, project no open ?"));
		return FALSE;
	}
	
	if(GetCountLayers() < 4)
	{
		wxLogDebug(_T("Not enought layers found. Is this a ToolMap project ?"));
		return FALSE;
	}
	return TRUE;
}

