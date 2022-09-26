/***************************************************************************
 tmsymbolrule.cpp
 -------------------
 copyright            : (C) 2012 CREALP Lucien Schreiber
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "tmsymbolrule.h"

#include <wx/arrimpl.cpp>

#include "tmgisdatavectorshp.h"
#include "tmlayerproperties.h"
#include "tmsymboldlg.h"
#include "tmsymbolvectorline.h"
#include "tmsymbolvectorpoint.h"
#include "tmsymbolvectorpolygon.h"
WX_DEFINE_OBJARRAY(tmSymbolRuleArray);

tmSymbolVector *tmSymbolRule::_NewSymbolVectorBasedOnSpatType(TM_GIS_SPATIAL_TYPES spatialtype) {
  tmSymbolVector *mySymbol = nullptr;
  switch (spatialtype) {
    case LAYER_SPATIAL_LINE:
      mySymbol = (tmSymbolVector *)new tmSymbolVectorLine();
      break;

    case LAYER_SPATIAL_POINT:
      mySymbol = (tmSymbolVector *)new tmSymbolVectorPoint();
      break;

    case LAYER_SPATIAL_POLYGON:
      mySymbol = (tmSymbolVector *)new tmSymbolVectorPolygon();
      break;

    default:
      wxLogError(_("Unsupported layer spatial type!"));
      break;
  }
  wxASSERT(mySymbol);
  return mySymbol;
}

tmSymbolRule::tmSymbolRule(TM_GIS_SPATIAL_TYPES spatialtype, tmSymbolVector *symbol) {
  SetRuleName(wxEmptyString);
  SetActive(true);
  SetAttributFilter(wxEmptyString);
  m_SpatialType = spatialtype;
  if (symbol != nullptr) {
    m_SymbolData = symbol;
  } else {
    m_SymbolData = _NewSymbolVectorBasedOnSpatType(GetSpatialType());
  }
  wxASSERT(m_SymbolData);
}

tmSymbolRule::~tmSymbolRule() {
  wxDELETE(m_SymbolData);
}

tmSymbolRule::tmSymbolRule(tmSymbolRule &source) {
  m_SpatialType = source.GetSpatialType();
  SetAttributFilter(source.GetAttributFilter());
  SetRuleName(source.GetRuleName());
  SetActive(source.IsActive());
  m_SymbolData = tmSymbolVector::CreateCopySymbolVectorBasedOnType(source.GetSpatialType(), TOC_NAME_NOT_GENERIC,
                                                                   (tmSymbol *)source.GetSymbolData());
  wxASSERT(m_SymbolData);
}

tmSymbolRule &tmSymbolRule::operator=(tmSymbolRule &source) {
  m_SpatialType = source.GetSpatialType();
  SetAttributFilter(source.GetAttributFilter());
  SetRuleName(source.GetRuleName());
  SetActive(source.IsActive());
  m_SymbolData = tmSymbolVector::CreateCopySymbolVectorBasedOnType(source.GetSpatialType(), TOC_NAME_NOT_GENERIC,
                                                                   (tmSymbol *)source.GetSymbolData());
  wxASSERT(m_SymbolData);
  return *this;
}

wxBrush tmSymbolRule::GetBrush() {
  wxBrush myBrush = *wxBLACK_BRUSH;
  switch (GetSpatialType()) {
    case LAYER_SPATIAL_LINE:
    case LAYER_SPATIAL_POINT:
      break;

    case LAYER_SPATIAL_POLYGON: {
      tmSymbolVectorPolygon *mySPoly = (tmSymbolVectorPolygon *)GetSymbolData();
      myBrush.SetColour(mySPoly->GetColourWithTransparency(mySPoly->GetFillColour(), mySPoly->GetTransparency()));
      myBrush.SetStyle(mySPoly->GetFillStyle());
    } break;

    default:
      wxLogError(_("spatial layer type isn't supported"));
      break;
  }
  return myBrush;
}

wxPen tmSymbolRule::GetPen() {
  wxPen myPen = *wxBLACK_PEN;
  switch (GetSpatialType()) {
    case LAYER_SPATIAL_LINE: {
      tmSymbolVectorLine *mySLine = (tmSymbolVectorLine *)GetSymbolData();
      myPen.SetWidth(mySLine->GetWidth());
      myPen.SetColour(mySLine->GetColourWithTransparency(mySLine->GetColour(), mySLine->GetTransparency()));
      if (mySLine->GetShape() == tmPENSTYLE_ORIENTED) {
        myPen.SetStyle(wxPENSTYLE_SOLID);
      } else {
        myPen.SetStyle((wxPenStyle)mySLine->GetShape());
      }
    } break;

    case LAYER_SPATIAL_POINT: {
      tmSymbolVectorPoint *mySPoint = (tmSymbolVectorPoint *)GetSymbolData();
      myPen.SetWidth(mySPoint->GetRadius());
      myPen.SetColour(mySPoint->GetColourWithTransparency(mySPoint->GetColour(), mySPoint->GetTransparency()));
    } break;

    case LAYER_SPATIAL_POLYGON: {
      tmSymbolVectorPolygon *mySPoly = (tmSymbolVectorPolygon *)GetSymbolData();
      myPen.SetWidth(mySPoly->GetBorderWidth());
      myPen.SetColour(mySPoly->GetColourWithTransparency(mySPoly->GetBorderColour(), mySPoly->GetTransparency()));
      if (mySPoly->GetBorderWidth() == 0) {
        myPen.SetStyle(wxPENSTYLE_TRANSPARENT);
      }
    } break;

    default:
      wxLogError(_("spatial layer type isn't supported"));
      break;
  }
  return myPen;
}

bool tmSymbolRule::IsOriented() {
  switch (GetSpatialType()) {
    case LAYER_SPATIAL_LINE: {
      tmSymbolVectorLine *mySLine = (tmSymbolVectorLine *)GetSymbolData();
      if (mySLine->GetShape() == tmPENSTYLE_ORIENTED) {
        return true;
      }
    } break;

    default:
      return false;
  }
  return false;
}

void tmSymbolRule::SetActive(bool value) {
  m_Active = value;
}

void tmSymbolRule::SetAttributFilter(wxString value) {
  m_AttributFilter = value;
}

void tmSymbolRule::SetRuleName(wxString value) {
  m_RuleName = value;
}

int tmSymbolRule::_GetRandomNumberForColor() {
  int output = 0 + (rand() % (int)(255 - 0 + 1));
  return output;
}

void tmSymbolRule::SetRandomColor() {
  wxASSERT(m_SymbolData);
  m_SymbolData->SetColour(wxColour(_GetRandomNumberForColor(), _GetRandomNumberForColor(), _GetRandomNumberForColor()));
}

void tmSymbolRule::InitRandomGenerator() {
  srand(wxDateTime::Now().GetTicks());
}

/***************************************************************************/ /**
 Symbol Rule manager
 *******************************************************************************/
