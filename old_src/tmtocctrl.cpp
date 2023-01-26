/***************************************************************************
 tmtocctrl.cpp
 Contain description of the GIS toc ctrl
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

#include "tmtocctrl.h"

#include <wx/textfile.h>

#include "tmlayerproperties.h"
#include "tmsymbol.h"  // for symbology
#include "tmsymbolrule.h"

DEFINE_EVENT_TYPE(tmEVT_LM_REMOVE)
DEFINE_EVENT_TYPE(tmEVT_LM_ADD)
DEFINE_EVENT_TYPE(tmEVT_LM_UPDATE)
DEFINE_EVENT_TYPE(tmEVT_LM_SHOW_PROPERTIES)
DEFINE_EVENT_TYPE(tmEVT_LM_SHOW_LABELS)
DEFINE_EVENT_TYPE(tmEVT_LM_TOC_EDITED)
DEFINE_EVENT_TYPE(tmEVT_EM_EDIT_START)
DEFINE_EVENT_TYPE(tmEVT_EM_EDIT_STOP)
DEFINE_EVENT_TYPE(tmEVT_TOC_SELECTION_CHANGED)

/******************************* TOC CONTROL *************************************/

BEGIN_EVENT_TABLE(tmTOCCtrl, wxTreeCtrl)
EVT_LEFT_DOWN(tmTOCCtrl::OnMouseClick)
EVT_TREE_ITEM_ACTIVATED(wxID_ANY, tmTOCCtrl::OnMouseItemDoubleClick)
EVT_TREE_SEL_CHANGED(wxID_ANY, tmTOCCtrl::OnLayerSelected)
// EVT_TREE_ITEM_RIGHT_CLICK(wxID_ANY, tmTOCCtrl::OnMouseItemRightClick)
EVT_TREE_ITEM_MENU(wxID_ANY, tmTOCCtrl::OnMouseItemRightClick)
EVT_MENU(ID_TOCMENU_REMOVE, tmTOCCtrl::OnRemoveItem)
EVT_MENU(ID_TOCMENU_PROPERTIES, tmTOCCtrl::OnShowProperties)
EVT_MENU(ID_TOCMENU_PROPERTIES_SAVE, tmTOCCtrl::OnPropertiesSave)
EVT_MENU(ID_TOCMENU_PROPERTIES_LOAD, tmTOCCtrl::OnPropertiesLoad)
EVT_MENU(ID_TOCMENU_SHOW_VERTEX_NONE, tmTOCCtrl::OnVertexMenu)
EVT_MENU(ID_TOCMENU_SHOW_VERTEX_ALL, tmTOCCtrl::OnVertexMenu)
EVT_MENU(ID_TOCMENU_SHOW_VERTEX_BEGIN_END, tmTOCCtrl::OnVertexMenu)
EVT_MENU(ID_TOCMENU_MOVE_TOP, tmTOCCtrl::OnMoveLayers)
EVT_MENU(ID_TOCMENU_MOVE_UP, tmTOCCtrl::OnMoveLayers)
EVT_MENU(ID_TOCMENU_MOVE_DOWN, tmTOCCtrl::OnMoveLayers)
EVT_MENU(ID_TOCMENU_MOVE_BOTTOM, tmTOCCtrl::OnMoveLayers)
EVT_MENU(ID_TOCMENU_EDIT_LAYER, tmTOCCtrl::OnEditingChange)
EVT_MENU(ID_TOCMENU_LABELS, tmTOCCtrl::OnShowLabels)
EVT_PAINT(tmTOCCtrl::OnPaint)
EVT_KEY_DOWN(tmTOCCtrl::OnShortcutKey)
EVT_TREE_BEGIN_DRAG(wxID_ANY, tmTOCCtrl::OnDragStart)
EVT_TREE_END_DRAG(wxID_ANY, tmTOCCtrl::OnDragStop)
END_EVENT_TABLE()

