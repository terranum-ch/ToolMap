/***************************************************************************
 tmattributionmanager.cpp
 Manage the attribution
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

#include "tmattributionmanager.h"

#include "../core/tmstatsevent.h"
#include "../database/database_tm.h"  // for database
#include "../gui/aattribbatch_dlg.h"  // batch attribution window
#include "../gui/information_dlg.h"   // for informations window

DEFINE_EVENT_TYPE(tmEVT_SHORTCUT_ATTRIBUTION_DONE);
DEFINE_EVENT_TYPE(tmEVT_AM_COPY_ATTRIBUTION);

BEGIN_EVENT_TABLE(tmAttributionManager, wxEvtHandler)
EVT_COMMAND(wxID_ANY, tmEVT_SELECTION_DONE, tmAttributionManager::OnSelection)
// EVT_COMMAND (wxID_ANY, tmEVT_ATTRIBUTION_BTN_PRESSED, tmAttributionManager::OnAttributeBtn)
EVT_COMMAND(wxID_ANY, tmEVT_ADD_BTN_PRESSED, tmAttributionManager::OnAddBtn)
EVT_COMMAND(wxID_ANY, tmEVT_REMOVE_BTN_PRESSED, tmAttributionManager::OnRemoveBtn)
EVT_COMMAND(wxID_ANY, tmEVT_INFO_BTN_PRESSED, tmAttributionManager::OnInfoBtn)
EVT_COMMAND(wxID_ANY, tmEVT_QUERY_RUN, tmAttributionManager::OnRunQuery)
EVT_COMMAND(wxID_ANY, tmEVT_SHORTCUT_REFRESH, tmAttributionManager::OnRefreshShortcut)
EVT_COMMAND(wxID_ANY, tmEVT_AM_SHORTCUT_PRESSED, tmAttributionManager::OnShortcutPressed)
EVT_COMMAND(wxID_ANY, tmEVT_AM_COPY_ATTRIBUTION, tmAttributionManager::OnCopyAttribution)
EVT_COMMAND(wxID_ANY, tmEVT_TOC_SELECTION_CHANGED, tmAttributionManager::OnLayerChanged)
END_EVENT_TABLE()

/***************************************************************************/ /**
  @brief Constructor
  @param parent Adress of the parent, used for event processing
  @param toc Adress of a #tmTOCCtrl
  @param panel Adress of a #AttribObjType_PANEL object
  @param selection Adress of a #tmSelectedDataMemory object
  @author Lucien Schreiber (c) CREALP 2008
  @date 04 November 2008
  *******************************************************************************/
tmAttributionManager::tmAttributionManager(wxWindow *parent, tmTOCCtrl *toc, AttribObjType_PANEL *panel,
                                           tmSelectedDataMemory *selection) {
  m_Parent = parent;
  m_TOC = toc;
  m_Panel = panel;
  m_SelData = selection;
  m_pPrjMem = NULL;
  m_InfoDLG = NULL;

  m_pDB = NULL;
  m_pLayerProperties = NULL;
  m_Parent->PushEventHandler(this);
  m_ShortcutLoaded = false;
}

/***************************************************************************/ /**
  @brief Connect shortcut
  @details This function is called when project is open
  @author Lucien Schreiber (c) CREALP 2008
  @date 18 December 2008
  *******************************************************************************/
void tmAttributionManager::ConnectShortcutEvent() {
  m_ShortcutLoaded = true;
}

/***************************************************************************/ /**
  @brief Disconnect shortcut
  @details This function is called when project is closed
  @author Lucien Schreiber (c) CREALP 2008
  @date 18 December 2008
  *******************************************************************************/
void tmAttributionManager::DisconnectShortcutEvent() {
  m_ShortcutLoaded = false;
}

/***************************************************************************/ /**
  @brief Called when a key is pressed
  @details This function respond to the F1-F12 shortcut key
  @author Lucien Schreiber (c) CREALP 2008
  @date 18 December 2008
  *******************************************************************************/
