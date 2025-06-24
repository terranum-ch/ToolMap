//
// Created by lsc on 26.05.25.
//

#include "tmwmsframe.h"

#include "../gis/tmwms.h"

tmWMSBrowserFrame::tmWMSBrowserFrame(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &pos,
                                     const wxSize &size, long style)
    : wxDialog(parent, id, title, pos, size, style) {
    _create_controls();

    m_ctrl_layer_list->EnableCheckBoxes();

    // create the following columns for the list control : checkbox, layer name, title, abstract
    m_ctrl_layer_list->InsertColumn(0, wxEmptyString, wxLIST_FORMAT_LEFT, 200);
    m_ctrl_layer_list->InsertColumn(1, _("Layer"), wxLIST_FORMAT_LEFT, 200);
    m_ctrl_layer_list->InsertColumn(2, _("Title"), wxLIST_FORMAT_LEFT, 200);
    m_ctrl_layer_list->InsertColumn(3, _("Abstract"), wxLIST_FORMAT_LEFT, 400);
    m_ctrl_layer_list->SetColumnWidth(0, 30); // Checkbox column width
    m_ctrl_layer_list->SetColumnWidth(1, 200); // Layer name column width
    m_ctrl_layer_list->SetColumnWidth(2, 200); // Title column width
    m_ctrl_layer_list->SetColumnWidth(3, 300); // Abstract column width
    m_ctrl_layer_list->SetWindowStyle(wxLC_REPORT | wxLC_SINGLE_SEL | wxLC_HRULES);

    // Connect Events
    m_btn_export->Bind(wxEVT_BUTTON, &tmWMSBrowserFrame::OnBtnExport, this);
    m_ctrl_btn_wms_load_layers->Bind(wxEVT_BUTTON, &tmWMSBrowserFrame::OnBtnLoadLayers, this);
    m_ctrl_layer_list->Bind(wxEVT_LIST_ITEM_ACTIVATED, &tmWMSBrowserFrame::OnDoubleClickItems, this);
    m_ctrl_search->Bind(wxEVT_SEARCHCTRL_SEARCH_BTN, &tmWMSBrowserFrame::OnSearchList, this);
    m_ctrl_search->Bind(wxEVT_SEARCH_CANCEL, &tmWMSBrowserFrame::OnSearchBtnCancel, this);
    m_info_text_ctrl->Bind(wxEVT_UPDATE_UI, &tmWMSBrowserFrame::UpdateInfoText, this);
}

void tmWMSBrowserFrame::OnBtnLoadLayers(wxCommandEvent &event) {
    // create temporary file to store the WMS capabilities XML
    wxFileName xml_output(wxFileName::CreateTempFileName("wms_capabilities_"));
    xml_output.SetExt("xml");
    wxLogDebug("Xml Output: '%s'", xml_output.GetFullPath());

    // get download capabilities from WMS server
    tmWMSBrowser wms_browser(m_ctrl_wms_url->GetValue());
    if (!wms_browser.DownloadCapabilities(xml_output.GetFullPath(), m_ctrl_wms_lang->GetValue())) {
        wxLogError(_("Failed to download WMS capabilities from the server."));
        return;
    }

    // Get the layers from the downloaded XML file
    m_layers_abstracts.Clear();
    m_layers_names.Clear();
    m_layers_titles.Clear();
    if (!wms_browser.GetLayers(m_layers_names, m_layers_titles, m_layers_abstracts)) {
        wxLogError(_("Failed to get layers from the WMS capabilities XML."));
        return;
    }

    // Clear the list control before adding new layers
    m_ctrl_layer_list->DeleteAllItems();

    // Add layers to the list control
    for (size_t i = 0; i < m_layers_names.GetCount(); ++i) {
        add_layer_to_list(m_layers_names[i], m_layers_titles[i], m_layers_abstracts[i], i);
    }
}

void tmWMSBrowserFrame::OnBtnExport(wxCommandEvent &event) {
    wxMessageBox(_("Export functionality is not yet implemented."), _("Info"), wxOK | wxICON_INFORMATION);
    event.Skip();
}

void tmWMSBrowserFrame::OnDoubleClickItems(wxListEvent &event) {
    tmWMSFrameDetails details_dlg(this, m_ctrl_layer_list->GetItemText(event.GetIndex(), 1),
                                  m_ctrl_layer_list->GetItemText(event.GetIndex(), 3),
                                  m_ctrl_layer_list->GetItemText(event.GetIndex(), 2));
    details_dlg.ShowModal();
}