/***************************************************************************/ /**
  @brief Init member variables
  @author Lucien Schreiber (c) CREALP 2008
  @date 10 July 2008
  *******************************************************************************/
void tmTOCCtrl::InitTocMemberValues() {
  m_ParentEvt = nullptr;
  m_ContextMenu = nullptr;
  m_ActualItemID = 0;
  m_EditingLayer = nullptr;
}




bool tmTOCCtrl::GetItemByID(wxTreeItemId &position, long searchedid) {
  bool bStart = true;
  while (1) {
    tmLayerProperties *myProp = IterateLayers(bStart);
    bStart = false;
    if (myProp == nullptr) {
      break;
    }

    if (myProp->GetID() == searchedid) {
      position = m_ActualItemID;
      return true;
    }
  }
  return false;
}

/***************************************************************************/ /**
  @brief Remove all layers from tree ctrl
  @author Lucien Schreiber (c) CREALP 2008
  @date 10 July 2008
  *******************************************************************************/
void tmTOCCtrl::ClearAllLayers() {
  // Delete all items and clear the root
  DeleteAllItems();
  m_root = nullptr;
}

/***************************************************************************/ /**
  @brief Set the layer style
  @details Change the style used for displaying the layer in the TOC
  @param id position of the item
  @param item value of the item is used for changing style, for exemple generic
  layers are displayed in bold
  @author Lucien Schreiber (c) CREALP 2008
  @date 10 July 2008
  *******************************************************************************/
void tmTOCCtrl::SetItemStyle(wxTreeItemId id, tmLayerProperties *item) {
  wxFont myFont = wxFont(*wxNORMAL_FONT);

  // change style to bold if generic layer
  if (item->GetType() < TOC_NAME_NOT_GENERIC) {
    myFont.SetWeight(wxFONTWEIGHT_BOLD);

    // SetItemBold(id, TRUE);
  }

  if (item == GetEditLayer()) {
    myFont.SetUnderlined(true);
  }

  SetItemFont(id, myFont);
}



/***************************************************************************/ /**
  @brief Select a item
  @details Based on the layer ID (MySQL column CONTENT_ID, in prj_toc).
  @param layerID Layer ID
  @author Lucien Schreiber (c) CREALP 2008
  @date 11 November 2008
  *******************************************************************************/
void tmTOCCtrl::SetSelectedLayer(int layerID) {
  tmLayerProperties *itemprop = nullptr;
  bool bFirstLoop = true;

  Freeze();
  UnselectAll();
  while (1) {
    itemprop = IterateLayers(bFirstLoop);
    bFirstLoop = false;
    if (!itemprop) {
      break;
    }

    if (itemprop->GetID() == layerID) {
      SelectItem(m_ActualItemID, true);
    }
  }
  Thaw();
}

/***************************************************************************/ /**
  @brief Get the selected layers position
  @details Count the number of previous layers before main layers. This is mainly
  used for graying items in contextual menu (move menu)
  @return  The layer's position, -1 in case of error
  @author Lucien Schreiber (c) CREALP 2008
  @date 27 October 2008
  *******************************************************************************/
int tmTOCCtrl::GetSelectedPosition() {
  if (!IsTOCReady()) {
    wxLogDebug(_T("TOC isn't ready..."));
    return -1;
  }

  wxTreeItemId selected = GetSelection();
  wxTreeItemIdValue cookie;

  int iTotalLayers = GetCountLayers();
  if (iTotalLayers == 0) {
    wxLogDebug(_T("Not able to count layers or layers count = 0"));
    return -1;
  }

  wxTreeItemId child;
  for (int i = 0; i < iTotalLayers; i++) {
    if (i == 0)
      child = GetFirstChild(m_root, cookie);
    else
      child = GetNextChild(m_root, cookie);

    if (child == selected) return i;
  }
  wxLogDebug(_T("Getting selected position error"));
  return -1;
}



