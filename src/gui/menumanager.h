/***************************************************************************
								menumanager.h
                    Deals with menu activities (enable / disable)
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
 @file menumanager.h
 @brief Deals with menu activation
 @details This class is the main point for enabling / disabling menu based on
 events.
 @author Lucien Schreiber (c) CREALP 2007
 @date 12 March 2008
 *******************************************************************************/



#ifndef _MENUMANAGER_H_
#define _MENUMANAGER_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "../core/toolmap.h"
#include <wx/docview.h>			// for recent documents
#include <wx/fileconf.h>		// for using file config even on windows 


enum MENUSTATUS
{
	MENU_DB_OPENED		=		1,
	MENU_DB_CLOSED		=		0,
	MENU_DB_SWITCH  =			MENU_DB_OPENED,
};


/***************************************************************************//**
 @brief Deals with menu activation
 @details This class is the main point for enabling / disabling menu based on
 events. Basic workflow is use SetStatus() function for passing one of the
 MENUSTATUS item and then in the main idle function call UpdateMenus() for
 enabling and disabling menu
 @author Lucien Schreiber (c) CREALP 2007
 @date 12 March 2008
*******************************************************************************/
class MenuManager : public wxObject
	{
	private:
		int m_MenuStatus;
		wxMenuBar * m_MenuBar;
				
		// store internally the status of each option
		bool bMenu_DB_IsOpen;
		
		// members for recent files
		wxFileHistory * m_pFilesHistory;
		wxFileConfig * m_pConfig;

		
		// functions used internally for uptading menus items
		void UpdateMenuProject();
		void UpdateMenuData ();
		void UpdateMenuView ();
		
		DECLARE_CLASS(MenuManager);
		
	public:
		MenuManager(wxMenuBar * menubar, wxFileConfig * configfile);
		~MenuManager();
		
		void SetStatus(MENUSTATUS flags);
		void UpdateMenusStatus();
		
		// deals with recent files
		void InitializeRecentFilesHistory();
		void TerminateRecentFilesHistory();
		void AddFileToRecent (const wxString & spath);
		void RemoveFileFromRecent (int fileid);
		bool GetRecentFile (wxString & filepath, int fileid);
		
		// Editing 
		void EditingStatus(bool started);
		
	};





#endif
