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
DEFINE_EVENT_TYPE(tmEVT_LM_UPDATE)
DEFINE_EVENT_TYPE(tmEVT_LM_SHOW_PROPERTIES)




/******************************* TOC CONTROL *************************************/

BEGIN_EVENT_TABLE(tmTOCCtrl, wxTreeCtrl)
	EVT_LEFT_DOWN(tmTOCCtrl::OnMouseClick)
	EVT_TREE_ITEM_RIGHT_CLICK(wxID_ANY, tmTOCCtrl::OnMouseItemRightClick)
	EVT_MENU (ID_TOCMENU_REMOVE, tmTOCCtrl::OnRemoveItem)
	EVT_MENU (ID_TOCMENU_PROPERTIES, tmTOCCtrl::OnShowProperties)
	EVT_MENU (ID_TOCMENU_SHOW_VERTEX_NONE,tmTOCCtrl::OnVertexMenu)
	EVT_MENU (ID_TOCMENU_SHOW_VERTEX_ALL,tmTOCCtrl::OnVertexMenu)
	EVT_MENU (ID_TOCMENU_SHOW_VERTEX_BEGIN_END,tmTOCCtrl::OnVertexMenu)
	EVT_MENU (ID_TOCMENU_MOVE_TOP, tmTOCCtrl::OnMoveLayers)
	EVT_MENU (ID_TOCMENU_MOVE_UP, tmTOCCtrl::OnMoveLayers)
	EVT_MENU (ID_TOCMENU_MOVE_DOWN, tmTOCCtrl::OnMoveLayers)
	EVT_MENU (ID_TOCMENU_MOVE_BOTTOM, tmTOCCtrl::OnMoveLayers)
	EVT_KEY_DOWN (tmTOCCtrl::OnShortcutKey)
END_EVENT_TABLE()


/***************************************************************************//**
 @brief Init member variables
 @author Lucien Schreiber (c) CREALP 2008
 @date 10 July 2008
 *******************************************************************************/
void tmTOCCtrl::InitTocMemberValues()
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
	InitTocMemberValues();
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
	if (!m_root.IsOk())
		return NULL;
	//wxASSERT_MSG(m_root.IsOk(), _T("m_root not ok, error"));

	
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
 @brief Return the Layer ID of the first selected layer
 @details The layer ID returned by this function is the CONTENT_ID of the layers
 stored in the Database
 @return  Properties of the first selected Layer or NULL if nothing selected. Do
 not delete the pointer returned, it belong to the tmTOCCtrl
 @author Lucien Schreiber (c) CREALP 2008
 @date 29 October 2008
 *******************************************************************************/
tmLayerProperties * tmTOCCtrl::GetSelectionLayer ()
{
	wxArrayTreeItemIds selecteds;
	if (GetSelections(selecteds) < 1)
		return NULL;
	
	tmLayerProperties * itemprop = (tmLayerProperties*)	GetItemData(selecteds.Item(0));
	if (!itemprop)
		return NULL;
	
	return itemprop;
}


/***************************************************************************//**
 @brief Get the selected layers position
 @details Count the number of previous layers before main layers. This is mainly
 used for graying items in contextual menu (move menu)
 @return  The layer's position, -1 in case of error
 @author Lucien Schreiber (c) CREALP 2008
 @date 27 October 2008
 *******************************************************************************/
int tmTOCCtrl::GetSelectedPosition ()
{
	if (!IsTOCReady())
	{
		wxLogDebug(_T("TOC isn't ready..."));
		return -1;
	}
	
	wxArrayTreeItemIds mySelecteds;
	GetSelections(mySelecteds);
	
	wxTreeItemIdValue cookie;
	
	int iTotalLayers = GetCountLayers();
	if (iTotalLayers == 0)
	{
		wxLogDebug(_T("Not able to count layers or layers count = 0"));
		return -1;
	}
	
	
	wxTreeItemId child; 
	for (int i = 0; i < iTotalLayers; i++)
	{
		if (i == 0)
			child = GetFirstChild(m_root, cookie);
		else
			child = GetNextChild(m_root, cookie);
		
		if (child == mySelecteds.Item(0))
			return i;
		
	}
	wxLogDebug(_T("Getting selected position error"));
	return -1;
}



