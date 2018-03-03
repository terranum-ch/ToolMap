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

#include "tmsymboldlgpointmultiple.h"


IMPLEMENT_DYNAMIC_CLASS(tmSymbolDLGPointMultiple, tmSymbolDLG)


tmSymbolDLGPointMultiple::tmSymbolDLGPointMultiple()
{
    _Init();
}


tmSymbolDLGPointMultiple::tmSymbolDLGPointMultiple(wxWindow *parent, wxWindowID id, const wxString &caption,
                                                   const wxPoint &pos, const wxSize &size, long style)
{
    _Init();
    Create(parent, id, caption, pos, size, style);
}


bool tmSymbolDLGPointMultiple::Create(wxWindow *parent, wxWindowID id, const wxString &caption, const wxPoint &pos,
                                      const wxSize &size, long style)
{
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    tmSymbolDLG::Create(parent, id, caption, pos, size, style);

    CreateControlsPoint();
    return true;
}


tmSymbolDLGPointMultiple::~tmSymbolDLGPointMultiple()
{

}


void tmSymbolDLGPointMultiple::_Init()
{
    m_PointColourCtrl = NULL;
    m_PointWidthCtrl = NULL;
    m_TransparencySlider = NULL;

    m_SymbologyTypeCtrl = NULL;
    m_MQueryCtrl = NULL;
    m_MSelColourCtrl = NULL;
    m_MSelWidthCtrl = NULL;
    m_MSelVisibleCtrl = NULL;
    m_MUnSelColourCtrl = NULL;
    m_MUnSelWidthCtrl = NULL;
    m_MUnSelVisibleCtrl = NULL;

    m_TocType = TOC_NAME_UNKNOWN;
}


