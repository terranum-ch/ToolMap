/***************************************************************************
								tmsymboldlg.cpp
						Display Symbology dialog
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

#include "tmsymboldlg.h"


IMPLEMENT_DYNAMIC_CLASS( tmSymbolDLG, wxDialog )



BEGIN_EVENT_TABLE( tmSymbolDLG, wxDialog )
END_EVENT_TABLE()


tmSymbolDLG::tmSymbolDLG()
{
    Init();
}



tmSymbolDLG::tmSymbolDLG( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, caption, pos, size, style);
}



bool tmSymbolDLG::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );
	
    CreateControlsBasic();
	
	SetSizeHint();
    return true;
}


void tmSymbolDLG::SetSizeHint ()
{
	if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
}


tmSymbolDLG::~tmSymbolDLG()
{

}


void tmSymbolDLG::Init()
{
	m_NoteBook = NULL;
}



void tmSymbolDLG::CreateControlsBasic()
{    
	tmSymbolDLG* itemDialog1 = this;
	
    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);
	
	m_NoteBook = new wxNotebook( itemDialog1, ID_SYMDLG_NOTEBOOK, 
								wxDefaultPosition, wxDefaultSize, wxBK_DEFAULT );
	
    wxPanel* itemPanel4 = new wxPanel( m_NoteBook, ID_SYMDLG_PANEL, 
									  wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer5 = new wxBoxSizer(wxVERTICAL);
    itemPanel4->SetSizer(itemBoxSizer5);
	
    wxTextCtrl* itemTextCtrl6 = new wxTextCtrl( itemPanel4, ID_SYMDLG_INFOTXT, _T(""),
											   wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE );
    itemBoxSizer5->Add(itemTextCtrl6, 1, wxGROW|wxALL, 5);
	
    m_NoteBook->AddPage(itemPanel4, _("Informations"));
	

    itemBoxSizer2->Add(m_NoteBook, 1, wxGROW|wxALL, 5);
	
    wxStdDialogButtonSizer* itemStdDialogButtonSizer27 = new wxStdDialogButtonSizer;
	
    itemBoxSizer2->Add(itemStdDialogButtonSizer27, 0, wxALIGN_RIGHT|wxALL, 5);
    wxButton* itemButton28 = new wxButton( itemDialog1, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer27->AddButton(itemButton28);
	
    wxButton* itemButton29 = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer27->AddButton(itemButton29);
	
    itemStdDialogButtonSizer27->Realize();

}






/*void tmSymbolDLG::CreateControlsPoint()
{
	wxPanel* itemPanel7 = new wxPanel( m_NoteBook, ID_PANEL1, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer8 = new wxBoxSizer(wxVERTICAL);
    itemPanel7->SetSizer(itemBoxSizer8);
	
    wxNotebook* itemNotebook9 = new wxNotebook( itemPanel7, ID_NOTEBOOK5, wxDefaultPosition, wxDefaultSize, wxBK_DEFAULT );
	
    wxPanel* itemPanel10 = new wxPanel( itemNotebook9, ID_PANEL11, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxFlexGridSizer* itemFlexGridSizer11 = new wxFlexGridSizer(2, 2, 0, 0);
    itemFlexGridSizer11->AddGrowableCol(1);
    itemPanel10->SetSizer(itemFlexGridSizer11);
	
    wxStaticText* itemStaticText12 = new wxStaticText( itemPanel10, wxID_STATIC, _("Color :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer11->Add(itemStaticText12, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    wxButton* itemButton13 = new wxButton( itemPanel10, ID_BUTTON12, _("Button"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer11->Add(itemButton13, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    wxStaticText* itemStaticText14 = new wxStaticText( itemPanel10, wxID_STATIC, _("Radius :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer11->Add(itemStaticText14, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    wxSpinCtrl* itemSpinCtrl15 = new wxSpinCtrl( itemPanel10, ID_SPINCTRL3, _T("0"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100, 0 );
    itemFlexGridSizer11->Add(itemSpinCtrl15, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    itemNotebook9->AddPage(itemPanel10, _("Unique"));
	
    wxPanel* itemPanel16 = new wxPanel( itemNotebook9, ID_PANEL12, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer17 = new wxBoxSizer(wxVERTICAL);
    itemPanel16->SetSizer(itemBoxSizer17);
	
    wxStaticText* itemStaticText18 = new wxStaticText( itemPanel16, wxID_STATIC, _("NOT IMPLEMENTED NOW"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer17->Add(itemStaticText18, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	
    itemNotebook9->AddPage(itemPanel16, _("Discrete"));
	
    wxPanel* itemPanel19 = new wxPanel( itemNotebook9, ID_PANEL13, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer20 = new wxBoxSizer(wxVERTICAL);
    itemPanel19->SetSizer(itemBoxSizer20);
	
    wxStaticText* itemStaticText21 = new wxStaticText( itemPanel19, wxID_STATIC, _("NOT IMPLEMENTED NOW"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer20->Add(itemStaticText21, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	
    itemNotebook9->AddPage(itemPanel19, _("Continuous"));
	
    itemBoxSizer8->Add(itemNotebook9, 1, wxGROW|wxALL, 5);
	
    wxStaticBox* itemStaticBoxSizer22Static = new wxStaticBox(itemPanel7, wxID_ANY, _("Transparency"));
    wxStaticBoxSizer* itemStaticBoxSizer22 = new wxStaticBoxSizer(itemStaticBoxSizer22Static, wxHORIZONTAL);
    itemBoxSizer8->Add(itemStaticBoxSizer22, 0, wxGROW|wxALL, 5);
    wxSlider* itemSlider23 = new wxSlider( itemPanel7, ID_SLIDER1, 0, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
    itemStaticBoxSizer22->Add(itemSlider23, 1, wxGROW|wxALL, 5);
	
    wxTextCtrl* itemTextCtrl24 = new wxTextCtrl( itemPanel7, ID_TEXTCTRL10, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticBoxSizer22->Add(itemTextCtrl24, 0, wxGROW|wxALL, 5);
	
    m_NoteBook->AddPage(itemPanel7, _("Point Symbology"));

	SetSizeHint();
	
}*/

