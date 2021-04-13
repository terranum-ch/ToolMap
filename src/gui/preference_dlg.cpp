/***************************************************************************
 preference_dlg.h
 Display Preference dialog
 -------------------
 copyright            : (C) 2011 CREALP Lucien Schreiber
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

#include "preference_dlg.h"

PreferenceDLG::PreferenceDLG(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &pos,
                             const wxSize &size, long style)
    : wxDialog(parent, id, title, pos, size, style) {
  _CreateControls();
}

PreferenceDLG::~PreferenceDLG() {}

void PreferenceDLG::_CreateControls() {
  this->SetSizeHints(wxDefaultSize, wxDefaultSize);

  wxBoxSizer *bSizer5;
  bSizer5 = new wxBoxSizer(wxVERTICAL);

  wxNotebook *m_notebook2;
  m_notebook2 = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0);
  wxPanel *m_panel2;
  m_panel2 = new wxPanel(m_notebook2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
  wxBoxSizer *bSizer3;
  bSizer3 = new wxBoxSizer(wxVERTICAL);

  wxFlexGridSizer *fgSizer2;
  fgSizer2 = new wxFlexGridSizer(4, 2, 0, 0);
  fgSizer2->AddGrowableCol(1);
  fgSizer2->SetFlexibleDirection(wxBOTH);
  fgSizer2->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

  wxStaticText *m_staticText5;
  m_staticText5 = new wxStaticText(m_panel2, wxID_ANY, _("Selection color:"), wxDefaultPosition, wxDefaultSize, 0);
  m_staticText5->Wrap(-1);
  fgSizer2->Add(m_staticText5, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

  m_SelColourCtrl =
      new wxColourPickerCtrl(m_panel2, wxID_ANY, *wxBLACK, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE);
  fgSizer2->Add(m_SelColourCtrl, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

  fgSizer2->Add(0, 0, 1, wxEXPAND, 5);

  m_SelOutlineCtrl = new wxCheckBox(m_panel2, wxID_ANY, _("Display outline"), wxDefaultPosition, wxDefaultSize, 0);
  fgSizer2->Add(m_SelOutlineCtrl, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

  fgSizer2->Add(0, 0, 1, wxEXPAND, 5);

  m_RelPathCtrl = new wxCheckBox(m_panel2, wxID_ANY, _("Save relative paths"), wxDefaultPosition, wxDefaultSize, 0);
  fgSizer2->Add(m_RelPathCtrl, 0, wxALL, 5);

  fgSizer2->Add(0, 0, 1, wxEXPAND, 5);

  m_BuildOverviewCtrl = new wxCheckBox(m_panel2, wxID_ANY, _("Build overviews"));
  fgSizer2->Add(m_BuildOverviewCtrl, 0, wxALL, 5);

  bSizer3->Add(fgSizer2, 1, wxEXPAND, 5);

  m_panel2->SetSizer(bSizer3);
  m_panel2->Layout();
  bSizer3->Fit(m_panel2);
  m_notebook2->AddPage(m_panel2, _("General"), false);
  wxPanel *m_panel3;
  m_panel3 = new wxPanel(m_notebook2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
  wxBoxSizer *bSizer6;
  bSizer6 = new wxBoxSizer(wxVERTICAL);

  m_UpdateCheckCtrl =
      new wxCheckBox(m_panel3, wxID_ANY, _("Check for updates at startup"), wxDefaultPosition, wxDefaultSize, 0);
  m_UpdateCheckCtrl->SetValue(true);
  bSizer6->Add(m_UpdateCheckCtrl, 0, wxALL, 5);

  wxStaticText *m_staticText7;
  m_staticText7 = new wxStaticText(m_panel3, wxID_ANY, _("Proxy informations:"), wxDefaultPosition, wxDefaultSize, 0);
  m_staticText7->Wrap(-1);
  bSizer6->Add(m_staticText7, 0, wxALL, 5);

  m_ProxyInfoCtrl = new wxTextCtrl(m_panel3, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
  bSizer6->Add(m_ProxyInfoCtrl, 0, wxALL | wxEXPAND, 5);

  wxStaticText *m_staticText8;
  m_staticText8 = new wxStaticText(m_panel3, wxID_ANY, _("Please use the following form: myproxy.com:8080"),
                                   wxDefaultPosition, wxDefaultSize, 0);
  m_staticText8->Wrap(-1);
  bSizer6->Add(m_staticText8, 0, wxALL, 5);

  m_panel3->SetSizer(bSizer6);
  m_panel3->Layout();
  bSizer6->Fit(m_panel3);
  m_notebook2->AddPage(m_panel3, _("Updates"), false);

  // DEBUG PANEL
  wxPanel *m_panel_debug = new wxPanel(m_notebook2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
  wxBoxSizer *bSizer51;
  bSizer51 = new wxBoxSizer(wxVERTICAL);

  m_ctrl_debug_error =
      new wxCheckBox(m_panel_debug, wxID_ANY, _("Log database errors"), wxDefaultPosition, wxDefaultSize, 0);
  bSizer51->Add(m_ctrl_debug_error, 0, wxALL, 5);

  m_ctrl_debug_query = new wxCheckBox(m_panel_debug, wxID_ANY, _("Log database queries (may slow down ToolMap)"),
                                      wxDefaultPosition, wxDefaultSize, 0);
  bSizer51->Add(m_ctrl_debug_query, 0, wxALL, 5);

  bSizer51->Add(0, 0, 1, wxEXPAND, 5);

  wxStaticText *m_staticText51;
  m_staticText51 = new wxStaticText(m_panel_debug, wxID_ANY, _("Restart ToolMap for these options to take effect."),
                                    wxDefaultPosition, wxDefaultSize, 0);
  m_staticText51->Wrap(-1);
  bSizer51->Add(m_staticText51, 0, wxALL, 5);

  m_panel_debug->SetSizer(bSizer51);
  m_panel_debug->Layout();
  bSizer51->Fit(m_panel_debug);
  m_notebook2->AddPage(m_panel_debug, _("Debug"), false);

  bSizer5->Add(m_notebook2, 1, wxEXPAND | wxALL, 5);

  wxStdDialogButtonSizer *m_sdbSizer2;
  wxButton *m_sdbSizer2OK;
  wxButton *m_sdbSizer2Cancel;
  m_sdbSizer2 = new wxStdDialogButtonSizer();
  m_sdbSizer2OK = new wxButton(this, wxID_OK);
  m_sdbSizer2->AddButton(m_sdbSizer2OK);
  m_sdbSizer2Cancel = new wxButton(this, wxID_CANCEL);
  m_sdbSizer2->AddButton(m_sdbSizer2Cancel);
  m_sdbSizer2->Realize();
  bSizer5->Add(m_sdbSizer2, 0, wxEXPAND | wxALL, 5);

  this->SetSizer(bSizer5);
  this->Layout();
  bSizer5->Fit(this);

  this->Centre(wxBOTH);
}

bool PreferenceDLG::TransferDataToWindow() {
  wxConfigBase *myConfig = wxFileConfig::Get();
  wxASSERT(myConfig);
  bool bCheckStartup = myConfig->ReadBool("UPDATE/check_on_start", true);
  wxString myProxyInfo = myConfig->Read("UPDATE/proxy_info", wxEmptyString);

  wxString mySelColorText = myConfig->Read("GENERAL/selection_color", wxEmptyString);
  bool mySelHalo = myConfig->ReadBool("GENERAL/selection_halo", false);
  bool myRelPath = myConfig->ReadBool("GENERAL/relative_path", true);

  wxColour mySelColor = *wxRED;
  if (mySelColorText != wxEmptyString) {
    mySelColor.Set(mySelColorText);
  }

  bool bCreateIndex = myConfig->ReadBool("SPATIAL_INDEX/create_index", true);

  m_BuildOverviewCtrl->SetValue(bCreateIndex);
  m_SelColourCtrl->SetColour(mySelColor);
  m_SelOutlineCtrl->SetValue(mySelHalo);
  m_RelPathCtrl->SetValue(myRelPath);
  m_UpdateCheckCtrl->SetValue(bCheckStartup);
  m_ProxyInfoCtrl->SetValue(myProxyInfo);
  return true;
}

bool PreferenceDLG::TransferDataFromWindow() {
  wxConfigBase *myConfig = wxFileConfig::Get();
  wxASSERT(myConfig);
  myConfig->Write("UPDATE/check_on_start", m_UpdateCheckCtrl->GetValue());
  myConfig->Write("UPDATE/proxy_info", m_ProxyInfoCtrl->GetValue());

  myConfig->Write("GENERAL/selection_color", m_SelColourCtrl->GetColour().GetAsString());
  myConfig->Write("GENERAL/selection_halo", m_SelOutlineCtrl->GetValue());
  myConfig->Write("GENERAL/relative_path", m_RelPathCtrl->GetValue());

  myConfig->Write("SPATIAL_INDEX/create_index", m_BuildOverviewCtrl->GetValue());
  return true;
}
