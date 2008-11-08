/***************************************************************************
 queries_panel.h
 Deals with the queries panel
 -------------------
 copyright            : (C) 2008 CREALP Lucien Schreiber 
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

#ifndef _TM_QUERIESEDITOR_H_
#define _TM_QUERIESEDITOR_H_


#include "wx/listctrl.h"
#include "wx/tglbtn.h"
#include "wx/statusbr.h"
#include "managed_aui_wnd.h"
#include "wxflatbutton.h"		// for flat button

#define ID_QUERIESEDITOR 10049
#define ID_LISTCTRL4 10051
#define ID_TOGGLEBUTTON13 10052
#define ID_TOGGLEBUTTON14 10064
#define ID_TOGGLEBUTTON15 10236
#define ID_BUTTON 10000
#define ID_STATUSBAR1 10065
#define SYMBOL_QUERIES_PANEL_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_QUERIES_PANEL_TITLE _("Queries Editor")
#define SYMBOL_QUERIES_PANEL_IDNAME ID_QUERIESEDITOR
#define SYMBOL_QUERIES_PANEL_SIZE wxSize(200, 100)
#define SYMBOL_QUERIES_PANEL_POSITION wxDefaultPosition



class Queries_PANEL: public ManagedAuiWnd
{
private:
	wxAuiPaneInfo m_PaneInfo;
	
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    Queries_PANEL(  wxWindow* parent, wxWindowID id, wxAuiManager * auimanager);

    /// Creation
   // bool Create( wxWindow* parent, wxWindowID id, wxAuiManager * auimanager);

    /// Destructor
    ~Queries_PANEL();

    /// Initialises member variables
    void InitMemberValues();

    /// Creates the controls and sizers
    wxSizer * CreateControls(wxWindow * parent,
							 bool call_fit = true,
							 bool set_sizer = true);

};

#endif


