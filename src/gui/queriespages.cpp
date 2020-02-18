/***************************************************************************
 queriespages.cpp
 Pages definition for the QueryWizard
 -------------------
 copyright : (C) 2009 CREALP Lucien Schreiber
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "queriespages.h"

#include <wx/arrimpl.cpp>  // This is a magic incantation which must be done!

#include "../database/database_tm.h"
#include "../gui/wxflatbutton.h"
WX_DEFINE_OBJARRAY(wxArrayRadioButton);

QueriesPageIntro::QueriesPageIntro(QueriesWizard *parent, DataBaseTM *database) : wxWizardPage(parent) {
  wxASSERT(database);
  m_Parent = parent;
  m_pDB = database;
  CreateControls();

  m_PageName = new QueriesPageName(m_Parent, this, nullptr);
  m_PageSelectionAttribut1 = new QueriesPageAttribut1(m_Parent, m_pDB, nullptr, m_PageName);
  m_PageExpertSQL = new QueriesPageSQL(m_Parent);
  m_PageObject = new QueriesPageObject(m_Parent, m_pDB, nullptr, nullptr);

  m_PageLayer = new QueriesPageLayer(m_Parent, m_pDB, this, m_PageName);
  m_PageGeneric = new QueriesPageGeneric(m_Parent, this, m_PageName);
  m_PageSelection = new QueriesPageSelection(m_Parent, m_pDB, this, m_PageSelectionAttribut1);
  m_PageExpert = new QueriesPageExpert(m_Parent, this, m_PageExpertSQL);
  m_PageObjectType = new QueriesPageObjectType(m_Parent, m_pDB, this, m_PageObject);

  m_PageGeomLine = new QueriesPageGeomLine(m_Parent, m_pDB, this, m_PageName);
  m_PageGeomNode = new QueriesPageGeomNodes(m_Parent, m_pDB, this, m_PageName);
}

QueriesPageIntro::~QueriesPageIntro() {
  delete m_PageLayer;
  delete m_PageName;
  delete m_PageGeneric;
  delete m_PageSelection;
  delete m_PageSelectionAttribut1;
  delete m_PageExpert;
  delete m_PageObject;

  delete m_PageExpertSQL;
  delete m_PageObjectType;
  delete m_PageGeomLine;
  delete m_PageGeomNode;
}

wxWizardPage *QueriesPageIntro::GetPrev() const {
  return nullptr;
}

int QueriesPageIntro::_GetRadioBoxSelection() const {
  int iReturn = wxNOT_FOUND;
  for (int i = 0; i < QUERY_NUMBER; i++) {
    if (m_RadioBtn[i]->GetValue()) {
      iReturn = i;
      break;
    }
  }
  return iReturn;
}

wxWizardPage *QueriesPageIntro::GetNext() const {
  switch ((tmQUERIES_TYPE)_GetRadioBoxSelection()) {
    case QUERY_LAYERS:
      m_PageName->SetPrev(m_PageLayer);
      return m_PageLayer;
      break;

    case QUERY_GENERIC:
      m_PageName->SetPrev(m_PageGeneric);
      return m_PageGeneric;
      break;

    case QUERY_OBJECTS:
      m_PageName->SetPrev(m_PageObjectType);
      wxWizardPageSimple::Chain(m_PageObjectType, m_PageObject);
      wxWizardPageSimple::Chain(m_PageObject, m_PageName);
      return m_PageObjectType;
      break;

    case QUERY_SELECTED:
      m_PageName->SetPrev(m_PageSelectionAttribut1);
      wxWizardPageSimple::Chain(m_PageSelection, m_PageSelectionAttribut1);
      wxWizardPageSimple::Chain(m_PageSelectionAttribut1, m_PageName);
      return m_PageSelection;
      break;

    case QUERY_SQL:
      m_PageName->SetPrev(m_PageExpertSQL);
      wxWizardPageSimple::Chain(m_PageExpert, m_PageExpertSQL);
      wxWizardPageSimple::Chain(m_PageExpertSQL, m_PageName);
      return m_PageExpert;
      break;

    case QUERY_LINES:
      m_PageName->SetPrev(m_PageGeomLine);
      return m_PageGeomLine;
      break;

    case QUERY_NODES:
      m_PageName->SetPrev(m_PageGeomNode);
      return m_PageGeomNode;
      break;

    case QUERY_DUPLICATE:
    case QUERY_CROSSING:
      m_PageName->SetPrev(m_PageExpert);
      wxWizardPageSimple::Chain(m_PageExpert, m_PageName);
      return m_PageExpert;
      break;

    default:
      break;
  }
  return m_PageName;
}

void QueriesPageIntro::CreateControls() {
  wxBoxSizer *bSizer1;
  bSizer1 = new wxBoxSizer(wxVERTICAL);

  wxStaticText *m_staticText1;
  m_staticText1 = new wxStaticText(this, wxID_ANY, wxT("Choose which type of query you want to add"), wxDefaultPosition,
                                   wxDefaultSize, 0);
  m_staticText1->Wrap(-1);
  bSizer1->Add(m_staticText1, 0, wxALL, 5);

  wxBoxSizer *bSizer2;
  bSizer2 = new wxBoxSizer(wxVERTICAL);

  wxStaticBoxSizer *sbSizer2;
  sbSizer2 = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, _("Attribution based query")), wxVERTICAL);

  m_RadioBtn.Add(new wxRadioButton(this, wxID_ANY, _("by Layer"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP));
  sbSizer2->Add(m_RadioBtn[QUERY_LAYERS], 0, wxALL, 5);

  m_RadioBtn.Add(new wxRadioButton(this, wxID_ANY, _("by Object Kind"), wxDefaultPosition, wxDefaultSize, 0));
  sbSizer2->Add(m_RadioBtn[QUERY_OBJECTS], 0, wxALL, 5);

  m_RadioBtn.Add(new wxRadioButton(this, wxID_ANY, _("From selected feature"), wxDefaultPosition, wxDefaultSize, 0));
  sbSizer2->Add(m_RadioBtn[QUERY_SELECTED], 0, wxALL, 5);

  m_RadioBtn.Add(new wxRadioButton(this, wxID_ANY, _("Generic query"), wxDefaultPosition, wxDefaultSize, 0));
  sbSizer2->Add(m_RadioBtn[QUERY_GENERIC], 0, wxALL, 5);

  m_RadioBtn.Add(new wxRadioButton(this, wxID_ANY, _("SQL query (expert)"), wxDefaultPosition, wxDefaultSize, 0));
  sbSizer2->Add(m_RadioBtn[QUERY_SQL], 0, wxALL, 5);

  bSizer2->Add(sbSizer2, 0, wxALL | wxEXPAND, 5);

  bSizer2->Add(0, 5, 1, wxEXPAND, 5);

  wxStaticBoxSizer *sbSizer3;
  sbSizer3 = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, _("Geometry based query")), wxVERTICAL);

  m_RadioBtn.Add(new wxRadioButton(this, wxID_ANY, _("by Length"), wxDefaultPosition, wxDefaultSize, 0));
  sbSizer3->Add(m_RadioBtn[QUERY_LINES], 0, wxALL, 5);

  m_RadioBtn.Add(new wxRadioButton(this, wxID_ANY, _("by number of nodes"), wxDefaultPosition, wxDefaultSize, 0));
  sbSizer3->Add(m_RadioBtn[QUERY_NODES], 0, wxALL, 5);

  m_RadioBtn.Add(new wxRadioButton(this, wxID_ANY, _("Look for Duplicates")));
  sbSizer3->Add(m_RadioBtn[QUERY_DUPLICATE], 0, wxALL, 5);

  m_RadioBtn.Add(new wxRadioButton(this, wxID_ANY, _("Look for Crossing")));
  sbSizer3->Add(m_RadioBtn[QUERY_CROSSING], 0, wxALL, 5);

  bSizer2->Add(sbSizer3, 0, wxALL | wxEXPAND, 5);

  bSizer1->Add(bSizer2, 1, wxEXPAND, 10);

  SetSizer(bSizer1);
  bSizer1->Fit(this);
}

bool QueriesPageIntro::TransferDataToWindow() {
  m_RadioBtn[m_Parent->GetData()->m_QueryType]->SetValue(true);
  if (m_Parent->GetData()->m_QueryObjectGeomID == wxNOT_FOUND) {
    m_RadioBtn[QUERY_SELECTED]->Enable(false);
  }
  return true;
}

bool QueriesPageIntro::TransferDataFromWindow() {
  m_Parent->GetData()->m_QueryType = (tmQUERIES_TYPE)_GetRadioBoxSelection();
  return true;
}

QueriesLayerList::QueriesLayerList(wxWindow *parent, wxWindowID id, wxSize size) : ListGenReport(parent, id, size) {
  wxArrayString myColNames;
  wxArrayInt myColsWidths;

  myColNames.Add(_("Name"));
  myColNames.Add(_("Type"));

  myColsWidths.Add(200);
  myColsWidths.Add(90);

  CreateColumns(&myColNames, &myColsWidths);
}

QueriesLayerList::~QueriesLayerList() {}

void QueriesLayerList::SetSelection(long index, bool selected) {
  if (selected) {
    SetItemState(index, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
  } else {
    SetItemState(index, 0, wxLIST_STATE_SELECTED | wxLIST_STATE_FOCUSED);
  }
}

QueriesPageLayer::QueriesPageLayer(QueriesWizard *parent, DataBaseTM *database, wxWizardPage *prev, wxWizardPage *next)
    : wxWizardPageSimple(parent, prev, next) {
  wxASSERT(database);
  m_pDB = database;
  m_Parent = parent;
  wxASSERT(next);
  _CreateControls();
}

QueriesPageLayer::~QueriesPageLayer() {}

bool QueriesPageLayer::TransferDataToWindow() {
  if (m_ListLayers->GetItemCount() == 0) {
    unsigned int iLayer = m_Layers.GetCount();
    for (unsigned int i = 0; i < iLayer; i++) {
      ProjectDefMemoryLayers *myLayer = m_Layers.Item(0);
      wxDELETE(myLayer);
      m_Layers.RemoveAt(0);
    }

    if (m_Parent->GetData()->GetLayers(m_pDB, m_Layers)) {
      // create lines layers for all polygons layers
      // those layers are only used for creating queries
      unsigned int j = 0;
      while (1) {
        if (j >= m_Layers.GetCount()) {
          break;
        }
        if (m_Layers.Item(j)->m_LayerType == LAYER_POLYGON) {
          ProjectDefMemoryLayers *myLayer = new ProjectDefMemoryLayers();
          *myLayer = *(m_Layers.Item(j));
          myLayer->m_LayerType = LAYER_LINE;
          j++;
          m_Layers.Insert(myLayer, j);
        }

        j++;
      }

      m_ListLayers->Freeze();

      for (unsigned int i = 0; i < m_Layers.GetCount(); i++) {
        m_ListLayers->AddItemToList(m_Layers.Item(i)->m_LayerName);
        m_ListLayers->SetItemText(i, 1, TOC_GENERIC_NAME_STRING[m_Layers.Item(i)->m_LayerType]);
      }

      m_ListLayers->SetSelection(0, true);
      m_ListLayers->Thaw();
    } else {
      wxLogError(_T("Unable to get the layers list"));
    }
  }

  m_ListLayers->SetFocus();
  return true;
}

bool QueriesPageLayer::TransferDataFromWindow() {
  long iNum = m_ListLayers->GetSelectedItem();
  wxASSERT(iNum != wxNOT_FOUND);

  if (iNum < 0) return false;

  if (m_Layers.GetCount() > 0) {
    m_Parent->GetData()->m_QueryLayerID = m_Layers.Item(iNum)->m_LayerID;
    m_Parent->GetData()->m_QueryLayerType = (TOC_GENERIC_NAME)m_Layers.Item(iNum)->m_LayerType;

    m_Parent->GetData()->m_QueryName = _("Layer - ") + m_Layers.Item(iNum)->m_LayerName;
  }
  return true;
}

void QueriesPageLayer::_CreateControls() {
  wxBoxSizer *bSizer3;
  bSizer3 = new wxBoxSizer(wxVERTICAL);

  wxStaticText *m_staticText2;
  m_staticText2 = new wxStaticText(this, wxID_ANY, _("Select all features from "), wxDefaultPosition, wxDefaultSize, 0);
  m_staticText2->Wrap(m_Parent->GetSize().GetWidth() - QUERIES_MARGIN_SIZE);
  bSizer3->Add(m_staticText2, 0, wxALL, 5);

  m_ListLayers = new QueriesLayerList(this, wxID_ANY, wxDefaultSize);
  bSizer3->Add(m_ListLayers, 1, wxALL | wxEXPAND, 5);

  SetSizer(bSizer3);
  bSizer3->Fit(this);
}

/***************************************************************************/ /**
 Choose spatial type of Objects
 Lucien Schreiber (c) CREALP 2010
 21 janvier 2010
 *******************************************************************************/
