/***************************************************************************
 information_dlg.h
 Display informations dialog for layers and features
 -------------------
 copyright            : (C) 2007 CREALP Lucien Schreiber
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

#include "information_dlg.h"

#include "../core/prjdefmemmanage.h"
#include "../gis/tmmanagerevent.h"
#include "../gis/tmselecteddatamemory.h"
#include "../gis/tmtocctrl.h"

//#include "../gis/tmgisdata.h"
#include <wx/clipbrd.h>
#include <wx/grid.h>

#include "../gis/tmgisdatavectormysql.h"

BEGIN_EVENT_TABLE(InformationDLG, wxDialog)
EVT_CLOSE(InformationDLG::OnCloseDlg)
END_EVENT_TABLE();

void InformationDLG::_CreateControls() {
  this->SetSizeHints(wxDefaultSize, wxDefaultSize);

  wxBoxSizer *bSizer22;
  bSizer22 = new wxBoxSizer(wxVERTICAL);

  wxNotebook *m_notebook = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0);

  wxPanel *m_panel5;
  m_panel5 = new wxPanel(m_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);

  wxBoxSizer *bSizer26;
  bSizer26 = new wxBoxSizer(wxVERTICAL);

  m_SelCtrl = new tmSelectionInfoCtrl(m_panel5, wxID_ANY, m_Selected, m_TOC);
  bSizer26->Add(m_SelCtrl, 1, wxEXPAND, 5);

  m_panel5->SetSizer(bSizer26);
  m_panel5->Layout();
  bSizer26->Fit(m_panel5);

  m_notebook->AddPage(m_panel5, _("Feature"), false);

  wxPanel *m_panel4;
  m_panel4 = new wxPanel(m_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
  wxBoxSizer *bSizer23;
  bSizer23 = new wxBoxSizer(wxVERTICAL);

  m_LayerCtrl = new tmLayerInfoCtrl(m_panel4, wxID_ANY, wxDefaultPosition, wxSize(350, 250));
  bSizer23->Add(m_LayerCtrl, 1, wxEXPAND, 5);

  m_panel4->SetSizer(bSizer23);
  m_panel4->Layout();
  bSizer23->Fit(m_panel4);
  m_notebook->AddPage(m_panel4, _("Layer"), false);

  bSizer22->Add(m_notebook, 1, wxEXPAND | wxALL, 5);

  this->SetSizer(bSizer22);
  this->Layout();
  bSizer22->Fit(this);

  this->Center(wxBOTH);

  // Otherwise ESC is hiding the window
  SetEscapeId(wxID_NONE);
}

void InformationDLG::OnCloseDlg(wxCloseEvent &event) {
  Destroy();
}

InformationDLG::InformationDLG(wxWindow *parent, tmTOCCtrl *toc, tmSelectedDataMemory *sel, wxWindowID id,
                               const wxString &title, const wxPoint &pos, const wxSize &size, long style)
    : wxDialog(parent, id, title, pos, size, style) {
  m_TOC = toc, m_Selected = sel;

  _CreateControls();
}

InformationDLG::~InformationDLG() {}

bool InformationDLG::TransferDataToWindow() {
  UpdateLayer();
  UpdateSelection();

  return true;
}

void InformationDLG::UpdateLayer() {
  wxASSERT(m_TOC);
  m_LayerCtrl->UpdateLayer(m_TOC);
}

void InformationDLG::UpdateSelection() {
  m_SelCtrl->UpdateSelection();
}

void InformationDLG::SetProject(PrjDefMemManage *project) {
  wxASSERT(m_SelCtrl);
  m_SelCtrl->SetProject(project);
}

/***************************************************************************/ /**
 Layer informations (wxHtmlWindow)
 author Lucien Schreiber (c) CREALP 2010
 date 01 mars 2010
 *******************************************************************************/
tmLayerInfoCtrl::tmLayerInfoCtrl(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size)
    : wxHtmlWindow(parent, id, pos, size) {}

tmLayerInfoCtrl::~tmLayerInfoCtrl() {}

