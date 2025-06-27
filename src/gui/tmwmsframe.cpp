//
// Created by lsc on 26.05.25.
//

#include "tmwmsframe.h"

#include "../gis/tmwms.h"

tmWMSBrowserFrame::tmWMSBrowserFrame(wxWindow *parent, bool is_project_open, wxWindowID id, const wxString &title,
                                     const wxPoint &pos,
                                     const wxSize &size, long style)
    : wxDialog(parent, id, title, pos, size, style) {
    _create_controls();

    m_is_project_open = is_project_open;
    if (!m_is_project_open) {
        m_ctrl_append_to_project->SetValue(false);
        m_ctrl_append_to_project->Disable();
    }

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
    // remove checked items
    m_checked_layers.Clear();

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
    m_layers_crs.Clear();
    if (!wms_browser.GetLayers(m_layers_names, m_layers_titles, m_layers_abstracts, m_layers_crs)) {
        wxLogError(_("Failed to get layers from the WMS capabilities XML."));
        return;
    }

    // add the crs to the list control
    if (m_layers_crs.GetCount() ==0) {
        wxLogError(_("No CRS found in the WMS capabilities XML."));
        m_layers_crs.Add("EPSG:3857"); // Default to EPSG:3857 if no CRS found
    }

    wxArrayString layers_crs_helper = m_layers_crs;
    // enhance the CRS list by adding common name to ESPG codes
    for (size_t i = 0; i < layers_crs_helper.GetCount(); ++i) {
        if (layers_crs_helper[i].StartsWith("EPSG:")) {
            wxString epsg_code = layers_crs_helper[i].AfterFirst(':');
            if (epsg_code == "3857") {
                layers_crs_helper[i] = "EPSG:3857 (WGS 84)";
            } else if (epsg_code == "21781") {
                layers_crs_helper[i] = "EPSG:21781 (CH1903)";
            } else if (epsg_code == "2056") {
                layers_crs_helper[i] = "EPSG:2056 (CH1903+)";
            }
        }
    }

    m_ctrl_projection->Set(layers_crs_helper);

    // Clear the list control before adding new layers
    m_ctrl_layer_list->DeleteAllItems();

    // Add layers to the list control
    for (size_t i = 0; i < m_layers_names.GetCount(); ++i) {
        add_layer_to_list(m_layers_names[i], m_layers_titles[i], m_layers_abstracts[i], i);
    }
}

void tmWMSBrowserFrame::OnBtnExport(wxCommandEvent &event) {
    _get_checked_layers();
    m_exported_layers.Clear();

    // display a directory dialog to select the output directory
    wxDirDialog dir_dlg(this, _("Select output directory"), wxEmptyString, wxDD_DEFAULT_STYLE);
    if (dir_dlg.ShowModal() != wxID_OK) {
        return; // User cancelled the dialog
    }
    wxString output_dir = dir_dlg.GetPath();
    if (output_dir.IsEmpty()) {
        wxLogError(_("No output directory selected."));
        return;
    }

    // get the selected projection
    wxString selected_projection =  m_layers_crs.Item( m_ctrl_projection->GetSelection());
    // get the projection digits and remove EPSG text
    wxString projection_code_txt = selected_projection.AfterFirst(':');

    // Get the checked layers and export them to XML files
    tmWMSFileXML wmsFileXML(m_ctrl_wms_url->GetValue());
    for (size_t i = 0; i < m_checked_layers.GetCount(); ++i) {
        int layer_index = m_checked_layers[i];
        wxString layer_name = m_layers_names[layer_index];
        if (!wmsFileXML.CreateXML(layer_name, output_dir + wxFileName::GetPathSeparator() + layer_name + projection_code_txt + ".xml", selected_projection)) {
            wxLogError(_("Failed to create XML file for layer: %s"), layer_name);
        } else {
            wxLogDebug(_("Exported layer '%s' to XML file."), layer_name);
            m_exported_layers.Add(output_dir + wxFileName::GetPathSeparator() + layer_name + ".xml");
        }
    }
    wxMessageBox(
        wxString::Format(_("%llu layers exported into the output directory: %s"),
                         static_cast<unsigned long long>(m_checked_layers.GetCount()), output_dir),
        _("Export Complete"), wxOK | wxICON_INFORMATION, this);
    Close(); // Close the dialog after export
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
    _get_checked_layers();
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
    _get_checked_layers();
    m_ctrl_layer_list->DeleteAllItems();
    for (size_t i = 0; i < m_layers_names.GetCount(); ++i) {
        add_layer_to_list(m_layers_names[i], m_layers_titles[i], m_layers_abstracts[i], i);
    }
}