void QueriesPageObjectType::_CreateControls() {
  wxBoxSizer *bSizer11;
  bSizer11 = new wxBoxSizer(wxVERTICAL);

  wxStaticText *m_staticText8;
  m_staticText8 = new wxStaticText(this, wxID_ANY, _("Choose the feature's type you want to select"), wxDefaultPosition,
                                   wxDefaultSize, 0);
  m_staticText8->Wrap(m_Parent->GetSize().GetWidth() - QUERIES_MARGIN_SIZE);
  bSizer11->Add(m_staticText8, 0, wxALL, 5);

  wxString m_LayerTypeChoices[] = {_("Lines"), _("Points"), _("Labels")};
  int m_LayerTypeNChoices = sizeof(m_LayerTypeChoices) / sizeof(wxString);
  m_LayerType = new wxRadioBox(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(150, -1), m_LayerTypeNChoices,
                               m_LayerTypeChoices, 1, wxRA_SPECIFY_COLS);
  m_LayerType->SetSelection(0);
  bSizer11->Add(m_LayerType, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);

  this->SetSizer(bSizer11);
  bSizer11->Fit(this);
}

QueriesPageObjectType::QueriesPageObjectType(QueriesWizard *parent, DataBaseTM *database, wxWizardPage *prev,
                                             wxWizardPage *next)
    : wxWizardPageSimple(parent, prev, next) {
  wxASSERT(database);
  m_pDB = database;
  m_Parent = parent;
  _CreateControls();
}

