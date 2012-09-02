/***************************************************************************
 tmsymboldlgline.cpp
 Display Line Symbology dialog
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


#include "tmsymboldlgline.h"
#include "tmlayerproperties.h"
#include "tmgisdatavectorshp.h"
#include "../core/datalistreportctrl.h"




/*************************************************************************************//**
Symbology dialog for lines supporting rules
*****************************************************************************************/
BEGIN_EVENT_TABLE(tmSymbolDLGLineRule, tmSymbolDLG)
EVT_BUTTON(ID_BTN_CLASSIFY_LINE, tmSymbolDLGLineRule::OnBtnClassify)
EVT_BUTTON(ID_BTN_ADD_LINE, tmSymbolDLGLineRule::OnBtnAdd)
EVT_BUTTON(ID_BTN_REMOVE_LINE, tmSymbolDLGLineRule::OnBtnRemove)
EVT_BUTTON(ID_BTN_REMOVEALL_LINE, tmSymbolDLGLineRule::OnBtnRemoveAll)
EVT_UPDATE_UI(ID_BTN_REMOVE_LINE, tmSymbolDLGLineRule::OnUpdateUIBtnRemove)
EVT_UPDATE_UI(ID_BTN_REMOVEALL_LINE, tmSymbolDLGLineRule::OnUpdateUIBtnRemoveAll)
EVT_LIST_ITEM_ACTIVATED(ID_LIST_SYMBOL_LINE, tmSymbolDLGLineRule::OnDoubleClick)
END_EVENT_TABLE()