tmSymbolRuleManager::tmSymbolRuleManager(tmLayerProperties *layerproperties) {
  m_DlgSelectedFieldname = wxEmptyString;
  m_DlgSelectedPanel = 0;
  m_LayerProperties = layerproperties;
  wxASSERT(m_LayerProperties);
}

tmSymbolRuleManager::~tmSymbolRuleManager() {
  tmSymbolRuleManager::RuleArrayClear(GetRulesRef());
}

void tmSymbolRuleManager::_CreateAttributIndex(tmLayerProperties *layerproperties) {}

bool tmSymbolRuleManager::ShowSymbolRuleDlg(wxWindow *parent, const wxPoint &position) {
  tmSymbolRule::InitRandomGenerator();
  switch (m_LayerProperties->GetSpatialType()) {
    case LAYER_SPATIAL_POLYGON: {
      tmSymbolDLGPolyRule *pdlg = new tmSymbolDLGPolyRule(parent, m_LayerProperties, SYMBOL_TMSYMBOLDLG_IDNAME,
                                                          SYMBOL_TMSYMBOLDLG_TITLE, position);
      tmSymbolRuleManager::RuleArrayCopy(GetRulesRef(), pdlg->GetRulesRef());

      tmSymbolVectorPolygon *mySymbolPoly = (tmSymbolVectorPolygon *)m_LayerProperties->GetSymbolRef();
      wxASSERT(mySymbolPoly);
      pdlg->SetPolyUniqueStyle(*(mySymbolPoly->GetSymbolData()));
      pdlg->SetSelectedPanel(m_DlgSelectedPanel);
      pdlg->SetSelectedField(m_DlgSelectedFieldname);

      if (pdlg->ShowModal() != wxID_OK) {
        wxDELETE(pdlg);
        return false;
      }
      tmSymbolRuleManager::RuleArrayCopy(pdlg->GetRulesRef(), GetRulesRef());
      m_DlgSelectedPanel = pdlg->GetSelectedPanel();
      m_DlgSelectedFieldname = pdlg->GetSelectedField();

      wxASSERT(mySymbolPoly);
      *(mySymbolPoly->GetSymbolData()) = pdlg->GetPolyUniqueStyle();

    } break;

    case LAYER_SPATIAL_LINE: {
      tmSymbolDLGLineRule *pdlg = new tmSymbolDLGLineRule(parent, m_LayerProperties, SYMBOL_TMSYMBOLDLG_IDNAME,
                                                          SYMBOL_TMSYMBOLDLG_TITLE, position);
      tmSymbolRuleManager::RuleArrayCopy(GetRulesRef(), pdlg->GetRulesRef());

      tmSymbolVectorLine *mySymbolLine = (tmSymbolVectorLine *)m_LayerProperties->GetSymbolRef();
      wxASSERT(mySymbolLine);
      pdlg->SetLineUniqueStyle(*(mySymbolLine->GetSymbolData()));
      pdlg->SetSelectedPanel(m_DlgSelectedPanel);
      pdlg->SetSelectedField(m_DlgSelectedFieldname);

      if (pdlg->ShowModal() != wxID_OK) {
        wxDELETE(pdlg);
        return false;
      }
      tmSymbolRuleManager::RuleArrayCopy(pdlg->GetRulesRef(), GetRulesRef());
      m_DlgSelectedPanel = pdlg->GetSelectedPanel();
      m_DlgSelectedFieldname = pdlg->GetSelectedField();

      wxASSERT(mySymbolLine);
      *(mySymbolLine->GetSymbolData()) = pdlg->GetLineUniqueStyle();

    } break;

    case LAYER_SPATIAL_POINT: {
      tmSymbolDLGPointRule *pdlg = new tmSymbolDLGPointRule(parent, m_LayerProperties, SYMBOL_TMSYMBOLDLG_IDNAME,
                                                            SYMBOL_TMSYMBOLDLG_TITLE, position);
      tmSymbolRuleManager::RuleArrayCopy(GetRulesRef(), pdlg->GetRulesRef());

      tmSymbolVectorPoint *mySymbolPoint = (tmSymbolVectorPoint *)m_LayerProperties->GetSymbolRef();
      wxASSERT(mySymbolPoint);
      pdlg->SetPointUniqueStyle(*(mySymbolPoint->GetSymbolData()));
      pdlg->SetSelectedPanel(m_DlgSelectedPanel);
      pdlg->SetSelectedField(m_DlgSelectedFieldname);

      if (pdlg->ShowModal() != wxID_OK) {
        wxDELETE(pdlg);
        return false;
      }
      tmSymbolRuleManager::RuleArrayCopy(pdlg->GetRulesRef(), GetRulesRef());
      m_DlgSelectedPanel = pdlg->GetSelectedPanel();
      m_DlgSelectedFieldname = pdlg->GetSelectedField();

      wxASSERT(mySymbolPoint);
      *(mySymbolPoint->GetSymbolData()) = pdlg->GetPointUniqueStyle();

    } break;

    default:
      wxLogError(_("Symbology dialog not implemented for this spatial type!"));
      break;
  }

  // create attribut index
  tmGISDataVectorSHP *myGISData = (tmGISDataVectorSHP *)tmGISData::LoadLayer(m_LayerProperties);
  wxString myQuery = wxString::Format(_T("DROP INDEX on %s"), m_LayerProperties->GetName().GetName());
  myGISData->ExecuteSQLQuery(myQuery);
  if (m_DlgSelectedFieldname != wxEmptyString && m_DlgSelectedPanel != 0) {
    myQuery = wxString::Format(_T("CREATE INDEX ON %s USING %s"), m_LayerProperties->GetName().GetName(),
                               m_DlgSelectedFieldname);
    myGISData->ExecuteSQLQuery(myQuery);
    wxLogMessage(_("Creating attribut filter for : %s"), m_DlgSelectedFieldname);
  }
  wxDELETE(myGISData);
  return true;
}