void tmAttributionManager::OnShortcutPressed(wxCommandEvent &event) {
  int iShortcutIndex = 0;
  int myLayerType = -1;
  wxString myDescription = _T("");
  wxArrayLong myKindValues;

  if (!m_ShortcutLoaded) {
    return;
  }

  int myKeyCode = event.GetInt();
  wxLogDebug(_T("Key pressed : value %d"), myKeyCode);
  if (myKeyCode >= WXK_F1 && myKeyCode <= WXK_F12) {
    // get the key index :
    iShortcutIndex = myKeyCode - WXK_F1 + 1;
    wxASSERT(iShortcutIndex >= 1 && iShortcutIndex <= 12);
    if (m_ShortcutMem.GetShortcut(iShortcutIndex, myLayerType, myDescription, myKindValues) != wxNOT_FOUND) {
      wxArrayLong *mySelItemsRef = m_SelData->GetSelectedValues();

      if (mySelItemsRef && ShortcutAttributionChecking(mySelItemsRef->GetCount(), myLayerType)) {
        wxLogDebug(_T("Shortcut found : %s"), myDescription.c_str());
        // Shortcuts are now adding data to attribution (#332)
        // REPLACE INTO generic_aat VALUES (KIND, GEOMETRY)
        wxString myStmt = wxEmptyString;
        for (unsigned int s = 0; s < mySelItemsRef->GetCount(); s++) {
          for (unsigned int k = 0; k < myKindValues.GetCount(); k++) {
            myStmt.Append(wxString::Format(_T("REPLACE INTO %s VALUES (%ld, %ld); "),
                                           TABLE_NAME_GIS_ATTRIBUTION[m_pLayerProperties->GetType()],
                                           myKindValues.Item(k), mySelItemsRef->Item(s)));
          }
        }

        if (!m_pDB->DataBaseQueryNoResults(myStmt)) {
          wxLogError(_("Adding kind(s) to selected features failed!"));
        }

        // send notification to frame
        wxCommandEvent evt(tmEVT_SHORTCUT_ATTRIBUTION_DONE, wxID_ANY);
        evt.SetString(myDescription);
        m_Parent->GetEventHandler()->QueueEvent(evt.Clone());

        // send statistics
        wxCommandEvent statevt(tmEVT_STAT_ATTRIB, wxID_ANY);
        m_Parent->GetEventHandler()->QueueEvent(statevt.Clone());
      }
    }
  }
}

/***************************************************************************/ /**
  @brief Validate program state before shortcut attribution
  @details We should ensure that :
  - Selected layer in the TOC correspond to the Shortcut layer type
  - There is some object selected
  - Display a validation
  dialog if trying to give values to more than one object to avoid big
  attribution mistakes
  @param iCount Number of selected object
  @param shortcutlayer_type The layer type of the shortcut
  @return  true if all checks listed above are valid, false if we shouldn't
  attribute
  @author Lucien Schreiber (c) CREALP 2008
  @date 18 December 2008
  *******************************************************************************/
bool tmAttributionManager::ShortcutAttributionChecking(int iCount, int shortcutlayer_type) {
  wxASSERT(m_pLayerProperties);
  if (m_pLayerProperties->GetType() != shortcutlayer_type) {
    wxLogDebug(
        _T("Something incorrect, selected shortcut")
        _T(" doesn't correspond to the selected layer"));
    return false;
  }

  if (iCount == 0) {
    wxLogDebug(_T("No object selected, unable to attribute"));
    return false;
  }

  // more than one object selected, display confirmation dialog box
  if (iCount > 1) {
    wxString myM = _("More than one object is selected!\n");
    wxString myM2 = _("Are you really sure you want to attributes\n");
    wxString myM3 = wxString::Format(_("the %d selected features ?\n"), iCount);

    if (wxMessageBox(myM + myM2 + myM3, _("Warning more than one object selected"), wxYES_NO | wxNO_DEFAULT,
                     m_Parent) == wxNO)
      return false;
  }

  return true;
}

/***************************************************************************/ /**
  @brief Destructor
  @details Doesn't do anything actually
  @author Lucien Schreiber (c) CREALP 2008
  @date 04 November 2008
  *******************************************************************************/