/*
void tmSymbolDLG::CreateControlsPolygon()
{
	wxPanel* itemPanel7 = new wxPanel( m_NoteBook, ID_PANEL1, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer8 = new wxBoxSizer(wxVERTICAL);
    itemPanel7->SetSizer(itemBoxSizer8);
	
    wxNotebook* itemNotebook9 = new wxNotebook( itemPanel7, ID_NOTEBOOK5, wxDefaultPosition, wxDefaultSize, wxBK_DEFAULT );
	
    wxPanel* itemPanel10 = new wxPanel( itemNotebook9, ID_PANEL11, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxFlexGridSizer* itemFlexGridSizer11 = new wxFlexGridSizer(4, 2, 0, 0);
    itemFlexGridSizer11->AddGrowableCol(1);
    itemPanel10->SetSizer(itemFlexGridSizer11);
	
    wxStaticText* itemStaticText12 = new wxStaticText( itemPanel10, wxID_STATIC, _("Border Color :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer11->Add(itemStaticText12, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    wxButton* itemButton13 = new wxButton( itemPanel10, ID_BUTTON12, _("Button"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer11->Add(itemButton13, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    wxStaticText* itemStaticText14 = new wxStaticText( itemPanel10, wxID_STATIC, _("Border Width :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer11->Add(itemStaticText14, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    wxSpinCtrl* itemSpinCtrl15 = new wxSpinCtrl( itemPanel10, ID_SPINCTRL3, _T("0"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100, 0 );
    itemFlexGridSizer11->Add(itemSpinCtrl15, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    wxStaticText* itemStaticText16 = new wxStaticText( itemPanel10, wxID_STATIC, _("Fill Color :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer11->Add(itemStaticText16, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    wxButton* itemButton17 = new wxButton( itemPanel10, ID_BUTTON1, _("Button"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer11->Add(itemButton17, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    wxStaticText* itemStaticText18 = new wxStaticText( itemPanel10, wxID_STATIC, _("Fill style :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer11->Add(itemStaticText18, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    wxArrayString itemChoice19Strings;
    itemChoice19Strings.Add(_("----------"));
    itemChoice19Strings.Add(_("..............."));
    wxChoice* itemChoice19 = new wxChoice( itemPanel10, ID_CHOICE4, wxDefaultPosition, wxDefaultSize, itemChoice19Strings, 0 );
    itemFlexGridSizer11->Add(itemChoice19, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    itemNotebook9->AddPage(itemPanel10, _("Unique"));
	
    wxPanel* itemPanel20 = new wxPanel( itemNotebook9, ID_PANEL12, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer21 = new wxBoxSizer(wxVERTICAL);
    itemPanel20->SetSizer(itemBoxSizer21);
	
    wxStaticText* itemStaticText22 = new wxStaticText( itemPanel20, wxID_STATIC, _("NOT IMPLEMENTED NOW"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer21->Add(itemStaticText22, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	
    itemNotebook9->AddPage(itemPanel20, _("Discrete"));
	
    wxPanel* itemPanel23 = new wxPanel( itemNotebook9, ID_PANEL13, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer24 = new wxBoxSizer(wxVERTICAL);
    itemPanel23->SetSizer(itemBoxSizer24);
	
    wxStaticText* itemStaticText25 = new wxStaticText( itemPanel23, wxID_STATIC, _("NOT IMPLEMENTED NOW"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer24->Add(itemStaticText25, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	
    itemNotebook9->AddPage(itemPanel23, _("Continuous"));
	
    itemBoxSizer8->Add(itemNotebook9, 1, wxGROW|wxALL, 5);
	
    wxStaticBox* itemStaticBoxSizer26Static = new wxStaticBox(itemPanel7, wxID_ANY, _("Transparency"));
    wxStaticBoxSizer* itemStaticBoxSizer26 = new wxStaticBoxSizer(itemStaticBoxSizer26Static, wxHORIZONTAL);
    itemBoxSizer8->Add(itemStaticBoxSizer26, 0, wxGROW|wxALL, 5);
    wxSlider* itemSlider27 = new wxSlider( itemPanel7, ID_SLIDER1, 0, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
    itemStaticBoxSizer26->Add(itemSlider27, 1, wxGROW|wxALL, 5);
	
    wxTextCtrl* itemTextCtrl28 = new wxTextCtrl( itemPanel7, ID_TEXTCTRL10, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticBoxSizer26->Add(itemTextCtrl28, 0, wxGROW|wxALL, 5);
	
    m_NoteBook->AddPage(itemPanel7, _("Polygon Symbology"));
	
	SetSizeHint();
}


void tmSymbolDLG::CreateControlsRaster()
{
	wxPanel* itemPanel7 = new wxPanel( m_NoteBook, ID_PANEL1, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer8 = new wxBoxSizer(wxVERTICAL);
    itemPanel7->SetSizer(itemBoxSizer8);
	
    wxStaticBox* itemStaticBoxSizer9Static = new wxStaticBox(itemPanel7, wxID_ANY, _("Color transparency"));
    wxStaticBoxSizer* itemStaticBoxSizer9 = new wxStaticBoxSizer(itemStaticBoxSizer9Static, wxVERTICAL);
    itemBoxSizer8->Add(itemStaticBoxSizer9, 1, wxGROW|wxALL, 5);
    wxButton* itemButton10 = new wxButton( itemPanel7, ID_BUTTON12, _("Pick a color"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticBoxSizer9->Add(itemButton10, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	
    wxStaticBox* itemStaticBoxSizer11Static = new wxStaticBox(itemPanel7, wxID_ANY, _("Transparency"));
    wxStaticBoxSizer* itemStaticBoxSizer11 = new wxStaticBoxSizer(itemStaticBoxSizer11Static, wxHORIZONTAL);
    itemBoxSizer8->Add(itemStaticBoxSizer11, 0, wxGROW|wxALL, 5);
    wxSlider* itemSlider12 = new wxSlider( itemPanel7, ID_SLIDER1, 0, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
    itemStaticBoxSizer11->Add(itemSlider12, 1, wxGROW|wxALL, 5);
	
    wxTextCtrl* itemTextCtrl13 = new wxTextCtrl( itemPanel7, ID_TEXTCTRL10, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticBoxSizer11->Add(itemTextCtrl13, 0, wxGROW|wxALL, 5);
	
    m_NoteBook->AddPage(itemPanel7, _("Raster Symbology"));
	
	SetSizeHint();
}
*/