void tmLayerInfoCtrl::UpdateLayer(tmTOCCtrl *toc) {
  tmLayerProperties *itemProp = toc->GetSelectionLayer();
  if (!itemProp) {
    wxLogDebug(_T("No layer selected, select a layer"));
    return;
  }

  tmGISData *myData = tmGISData::LoadLayer(itemProp);
  if (!myData) {
    wxLogError(_("Error loading GIS data for metadata"));
    return;
  }
  wxString myMetaData = myData->GetMetaDataAsHtml();
  SetPage(myMetaData);
}

/***************************************************************************/ /**
 Selection Control
 author Lucien Schreiber (c) CREALP 2010
 date 02 mars 2010
 *******************************************************************************/
BEGIN_EVENT_TABLE(tmSelectionInfoCtrl, wxTreeMultiCtrl)
EVT_LEFT_UP(tmSelectionInfoCtrl::OnItemLeftClick)
EVT_RIGHT_UP(tmSelectionInfoCtrl::OnItemRightClick)
EVT_MENU(ID_POPUP_ONLY_THIS_OBJ, tmSelectionInfoCtrl::OnPopupSelectionThis)
EVT_MENU(ID_POPUP_REMOVE_FROM_SEL, tmSelectionInfoCtrl::OnPopupSelectionRemove)
EVT_MENU(ID_POPUP_MOVE_TO, tmSelectionInfoCtrl::OnPopupMove)
EVT_MENU(ID_POPUP_ZOOM_TO, tmSelectionInfoCtrl::OnPopupZoom)
EVT_MENU(wxID_COPY, tmSelectionInfoCtrl::OnPopupCopy)
END_EVENT_TABLE()

tmSelectionInfoCtrl::tmSelectionInfoCtrl(wxWindow *window, wxWindowID id, tmSelectedDataMemory *sel, tmTOCCtrl *toc,
                                         const wxPoint &pos, const wxSize &size, long style)
    : wxTreeMultiCtrl(window, id, pos, size, style) {
  wxASSERT(sel);
  wxASSERT(toc);
  m_Selected = sel;
  m_Toc = toc;
  m_ClickedItemID = wxNOT_FOUND;
  m_Project = nullptr;
  // SetBackgroundColour(*wxWHITE);
  AddRoot(_("Selected features"), _("Selected features"));
}

tmSelectionInfoCtrl::~tmSelectionInfoCtrl() {}

void tmSelectionInfoCtrl::_DeleteAllInfos(const wxTreeMultiItem &dontdelete) {
  // Freeze();
  int myFirstChildCookie = 0;
  int myIterateChildCookie = 0;
  wxTreeMultiItem myParent = GetFirstChild(GetFirstRoot(), myFirstChildCookie);
  wxTreeMultiItem myChild;

  while (1) {
    if (!myParent.IsOk()) {
      break;
    }

    if (myParent != dontdelete) {
      myChild = GetFirstChild(myParent, myIterateChildCookie);
      if (myChild.IsOk()) {
        DeleteChildren(myParent);
      }

      Collapse(myParent, false);
    }
    myParent = GetNextChild(GetFirstRoot(), myFirstChildCookie);
  }
  // Thaw();
}

wxMenu *tmSelectionInfoCtrl::_CreatePopupMenu() {
  wxMenu *myMenu = new wxMenu();
  wxMenuItem *m_menuItem61;
  m_menuItem61 = new wxMenuItem(myMenu, ID_POPUP_MOVE_TO, wxString(wxT("Move to")), wxEmptyString, wxITEM_NORMAL);
  myMenu->Append(m_menuItem61);

  wxMenuItem *m_menuItem7;
  m_menuItem7 = new wxMenuItem(myMenu, ID_POPUP_ZOOM_TO, wxString(wxT("Zoom to")), wxEmptyString, wxITEM_NORMAL);
  myMenu->Append(m_menuItem7);

  myMenu->AppendSeparator();

  wxMenuItem *m_menuItem9;
  m_menuItem9 = new wxMenuItem(myMenu, ID_POPUP_REMOVE_FROM_SEL, wxString(wxT("Remove from selection")), wxEmptyString,
                               wxITEM_NORMAL);
  myMenu->Append(m_menuItem9);

  wxMenuItem *m_menuItem10;
  m_menuItem10 = new wxMenuItem(myMenu, ID_POPUP_ONLY_THIS_OBJ, wxString(wxT("Select this feature only")),
                                wxEmptyString, wxITEM_NORMAL);
  myMenu->Append(m_menuItem10);

  myMenu->AppendSeparator();

  wxMenuItem *m_menuItem11;
  m_menuItem11 =
      new wxMenuItem(myMenu, wxID_COPY, wxString(wxT("Copy data to clipboard")), wxEmptyString, wxITEM_NORMAL);
  myMenu->Append(m_menuItem11);

  return myMenu;
}

