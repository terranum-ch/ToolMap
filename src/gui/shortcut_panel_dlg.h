/***************************************************************************
							shortcut_panel_dlg.h
						Dialog box for editing shortcuts
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


#ifndef _TM_SHORTCUT_PANEL_DLG_H_
#define _TM_SHORTCUT_PANEL_DLG_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "tmchecklistbox.h"		// check list box
#include <wx/statline.h>		// static line
#include "shortcut_defs.h"		// for key definitions (F1,...)
#include "../database/database_tm.h"	// for databse acces 

#define ID_DLG_SHORTCUT_LIST 21000



class Shortcut_Panel_DLG : public wxDialog 
	{
	private:
		wxChoice* m_ShortcutKey;
		wxTextCtrl* m_ShortcutDescription;
		tmCheckListBox * m_TypeList;
		wxButton* m_SaveButton;
		
		DataBaseTM * m_pDB;
		
		// create controls
		void CreateControls();
		
	protected:
		
	public:
		Shortcut_Panel_DLG( wxWindow* parent, wxWindowID id = wxID_ANY,
						   const wxString& title = _("Edit Shortcuts"),
						   const wxPoint& pos = wxDefaultPosition,
						   const wxSize& size = wxDefaultSize,
						   long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER );
		~Shortcut_Panel_DLG();
		
		// key setter
		void SetKeyList (const wxArrayString & keys);
		void SetDataBase (DataBaseTM * database)
		{
			wxASSERT(database);
			m_pDB = database;
		}
		bool SetTypeList (DataBaseTM * database, int layer_type, int key = 0);
		
		// transfer data from windows
		virtual bool TransferDataFromWindow();
		virtual bool TransferDataToWindow();
		wxArrayLong m_CheckedTypes;
		int m_SelectedKey;
		wxString m_Description;
		
		
	};





#endif