void tmWMSBrowserFrame::UpdateInfoText(wxUpdateUIEvent &event) {
    if (m_info_text_ctrl == nullptr) {
        return; // Ensure status bar is created
    }
    // display total number of layers and total number of displayed layers
    wxString status_text = wxString::Format(_("Total: %llu\tDisplayed: %d"),
                                            static_cast<unsigned long long>(m_layers_names.GetCount()),
                                            m_ctrl_layer_list->GetItemCount());
    m_info_text_ctrl->SetLabel(status_text);
}

void tmWMSBrowserFrame::_get_checked_layers() {
    // iterate the items and update the checked layers
    for (long i = 0; i < m_ctrl_layer_list->GetItemCount(); ++i) {
        int layer_index = m_ctrl_layer_list->GetItemData(i);
        if (m_ctrl_layer_list->IsItemChecked(i)) {
            if (m_checked_layers.Index(layer_index) == wxNOT_FOUND) {
                m_checked_layers.Add(layer_index);
            }
        } else {
            if (m_checked_layers.Index(layer_index) != wxNOT_FOUND) {
                m_checked_layers.Remove(layer_index);
            }
        }
    }
}

/// \brief Adds a layer to the list control.
void tmWMSBrowserFrame::add_layer_to_list(const wxString &layer_name, const wxString &layer_title,
                                          const wxString &layer_abstract, int layer_index) {
    m_ctrl_layer_list->InsertItem(m_ctrl_layer_list->GetItemCount(), wxEmptyString);
    m_ctrl_layer_list->SetItem(m_ctrl_layer_list->GetItemCount() - 1, 1, layer_name);
    m_ctrl_layer_list->SetItem(m_ctrl_layer_list->GetItemCount() - 1, 2, layer_title);
    m_ctrl_layer_list->SetItem(m_ctrl_layer_list->GetItemCount() - 1, 3, layer_abstract);
    m_ctrl_layer_list->SetItemData(m_ctrl_layer_list->GetItemCount() - 1, layer_index);
    if (m_checked_layers.Index(layer_index) != wxNOT_FOUND) {
        m_ctrl_layer_list->CheckItem(m_ctrl_layer_list->GetItemCount() - 1, true);
    } else {
        m_ctrl_layer_list->CheckItem(m_ctrl_layer_list->GetItemCount() - 1, false);
    }
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

    m_ctrl_append_to_project = new wxCheckBox(sbSizer2->GetStaticBox(), wxID_ANY, _("Append to project"),
                                              wxDefaultPosition, wxDefaultSize, 0);
    m_ctrl_append_to_project->SetValue(true);
    sbSizer2->Add(m_ctrl_append_to_project, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

    m_staticText5 = new wxStaticText(sbSizer2->GetStaticBox(), wxID_ANY, _("Projection:"), wxDefaultPosition,
                                     wxDefaultSize, 0);
    m_staticText5->Wrap(-1);
    sbSizer2->Add(m_staticText5, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

    wxArrayString m_ctrl_projectionChoices;
    m_ctrl_projection = new wxChoice(sbSizer2->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize,
                                     m_ctrl_projectionChoices, 0);
    m_ctrl_projection->SetSelection(0);
    sbSizer2->Add(m_ctrl_projection, 1, wxALL | wxALIGN_CENTER_VERTICAL, 5);

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

    wxBoxSizer *bSizer3;
    bSizer3 = new wxBoxSizer(wxHORIZONTAL);

    m_info_text_ctrl = new wxStaticText(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
    m_info_text_ctrl->Wrap(-1);
    bSizer3->Add(m_info_text_ctrl, 1, wxALL | wxALIGN_CENTER_VERTICAL, 5);


    bSizer1->Add(bSizer3, 0, wxEXPAND, 5);


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