void tmWMSBrowserFrame::OnSearchList(wxCommandEvent &event) {
    // remove item from the list not matching the search text
    wxString search_text = m_ctrl_search->GetValue().Lower();
    m_ctrl_layer_list->DeleteAllItems();
    if (search_text.IsEmpty()) {
        for (size_t i = 0; i < m_layers_names.GetCount(); ++i) {
            add_layer_to_list(m_layers_names[i], m_layers_titles[i], m_layers_abstracts[i], i);
        }
    } else {
        for (size_t i = 0; i < m_layers_names.GetCount(); ++i) {
            if (m_layers_titles[i].Lower().Contains(search_text)) {
                add_layer_to_list(m_layers_names[i], m_layers_titles[i], m_layers_abstracts[i], i);
            }
        }
    }
}

void tmWMSBrowserFrame::OnSearchBtnCancel(wxCommandEvent &event) {
    m_ctrl_layer_list->DeleteAllItems();
    for (size_t i = 0; i < m_layers_names.GetCount(); ++i) {
        add_layer_to_list(m_layers_names[i], m_layers_titles[i], m_layers_abstracts[i], i);
    }
}

void tmWMSBrowserFrame::UpdateInfoText(wxUpdateUIEvent & event) {
    if (m_info_text_ctrl == nullptr) {
        return; // Ensure status bar is created
    }
    // display total number of layers and total number of displayed layers
    wxString status_text = wxString::Format(_("Total: %llu, Displayed: %d"),
                       static_cast<unsigned long long>(m_layers_names.GetCount()),
                       m_ctrl_layer_list->GetItemCount());
    m_info_text_ctrl->SetLabel(status_text);
}

/// \brief Adds a layer to the list control.
void tmWMSBrowserFrame::add_layer_to_list(const wxString &layer_name, const wxString &layer_title,
                                          const wxString &layer_abstract, int layer_index) {
    m_ctrl_layer_list->InsertItem(m_ctrl_layer_list->GetItemCount(), wxEmptyString);
    m_ctrl_layer_list->SetItem(m_ctrl_layer_list->GetItemCount() - 1, 1, layer_name);
    m_ctrl_layer_list->SetItem(m_ctrl_layer_list->GetItemCount() - 1, 2, layer_title);
    m_ctrl_layer_list->SetItem(m_ctrl_layer_list->GetItemCount() - 1, 3, layer_abstract);
    m_ctrl_layer_list->SetItemData(m_ctrl_layer_list->GetItemCount() - 1, layer_index);
}

