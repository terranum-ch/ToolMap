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

// comment doxygen


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
#define SYMBOL_IMPORTLOG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_IMPORTLOG_TITLE _("Log window")
#define SYMBOL_IMPORTLOG_IDNAME ID_DLGLOG
#define SYMBOL_IMPORTLOG_SIZE wxDefaultSize
#define SYMBOL_IMPORTLOG_POSITION wxDefaultPosition


/*!
 * ImportLog class declaration
 */
class ImportLog: public wxDialog
{    
    DECLARE_DYNAMIC_CLASS( ImportLog )
    //DECLARE_EVENT_TABLE()

public:
    /// Constructors
    ImportLog();
    ImportLog( wxWindow* parent, wxWindowID id = SYMBOL_IMPORTLOG_IDNAME, const wxString& caption = SYMBOL_IMPORTLOG_TITLE, const wxPoint& pos = SYMBOL_IMPORTLOG_POSITION, const wxSize& size = SYMBOL_IMPORTLOG_SIZE, long style = SYMBOL_IMPORTLOG_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_IMPORTLOG_IDNAME, const wxString& caption = SYMBOL_IMPORTLOG_TITLE, const wxPoint& pos = SYMBOL_IMPORTLOG_POSITION, const wxSize& size = SYMBOL_IMPORTLOG_SIZE, long style = SYMBOL_IMPORTLOG_STYLE );

    /// Destructor
    ~ImportLog();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();


};

#endif