/***************************************************************************/ /**
  @brief Called when right mouse is clicked on item
  @details Check that some first level item (layers) where clicked and then call
  contextual menu
  @author Lucien Schreiber (c) CREALP 2008
  @date 08 August 2008
  *******************************************************************************/
void tmTOCCtrl::OnMouseItemRightClick(wxTreeEvent &event) {
  wxTreeItemId itemid = event.GetItem();
  if (!itemid.IsOk()) {
    return;
  }

  // select item where right click occur
  Freeze();
  UnselectAll();
  SelectItem(itemid, true);
  Thaw();

  if (GetItemParent(itemid) != m_root) {
    wxLogDebug(_T("No menu for this item : Maybe the parent item ?"));
    return;
  }

  wxDELETE(m_ContextMenu);
  m_ContextMenu = new tmTOCCtrlMenu((tmLayerProperties *)GetItemData(itemid), GetSelectedPosition(), GetCountLayers());
  PopupMenu(m_ContextMenu, event.GetPoint());
  event.Skip();
}

/***************************************************************************/ /**
  @brief Called when user double-click an item
  @details Actually display the show properties dialog
  @author Lucien Schreiber (c) CREALP 2009
  @date 18 February 2009
  *******************************************************************************/
void tmTOCCtrl::OnMouseItemDoubleClick(wxTreeEvent &event) {
  int flags = 0;
  // wxTreeItemId clickedid = HitTest(event.GetPoint(), flags);
  HitTest(event.GetPoint(), flags);
  if (flags & wxTREE_HITTEST_ONITEMICON) {
    event.Skip();
    return;
  }

  if (event.GetItem().IsOk()) {
    wxCommandEvent evt;
    OnShowProperties(evt);
  }
  event.Skip();
}


/***************************************************************************/ /**
  @brief Called when a key is pressed
  @details This function implements the keyboard shortcuts for some TOC Ctrl
  shortcuts such as Moving layers.
  @param event Contain the key pressed
  @author Lucien Schreiber (c) CREALP 2008
  @date 27 October 2008
  *******************************************************************************/
void tmTOCCtrl::OnShortcutKey(wxKeyEvent &event) {
  if (event.CmdDown() && IsTOCReady()) {
    wxTreeItemId itemid = GetSelection();
    if (!itemid.IsOk()) {
      return;
    }

    wxCommandEvent evt(wxEVT_COMMAND_MENU_SELECTED);
    int eventid = wxID_ANY;
    switch (event.GetKeyCode()) {
      case WXK_HOME:  // move layers to top
        eventid = ID_TOCMENU_MOVE_TOP;
        break;

      case WXK_PAGEUP:  // move layers up
        eventid = ID_TOCMENU_MOVE_UP;
        break;

      case WXK_PAGEDOWN:  // move layers down
        eventid = ID_TOCMENU_MOVE_DOWN;
        break;

      case WXK_END:  // move layers to the bottom
        eventid = ID_TOCMENU_MOVE_BOTTOM;
        break;
    }

    if (eventid != wxID_ANY) {
      evt.SetId(eventid);
      GetEventHandler()->QueueEvent(evt.Clone());
      return;  // do not propagate event.
    }
  }

  switch (event.GetKeyCode()) {
    case WXK_SPACE:
      return;  // do not propagate space event because it triggers the display of the symbology when we want to disable
               // snapping.
  }

  event.Skip();
}

/***************************************************************************/ /**
  @brief Called when layer selection change
  @author Lucien Schreiber (c) CREALP 2009
  @date 07 April 2009
  *******************************************************************************/
void tmTOCCtrl::OnLayerSelected(wxTreeEvent &event) {
  wxCommandEvent evt(tmEVT_TOC_SELECTION_CHANGED, wxID_ANY);
  GetEventHandler()->QueueEvent(evt.Clone());

  event.Skip();
}