QueriesPageObjectType::~QueriesPageObjectType() {}

bool QueriesPageObjectType::TransferDataToWindow() {
  m_LayerType->SetSelection(m_Parent->GetData()->m_QueryLayerType);
  return true;
}

bool QueriesPageObjectType::TransferDataFromWindow() {
  int mySelection = m_LayerType->GetSelection();
  wxASSERT(mySelection >= TOC_NAME_LINES && mySelection <= TOC_NAME_LABELS);
  m_Parent->GetData()->m_QueryLayerType = (TOC_GENERIC_NAME)mySelection;
  return true;
}

/***************************************************************************/ /**
 Object Page
 Lucien Schreiber (c) CREALP 2010
 21 janvier 2010
 *******************************************************************************/
void QueriesPageObject::_CreateControls() {
  wxBoxSizer *bSizer11;
  bSizer11 = new wxBoxSizer(wxVERTICAL);

  wxStaticText *m_staticText8;
  m_staticText8 = new wxStaticText(this, wxID_ANY, _("Select object Kind"), wxDefaultPosition, wxDefaultSize, 0);
  m_staticText8->Wrap(m_Parent->GetSize().GetWidth() - QUERIES_MARGIN_SIZE);
  bSizer11->Add(m_staticText8, 0, wxALL, 5);

  m_ListType = new wxListBox(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, nullptr, wxLB_SINGLE);
  bSizer11->Add(m_ListType, 1, wxALL | wxEXPAND, 5);

  this->SetSizer(bSizer11);
  bSizer11->Fit(this);
}

QueriesPageObject::QueriesPageObject(QueriesWizard *parent, DataBaseTM *database, wxWizardPage *prev,
                                     wxWizardPage *next)
    : wxWizardPageSimple(parent, prev, next) {
  wxASSERT(database);
  m_pDB = database;
  m_Parent = parent;
  _CreateControls();
}

QueriesPageObject::~QueriesPageObject() {}

