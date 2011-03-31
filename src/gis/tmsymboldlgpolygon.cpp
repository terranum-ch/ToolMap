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


