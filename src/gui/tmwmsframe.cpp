//
// Created by lsc on 26.05.25.
//

#include "tmwmsframe.h"

tmWMSBrowserFrame::tmWMSBrowserFrame(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos,
                                     const wxSize& size, long style)
    : wxDialog(parent, id, title, pos, size, style) {
    _create_controls();

    // Connect Events
    m_btn_export->Bind(wxEVT_BUTTON, &tmWMSBrowserFrame::OnBtnExport, this);
    m_ctrl_btn_wms_load_layers->Bind(wxEVT_BUTTON, &tmWMSBrowserFrame::OnBtnLoadLayers, this);
}

void tmWMSBrowserFrame::OnBtnLoadLayers(wxCommandEvent& event) {
    wxMessageBox(_("Loading layers is not yet implemented."), _("Info"), wxOK | wxICON_INFORMATION);
    event.Skip();
}

void tmWMSBrowserFrame::OnBtnExport(wxCommandEvent& event) {
    wxMessageBox(_("Export functionality is not yet implemented."), _("Info"), wxOK | wxICON_INFORMATION);
    event.Skip();
}

void tmWMSBrowserFrame::_create_controls() {
    this->SetSizeHints(wxDefaultSize, wxDefaultSize);

    wxBoxSizer* bSizer1;
    bSizer1 = new wxBoxSizer(wxVERTICAL);

    wxStaticBoxSizer* sbSizer1;
    sbSizer1 = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, _("WMS server")), wxHORIZONTAL);

    m_staticText1 = new wxStaticText(sbSizer1->GetStaticBox(), wxID_ANY, _("Url:"), wxDefaultPosition, wxDefaultSize,
                                     0);
    m_staticText1->Wrap(-1);
    sbSizer1->Add(m_staticText1, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

    m_ctrl_wms_url = new wxComboBox(sbSizer1->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize,
                                    0, NULL, 0);
    m_ctrl_wms_url->Append(_("https://wms.geo.admin.ch/?SERVICE=WMS&VERSION=1.3.0"));
    sbSizer1->Add(m_ctrl_wms_url, 1, wxALL | wxALIGN_CENTER_VERTICAL, 5);

    m_ctrl_btn_wms_load_layers = new wxButton(sbSizer1->GetStaticBox(), wxID_ANY, _("Load layers"), wxDefaultPosition,
                                              wxDefaultSize, 0);
    sbSizer1->Add(m_ctrl_btn_wms_load_layers, 0, wxALL, 5);

    bSizer1->Add(sbSizer1, 0, wxEXPAND | wxALL, 5);

    m_ctrl_layer_list = new wxListCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_ICON | wxLC_REPORT);
    m_ctrl_layer_list->SetMinSize(wxSize(800, 600));

    bSizer1->Add(m_ctrl_layer_list, 1, wxALL | wxEXPAND, 5);

    wxStaticBoxSizer* sbSizer2;
    sbSizer2 = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, _("label")), wxHORIZONTAL);

    m_btn_export = new wxButton(sbSizer2->GetStaticBox(), wxID_ANY, _("Export..."), wxDefaultPosition, wxDefaultSize,
                                0);
    sbSizer2->Add(m_btn_export, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

    sbSizer2->Add(0, 0, 1, wxEXPAND, 5);

    m_staticText2 = new wxStaticText(sbSizer2->GetStaticBox(), wxID_ANY, _("Filter: "), wxDefaultPosition,
                                     wxDefaultSize, 0);
    m_staticText2->Wrap(-1);
    sbSizer2->Add(m_staticText2, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

    m_ctrl_search = new wxSearchCtrl(sbSizer2->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition,
                                     wxSize(200, -1), 0);
#ifndef __WXMAC__
    m_ctrl_search->ShowSearchButton(true);
#endif
    m_ctrl_search->ShowCancelButton(true);
    sbSizer2->Add(m_ctrl_search, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

    bSizer1->Add(sbSizer2, 0, wxEXPAND | wxALL, 5);

    this->SetSizer(bSizer1);
    this->Layout();
    bSizer1->Fit(this);

    this->Centre(wxBOTH);
}