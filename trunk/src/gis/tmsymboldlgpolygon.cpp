/***************************************************************************
								tmsymboldlgpolygon.cpp
						Display Polygon Symbology dialog
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

#include "tmsymboldlgpolygon.h"
#include "tmlayerproperties.h"
#include "tmgisdatavectorshp.h"
#include "../core/datalistreportctrl.h"


IMPLEMENT_DYNAMIC_CLASS( tmSymbolDLGPolygon, tmSymbolDLG )



tmSymbolDLGPolygon::tmSymbolDLGPolygon()
{
    _Init();
}



tmSymbolDLGPolygon::tmSymbolDLGPolygon( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    _Init();
    Create(parent, id, caption, pos, size, style);
}



bool tmSymbolDLGPolygon::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    tmSymbolDLG::Create( parent, id, caption, pos, size, style );
	    
	CreateControlsPoly();
	
	return true;
}




tmSymbolDLGPolygon::~tmSymbolDLGPolygon()
{

}



void tmSymbolDLGPolygon::_Init()
{
	m_PolygonBorderColourCtrl	= NULL;
	m_PolygonFillColourCtrl		= NULL;
	m_PolygonBorderWidthCtrl	= NULL;
	m_PolygonFillPattern		= NULL;
	m_TransparencySlider		= NULL;
}



void tmSymbolDLGPolygon::CreateControlsPoly()
{
    wxBoxSizer* itemBoxSizer8 = new wxBoxSizer(wxVERTICAL);
    m_SymbolPanel->SetSizer(itemBoxSizer8);
	
    wxFlexGridSizer* itemFlexGridSizer11 = new wxFlexGridSizer(4, 2, 0, 0);
    itemFlexGridSizer11->AddGrowableCol(1);
	
    wxStaticText* itemStaticText12 = new wxStaticText( m_SymbolPanel, wxID_STATIC, _("Border Color :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer11->Add(itemStaticText12, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    m_PolygonBorderColourCtrl = new wxColourPickerCtrl(m_SymbolPanel, ID_SYMDLGPLG_BORDER_COLOR);
    itemFlexGridSizer11->Add(m_PolygonBorderColourCtrl, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    wxStaticText* itemStaticText14 = new wxStaticText( m_SymbolPanel, wxID_STATIC, _("Border Width :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer11->Add(itemStaticText14, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    m_PolygonBorderWidthCtrl = new wxSpinCtrl( m_SymbolPanel, ID_SYMDLGPLG_BORDER_WIDTH, _T("0"),
											  wxDefaultPosition, wxDefaultSize, 
											  wxSP_ARROW_KEYS, 0, 100, 0 );
    itemFlexGridSizer11->Add(m_PolygonBorderWidthCtrl, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    wxStaticText* itemStaticText16 = new wxStaticText( m_SymbolPanel, wxID_STATIC, _("Fill Color :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer11->Add(itemStaticText16, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    m_PolygonFillColourCtrl = new wxColourPickerCtrl( m_SymbolPanel, ID_SYMDLGPLG_FILL_COLOR);
    itemFlexGridSizer11->Add(m_PolygonFillColourCtrl, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    wxStaticText* itemStaticText18 = new wxStaticText( m_SymbolPanel, wxID_STATIC, _("Fill style :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer11->Add(itemStaticText18, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
	m_PolygonFillPattern = new wxChoice( m_SymbolPanel, ID_SYMDLGPLG_FILL_PATTERN, 
										wxDefaultPosition, wxDefaultSize, 
										(sizeof(tmSYMBOLFILLSTYLES_NAME) / sizeof (wxString)),
										tmSYMBOLFILLSTYLES_NAME, 0 );
    itemFlexGridSizer11->Add(m_PolygonFillPattern, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    itemBoxSizer8->Add(itemFlexGridSizer11, 1, wxGROW|wxALL, 5);
	
    wxStaticBox* itemStaticBoxSizer26Static = new wxStaticBox(m_SymbolPanel, wxID_ANY, _("Transparency"));
    wxStaticBoxSizer* itemStaticBoxSizer26 = new wxStaticBoxSizer(itemStaticBoxSizer26Static, wxHORIZONTAL);
    itemBoxSizer8->Add(itemStaticBoxSizer26, 0, wxGROW|wxALL, 5);
    
	m_TransparencySlider = new tmSliderWithText(m_SymbolPanel, ID_SYMDLGPLG_TRANSPARENCY,
												0,0,100, _T("%"));
    itemStaticBoxSizer26->Add(m_TransparencySlider, 1, wxGROW|wxALL, 5);
	
	// adapt dialog size to new controls added
	SetSizeHint();
}


bool tmSymbolDLGPolygon::TransferDataToWindow()
{
	m_PolygonBorderColourCtrl->SetColour(m_DlgData.m_bColour);
	m_PolygonBorderWidthCtrl->SetValue(m_DlgData.m_bWidth);
	m_PolygonFillColourCtrl->SetColour(m_DlgData.m_fColour);
	m_PolygonFillPattern->SetSelection(m_DlgData.m_fStyle);
	m_TransparencySlider->SetValue(m_DlgData.m_GlobalTransparency);
	return TRUE;
}



bool tmSymbolDLGPolygon::TransferDataFromWindow()
{
	m_DlgData.m_bColour = m_PolygonBorderColourCtrl->GetColour();
	m_DlgData.m_bWidth = m_PolygonBorderWidthCtrl->GetValue();
	m_DlgData.m_fColour = m_PolygonFillColourCtrl->GetColour();
	m_DlgData.m_fStyle = m_PolygonFillPattern->GetSelection();
	m_DlgData.m_GlobalTransparency = m_TransparencySlider->GetValue();
	return TRUE;
}





/*************************************************************************************//**
Symbology dialog supporting rules
*****************************************************************************************/
BEGIN_EVENT_TABLE(tmSymbolDLGPolyRule, tmSymbolDLG)
EVT_BUTTON(ID_BTN_CLASSIFY, tmSymbolDLGPolyRule::OnBtnClassify)
EVT_BUTTON(ID_BTN_ADD, tmSymbolDLGPolyRule::OnBtnAdd)
EVT_BUTTON(ID_BTN_REMOVE, tmSymbolDLGPolyRule::OnBtnRemove)
EVT_BUTTON(ID_BTN_REMOVEALL, tmSymbolDLGPolyRule::OnBtnRemoveAll)
END_EVENT_TABLE()