bool tmSymbolRuleManager::IsUsingRules() {
  if (m_DlgSelectedPanel == 1) {
    return true;
  }
  return false;
}

bool tmSymbolRuleManager::Serialize(tmSerialize &s) {
  // serialize basic symbology
  bool bReturn = m_LayerProperties->GetSymbolRef()->Serialize(s);
  if (m_LayerProperties->GetType() != TOC_NAME_SHP) {
    return bReturn;
  }

  // if needed, serialize rules
  if (!bReturn) {
    wxLogError(_("Error saving basic symbology"));
    return bReturn;
  }

  s.EnterObject();
  if (s.IsStoring()) {
    s << m_DlgSelectedPanel;
    s << m_DlgSelectedFieldname;
    s << (int)m_Rules.GetCount();
    for (unsigned int i = 0; i < m_Rules.GetCount(); i++) {
      s << m_Rules.Item(i)->GetRuleName();
      wxString myAttributFilter = m_Rules.Item(i)->GetAttributFilter();
      myAttributFilter.Replace(_T("\""), _T("\\\""));
      s << myAttributFilter;
      s << m_Rules.Item(i)->IsActive();
      m_Rules.Item(i)->GetSymbolData()->Serialize(s);
    }
  } else {
    s >> m_DlgSelectedPanel;
    s >> m_DlgSelectedFieldname;
    int myCount = 0;
    s >> myCount;
    m_Rules.Clear();
    for (int i = 0; i < myCount; i++) {
      tmSymbolRule *myRule = new tmSymbolRule(m_LayerProperties->GetSpatialType(), nullptr);
      wxString myRuleName;
      wxString myRuleFilter;
      bool myRuleActive;
      s >> myRuleName;
      s >> myRuleFilter;
      s >> myRuleActive;
      myRule->SetRuleName(myRuleName);
      myRule->SetAttributFilter(myRuleFilter);
      myRule->SetActive(myRuleActive);
      myRule->GetSymbolData()->Serialize(s);
      m_Rules.Add(myRule);
    }
  }
  s.LeaveObject();
  return s.IsOk();
}

