/***************************************************************************
								logwindow_dlg.h
							Display the log windows
                             -------------------
    copyright            : (C) 2007 CREALP Lucien Schreiber 
    email                : lucien.schreiber at crealp dot vs dot ch
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/***************************************************************************//**
 @file logwindow_dlg.h
 @brief Definition of the Log window dialog
 @author Lucien Schreiber (c) CREALP 2007
 @date 04 December 2007
 *******************************************************************************/


#ifndef LOGWINDOW_DLG_H
#define LOGWINDOW_DLG_H

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#define ID_DLGLOG 10157
#define ID_DLGLOG_TXT 10158
#define SYMBOL_IMPORTLOG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxFRAME_FLOAT_ON_PARENT
#define SYMBOL_IMPORTLOG_TITLE _("Log window")
#define SYMBOL_IMPORTLOG_IDNAME ID_DLGLOG
#define SYMBOL_IMPORTLOG_SIZE wxDefaultSize
#define SYMBOL_IMPORTLOG_POSITION wxDefaultPosition


/***************************************************************************//**
 @brief Definition of the Log window dialog
 @details Derived from wxDialog, this class implement the Log Window.
 @author Lucien Schreiber (c) CREALP 2007
 @date 04 December 2007
 *******************************************************************************/
class ImportLogDLG : public wxDialog
{
private:

    void OnCloseLogWindow(wxCommandEvent &event);

DECLARE_DYNAMIC_CLASS(ImportLog);
DECLARE_EVENT_TABLE();

public:
    /// Default Constructors
    ImportLogDLG();

    /// Use this constructor for creating the dialog
    ImportLogDLG(wxWindow *parent, wxWindowID id = SYMBOL_IMPORTLOG_IDNAME,
                 const wxString &caption = SYMBOL_IMPORTLOG_TITLE, const wxPoint &pos = SYMBOL_IMPORTLOG_POSITION,
                 const wxSize &size = SYMBOL_IMPORTLOG_SIZE, long style = SYMBOL_IMPORTLOG_STYLE);

    /// Window creation
    bool
    Create(wxWindow *parent, wxWindowID id = SYMBOL_IMPORTLOG_IDNAME, const wxString &caption = SYMBOL_IMPORTLOG_TITLE,
           const wxPoint &pos = SYMBOL_IMPORTLOG_POSITION, const wxSize &size = SYMBOL_IMPORTLOG_SIZE,
           long style = SYMBOL_IMPORTLOG_STYLE);

    /// Destructor
    ~ImportLogDLG();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();


};

#endif

