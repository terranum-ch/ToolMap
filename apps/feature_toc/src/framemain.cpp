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

  m_log_old = wxLog::SetActiveTarget(new wxLogTextCtrl(m_text_ctrl));
  wxLog::GetActiveTarget()->SetLogLevel(wxLOG_Debug);
  wxLogMessage("This is the log window");
}

void FrameMain::_create_controls() {
  this->SetSizeHints(wxDefaultSize, wxDefaultSize);

  wxBoxSizer *bSizer1;
  bSizer1 = new wxBoxSizer(wxHORIZONTAL);

  m_toc_ctrl = new TocCtrl(this, wxID_ANY);
  bSizer1->Add(m_toc_ctrl, 1, wxEXPAND, 5);

  m_text_ctrl = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
  bSizer1->Add(m_text_ctrl, 1, wxEXPAND, 5);

  this->SetSizer(bSizer1);
  this->Layout();

  this->Centre(wxBOTH);
}

void FrameMain::_create_statusbar() {
  CreateStatusBar(2);
  SetStatusBarPane(-1);
  wxString my_status_text = wxString::Format(_("Version: %s.%s.%s (%s)"), feature_toc_MAJOR_VERSION,
                                             feature_toc_MINOR_VERSION, GIT_NUMBER, GIT_REV);
  SetStatusText(my_status_text, 1);
}

void FrameMain::_create_menubar() {
  wxMenuBar* m_menubar;
  m_menubar = new wxMenuBar( 0 );
  wxMenu* m_menu_Tools;
  m_menu_Tools = new wxMenu();
  m_menu_item_colour = new wxMenuItem( m_menu_Tools, wxID_ANY, wxString( wxT("Set toc color...") ) , wxEmptyString, wxITEM_NORMAL );
  m_menu_Tools->Append( m_menu_item_colour );

  wxMenuItem* m_menu_item_about;
  m_menu_item_about = new wxMenuItem( m_menu_Tools, wxID_ABOUT, wxString( wxT("About...") ) , wxEmptyString, wxITEM_NORMAL );
  m_menu_Tools->Append( m_menu_item_about );

  m_menubar->Append( m_menu_Tools, wxT("Tools") );

  wxMenu* m_menu_data;
  m_menu_data = new wxMenu();
  m_menu_item_add_item = new wxMenuItem( m_menu_data, wxID_ANY, wxString( wxT("Add item...") ) + wxT('\t') + wxT("Ctrl+N"), wxEmptyString, wxITEM_NORMAL );
  m_menu_data->Append( m_menu_item_add_item );

  m_menu_item_add_group = new wxMenuItem( m_menu_data, wxID_ANY, wxString( wxT("Add group...") ) + wxT('\t') + wxT("Ctrl+G"), wxEmptyString, wxITEM_NORMAL );
  m_menu_data->Append( m_menu_item_add_group );

  m_menu_data->AppendSeparator();

  m_menu_item_remove_selected = new wxMenuItem( m_menu_data, wxID_ANY, wxString( wxT("Remove selected") ) , wxEmptyString, wxITEM_NORMAL );
  m_menu_data->Append( m_menu_item_remove_selected );

  m_menubar->Append( m_menu_data, wxT("Data") );

  this->SetMenuBar( m_menubar );
}

void FrameMain::on_about(wxCommandEvent &event) {
  wxAboutDialogInfo aboutInfo;
  aboutInfo.SetName("feature toc");
  wxString myVersion(feature_toc_MAJOR_VERSION);
  myVersion << "." << feature_toc_MINOR_VERSION;
  aboutInfo.SetVersion(myVersion);
  aboutInfo.SetDescription(_("feature toc test"));
  aboutInfo.SetCopyright("(C) 2022");
  wxAboutBox(aboutInfo);
}

void FrameMain::_connect_events() {
  Bind(wxEVT_MENU, &FrameMain::on_about, this, wxID_ABOUT);
  Bind(wxEVT_MENU, &FrameMain::on_change_color, this, m_menu_item_colour->GetId());
  Bind(wxEVT_MENU, &FrameMain::on_add_item, this, m_menu_item_add_item->GetId());
  Bind(wxEVT_MENU, &FrameMain::on_add_group, this, m_menu_item_add_group->GetId());
}

/// Adding some test data to the Tree Control
void FrameMain::_add_tree_data() {
  m_toc_ctrl->add_test_data();
}

FrameMain::~FrameMain() {
  delete wxLog::SetActiveTarget(m_log_old);
}

void FrameMain::on_change_color(wxCommandEvent &event) {
  wxColour my_actual_colour = m_toc_ctrl->GetColourNormal();
  wxColour my_toc_colour = wxGetColourFromUser(this, my_actual_colour, "Select TOC colour");
  m_toc_ctrl->SetColour(my_toc_colour);
}

/// Add an item to the toc. If nothing selected, the item is added to the end,
/// otherwise, the item is added to the selected node or selected parent node
/// \param event
void FrameMain::on_add_item(wxCommandEvent &event) {
  // get new node information
  wxString new_node_name = wxGetTextFromUser("Node name");
  if (new_node_name.IsEmpty()){
    return;
  }
  wxString my_layer_types [] = {"Shapefile", "Database", "Image", "Web"};
  int my_index = wxGetSingleChoiceIndex("Select layer type", "Layer type", wxArrayString(sizeof (my_layer_types) / sizeof (wxString),my_layer_types));
  if (my_index == -1){
    return;
  }
  my_index++;

  // get selected node
  wxDataViewItem my_sel_item = m_toc_ctrl->GetSelection();
  auto * my_model = m_toc_ctrl->GetTocModel();
  if (!my_sel_item.IsOk()){
    wxLogMessage("Nothing selected, adding item to the end of the TOC");
    my_model->NodeAdd(TocCtrlModel::ConvertFromwxDataViewItem(my_model->GetRoot()), new_node_name, true, my_index, false);
  }
  else {
    auto * my_node = TocCtrlModel::ConvertFromwxDataViewItem(my_sel_item);
    if (!my_node->IsContainer()){
      my_node = my_node->GetParent();
    }
    my_model->NodeAdd(my_node, new_node_name, true, my_index, false);
  }
}

void FrameMain::on_add_group(wxCommandEvent &event) {

}