void tmSymbolRuleManager::RuleArrayClear(tmSymbolRuleArray *rules) {
  wxASSERT(rules);
  unsigned int iCount = rules->GetCount();
  for (unsigned int i = 0; i < iCount; i++) {
    tmSymbolRule *myRule = rules->Item(0);
    wxDELETE(myRule);
    rules->RemoveAt(0);
  }
}

void tmSymbolRuleManager::RuleArrayCopy(const tmSymbolRuleArray *srcrules, tmSymbolRuleArray *dstrules) {
  wxASSERT(srcrules);
  wxASSERT(dstrules);
  tmSymbolRuleManager::RuleArrayClear(dstrules);
  for (unsigned int i = 0; i < srcrules->GetCount(); i++) {
    tmSymbolRule *myRule = new tmSymbolRule(srcrules->Item(i)->GetSpatialType(), nullptr);
    *myRule = *(srcrules->Item(i));
    dstrules->Add(myRule);
  }
  wxASSERT(dstrules->GetCount() == srcrules->GetCount());
}

tmSymbolRuleManager &tmSymbolRuleManager::operator=(const tmSymbolRuleManager &source) {
  m_DlgSelectedPanel = source.m_DlgSelectedPanel;
  m_DlgSelectedFieldname = source.m_DlgSelectedFieldname;
  tmSymbolRuleManager::RuleArrayCopy(&(source.m_Rules), &m_Rules);
  return *this;
}

/*************************************************************************************/ /**
  Edit rule symbology dialog
 *****************************************************************************************/
BEGIN_EVENT_TABLE(tmSymbolRuleEdit_DLG, wxDialog)
EVT_BUTTON(ID_SYMBOLEDIT_COLORCTRL, tmSymbolRuleEdit_DLG::OnSymbologyEdit)
END_EVENT_TABLE()

