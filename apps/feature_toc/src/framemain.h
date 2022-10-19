#ifndef _MAINFRAME_H
#define _MAINFRAME_H

#include "wx/wxprec.h"

#ifndef WX_PRECOMP

#include "wx/wx.h"

#endif

#include <wx/filename.h>
#include <wx/splitter.h>
#include <wx/srchctrl.h>
#include <wx/systhemectrl.h>

#include <wx/aboutdlg.h>
#include "tocctrl.h"

class FrameMain : public wxFrame {
public:
    FrameMain(const wxString &title);


private:
//  void _create_menubar();
//  void _create_statusbar();
    void _connect_events();

    void _create_controls();

    void _create_statusbar();

    void _create_menubar();

    void _add_tree_data();

    void on_about(wxCommandEvent & event);

    void on_dnd_begin(wxDataViewEvent & event);
    void on_dnd_possible(wxDataViewEvent & event);
    void on_dnd_drop(wxDataViewEvent & event);

//  void _create_toolbar();
//  void _assign_image_to_treectrl();
//  void _update_title();


    TocCtrl * m_toc_ctrl;
    wxTextCtrl *m_textCtrl;
};

#endif