void tmWMSBrowserFrame::_create_controls() {
    this->SetSizeHints(wxDefaultSize, wxDefaultSize);

    wxBoxSizer *bSizer1;
    bSizer1 = new wxBoxSizer(wxVERTICAL);

    wxStaticBoxSizer *sbSizer1;
    sbSizer1 = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, _("WMS server")), wxHORIZONTAL);

    m_staticText1 = new wxStaticText(sbSizer1->GetStaticBox(), wxID_ANY, _("Url:"), wxDefaultPosition, wxDefaultSize,
                                     0);
    m_staticText1->Wrap(-1);
    sbSizer1->Add(m_staticText1, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

    m_ctrl_wms_url = new wxComboBox(sbSizer1->GetStaticBox(), wxID_ANY,
                                    _("https://wms.geo.admin.ch/?SERVICE=WMS&VERSION=1.3.0"), wxDefaultPosition,
                                    wxDefaultSize, 0, NULL, 0);
    m_ctrl_wms_url->Append(_("https://wms.geo.admin.ch/?SERVICE=WMS&VERSION=1.3.0"));
    m_ctrl_wms_url->SetSelection(0);
    sbSizer1->Add(m_ctrl_wms_url, 1, wxALL | wxALIGN_CENTER_VERTICAL, 5);

    m_staticText3 = new wxStaticText(sbSizer1->GetStaticBox(), wxID_ANY, _("Lang"), wxDefaultPosition, wxDefaultSize,
                                     0);
    m_staticText3->Wrap(-1);
    sbSizer1->Add(m_staticText3, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

    m_ctrl_wms_lang = new wxComboBox(sbSizer1->GetStaticBox(), wxID_ANY, _("en"), wxDefaultPosition, wxSize(100, -1), 0,
                                     NULL, 0);
    m_ctrl_wms_lang->Append(_("fr"));
    m_ctrl_wms_lang->Append(_("en"));
    m_ctrl_wms_lang->Append(_("de"));
    m_ctrl_wms_lang->Append(_("it"));
    m_ctrl_wms_lang->SetSelection(1);
    sbSizer1->Add(m_ctrl_wms_lang, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

    m_ctrl_btn_wms_load_layers = new wxButton(sbSizer1->GetStaticBox(), wxID_ANY, _("Load layers"), wxDefaultPosition,
                                              wxDefaultSize, 0);
    sbSizer1->Add(m_ctrl_btn_wms_load_layers, 0, wxALL, 5);


    bSizer1->Add(sbSizer1, 0, wxEXPAND | wxALL, 5);

    m_ctrl_layer_list = new wxListCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_ICON | wxLC_REPORT);
    m_ctrl_layer_list->SetMinSize(wxSize(800, 600));

    bSizer1->Add(m_ctrl_layer_list, 1, wxALL | wxEXPAND, 5);

    wxStaticBoxSizer *sbSizer2;
    sbSizer2 = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, _("label")), wxHORIZONTAL);

    m_btn_export = new wxButton(sbSizer2->GetStaticBox(), wxID_ANY, _("Export..."), wxDefaultPosition, wxDefaultSize,
                                0);
    sbSizer2->Add(m_btn_export, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);


    sbSizer2->Add(0, 0, 1, wxEXPAND, 5);

    m_info_text_ctrl = new wxStaticText(sbSizer2->GetStaticBox(), wxID_ANY, _T("Total: 0, Displayed: 0"), wxDefaultPosition,
                                        wxDefaultSize, 0);
    m_info_text_ctrl->Wrap(-1);
    sbSizer2->Add(m_info_text_ctrl, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);


    sbSizer2->Add(0, 0, 1, wxEXPAND, 5);

    m_staticText2 = new wxStaticText(sbSizer2->GetStaticBox(), wxID_ANY, _("Filter title: "), wxDefaultPosition,
                                     wxDefaultSize, 0);
    m_staticText2->Wrap(-1);
    sbSizer2->Add(m_staticText2, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

    m_ctrl_search = new wxSearchCtrl(sbSizer2->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition,
                                     wxSize(200, -1), 0);
#ifndef __WXMAC__
	m_ctrl_search->ShowSearchButton( true );
#endif
    m_ctrl_search->ShowCancelButton(true);
    sbSizer2->Add(m_ctrl_search, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);


    bSizer1->Add(sbSizer2, 0, wxEXPAND | wxALL, 5);


    this->SetSizer(bSizer1);
    this->Layout();
    bSizer1->Fit(this);

    this->Centre(wxBOTH);
}


tmWMSFrameDetails::tmWMSFrameDetails(wxWindow *parent, const wxString &name_wms, const wxString &abstract_wms,
                                     const wxString &title_wms, wxWindowID id, const wxString &title,
                                     const wxPoint &pos, const wxSize &size, long style) : wxDialog(
    parent, id, title, pos, size, style) {
    m_name_wms = name_wms;
    m_abstract_wms = abstract_wms;
    m_title_wms = title_wms;
    _create_controls();
}


void tmWMSFrameDetails::_create_controls() {
    this->SetSizeHints(wxDefaultSize, wxDefaultSize);

    wxBoxSizer *bSizer2;
    bSizer2 = new wxBoxSizer(wxVERTICAL);

    m_ctrl_html = new wxHtmlWindow(this, wxID_ANY, wxDefaultPosition, wxSize(500, 400), wxHW_SCROLLBAR_AUTO);
    bSizer2->Add(m_ctrl_html, 1, wxALL | wxEXPAND, 5);


    this->SetSizer(bSizer2);
    this->Layout();
    bSizer2->Fit(this);

    this->Centre(wxBOTH);
}

bool tmWMSFrameDetails::TransferDataToWindow() {
    // construct html content with WMS layer details
    wxString html_txt = _("<H1>Layer Details</H1>");
    html_txt += wxString::Format(_("<b>Name:</b> %s<br><br>"), m_name_wms);
    html_txt += wxString::Format(_("<b>Title:</b> %s<br><br>"), m_title_wms);
    html_txt += wxString::Format(_("<b>Abstract:</b> %s"), m_abstract_wms);
    m_ctrl_html->SetPage(html_txt);
    return wxDialog::TransferDataToWindow();
}
