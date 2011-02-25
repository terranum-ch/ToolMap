/***************************************************************************
								tmsymboldlgpoint.cpp
						Display Point Symbology dialog
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

#include "tmsymboldlgpoint.h"


IMPLEMENT_DYNAMIC_CLASS( tmSymbolDLGPoint, tmSymbolDLG )



BEGIN_EVENT_TABLE( tmSymbolDLGPoint, tmSymbolDLG )
END_EVENT_TABLE()


tmSymbolDLGPoint::tmSymbolDLGPoint()
{
    _Init();
}



tmSymbolDLGPoint::tmSymbolDLGPoint( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    _Init();
    Create(parent, id, caption, pos, size, style);
}



bool tmSymbolDLGPoint::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    tmSymbolDLG::Create( parent, id, caption, pos, size, style );
	    
	CreateControlsPoint();
	return true;
}




tmSymbolDLGPoint::~tmSymbolDLGPoint()
{

}



void tmSymbolDLGPoint::_Init()
{
	m_PointColourCtrl		= NULL;
	m_PointWidthCtrl		= NULL;
	m_TransparencySlider	= NULL;
}



void tmSymbolDLGPoint::CreateControlsPoint()
{
    wxBoxSizer* itemBoxSizer8 = new wxBoxSizer(wxVERTICAL);
    m_SymbolPanel->SetSizer(itemBoxSizer8);
	
    wxFlexGridSizer* itemFlexGridSizer11 = new wxFlexGridSizer(2, 2, 0, 0);
    itemFlexGridSizer11->AddGrowableCol(1);
	
    wxStaticText* itemStaticText12 = new wxStaticText( m_SymbolPanel, wxID_STATIC, _("Color :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer11->Add(itemStaticText12, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    m_PointColourCtrl = new wxColourPickerCtrl(m_SymbolPanel, ID_SYMDLGP_COLOR);
    itemFlexGridSizer11->Add(m_PointColourCtrl, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    wxStaticText* itemStaticText14 = new wxStaticText( m_SymbolPanel, wxID_STATIC, _("Radius :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer11->Add(itemStaticText14, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    m_PointWidthCtrl = new wxSpinCtrl( m_SymbolPanel, ID_SYMDLGP_WIDTH, _T("0"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100, 0 );
    itemFlexGridSizer11->Add(m_PointWidthCtrl, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    itemBoxSizer8->Add(itemFlexGridSizer11, 1, wxGROW|wxALL, 5);
	
    wxStaticBox* itemStaticBoxSizer22Static = new wxStaticBox(m_SymbolPanel, wxID_ANY, _("Transparency"));
    wxStaticBoxSizer* itemStaticBoxSizer22 = new wxStaticBoxSizer(itemStaticBoxSizer22Static, wxHORIZONTAL);
    itemBoxSizer8->Add(itemStaticBoxSizer22, 0, wxGROW|wxALL, 5);
   
	m_TransparencySlider = new tmSliderWithText(m_SymbolPanel, ID_SYMDLGP_TRANSPARENCY,0,0,100,_T("%"),
												wxDefaultPosition, wxDefaultSize, 0);
    itemStaticBoxSizer22->Add(m_TransparencySlider, 1, wxGROW|wxALL, 5);
	
	// adapt dialog size to new controls added
	SetSizeHint();
}


bool tmSymbolDLGPoint::TransferDataToWindow()
{
	m_PointColourCtrl->SetColour(m_DlgData.m_Colour);
	m_PointWidthCtrl->SetValue(m_DlgData.m_Radius);
	m_TransparencySlider->SetValue(m_DlgData.m_GlobalTransparency);
	return TRUE;
}



bool tmSymbolDLGPoint::TransferDataFromWindow()
{
	m_DlgData.m_Colour = m_PointColourCtrl->GetColour();
	m_DlgData.m_Radius = m_PointWidthCtrl->GetValue();
	m_DlgData.m_GlobalTransparency = m_TransparencySlider->GetValue();
	return TRUE;
}