/***************************************************************************/ /**
 @brief Create grid controls for displaying informations
 @details This function may do the following : if both array are empty a static
  text ctrl will be added containing the following sentence : 'No data
  retreived'. Array values' are then parsed for building one or more grids
  depending if the break sentence (##<BREAK HERE>##) is encontred
 @author Lucien Schreiber (c) CREALP 2010
 @date 08 avril 2010
 *******************************************************************************/
void tmSelectionInfoCtrl::_CreateInfoControl(const wxTreeMultiItem &item, const wxArrayString &header,
                                             const wxArrayString &values) {
  wxASSERT(header.GetCount() == values.GetCount());

  // no data, create a static text ctrl
  if (header.GetCount() == 0) {
    wxStaticText *myText = new wxStaticText(this, wxID_ANY, _("No data to display"));
    AppendWindow(item, myText);
    return;
  }

  // search for separator
  wxArrayInt mySeparatorPosition;
  wxString mySeparatorText = _T("##<BREAK HERE>##");
  for (unsigned int i = 0; i < header.GetCount(); i++) {
    if (header.Item(i) == mySeparatorText) {
      wxASSERT(values.Item(i) == mySeparatorText);
      mySeparatorPosition.Add(i);
    }
  }
  mySeparatorPosition.Add(header.GetCount());

  // complex case, multiple information table
  // Freeze();
  for (unsigned int i = 0; i < mySeparatorPosition.GetCount(); i++) {
    wxGrid *myGrid = new wxGrid(this, wxID_ANY);
    myGrid->EnableEditing(false);
    int iNumCol = mySeparatorPosition.Item(i);
    if (i > 0) {
      iNumCol -= mySeparatorPosition.Item(i - 1);
      iNumCol--;
    }
    myGrid->CreateGrid(1, iNumCol);
    myGrid->SetRowLabelValue(0, _T(""));
    myGrid->SetRowLabelSize(1);

    // add values
    int iStartPos = 0;
    if (i > 0) {
      iStartPos = mySeparatorPosition.Item(i - 1) + 1;
    }
    for (int j = 0; j < iNumCol; j++) {
      myGrid->SetColLabelValue(j, header.Item(j + iStartPos));
      myGrid->SetCellValue(0, j, values.Item(j + iStartPos));
    }

    myGrid->AutoSize();
    AppendWindow(item, myGrid);
  }
  // TODO: Thaw crash but when corrected in wxWidgets, uncomment bellow and Freeze above Toooo !!!!
  // Thaw();
}

void tmSelectionInfoCtrl::_UpdateSelection() {
  wxWindow *myWindow = wxTheApp->GetTopWindow();
  wxASSERT(myWindow);

  // update display
  wxCommandEvent evt(tmEVT_LM_UPDATE, wxID_ANY);
  myWindow->GetEventHandler()->QueueEvent(evt.Clone());

  // update selecction / attribution panel
  wxCommandEvent evt2(tmEVT_SELECTION_DONE, wxID_ANY);
  myWindow->GetEventHandler()->QueueEvent(evt2.Clone());
}