bool QueriesPageObject::TransferDataToWindow() {
  m_ListType->Freeze();
  m_ListType->Clear();

  if (!m_Parent->GetData()->GetObjectsForTypes(m_pDB, m_Objects)) {
    m_ListType->Thaw();
    return true;
  }

  for (unsigned int i = 0; i < m_Objects.GetCount(); i++) {
    m_ListType->Append(m_Objects.Item(i)->m_ObjectName);
  }

  m_ListType->SetSelection(0);
  m_ListType->Thaw();
  return true;
}

bool QueriesPageObject::TransferDataFromWindow() {
  if (m_ListType->GetCount() == 0) {
    return true;
  }

  int myIndex = m_ListType->GetSelection();
  wxASSERT(myIndex != wxNOT_FOUND);
  wxASSERT(m_Objects.GetCount() > 0);

  if (myIndex < 0) return false;

  m_Parent->GetData()->m_QueryObjectID = m_Objects.Item(myIndex)->m_ObjectID;

  m_Parent->GetData()->m_QueryName = _("Kind - ") + m_Objects.Item(myIndex)->m_ObjectName;

  return true;
}

void QueriesPageSelection::_CreateControls() {
  wxBoxSizer *bSizer10;
  bSizer10 = new wxBoxSizer(wxVERTICAL);

  wxStaticText *m_staticText7;
  m_staticText7 =
      new wxStaticText(this, wxID_ANY, _("Select feature by object Kind"), wxDefaultPosition, wxDefaultSize, 0);
  m_staticText7->Wrap(m_Parent->GetSize().GetWidth() - QUERIES_MARGIN_SIZE);
  bSizer10->Add(m_staticText7, 0, wxALL, 5);

  wxArrayString m_SelTypeListChoices;
  m_SelTypeList = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_SelTypeListChoices, 0);
  m_SelTypeList->SetSelection(0);
  bSizer10->Add(m_SelTypeList, 0, wxEXPAND | wxALL, 15);

  this->SetSizer(bSizer10);
  bSizer10->Fit(this);
}

QueriesPageSelection::QueriesPageSelection(QueriesWizard *parent, DataBaseTM *database, wxWizardPage *prev,
                                           wxWizardPageSimple *next)
    : wxWizardPageSimple(parent, prev, next) {
  wxASSERT(database);
  m_Parent = parent;
  m_pDB = database;
  _CreateControls();
}

QueriesPageSelection::~QueriesPageSelection() {}

bool QueriesPageSelection::TransferDataToWindow() {
  if (m_SelTypeList->IsEmpty()) {
    if (m_Parent->GetData()->GetObjectsForSelection(m_pDB, m_Types)) {
      m_SelTypeList->Freeze();
      for (unsigned int i = 0; i < m_Types.GetCount(); i++) {
        m_SelTypeList->Append(m_Types.Item(i)->m_ObjectName);
      }
      m_SelTypeList->SetSelection(0);
      m_SelTypeList->Thaw();
    }
  }
  m_SelTypeList->SetFocus();
  return true;
}

bool QueriesPageSelection::TransferDataFromWindow() {
  unsigned int mySel = m_SelTypeList->GetSelection();

  if (m_Types.GetCount() > 0) {
    wxASSERT(mySel < m_Types.GetCount());

    m_Parent->GetData()->m_QueryObjectID = m_Types.Item(mySel)->m_ObjectID;
  }

  return true;
}

/***************************************************************************/ /**
 Select advanced attribution type
 Lucien Schreiber (c) CREALP 2010
 27 janvier 2010
  *******************************************************************************/
void QueriesPageAttribut1::_CreateControls() {
  wxBoxSizer *bSizer10;
  bSizer10 = new wxBoxSizer(wxVERTICAL);

  wxStaticText *m_staticText7;
  m_staticText7 = new wxStaticText(this, wxID_ANY, _("Object attribute options"), wxDefaultPosition, wxDefaultSize, 0);
  m_staticText7->Wrap(m_Parent->GetSize().GetWidth() - QUERIES_MARGIN_SIZE);
  bSizer10->Add(m_staticText7, 0, wxALL, 5);

  wxString m_AdvAttribRadioChoices[] = {_("Don't use object attribute"), _("Select object with no object attribute"),
                                        _("Select object with object attribute")};
  int m_AdvAttribRadioNChoices = sizeof(m_AdvAttribRadioChoices) / sizeof(wxString);
  m_AdvAttribRadio = new wxRadioBox(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize,
                                    m_AdvAttribRadioNChoices, m_AdvAttribRadioChoices, 1, wxRA_SPECIFY_COLS);
  m_AdvAttribRadio->SetSelection(0);
  bSizer10->Add(m_AdvAttribRadio, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);

  this->SetSizer(bSizer10);
  bSizer10->Fit(this);
}

QueriesPageAttribut1::QueriesPageAttribut1(QueriesWizard *parent, DataBaseTM *database, wxWizardPage *prev,
                                           wxWizardPage *next)
    : wxWizardPageSimple(parent, prev, next) {
  m_Parent = parent;
  m_pDB = database;
  m_PageName = (wxWizardPageSimple *)next;
  m_QueryPageAttribut2 = new QueriesPageAttribut2(parent, database, nullptr, nullptr);

  _CreateControls();
}

QueriesPageAttribut1::~QueriesPageAttribut1() {
  delete m_QueryPageAttribut2;
}

