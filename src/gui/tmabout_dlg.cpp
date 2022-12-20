/***************************************************************************
 tmabout_dlg.cpp
 about dialog (inspired by firefox about)
 -------------------
 copyright : (C) 2007 CREALP Lucien Schreiber
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

#include "tmabout_dlg.h"

#include "../core/toolmap.h"
#include "bitmaps.h"
#include "curl/curl.h"
#include "gdal_priv.h"
#include "geos_c.h"
#include "mysql.h"
#include "proj.h"
#include "version.h"

BEGIN_EVENT_TABLE(tmAboutDLG, wxDialog)
EVT_BUTTON(ID_ABOUT_BTN, tmAboutDLG::OnButton)
END_EVENT_TABLE()

tmAboutDLG::tmAboutDLG(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size,
                       long style)
    : wxDialog(parent, id, title, pos, size, style) {
    CreateControls(parent);
}

tmAboutDLG::~tmAboutDLG() {}

wxString tmAboutDLG::GetVersionText() {
    wxString myVersion = _("version ") + ToolMap_MAJOR_VERSION + "." + ToolMap_MINOR_VERSION + ".";
    myVersion.Append(GIT_NUMBER);
    myVersion.Append(_T(" - "));
    myVersion.Append(g_CodeName);
    return myVersion;
}

wxString tmAboutDLG::GetComponentsVersion() {
    wxString myVersion;
    // software version
    myVersion << "Version: " << ToolMap_MAJOR_VERSION << "." << ToolMap_MINOR_VERSION << "." << GIT_NUMBER << "\n";
    myVersion << "Revision: " << GIT_REV << "\n"
              << "Tag: " << GIT_TAG << "\n"
              << "Branch: " << GIT_BRANCH << "\n";

    // libs version
    myVersion << wxVERSION_STRING << "\n";
#ifdef _OPENMP
    myVersion << "OpenMP: " << _OPENMP << "\n";
#endif

    myVersion << _("Gdal: ") << GDAL_RELEASE_NAME << "\n";
    myVersion << _("Geos: ") << GEOS_VERSION << "\n";
    myVersion << _("Proj: ") << PROJ_VERSION_MAJOR << "." << PROJ_VERSION_MINOR << "." << PROJ_VERSION_PATCH << "\n";
    myVersion << _("Libcurl: ") << LIBCURL_VERSION << "\n";
    myVersion << _("MySQL: ") << mysql_get_client_info() << "\n";
    myVersion << wxGetOsDescription();
    return myVersion;
}

wxString tmAboutDLG::GetButtonText() {
    if (m_PanelLicence->IsShown())
        return _("Show components");
    else
        return _("Show license");
}

void tmAboutDLG::CreateControls(wxWindow* parent) {
    this->SetSizeHints(wxDefaultSize, wxDefaultSize);

    wxBoxSizer* bSizer36;
    bSizer36 = new wxBoxSizer(wxVERTICAL);

    wxPanel* myPanelImg;
    myPanelImg = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
    // myPanelImg->SetBackgroundColour(wxColour(255, 255, 255));
    // myPanelImg->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW));

    wxBoxSizer* bSizer37;
    bSizer37 = new wxBoxSizer(wxVERTICAL);

    wxStaticBitmap* m_bitmap1 = new wxStaticBitmap(myPanelImg, wxID_ANY, Bitmaps::GetLogo(wxSize(128, 128)),
                                                   wxDefaultPosition, wxSize(-1, -1), 0);
    // m_bitmap1->SetBackgroundColour(wxColour(255, 255, 255));
    bSizer37->Add(m_bitmap1, 0, wxALL | wxEXPAND, 5);

    m_TextVersion = new wxStaticText(myPanelImg, wxID_ANY, GetVersionText(), wxDefaultPosition, wxDefaultSize, 0);
    m_TextVersion->Wrap(-1);
    m_TextVersion->SetFont(wxFontInfo(18).Bold());

    bSizer37->Add(m_TextVersion, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);

    bSizer37->Add(0, 5, 1, wxEXPAND, 5);

    wxStaticText* m_staticText23;
    m_staticText23 = new wxStaticText(myPanelImg, wxID_ANY, _("Let's bring consistency into your maps"),
                                      wxDefaultPosition, wxDefaultSize, 0);
    m_staticText23->Wrap(-1);
    bSizer37->Add(m_staticText23, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);

    bSizer37->Add(0, 5, 1, wxEXPAND, 5);

    myPanelImg->SetSizer(bSizer37);
    myPanelImg->Layout();
    bSizer37->Fit(myPanelImg);
    bSizer36->Add(myPanelImg, 1, wxEXPAND, 5);

    m_PanelLicence = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
    // m_PanelLicence->SetBackgroundColour(wxColour(255, 255, 255));

    wxBoxSizer* bSizer40;
    bSizer40 = new wxBoxSizer(wxVERTICAL);

    wxStaticText* m_staticText22;
    wxString myLicenceTxt = _(
        " Copyright (c) 2022 TERRANUM\n Copyright (c) 2013 SWISSTOPO\n Copyright (c) 2013 CREALP \n\nThis program is "
        "free "
        "software; you can redistribute it and/or modify\nit under the terms of the GNU General Public License as "
        "published by\nthe Free Software Foundation; either version 2 of the License, or\n(at your option) any later "
        "version.\n\nThis program is distributed in the hope that it will be useful,\nbut WITHOUT ANY WARRANTY; "
        "without "
        "even the implied warranty of\nMERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\nGNU General "
        "Public "
        "License for more details.\n\nYou should have received a copy of the GNU General Public License\nalong with "
        "this "
        "program; if not, write to the Free Software\nFoundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.");
#if wxUSE_UNICODE
    wxString myCopyRightTxt = wxString::FromUTF8("\xc2\xa9");
    myLicenceTxt.Replace(_T("(c)"), myCopyRightTxt);
    myLicenceTxt.Replace(_T("(C)"), myCopyRightTxt);
#endif

    m_staticText22 = new wxStaticText(m_PanelLicence, wxID_ANY, myLicenceTxt, wxDefaultPosition, wxDefaultSize,
                                      wxALIGN_CENTRE);
    m_staticText22->Wrap(-1);
    wxFont myOrgFont = m_staticText22->GetFont();
    int mySize = myOrgFont.GetPointSize();
#ifdef __WINDOWS__
    mySize++;
#endif
    m_staticText22->SetFont(wxFontInfo(mySize - 1));

    bSizer40->Add(m_staticText22, 0, wxALL, 5);

    m_PanelLicence->SetSizer(bSizer40);
    m_PanelLicence->Layout();
    bSizer40->Fit(m_PanelLicence);
    bSizer36->Add(m_PanelLicence, 0, wxEXPAND, 5);

    m_PanelAuthor = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
    // m_PanelAuthor->SetBackgroundColour(wxColour(255, 255, 255));
    m_PanelAuthor->Hide();

    wxBoxSizer* bSizer41;
    bSizer41 = new wxBoxSizer(wxVERTICAL);

    wxTextCtrl* txtctrl = new wxTextCtrl(m_PanelAuthor, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize,
                                         wxTE_MULTILINE);
    txtctrl->SetValue(GetComponentsVersion());
    bSizer41->Add(txtctrl, 1, wxALL | wxEXPAND, 5);

    m_PanelAuthor->SetSizer(bSizer41);
    m_PanelAuthor->Layout();
    bSizer41->Fit(m_PanelAuthor);
    bSizer36->Add(m_PanelAuthor, 1, wxEXPAND, 5);

    wxPanel* m_PanelButton;
    m_PanelButton = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
    wxBoxSizer* bSizer38;
    bSizer38 = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer* bSizer39;
    bSizer39 = new wxBoxSizer(wxHORIZONTAL);

    wxHyperlinkCtrl* m_hyperlink2;
    m_hyperlink2 = new wxHyperlinkCtrl(m_PanelButton, wxID_ANY, _("www.toolmap.ch"), wxT("http://www.toolmap.ch"),
                                       wxDefaultPosition, wxDefaultSize, wxHL_DEFAULT_STYLE);
    bSizer39->Add(m_hyperlink2, 0, wxALL, 5);

    bSizer39->Add(0, 0, 1, wxEXPAND, 5);

    m_Button = new wxButton(m_PanelButton, ID_ABOUT_BTN, GetButtonText(), wxDefaultPosition, wxDefaultSize, 0);
    bSizer39->Add(m_Button, 0, wxALL, 5);

    bSizer38->Add(bSizer39, 1, wxEXPAND, 5);

    m_PanelButton->SetSizer(bSizer38);
    m_PanelButton->Layout();
    bSizer38->Fit(m_PanelButton);
    bSizer36->Add(m_PanelButton, 0, wxALL | wxEXPAND, 5);

    this->SetSizer(bSizer36);
    this->Layout();
    bSizer36->Fit(this);
    CenterOnParent(wxBOTH);
}

void tmAboutDLG::OnButton(wxCommandEvent& event) {
    bool myStatus = m_PanelLicence->IsShown();

    m_PanelLicence->Show(!myStatus);
    m_PanelAuthor->Show(myStatus);
    m_Button->SetLabel(GetButtonText());
    this->Layout();
    event.Skip();
}
