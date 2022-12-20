/***************************************************************************
 project_def.h
 Project definition dialog
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

/***************************************************************************/ /**
  @file project_def_dlg.h
  @brief Definition of the Project definition dialog
  @details This file provides the class needed for dealing with project
  definition (layers, object and optional field). This dialog is called either
  for a new project or changing the parameters of an existing one. The two class
  implemented are : ProjectDefDLG for the dialog and ProjectDefList for the list
  inside the dialog.
  @author Lucien Schreiber (c) CREALP 2007
  @date 04 December 2007
  *******************************************************************************/

#ifndef PROJECT_DEF_DLG_H
#define PROJECT_DEF_DLG_H

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

// Include wxWidgets' headers
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/listctrl.h>

#include "listgenreport.h"
#include "project_def_layers_dlg.h"
#include "wxflatbutton.h"
// #include <wx/dirdlg.h> // for directory selection dialog
#include "../core/wxdirpickerctrlbest.h"  // directory picker (best version)
#include "listgenreport_status.h"         // list with status

/*!
 * Forward declarations
 */

////@begin forward declarations
class ProjectDefList;

class wxToggleButton;
////@end forward declarations

/*!
 * Control identifiers
 */
#define ID_DLGPD 10000
#define ID_DLGPD_NOTEBOOK 10262
#define ID_DLGPD_PANEL_PROJ 10263
#define ID_DLGPD_PROJ_PATH 10267
#define ID_DLGPD_PROJ_NAME 10268
#define ID_DLGPD_PROJ_UNIT 10269
#define ID_DLGPD_PROJ_PROJECTION 10270
#define ID_DLGPD_PROJ_AUTHOR 10219
#define ID_DLGPD_PROJ_COMMENT 10273
#define ID_DLGPD_PANEL_SPAT 10119
#define ID_DLGPD_SPAT_MODEL_LIST 10156
#define ID_DLGPD_SPAT_MDL_ADD 10271
#define ID_DLGPD_SPAT_MDL_DEL 10272
#define SYMBOL_PROJECTDEFDLG_STYLE wxCAPTION | wxRESIZE_BORDER | wxSYSTEM_MENU | wxCLOSE_BOX
#define SYMBOL_PROJECTDEFDLG_TITLE _("Project Definition")
#define SYMBOL_PROJECTDEFDLG_IDNAME ID_DLGPD
#define SYMBOL_PROJECTDEFDLG_SIZE wxSize(400, 300)
#define SYMBOL_PROJECTDEFDLG_POSITION wxDefaultPosition

/***************************************************************************/ /**
  @brief Definition of the Project definition dialog
  @details This provides the class needed for dealing with project definition
  (layers, object and optional field). This dialog is called either for a new
  project or changing the parameters of an existing one. This dialog implement a
  ProjectDefList List. for displaying the spatial themes of the project
  @author Lucien Schreiber (c) CREALP 2007
  @date 04 December 2007
  *******************************************************************************/
class ProjectDefDLG : public wxDialog {
  private:
    // Project memory class
    PrjDefMemManage* m_pPrjDefinition;

    void OnAddLayer(wxCommandEvent& event);

    void OnRemoveLayer(wxCommandEvent& event);

    void OnIdleWait(wxIdleEvent& event);

    bool CheckIdleRules();

    DECLARE_DYNAMIC_CLASS(ProjectDefDLG);
    DECLARE_EVENT_TABLE();

    bool m_bIsModeEditing;

    void DisableControlsForEdition();

  public:
    /// Constructors
    ProjectDefDLG();

    ProjectDefDLG(wxWindow* parent, PrjDefMemManage* myPrjDefinition, bool isEditingMode = FALSE,
                  wxWindowID id = SYMBOL_PROJECTDEFDLG_IDNAME, const wxString& caption = SYMBOL_PROJECTDEFDLG_TITLE,
                  const wxPoint& pos = SYMBOL_PROJECTDEFDLG_POSITION, const wxSize& size = SYMBOL_PROJECTDEFDLG_SIZE,
                  long style = SYMBOL_PROJECTDEFDLG_STYLE);

    /// Creation
    bool Create(wxWindow* parent, wxWindowID id = SYMBOL_PROJECTDEFDLG_IDNAME,
                const wxString& caption = SYMBOL_PROJECTDEFDLG_TITLE,
                const wxPoint& pos = SYMBOL_PROJECTDEFDLG_POSITION, const wxSize& size = SYMBOL_PROJECTDEFDLG_SIZE,
                long style = SYMBOL_PROJECTDEFDLG_STYLE);

    /// Destructor
    ~ProjectDefDLG();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

    void SetNotebook(int notebooknumber);

    // void RemoveObjFromArray();
    virtual bool TransferDataFromWindow();

    virtual bool TransferDataToWindow();

    bool IsEditMode() {
        return m_bIsModeEditing;
    }

    wxPanel* m_DlgPd_Panel_Proj;
    // wxDirPickerCtrlBest* m_DlgPD_Proj_Path;
    // wxTextCtrl* m_DlgPd_Proj_Name;
    wxChoice* m_DlgPd_Proj_Unit;
    wxChoice* m_DlgPd_Proj_Projection;
    wxTextCtrl* m_DlgPd_Proj_Author;
    wxTextCtrl* m_DlgPd_Proj_Comment;
    wxPanel* m_DlgPd_Panel_Spatial;
    ProjectDefList* m_DlgPd_Stat_Model_List;
    wxFlatButton* m_DlgPd_Spat_Mdl_Add;
    wxFlatButton* m_DljPd_Spat_Mdl_Del;
    wxButton* m_DlgPd_Button_Ok;
    wxStatusBar* m_DlgPd_Status;
};

/***************************************************************************/ /**
  @brief List of the Project Definition Dialog
  @details The list managed here deals with project's spatial layers and is part
  of the ProjectDefDLG.
  @author Lucien Schreiber (c) CREALP 2007
  @date 04 December 2007
  *******************************************************************************/
class ProjectDefList : public ListGenReportWithStatus {
  private:
    bool m_bIsModeEditing;
    // void OnMySelectionChange (wxListEvent & event);

    wxChoice* m_ChoiceToChange;
    PrjMemLayersArray* m_LayersArray;
    // create object for storing layers data

    ProjectDefLayersDlg* m_LayersDialog;

    PrjDefMemManage* m_pPrjDefinition;
    ProjectDefMemoryLayers* m_LayersObj;

    virtual void BeforeAdding();

    virtual void AfterAdding(bool bRealyAddItem);

    virtual void BeforeDeleting();

    virtual void BeforeEditing();

    virtual void AfterEditing(bool bRealyEdited);

    bool m_bAscending[2];

    virtual void OnSortColumns(wxListEvent& event);

  public:
    static const int ID_PARAMLIST;

    ProjectDefList(wxWindow* parent, wxWindowID id, wxSize size, ProjectDefDLG* myParentDlg);

    ~ProjectDefList();

    void PassPrjDefToList(PrjDefMemManage* myPrjMemManage) {
        m_pPrjDefinition = myPrjMemManage;
    }

    // int GetParamType (wxString myTextParam);
    //
    // wxString GetParamType (int imyType);
    //
    // void AddItemToParamList(wxString myValue, wxString myComment, ACOMMENTPOSITION type);
    // void SetChoiceList (wxChoice * myChoiceList);
    // DECLARE_EVENT_TABLE();
};

#endif