tmAttributionManager::~tmAttributionManager() {
  UnInitAttributionManager();
  m_Parent->PopEventHandler(false);
  // m_Parent->SetEventHandler(m_Parent);
}

/***************************************************************************/ /**
  @brief Init attribution manager
  @details This function should be the first called for beeing able to use the
  attibution manager
  @return  true if Database, TOC, selection data, and panel are valid
  @author Lucien Schreiber (c) CREALP 2008
  @date 04 November 2008
  *******************************************************************************/
bool tmAttributionManager::InitAttributionManager(DataBaseTM *pDb, PrjDefMemManage *memprojdef) {
  m_pDB = pDb;
  wxASSERT(m_pDB);

  m_pPrjMem = memprojdef;
  wxASSERT(m_pPrjMem);

  m_pLayerProperties = NULL;

  m_InfoDLG = (InformationDLG *)wxWindow::FindWindowById(ID_INFORMATION_DLG);
  if (m_InfoDLG != NULL) {
    m_InfoDLG->SetProject(m_pPrjMem);
  }

  // check validity for all objects
  bool bIsReady = IsAttributionManagerReady();
  if (bIsReady) ConnectShortcutEvent();

  // disable info button
  // m_Panel->SetInfoBtn(1);

  return bIsReady;
}

/***************************************************************************/ /**
  @brief Uninit attribution manager
  @details Mark the attribution manager as not valid.
  @author Lucien Schreiber (c) CREALP 2008
  @date 04 November 2008
  *******************************************************************************/
void tmAttributionManager::UnInitAttributionManager() {
  m_pDB = NULL;
  m_pLayerProperties = NULL;

  DisconnectShortcutEvent();
  m_Panel->SetInfoBtn(0);
}

/***************************************************************************/ /**
  @brief Checks all #tmAttributionManager object for validity
  @return  true if all objects are valid, false otherwise
  @author Lucien Schreiber (c) CREALP 2008
  @date 04 November 2008
  *******************************************************************************/
bool tmAttributionManager::IsAttributionManagerReady() {
  if (!m_Parent) return false;

  if (!m_Panel) return false;

  if (!m_SelData) return false;

  if (!m_TOC) return false;

  if (!m_pDB) return false;

  return true;
}

/***************************************************************************/ /**
  @brief Checks that only one object is selected
  @details Use the IsAttributionManagerReady() before to ensure that all controls
  are well initialised
  @return true if only one item is selected
  @author Lucien Schreiber (c) CREALP 2009
  @date 11 March 2009
  *******************************************************************************/
bool tmAttributionManager::IsOnlyOneObjSelected() {
  wxASSERT(m_SelData);
  if (m_SelData->GetCount() == 1) return true;
  return false;
}

/***************************************************************************/ /**
  @brief Called when selection was made
  @details This is called by the #tmRenderer when a selection (either by point or
  rectangle) was made. This event is also used by #tmLayerManager::OnSelection
  @param event Contain data about the status of the shift key and the rectangle
  size.
  - Use event.GetInt() for getting the status of the shift key
  - Use event.GetClientObject() for getting the wxRect.
  @author Lucien Schreiber (c) CREALP 2008
  @date 29 October 2008
  *******************************************************************************/
