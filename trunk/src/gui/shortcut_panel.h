/***************************************************************************
								shortcut_panel.h
							Display the Shortcut panel
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



#ifndef _TM_SHORTCUT_PANEL_H
#define _TM_SHORTCUT_PANEL_H

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "managed_aui_wnd.h"
#include "listgenreport.h"
#include "wxflatbutton.h"


#define IDDLG_SHORT_TARGET 21000
#define IDDLG_SHORT_LIST 21001
#define IDDLG_SHORT_ADD_BTN 21002
#define IDDLG_SHORT_DEL_BTN 21003
#define IDDLG_SHORT_EDIT_BTN 21004
#define SHORTCUT_PANEL_TITLE _("Shortcuts")
#define SHORTCUT_PANEL_SIZE wxSize(250, 200)


class Shortcuts_PANEL : public ManagedAuiWnd 
	{
		
	private:
		//Control Member
		wxChoice* m_TargetChoice;
		wxListCtrl* m_ListShortcuts;
		
		// other member
		wxWindow * m_ParentEvt;
		wxAuiPaneInfo m_PaneInfo;
		
		
		
		// event function for panel
		void OnChangeTarget( wxCommandEvent& event ){ event.Skip(); }
		void OnShortcutAdd( wxCommandEvent& event ){ event.Skip(); }
		void OnShortcutDel( wxCommandEvent& event ){ event.Skip(); }
		void OnShortcutEdit( wxCommandEvent& event ){ event.Skip(); }
		
		
		/// Creates the controls and sizers
		wxSizer * CreateControls(wxWindow * parent,
								 bool call_fit = true,
								 bool set_sizer = true);
		
		void InitMemberValues();
		
		DECLARE_EVENT_TABLE()
		
	public:
		Shortcuts_PANEL( wxWindow* parent, wxWindowID id, wxAuiManager * auimanager);
		~Shortcuts_PANEL();
		
		
		
	};




#endif
