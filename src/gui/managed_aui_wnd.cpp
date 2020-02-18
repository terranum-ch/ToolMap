/***************************************************************************
 managed_aui_wnd.cpp
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

#include "managed_aui_wnd.h"

ManagedAuiWnd::ManagedAuiWnd(wxAuiManager *AuiManager) : m_AuiMgr(AuiManager) {
  InitValues();
  // wxLogDebug(_T("Value of m_AuiMgr (parent) = %p"), m_AuiPaneInfo);
  // wxLogDebug(_T("ManagedAuiWnd Ctor called %p"), m_AuiMgr);
}

ManagedAuiWnd::~ManagedAuiWnd() {}

void ManagedAuiWnd::InitValues() {
  m_AuiPanelName = _T("");
}

void ManagedAuiWnd::AddManagedPane(wxWindow *window, const wxAuiPaneInfo &paneinfo, bool bShow) {
  m_AuiMgr->AddPane(window, paneinfo);
  if (bShow) {
    ShowPanel();
  } else
    HidePanel();
}

void ManagedAuiWnd::ShowPanel() {
  wxASSERT_MSG(!m_AuiPanelName.IsEmpty(), _T("Define value for m_auiPanelName in the child class..."));
  m_AuiMgr->GetPane(m_AuiPanelName).Show();
  m_AuiMgr->Update();
}

void ManagedAuiWnd::HidePanel() {
  wxASSERT_MSG(!m_AuiPanelName.IsEmpty(), _T("Define value for m_auiPanelName in the child class..."));
  m_AuiMgr->GetPane(m_AuiPanelName).Hide();
  m_AuiMgr->Update();
}

bool ManagedAuiWnd::IsPanelShown() {
  wxASSERT_MSG(!m_AuiPanelName.IsEmpty(), _T("Define value for m_auiPanelName in the child class..."));
  if (m_AuiMgr->GetPane(m_AuiPanelName).IsShown()) {
    return TRUE;
  }
  return FALSE;
}