/***************************************************************************//**
 @brief Moving items between position
 @details This function may be used for moving items passed as parameter to the
 new position.
 @param item Id of the item to move
 @param newpos New position for the item
 @return  true if items was succesfully moved
 @author Lucien Schreiber (c) CREALP 2008
 @date 27 October 2008
 *******************************************************************************/
bool tmTOCCtrl::MoveLayers (const wxTreeItemId & item, int newpos)
{
	// getting data from layers 1;
	bool item1_bold = IsBold(item);
	int item1_image = GetItemImage(item, wxTreeItemIcon_Normal);
	wxString item1_text = GetItemText(item);
	tmLayerProperties * item1_data = (tmLayerProperties*) GetItemData(item);
	
	tmLayerProperties * newinserteditemData = new tmLayerProperties (*item1_data);
	
	// inserting item to the new position
	wxTreeItemId newinserteditem = InsertItem(m_root,
											  newpos, 
											  item1_text,
											  item1_image,
											  item1_image,
											  newinserteditemData);
	SetItemBold(newinserteditem, item1_bold);
	SelectItem(newinserteditem, true);
	
	Delete(item);
	
	return true;
}



/***************************************************************************//**
 @brief Swaping items between position
 @details This function may be used for swaping items passed as parameter to the
 new position.
 @param item Id of the item to swap
 @param newpos item with wich to swap
 @return bool true if swapping was successful
 @author Lucien Schreiber (c) CREALP 2008
 @date 27 October 2008
 *******************************************************************************/
bool tmTOCCtrl::SwapLayers (const wxTreeItemId & item, int newpos)
{
	// getting data from layers 1;
	bool item1_bold = IsBold(item);
	int item1_image = GetItemImage(item, wxTreeItemIcon_Normal);
	wxString item1_text = GetItemText(item);
	tmLayerProperties * item1_data = (tmLayerProperties*) GetItemData(item);
	
	
	// searching and getting data from new position
	wxTreeItemId item2;
	wxTreeItemIdValue cookie;
	
	for (int i = 0; i<= newpos; i++)
	{
		if (i==0)
			item2 = GetFirstChild(m_root, cookie);
		else
			item2 = GetNextChild(m_root, cookie);
	}
	if (!item2.IsOk())
	{
		wxLogDebug(_T("Returned item isn't valid"));
		return false;
	}
	
	bool item2_bold = IsBold(item2);
	int item2_image = GetItemImage(item2, wxTreeItemIcon_Normal);
	wxString item2_text = GetItemText(item2);
	tmLayerProperties * item2_data = (tmLayerProperties*) GetItemData(item2);
	
	// swapping items
	SetItemBold(item, item2_bold);
	SetItemImage(item, item2_image);
	SetItemText(item, item2_text);
	SetItemData(item, item2_data);
	SelectItem(item, false);
	
	SetItemBold(item2, item1_bold);
	SetItemImage(item2, item1_image);
	SetItemText(item2, item1_text);
	SetItemData(item2, item1_data);
	SelectItem(item2, true);
	
	return true;
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
		wxCommandEvent evt(tmEVT_LM_UPDATE, wxID_ANY);
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
	m_ContextMenu = new tmTOCCtrlMenu((tmLayerProperties*)GetItemData(itemid),
									  GetSelectedPosition(),
									  GetCountLayers());
	PopupMenu(m_ContextMenu, event.GetPoint());
}



/***************************************************************************//**
 @brief Called when user move layers
 @author Lucien Schreiber (c) CREALP 2008
 @date 27 October 2008
 *******************************************************************************/
void tmTOCCtrl::OnMoveLayers (wxCommandEvent & event)
{
	wxLogDebug(_T("Moving layers..."));
	
	wxArrayTreeItemIds mySelecteds;
	GetSelections(mySelecteds);
	
	
	switch (event.GetId())
	{
		case ID_TOCMENU_MOVE_TOP:
			MoveLayers(mySelecteds.Item(0), 0);
			break;
			
		case ID_TOCMENU_MOVE_UP:
			SwapLayers(mySelecteds.Item(0), GetSelectedPosition() -1);
			break;
			
		case ID_TOCMENU_MOVE_DOWN:
			SwapLayers(mySelecteds.Item(0), GetSelectedPosition() + 1);
			break;
			
		default:
			MoveLayers(mySelecteds.Item(0), GetCountLayers());
			break;
	}
	
	// update display
	wxCommandEvent evt(tmEVT_LM_UPDATE, wxID_ANY);
	GetEventHandler()->AddPendingEvent(evt);
	
}