void tmAttributionManager::OnSelection(wxCommandEvent &event) {
  // update info dlg
  m_InfoDLG = (InformationDLG *)wxWindow::FindWindowById(ID_INFORMATION_DLG);
  if (m_InfoDLG != NULL) {
    m_InfoDLG->UpdateSelection();
  }

  int iSelFeatureCount = m_SelData->GetCount();

  // some verifications :
  // A layer must be selected
  // A construction layer must be selected (< TOC_NAME_FRAME)
  m_pLayerProperties = m_TOC->GetSelectionLayer();
  if (!m_pLayerProperties) {
    event.Skip();
    return;
  }

  if (m_pLayerProperties->GetType() >= TOC_NAME_FRAME) {
    event.Skip();
    return;
  }

  bool bEditMode = false;
  if (m_TOC->GetEditLayer() != NULL) {
    bEditMode = true;
  }
  m_Panel->SetAttributeBtn(iSelFeatureCount, bEditMode);

  m_Panel->SetInfoBtn(iSelFeatureCount);
  TOC_GENERIC_NAME mySelType = m_pLayerProperties->GetType();
  m_Panel->SetVisibleNotebook(mySelType);

  // if auto display attribute is checked
  if (m_Panel->IsAutoDisplayAttributeChecked()) {
    if (iSelFeatureCount == 1) {
      wxCommandEvent evt(tmEVT_INFO_BTN_PRESSED, wxID_ANY);
      m_Parent->GetEventHandler()->QueueEvent(evt.Clone());
    } else {
      m_Panel->EmptyListValues();
    }
  }

  // propagate to menu
  event.Skip();
}

void tmAttributionManager::OnAddBtn(wxCommandEvent &event) {

  if (!m_pLayerProperties) {
    wxLogError(_("No layer selected!"));
    return;
  }

  wxString myMsg = _("Selected panel doesn't correspond to the edited layer");
  if (m_pLayerProperties->GetType() != m_Panel->GetVisibleNotebook()) {
    wxMessageBox(myMsg, _("Attribution error"), wxOK | wxICON_ERROR, m_Parent);
    return;
  }

  // get checked values
  wxArrayLong myKindValues;
  m_Panel->GetSelectedValues(m_pLayerProperties->GetType(), myKindValues, true);
  m_Panel->GetSelectedValues(m_pLayerProperties->GetType(), myKindValues, false);
  if (myKindValues.GetCount() == 0) {
    wxLogWarning(_("There is no kind checked! Adding nothing to attribution didn't mean anything!"));
    return;
  }

  // REPLACE INTO generic_aat VALUES (KIND, GEOMETRY)
  wxString myStmt = wxEmptyString;
  wxArrayLong *mySelItemsRef = m_SelData->GetSelectedValues();
  for (unsigned int s = 0; s < mySelItemsRef->GetCount(); s++) {
    for (unsigned int k = 0; k < myKindValues.GetCount(); k++) {
      myStmt.Append(wxString::Format(_T("REPLACE INTO %s VALUES (%ld, %ld); "),
                                     TABLE_NAME_GIS_ATTRIBUTION[m_pLayerProperties->GetType()], myKindValues.Item(k),
                                     mySelItemsRef->Item(s)));
    }
  }

  if (!m_pDB->DataBaseQueryNoResults(myStmt)) {
    wxLogError(_("Adding kind(s) to selected features failed!"));
  }

  // clear list if needed
  if (m_Panel->IsEmptyListValuesRequired()) {
    m_Panel->EmptyListValues();
  }

  // focus to the renderer
  wxCommandEvent evt(tmEVT_FOCUS_RENDERER, wxID_ANY);
  m_Parent->GetEventHandler()->QueueEvent(evt.Clone());

  // send statistics
  wxCommandEvent statevt(tmEVT_STAT_ATTRIB, wxID_ANY);
  m_Parent->GetEventHandler()->QueueEvent(statevt.Clone());
}