/***************************************************************************/ /**
  @brief Called when Editing is started or stoped
  @author Lucien Schreiber (c) CREALP 2009
  @date 02 February 2009
  *******************************************************************************/
void tmTOCCtrl::OnEditingChange(wxCommandEvent &event) {
  if (event.IsChecked())
    StartEditing();
  else
    StopEditing(true);
}

/***************************************************************************/ /**
  @brief Called when editing is started
  @details Display a visual indication when a layer is in editing mode
  @author Lucien Schreiber (c) CREALP 2009
  @date 02 February 2009
  *******************************************************************************/
void tmTOCCtrl::StartEditing() {
  // stop editing if needed
  if (GetEditLayer()) StopEditing();  // no message sent

  // get selected item
  tmLayerProperties *item = GetSelectionLayer();
  item->SetEditing(true);
  wxASSERT(item);
  SetEditLayer(item);

  // SwitchVisualEditingStyle(selection.Item(0), true);
  SetItemStyle(GetSelection(), item);

  m_ContextMenu->Check(ID_TOCMENU_EDIT_LAYER, true);

  // sent message
  wxCommandEvent evt(tmEVT_EM_EDIT_START, wxID_ANY);
  GetEventHandler()->QueueEvent(evt.Clone());
}

/***************************************************************************/ /**
  @brief Called when editing is stoped
  @details Display a visual indication when a layer is in editing mode
  @param bSentmessage if set to true, we sent a message indicating the end of
  the editing process
  @author Lucien Schreiber (c) CREALP 2009
  @date 02 February 2009
  *******************************************************************************/
void tmTOCCtrl::StopEditing(bool bSentmessage) {
  // get selected item
  bool bReset = true;
  tmLayerProperties *iterlayer = nullptr;
  while (1) {
    iterlayer = IterateLayers(bReset);
    bReset = false;
    if (!iterlayer) {
      break;
    }

    if (iterlayer == GetEditLayer()) {
      iterlayer->SetEditing(false);
      SetEditLayer(nullptr);
      SetItemStyle(m_ActualItemID, (tmLayerProperties *)GetItemData(m_ActualItemID));
      break;
    }
  }

  if (bSentmessage) {
    wxCommandEvent evt(tmEVT_EM_EDIT_STOP, wxID_ANY);
    GetEventHandler()->QueueEvent(evt.Clone());
  }
}

/***************************************************************************/ /**
  @brief Called when contextual menu "Properties" is pressed
  @details All events from tmTOCCtrlMenu are processed in tmTOCCtrl. As this event
  requires data from GIS Layers, it is processed into #tmLayerManager
  @author Lucien Schreiber (c) CREALP 2008
  @date 11 August 2008
  *******************************************************************************/
void tmTOCCtrl::OnShowProperties(wxCommandEvent &event) {
  // get selected item
  wxTreeItemId selected = GetSelection();

  // check for item != root
  wxTreeItemId myID = selected;
  if (myID == GetRootItem()) {
    wxLogError(_("Properties not available for project, select a layer."));
    return;
  }

  tmLayerProperties *item = (tmLayerProperties *)GetItemData(selected);

  wxASSERT(item->GetSymbolRef());
  wxCommandEvent Evt(tmEVT_LM_SHOW_PROPERTIES, wxID_ANY);
  Evt.SetClientData(item);
  GetEventHandler()->QueueEvent(Evt.Clone());
}