/***************************************************************************//**
 @brief Called when a key is pressed
 @details This function implements the keyboard shortcuts for some TOC Ctrl
 shortcuts such as Moving layers.
 @param event Contain the key pressed
 @author Lucien Schreiber (c) CREALP 2008
 @date 27 October 2008
 *******************************************************************************/
void tmTOCCtrl::OnShortcutKey (wxKeyEvent & event)
{
	if (event.CmdDown() && IsTOCReady())
	{
		wxArrayTreeItemIds selected;
		wxCommandEvent evt (wxEVT_COMMAND_MENU_SELECTED);
		int eventid = wxID_ANY;
		if(GetSelections(selected) == 1)
		{
			switch (event.GetKeyCode())
			{
				case  WXK_HOME: // move layers to top
					eventid =ID_TOCMENU_MOVE_TOP;
					break;
					
				case WXK_PAGEUP: // move layers up
					eventid = ID_TOCMENU_MOVE_UP;
					break;
					
				case WXK_PAGEDOWN: // move layers down
					eventid = ID_TOCMENU_MOVE_DOWN;
					break;

				case WXK_END: // move layers to the bottom
					eventid = ID_TOCMENU_MOVE_BOTTOM;
					break;
			}
		}
		
		if (eventid != wxID_ANY)
		{
			//wxCommandEvent evt2 (, ID_TOCMENU_MOVE_UP);
			evt.SetId(eventid);
			GetEventHandler()->AddPendingEvent(evt);
			return; // do not propagate event.
		}
		
	}
	event.Skip();
}


				  
/***************************************************************************//**
 @brief Called when contextual menu "Properties" is pressed
 @details All events from tmTOCCtrlMenu are processed in tmTOCCtrl. As this event
 requires data from GIS Layers, it is processed into #tmLayerManager
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
	wxCommandEvent Evt (tmEVT_LM_SHOW_PROPERTIES, wxID_ANY);
	Evt.SetClientData(item);
	GetEventHandler()->AddPendingEvent(Evt);
}



/***************************************************************************//**
 @brief Called when contextual menu "Vertex" is pressed
 @author Lucien Schreiber (c) CREALP 2008
 @date 16 October 2008
 *******************************************************************************/
void tmTOCCtrl::OnVertexMenu (wxCommandEvent & event)
{
	// get selected item
	wxArrayTreeItemIds selection;
	GetSelections(selection);
	
	tmLayerProperties * item = (tmLayerProperties*) GetItemData(selection.Item(0));
	// keep old value to avoid drawing if value not changed
	int oldflags = item->m_DrawFlags;
	
	switch (event.GetId())
	{
		case ID_TOCMENU_SHOW_VERTEX_ALL:
			item->m_DrawFlags = tmDRAW_VERTEX_ALL;
			break;
		case ID_TOCMENU_SHOW_VERTEX_BEGIN_END:
			item->m_DrawFlags = tmDRAW_VERTEX_BEGIN_END;
			break;
		default:
			item->m_DrawFlags = tmDRAW_VERTEX_NONE;
			break;
	}
	
	if (item->m_DrawFlags != oldflags)
	{
		// send event to the layer manager for updating display
		wxCommandEvent evt(tmEVT_LM_UPDATE, wxID_ANY);
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



/***************************************************************************//**
 @brief Expand all layers
 @details Call this function for expanding all layers once the root layers is
 defined
 @return  true if expanding was allowed (root item defined), false otherwise
 @author Lucien Schreiber (c) CREALP 2008
 @date 27 October 2008
 *******************************************************************************/
bool tmTOCCtrl::ExpandAllLayers()
{
	if(IsTOCReady())
	{
		ExpandAllChildren(m_root);
		return true;
	}
	return false;
}