void tmAttributionManager::OnRemoveBtn(wxCommandEvent &event) {
  wxASSERT(m_pLayerProperties);
  wxString myMsg = _("Selected panel doesn't correspond to the edited layer");
  if (m_pLayerProperties->GetType() != m_Panel->GetVisibleNotebook()) {
    wxMessageBox(myMsg, _("Attribution error"), wxOK | wxICON_ERROR, m_Parent);
    return;
  }

  // get checked values
  wxArrayLong myKindValues;
  m_Panel->GetSelectedValues(m_pLayerProperties->GetType(), myKindValues, true);
  m_Panel->GetSelectedValues(m_pLayerProperties->GetType(), myKindValues, false);
  if (myKindValues.GetCount() == 0) {
    wxLogWarning(_("There is nothing checked! Removing nothing from attribution didn't mean anything!"));
    return;
  }

  // DELETE FROM generic_aat WHERE OBJECT_VAL_ID = ? AND OBJECT_GEOM_ID = ?
  wxString myStmt = wxEmptyString;
  wxArrayLong *mySelItemsRef = m_SelData->GetSelectedValues();
  for (unsigned int s = 0; s < mySelItemsRef->GetCount(); s++) {
    for (unsigned int k = 0; k < myKindValues.GetCount(); k++) {
      myStmt.Append(wxString::Format(_T("DELETE FROM %s WHERE OBJECT_VAL_ID = %ld AND OBJECT_GEOM_ID = %ld; "),
                                     TABLE_NAME_GIS_ATTRIBUTION[m_pLayerProperties->GetType()], myKindValues.Item(k),
                                     mySelItemsRef->Item(s)));
    }
  }

  if (!m_pDB->DataBaseQueryNoResults(myStmt)) {
    wxLogError(_("Removing kind(s) to selected features failed!"));
  }

  // clear list if needed
  if (m_Panel->IsEmptyListValuesRequired()) {
    m_Panel->EmptyListValues();
  }

  // focus to the renderer
  wxCommandEvent evt(tmEVT_FOCUS_RENDERER, wxID_ANY);
  m_Parent->GetEventHandler()->QueueEvent(evt.Clone());

  // send statistics
  wxCommandEvent statevt(tmEVT_STAT_ATTRIB, wxID_ANY);
  m_Parent->GetEventHandler()->QueueEvent(statevt.Clone());
}

/***************************************************************************/ /**
  @brief Called when user press the info button
  @details This function is called by the #AttribObjType_PANEL
  @author Lucien Schreiber (c) CREALP 2008
  @date 07 November 2008
  *******************************************************************************/
void tmAttributionManager::OnInfoBtn(wxCommandEvent &event) {
  wxASSERT(m_pLayerProperties);
  wxASSERT(m_pDB);

  // create attribution object based on type
  wxArrayLong *mySelObjArray = m_SelData->GetSelectedValues();
  tmAttributionData *myAttrib = CreateAttributionData(m_pLayerProperties->GetType());

  myAttrib->Create(mySelObjArray, m_pDB);
  if (!myAttrib->GetInfoBasic(m_Panel)) {
    wxLogError(_("Error getting informations for objects"));
  }

  wxDELETE(myAttrib);
  wxDELETE(mySelObjArray);
}

/***************************************************************************/ /**
  @brief Called when user modifies the shortcut
  @details This function reload the shortcuts into memory when shortcuts are
  changed
  @author Lucien Schreiber (c) CREALP 2008
  @date 18 December 2008
  *******************************************************************************/
void tmAttributionManager::OnRefreshShortcut(wxCommandEvent &event) {
  wxLogMessage(_T("%d : shortcuts loaded into memory"), LoadShortcutIntoMemory());
}

/***************************************************************************/ /**
  @brief Load shortcuts from database into memory
  @return  the number of loaded shortcuts
  @author Lucien Schreiber (c) CREALP 2008
  @date 18 December 2008
  *******************************************************************************/
int tmAttributionManager::LoadShortcutIntoMemory() {
  wxASSERT(m_pDB);
  bool bFirstLoop = true;
  int myLayerType = -1;
  int myKey = 0;
  wxString myDescription = _T("");
  long myShortcutValues = 0;

  // clear shortcut from memory
  m_ShortcutMem.Clear();

  // load shortcuts from memory
  while (1) {
    if (!m_pDB->GetNextShortcutFull(bFirstLoop, myLayerType, myKey, myDescription, myShortcutValues)) {
      break;
    }
    bFirstLoop = false;

    // wxLogDebug(_T("Shortcut : %d, %s, %d "),
    //    myKey, myDescription.c_str(),myShortcutValues);

    m_ShortcutMem.AddShortcutMemory(myLayerType, myKey, myDescription, myShortcutValues);
  }

  return m_ShortcutMem.GetCount();
}

