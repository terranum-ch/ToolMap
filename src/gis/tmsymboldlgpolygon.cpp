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



BEGIN_EVENT_TABLE( tmSymbolDLGPolygon, tmSymbolDLG )
END_EVENT_TABLE()


tmSymbolDLGPolygon::tmSymbolDLGPolygon()
{
    Init();
}



tmSymbolDLGPolygon::tmSymbolDLGPolygon( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, caption, pos, size, style);
}



bool tmSymbolDLGPolygon::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    tmSymbolDLG::Create( parent, id, caption, pos, size, style );
	    
	CreateControlsLine();
	
	return true;
}




tmSymbolDLGPolygon::~tmSymbolDLGPolygon()
{

}



void tmSymbolDLGPolygon::Init()
{
	m_PolygonBorderColourCtrl	= NULL;
	m_PolygonFillColourCtrl		= NULL;
	m_PolygonBorderWidthCtrl	= NULL;
	m_PolygonFillPattern		= NULL;
	m_TransparencySlider		= NULL;
}



void tmSymbolDLGPolygon::CreateControlsLine()
{
	wxString sFunction = wxString::FromAscii(__FUNCTION__);
	wxString sFunctionLineError = wxString::Format( _T("%s line %d : "),
												   sFunction.c_str(), __LINE__); 
	wxString sErrMsg = wxString::Format(_T("%s Undefined m_Notebook"), sFunctionLineError.c_str());
	wxASSERT_MSG(m_NoteBook,sErrMsg);

	
	
	wxPanel* itemPanel7 = new wxPanel( m_NoteBook, ID_SYMDLGPLG_PANEL, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer8 = new wxBoxSizer(wxVERTICAL);
    itemPanel7->SetSizer(itemBoxSizer8);
	
    wxNotebook* itemNotebook9 = new wxNotebook( itemPanel7, ID_SYMDLGPLG_NOTEBOOK, wxDefaultPosition, wxDefaultSize, wxBK_DEFAULT );
	
    wxPanel* itemPanel10 = new wxPanel( itemNotebook9, ID_SYMDLGPLG_PANEL2, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxFlexGridSizer* itemFlexGridSizer11 = new wxFlexGridSizer(4, 2, 0, 0);
    itemFlexGridSizer11->AddGrowableCol(1);
    itemPanel10->SetSizer(itemFlexGridSizer11);
	
    wxStaticText* itemStaticText12 = new wxStaticText( itemPanel10, wxID_STATIC, _("Border Color :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer11->Add(itemStaticText12, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    m_PolygonBorderColourCtrl = new tmColourPickerCtrl(itemPanel10, ID_SYMDLGPLG_BORDER_COLOR);
    itemFlexGridSizer11->Add(m_PolygonBorderColourCtrl, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    wxStaticText* itemStaticText14 = new wxStaticText( itemPanel10, wxID_STATIC, _("Border Width :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer11->Add(itemStaticText14, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    m_PolygonBorderWidthCtrl = new wxSpinCtrl( itemPanel10, ID_SYMDLGPLG_BORDER_WIDTH, _T("0"),
											  wxDefaultPosition, wxDefaultSize, 
											  wxSP_ARROW_KEYS, 0, 100, 0 );
    itemFlexGridSizer11->Add(m_PolygonBorderWidthCtrl, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    wxStaticText* itemStaticText16 = new wxStaticText( itemPanel10, wxID_STATIC, _("Fill Color :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer11->Add(itemStaticText16, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    m_PolygonFillColourCtrl = new tmColourPickerCtrl( itemPanel10, ID_SYMDLGPLG_FILL_COLOR);
    itemFlexGridSizer11->Add(m_PolygonFillColourCtrl, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    wxStaticText* itemStaticText18 = new wxStaticText( itemPanel10, wxID_STATIC, _("Fill style :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer11->Add(itemStaticText18, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    wxArrayString itemChoice19Strings;
    itemChoice19Strings.Add(_("----------"));
    itemChoice19Strings.Add(_("..............."));
    
	m_PolygonFillPattern = new wxChoice( itemPanel10, ID_SYMDLGPLG_FILL_PATTERN, 
										wxDefaultPosition, wxDefaultSize, itemChoice19Strings, 0 );
    itemFlexGridSizer11->Add(m_PolygonFillPattern, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    itemNotebook9->AddPage(itemPanel10, _("Unique"));
	
    wxPanel* itemPanel20 = new wxPanel( itemNotebook9, ID_SYMDLGPLG_PANEL3, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer21 = new wxBoxSizer(wxVERTICAL);
    itemPanel20->SetSizer(itemBoxSizer21);
	
    wxStaticText* itemStaticText22 = new wxStaticText( itemPanel20, wxID_STATIC, _("NOT IMPLEMENTED NOW"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer21->Add(itemStaticText22, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	
    itemNotebook9->AddPage(itemPanel20, _("Discrete"));
	
    wxPanel* itemPanel23 = new wxPanel( itemNotebook9, ID_SYMDLGPLG_PANEL4, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer24 = new wxBoxSizer(wxVERTICAL);
    itemPanel23->SetSizer(itemBoxSizer24);
	
    wxStaticText* itemStaticText25 = new wxStaticText( itemPanel23, wxID_STATIC, _("NOT IMPLEMENTED NOW"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer24->Add(itemStaticText25, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	
    itemNotebook9->AddPage(itemPanel23, _("Continuous"));
	
    itemBoxSizer8->Add(itemNotebook9, 1, wxGROW|wxALL, 5);
	
    wxStaticBox* itemStaticBoxSizer26Static = new wxStaticBox(itemPanel7, wxID_ANY, _("Transparency"));
    wxStaticBoxSizer* itemStaticBoxSizer26 = new wxStaticBoxSizer(itemStaticBoxSizer26Static, wxHORIZONTAL);
    itemBoxSizer8->Add(itemStaticBoxSizer26, 0, wxGROW|wxALL, 5);
    
	m_TransparencySlider = new tmSliderWithText(itemPanel7, ID_SYMDLGPLG_TRANSPARENCY,
												0,0,100, _T("%"));
    itemStaticBoxSizer26->Add(m_TransparencySlider, 1, wxGROW|wxALL, 5);
	
     m_NoteBook->AddPage(itemPanel7, _("Polygon Symbology"));
	
	
	// adapt dialog size to new controls added
	SetSizeHint();
}



