#ifndef _MAINFRAME_H
#define _MAINFRAME_H

#include "wx/wxprec.h"

#ifndef WX_PRECOMP

#include "wx/wx.h"

#endif

#include <wx/aboutdlg.h>
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

  TocCtrl *m_toc_ctrl;
  wxTextCtrl *m_text_ctrl;
  wxLog *m_log_old;
};

#endif