/***************************************************************************/ /**
  @brief Create an object of #tmAttributionData type
  @details This function may be used for getting a valid pointer on one of the
  #tmAttributionData derived class.
  @param type one of the following :
  - TOC_NAME_LINES
  - TOC_NAME_LABELS
  - TOC_NAME_POINTS
  - TOC_NAME_ANNOTATION
  @return  A valid pointer of type #tmAttributionData class
  @author Lucien Schreiber (c) CREALP 2008
  @date 06 November 2008
  *******************************************************************************/
tmAttributionData *tmAttributionManager::CreateAttributionData(int type) {
  tmAttributionData *myAttrib = NULL;
  switch (type) {
    case TOC_NAME_LINES:
      myAttrib = new tmAttributionDataLine();
      break;

    case TOC_NAME_POINTS:
      myAttrib = new tmAttributionDataPoint();
      break;

    case TOC_NAME_LABELS:
      myAttrib = new tmAttributionDataLabel();
      break;

    default:
      myAttrib = new tmAttributionData();
      break;
  }

  if (myAttrib == NULL) {
    wxLogDebug(_T("Error creating attribution object"));
  }

  return myAttrib;
}

/***************************************************************************/ /**
  @brief Copy attribution between objects
  @details This event function may be called for copying attributions between
  objects.
  @param event event should contain following informations :
  - event.GetExtraLong() the ID of the object we copy attribution from
  - event.GetClientData() (wxArrayLong*) the ID of objects we copy attribution to.
  @author Lucien Schreiber (c) CREALP 2009
  @date 16 February 2009
  *******************************************************************************/
void tmAttributionManager::OnCopyAttribution(wxCommandEvent &event) {
  tmAttributionData *myAttrib = CreateAttributionData(m_TOC->GetEditLayer()->GetType());
  if (myAttrib == NULL) return;

  // init
  wxArrayLong *mySelectedValues = (wxArrayLong *)event.GetClientData();
  myAttrib->Create(mySelectedValues, m_pDB);
  myAttrib->SetDataBaseTable(TABLE_NAME_GIS_ATTRIBUTION[m_TOC->GetEditLayer()->GetType()]);

  // copy attribution basic
  myAttrib->CopyAttributesBasic(event.GetExtraLong());

  // copy attribution advanced
  tmLayerValueArray myLayersID;
  if (!myAttrib->GetAttributionLayersID(event.GetExtraLong(), myLayersID)) {
    return;
  }

  wxASSERT(m_pPrjMem);
  PrjMemLayersArray myLayersInfoArray;
  for (unsigned int i = 0; i < myLayersID.GetCount(); i++) {
    wxLogDebug(_T("layer %ld searched"), myLayersID.Item(i).m_Oid);
    ProjectDefMemoryLayers *myActualLayer = m_pPrjMem->FindLayerByRealID(myLayersID.Item(i).m_Oid);
    if (myActualLayer) {
      myLayersInfoArray.Add(new ProjectDefMemoryLayers());
      *(myLayersInfoArray.Item(myLayersInfoArray.GetCount() - 1)) = *myActualLayer;
    } else
      wxLogDebug(_T("Layers %ld not found "), myLayersID.Item(i).m_Oid);
  }

  // get advanced attribution values (if existing)
  wxArrayString myValues;
  bool bGetAAttrib = myAttrib->GetAttributesAdvanced(event.GetExtraLong(), &myLayersInfoArray, myValues);
  if (!bGetAAttrib) {
    // wxLogError(_T("Problem getting advanced attribution values"));
    wxDELETE(mySelectedValues);
    wxDELETE(myAttrib);
    return;
  }

  wxLogMessage("Getting values for item : %ld", event.GetExtraLong());
  for (unsigned int i = 0; i < myValues.GetCount(); i++) {
    wxLogMessage(myValues.Item(i));
  }

  for (unsigned int i = 0; i < mySelectedValues->GetCount(); i++) {
    bool bClean =
        myAttrib->CleanAttributesAdvanced(mySelectedValues->Item(i), m_pPrjMem, myLayersInfoArray.Item(0)->m_LayerType);
    bool bAttrib = myAttrib->SetAttributesAdvanced(mySelectedValues->Item(i), &myLayersInfoArray, myValues);
    if (!bClean || !bAttrib) {
      wxLogError("Error setting advanced attribution for object id: %ld!", mySelectedValues->Item(i));
    }
  }

  wxDELETE(mySelectedValues);
  wxDELETE(myAttrib);
}

