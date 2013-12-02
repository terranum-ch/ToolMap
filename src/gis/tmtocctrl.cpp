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
#include "tmsymbol.h"				// for symbology
#include "tmlayerproperties.h"
#include "../gui/tmwebframe.h"




DEFINE_EVENT_TYPE(tmEVT_LM_REMOVE)
DEFINE_EVENT_TYPE(tmEVT_LM_ADD)
DEFINE_EVENT_TYPE(tmEVT_LM_UPDATE)
DEFINE_EVENT_TYPE(tmEVT_LM_SHOW_PROPERTIES)
DEFINE_EVENT_TYPE(tmEVT_EM_EDIT_START)
DEFINE_EVENT_TYPE(tmEVT_EM_EDIT_STOP)
DEFINE_EVENT_TYPE(tmEVT_TOC_SELECTION_CHANGED)



/******************************* TOC CONTROL *************************************/

BEGIN_EVENT_TABLE(tmTOCCtrl, wxTreeCtrl)
	EVT_LEFT_DOWN(tmTOCCtrl::OnMouseClick)
	EVT_TREE_ITEM_ACTIVATED(wxID_ANY, tmTOCCtrl::OnMouseItemDoubleClick)
	EVT_TREE_SEL_CHANGED(wxID_ANY, tmTOCCtrl::OnLayerSelected)
	//EVT_TREE_ITEM_RIGHT_CLICK(wxID_ANY, tmTOCCtrl::OnMouseItemRightClick)
	EVT_TREE_ITEM_MENU(wxID_ANY,tmTOCCtrl::OnMouseItemRightClick)
	EVT_MENU (ID_TOCMENU_REMOVE, tmTOCCtrl::OnRemoveItem)
	EVT_MENU (ID_TOCMENU_PROPERTIES, tmTOCCtrl::OnShowProperties)
	EVT_MENU (ID_TOCMENU_SHOW_VERTEX_NONE,tmTOCCtrl::OnVertexMenu)
	EVT_MENU (ID_TOCMENU_SHOW_VERTEX_ALL,tmTOCCtrl::OnVertexMenu)
	EVT_MENU (ID_TOCMENU_SHOW_VERTEX_BEGIN_END,tmTOCCtrl::OnVertexMenu)
	EVT_MENU (ID_TOCMENU_MOVE_TOP, tmTOCCtrl::OnMoveLayers)
	EVT_MENU (ID_TOCMENU_MOVE_UP, tmTOCCtrl::OnMoveLayers)
	EVT_MENU (ID_TOCMENU_MOVE_DOWN, tmTOCCtrl::OnMoveLayers)
	EVT_MENU (ID_TOCMENU_MOVE_BOTTOM, tmTOCCtrl::OnMoveLayers)
	EVT_MENU (ID_TOCMENU_EDIT_LAYER, tmTOCCtrl::OnEditingChange)
    EVT_MENU (ID_TOCMENU_SHOW_WEBFRAME, tmTOCCtrl::OnShowWebFrame)
    EVT_PAINT(tmTOCCtrl::OnPaint)
	EVT_KEY_DOWN (tmTOCCtrl::OnShortcutKey)
	EVT_TREE_BEGIN_DRAG(wxID_ANY, tmTOCCtrl::OnDragStart)
	EVT_TREE_END_DRAG (wxID_ANY, tmTOCCtrl::OnDragStop)
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
	m_EditingLayer = NULL;
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
	wxBitmap myUncheckedbmp (16,16);
    wxBitmap myCheckedbmp (16,16);
    m_IsImageInited = false;
    images->Add(myUncheckedbmp);
    images->Add(myCheckedbmp);
    AssignImageList(images);
}



// used mainly to init listbitmap
void tmTOCCtrl::OnPaint(wxPaintEvent & event){
    //wxPaintDC myUnusedPaintDC (this);
    
    if (m_IsImageInited == false) { 
        
        // unchecked
        wxBitmap myTempBmp (16,16);
        wxMemoryDC myDC;
        myDC.SelectObject(myTempBmp);
        myDC.SetBackground(*wxTheBrushList->FindOrCreateBrush(GetBackgroundColour(), wxSOLID));
        myDC.Clear();
        wxRendererNative::Get().DrawCheckBox(this, myDC, wxRect(0, 0, 16, 16), 0);
        
        // checked
        wxBitmap myTempBmp2 (16,16);
        myDC.SelectObject(myTempBmp2);
        myDC.SetBackground(*wxTheBrushList->FindOrCreateBrush(GetBackgroundColour(), wxSOLID));
        myDC.Clear();
        wxRendererNative::Get().DrawCheckBox(this, myDC, wxRect(0, 0, 16, 16), wxCONTROL_CHECKED);
        myDC.SelectObject(wxNullBitmap);
       
        GetImageList()->Replace(0, myTempBmp);
        GetImageList()->Replace(1, myTempBmp2);
        m_IsImageInited = true;
    }

    event.Skip();
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
}