void tmSymbolDLGPointMultiple::CreateControlsPoint()
{
    //this->SetSizeHints( wxDefaultSize, wxDefaultSize );

    wxBoxSizer *itemBoxSizer8 = new wxBoxSizer(wxVERTICAL);
    m_SymbolPanel->SetSizer(itemBoxSizer8);


    m_SymbologyTypeCtrl = new wxNotebook(m_SymbolPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0);


    // UNIQUE SYMBOLOGY
    wxPanel *m_panel2;
    m_panel2 = new wxPanel(m_SymbologyTypeCtrl, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
    wxBoxSizer *bSizer4;
    bSizer4 = new wxBoxSizer(wxVERTICAL);


    wxFlexGridSizer *itemFlexGridSizer11 = new wxFlexGridSizer(2, 2, 0, 0);
    itemFlexGridSizer11->AddGrowableCol(1);

    wxStaticText *itemStaticText12 = new wxStaticText(m_panel2, wxID_STATIC, _("Color :"), wxDefaultPosition,
                                                      wxDefaultSize, 0);
    itemFlexGridSizer11->Add(itemStaticText12, 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL | wxALL, 5);

    m_PointColourCtrl = new wxColourPickerCtrl(m_panel2, ID_SYMDLGP_COLOR);
    itemFlexGridSizer11->Add(m_PointColourCtrl, 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL | wxALL, 5);

    wxStaticText *itemStaticText14 = new wxStaticText(m_panel2, wxID_STATIC, _("Radius :"), wxDefaultPosition,
                                                      wxDefaultSize, 0);
    itemFlexGridSizer11->Add(itemStaticText14, 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL | wxALL, 5);

    m_PointWidthCtrl = new wxSpinCtrl(m_panel2, ID_SYMDLGP_WIDTH, _T("0"), wxDefaultPosition, wxDefaultSize,
                                      wxSP_ARROW_KEYS, 0, 100, 0);
    itemFlexGridSizer11->Add(m_PointWidthCtrl, 0, wxGROW | wxALIGN_CENTER_VERTICAL | wxALL, 5);

    bSizer4->Add(itemFlexGridSizer11, 1, wxGROW | wxALL, 5);


    m_panel2->SetSizer(bSizer4);
    m_SymbologyTypeCtrl->AddPage(m_panel2, _("Unique"), false);



    // MULTIPLE SYMBOLOGY
    wxPanel *m_panel1;
    m_panel1 = new wxPanel(m_SymbologyTypeCtrl, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
    wxBoxSizer *bSizer2;
    bSizer2 = new wxBoxSizer(wxVERTICAL);

    wxStaticBoxSizer *sbSizer10;
    sbSizer10 = new wxStaticBoxSizer(new wxStaticBox(m_panel1, wxID_ANY, _("Query:")), wxVERTICAL);

    wxArrayString m_MQueryCtrlChoices;
    m_MQueryCtrl = new wxChoice(m_panel1, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_MQueryCtrlChoices, 0);
    m_MQueryCtrl->SetSelection(0);
    sbSizer10->Add(m_MQueryCtrl, 0, wxALL | wxEXPAND, 5);

    bSizer2->Add(sbSizer10, 0, wxEXPAND | wxALL, 5);

    wxBoxSizer *bSizer3;
    bSizer3 = new wxBoxSizer(wxHORIZONTAL);

    wxStaticBoxSizer *sbSizer3;
    sbSizer3 = new wxStaticBoxSizer(new wxStaticBox(m_panel1, wxID_ANY, _("Selected features")), wxVERTICAL);

    wxFlexGridSizer *fgSizer10;
    fgSizer10 = new wxFlexGridSizer(3, 2, 0, 0);
    fgSizer10->AddGrowableCol(1);
    fgSizer10->SetFlexibleDirection(wxBOTH);
    fgSizer10->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

    wxStaticText *m_staticText1;
    m_staticText1 = new wxStaticText(m_panel1, wxID_ANY, _("Color:"), wxDefaultPosition, wxDefaultSize, 0);
    m_staticText1->Wrap(-1);
    fgSizer10->Add(m_staticText1, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

    m_MSelColourCtrl = new wxColourPickerCtrl(m_panel1, wxID_ANY, *wxBLACK, wxDefaultPosition, wxDefaultSize,
                                              wxCLRP_DEFAULT_STYLE);
    fgSizer10->Add(m_MSelColourCtrl, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

    wxStaticText *m_staticText3;
    m_staticText3 = new wxStaticText(m_panel1, wxID_ANY, _("Width:"), wxDefaultPosition, wxDefaultSize, 0);
    m_staticText3->Wrap(-1);
    fgSizer10->Add(m_staticText3, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

    m_MSelWidthCtrl = new wxSpinCtrl(m_panel1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize,
                                     wxSP_ARROW_KEYS, 0, 10, 0);
    fgSizer10->Add(m_MSelWidthCtrl, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

    wxStaticText *m_staticText19;
    m_staticText19 = new wxStaticText(m_panel1, wxID_ANY, _("Visible:"), wxDefaultPosition, wxDefaultSize, 0);
    m_staticText19->Wrap(-1);
    fgSizer10->Add(m_staticText19, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

    m_MSelVisibleCtrl = new wxCheckBox(m_panel1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
    m_MSelVisibleCtrl->SetValue(true);
    fgSizer10->Add(m_MSelVisibleCtrl, 0, wxALL, 5);

    sbSizer3->Add(fgSizer10, 1, wxEXPAND, 5);

    bSizer3->Add(sbSizer3, 0, wxALL, 5);

    wxStaticBoxSizer *sbSizer31;
    sbSizer31 = new wxStaticBoxSizer(new wxStaticBox(m_panel1, wxID_ANY, _("Unselected features")), wxVERTICAL);

    wxFlexGridSizer *fgSizer101;
    fgSizer101 = new wxFlexGridSizer(3, 2, 0, 0);
    fgSizer101->AddGrowableCol(1);
    fgSizer101->SetFlexibleDirection(wxBOTH);
    fgSizer101->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

    wxStaticText *m_staticText11;
    m_staticText11 = new wxStaticText(m_panel1, wxID_ANY, _("Color:"), wxDefaultPosition, wxDefaultSize, 0);
    m_staticText11->Wrap(-1);
    fgSizer101->Add(m_staticText11, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

    m_MUnSelColourCtrl = new wxColourPickerCtrl(m_panel1, wxID_ANY, *wxBLACK, wxDefaultPosition, wxDefaultSize,
                                                wxCLRP_DEFAULT_STYLE);
    fgSizer101->Add(m_MUnSelColourCtrl, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

    wxStaticText *m_staticText31;
    m_staticText31 = new wxStaticText(m_panel1, wxID_ANY, _("Width:"), wxDefaultPosition, wxDefaultSize, 0);
    m_staticText31->Wrap(-1);
    fgSizer101->Add(m_staticText31, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

    m_MUnSelWidthCtrl = new wxSpinCtrl(m_panel1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize,
                                       wxSP_ARROW_KEYS, 0, 10, 0);
    fgSizer101->Add(m_MUnSelWidthCtrl, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

    wxStaticText *m_staticText21;
    m_staticText21 = new wxStaticText(m_panel1, wxID_ANY, _("Visible"), wxDefaultPosition, wxSize(-1, -1), 0);
    m_staticText21->Wrap(-1);
    fgSizer101->Add(m_staticText21, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

    m_MUnSelVisibleCtrl = new wxCheckBox(m_panel1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
    m_MUnSelVisibleCtrl->SetValue(true);
    fgSizer101->Add(m_MUnSelVisibleCtrl, 0, wxALL, 5);

    sbSizer31->Add(fgSizer101, 1, wxEXPAND, 5);

    bSizer3->Add(sbSizer31, 0, wxALL, 5);

    bSizer2->Add(bSizer3, 1, wxEXPAND, 5);

    m_panel1->SetSizer(bSizer2);
    m_panel1->Layout();
    bSizer2->Fit(m_panel1);
    m_SymbologyTypeCtrl->AddPage(m_panel1, _("Multiple"), false);

    itemBoxSizer8->Add(m_SymbologyTypeCtrl, 1, wxEXPAND | wxALL, 5);


    wxStaticBox *itemStaticBoxSizer22Static = new wxStaticBox(m_SymbolPanel, wxID_ANY, _("Transparency"));
    wxStaticBoxSizer *itemStaticBoxSizer22 = new wxStaticBoxSizer(itemStaticBoxSizer22Static, wxHORIZONTAL);
    itemBoxSizer8->Add(itemStaticBoxSizer22, 0, wxGROW | wxALL, 5);

    m_TransparencySlider = new tmSliderWithText(m_SymbolPanel, ID_SYMDLGP_TRANSPARENCY, 0, 0, 100, _T("%"),
                                                wxDefaultPosition, wxDefaultSize, 0);
    itemStaticBoxSizer22->Add(m_TransparencySlider, 1, wxGROW | wxALL, 5);

    // adapt dialog size to new controls added
    SetSizeHint();
}


bool tmSymbolDLGPointMultiple::TransferDataToWindow()
{
    wxASSERT(m_TocType == TOC_NAME_POINTS || m_TocType == TOC_NAME_LABELS);
    _LoadQueries(m_MQueryCtrl, m_TocType);

    m_SymbologyTypeCtrl->SetSelection(m_DlgData.m_PanelNo);

    // set unique symbology
    m_PointColourCtrl->SetColour(m_DlgData.m_Colour);
    m_PointWidthCtrl->SetValue(m_DlgData.m_Radius);
    m_TransparencySlider->SetValue(m_DlgData.m_GlobalTransparency);

    // set multiple symbology
    int myQueryIndex = wxNOT_FOUND;
    for (unsigned int i = 0; i < m_QueriesId.GetCount(); i++) {
        if (m_DlgData.m_QueryID == m_QueriesId.Item(i)) {
            myQueryIndex = i;
            break;
        }
    }
    if (myQueryIndex == wxNOT_FOUND) {
        // Query was removed...
        m_MQueryCtrl->Append(_("SELECT A QUERY!"));
        m_MQueryCtrl->Select(m_MQueryCtrl->GetCount() - 1);
        m_QueriesId.Add(wxNOT_FOUND);
    } else {
        m_MQueryCtrl->Select(myQueryIndex);
    }
    m_MSelColourCtrl->SetColour(m_DlgData.m_SelColourMultiple);
    m_MSelWidthCtrl->SetValue(m_DlgData.m_SelRadiusMultiple);
    m_MSelVisibleCtrl->SetValue(m_DlgData.m_SelVisible);

    m_MUnSelColourCtrl->SetColour(m_DlgData.m_UnSelColourMultiple);
    m_MUnSelWidthCtrl->SetValue(m_DlgData.m_UnSelRadiusMultiple);
    m_MUnSelVisibleCtrl->SetValue(m_DlgData.m_UnSelVisible);
    return true;
}


bool tmSymbolDLGPointMultiple::TransferDataFromWindow()
{
    m_DlgData.m_PanelNo = m_SymbologyTypeCtrl->GetSelection();
    m_DlgData.m_Colour = m_PointColourCtrl->GetColour();
    m_DlgData.m_Radius = m_PointWidthCtrl->GetValue();
    m_DlgData.m_GlobalTransparency = m_TransparencySlider->GetValue();

    // get query id
    m_DlgData.m_QueryID = m_QueriesId.Item(m_MQueryCtrl->GetSelection());
    if (m_DlgData.m_QueryID == wxNOT_FOUND) {
        m_DlgData.m_PanelNo = 0;
    }

    m_DlgData.m_SelColourMultiple = m_MSelColourCtrl->GetColour();
    m_DlgData.m_SelRadiusMultiple = m_MSelWidthCtrl->GetValue();
    m_DlgData.m_SelVisible = m_MSelVisibleCtrl->GetValue();

    m_DlgData.m_UnSelColourMultiple = m_MUnSelColourCtrl->GetColour();
    m_DlgData.m_UnSelRadiusMultiple = m_MUnSelWidthCtrl->GetValue();
    m_DlgData.m_UnSelVisible = m_MUnSelVisibleCtrl->GetValue();
    return true;
}