bool QueriesPageAttribut1::TransferDataToWindow() {
  m_AdvAttribRadio->Enable(AATTRIBUTION_EMPTY, true);
  m_AdvAttribRadio->Enable(AATTRIBUTION_YES, true);

  // check if fields are present
  long myLayerID = wxNOT_FOUND;
  if (!m_Parent->GetData()->GetParentLayer(m_pDB, myLayerID)) {
    m_AdvAttribRadio->Enable(AATTRIBUTION_EMPTY, false);
    m_AdvAttribRadio->Enable(AATTRIBUTION_YES, false);
    return true;
  }

  if (!m_Parent->GetData()->GetFieldsValues(m_pDB, myLayerID, m_Parent->GetData()->m_QueryFields,
                                           m_Parent->GetData()->m_QueryFieldsValues)) {
    // disabling for security
    wxLogError(_T("Problem getting fields value for query"));
    m_AdvAttribRadio->Enable(AATTRIBUTION_EMPTY, false);
    m_AdvAttribRadio->Enable(AATTRIBUTION_YES, false);
    return true;
  }

  if (!m_Parent->GetData()->HasFieldsValues()) {
    m_AdvAttribRadio->Enable(AATTRIBUTION_YES, false);
  }

  return true;
}

bool QueriesPageAttribut1::TransferDataFromWindow() {
  m_Parent->GetData()->m_QueryFieldsStatus = (tmQUERIES_AATTRIBUTION_TYPE)m_AdvAttribRadio->GetSelection();

  if (m_AdvAttribRadio->GetSelection() == 2) {  // advanced attribution
    m_PageName->SetPrev(m_QueryPageAttribut2);
    wxWizardPageSimple::Chain(this, m_QueryPageAttribut2);
    wxWizardPageSimple::Chain(m_QueryPageAttribut2, m_PageName);
  } else {
    m_PageName->SetPrev(this);
    wxWizardPageSimple::Chain(this, m_PageName);
  }

  return true;
}

void QueriesPageAttribut2::_CreateControls() {
  wxBoxSizer *bSizer12;
  bSizer12 = new wxBoxSizer(wxVERTICAL);

  wxStaticText *m_AdvText;
  m_AdvText =
      new wxStaticText(this, wxID_ANY, _("If needed, delete unwanted criteria"), wxDefaultPosition, wxDefaultSize, 0);
  m_AdvText->Wrap(m_Parent->GetSize().GetWidth() - QUERIES_MARGIN_SIZE);
  bSizer12->Add(m_AdvText, 0, wxALL, 5);

  wxStaticBoxSizer *m_AdvSizer;
  m_AdvSizer = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, _("Object attribute")), wxVERTICAL);

  m_AdvAttributs = new wxListBox(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, 0);
  m_AdvSizer->Add(m_AdvAttributs, 1, wxALL | wxEXPAND, 5);

  m_ReloadButton = new wxFlatButton(this, wxID_ANY, _("Reload"), wxDefaultSize);
  m_AdvSizer->Add(m_ReloadButton, 0, wxALL | wxALIGN_RIGHT, 5);

  bSizer12->Add(m_AdvSizer, 1, wxEXPAND | wxALL, 5);

  this->SetSizer(bSizer12);

  bSizer12->Fit(this);
}

void QueriesPageAttribut2::OnReloadAttributs(wxCommandEvent &event) {
  _LoadAttributs();
  event.Skip();
}

void QueriesPageAttribut2::_LoadAttributs() {
  wxASSERT(m_Parent);
  wxASSERT(m_pDB);

  m_AdvAttributs->Freeze();
  m_AdvAttributs->Clear();

  wxASSERT(m_Parent->GetData()->m_QueryLayerID != wxNOT_FOUND);
  if (!m_Parent->GetData()->GetFieldsValues(m_pDB, m_Parent->GetData()->m_QueryLayerID,
                                           m_Parent->GetData()->m_QueryFields,
                                           m_Parent->GetData()->m_QueryFieldsValues)) {
    m_AdvAttributs->Thaw();
    return;
  }

  // adding to list
  QueriesData *myData = m_Parent->GetData();
  wxASSERT(myData);
  wxString myText = wxEmptyString;

  for (unsigned int i = 0; i < myData->m_QueryFields.GetCount(); i++) {
    myText.Clear();
    myText.Append(myData->m_QueryFields.Item(i)->m_Fieldname);
    myText.Append(_T(" = "));
    if (myData->m_QueryFieldsValues.Item(i) == wxEmptyString) {
      myText.Append(_("Empty"));
    } else {
      myText.Append(myData->m_QueryFieldsValues.Item(i));
    }

    m_AdvAttributs->Append(myText);
  }

  m_AdvAttributs->Thaw();
}

void QueriesPageAttribut2::OnDeleteAttribut(wxKeyEvent &event) {
  if (event.GetKeyCode() == WXK_BACK || event.GetKeyCode() == WXK_DELETE) {
    if (m_AdvAttributs->GetSelection() == wxNOT_FOUND) {
      event.Skip();
      return;
    }

    if (!m_Parent->GetData()->DeleteFieldsValue(m_AdvAttributs->GetSelection())) {
      event.Skip();
      return;
    }

    m_AdvAttributs->Delete(m_AdvAttributs->GetSelection());
  }
  event.Skip();
}

QueriesPageAttribut2::QueriesPageAttribut2(QueriesWizard *parent, DataBaseTM *database, wxWizardPageSimple *prev,
                                           wxWizardPageSimple *next)
    : wxWizardPageSimple(parent, prev, next) {
  m_Parent = parent;
  m_pDB = database;

  _CreateControls();

  // Connect Events
  m_AdvAttributs->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(QueriesPageAttribut2::OnDeleteAttribut), NULL, this);
  m_ReloadButton->Connect(EVT_FLATBUTTON_CLICKED, wxCommandEventHandler(QueriesPageAttribut2::OnReloadAttributs), NULL,
                          this);
}

