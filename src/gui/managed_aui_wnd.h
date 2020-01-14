/***************************************************************************
 managed_aui_wnd.h
 Main class for managed aui windows
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

#ifndef MANAGED_AUI_WND_H
#define MANAGED_AUI_WND_H

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

// Include wxWidgets' headers
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/aui/aui.h>

class ManagedAuiWnd : public wxEvtHandler {
 protected:
  wxAuiManager *m_AuiMgr;
  wxString m_AuiPanelName;

  // private member function
  void InitValues();

 public:
  ManagedAuiWnd(wxAuiManager *AuiManager);

  ~ManagedAuiWnd();

  void AddManagedPane(wxWindow *window, const wxAuiPaneInfo &paneinfo, bool bShow = FALSE);

  void ShowPanel();

  void HidePanel();

  bool IsPanelShown();
};

#endif
