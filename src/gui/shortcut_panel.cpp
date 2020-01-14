/***************************************************************************
 shortcut_panel.cpp
  Display the Shortcut panel
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

#include "shortcut_panel.h"

#include "../database/database_tm.h"

DEFINE_EVENT_TYPE(tmEVT_SHORTCUT_REFRESH)

BEGIN_EVENT_TABLE(Shortcuts_PANEL, ManagedAuiWnd)
EVT_CHOICE(IDDLG_SHORT_TARGET, Shortcuts_PANEL::OnChangeTarget)
EVT_FLATBUTTON(IDDLG_SHORT_ADD_BTN, Shortcuts_PANEL::OnShortcutAdd)
EVT_FLATBUTTON(IDDLG_SHORT_DEL_BTN, Shortcuts_PANEL::OnShortcutDel)
// EVT_FLATBUTTON( IDDLG_SHORT_EDIT_BTN, Shortcuts_PANEL::OnShortcutEdit )
END_EVENT_TABLE()

Shortcuts_PANEL::Shortcuts_PANEL(wxWindow *parent, wxWindowID id, wxAuiManager *auimanager)
    : ManagedAuiWnd(auimanager) {
  InitMemberValues();
  wxASSERT(parent);
  m_ParentEvt = parent;
  m_ParentEvt->PushEventHandler(this);

  // create the controls
  wxPanel *ContentFrame = new wxPanel(parent, wxID_ANY);
  CreateControls(ContentFrame);
  ContentFrame->Fit();

  // define properties of Panel.
  m_PaneInfo.Name(SHORTCUT_PANEL_TITLE);
  m_PaneInfo.Caption(SHORTCUT_PANEL_TITLE);
  m_PaneInfo.Float();
  m_PaneInfo.Right();
  m_PaneInfo.Layer(4);
  m_PaneInfo.Position(4);
  m_PaneInfo.MinSize(SHORTCUT_PANEL_SIZE);
  m_PaneInfo.CloseButton(true);
  m_PaneInfo.Hide();
  m_PaneInfo.FloatingPosition(100, 150);
  m_PaneInfo.FloatingSize(SHORTCUT_PANEL_SIZE + wxSize(100, 100));

  m_AuiPanelName = SHORTCUT_PANEL_TITLE;

  AddManagedPane(ContentFrame, m_PaneInfo);
}

void Shortcuts_PANEL::InitMemberValues() {
  m_TargetChoice = NULL;
  m_ListShortcuts = NULL;
  m_ParentEvt = NULL;
  m_pDB = NULL;
  m_ProjectOpen = false;
}

Shortcuts_PANEL::~Shortcuts_PANEL() {
  m_ParentEvt->PopEventHandler(FALSE);
}

/***************************************************************************/ /**
  @brief Setter for the Database
  @details Set database to the panel and to the contained list
  @author Lucien Schreiber (c) CREALP 2008
  @date 12 December 2008
  *******************************************************************************/
void Shortcuts_PANEL::SetDataBase(DataBaseTM *database) {
  wxASSERT(database);
  m_pDB = database;
  m_ListShortcuts->SetDataBase(database);
}

/***************************************************************************/ /**
  @brief Control creation
  @author Lucien Schreiber (c) CREALP 2008
  @date 08 November 2008
  *******************************************************************************/
