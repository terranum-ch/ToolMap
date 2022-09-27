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


class FrameMain : public wxFrame {
 public:
  FrameMain(const wxString& title);



 private:
//  void _create_menubar();
//  void _create_statusbar();
//  void _connect_events();
  void _create_controls();
//  void _create_toolbar();
//  void _assign_image_to_treectrl();
//  void _update_title();


  wxString m_soft_name = _("Bookon");
};

#endif