/***************************************************************************/ /**
  @brief Received when user select run Query
  @details This function receive the query from the #Queries_PANEL. The query may
  be retreived from GetString() function
  @author Lucien Schreiber (c) CREALP 2008
  @date 10 November 2008
  *******************************************************************************/
void tmAttributionManager::OnRunQuery(wxCommandEvent &event) {
  wxASSERT(m_pDB);
  wxASSERT(m_TOC);
  wxASSERT(m_SelData);

  wxLogDebug(_T("Running query"));

  // getting the query
  wxString myQuery = event.GetString();

  // getting the layer ID
  int myLayerType = event.GetInt();
  int myLayerID = m_pDB->GetLayerID(static_cast<TOC_GENERIC_NAME>(myLayerType));
  if (myLayerID == wxNOT_FOUND) {
    wxASSERT_MSG(0, _T("Error getting layer ID"));
    return;
  }
  // Selecting layer in TOC
  m_TOC->SetSelectedLayer(myLayerID);

  // passing the query
  if (!m_pDB->DataBaseQuery(myQuery)) {
    wxString szError = _("Error running the query. See the log window for a more detailled explaintion");
    wxMessageBox(szError, _("Error running the query"), wxICON_EXCLAMATION | wxOK, m_Parent);
    return;
  }

  // clear selection only if query is correct
  m_SelData->Clear();

  // query succeed, getting results
  wxArrayLong myResults;
  if (m_pDB->DataBaseGetResults(myResults)) {
    m_SelData->SetLayerID(myLayerID);
    m_SelData->AddSelected(&myResults);
  }

  // update display
  wxCommandEvent evt(tmEVT_LM_UPDATE, wxID_ANY);
  m_Parent->GetEventHandler()->QueueEvent(evt.Clone());

  // update selecction / attribution panel
  wxCommandEvent evt2(tmEVT_SELECTION_DONE, wxID_ANY);
  m_Parent->GetEventHandler()->QueueEvent(evt2.Clone());
}

// TODO: Add comment here
int tmAttributionManager::DisplayAAttributionWindow(wxArrayString *values, PrjMemLayersArray *layers,
                                                    const tmLayerValueArray &arrayidname) {
  tmAAttribWindow myAADlg(m_Parent, layers, values, arrayidname, wxID_ANY);
  return myAADlg.ShowModal();
}

/***************************************************************************/ /**
  @brief Advanced Attribution button is pressed
  @return  true if the advanced attribution succeed, false if cancelled or if an
  error occur
  @author Lucien Schreiber (c) CREALP 2009
  @date 11 March 2009
  *******************************************************************************/
