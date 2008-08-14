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
    Init();
}



tmSymbolDLGPoint::tmSymbolDLGPoint( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, caption, pos, size, style);
}



bool tmSymbolDLGPoint::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    tmSymbolDLG::Create( parent, id, caption, pos, size, style );
	    
	CreateControlsLine();
	
	return true;
}




tmSymbolDLGPoint::~tmSymbolDLGPoint()
{

}



void tmSymbolDLGPoint::Init()
{
	m_PointColourCtrl		= NULL;
	m_PointWidthCtrl		= NULL;
	m_TransparencySlider	= NULL;
}



void tmSymbolDLGPoint::CreateControlsLine()
{
	wxString sFunction = wxString::FromAscii(__FUNCTION__);
	wxString sFunctionLineError = wxString::Format( _T("%s line %d : "),
												   sFunction.c_str(), __LINE__); 
	wxString sErrMsg = wxString::Format(_T("%s Undefined m_Notebook"), sFunctionLineError.c_str());
	wxASSERT_MSG(m_NoteBook,sErrMsg);
	
	
	wxPanel* itemPanel7 = new wxPanel( m_NoteBook, ID_SYMDLGP_PANEL, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer8 = new wxBoxSizer(wxVERTICAL);
    itemPanel7->SetSizer(itemBoxSizer8);
	
    wxNotebook* itemNotebook9 = new wxNotebook( itemPanel7, ID_SYMDLGP_NOTEBOOK, wxDefaultPosition, wxDefaultSize, wxBK_DEFAULT );
	
    wxPanel* itemPanel10 = new wxPanel( itemNotebook9, ID_SYMDLGP_PANEL2, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxFlexGridSizer* itemFlexGridSizer11 = new wxFlexGridSizer(2, 2, 0, 0);
    itemFlexGridSizer11->AddGrowableCol(1);
    itemPanel10->SetSizer(itemFlexGridSizer11);
	
    wxStaticText* itemStaticText12 = new wxStaticText( itemPanel10, wxID_STATIC, _("Color :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer11->Add(itemStaticText12, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    m_PointColourCtrl = new tmColourPickerCtrl(itemPanel10, ID_SYMDLGP_COLOR);
    itemFlexGridSizer11->Add(m_PointColourCtrl, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    wxStaticText* itemStaticText14 = new wxStaticText( itemPanel10, wxID_STATIC, _("Radius :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer11->Add(itemStaticText14, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    m_PointWidthCtrl = new wxSpinCtrl( itemPanel10, ID_SYMDLGP_WIDTH, _T("0"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100, 0 );
    itemFlexGridSizer11->Add(m_PointWidthCtrl, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    itemNotebook9->AddPage(itemPanel10, _("Unique"));
	
    wxPanel* itemPanel16 = new wxPanel( itemNotebook9, ID_SYMDLGP_PANEL3, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer17 = new wxBoxSizer(wxVERTICAL);
    itemPanel16->SetSizer(itemBoxSizer17);
	
    wxStaticText* itemStaticText18 = new wxStaticText( itemPanel16, wxID_STATIC, _("NOT IMPLEMENTED NOW"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer17->Add(itemStaticText18, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	
    itemNotebook9->AddPage(itemPanel16, _("Discrete"));
	
    wxPanel* itemPanel19 = new wxPanel( itemNotebook9, ID_SYMDLGP_PANEL4, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer20 = new wxBoxSizer(wxVERTICAL);
    itemPanel19->SetSizer(itemBoxSizer20);
	
    wxStaticText* itemStaticText21 = new wxStaticText( itemPanel19, wxID_STATIC, _("NOT IMPLEMENTED NOW"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer20->Add(itemStaticText21, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	
    itemNotebook9->AddPage(itemPanel19, _("Continuous"));
	
    itemBoxSizer8->Add(itemNotebook9, 1, wxGROW|wxALL, 5);
	
    wxStaticBox* itemStaticBoxSizer22Static = new wxStaticBox(itemPanel7, wxID_ANY, _("Transparency"));
    wxStaticBoxSizer* itemStaticBoxSizer22 = new wxStaticBoxSizer(itemStaticBoxSizer22Static, wxHORIZONTAL);
    itemBoxSizer8->Add(itemStaticBoxSizer22, 0, wxGROW|wxALL, 5);
   
	m_TransparencySlider = new tmSliderWithText(itemPanel7, ID_SYMDLGP_TRANSPARENCY,0,0,100,_T("%"),
												wxDefaultPosition, wxDefaultSize, 0);
    itemStaticBoxSizer22->Add(m_TransparencySlider, 0, wxGROW|wxALL, 5);
	
    m_NoteBook->AddPage(itemPanel7, _("Point Symbology"));
	
	
	// adapt dialog size to new controls added
	SetSizeHint();
}