bool tmSelectionInfoCtrl::_GetData(long oid, wxArrayString &header, wxArrayString &values) {
  header.Clear();
  values.Clear();

  wxASSERT(m_Project);
  wxASSERT(m_Toc);

  if (oid == wxNOT_FOUND) {
    wxLogError(_T("No object ID found."));
    return false;
  }

  tmLayerProperties *itemProp = m_Toc->GetSelectionLayer();
  if (!itemProp) {
    wxLogError(_T("No layer selected, select a layer"));
    return false;
  }

  tmGISData *myData = tmGISData::LoadLayer(itemProp);
  if (!myData) {
    wxLogError(_("Error loading GIS data for selected layer"));
    return false;
  }

  // don't do anything if layer aren't vector
  bool bReturn = true;
  switch (myData->GetDataType()) {
    case tmGIS_VECTOR_MYSQL:
      ((tmGISDataVectorMYSQL *)myData)->SetProject(m_Project);
    case tmGIS_VECTOR_SHAPEFILE:
      bReturn = ((tmGISDataVector *)myData)->GetFieldsName(header, oid);
      if (!bReturn) {
        break;
      }
      bReturn = false;
      bReturn = ((tmGISDataVector *)myData)->GetFieldsValue(values, oid);
      break;

    default:
      break;
  }

  wxDELETE(myData);
  if (!bReturn) {
    wxLogDebug(_T("Error getting informations for oid : %ld"), oid);
  }

  // Ensure to have existing entries in the list
  if (header.GetCount() > values.GetCount()) {
    values.Add(wxEmptyString, header.GetCount() - values.GetCount());
  }

  return bReturn;
}

bool tmSelectionInfoCtrl::_GetItemByMousePos(wxTreeMultiItem &item, const wxPoint &position) {
  int xViewScrolled = 0, yViewScrolled = 0;
  GetViewStart(&xViewScrolled, &yViewScrolled);

  int xUnitScroll = 0, yUnitScroll = 0;
  GetScrollPixelsPerUnit(&xUnitScroll, &yUnitScroll);

  wxPoint myPosition;
  myPosition.x = position.x + xViewScrolled * xUnitScroll;
  myPosition.y = position.y + yViewScrolled * yUnitScroll;

  int myFlags = 0;
  item = HitTest(myPosition, myFlags);

  if (!item.IsOk()) {
    return false;
  }

  if (myFlags == wxTMC_HITTEST_WINDOW) {
    return false;
  }

  if (item == GetFirstRoot()) {
    return false;
  }

  return true;
}

void tmSelectionInfoCtrl::OnItemLeftClick(wxMouseEvent &event) {
  wxPoint myRealPosition = event.GetPosition();
  m_ClickedItemID = wxNOT_FOUND;

  wxTreeMultiItem clickeditem;
  if (!_GetItemByMousePos(clickeditem, myRealPosition)) {
    return;
  }

  if (!clickeditem.GetName().ToLong(&m_ClickedItemID)) {
    wxFAIL;
    return;
  }

  int myCookie = 0;
  wxTreeMultiItem myCtrl = GetFirstChild(clickeditem, myCookie);
  if (myCtrl.IsOk()) {
    return;
  }

  wxArrayString myHeader;
  wxArrayString myValues;

  _GetData(m_ClickedItemID, myHeader, myValues);
  _CreateInfoControl(clickeditem, myHeader, myValues);

  // remove all control with windows
  _DeleteAllInfos(clickeditem);

  // event.Skip();
}

void tmSelectionInfoCtrl::OnItemRightClick(wxMouseEvent &event) {
  m_ClickedItemID = wxNOT_FOUND;

  wxTreeMultiItem myClickedItem;
  if (!_GetItemByMousePos(myClickedItem, event.GetPosition())) {
    return;
  }

  wxLogMessage(myClickedItem.GetName());
  if (!myClickedItem.GetName().ToLong(&m_ClickedItemID)) {
    wxFAIL;
    return;
  }

  PopupMenu(_CreatePopupMenu(), event.GetPosition().x, event.GetPosition().y);
  // event.Skip();
}

void tmSelectionInfoCtrl::OnPopupSelectionThis(wxCommandEvent &event) {
  wxASSERT(m_Selected);
  wxASSERT(m_ClickedItemID != wxNOT_FOUND);
  wxLogMessage(_("Selecting item %ld"), m_ClickedItemID);

  m_Selected->SetSelected(m_ClickedItemID);

  _UpdateSelection();
}

void tmSelectionInfoCtrl::OnPopupSelectionRemove(wxCommandEvent &event) {
  wxASSERT(m_Selected);
  wxASSERT(m_ClickedItemID != wxNOT_FOUND);
  wxLogMessage(_("Removing item %ld from selection"), m_ClickedItemID);

  wxArrayLong myRemoveArray;
  myRemoveArray.Add(m_ClickedItemID);
  m_Selected->Remove(&myRemoveArray);

  _UpdateSelection();
}