bool tmAttributionManager::AAttributionButtonShow() {
  // validating
  if (!IsAttributionManagerReady()) {
    wxLogError(_("Project isn't ready, open a project or define a Edit Layer"));
    return false;
  }
  if (!IsOnlyOneObjSelected()) {
    wxLogError(_("This works only if one object is selected, select only one object"));
    return false;
  }
  tmLayerProperties *myEditLayer = m_TOC->GetEditLayer();
  if (myEditLayer == NULL) {
    wxLogError(_("You aren't in edit mode, please start editing"));
    return false;
  }

  tmAttributionData *myAttribObj = CreateAttributionData(myEditLayer->GetType());
  if (myAttribObj == NULL) return false;
  myAttribObj->Create(m_SelData->GetSelectedValues(), m_pDB);

  // attributed layers
  // wxArrayLong myLayersID;
  tmLayerValueArray myLayersID;
  if (!myAttribObj->GetAttributionLayersID(m_SelData->GetSelectedUnique(), myLayersID)) return false;

  // get layers info
  wxASSERT(m_pPrjMem);
  PrjMemLayersArray myLayersInfoArray;
  for (unsigned int i = 0; i < myLayersID.GetCount(); i++) {
    wxLogDebug(_T("layer %ld searched"), myLayersID.Item(i).m_Oid);

    ProjectDefMemoryLayers *myActualLayer = m_pPrjMem->FindLayerByRealID(myLayersID.Item(i).m_Oid);
    if (myActualLayer) {
      myLayersInfoArray.Add(new ProjectDefMemoryLayers());
      *(myLayersInfoArray.Item(myLayersInfoArray.GetCount() - 1)) = *myActualLayer;
    } else
      wxLogDebug(_T("Layers %ld not found "), myLayersID.Item(i).m_Oid);
  }

  // get advanced attribution values (if existing)
  wxArrayString myValues;
  bool bGetAAttrib = myAttribObj->GetAttributesAdvanced(m_SelData->GetSelectedUnique(), &myLayersInfoArray, myValues);
  if (!bGetAAttrib) {
    wxLogDebug(_T("Problem getting advanced attribution values"));
  }

  if (DisplayAAttributionWindow(&myValues, &myLayersInfoArray, myLayersID) == wxID_OK) {
    bool bClean = myAttribObj->CleanAttributesAdvanced(m_SelData->GetSelectedUnique(), m_pPrjMem,
                                                       myLayersInfoArray.Item(0)->m_LayerType);
    bool bAttrib = myAttribObj->SetAttributesAdvanced(m_SelData->GetSelectedUnique(), &myLayersInfoArray, myValues);
    if (bClean && bAttrib) return true;
    return false;
  }

  return true;
}

void tmAttributionManager::AAttributionBatchShow() {
  // validating
  if (!IsAttributionManagerReady()) {
    wxLogError(_("Project isn't ready, open a project or define a Edit Layer"));
    return;
  }

  tmLayerProperties *myEditLayer = m_TOC->GetEditLayer();
  if (myEditLayer == NULL) {
    wxLogError(_("You aren't in edit mode, please start editing"));
    return;
  }

  wxASSERT(m_pPrjMem);
  wxASSERT(m_SelData);
  tmAAttribBatchManager myBatchManager(m_pPrjMem, m_pDB, m_SelData, (PRJDEF_LAYERS_TYPE)myEditLayer->GetType());
  if (!myBatchManager.IsOk()) {
    wxLogError(_("Batch attribution isn't possible"));
    return;
  }

  AAttribBatch_DLG myDlg(m_Parent, &myBatchManager);
  myDlg.ShowModal();
}

/***************************************************************************/ /**
  @brief Show the information dialog if not visible
  @details If the informations windows was allready visible then we don't do
  anything
  @author Lucien Schreiber (c) CREALP 2009
  @date 07 April 2009
  *******************************************************************************/
void tmAttributionManager::DisplayInformationsWnd() {
  // search info dialog
  m_InfoDLG = (InformationDLG *)wxWindow::FindWindowById(ID_INFORMATION_DLG);
  if (m_InfoDLG == NULL) {
    m_InfoDLG = new InformationDLG(m_Parent, m_TOC, m_SelData, ID_INFORMATION_DLG);
    m_InfoDLG->SetProject(m_pPrjMem);
    m_InfoDLG->Show();
  } else {
    m_InfoDLG->Raise();
  }
}

/***************************************************************************/ /**
  @brief Called when layer selection change in the TOC
  @details Update the information window if visible
  @author Lucien Schreiber (c) CREALP 2009
  @date 07 April 2009
  *******************************************************************************/
void tmAttributionManager::OnLayerChanged(wxCommandEvent &event) {
  m_InfoDLG = (InformationDLG *)wxWindow::FindWindowById(ID_INFORMATION_DLG);
  if (m_InfoDLG != NULL) {
    m_InfoDLG->UpdateLayer();
  }
}

void tmAttributionManager::OnSelectionChanged(wxCommandEvent &event) {
  m_InfoDLG = (InformationDLG *)wxWindow::FindWindowById(ID_INFORMATION_DLG);
  if (m_InfoDLG != NULL) {
    m_InfoDLG->UpdateSelection();
  }
}
