#include "framecomponent.h"
#include "version.h"

#include "../img/misc_bmp.h"
#include "gdal_priv.h"
#include "geos_c.h"
#include "curl/curl.h"
#include "mysql.h"
#include "proj.h"

FrameComponents::FrameComponents(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size,
                       long style)
    : wxDialog(parent, id, title, pos, size, style) {
  _create_controls();

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
  m_ctrl_txt->SetValue(myVersion);

  m_ctrl_bmp->SetBitmap(*_img_icon_toolmap);

  Layout();
  GetSizer()->Fit(this);
}

FrameComponents::~FrameComponents() {}

void FrameComponents::_create_controls() {
  SetSizeHints(wxDefaultSize, wxDefaultSize);

  wxBoxSizer* bSizer1;
  bSizer1 = new wxBoxSizer(wxVERTICAL);

  wxPanel* m_panel1;
  m_panel1 = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
  wxBoxSizer* bSizer2;
  bSizer2 = new wxBoxSizer(wxVERTICAL);

  wxStaticText* m_staticText1;
  m_staticText1 = new wxStaticText(m_panel1, wxID_ANY, _("ToolMap"), wxDefaultPosition, wxDefaultSize, 0);
  m_staticText1->Wrap(-1);
  bSizer2->Add(m_staticText1, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);

  m_ctrl_bmp = new wxStaticBitmap(m_panel1, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize(-1, -1), 0);
  bSizer2->Add(m_ctrl_bmp, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);

  m_ctrl_txt = new wxTextCtrl(m_panel1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(350, 200), wxTE_MULTILINE);
  bSizer2->Add(m_ctrl_txt, 1, wxALL | wxEXPAND, 5);

  m_ctrl_btn = new wxStdDialogButtonSizer();
  m_ctrl_btnOK = new wxButton(m_panel1, wxID_OK);
  m_ctrl_btn->AddButton(m_ctrl_btnOK);
  m_ctrl_btn->Realize();

  bSizer2->Add(m_ctrl_btn, 0, wxALL | wxEXPAND, 5);

  m_panel1->SetSizer(bSizer2);
  m_panel1->Layout();
  bSizer2->Fit(m_panel1);
  bSizer1->Add(m_panel1, 1, wxEXPAND, 5);

  SetSizer(bSizer1);
  Layout();
  bSizer1->Fit(this);

  // Centre(wxBOTH);
}