void tmSelectionInfoCtrl::OnPopupMove(wxCommandEvent &event) {
  wxASSERT(m_Selected);
  wxASSERT(m_ClickedItemID != wxNOT_FOUND);

  // Send event to layermanager
  wxWindow *myWindow = wxTheApp->GetTopWindow();
  wxASSERT(myWindow);

  wxCommandEvent evt(tmEVT_LM_MOVE_TO_FEATURE, wxID_ANY);
  evt.SetExtraLong(m_ClickedItemID);
  myWindow->GetEventHandler()->QueueEvent(evt.Clone());
}

void tmSelectionInfoCtrl::OnPopupZoom(wxCommandEvent &event) {
  wxASSERT(m_Selected);
  wxASSERT(m_ClickedItemID != wxNOT_FOUND);

  // Send event to layermanager
  wxWindow *myWindow = wxTheApp->GetTopWindow();
  wxASSERT(myWindow);

  wxCommandEvent evt(tmEVT_LM_ZOOM_TO_FEATURE, wxID_ANY);
  evt.SetExtraLong(m_ClickedItemID);
  myWindow->GetEventHandler()->QueueEvent(evt.Clone());
}

void tmSelectionInfoCtrl::OnPopupCopy(wxCommandEvent &event) {
  wxASSERT(m_ClickedItemID != wxNOT_FOUND);

  wxArrayString myHeader;
  wxArrayString myValues;
  if (!_GetData(m_ClickedItemID, myHeader, myValues)) {
    wxLogWarning(_("Unable to retrieve data ! Data not copied to clipboard"));
    return;
  }

  // prepare data
  wxString myConcatInfo;
  wxString mySeparatorText = _T("##<BREAK HERE>##");
  for (unsigned int i = 0; i < myHeader.GetCount(); i++) {
    if (myHeader.Item(i) != mySeparatorText) {
      myConcatInfo.Append(myHeader.Item(i));
    }
    myConcatInfo.Append(_T("\t"));
  }
  myConcatInfo.Append(_T("\n"));
  for (unsigned int i = 0; i < myValues.GetCount(); i++) {
    if (myHeader.Item(i) != mySeparatorText) {
      myConcatInfo.Append(myValues.Item(i));
      ;
    }

    myConcatInfo.Append(_T("\t"));
  }

  // copy data
  if (!wxTheClipboard->Open()) {
    wxLogError(_("Unable to open the clipboard"));
    return;
  }

  // This data objects are held by the clipboard,
  // so do not delete them in the app.
  wxTheClipboard->SetData(new wxTextDataObject(myConcatInfo));
  wxTheClipboard->Close();
  wxLogStatus(_("Data copied to clipboard"));
}

void tmSelectionInfoCtrl::UpdateSelection() {
  wxASSERT(m_Selected);
  // Freeze();
  DeleteChildren(GetFirstRoot());

  wxArrayLong *mySelVal = m_Selected->GetSelectedValues();
  if (!mySelVal) {
    // Thaw();
    return;
  }

  wxTreeMultiItem myItem;
  for (unsigned int i = 0; i < m_Selected->GetCount(); i++) {
    myItem = AppendNode(GetFirstRoot(), wxString() << mySelVal->Item(i), wxString() << mySelVal->Item(i));
    Collapse(myItem, false);
  }

  // feature #189 display information automatically if only one feature selected
  if (m_Selected->GetCount() == 1) {
    if (myItem.IsOk()) {
      wxArrayString myHeader;
      wxArrayString myValues;

      if (myItem.GetName().ToLong(&m_ClickedItemID) != false) {
        _GetData(m_ClickedItemID, myHeader, myValues);
        _CreateInfoControl(myItem, myHeader, myValues);

        // remove all control with windows
        _DeleteAllInfos(myItem);
        Expand(myItem, false);
      }
    }
  }
  // Thaw();
}

void tmSelectionInfoCtrl::SetProject(PrjDefMemManage *project) {
  m_Project = project;
}
