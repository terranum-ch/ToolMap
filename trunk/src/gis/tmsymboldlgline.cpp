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

// comment doxygen

#include "tmsymboldlgline.h"


IMPLEMENT_DYNAMIC_CLASS( tmSymbolDLGLine, tmSymbolDLG )



BEGIN_EVENT_TABLE( tmSymbolDLGLine, tmSymbolDLG )
END_EVENT_TABLE()


tmSymbolDLGLine::tmSymbolDLGLine()
{
    _Init();
}



tmSymbolDLGLine::tmSymbolDLGLine( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    _Init();
    Create(parent, id, caption, pos, size, style);
}



bool tmSymbolDLGLine::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    tmSymbolDLG::Create( parent, id, caption, pos, size, style );
	    
	CreateControlsLine();
	
	return true;
}




tmSymbolDLGLine::~tmSymbolDLGLine()
{

}



void tmSymbolDLGLine::_Init()
{
	m_LineColourCtrl		= NULL;
	m_LineWidthCtrl			= NULL;
	m_LinePatternCtrl		= NULL;
	m_TransparencySlider	= NULL;
}



void tmSymbolDLGLine::CreateControlsLine()
{
    wxBoxSizer* itemBoxSizer8 = new wxBoxSizer(wxVERTICAL);
    m_SymbolPanel->SetSizer(itemBoxSizer8);
	
    wxFlexGridSizer* itemFlexGridSizer11 = new wxFlexGridSizer(3, 2, 0, 0);
    itemFlexGridSizer11->AddGrowableCol(1);
 	
    wxStaticText* itemStaticText12 = new wxStaticText( m_SymbolPanel, wxID_STATIC, _("Color :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer11->Add(itemStaticText12, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    m_LineColourCtrl = new wxColourPickerCtrl(m_SymbolPanel, ID_SYMDLGL_COLOR);
	itemFlexGridSizer11->Add(m_LineColourCtrl, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);													   
	
	
    wxStaticText* itemStaticText14 = new wxStaticText( m_SymbolPanel, wxID_STATIC, _("Symbol :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer11->Add(itemStaticText14, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    m_LinePatternCtrl = new wxChoice( m_SymbolPanel, ID_SYMDLGL_PATTERN, wxDefaultPosition, wxDefaultSize,
									  (sizeof(tmSYMBOLPENSTYLES_NAME) / sizeof(wxString)),
									 tmSYMBOLPENSTYLES_NAME, 0 );
    itemFlexGridSizer11->Add(m_LinePatternCtrl, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    wxStaticText* itemStaticText16 = new wxStaticText( m_SymbolPanel, wxID_STATIC, _("Width :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer11->Add(itemStaticText16, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    m_LineWidthCtrl = new wxSpinCtrl( m_SymbolPanel, ID_SYMDLGL_WIDTH, _T("0"),
												wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS,
												0, 100, 1 );
    itemFlexGridSizer11->Add(m_LineWidthCtrl, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);
    itemBoxSizer8->Add(itemFlexGridSizer11, 1, wxGROW|wxALL, 5);
	
    wxStaticBox* itemStaticBoxSizer24Static = new wxStaticBox(m_SymbolPanel, wxID_ANY, _("Transparency"));
    wxStaticBoxSizer* itemStaticBoxSizer24 = new wxStaticBoxSizer(itemStaticBoxSizer24Static, wxHORIZONTAL);
    itemBoxSizer8->Add(itemStaticBoxSizer24, 0, wxGROW|wxALL, 5);
  
	m_TransparencySlider = new tmSliderWithText(m_SymbolPanel, ID_SYMDLGL_TRANSPARENCY,0,0,100,_T("%"),
														   wxDefaultPosition, wxDefaultSize, 0);
	itemStaticBoxSizer24->Add(m_TransparencySlider, 1, wxGROW|wxALL, 5);
	
	// adapt dialog size to new controls added
	SetSizeHint();
}



bool tmSymbolDLGLine::TransferDataToWindow()
{
	m_LineColourCtrl->SetColour(m_DlgData.m_Colour);
	m_LinePatternCtrl->SetSelection(m_DlgData.m_Shape);
	m_LineWidthCtrl->SetValue(m_DlgData.m_Width);
	m_TransparencySlider->SetValue(m_DlgData.m_GlobalTransparency);
	return true;
}



bool tmSymbolDLGLine::TransferDataFromWindow()
{
	m_DlgData.m_Colour = m_LineColourCtrl->GetColour();
	m_DlgData.m_Shape = m_LinePatternCtrl->GetSelection();
	m_DlgData.m_Width = m_LineWidthCtrl->GetValue();
	m_DlgData.m_GlobalTransparency = m_TransparencySlider->GetValue();
	return true;
}