wxSizer *Shortcuts_PANEL::CreateControls(wxWindow *parent, bool call_fit, bool set_sizer) {
  wxBoxSizer *bSizer1;
  bSizer1 = new wxBoxSizer(wxVERTICAL);

  wxBoxSizer *bSizer2;
  bSizer2 = new wxBoxSizer(wxHORIZONTAL);

  wxStaticText *m_staticText1;
  m_staticText1 = new wxStaticText(parent, wxID_ANY, _("Target :"), wxDefaultPosition, wxDefaultSize, 0);
  m_staticText1->Wrap(-1);
  bSizer2->Add(m_staticText1, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

  m_TargetChoice =
      new wxChoice(parent, IDDLG_SHORT_TARGET, wxDefaultPosition, wxDefaultSize, 3, TOC_GENERIC_NAME_STRING, 0);
  m_TargetChoice->SetSelection(TOC_NAME_LINES);
  bSizer2->Add(m_TargetChoice, 1, wxALL | wxEXPAND, 5);

  bSizer1->Add(bSizer2, 0, wxEXPAND, 5);

  wxArrayString myColName;
  myColName.Add(_("Function Key"));
  myColName.Add(_("Description"));

  wxArrayInt myColWidth;
  myColWidth.Add(80);
  myColWidth.Add(200);

  m_ListShortcuts = new ShortcutList(parent, m_ParentEvt, IDDLG_SHORT_LIST, &myColName, &myColWidth, wxSize(100, 100));
  bSizer1->Add(m_ListShortcuts, 1, wxALL | wxEXPAND, 0);

  wxBoxSizer *bSizer3;
  bSizer3 = new wxBoxSizer(wxHORIZONTAL);

  wxFlatButton *myShortcutAdd = new wxFlatButton(parent, IDDLG_SHORT_ADD_BTN, wxFLATBUTTON_TEXT_ADD);
  bSizer3->Add(myShortcutAdd, 0, wxLEFT, 0);

  wxFlatButton *myShortcutDel = new wxFlatButton(parent, IDDLG_SHORT_DEL_BTN, wxFLATBUTTON_TEXT_REMOVE);
  bSizer3->Add(myShortcutDel, 0, wxLEFT, 5);

  /*wxFlatButton * myShortcutEdit = new wxFlatButton( parent, IDDLG_SHORT_EDIT_BTN, _("Edit selected..."),
  wxDefaultSize); bSizer3->Add( myShortcutEdit, 0, wxLEFT|wxRIGHT, 5 );*/

  bSizer1->Add(bSizer3, 0, wxALL, 5);

  if (set_sizer) {
    parent->SetSizer(bSizer1);
    if (call_fit) {
      bSizer1->SetSizeHints(parent);
    }
  }

  return bSizer1;
}

/***************************************************************************/ /**
  @brief Load Shortcut into the Shortcut list
  @details This function loads the shortcuts from the DB into the shortcut list
  layer type is selected by actual list choice.
  @param bStoreShortcutinmemory If set to true, Shortcuts are stored into memory.
  @return  Number of values loaded
  @author Lucien Schreiber (c) CREALP 2008
  @date 11 December 2008
  *******************************************************************************/
int Shortcuts_PANEL::LoadShortcutList(bool bStoreShortcutinmemory) {
  wxASSERT(m_pDB);
  if (!m_pDB) {
    SetProjectOpen(false);
    return wxNOT_FOUND;
  }

  // clear list
  m_ListShortcuts->DeleteAllItems();

  // load data from db and add in the shortcut list
  bool myFirstLoop = true;
  int iCount = 0;
  wxString myKey = _T("");
  wxString myDescription = _T("");

  while (1) {
    if (!m_pDB->GetNextShortcutByLayerType(m_TargetChoice->GetSelection(), myKey, myDescription, myFirstLoop)) {
      break;
    }

    myFirstLoop = false;

    m_ListShortcuts->AddItemToList(myKey, iCount);
    m_ListShortcuts->SetItemText(iCount, 1, myDescription);

    iCount++;
  }

  // save shortcut into memory
  if (bStoreShortcutinmemory) m_ListShortcuts->RefreshShortcuts();

  return iCount;
}

/***************************************************************************/ /**
  @brief When target changes
  @author Lucien Schreiber (c) CREALP 2008
  @date 12 December 2008
  *******************************************************************************/
void Shortcuts_PANEL::OnChangeTarget(wxCommandEvent &event) {
  LoadShortcutList();
  m_ListShortcuts->SetLayerType(event.GetSelection());
  event.Skip();
}

/***************************************************************************/ /**
  @brief Called when adding new shortcut
  @details Check is done if project is open
  @author Lucien Schreiber (c) CREALP 2008
  @date 12 December 2008
  *******************************************************************************/
void Shortcuts_PANEL::OnShortcutAdd(wxCommandEvent &event) {
  if (m_ProjectOpen) {
    m_ListShortcuts->AddItem();
  }

  event.Skip();
}

/***************************************************************************/ /**
  @brief Called when removing a shortcut
  @details Check is done if project is open
  @author Lucien Schreiber (c) CREALP 2008
  @date 12 December 2008
  *******************************************************************************/
void Shortcuts_PANEL::OnShortcutDel(wxCommandEvent &event) {
  if (m_ProjectOpen) {
    m_ListShortcuts->DeleteItem();
  }

  event.Skip();
}

/***************************** SHORTCUT LIST *****************************/

/***************************************************************************/ /**
  @brief Constructor
  @author Lucien Schreiber (c) CREALP 2008
  @date 10 December 2008
  *******************************************************************************/
ShortcutList::ShortcutList(wxWindow *parent, wxWindow *parent_evt, wxWindowID id, wxArrayString *pColsName,
                           wxArrayInt *pColsSize, wxSize size)
    : ListGenReportWithDialog(parent, id, pColsName, pColsSize, size, wxLC_REPORT | wxLC_SINGLE_SEL) {
  m_LayerType = 0;
  m_ParentEvt = parent_evt;
  m_pDB = NULL;
  m_OldKey = -1;
}

/***************************************************************************/ /**
  @brief Destructor
  @author Lucien Schreiber (c) CREALP 2008
  @date 10 December 2008
  *******************************************************************************/
ShortcutList::~ShortcutList() {}

/***************************************************************************/ /**
  @brief Convert shortcut key displayed as string into integer
  @details An internal verification is done to avoid being outer bound
  @param myShortCut The shortcut we want to convert
  @return  The shortcut as integer
  @author Lucien Schreiber (c) CREALP 2008
  @date 15 December 2008
  *******************************************************************************/
int ShortcutList::GetShortcutInt(const wxString &myShortCut) {
  // convert key to integer
  wxString myActualKey = myShortCut.AfterFirst('F');
  int iActualKey = wxAtoi(myActualKey);
  wxASSERT(iActualKey > 0 && iActualKey < 13);
  return iActualKey;
}

/***************************************************************************/ /**
  @brief Called just before adding shortcut
  @details Prepare the dialog to be displayed for adding a shortcut
  @author Lucien Schreiber (c) CREALP 2008
  @date 12 December 2008
  *******************************************************************************/
void ShortcutList::BeforeAdding() {
  // get unused key (all non assigned keys)
  wxArrayString myUnusedKeys;
  bool bGetKeys = m_pDB->GetAllUnusedShortcuts(myUnusedKeys);
  wxASSERT(bGetKeys);

  Shortcut_Panel_DLG *myDlg = new Shortcut_Panel_DLG(this);
  myDlg->SetKeyList(myUnusedKeys);
  myDlg->SetTypeList(m_pDB, m_LayerType);

  SetDialog(myDlg);
}

/***************************************************************************/ /**
  @brief Called just after adding shortcut
  @details Add the new shortcut to the database
  @param bRealyAddItem is true if
  item was added, and false if dialog was cancelled
  @author Lucien Schreiber (c) CREALP 2008
  @date 12 December 2008
  *******************************************************************************/
void ShortcutList::AfterAdding(bool bRealyAddItem) {
  Shortcut_Panel_DLG *myDlg = static_cast<Shortcut_Panel_DLG *>(m_pDialog);
  if (bRealyAddItem) {
    m_pDB->EditShortcut(myDlg->m_SelectedKey, myDlg->m_Description, myDlg->m_CheckedTypes);

    AddItemToList(GetKeyFromInt(myDlg->m_SelectedKey), -1);
    SetItemText(GetItemCount() - 1, 1, myDlg->m_Description);
    RefreshShortcuts();
  }

  // delete m_pDialog;
}

/***************************************************************************/ /**
  @brief Called just before editing a shortcut
  @author Lucien Schreiber (c) CREALP 2008
  @date 15 December 2008
  *******************************************************************************/
void ShortcutList::BeforeEditing() {
  // get actual selected key
  wxString myActualKey = GetItemColText(GetSelectedItem(), 0);

  wxArrayString myUnusedKeys;

  // get unused key (all non assigned keys)
  bool bGetKeys = m_pDB->GetAllUnusedShortcuts(myUnusedKeys);
  // add actual selected key
  myUnusedKeys.Insert(myActualKey, 0);
  wxASSERT(bGetKeys);

  // convert key to integer
  m_OldKey = GetShortcutInt(myActualKey);

  // load check list
  Shortcut_Panel_DLG *myDlg = new Shortcut_Panel_DLG(this);
  myDlg->m_Description = GetItemColText(GetSelectedItem(), 1);
  myDlg->SetKeyList(myUnusedKeys);
  myDlg->SetTypeList(m_pDB, m_LayerType, m_OldKey);

  SetDialog(myDlg);
}

/***************************************************************************/ /**
  @brief Called just after editing a shortcut
  @author Lucien Schreiber (c) CREALP 2008
  @date 15 December 2008
  *******************************************************************************/
void ShortcutList::AfterEditing(bool bRealyEdited) {
  Shortcut_Panel_DLG *myDlg = static_cast<Shortcut_Panel_DLG *>(m_pDialog);
  if (bRealyEdited) {
    if (m_OldKey != myDlg->m_SelectedKey) m_pDB->DeleteShortcut(m_OldKey);

    m_pDB->EditShortcut(myDlg->m_SelectedKey, myDlg->m_Description, myDlg->m_CheckedTypes);

    // updating list
    int iIndex = GetSelectedItem();
    SetItemText(iIndex, 0, GetKeyFromInt(myDlg->m_SelectedKey));
    SetItemText(iIndex, 1, myDlg->m_Description);
    RefreshShortcuts();
  }

  // delete m_pDialog;
}

/***************************************************************************/ /**
  @brief Called just before deleting a shortcut
  @author Lucien Schreiber (c) CREALP 2008
  @date 15 December 2008
  *******************************************************************************/
void ShortcutList::BeforeDeleting() {
  int mySelected = GetSelectedItem();
  m_pDB->DeleteShortcut(GetShortcutInt(GetItemColText(mySelected, 0)));
  RefreshShortcuts();
}

/***************************************************************************/ /**
  @brief Send order to refresh shortcuts
  @author Lucien Schreiber (c) CREALP 2008
  @date 18 December 2008
  *******************************************************************************/
void ShortcutList::RefreshShortcuts() {
  wxCommandEvent evt(tmEVT_SHORTCUT_REFRESH, wxID_ANY);
  m_ParentEvt->GetEventHandler()->QueueEvent(evt.Clone());
}
