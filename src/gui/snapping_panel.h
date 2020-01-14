/***************************************************************************
 snapping_panel.h
 Deals with the snapping panel, used for setting the
 snapping for layers
 -------------------
 copyright            : (C) 2007 CREALP Lucien Schreiber
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef _SNAPPING_PANEL_H_
#define _SNAPPING_PANEL_H_

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/spinctrl.h>

#include "../core/tmsnappingmemory.h"  // store snapping status into memory
#include "listgenreport.h"             // listgenreport support
#include "managed_aui_wnd.h"
#include "wxflatbutton.h"  // for flat button

class DataBaseTM;

class DataListReportCtrl;

#define ID_SNAP_TOLERENCE_TXT 22000
#define ID_SNAP_LIST 22001
#define ID_SNAP_ADD 22002
#define ID_SNAP_REMOVE 22003
#define ID_SNAP_CLEAR 22004
#define SNAPPING_PANEL_TITLE _("Snapping")
#define SNAPPING_PANEL_SIZE wxSize(230, 150)

class tmRenderer;

class tmSnapping_DLG : public wxDialog {
 public:
  tmSnapping_DLG(wxWindow *parent, wxWindowID id = wxID_ANY, const wxString &title = wxEmptyString,
                 const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize,
                 long style = wxDEFAULT_DIALOG_STYLE);

  ~tmSnapping_DLG();

  virtual bool TransferDataToWindow();

  virtual bool TransferDataFromWindow();

  int m_SnapStatus;
  wxString m_LayerName;

 private:
  wxStaticText *m_LayerNameCtrl;
  wxRadioButton *m_SnapNoneCtrl;
  wxRadioButton *m_SnapBeginEndCtrl;
  wxRadioButton *m_SnapAllCtrl;
};

/***************************************************************************/ /**
  @brief The panel used for setting the snapping
  @author Lucien Schreiber (c) CREALP 2009
  @date 19 January 2009
  *******************************************************************************/
class Snapping_PANEL : public ManagedAuiWnd {
 private:
  wxSpinCtrl *m_TolerenceCtrl;
  DataListReportCtrl *m_ListCtrl;
  DataBaseTM *m_pDB;
  tmRenderer *m_Renderer;
  wxWindow *m_ParentEvt;
  wxAuiPaneInfo m_PaneInfo;
  tmSnappingMemory m_SnappingMemory;

  wxPanel *_CreateControls();

  void OnUpdateTolerence(wxSpinEvent &event);

  void OnContextualMenu(wxContextMenuEvent &event);

  void OnDClickList(wxListEvent &event);

  DECLARE_EVENT_TABLE()

 public:
  Snapping_PANEL(wxWindow *parent, wxWindowID id, wxAuiManager *auimanager);

  ~Snapping_PANEL();

  void SetDataBase(DataBaseTM *database) {
    m_pDB = database;
  }

  void SetRendererRef(tmRenderer *renderer) {
    m_Renderer = renderer;
  }

  void LoadSnappingStatus();

  bool SaveSnappingStatus();

  void OnAddSnapping(wxCommandEvent &event);

  void OnRemoveSnapping(wxCommandEvent &event);

  void OnUpdateUIContextualMenuAdd(wxUpdateUIEvent &event);

  void OnUpdateUIContextualMenuRemove(wxUpdateUIEvent &event);

  tmSnappingMemory *GetSnappingMemoryRef() {
    return &m_SnappingMemory;
  }
};

#endif