void tmTOCCtrl::OnPropertiesSave(wxCommandEvent &event) {
  // get selected item
  wxTreeItemId selected = GetSelection();
  wxTreeItemId myID = selected;
  if (myID == GetRootItem()) {
    wxLogError(_("Saving Properties not available for project, select a layer."));
    return;
  }

  // get layer info: name, spatial type, symbology
  tmLayerProperties *item = (tmLayerProperties *)GetItemData(selected);
  wxFileName myLayerName = item->GetName();
  wxString mySpatialType;
  mySpatialType << item->GetSpatialType();
  tmSerialize out;
  item->GetSymbolRuleManagerRef()->Serialize(out);

  myLayerName.SetExt(_T("tly"));

  // show save dialog
  wxString mySaveFilePathTxt =
      wxSaveFileSelector(_("Save symbology into"), myLayerName.GetExt(), myLayerName.GetFullName(), this);

  if (mySaveFilePathTxt == wxEmptyString) {
    return;
  }

  wxFile myFile(mySaveFilePathTxt, wxFile::write);
  if (!myFile.IsOpened()) {
    wxLogError(_("Error creating file: '%s'"), mySaveFilePathTxt);
    return;
  }

  myFile.Write(myLayerName.GetName() << _T("\n"));
  myFile.Write(mySpatialType << _T("\n"));
  myFile.Write(out.GetString());
  myFile.Close();
}

void tmTOCCtrl::OnPropertiesLoad(wxCommandEvent &event) {
  // get selected item
  wxTreeItemId selected = GetSelection();
  wxTreeItemId myID = selected;
  if (myID == GetRootItem()) {
    wxLogError(_("Loading Properties not available for project, select a layer."));
    return;
  }

  // show load dialog
  wxString myLoadFilePathTxt = wxLoadFileSelector(_("Load symbology"), _T("tly"), wxEmptyString, this);

  if (myLoadFilePathTxt == wxEmptyString) {
    return;
  }

  // try to load the symbology file
  wxTextFile myFile;
  if (!myFile.Open(myLoadFilePathTxt)) {
    wxLogError(_("Unable to open symbology file: %s"), myLoadFilePathTxt);
    return;
  }

  // load selected layer information
  tmLayerProperties *item = (tmLayerProperties *)GetItemData(selected);
  wxFileName myLayerName = item->GetName();
  wxString mySpatialType;
  mySpatialType << item->GetSpatialType();

  // check layer name stored
  wxString myLayerNameInTxtFile = myFile.GetFirstLine();
  if (myLayerNameInTxtFile != myLayerName.GetName()) {
    wxMessageDialog myDlg(
        this,
        wxString::Format(_T("This symbology was saved for layer: %s. Apply for selected layer?"), myLayerNameInTxtFile),
        _T("Different layer names"), wxYES_NO | wxCANCEL);
    if (myDlg.ShowModal() != wxID_YES) {
      return;
    }
  }

  // check geometry stored
  wxString myFileSpatialType = myFile.GetNextLine();
  if (myFileSpatialType != mySpatialType) {
    wxMessageDialog myDlg(this,
                          wxString::Format(_T("This symbology was saved for '%s'. Apply for '%s'?"),
                                           TM_GIS_SPATIAL_TYPES_STRING[wxAtoi(myFileSpatialType)],
                                           TM_GIS_SPATIAL_TYPES_STRING[wxAtoi(mySpatialType)]),
                          _T("Different spatial types"), wxYES_NO | wxCANCEL);
    if (myDlg.ShowModal() != wxID_YES) {
      return;
    }
  }

  // load serialisation
  wxString mySymbologyTxt = myFile.GetNextLine();
  if (mySymbologyTxt.IsEmpty()) {
    wxLogError(_("No symbology of empty symbology found in: %s"), myLoadFilePathTxt);
    return;
  }

  // escaping character are only needed for storing in the database.
  mySymbologyTxt.Replace(_T("\\\""), _T("\""));
  tmSerialize in(mySymbologyTxt);
  item->GetSymbolRuleManagerRef()->Serialize(in);

  // update display
  wxCommandEvent evt(tmEVT_LM_UPDATE, wxID_ANY);
  GetEventHandler()->QueueEvent(evt.Clone());

  // save new status
  wxCommandEvent evtSave(tmEVT_LM_TOC_EDITED, wxID_ANY);
  GetEventHandler()->QueueEvent(evtSave.Clone());
}

