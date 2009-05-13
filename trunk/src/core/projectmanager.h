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
//#include "../database/db_project.h"			// for project in memory (creation)
#include "../database/database_tm.h"		// for database and TM database operations
#include "../gui/project_def_dlg.h"			// new project definition dialog
#include "../gui/objectattribution_dlg.h"	// for displaying object definition dlg.
#include "../gui/projectproperties_dlg.h"	// for displaying project properties dlg.
#include "../gui/projectdefnew_dlg.h"		// for the first dialog (creating the db)
#include "backup.h"							// for backup and restore operations
#include "../gui/menumanager.h"				// for menu management (recent, greyed,...)
#include <wx/busyinfo.h>					// informations message for busy time
#include "../gis/tmprogress.h"				// progress indicator
#include "../gis/tmattributionmanager.h"	// for attribution manager
#include "../gui/queries_panel.h"			// for queries panel
#include "../gui/shortcut_panel.h"			// for shortcut panel
#include "../gui/snapping_panel.h"			// for snapping panel
#include "../gis/tmeditmanager.h"			// for editing stuff
#include "../gui/tmtoolmanager.h"			// for tool manager (dangling nodes)

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
		wxStatusBar * m_ParentStatus;
		tmLayerManager * m_LayerManager;
		tmAttributionManager * m_AttribManager;
		Queries_PANEL * m_QueriesPanel;
		Shortcuts_PANEL * m_ShortcutPanel;
		Snapping_PANEL * m_SnappingPanel;
		tmEditManager * m_EditManager;
		PrjDefMemManage * m_PrjMem;
		tmToolManager * m_ToolManager;
		
		DECLARE_CLASS(ProjectManager)
		
		bool IsDataBasePath(const wxString & path);
		
	public:
		ProjectManager(wxWindow * parent);
		~ProjectManager();
		
		// project operations 
		bool CreateNewProject();
		void CloseProject();
		int OpenProject(const wxString & path);
		bool EditProjectObjectDefinition ();
		bool EditProjectSettings ();
		bool EditProject ();
		bool BackupProject ();
		
		bool TempTempInitTOC ();
		
		// project infos
		wxString GetProjectName();
		
		// setter and getter
		void SetMenuManager(MenuManager * pMenu){m_pMManager = pMenu;}
		MenuManager * GetMenuManager() {return m_pMManager;}
		ObjectManager * GetObjectManager() {return m_Obj;}
		void SetStatusBar (wxStatusBar * status) {m_ParentStatus = status;}
		void SetAttributionManager (tmAttributionManager * pAManager) {m_AttribManager = pAManager;}
		void SetQueriesPanel (Queries_PANEL * queriespanel) {m_QueriesPanel = queriespanel;}
		void SetShortcutPanel (Shortcuts_PANEL * panel) {m_ShortcutPanel = panel;}
		void SetSnappingPanel (Snapping_PANEL * panel) {m_SnappingPanel = panel;}
		void SetEditManager (tmEditManager * editmanager) {m_EditManager = editmanager;}
		DataBaseTM * GetDatabase() {return m_DB;}
		void SetToolManager (tmToolManager * tm){m_ToolManager = tm;}
		
		void SetLayerManager(tmLayerManager * layermanager){m_LayerManager = layermanager;}
		bool IsProjectOpen () {return bProjectIsOpen;}
		
		// project defintion into memory
		bool LoadProjectDefintion (short int message = 0);
		PrjDefMemManage * GetMemoryProjectDefinition();
		
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
		bool UpdateObjectLists(DataBaseTM * pDB);
	
		
	};


#endif
