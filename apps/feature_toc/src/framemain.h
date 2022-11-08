#ifndef _MAINFRAME_H
#define _MAINFRAME_H

#include "wx/wxprec.h"

#ifndef WX_PRECOMP

#include "wx/wx.h"

#endif

#include <wx/aboutdlg.h>
#include <wx/colordlg.h>
#include <wx/filename.h>
#include <wx/splitter.h>
#include <wx/srchctrl.h>
#include <wx/systhemectrl.h>

#include "tocctrl.h"

class FrameMain : public wxFrame {
 public:
  FrameMain(const wxString &title);
  ~FrameMain();

 private:
  void _connect_events();

  void _create_controls();

  void _create_statusbar();

  void _create_menubar();

  void _add_tree_data();

  void on_about(wxCommandEvent &event);
  void on_change_color(wxCommandEvent &event);
  void on_add_item(wxCommandEvent & event);
  void on_add_group(wxCommandEvent & event);
  void on_remove(wxCommandEvent & event);

  TocCtrl *m_toc_ctrl;
  wxTextCtrl *m_text_ctrl;
  wxLog *m_log_old;

  wxMenuItem* m_menu_item_colour;
  wxMenuItem* m_menu_item_add_item;
  wxMenuItem* m_menu_item_add_group;
  wxMenuItem* m_menu_item_remove_selected;
};

#endif