void tmSymbolDLGLineRule::_CreateControls() {
    this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* itemBoxSizer8;
	itemBoxSizer8 = new wxBoxSizer( wxVERTICAL );
	
	m_SymbologyTypeCtrl = new wxNotebook( m_SymbolPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	
    // UNIQUE
    wxPanel* m_panel2;
	m_panel2 = new wxPanel( m_SymbologyTypeCtrl, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxVERTICAL );
	
    
    wxFlexGridSizer* itemFlexGridSizer11 = new wxFlexGridSizer(3, 2, 0, 0);
    itemFlexGridSizer11->AddGrowableCol(1);
 	
    wxStaticText* itemStaticText12 = new wxStaticText( m_panel2, wxID_STATIC, _("Color :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer11->Add(itemStaticText12, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    m_LineColourCtrl = new wxColourPickerCtrl(m_panel2, ID_SYMDLGL_COLOR);
	itemFlexGridSizer11->Add(m_LineColourCtrl, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
	
    wxStaticText* itemStaticText14 = new wxStaticText( m_panel2, wxID_STATIC, _("Symbol :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer11->Add(itemStaticText14, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    m_LinePatternCtrl = new wxChoice( m_panel2, ID_SYMDLGL_PATTERN, wxDefaultPosition, wxDefaultSize,
                                     (sizeof(tmSYMBOLPENSTYLES_NAME) / sizeof(wxString)),
									 tmSYMBOLPENSTYLES_NAME, 0 );
    itemFlexGridSizer11->Add(m_LinePatternCtrl, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    wxStaticText* itemStaticText16 = new wxStaticText( m_panel2, wxID_STATIC, _("Width :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer11->Add(itemStaticText16, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    m_LineWidthCtrl = new wxSpinCtrl( m_panel2, ID_SYMDLGL_WIDTH, _T("0"),
                                     wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS,
                                     0, 100, 1 );
    itemFlexGridSizer11->Add(m_LineWidthCtrl, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);
    bSizer4->Add(itemFlexGridSizer11, 1, wxGROW|wxALL, 5);
	
    wxStaticBox* itemStaticBoxSizer24Static = new wxStaticBox(m_panel2, wxID_ANY, _("Transparency"));
    wxStaticBoxSizer* itemStaticBoxSizer24 = new wxStaticBoxSizer(itemStaticBoxSizer24Static, wxHORIZONTAL);
    bSizer4->Add(itemStaticBoxSizer24, 0, wxGROW|wxALL, 5);
    
	m_TransparencySlider = new tmSliderWithText(m_panel2, ID_SYMDLGL_TRANSPARENCY,0,0,100,_T("%"),
                                                wxDefaultPosition, wxDefaultSize, 0);
	itemStaticBoxSizer24->Add(m_TransparencySlider, 1, wxGROW|wxALL, 5);
	
	// adapt dialog size to new controls added
	SetSizeHint();

    
    
    
    
	m_panel2->SetSizer( bSizer4 );
	m_panel2->Layout();
	bSizer4->Fit( m_panel2 );
	m_SymbologyTypeCtrl->AddPage( m_panel2, _("Unique"), false );
	
    
    // MULTIPLE
    wxPanel* m_panel1;
	m_panel1 = new wxPanel( m_SymbologyTypeCtrl, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* sbSizer10;
	sbSizer10 = new wxStaticBoxSizer( new wxStaticBox( m_panel1, wxID_ANY, _("Category:") ), wxHORIZONTAL );
	
	wxStaticText* m_staticText25;
	m_staticText25 = new wxStaticText( m_panel1, wxID_ANY, _("Column:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText25->Wrap( -1 );
	sbSizer10->Add( m_staticText25, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
    // get fields name
    wxASSERT(m_GISData);
    wxArrayString myFieldsName;
    m_GISData->GetFieldsName(myFieldsName);
    
    
	m_CategoryColumnCtrl = new wxChoice( m_panel1, wxID_ANY, wxDefaultPosition, wxDefaultSize, myFieldsName, 0 );
	m_CategoryColumnCtrl->SetSelection( 0 );
	sbSizer10->Add( m_CategoryColumnCtrl, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 5 );
	
	bSizer2->Add( sbSizer10, 0, wxEXPAND|wxALL, 5 );
	
    m_SymbolListCtrl = new DataListReportCtrl(m_panel1, ID_LIST_SYMBOL_LINE, wxDefaultPosition, wxSize( 300,200 ) ,wxLC_REPORT | wxLC_HRULES | wxLC_VRULES);
	bSizer2->Add( m_SymbolListCtrl, 1, wxEXPAND|wxALL, 5 );
	
	wxBoxSizer* bSizer17;
	bSizer17 = new wxBoxSizer( wxHORIZONTAL );
	
	m_ClassifyBtn = new wxButton( m_panel1, ID_BTN_CLASSIFY_LINE, _("Classify"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer17->Add( m_ClassifyBtn, 0, wxALL, 5 );
	
	m_AddBtn = new wxButton( m_panel1, ID_BTN_ADD_LINE, _("Add"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer17->Add( m_AddBtn, 0, wxALL, 5 );
	
	m_RemoveBtn = new wxButton( m_panel1, ID_BTN_REMOVE_LINE, _("Remove"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer17->Add( m_RemoveBtn, 0, wxALL, 5 );
	
	m_RemoveAllBtn = new wxButton( m_panel1, ID_BTN_REMOVEALL_LINE, _("Remove all"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer17->Add( m_RemoveAllBtn, 0, wxALL, 5 );
	
	bSizer2->Add( bSizer17, 0, wxEXPAND, 5 );
	
	m_panel1->SetSizer( bSizer2 );
	m_panel1->Layout();
	bSizer2->Fit( m_panel1 );
	m_SymbologyTypeCtrl->AddPage( m_panel1, _("Multiple"), false );
	
	itemBoxSizer8->Add( m_SymbologyTypeCtrl, 1, wxEXPAND | wxALL, 5 );
	
    m_SymbolPanel->SetSizer(itemBoxSizer8);
    SetSizeHint();
    
    
    // create table columns
    m_SymbolListCtrl->InsertColumn(0, "");
    m_SymbolListCtrl->InsertColumn(1, _("Name"));
    m_SymbolListCtrl->InsertColumn(2, _("Query"));
    
    wxSize mySize = m_SymbolListCtrl->GetSize();
    int myColSize = (mySize.GetWidth() -26) / 2.0;
    if (myColSize < 50) {
        myColSize = 50;
    }
    
    m_SymbolListCtrl->SetColumnWidth(0, 26);
    m_SymbolListCtrl->SetColumnWidth(1, myColSize);
    m_SymbolListCtrl->SetColumnWidth(2, myColSize);
    
    m_ImgList= new wxImageList(16,16);
    m_SymbolListCtrl->SetImageList(m_ImgList, wxIMAGE_LIST_SMALL);
}

void tmSymbolDLGLineRule::_LoadTableData() {
    wxASSERT(m_SymbolListCtrl);
    wxWindowUpdateLocker noUpdates(m_SymbolListCtrl);
    m_SymbolListCtrl->DeleteAllItems();
    m_ImgList->RemoveAll();
    
    
    for (unsigned int i  = 0; i< m_Rules.GetCount(); i++) {
        long myListIndex = m_SymbolListCtrl->InsertItem(m_SymbolListCtrl->GetItemCount(), _T(""));
        m_ImgList->Add(_CreateColorBitmap(m_Rules[i]->GetBrush(), m_Rules[i]->GetPen()));
        m_SymbolListCtrl->SetItemImage(myListIndex, i);
        if (m_Rules[i]->IsActive() == false) {
            m_SymbolListCtrl->SetItemTextColour(myListIndex, *wxLIGHT_GREY);
        }
        else{
            m_SymbolListCtrl->SetItemTextColour(myListIndex, *wxBLACK);
        }
        m_SymbolListCtrl->SetText(myListIndex, m_Rules[i]->GetRuleName(), 1);
        m_SymbolListCtrl->SetText(myListIndex, m_Rules[i]->GetAttributFilter(), 2);
    }
}



wxBitmap tmSymbolDLGLineRule::_CreateColorBitmap(const wxBrush & brush, const wxPen & pen) {
    wxBitmap myTestBmp (16,16);
    {
        wxMemoryDC renderer_dc;
        renderer_dc.SelectObject(myTestBmp);
        wxBrush myBackBrush(m_SymbolListCtrl->GetBackgroundColour());
        renderer_dc.SetBackground(myBackBrush);
        renderer_dc.Clear();
        
        wxPen myPen (pen);
        myPen.SetWidth(3);
        renderer_dc.SetPen(myPen);
        renderer_dc.DrawLine(0, 8, 16, 8);
    }
    return myTestBmp;
}



void tmSymbolDLGLineRule::OnBtnClassify(wxCommandEvent & event) {
    // clear rules
    tmSymbolRuleManager::RuleArrayClear(&m_Rules);
    
    wxArrayString myFieldValues;
    wxASSERT(m_GISData);
    wxString myFieldName = m_CategoryColumnCtrl->GetString(m_CategoryColumnCtrl->GetSelection());
    if (m_GISData->GetDistinctFieldsValue(myFieldName, myFieldValues)==false) {
        wxLogError(_("Unable to get fields values for '%s'"), myFieldName);
        return;
    }
    
    for (unsigned int i = 0; i< myFieldValues.GetCount(); i++) {
        tmSymbolRule * myRule = new tmSymbolRule(m_LayerProperties->GetSpatialType(), NULL);
        myRule->SetRuleName(myFieldValues[i]);
        myRule->SetAttributFilter(wxString::Format(_T("%s=\"%s\""),myFieldName, myFieldValues[i]));
        myRule->SetRandomColor();
        m_Rules.Add(myRule);
    }
    _LoadTableData();
}



void tmSymbolDLGLineRule::OnBtnAdd(wxCommandEvent & event) {
    tmSymbolRule * myRule = new tmSymbolRule(m_LayerProperties->GetSpatialType(), NULL);
    myRule->SetRandomColor();
    tmSymbolRuleEdit_DLG myDlg(this, myRule);
    if (myDlg.ShowModal() != wxID_OK) {
        wxDELETE(myRule);
        return;
    }
    
    *myRule = *(myDlg.GetRule());
    m_Rules.Add(myRule);
    _LoadTableData();
}



void tmSymbolDLGLineRule::OnBtnRemove(wxCommandEvent & event) {
    wxASSERT(m_SymbolListCtrl);
    long myIndex = m_SymbolListCtrl->GetSelectedFirst();
    wxASSERT(myIndex >= 0 && myIndex < m_Rules.GetCount());
    tmSymbolRule * myRule = m_Rules[myIndex];
    wxDELETE(myRule);
    m_Rules.RemoveAt(myIndex);
    m_SymbolListCtrl->DeleteItem(myIndex);
}



void tmSymbolDLGLineRule::OnBtnRemoveAll(wxCommandEvent & event) {
    tmSymbolRuleManager::RuleArrayClear(GetRulesRef());
    _LoadTableData();
}



void tmSymbolDLGLineRule::OnDoubleClick(wxListEvent & event) {
    long myItemIndex = event.GetIndex();
    tmSymbolRule * myRule = m_Rules[myItemIndex];
    wxASSERT(myRule);
    tmSymbolRuleEdit_DLG myDlg(this, myRule);
    if (myDlg.ShowModal() != wxID_OK) {
        return;
    }
    
    *myRule = *(myDlg.GetRule());
    _LoadTableData();
}



void tmSymbolDLGLineRule::OnUpdateUIBtnRemove(wxUpdateUIEvent & event) {
    if (m_SymbolListCtrl && m_SymbolListCtrl->GetSelectedFirst() != wxNOT_FOUND) {
        event.Enable(true);
        return;
    }
    event.Enable(false);
}



void tmSymbolDLGLineRule::OnUpdateUIBtnRemoveAll(wxUpdateUIEvent & event) {
    if (m_SymbolListCtrl && m_SymbolListCtrl->GetItemCount() > 0) {
        event.Enable(true);
        return;
    }
    event.Enable(false);
}



bool tmSymbolDLGLineRule::TransferDataToWindow() {
    _LoadTableData();
    m_SymbologyTypeCtrl->SetSelection(m_LineUniqueStyle.m_PanelNo);
    wxArrayString myFieldsString = m_CategoryColumnCtrl->GetStrings();
    for (unsigned int i = 0; i< myFieldsString.GetCount(); i++) {
        if (myFieldsString[i] == GetSelectedField()) {
            m_CategoryColumnCtrl->SetSelection(i);
            break;
        }
    }
    
    // transfert unique symbology
    m_LineColourCtrl->SetColour(m_LineUniqueStyle.m_Colour);
	m_LineWidthCtrl->SetValue(m_LineUniqueStyle.m_Width);
	m_LinePatternCtrl->SetSelection(m_LineUniqueStyle.m_Shape);
	m_TransparencySlider->SetValue(m_LineUniqueStyle.m_GlobalTransparency);
    return true;
}



bool tmSymbolDLGLineRule::TransferDataFromWindow() {
    SetSelectedPanel(m_SymbologyTypeCtrl->GetSelection());
    SetSelectedField(m_CategoryColumnCtrl->GetString(m_CategoryColumnCtrl->GetSelection()));
    
    // transfert unique
    m_LineUniqueStyle.m_Colour = m_LineColourCtrl->GetColour();
	m_LineUniqueStyle.m_Width = m_LineWidthCtrl->GetValue();
	m_LineUniqueStyle.m_Shape = m_LinePatternCtrl->GetSelection();
	m_LineUniqueStyle.m_GlobalTransparency = m_TransparencySlider->GetValue();
    return true;
}



tmSymbolDLGLineRule::tmSymbolDLGLineRule(wxWindow * parent, tmLayerProperties * layerproperties, wxWindowID id, const wxString & caption, const wxPoint & pos, const wxSize & size, long style){
    m_LayerProperties = layerproperties;
    m_GISData = (tmGISDataVectorSHP*) tmGISData::LoadLayer(m_LayerProperties);
    tmSymbolDLGLineRule::Create(parent, id, caption, pos, size, style);
}


tmSymbolDLGLineRule::~tmSymbolDLGLineRule(){
    unsigned int iCount = m_Rules.GetCount();
    for (unsigned int i = 0; i< iCount; i++) {
        tmSymbolRule * myRule =  m_Rules.Item(0);
        wxDELETE(myRule);
        m_Rules.RemoveAt(0);
    }
    wxDELETE(m_ImgList);
}


bool tmSymbolDLGLineRule::Create(wxWindow * parent, wxWindowID id, const wxString & caption, const wxPoint & pos, const wxSize & size, long style) {
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    tmSymbolDLG::Create( parent, id, caption, pos, size, style );
	_CreateControls();
    return true;
}


int tmSymbolDLGLineRule::GetSelectedPanel() {
    return m_LineUniqueStyle.m_PanelNo;
}

void tmSymbolDLGLineRule::SetLineUniqueStyle(tmSymbolDataLineUnique value) {
    m_LineUniqueStyle = value;
}

void tmSymbolDLGLineRule::SetSelectedField(wxString value) {
    m_SelectedField = value;
}

void tmSymbolDLGLineRule::SetSelectedPanel(int panelindex) {
    m_LineUniqueStyle.m_PanelNo = panelindex;
}



