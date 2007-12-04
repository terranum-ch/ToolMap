/***************************************************************************
								tocwindow_dlg.h
                    Display the TOC, try using the wxDrawerWindow class
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


#ifndef TOCWINDOW_DLG_H
#define TOCWINDOW_DLG_H

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <wx/mac/carbon/drawer.h>
#include "wx/treectrl.h"
#include "wx/tglbtn.h"

#define ID_TOCWINDOW_DLG 10035
#define ID_TREECTRL1 10127
#define ID_DLGTOC_ADD 10147
#define ID_DLGTOC_REMOVE 10145
#define SYMBOL_TOCWINDOW_DLG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_TOCWINDOW_DLG_TITLE _("Table of content")
#define SYMBOL_TOCWINDOW_DLG_IDNAME ID_TOCWINDOW_DLG
#define SYMBOL_TOCWINDOW_DLG_SIZE wxSize(200, -1)
#define SYMBOL_TOCWINDOW_DLG_POSITION wxDefaultPosition


class TocWindowDlg : public wxDrawerWindow
{
	private:
		DECLARE_DYNAMIC_CLASS( TocWindowDlg )
	
	public:
		TocWindowDlg();
		TocWindowDlg(wxWindow * parent, wxWindowID id=ID_TOCWINDOW_DLG,
			const wxString & title=SYMBOL_TOCWINDOW_DLG_TITLE);
		~TocWindowDlg(); 
		
	private:
		/// Creates the controls and sizers
		void CreateControls();
		void Init();
};



#endif
