/***************************************************************************
 danglingnodes_dlg.cpp
 Display Dangling nodes Dialog
 -------------------
 copyright            : (C) 2009 CREALP Lucien Schreiber 
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

#include "danglingnodes_dlg.h"


BEGIN_EVENT_TABLE(DanglingNode_DLG, wxDialog)
                EVT_BUTTON(ID_DLGDN_CLEAR, DanglingNode_DLG::OnClearDangling)
END_EVENT_TABLE()


DanglingNode_DLG::DanglingNode_DLG(wxWindow *parent, wxWindowID id,
                                   const wxString &title, const wxPoint &pos,
                                   const wxSize &size, long style) :
        wxDialog(parent, id, title, pos, size, style)
{
    m_SelectedLayer = wxNOT_FOUND;
    CreateControls();
}


DanglingNode_DLG::~DanglingNode_DLG()
{
}


void DanglingNode_DLG::CreateControls()
{
    this->SetSizeHints(wxDefaultSize, wxDefaultSize);

    wxBoxSizer *bSizer24;
    bSizer24 = new wxBoxSizer(wxVERTICAL);

    wxStaticText *m_staticText6;
    m_staticText6 = new wxStaticText(this, wxID_ANY,
                                     _("Search for dangling nodes on:"),
                                     wxDefaultPosition, wxDefaultSize, 0);
    m_staticText6->Wrap(-1);
    bSizer24->Add(m_staticText6, 0, wxTOP | wxRIGHT | wxLEFT, 10);

    wxArrayString m_LayerChoiceChoices;
    m_LayerChoice = new wxListBox(this, wxID_ANY, wxDefaultPosition, wxSize(300, 200), m_LayerChoiceChoices, 0);
    //m_LayerChoice->SetSelection( 0 );
    bSizer24->Add(m_LayerChoice, 1, wxALL | wxEXPAND, 5);


    //bSizer24->Add( 0, 20, 1, wxEXPAND, 0 );

    wxBoxSizer *bSizer25;
    bSizer25 = new wxBoxSizer(wxHORIZONTAL);

    wxButton *m_button21;
    m_button21 = new wxButton(this, ID_DLGDN_CLEAR, _("Clear"), wxDefaultPosition, wxDefaultSize,
                              0); //  wxSize( 100,-1 ), 0 );
    bSizer25->Add(m_button21, 0, wxALL, 5);

    wxButton *m_button22;
    m_button22 = new wxButton(this, wxID_OK, _("Search"), wxDefaultPosition, wxDefaultSize, 0);
    m_button22->SetDefault();
    bSizer25->Add(m_button22, 0, wxALL, 5);

    bSizer24->Add(bSizer25, 0, wxALIGN_RIGHT | wxTOP | wxBOTTOM, 5);

    this->SetSizer(bSizer24);
    this->Layout();
    bSizer24->Fit(this);

    this->CenterOnParent(wxBOTH);

}


void DanglingNode_DLG::SetLayers(const wxArrayString &layers,
                                 int selected)
{
    m_SelectedLayer = selected;

    wxASSERT(layers.GetCount() > 1);
    m_LayerChoice->Append(layers);

    if (selected == wxNOT_FOUND) {
        m_LayerChoice->SetSelection(0);
    } else {
        m_LayerChoice->SetSelection(selected);
    }
}


bool DanglingNode_DLG::TransferDataFromWindow()
{
    m_SelectedLayer = m_LayerChoice->GetSelection();
    return true;
}

void DanglingNode_DLG::OnClearDangling(wxCommandEvent &event)
{
    EndModal(ID_DLGDN_CLEAR);
}