QueriesPageAttribut2::~QueriesPageAttribut2() {
  // Disconnect Events
  m_AdvAttributs->Disconnect(wxEVT_KEY_DOWN, wxKeyEventHandler(QueriesPageAttribut2::OnDeleteAttribut), NULL, this);
  m_ReloadButton->Disconnect(EVT_FLATBUTTON_CLICKED, wxCommandEventHandler(QueriesPageAttribut2::OnReloadAttributs),
                             NULL, this);
}

bool QueriesPageAttribut2::TransferDataToWindow() {
  _LoadAttributs();
  return true;
}

bool QueriesPageAttribut2::TransferDataFromWindow() {
  if (m_AdvAttributs->IsEmpty()) {
    m_Parent->GetData()->m_QueryFieldsStatus = AATTRIBUTION_EMPTY;
  }
  return true;
}

void QueriesPageGenericData::_Add(int layertarger, const wxString &description, const wxString &sql) {
  m_TargetLayer.Add(layertarger);
  m_Description.Add(description);
  m_SQL.Add(sql);
}

QueriesPageGenericData::QueriesPageGenericData() {
  _Add(TOC_NAME_LINES, _("Line(s) with no attribution"),
       _T("SELECT l.OBJECT_ID FROM generic_lines l WHERE l.OBJECT_ID NOT IN (SELECT generic_aat.OBJECT_GEOM_ID FROM ")
       _T("generic_aat)"));
  _Add(TOC_NAME_LINES, _("Line(s) with more than one attribution"),
       _T("SELECT l.OBJECT_ID FROM generic_lines l WHERE OBJECT_ID IN (SELECT OBJECT_GEOM_ID  FROM generic_aat GROUP ")
       _T("BY OBJECT_GEOM_ID HAVING COUNT(*) > 1)"));
  _Add(TOC_NAME_LINES, _("Line(s) with one attribution"),
       _T("SELECT l.OBJECT_ID FROM generic_lines l WHERE OBJECT_ID IN (SELECT OBJECT_GEOM_ID  FROM generic_aat GROUP ")
       _T("BY OBJECT_GEOM_ID HAVING COUNT(*) = 1)"));
  _Add(TOC_NAME_POINTS, _("Point(s) with no attribution"),
       _T("SELECT l.OBJECT_ID FROM generic_points l WHERE l.OBJECT_ID NOT IN (SELECT generic_pat.OBJECT_GEOM_ID FROM ")
       _T("generic_pat)"));
  _Add(TOC_NAME_POINTS, _("Point(s) with more than one attribution"),
       _T("SELECT l.OBJECT_ID FROM generic_points l WHERE OBJECT_ID IN (SELECT OBJECT_GEOM_ID  FROM generic_pat GROUP ")
       _T("BY OBJECT_GEOM_ID HAVING COUNT(*) > 1)"));
  _Add(TOC_NAME_POINTS, _("Point(s) with one attribution"),
       _T("SELECT l.OBJECT_ID FROM generic_points l WHERE OBJECT_ID IN (SELECT OBJECT_GEOM_ID  FROM generic_pat GROUP ")
       _T("BY OBJECT_GEOM_ID HAVING COUNT(*) = 1)"));
  _Add(TOC_NAME_LABELS, _("Label(s) with no attribution"),
       _T("SELECT l.OBJECT_ID FROM generic_labels l WHERE l.OBJECT_ID NOT IN (SELECT generic_lat.OBJECT_GEOM_ID FROM ")
       _T("generic_lat)"));
  _Add(TOC_NAME_LABELS, _("Label(s) with more than one attribution"),
       _T("SELECT l.OBJECT_ID FROM generic_labels l WHERE OBJECT_ID IN (SELECT OBJECT_GEOM_ID  FROM generic_lat GROUP ")
       _T("BY OBJECT_GEOM_ID HAVING COUNT(*) > 1)"));
  _Add(TOC_NAME_LABELS, _("Label(s) with one attribution"),
       _T("SELECT l.OBJECT_ID FROM generic_labels l WHERE OBJECT_ID IN (SELECT OBJECT_GEOM_ID  FROM generic_lat GROUP ")
       _T("BY OBJECT_GEOM_ID HAVING COUNT(*) = 1)"));
}

QueriesPageGenericData::~QueriesPageGenericData() {}

void QueriesPageGenericData::GetData(int index, int &layertarget, wxString &description, wxString &sql) {
  layertarget = m_TargetLayer.Item(index);
  description = m_Description.Item(index);
  sql = m_SQL.Item(index);
}

wxArrayString QueriesPageGenericData::GetDescription() {
  return m_Description;
}

void QueriesPageGeneric::_CreateControls() {
  wxBoxSizer *bSizer3;
  bSizer3 = new wxBoxSizer(wxVERTICAL);

  wxStaticText *m_staticText2;
  m_staticText2 = new wxStaticText(this, wxID_ANY, _("Choose the generic query you want to add"), wxDefaultPosition,
                                   wxDefaultSize, 0);
  m_staticText2->Wrap(m_Parent->GetSize().GetWidth() - QUERIES_MARGIN_SIZE);
  bSizer3->Add(m_staticText2, 0, wxALL, 5);

  m_ListGeneric = new wxListBox(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_SINGLE);
  bSizer3->Add(m_ListGeneric, 1, wxALL | wxEXPAND, 5);

  this->SetSizer(bSizer3);
  bSizer3->Fit(this);
}

