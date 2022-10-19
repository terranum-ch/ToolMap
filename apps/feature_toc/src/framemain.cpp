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

    m_toc_ctrl = new TocCtrl(this, wxID_ANY);
    bSizer1->Add(m_toc_ctrl, 1, wxEXPAND, 5);

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
    Bind(wxEVT_DATAVIEW_ITEM_BEGIN_DRAG, &FrameMain::on_dnd_begin, this, m_toc_ctrl->GetId());
    Bind(wxEVT_DATAVIEW_ITEM_DROP_POSSIBLE, &FrameMain::on_dnd_possible, this, m_toc_ctrl->GetId());
    Bind(wxEVT_DATAVIEW_ITEM_DROP, &FrameMain::on_dnd_drop, this, m_toc_ctrl->GetId());
}

/// Adding some test data to the Tree Control
void FrameMain::_add_tree_data() {
    m_toc_ctrl->add_test_data();
}

void FrameMain::on_dnd_begin(wxDataViewEvent &event) {
    wxDataViewItem item( event.GetItem() );

    auto * my_model = dynamic_cast<TocCtrlModel *>(m_toc_ctrl->GetModel());

    // only allow drags for item, not containers
    if (my_model->IsContainer( item ) )
    {
        wxLogMessage("Forbidding starting dragging");
        event.Veto();
        return;
    }

    TocCtrlModelNode * my_node = (TocCtrlModelNode*) item.GetID();
    // MyMusicTreeModelNode *node = (MyMusicTreeModelNode*) item.GetID();
    wxTextDataObject *obj = new wxTextDataObject;
    obj->SetText( my_node->m_title );
    event.SetDataObject( obj );
    event.SetDragFlags(wxDrag_AllowMove); // allows both copy and move

    wxLogMessage("Starting dragging \"%s\"", my_node->m_title);
}

void FrameMain::on_dnd_possible(wxDataViewEvent &event) {
    if (event.GetDataFormat() != wxDF_UNICODETEXT)
        event.Veto();
    else
        event.SetDropEffect(wxDragMove); // check 'move' drop effect
}

void FrameMain::on_dnd_drop(wxDataViewEvent &event) {
    wxDataViewItem item( event.GetItem() );

    if (event.GetDataFormat() != wxDF_UNICODETEXT)
    {
        event.Veto();
        return;
    }

    // Note that instead of recreating a new data object here we could also
    // retrieve the data object from the event, using its GetDataObject()
    // method. This would be more efficient as it would avoid copying the text
    // one more time, but would require a cast in the code and we don't really
    // care about efficiency here.
    wxTextDataObject obj;
    obj.SetData( wxDF_UNICODETEXT, event.GetDataSize(), event.GetDataBuffer() );

    auto * my_model = dynamic_cast<TocCtrlModel *>(m_toc_ctrl->GetModel());


    if ( item.IsOk() )
    {
        if (my_model->IsContainer(item))
        {
            wxLogMessage("Container");
            //wxLogMessage("Text '%s' dropped in container '%s' (proposed index = %i)",
            //             obj.GetText(), my_model->GetTitle(item), event.GetProposedDropIndex());
        }
        else
            wxLogMessage("Item");
            //wxLogMessage("Text '%s' dropped on item '%s'", obj.GetText(), my_model->GetTitle(item));
    }
    else
        wxLogMessage("Background");
        //wxLogMessage("Text '%s' dropped on background (proposed index = %i)", obj.GetText(), event.GetProposedDropIndex());
}
