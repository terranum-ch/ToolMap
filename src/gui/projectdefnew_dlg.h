/***************************************************************************
 projectdefnew_dlg.h
 New Project creation first dialog
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

#ifndef _PROJECTDEFNEW_DLG_H_
#define _PROJECTDEFNEW_DLG_H_


#include "wx/statline.h"
#include "../core/wxdirpickerctrlbest.h"    // directory picker (best version)
#include "../core/tmvalidator.h"            // for controls validation
#include "../core/prjdefmemmanage.h"    // for storing project def in memory

#define ID_DLGPD_PROJ_PATH_NEW 10019
#define ID_DLGPD_PROJ_NAME_NEW 10020
#define ID_DLGPD_PROJ_UNIT_NEW 10021
#define ID_DLGPD_PROJ_PROJECTION_NEW 10022
#define ID_PROJECTDEFNEW 10018
#define ID_TEXTCTRL1 10114
#define ID_TEXTCTRL2 10115
#define SYMBOL_PROJECTDEFNEW_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxTAB_TRAVERSAL
#define SYMBOL_PROJECTDEFNEW_TITLE _("New project")
#define SYMBOL_PROJECTDEFNEW_IDNAME ID_PROJECTDEFNEW
#define SYMBOL_PROJECTDEFNEW_SIZE wxSize(400, 300)
#define SYMBOL_PROJECTDEFNEW_POSITION wxDefaultPosition

class ProjectDefNew : public wxDialog
{
DECLARE_DYNAMIC_CLASS(ProjectDefNew)

DECLARE_EVENT_TABLE()
    PrjDefMemManage *m_PrjDefinition;

    // private functions
    bool CheckIdleRules();

    void OnIdleWait(wxIdleEvent &event);

public:
    /// Constructors
    ProjectDefNew();

    ProjectDefNew(wxWindow *parent,
                  PrjDefMemManage *PrjDefinition,
                  wxWindowID id = SYMBOL_PROJECTDEFNEW_IDNAME,
                  const wxString &caption = SYMBOL_PROJECTDEFNEW_TITLE,
                  const wxPoint &pos = SYMBOL_PROJECTDEFNEW_POSITION,
                  const wxSize &size = SYMBOL_PROJECTDEFNEW_SIZE,
                  long style = SYMBOL_PROJECTDEFNEW_STYLE);

    /// Creation
    bool Create(wxWindow *parent, wxWindowID id = SYMBOL_PROJECTDEFNEW_IDNAME,
                const wxString &caption = SYMBOL_PROJECTDEFNEW_TITLE,
                const wxPoint &pos = SYMBOL_PROJECTDEFNEW_POSITION,
                const wxSize &size = SYMBOL_PROJECTDEFNEW_SIZE,
                long style = SYMBOL_PROJECTDEFNEW_STYLE);

    /// Destructor
    ~ProjectDefNew();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

    PrjDefMemManage *GetProjectDefinition()
    { return m_PrjDefinition; }

    wxDirPickerCtrlBest *m_DlgPD_Proj_Path;
    wxTextCtrl *m_DlgPd_Proj_Name;
    wxChoice *m_DlgPd_Proj_Unit;
    wxChoice *m_DlgPd_Proj_Projection;
    wxButton *m_DlgPd_Button_Ok;

    virtual bool TransferDataFromWindow();
};

#endif