QueriesPageGeneric::QueriesPageGeneric(QueriesWizard *parent, wxWizardPage *prev, wxWizardPage *next)
    : wxWizardPageSimple(parent, prev, next) {
  m_GenericData = nullptr;
  m_Parent = parent;
  _CreateControls();
}

QueriesPageGeneric::~QueriesPageGeneric() {}

bool QueriesPageGeneric::TransferDataToWindow() {
  m_GenericData = new QueriesPageGenericData();

  if (m_ListGeneric->IsEmpty()) {
    m_ListGeneric->Append(m_GenericData->GetDescription());
  }

  if (m_ListGeneric->GetSelection() == wxNOT_FOUND) {
    m_ListGeneric->SetSelection(0);
  }
  m_ListGeneric->SetFocus();

  return true;
}

bool QueriesPageGeneric::TransferDataFromWindow() {
  // get data back from generic list
  int iTarget = wxNOT_FOUND;
  wxString myDesc = wxEmptyString;
  wxString mySQL = wxEmptyString;

  int iSelected = m_ListGeneric->GetSelection();
  m_GenericData->GetData(iSelected, iTarget, myDesc, mySQL);

  // set data to the parent
  m_Parent->GetData()->m_QueryLayerType = (TOC_GENERIC_NAME)iTarget;
  m_Parent->GetData()->m_QuerySQL = mySQL;

  // propose a name if name is empty
  if (m_Parent->GetData()->m_QueryName.IsEmpty()) {
    m_Parent->GetData()->m_QueryName = myDesc;
  }

  delete m_GenericData;
  m_GenericData = nullptr;
  return true;
}

void QueriesPageExpert::_CreateControls() {
  wxBoxSizer *bSizer11;
  bSizer11 = new wxBoxSizer(wxVERTICAL);

  wxStaticText *m_staticText8;
  m_staticText8 = new wxStaticText(this, wxID_ANY, _("Choose on which build-up layer the query\nwill apply"),
                                   wxDefaultPosition, wxDefaultSize, 0);
  m_staticText8->Wrap(m_Parent->GetSize().GetWidth() - QUERIES_MARGIN_SIZE);
  bSizer11->Add(m_staticText8, 0, wxALL, 5);

#ifdef USE_NOTES
  wxString m_LayerTypeChoices[] = {_("Lines"), _("Points"), _("Labels"), _("Notes")};
#else
  wxString m_LayerTypeChoices[] = {_("Lines"), _("Points"), _("Labels")};
#endif
  int m_LayerTypeNChoices = sizeof(m_LayerTypeChoices) / sizeof(wxString);
  m_LayerType = new wxRadioBox(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(150, -1), m_LayerTypeNChoices,
                               m_LayerTypeChoices, 1, wxRA_SPECIFY_COLS);
  m_LayerType->SetSelection(0);
  bSizer11->Add(m_LayerType, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);

  this->SetSizer(bSizer11);
  bSizer11->Fit(this);
}

QueriesPageExpert::QueriesPageExpert(QueriesWizard *parent, wxWizardPage *prev, wxWizardPageSimple *next)
    : wxWizardPageSimple(parent, prev, next) {
  m_Parent = parent;
  _CreateControls();
}

QueriesPageExpert::~QueriesPageExpert() {}

bool QueriesPageExpert::TransferDataToWindow() {
  return true;
}

bool QueriesPageExpert::TransferDataFromWindow() {
  m_Parent->GetData()->m_QueryLayerType = (TOC_GENERIC_NAME)m_LayerType->GetSelection();

  if (m_Parent->GetData()->m_QueryType == QUERY_DUPLICATE) {
    m_Parent->GetData()->m_QueryName =
        wxString::Format(_("(duplicate) -  %s"), TOC_GENERIC_NAME_STRING[m_LayerType->GetSelection()].c_str());
  }

  if (m_Parent->GetData()->m_QueryType == QUERY_CROSSING) {
    m_Parent->GetData()->m_QueryName =
        wxString::Format(_("(crossing) -  %s"), TOC_GENERIC_NAME_STRING[m_LayerType->GetSelection()].c_str());
  }
  return true;
}

void QueriesPageSQL::_CreateControls() {
  wxBoxSizer *bSizer9;
  bSizer9 = new wxBoxSizer(wxVERTICAL);

  wxStaticText *m_staticText8;
  m_staticText8 = new wxStaticText(this, wxID_ANY, _("Edit SQL code"), wxDefaultPosition, wxDefaultSize, 0);
  m_staticText8->Wrap(m_Parent->GetSize().GetWidth() - QUERIES_MARGIN_SIZE);
  bSizer9->Add(m_staticText8, 0, wxALL, 5);

  m_SQLText = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
  bSizer9->Add(m_SQLText, 1, wxALL | wxEXPAND, 5);

  this->SetSizer(bSizer9);
  bSizer9->Fit(this);
}

QueriesPageSQL::QueriesPageSQL(QueriesWizard *parent, wxWizardPageSimple *prev, wxWizardPageSimple *next)
    : wxWizardPageSimple(parent, prev, next) {
  m_Parent = parent;
  _CreateControls();
}

QueriesPageSQL::~QueriesPageSQL() {}

bool QueriesPageSQL::TransferDataToWindow() {
  return true;
}

bool QueriesPageSQL::TransferDataFromWindow() {
  m_Parent->GetData()->m_QuerySQL = m_SQLText->GetValue();
  return true;
}

/***************************************************************************/ /**
 Selecting small lines
 Lucien Schreiber (c) CREALP 2010
 27 janvier 2010
 *******************************************************************************/