void tmSymbolRuleEdit_DLG::_CreateControls() {
  this->SetSizeHints(wxDefaultSize, wxDefaultSize);

  wxBoxSizer *bSizer13;
  bSizer13 = new wxBoxSizer(wxVERTICAL);

  wxFlexGridSizer *fgSizer5;
  fgSizer5 = new wxFlexGridSizer(4, 2, 0, 0);
  fgSizer5->AddGrowableCol(1);
  fgSizer5->SetFlexibleDirection(wxBOTH);
  fgSizer5->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

  wxStaticText *m_staticText14;
  m_staticText14 = new wxStaticText(this, wxID_ANY, _("Name:"), wxDefaultPosition, wxDefaultSize, 0);
  m_staticText14->Wrap(-1);
  fgSizer5->Add(m_staticText14, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

  m_NameCtrl = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
  fgSizer5->Add(m_NameCtrl, 0, wxALL | wxEXPAND, 5);

  wxStaticText *m_staticText15;
  m_staticText15 = new wxStaticText(this, wxID_ANY, _("Query:"), wxDefaultPosition, wxDefaultSize, 0);
  m_staticText15->Wrap(-1);
  fgSizer5->Add(m_staticText15, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

  m_AttributeCtrl = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(250, -1), 0);
  fgSizer5->Add(m_AttributeCtrl, 0, wxALL | wxEXPAND, 5);

  fgSizer5->Add(0, 0, 1, wxEXPAND, 5);

  m_ColourCtrl =
      new wxButton(this, ID_SYMBOLEDIT_COLORCTRL, _("Edit Symbology..."), wxDefaultPosition, wxDefaultSize, 0);
  fgSizer5->Add(m_ColourCtrl, 0, wxALL, 5);

  fgSizer5->Add(0, 0, 1, wxEXPAND, 5);

  m_EnabledCtrl = new wxCheckBox(this, wxID_ANY, _("Enabled"), wxDefaultPosition, wxDefaultSize, 0);
  fgSizer5->Add(m_EnabledCtrl, 0, wxALL, 5);

  bSizer13->Add(fgSizer5, 1, wxEXPAND | wxALL, 5);

  bSizer13->Add(0, 0, 0, wxEXPAND, 5);

  wxStdDialogButtonSizer *m_sdbSizer2;
  wxButton *m_sdbSizer2OK;
  wxButton *m_sdbSizer2Cancel;
  m_sdbSizer2 = new wxStdDialogButtonSizer();
  m_sdbSizer2OK = new wxButton(this, wxID_OK);
  m_sdbSizer2->AddButton(m_sdbSizer2OK);
  m_sdbSizer2Cancel = new wxButton(this, wxID_CANCEL);
  m_sdbSizer2->AddButton(m_sdbSizer2Cancel);
  m_sdbSizer2->Realize();
  bSizer13->Add(m_sdbSizer2, 0, wxEXPAND | wxALL, 5);

  this->SetSizer(bSizer13);
  this->Layout();
  bSizer13->Fit(this);

  this->Centre(wxBOTH);
}

void tmSymbolRuleEdit_DLG::OnSymbologyEdit(wxCommandEvent &event) {
  wxASSERT(m_Rule);
  m_Rule->GetSymbolData()->ShowSymbologyDialog(this);
}

bool tmSymbolRuleEdit_DLG::TransferDataFromWindow() {
  m_Rule->SetRuleName(m_NameCtrl->GetValue());
  m_Rule->SetAttributFilter(m_AttributeCtrl->GetValue());
  m_Rule->SetActive(m_EnabledCtrl->GetValue());
  return true;
}

bool tmSymbolRuleEdit_DLG::TransferDataToWindow() {
  m_NameCtrl->SetValue(m_Rule->GetRuleName());
  m_AttributeCtrl->SetValue(m_Rule->GetAttributFilter());
  m_EnabledCtrl->SetValue(m_Rule->IsActive());
  return true;
}

tmSymbolRuleEdit_DLG::tmSymbolRuleEdit_DLG(wxWindow *parent, tmSymbolRule *rule, wxWindowID id, const wxString &caption,
                                           const wxPoint &pos, const wxSize &size)
    : wxDialog(parent, id, caption, pos, size) {
  m_Rule = new tmSymbolRule(*rule);
  _CreateControls();
}

tmSymbolRuleEdit_DLG::~tmSymbolRuleEdit_DLG() {
  wxDELETE(m_Rule);
}