void tmTOCCtrl::OnShowLabels(wxCommandEvent &event) {
  // get selected item
  wxTreeItemId selected = GetSelection();
  tmLayerProperties *item = (tmLayerProperties *)GetItemData(selected);
  if (item == nullptr) {
    return;
  }
  wxASSERT(item->GetSymbolRef());
  wxCommandEvent Evt(tmEVT_LM_SHOW_LABELS, wxID_ANY);
  Evt.SetClientData(item);
  GetEventHandler()->QueueEvent(Evt.Clone());
}

/***************************************************************************/ /**
  @brief Called when contextual menu "Vertex" is pressed
  @author Lucien Schreiber (c) CREALP 2008
  @date 16 October 2008
  *******************************************************************************/
void tmTOCCtrl::OnVertexMenu(wxCommandEvent &event) {
  tmLayerProperties *item = GetSelectionLayer();
  wxASSERT(item);

  // keep old value to avoid drawing if value not changed
  int oldflags = item->GetVertexFlags();

  switch (event.GetId()) {
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

  if (item->GetVertexFlags() != oldflags) {
    // send event to the layer manager for updating display
    wxCommandEvent evt(tmEVT_LM_UPDATE, wxID_ANY);
    GetEventHandler()->QueueEvent(evt.Clone());
    // save new status
    wxCommandEvent evtSave(tmEVT_LM_TOC_EDITED, wxID_ANY);
    GetEventHandler()->QueueEvent(evtSave.Clone());
  }
}

/***************************************************************************/ /**
  @brief Called when user try to remove a layer
  @note Verifications are done internally to ensure that a valid layers
  was selected and that we don't try to remove a generic layer.
  @author Lucien Schreiber (c) CREALP 2008
  @date 10 July 2008
  *******************************************************************************/
void tmTOCCtrl::OnRemoveItem(wxCommandEvent &event) {
  wxTreeItemId selected = GetSelection();
  if (!selected.IsOk() || selected == m_root) {
    wxLogMessage(_("No layer selected, or unable to delete selected layer"));
    return;
  }

  // not able to remove generic layers
  tmLayerProperties *item = (tmLayerProperties *)GetItemData(selected);
  wxASSERT(item);

  if (item->GetType() < TOC_NAME_NOT_GENERIC) {
    wxLogMessage(_("Not allowed to remove generic layers from project"));
    return;
  }

  wxCommandEvent evt(tmEVT_LM_REMOVE, wxID_ANY);
  evt.SetExtraLong(item->GetID());
  UnselectAll();

  if (RemoveLayer(selected, TRUE)) {
    GetEventHandler()->QueueEvent(evt.Clone());
  }
}

/***************************************************************************/ /**
  @brief Checks if the TOC is ready
  @details This function may be used to know if the TOC was correctely started
  and ready to be filled with misc layers. Following checks are made :
  - Root item exists
  - Number of layers is not less than 4 (generic layers)
  @return  TRUE if both tests above pass, FALSE otherwise. In debug mode messages
  are sent for explaining problem
  @author Lucien Schreiber (c) CREALP 2008
  @date 11 July 2008
  *******************************************************************************/
bool tmTOCCtrl::IsTOCReady() {
  if (!m_root.IsOk()) {
    // wxLogDebug(_T("No root item defined, project no open ?"));
    return FALSE;
  }

  if (GetCountLayers() < 4) {
    wxLogDebug(_T("Not enought layers found. Is this a ToolMap project ?"));
    return FALSE;
  }
  return TRUE;
}

/***************************************************************************/ /**
  @brief Expand all layers
  @details Call this function for expanding all layers once the root layers is
  defined
  @return  true if expanding was allowed (root item defined), false otherwise
  @author Lucien Schreiber (c) CREALP 2008
  @date 27 October 2008
  *******************************************************************************/
bool tmTOCCtrl::ExpandAllLayers() {
  if (IsTOCReady()) {
    ExpandAllChildren(m_root);
    return true;
  }
  return false;
}