void tmSymbolDLGPolyRule::_CreateControls() {
    this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* itemBoxSizer8;
	itemBoxSizer8 = new wxBoxSizer( wxVERTICAL );
	
	m_SymbologyTypeCtrl = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	wxPanel* m_panel2;
	m_panel2 = new wxPanel( m_SymbologyTypeCtrl, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxVERTICAL );
	
	m_panel2->SetSizer( bSizer4 );
	m_panel2->Layout();
	bSizer4->Fit( m_panel2 );
	m_SymbologyTypeCtrl->AddPage( m_panel2, _("Unique"), false );
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
	
	//m_SymbolListCtrl = new wxListCtrl( m_panel1, wxID_ANY, wxDefaultPosition, , wxLC_REPORT );
    m_SymbolListCtrl = new DataListReportCtrl(m_panel1, wxID_ANY, wxDefaultPosition, wxSize( 300,200 ) ,wxLC_REPORT | wxLC_HRULES | wxLC_VRULES);
	bSizer2->Add( m_SymbolListCtrl, 1, wxEXPAND|wxALL, 5 );
	
	wxBoxSizer* bSizer17;
	bSizer17 = new wxBoxSizer( wxHORIZONTAL );
	
	m_ClassifyBtn = new wxButton( m_panel1, ID_BTN_CLASSIFY, _("Classify"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer17->Add( m_ClassifyBtn, 0, wxALL, 5 );
	
	m_AddBtn = new wxButton( m_panel1, ID_BTN_ADD, _("Add"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer17->Add( m_AddBtn, 0, wxALL, 5 );
	
	m_RemoveBtn = new wxButton( m_panel1, ID_BTN_REMOVE, _("Remove"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer17->Add( m_RemoveBtn, 0, wxALL, 5 );
	
	m_RemoveAllBtn = new wxButton( m_panel1, ID_BTN_REMOVEALL, _("Remove all"), wxDefaultPosition, wxDefaultSize, 0 );
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



void tmSymbolDLGPolyRule::_LoadTableData() {
    wxASSERT(m_SymbolListCtrl);
    wxWindowUpdateLocker noUpdates(m_SymbolListCtrl);
    m_SymbolListCtrl->DeleteAllItems();
    
    for (unsigned int i  = 0; i< m_Rules.GetCount(); i++) {
        long myListIndex = m_SymbolListCtrl->InsertItem(m_SymbolListCtrl->GetItemCount(), _T(""));
        m_SymbolListCtrl->SetItemImage(myListIndex, i);
        m_SymbolListCtrl->SetText(myListIndex, m_Rules[i]->GetRuleName(), 1);
        m_SymbolListCtrl->SetText(myListIndex, m_Rules[i]->GetAttributFilter(), 2);
    }
}



wxBitmap tmSymbolDLGPolyRule::_CreateColorBitmap(const wxBrush & brush, const wxPen & pen) {
    wxBitmap myTestBmp (16,16);
    {
        wxMemoryDC renderer_dc;
        renderer_dc.SelectObject(myTestBmp);
        renderer_dc.SetPen(pen);
        renderer_dc.SetBrush(brush);
        renderer_dc.DrawRectangle(0,0,16,16);
    }
    return myTestBmp;
}



void tmSymbolDLGPolyRule::OnBtnClassify(wxCommandEvent & event) {
    // clear rules
    tmSymbolRuleArrayClear(&m_Rules);
    m_ImgList->RemoveAll();

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
        myRule->SetAttributFilter(wxString::Format(_T("%s='%s'"),myFieldName, myFieldValues[i]));
        myRule->SetRandomColor();
        m_ImgList->Add(_CreateColorBitmap(myRule->GetBrush(), myRule->GetPen()));
        m_Rules.Add(myRule);
    }
    _LoadTableData();
}



void tmSymbolDLGPolyRule::OnBtnAdd(wxCommandEvent & event) {
}



void tmSymbolDLGPolyRule::OnBtnRemove(wxCommandEvent & event) {
}



void tmSymbolDLGPolyRule::OnBtnRemoveAll(wxCommandEvent & event) {
}





bool tmSymbolDLGPolyRule::TransferDataToWindow() {
    _LoadTableData();
    return true;
}



bool tmSymbolDLGPolyRule::TransferDataFromWindow() {
    return true;
}



tmSymbolDLGPolyRule::tmSymbolDLGPolyRule(wxWindow * parent, tmLayerProperties * layerproperties, wxWindowID id, const wxString & caption, const wxPoint & pos, const wxSize & size, long style){
    m_LayerProperties = layerproperties;
    m_GISData = (tmGISDataVectorSHP*) tmGISData::LoadLayer(m_LayerProperties);
    tmSymbolDLGPolyRule::Create(parent, id, caption, pos, size, style);
}



tmSymbolDLGPolyRule::~tmSymbolDLGPolyRule() {
    unsigned int iCount = m_Rules.GetCount();
    for (unsigned int i = 0; i< iCount; i++) {
        tmSymbolRule * myRule =  m_Rules.Item(0);
        wxDELETE(myRule);
        m_Rules.RemoveAt(0);
    }
    wxDELETE(m_ImgList);
}



bool tmSymbolDLGPolyRule::Create(wxWindow * parent, wxWindowID id, const wxString & caption, const wxPoint & pos, const wxSize & size, long style) {
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    tmSymbolDLG::Create( parent, id, caption, pos, size, style );
	_CreateControls();
    return true;
}




