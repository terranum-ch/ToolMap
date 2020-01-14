/***************************************************************************
 queries_panel.h
 Deals with the queries panel
 -------------------
 copyright            : (C) 2008 CREALP Lucien Schreiber
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef _TM_QUERIESEDITOR_H_
#define _TM_QUERIESEDITOR_H_

#include <wx/listctrl.h>
#include <wx/statusbr.h>
#include <wx/tglbtn.h>

#include "../core/queriesbuilder.h"  // for query builder
#include "../gis/tmmanagerevent.h"   // for manager event
#include "listgenreport_status.h"    // for list with  status support
#include "managed_aui_wnd.h"
#include "queries_panel_dlg.h"  // for the queries edition dialog
#include "wxflatbutton.h"       // for flat button

class DataBaseTM;

class Queries_PANEL;

const int ID_QUERIESEDITOR = 10049;
const int ID_QUERIES_LIST = 10051;
const int ID_QUERIES_ADD = 10052;
const int ID_QUERIES_REMOVE = 10064;
const int ID_QUERIES_RUN = 10236;
const int ID_QUERIES_APPLY_SYMBOLOGY = 10066;
const int ID_QUERIES_EDIT = 10067;
#define SYMBOL_QUERIES_PANEL_STYLE wxCAPTION | wxRESIZE_BORDER | wxSYSTEM_MENU | wxCLOSE_BOX
#define SYMBOL_QUERIES_PANEL_TITLE _("Queries")
#define SYMBOL_QUERIES_PANEL_IDNAME ID_QUERIESEDITOR
#define SYMBOL_QUERIES_PANEL_SIZE wxSize(200, 100)
#define SYMBOL_QUERIES_PANEL_POSITION wxDefaultPosition

DECLARE_EVENT_TYPE(tmEVT_QUERY_MENU, -1)

class QueriesList : public ListGenReportWithDialog {
 private:
  DataBaseTM *m_pDB;
  tmSelectedDataMemory *m_Selected;
  Queries_PANEL *m_QueryPanel;

  virtual void BeforeAdding();

  virtual void AfterAdding(bool bRealyAddItem);

  virtual void BeforeDeleting();

  virtual void BeforeEditing();

  virtual void AfterEditing(bool bRealyEdited);

  virtual void OnDoubleClickItem(wxListEvent &event);

  void OnQueryEdit(wxCommandEvent &event);

  void OnQueryMenuUpdateUISelected(wxUpdateUIEvent &event);

  DECLARE_EVENT_TABLE();

 protected:
  virtual void OnContextMenu(wxListEvent &event);

 public:
  QueriesList(wxWindow *parent, Queries_PANEL *queryparent, wxWindowID id, wxArrayString *pColsName,
              wxArrayInt *pColsSize = NULL, wxSize size = wxDefaultSize);

  ~QueriesList();

  void OnContextualMenu(wxContextMenuEvent &event);

  // setter
  void SetDataBase(DataBaseTM *database) {
    m_pDB = database;
  }

  void SetSelected(tmSelectedDataMemory *selected) {
    m_Selected = selected;
  }

  virtual void AddItem();
};

/***************************************************************************/ /**
  @brief Display the Queries Panel
  @details Queries are immediatly added or removed from the database.
  @author Lucien Schreiber (c) CREALP 2008
  @date 09 November 2008
  *******************************************************************************/
class Queries_PANEL : public ManagedAuiWnd {
 private:
  wxAuiPaneInfo m_PaneInfo;
  DataBaseTM *m_pDB;
  QueriesList *m_QueriesList;
  wxWindow *m_ParentEvt;
  bool m_IsProjectOpen;
  tmTOCCtrl *m_TOC;

  /// Initialises member variables
  void InitMemberValues();

  // event function
  void OnPressQueryMenu(wxCommandEvent &event);

  DECLARE_EVENT_TABLE()

 public:
  /// Constructors
  Queries_PANEL(wxWindow *parent, wxWindowID id, wxAuiManager *auimanager);

  // public event function
  void OnAddQueries(wxCommandEvent &event);

  void OnRemoveQueries(wxCommandEvent &event);

  void OnRunQueries(wxCommandEvent &event);

  void OnQueryApplySymbology(wxCommandEvent &event);

  /// Destructor
  ~Queries_PANEL();

  /// Creates the controls and sizers
  wxSizer *CreateControls(wxWindow *parent, bool call_fit = true, bool set_sizer = true);

  void SetDataBase(DataBaseTM *database);

  void SetSelectedData(tmSelectedDataMemory *selected);

  void SetTOCCtrl(tmTOCCtrl *toc) {
    m_TOC = toc;
  }

  bool LoadQueries(DataBaseTM *database);

  void EnableQueriesPanel(bool projectopen = true);

  bool IsQuerySelected();
};

#endif
