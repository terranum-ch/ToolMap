/***************************************************************************
 pdfexportwizard.cpp
 -------------------
 copyright            : (C) 2011 CREALP Lucien Schreiber 
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

#include "pdfexportwizard.h"

void PdfExportWizard::_CreateControls()
{
    // Page Layout
    m_PageLayout = new wxWizardPageSimple(this);
    wxBoxSizer *bSizer1;
    bSizer1 = new wxBoxSizer(wxVERTICAL);

    wxPanel *m_panel1 = new wxPanel(m_PageLayout, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
    wxStaticBoxSizer *sbSizer1;
    sbSizer1 = new wxStaticBoxSizer(new wxStaticBox(m_panel1, wxID_ANY, _("Type")), wxVERTICAL);

    m_ExportOnePageCtrl = new wxRadioButton(m_panel1, wxID_ANY, _("One Page"), wxDefaultPosition, wxDefaultSize,
                                            wxRB_GROUP);
    m_ExportOnePageCtrl->SetValue(true);
    sbSizer1->Add(m_ExportOnePageCtrl, 0, wxALL, 5);

    m_ExportMultiPageCtrl = new wxRadioButton(m_panel1, wxID_ANY, _("One layer per page"), wxDefaultPosition,
                                              wxDefaultSize, 0);
    sbSizer1->Add(m_ExportMultiPageCtrl, 0, wxALL, 5);

    m_panel1->SetSizer(sbSizer1);
    m_panel1->Layout();
    bSizer1->Fit(m_panel1);
    bSizer1->Add(m_panel1, 0, wxALL | wxEXPAND, 5);


    wxPanel *m_panel2 = new wxPanel(m_PageLayout, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
    wxStaticBoxSizer *sbSizer2;
    sbSizer2 = new wxStaticBoxSizer(new wxStaticBox(m_panel2, wxID_ANY, _("Layout")), wxVERTICAL);

    wxBoxSizer *bSizer4;
    bSizer4 = new wxBoxSizer(wxHORIZONTAL);

    m_ExportOneColCtrl = new wxRadioButton(m_panel2, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize,
                                           wxRB_GROUP);
    bSizer4->Add(m_ExportOneColCtrl, 0, wxTOP | wxBOTTOM | wxLEFT, 5);

    wxBoxSizer *bSizer5;
    bSizer5 = new wxBoxSizer(wxVERTICAL);

    wxStaticText *m_staticText1 = new wxStaticText(m_panel2, wxID_ANY, _("Object Kind"), wxDefaultPosition,
                                                   wxDefaultSize, 0);
    m_staticText1->Wrap(-1);
    bSizer5->Add(m_staticText1, 0, wxTOP | wxRIGHT, 5);

    wxStaticText *m_staticText2 = new wxStaticText(m_panel2, wxID_ANY, _("Object Attribute"), wxDefaultPosition,
                                                   wxDefaultSize, 0);
    m_staticText2->Wrap(-1);
    bSizer5->Add(m_staticText2, 0, wxBOTTOM | wxRIGHT, 5);

    bSizer4->Add(bSizer5, 0, wxEXPAND, 5);

    sbSizer2->Add(bSizer4, 1, wxEXPAND, 5);


    m_ExportTwoColCtrl = new wxRadioButton(m_panel2, wxID_ANY, _("Object Kind / Object Attribute"), wxDefaultPosition,
                                           wxDefaultSize, 0);
    m_ExportTwoColCtrl->SetValue(true);
    sbSizer2->Add(m_ExportTwoColCtrl, 0, wxALL, 5);


    m_panel2->SetSizer(sbSizer2);
    m_panel2->Layout();
    bSizer1->Fit(m_panel2);
    bSizer1->Add(m_panel2, 0, wxALL | wxEXPAND, 5);

    m_PageLayout->SetSizer(bSizer1);
    m_PageLayout->Layout();



    // page paper
    m_PagePaper = new wxWizardPageSimple(this);
    wxBoxSizer *bSizer2;
    bSizer2 = new wxBoxSizer(wxVERTICAL);

    wxStaticBoxSizer *sbSizer3;
    sbSizer3 = new wxStaticBoxSizer(new wxStaticBox(m_PagePaper, wxID_ANY, _("Page Size")), wxVERTICAL);

    wxString m_PaperSizeCtrlChoices[] = {_("A4"), _("A3"), _("A2"), _("A1"), _("A0")};
    int m_PaperSizeCtrlNChoices = sizeof(m_PaperSizeCtrlChoices) / sizeof(wxString);
    m_PaperSizeCtrl = new wxChoice(m_PagePaper, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_PaperSizeCtrlNChoices,
                                   m_PaperSizeCtrlChoices, 0);
    m_PaperSizeCtrl->SetSelection(0);
    sbSizer3->Add(m_PaperSizeCtrl, 0, wxALL, 5);

    bSizer2->Add(sbSizer3, 0, wxALL | wxEXPAND, 5);

    wxStaticBoxSizer *sbSizer4;
    sbSizer4 = new wxStaticBoxSizer(new wxStaticBox(m_PagePaper, wxID_ANY, _("Page orientation")), wxVERTICAL);

    m_PaperPortraitCtrl = new wxRadioButton(m_PagePaper, wxID_ANY, _("Portrait"), wxDefaultPosition, wxDefaultSize,
                                            wxRB_GROUP);
    m_PaperPortraitCtrl->SetValue(true);
    sbSizer4->Add(m_PaperPortraitCtrl, 0, wxALL, 5);

    m_PaperLandscapeCtrl = new wxRadioButton(m_PagePaper, wxID_ANY, _("Landscape"), wxDefaultPosition, wxDefaultSize,
                                             0);
    sbSizer4->Add(m_PaperLandscapeCtrl, 0, wxALL, 5);

    bSizer2->Add(sbSizer4, 0, wxALL | wxEXPAND, 5);

    m_PagePaper->SetSizer(bSizer2);
    m_PagePaper->Layout();


    // page decoration
    m_PageDecoration = new wxWizardPageSimple(this);
    wxBoxSizer *bSizer3;
    bSizer3 = new wxBoxSizer(wxVERTICAL);

    wxStaticBoxSizer *sbSizer5;
    sbSizer5 = new wxStaticBoxSizer(new wxStaticBox(m_PageDecoration, wxID_ANY, _("Decorations")), wxVERTICAL);

    m_DecorationSimpleCtrl = new wxCheckBox(m_PageDecoration, wxID_ANY, _("Use very simple decorations"),
                                            wxDefaultPosition, wxDefaultSize, 0);
    sbSizer5->Add(m_DecorationSimpleCtrl, 0, wxALL, 5);

    bSizer3->Add(sbSizer5, 0, wxEXPAND | wxALL, 5);

    m_PageDecoration->SetSizer(bSizer3);
    m_PageDecoration->Layout();
}


void PdfExportWizard::OnRadioOnePageClick(wxCommandEvent &event)
{
    wxWizardPageSimple::Chain(m_PageLayout, m_PageDecoration);
    event.Skip();
}


void PdfExportWizard::OnRadioMultiplePageClick(wxCommandEvent &event)
{
    wxWizardPageSimple::Chain(m_PageLayout, m_PagePaper);
    wxWizardPageSimple::Chain(m_PagePaper, m_PageDecoration);
    event.Skip();
}


void PdfExportWizard::OnLastPage(wxWizardEvent &event)
{
    if (event.GetPage() == m_PageDecoration && event.GetDirection() == true) {
        wxButton *myFinishBtn = (wxButton *) FindWindowById(wxID_FORWARD);
        if (m_TextBtnNext != NULL && myFinishBtn != NULL) {
            myFinishBtn->SetLabel(*m_TextBtnNext);
        }
    }
    event.Skip();
}


PdfExportWizard::PdfExportWizard(wxWindow *parent, wxWindowID id, const wxString &title)
        : wxWizard(parent, id, title)
{
    m_PageLayout = NULL;
    m_PagePaper = NULL;
    m_PageDecoration = NULL;
    m_TextBtnNext = NULL;

    _CreateControls();
    wxWizardPageSimple::Chain(m_PageLayout, m_PageDecoration);

    wxButton *myBtn = (wxButton *) FindWindowById(wxID_FORWARD);
    if (myBtn != NULL) {
        m_TextBtnNext = new wxString(myBtn->GetLabel());
    }


    // specifiy size
    GetPageAreaSizer()->Add(m_PageLayout);
    GetPageAreaSizer()->Add(m_PagePaper);
    GetPageAreaSizer()->Add(m_PageDecoration);
    wxSize mySize = GetPageAreaSizer()->CalcMin();
    SetMinSize(mySize);
    SetPageSize(mySize);

    // connect event
    m_ExportOnePageCtrl->Bind(wxEVT_COMMAND_RADIOBUTTON_SELECTED, &PdfExportWizard::OnRadioOnePageClick, this);
    m_ExportMultiPageCtrl->Bind(wxEVT_COMMAND_RADIOBUTTON_SELECTED, &PdfExportWizard::OnRadioMultiplePageClick, this);
    this->Bind(wxEVT_WIZARD_PAGE_CHANGED, &PdfExportWizard::OnLastPage, this);
}


PdfExportWizard::~PdfExportWizard()
{
    m_ExportOnePageCtrl->Unbind(wxEVT_COMMAND_RADIOBUTTON_SELECTED, &PdfExportWizard::OnRadioOnePageClick, this);
    m_ExportMultiPageCtrl->Unbind(wxEVT_COMMAND_RADIOBUTTON_SELECTED, &PdfExportWizard::OnRadioMultiplePageClick, this);
    this->Unbind(wxEVT_WIZARD_PAGE_CHANGED, &PdfExportWizard::OnLastPage, this);
    wxDELETE(m_TextBtnNext);
}


int PdfExportWizard::ShowWizard()
{
    if (RunWizard(m_PageLayout) == true) {
        return wxID_OK;
    }
    return wxID_CANCEL;
}


bool PdfExportWizard::GetWizardChoices(PdfDocument &document)
{
    document.SetOnePage(m_ExportOnePageCtrl->GetValue());
    document.SetTwoColsLayout(m_ExportTwoColCtrl->GetValue());
    if (m_ExportOnePageCtrl->GetValue() == false) {
        wxPaperSize myPaper = wxPAPER_A4;
        switch (m_PaperSizeCtrl->GetSelection()) {
            case 1:
                myPaper = wxPAPER_A3;
                break;

            case 2:
                myPaper = wxPAPER_A2;
                break;

            case 3:
                myPaper = wxPAPER_A1;
                break;

            case 4:
                myPaper = wxPAPER_A0;
                break;

            default:
                myPaper = wxPAPER_A4;
                break;
        }
        document.SetPaperFormat(myPaper);
        document.SetPageBreak(true);
        wxPrintOrientation myOrientation = wxPORTRAIT;
        if (m_PaperLandscapeCtrl->GetValue() == true) {
            myOrientation = wxLANDSCAPE;
        }
        document.SetPaperOrientation(myOrientation);
    }

    document.SetDecorate(!(m_DecorationSimpleCtrl->GetValue()));
    return true;
}



