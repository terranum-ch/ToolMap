/***************************************************************************
							projectnewexist_dlg.h
                    Display New Project from existing dialog
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


#ifndef PROJECTNEWEXIST_DLG_H
#define PROJECTNEWEXIST_DLG_H

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif


#include "wx/statline.h"
#include <wx/dirdlg.h>				// for directory selection dialog
#include <wx/dir.h>					// for checking if a directory exists



#define ID_NEWPROJECTFROMEXISTING 10036
#define ID_TEXTCTRL15 10149
#define ID_DLGNPE_NEW_PRJ_PATH_BTN 10150
#define ID_TEXTCTRL16 10151
#define ID_STATICLINE1 10152
#define ID_TEXTCTRL17 10153
#define ID_DLGNPE_OLD_PRJ_PATH_BTN 10154
#define ID_DLGNPE_RADIO_OPTIONS 10155
#define SYMBOL_PROJECTNEWEXISTDLG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_PROJECTNEWEXISTDLG_TITLE _("New project from existing")
#define SYMBOL_PROJECTNEWEXISTDLG_IDNAME ID_NEWPROJECTFROMEXISTING
#define SYMBOL_PROJECTNEWEXISTDLG_SIZE wxSize(400, 300)
#define SYMBOL_PROJECTNEWEXISTDLG_POSITION wxDefaultPosition


class ProjectNewExistDLG: public wxDialog
	{    
	private:
		void OnButtonPathNew (wxCommandEvent & event);
		void OnButtonPathOld (wxCommandEvent & event);
		void OnIdleWait(wxIdleEvent & event);
		
		
		DECLARE_DYNAMIC_CLASS( ProjectNewExistDLG )
		DECLARE_EVENT_TABLE()
		
	public:

		ProjectNewExistDLG();
		ProjectNewExistDLG( wxWindow* parent,
						   wxWindowID id = SYMBOL_PROJECTNEWEXISTDLG_IDNAME, 
						   const wxString& caption = SYMBOL_PROJECTNEWEXISTDLG_TITLE, 
						   const wxPoint& pos = SYMBOL_PROJECTNEWEXISTDLG_POSITION, 
						   const wxSize& size = SYMBOL_PROJECTNEWEXISTDLG_SIZE, 
						   long style = SYMBOL_PROJECTNEWEXISTDLG_STYLE );
		

		bool Create( wxWindow* parent, 
					wxWindowID id = SYMBOL_PROJECTNEWEXISTDLG_IDNAME, 
					const wxString& caption = SYMBOL_PROJECTNEWEXISTDLG_TITLE,
					const wxPoint& pos = SYMBOL_PROJECTNEWEXISTDLG_POSITION, 
					const wxSize& size = SYMBOL_PROJECTNEWEXISTDLG_SIZE, 
					long style = SYMBOL_PROJECTNEWEXISTDLG_STYLE );
		

		~ProjectNewExistDLG();
		

		void Init();
		

		void CreateControls();
		
	
		virtual bool TransferDataFromWindow();
		
		wxTextCtrl* m_DlgPNE_New_Prj_Path;
		wxTextCtrl* m_DlgPNE_New_Prj_Name;
		wxTextCtrl* m_DlgPNE_Old_Prj_Path;
		wxRadioBox* m_DlgPNE_Options;
		wxButton* m_DlgPNE_Btn_OK;
		
		wxString m_New_Prj_Path;
		wxString m_New_Prj_Name;
		wxString m_Old_Prj_Path;
		unsigned int m_sOption_Type;
		
	};






#endif