tmTOCCtrl::~tmTOCCtrl()
{
	wxDELETE(m_ContextMenu);	
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
	
	wxString myDisplayName = item->GetNameDisplay();
	
	wxTreeItemId itemid;
	// appending item
	if (!position.IsOk())
	{
		itemid = AppendItem(m_root, myDisplayName, item->IsVisible(), -1, item);
	}
	else
	{
		// inserting item
		itemid = InsertItem(m_root, position, myDisplayName, item->IsVisible(), -1, item);
		wxLogDebug(_T("Inserting item"));
	}
	
	// setting item style
	SetItemStyle(itemid, item);
	
	return TRUE;
}



bool tmTOCCtrl::UpdateLayerName (tmLayerProperties * item, const wxString & newname){
    if (!m_root.IsOk()){
		wxLogError(_T("Root item not defined, define it first"));
		return false;
	}

    wxTreeItemId myItemID;
    myItemID = GetLastChild(m_root);
    while (1) {
        if (myItemID.IsOk() == false) {
            return false;
        }
        
        if (GetItemData(myItemID) == item) {
            SetItemText(myItemID, newname);
            break;
        }
        myItemID = GetPrevSibling (myItemID);
    }
    return true;
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
	if(GetItemParent(position) != m_root || position == m_root)
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
	
	SetItemText(position, newitemdata->GetNameDisplay());
	SetItemImage(position, newitemdata->IsVisible());
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



bool tmTOCCtrl::GetItemByID (wxTreeItemId & position,long searchedid){
	
	bool bStart = true;
	while (1){
		tmLayerProperties * myProp = IterateLayers(bStart);
		bStart = false;
		if (myProp == NULL) {
			break;
		}
		
		if (myProp->GetID() == searchedid) {
			position = m_ActualItemID;
			return true;
		}
	}
	return false;
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
	wxFont myFont = wxFont(*wxNORMAL_FONT);
	
	// change style to bold if generic layer
	if (item->GetType() < TOC_NAME_NOT_GENERIC)
	{
		myFont.SetWeight(wxFONTWEIGHT_BOLD);
		
		//SetItemBold(id, TRUE);
	}
	
	if (item == GetEditLayer())
	{
		myFont.SetUnderlined(true);
	}
	
	SetItemFont(id, myFont);
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
	wxTreeItemId selected = GetSelection();
	if (selected.IsOk()==false){
		return NULL;
	}

	tmLayerProperties * itemprop = (tmLayerProperties*)	GetItemData(selected);
	if (!itemprop)
		return NULL;
	
	return itemprop;
}



/***************************************************************************//**
 @brief Get a layer by ID
 @details we are talking about the CONTENT_ID column
 @param layerid The layer id (see CONTENT_ID column)
 @return a valid tmLayerProperties object or null if
 nothing found (do not delete, belong to the TOC)
 @author Lucien Schreiber (c) CREALP 2009
 @date 30 January 2009
 *******************************************************************************/
tmLayerProperties * tmTOCCtrl::GetLayerById (long layerid)
{
	bool bReset = true;
	tmLayerProperties * myReturnedLayer = NULL;
	
	for (unsigned int i = 0; i<GetCountLayers();i++)
	{
		tmLayerProperties * myIteratedLayer = IterateLayers(bReset);
		if (myIteratedLayer && myIteratedLayer->GetID() == layerid){
			myReturnedLayer = myIteratedLayer;
            break;
        }
		bReset = false;
	}
	return myReturnedLayer;
}



tmLayerProperties * tmTOCCtrl::GetLayerByName (const wxString & layername){
    bool bReset = true;
	tmLayerProperties * myReturnedLayer = NULL;
	for (unsigned int i = 0; i<GetCountLayers();i++)
	{
		tmLayerProperties * myIteratedLayer = IterateLayers(bReset);
		if (myIteratedLayer && myIteratedLayer->GetName() == layername){
			myReturnedLayer = myIteratedLayer;
            break;
        }
		bReset = false;
	}
	return myReturnedLayer;
}



/***************************************************************************//**
 @brief Select a item
 @details Based on the layer ID (MySQL column CONTENT_ID, in prj_toc).
 @param layerID Layer ID
 @author Lucien Schreiber (c) CREALP 2008
 @date 11 November 2008
 *******************************************************************************/
void tmTOCCtrl::SetSelectedLayer (int layerID)
{
	tmLayerProperties * itemprop = NULL;
	bool bFirstLoop = true;
	
	Freeze();
	UnselectAll();
	while (1)
	{
		itemprop = IterateLayers(bFirstLoop);
		bFirstLoop = false;
		if (!itemprop){
			break;
		}
		
		if (itemprop->GetID() == layerID){
			SelectItem(m_ActualItemID, true);
		}
	
	}
	Thaw();
	
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
	
	wxTreeItemId selected = GetSelection();
	wxTreeItemIdValue cookie;
	
	int iTotalLayers = GetCountLayers();
	if (iTotalLayers == 0){
		wxLogDebug(_T("Not able to count layers or layers count = 0"));
		return -1;
	}
	
	
	wxTreeItemId child; 
	for (int i = 0; i < iTotalLayers; i++){
		if (i == 0)
			child = GetFirstChild(m_root, cookie);
		else
			child = GetNextChild(m_root, cookie);
		
		if (child == selected)
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
	wxFont item1_font = GetItemFont(item);
	int item1_image = GetItemImage(item, wxTreeItemIcon_Normal);
	wxString item1_text = GetItemText(item);
 	tmLayerProperties * item1_data = (tmLayerProperties*) GetItemData(item);
	tmLayerProperties * newinserteditemData = new tmLayerProperties (*item1_data);
	
	// inserting item to the new position
	Freeze();
	wxTreeItemId newinserteditem = InsertItem(m_root,
											  newpos, 
											  item1_text,
											  item1_image,
											  -1,
											  newinserteditemData);
	SetItemBold(newinserteditem, item1_bold);
	SetItemFont(newinserteditem, item1_font);
	SelectItem(newinserteditem, true);
	
	item1_data->SetWebFrame(NULL, wxID_ANY, wxSize(0,0)); // avoid frame deleting
	Delete(item);
	Thaw();
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
	wxFont item1_font = GetItemFont(item);
	
	
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
	wxFont item2_font = GetItemFont(item2);
	
	// swapping items
	Freeze();
	SetItemBold(item, item2_bold);
	SetItemImage(item, item2_image);
	SetItemText(item, item2_text);
	SetItemData(item, item2_data);
	SetItemFont(item, item2_font);
	//SelectItem(item, false);
	
	
	SetItemBold(item2, item1_bold);
	SetItemImage(item2, item1_image);
	SetItemText(item2, item1_text);
	SetItemData(item2, item1_data);
	SetItemFont(item2, item1_font);
	SelectItem(item2, true);
	Thaw();

	
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
		if(itemdata->IsVisible() == true){
            itemdata->SetVisible(false);
        }
        else {
            itemdata->SetVisible(true);
        }
				
		// change item image
		EditLayer(itemdata, clickedid);
		
		// Send message show/hide to layermanager
		wxCommandEvent evt(tmEVT_LM_UPDATE, wxID_ANY);
		evt.SetInt((int) itemdata->IsVisible());
		GetEventHandler()->AddPendingEvent(evt);
		return;
	}
	
	/*if (clickedid.IsOk()== true){
		SelectItem(clickedid);
	}*/
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
	if(itemid.IsOk()==false){
		return;
	}

	// select item where right click occur
	Freeze();
	UnselectAll();
	SelectItem(itemid,true);
	Thaw();

	if (GetItemParent(itemid) != m_root){
		wxLogDebug(_T("No menu for this item : Maybe the parent item ?"));
		return;
	}
	
	wxDELETE(m_ContextMenu);
	m_ContextMenu = new tmTOCCtrlMenu((tmLayerProperties*)GetItemData(itemid), GetSelectedPosition(), GetCountLayers());
	PopupMenu(m_ContextMenu, event.GetPoint());
	event.Skip();
}


/***************************************************************************//**
 @brief Called when user double-click an item
 @details Actually display the show properties dialog
 @author Lucien Schreiber (c) CREALP 2009
 @date 18 February 2009
 *******************************************************************************/
void tmTOCCtrl::OnMouseItemDoubleClick (wxTreeEvent & event)
{
	int flags = 0;
	//wxTreeItemId clickedid = HitTest(event.GetPoint(), flags);
    HitTest(event.GetPoint(), flags);
	if (flags & wxTREE_HITTEST_ONITEMICON){
		event.Skip();
		return;
	}

	if(event.GetItem().IsOk() == true){
		wxCommandEvent evt;
		OnShowProperties(evt);
	}
	event.Skip();
}



/***************************************************************************//**
 @brief Called when user move layers
 @author Lucien Schreiber (c) CREALP 2008
 @date 27 October 2008
 *******************************************************************************/
void tmTOCCtrl::OnMoveLayers (wxCommandEvent & event)
{
	wxLogDebug(_T("Moving layers..."));
	
	wxTreeItemId selected = GetSelection();
	
	switch (event.GetId()){
		case ID_TOCMENU_MOVE_TOP:
			MoveLayers(selected, 0);
			break;
			
		case ID_TOCMENU_MOVE_UP:
			SwapLayers(selected, GetSelectedPosition() -1);
			break;
			
		case ID_TOCMENU_MOVE_DOWN:
			SwapLayers(selected, GetSelectedPosition() + 1);
			break;
			
		default:
			MoveLayers(selected, GetCountLayers());
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
		wxTreeItemId itemid = GetSelection();
		if(itemid.IsOk()==false){
			return;
		}
		
		wxCommandEvent evt (wxEVT_COMMAND_MENU_SELECTED);
		int eventid = wxID_ANY;
		switch (event.GetKeyCode()){
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
		
		if (eventid != wxID_ANY)
		{
			evt.SetId(eventid);
			GetEventHandler()->AddPendingEvent(evt);
			return; // do not propagate event.
		}
		
	}
	event.Skip();
}


/***************************************************************************//**
 @brief Called when layer selection change
 @author Lucien Schreiber (c) CREALP 2009
 @date 07 April 2009
 *******************************************************************************/
void tmTOCCtrl::OnLayerSelected (wxTreeEvent & event)
{
	wxCommandEvent evt(tmEVT_TOC_SELECTION_CHANGED, wxID_ANY);
	GetEventHandler()->AddPendingEvent(evt);
	
	event.Skip();
}



/***************************************************************************//**
 @brief Called when Editing is started or stoped
 @author Lucien Schreiber (c) CREALP 2009
 @date 02 February 2009
 *******************************************************************************/
void tmTOCCtrl::OnEditingChange (wxCommandEvent & event)
{
	if (event.IsChecked())
		StartEditing();
	else
		StopEditing(true);
}



void tmTOCCtrl::OnShowWebFrame (wxCommandEvent & event){
	wxTreeItemId itemid = GetSelection();
    if(itemid.IsOk()==false){
        return;
    }
    
    tmLayerProperties * myLayerProp = static_cast<tmLayerProperties *>(GetItemData(itemid));
    if (myLayerProp == NULL) {
        wxLogError(_("No data assigned to this item!"));
        return;
    }
    
    bool isVisible = myLayerProp->GetWebFrameRef()->IsVisible();
    if (isVisible == true) {
        myLayerProp->GetWebFrameRef()->Hide();
    }
    else {
        myLayerProp->GetWebFrameRef()->Show();
    }
}



void tmTOCCtrl::OnDragStart(wxTreeEvent & event){
	m_DragItemID = wxTreeItemId();
	wxASSERT(m_DragItemID.IsOk() == false);
	
	if ( event.GetItem() != GetRootItem() ){
		event.Allow();
		m_DragItemID = event.GetItem();
	}
	else {
		wxLogWarning("This can't be dragged!");
	}
}



void tmTOCCtrl::OnDragStop(wxTreeEvent & event){
	wxLogMessage("Dragging stopped!");
	
	wxTreeItemId myItemStart = m_DragItemID;
	wxTreeItemId myItemStop = event.GetItem();
	
	m_DragItemID = wxTreeItemId();
	wxASSERT(m_DragItemID.IsOk() == false);
	
	if (myItemStop == myItemStart) {
		return;
	}
	
	if (myItemStop.IsOk()) {
		int myItemStartPos = wxNOT_FOUND;
		int myItemStopPos = wxNOT_FOUND;
		
		wxASSERT(m_root.IsOk());
		wxTreeItemIdValue myCookie;
		wxTreeItemId myFirstLayer = GetFirstChild(m_root, myCookie);
		wxASSERT(myFirstLayer.IsOk());
		if (myFirstLayer == myItemStart) {
			myItemStartPos = 0;
		}
		if (myFirstLayer == myItemStop) {
			myItemStopPos = 0;
		}
		
		int myIterPosition = 1;
		while (1) {
			wxTreeItemId myIterLayer = GetNextChild(m_root, myCookie);
			if (myIterLayer.IsOk() == false) {
				break;
			}
			if (myIterLayer == myItemStart) {
				myItemStartPos = myIterPosition;
			}
			if (myIterLayer == myItemStop) {
				myItemStopPos = myIterPosition;
			}
			myIterPosition++;
		}
		
		wxLogMessage("Item %d moved to %d", myItemStartPos, myItemStopPos);
		
		// move items
		if (abs(myItemStopPos - myItemStartPos) == 1) {
			SwapLayers(myItemStart, myItemStopPos);
		}
		else {
			if (myItemStopPos == 0) {
				MoveLayers(myItemStart, 0);
			}
			else {
				MoveLayers(myItemStart, myItemStopPos+1);
			}
		}
		
		// update display
		wxCommandEvent evt(tmEVT_LM_UPDATE, wxID_ANY);
		GetEventHandler()->AddPendingEvent(evt);
	}
}



/***************************************************************************//**
 @brief Called when editing is started
 @details Display a visual indication when a layer is in editing mode
 @author Lucien Schreiber (c) CREALP 2009
 @date 02 February 2009
 *******************************************************************************/
void tmTOCCtrl::StartEditing ()
{
	// stop editing if needed
	if (GetEditLayer())
		StopEditing(); // no message sent
	
	// get selected item
	tmLayerProperties * item = GetSelectionLayer();
	item->SetEditing(true);
	wxASSERT(item);
	SetEditLayer(item);
	
	
	//SwitchVisualEditingStyle(selection.Item(0), true);
	SetItemStyle(GetSelection(), item);
	
	m_ContextMenu->Check(ID_TOCMENU_EDIT_LAYER, true);
	
	// sent message
	wxCommandEvent evt(tmEVT_EM_EDIT_START, wxID_ANY);
	GetEventHandler()->AddPendingEvent(evt);
	
}


/***************************************************************************//**
 @brief Called when editing is stoped
 @details Display a visual indication when a layer is in editing mode
 @param bSentmessage if set to true, we sent a message indicating the end of 
 the editing process
 @author Lucien Schreiber (c) CREALP 2009
 @date 02 February 2009
 *******************************************************************************/
void tmTOCCtrl::StopEditing (bool bSentmessage)
{
	// get selected item
	bool bReset = true;
	tmLayerProperties * iterlayer = NULL;
	while (1)
	{
		iterlayer = IterateLayers(bReset);
		bReset = false;
		if (!iterlayer)
		{
			break;
		}
		
		
		if (iterlayer == GetEditLayer())
		{
			iterlayer->SetEditing(false);
			SetEditLayer(NULL);
			SetItemStyle(m_ActualItemID, 
						 (tmLayerProperties*) GetItemData(m_ActualItemID));
			break;
		}
	}
	
	if (bSentmessage)
	{
		wxCommandEvent evt(tmEVT_EM_EDIT_STOP, wxID_ANY);
		GetEventHandler()->AddPendingEvent(evt);
	}

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
	wxTreeItemId selected = GetSelection();

	// check for item != root
	wxTreeItemId myID = selected;
	if (myID == GetRootItem()){
		wxLogError(_("Properties not availlable for project, select a layer."));
		return;
	}

	tmLayerProperties * item = (tmLayerProperties*) GetItemData(selected);

	wxASSERT(item->GetSymbolRef());
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
	tmLayerProperties * item = GetSelectionLayer();
	wxASSERT(item);
	
	// keep old value to avoid drawing if value not changed
	int oldflags = item->GetVertexFlags();
	
	switch (event.GetId())
	{
		case ID_TOCMENU_SHOW_VERTEX_ALL:
			item->SetVertexFlags(tmDRAW_VERTEX_ALL);
			break;
		case ID_TOCMENU_SHOW_VERTEX_BEGIN_END:
			item->SetVertexFlags(tmDRAW_VERTEX_BEGIN_END);
			break;
		default:
			item->SetVertexFlags(tmDRAW_VERTEX_NONE);
			break;
	}
	
	if (item->GetVertexFlags() != oldflags)
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
	wxTreeItemId selected = GetSelection();
	if(selected.IsOk() == false || selected == m_root){
		wxLogMessage(_("No layer selected, or unable to delete selected layer"));
		return;
	}
	
	// not able to remove generic layers
	tmLayerProperties * item = (tmLayerProperties*) GetItemData(selected);
	wxASSERT(item);

	if (item->GetType() < TOC_NAME_NOT_GENERIC)
	{
		wxLogMessage(_("Not allowed to remove generic layers from project"));
		return;
	}
	
	wxCommandEvent evt(tmEVT_LM_REMOVE, wxID_ANY);
	evt.SetExtraLong(item->GetID());
	UnselectAll();


	if (RemoveLayer(selected, TRUE))
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

