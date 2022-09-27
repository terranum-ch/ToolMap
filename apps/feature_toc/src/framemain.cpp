#include "framemain.h"

#include "bitmaps.h"

extern const char *feature_toc_MAJOR_VERSION;
extern const char *feature_toc_MINOR_VERSION;
extern const char *GIT_REV;
extern const char *GIT_TAG;
extern const char *GIT_BRANCH;
extern const char *GIT_NUMBER;

FrameMain::FrameMain(const wxString &title) : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(900, 700)) {
    wxInitAllImageHandlers();
    _create_controls();
    _create_statusbar();
    _create_menubar();
    _connect_events();

    SetIcon(wxBitmapBundle::FromSVG(feature_toc_bitmaps::app_icon, wxSize(32, 32)).GetIcon(wxSize(32, 32)));

    _add_tree_data();
}

void FrameMain::_create_controls() {
    this->SetSizeHints(wxDefaultSize, wxDefaultSize);

    wxBoxSizer *bSizer1;
    bSizer1 = new wxBoxSizer(wxHORIZONTAL);

    m_TreeListCtrl = new wxTreeListCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTL_DEFAULT_STYLE);
    m_TreeListCtrl->AppendColumn(wxT("Column1"), wxCOL_WIDTH_DEFAULT, wxALIGN_LEFT, wxCOL_RESIZABLE);
    m_TreeListCtrl->AppendColumn(wxT("Column2"), wxCOL_WIDTH_DEFAULT, wxALIGN_LEFT, wxCOL_RESIZABLE);

    bSizer1->Add(m_TreeListCtrl, 1, wxEXPAND, 5);

    m_textCtrl = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
    bSizer1->Add(m_textCtrl, 1, wxEXPAND, 5);


    this->SetSizer(bSizer1);
    this->Layout();

    this->Centre(wxBOTH);
}

void FrameMain::_create_statusbar() {
    CreateStatusBar(2);
    SetStatusBarPane(-1);
    SetStatusText(
            wxString::Format(_("Version: %s.%s.%s (%s)"), feature_toc_MAJOR_VERSION, feature_toc_MINOR_VERSION,
                             GIT_NUMBER, GIT_REV),
            1);
}

/// Adding some test data to the Tree Control
void FrameMain::_add_tree_data() {

}

void FrameMain::_create_menubar() {
    wxMenuBar *m_menubar;
    m_menubar = new wxMenuBar(0);
    wxMenu *m_menu_Tools;
    m_menu_Tools = new wxMenu();
    wxMenuItem *m_menu_about;
    m_menu_about = new wxMenuItem(m_menu_Tools, wxID_ABOUT, wxString(wxT("About...")), wxEmptyString, wxITEM_NORMAL);
    m_menu_Tools->Append(m_menu_about);

    m_menubar->Append(m_menu_Tools, wxT("Tools"));

    this->SetMenuBar(m_menubar);
}

void FrameMain::on_about(wxCommandEvent & event) {
 wxAboutDialogInfo aboutInfo;
    aboutInfo.SetName("feature toc");
    wxString myVersion (feature_toc_MAJOR_VERSION);
    myVersion << "." << feature_toc_MINOR_VERSION;
    aboutInfo.SetVersion(myVersion);
    aboutInfo.SetDescription(_("feature toc test"));
    aboutInfo.SetCopyright("(C) 2022");
    wxAboutBox(aboutInfo);
}

void FrameMain::_connect_events() {
    Bind(wxEVT_MENU, &FrameMain::on_about, this, wxID_ABOUT);
}