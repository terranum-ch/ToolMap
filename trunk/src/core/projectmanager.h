/***************************************************************************
								projectmanager.h
				Class used for opening, closing, backuping the project
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
 @file projectmanager.h
 @brief Deals with project operations
 @details This file implement the ProjectManager class and is used in response to menu event 
 to open, close, backup, create new projets.
 @author Lucien Schreiber (c) CREALP 2007
 @date 10 March 2008
 *******************************************************************************/



#ifndef _PROJECTMANAGER_H
#define _PROJECTMANAGER_H

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif


// include
#include "../database/db_project.h"			// for project in memory (creation)
#include "../database/database_tm.h"		// for database and TM database operations
#include "../gui/project_def_dlg.h"			// new project definition dialog
#include "../gui/objectattribution_dlg.h"	// for displaying object definition dlg.
#include "../gui/projectproperties_dlg.h"	// for displaying project properties dlg.
#include "backup.h"							// for backup and restore operations
#include "../gui/menumanager.h"					// for menu management (recent, greyed,...)

class MenuManager;
class ObjectManager;

/***************************************************************************//**
 @brief Project operations (new, open,...)
 @details This is used to implement all project operations such as creating new
 project, opening existing ToolMap project, closing project,...
 @author Lucien Schreiber (c) CREALP 2007
 @date 10 March 2008
 *******************************************************************************/
class ProjectManager : public wxObject
	{
	private:
		bool bProjectIsOpen;
		DataBaseTM * m_DB;
		wxWindow * m_Parent;
		MenuManager * m_pMManager;
		ObjectManager * m_Obj;
		
		DECLARE_CLASS(ProjectManager)
		
		bool IsDataBasePath(const wxString & path);
		
	public:
		ProjectManager(wxWindow * parent);
		~ProjectManager();
		
		// project operations 
		bool CreateNewProject();
		void CloseProject();
		bool OpenProject(const wxString & path);
		bool EditProjectObjectDefinition ();
		bool EditProjectSettings ();
		bool EditProject ();
		bool BackupProject ();
		
		// project infos
		wxString GetProjectName();
		
		// setter and getter
		void SetMenuManager(MenuManager * pMenu){m_pMManager = pMenu;}
		MenuManager * GetMenuManager() {return m_pMManager;}
		ObjectManager * GetObjectManager() {return m_Obj;}
	};



/***************************************************************************//**
 @brief Deals with objects in the attribution panels
 @author Lucien Schreiber (c) CREALP 2008
 @date 16 May 2008
 *******************************************************************************/
class ObjectManager : public wxObject 
	{
	private:
		AttribObjType_PANEL * m_panel;
		void InitValues();
		
		
	public:
		// ctor
		ObjectManager() {InitValues();}
		ObjectManager(AttribObjType_PANEL * panel)
		{
			InitValues();
			SetPanel(panel);
		}
		
		// setter
		void SetPanel (AttribObjType_PANEL * panel) {m_panel = panel;}
			
		
		// functions
		bool UpdatePointList(DataBaseTM * pDB);
		
	};


#endif