void QueriesPageGeomLine::_CreateControls() {
  wxBoxSizer *bSizer15;
  bSizer15 = new wxBoxSizer(wxVERTICAL);

  wxStaticText *m_staticText13;
  m_staticText13 =
      new wxStaticText(this, wxID_ANY, _("Select lines shorter than (map units)"), wxDefaultPosition, wxDefaultSize, 0);
  m_staticText13->Wrap(m_Parent->GetSize().GetWidth() - QUERIES_MARGIN_SIZE);
  bSizer15->Add(m_staticText13, 0, wxALL, 5);

  m_SpinLineSize =
      new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 10000, 10);
  bSizer15->Add(m_SpinLineSize, 0, wxALL | wxEXPAND, 5);

  this->SetSizer(bSizer15);
  bSizer15->Fit(this);
}

QueriesPageGeomLine::QueriesPageGeomLine(QueriesWizard *parent, DataBaseTM *database, wxWizardPage *prev,
                                         wxWizardPage *next)
    : wxWizardPageSimple(parent, prev, next) {
  m_Parent = parent;
  m_pDB = database;
  _CreateControls();
}

QueriesPageGeomLine::~QueriesPageGeomLine() {}

bool QueriesPageGeomLine::TransferDataToWindow() {
  m_SpinLineSize->SetValue(m_Parent->GetData()->m_QueryLineSize);
  m_SpinLineSize->SetFocus();
  return true;
}

bool QueriesPageGeomLine::TransferDataFromWindow() {
  m_Parent->GetData()->m_QueryLineSize = m_SpinLineSize->GetValue();

  // offer a name
  m_Parent->GetData()->m_QueryName = wxString::Format(_("Geom - Lines shorter than %d"), m_SpinLineSize->GetValue());
  return true;
}

/***************************************************************************/ /**
 Selecting lines with few nodes
 Lucien Schreiber (c) CREALP 2010
 27 janvier 2010
 *******************************************************************************/
void QueriesPageGeomNodes::_CreateControls() {
  wxBoxSizer *bSizer16;
  bSizer16 = new wxBoxSizer(wxVERTICAL);

  wxStaticText *m_staticText14;
  m_staticText14 = new wxStaticText(this, wxID_ANY, _("Select lines with number of nodes less than"), wxDefaultPosition,
                                    wxDefaultSize, 0);
  m_staticText14->Wrap(-1);
  bSizer16->Add(m_staticText14, 0, wxALL, 5);

  m_SpinFewNodes =
      new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 2, 10000, 2);
  bSizer16->Add(m_SpinFewNodes, 0, wxALL | wxEXPAND, 5);

  this->SetSizer(bSizer16);
  bSizer16->Fit(this);
}

QueriesPageGeomNodes::QueriesPageGeomNodes(QueriesWizard *parent, DataBaseTM *database, wxWizardPage *prev,
                                           wxWizardPage *next)
    : wxWizardPageSimple(parent, prev, next) {
  m_Parent = parent;
  m_pDB = database;
  _CreateControls();
}

QueriesPageGeomNodes::~QueriesPageGeomNodes() {}

bool QueriesPageGeomNodes::TransferDataToWindow() {
  m_SpinFewNodes->SetValue(m_Parent->GetData()->m_QueryNodeNumber);
  m_SpinFewNodes->SetFocus();
  return true;
}

bool QueriesPageGeomNodes::TransferDataFromWindow() {
  m_Parent->GetData()->m_QueryNodeNumber = m_SpinFewNodes->GetValue();

  // offer a name
  m_Parent->GetData()->m_QueryName =
      wxString::Format(_("Geom - Lines with number of nodes less than %d"), m_SpinFewNodes->GetValue());

  return true;
}

QueriesPageName::QueriesPageName(QueriesWizard *parent, wxWizardPage *prev, wxWizardPage *next)
    : wxWizardPageSimple(parent, prev, next) {
  m_Parent = parent;
  _CreateControls();
}

QueriesPageName::~QueriesPageName() {}

void QueriesPageName::_CreateControls() {
  wxBoxSizer *bSizer4;
  bSizer4 = new wxBoxSizer(wxVERTICAL);

  wxStaticText *m_staticText3;
  m_staticText3 = new wxStaticText(this, wxID_ANY, _("Query name"), wxDefaultPosition, wxDefaultSize, 0);
  m_staticText3->Wrap(m_Parent->GetSize().GetWidth() - QUERIES_MARGIN_SIZE);
  bSizer4->Add(m_staticText3, 0, wxALL, 5);

  m_TextName = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
  m_TextName->SetMaxLength(200);
  bSizer4->Add(m_TextName, 0, wxALL | wxEXPAND, 5);

  m_RunQueryBox = new wxCheckBox(this, wxID_ANY, _("Run Query after Finish"), wxDefaultPosition, wxDefaultSize, 0);
  m_RunQueryBox->SetValue(true);

  bSizer4->Add(m_RunQueryBox, 0, wxALL, 5);

  this->SetSizer(bSizer4);
  bSizer4->Fit(this);
}

bool QueriesPageName::TransferDataToWindow() {
  m_TextName->SetValue(m_Parent->GetData()->m_QueryName);
  m_RunQueryBox->SetValue(m_Parent->GetData()->m_QueryRun);
  return true;
}

bool QueriesPageName::TransferDataFromWindow() {
  m_Parent->GetData()->m_QueryName = m_TextName->GetValue();
  m_Parent->GetData()->m_QueryRun = m_RunQueryBox->IsChecked();
  return true;
}
