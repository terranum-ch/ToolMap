/***************************************************************************
								toolmap.h
				main file for toolmap 2, contain app and main frame
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

/*! \mainpage ToolMap 
 
 \section intro_sec Introduction
 
 This program is intended to be a full featured multi-layers GIS editor, implementing the
 SION Method. More informations on the method could be found here : http://www.crealp.ch
 
 This program was the result of a partnership between Swisstopo and the CREALP. For licence
 informations please refer to the corresponding section.
 
 \image html toolmap.jpg 
 
 \section doc Documentation topics
 To find our way in all the documentated class of the programm, We have sorted our
 documentation based on topics or part of the program. Some topics are presented here :
 
  
 - \subpage intro 
 - \subpage Prj_def "Project definition"
 - \subpage Prj_Edit_Obj "Object Definition"

 \section Licence 
 
 This program is free software; you can redistribute it and/or modify 
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or 
 (at your option) any later version.
 
 \author Lucien Schreiber (c) CREALP 2007
 \date  Since november 2007

 
 \page intro Introduction 
 During ToolMap 2 startup, the followed process is described hereafter :
 - ToolMapApp::OnInit() function is called and create the program frame (see class ToolMapFrame)
 - ToolMapFrame object is then initialised and create both the menu bar and the toolbar.
		- ToolMapFrame::CreateToolMapMenu
		- ToolMapFrame::CreateToolMapToolBar
 
 Those two last function were copy-pasted from DialogBlocks (http://www.anthemion.co.uk/dialogblocks/)
 a graphical UI designer.
 
 The program is then started and mainly waiting for event.
 */


#ifndef TOOLMAP_H
#define TOOLMAP_H

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <wx/sysopt.h>					// for dealing with systemoptions
#include <wx/toolbar.h>
#include "../gui/project_def_dlg.h"		// project definition dialog
#include "../gui/logwindow_dlg.h"		// log window definition
#include "../gui/tocwindow_dlg.h"		// Table of content (TOC) definition
#include "../database/db_project.h"
#include "../gui/menumanager.h"			// for menu management (enable / diable)

#include "../gui/attribution_obj_type.h"
#include "../gui/main_panel.h"
#include "../gui/projectnewexist_dlg.h"
#include "projectmanager.h"
#include <wx/fileconf.h>		// for using file configuration even on windows


//// temp for testing 
#include "diroperation.h"
//// end of testing

class MenuManager;
class ProjectManager;

#include <wx/mstream.h>


// items from menu project.
#define ID_MENU_NEW_PRJ_EMPTY		10300
#define ID_MENU_NEW_PRJ_EXISTING	10301
#define ID_MENU_OPEN_PRJ			10302
#define ID_MENU_RECENT				10303
#define ID_MENU_BACKUP_PRJ			10304
#define ID_MENU_RESTORE_PRJ			10305
#define ID_MENU_EXPORT			10306
#define ID_MENU_EXPORT_LAYER		10307
#define ID_MENU_EXPORT_FULL			10308
#define ID_MENU_PRJ_EDIT		10309
#define ID_MENU_PRJ_DEF				10310
#define ID_MENU_OBJ_DEF				10311
#define ID_MENU_PRJ_SETTINGS		10312

#define ID_MENU_ADD_SPATIAL_DATA 10022
#define ID_MENU_IMPORT_GIS_DATA 10112
#define ID_MENU_ZOOM_PREVIOUS_EXTEND 10023
#define ID_MENU_ZOOM 10164
#define ID_MENU_PAN 10165
#define ID_MENU_ZOOM_FIT 10113
#define ID_MENU_ZOOM_GIVEN_SCALE 10120
#define ID_MENU_ZOOM_COORDINATE 10210
#define ID_MENU_ZOOM_NEXT_SCALE 10184
#define ID_MENU_PREVIOUS_SCALE 10185
#define ID_MENU_SELECTION 10131
#define ID_MENU_SELECTED_LAYER 10208
#define ID_MENU_UNDO 10179
#define ID_MENU_REDO 10180
#define ID_MENU_DRAW 10176
#define ID_MENU_MODIFY 10177
#define ID_MENU_CUT_LINES 10178
#define ID_MENU_EDIT_VERTEX_POS 10211
#define ID_MENU_MERGE_LINES 10003
#define ID_MENU_CREATE_INTERSECTIONS 10174
#define ID_MENU_FLIP_LINE 10004
#define ID_MENU_DELETE_OBJ 10122
#define ID_MENU_ADJUST_SNAPPING 10001
#define ID_MENU_ATTRIB_TYPES 10166
#define ID_MENU_ATTRIB_ATTRIBUTES 10175
#define ID_MENU_ATTRIB_BATCH 10167
#define ID_MENU_COPY_PASTE_ATTRIB 10231
#define ID_MENU_SHORTCUTS 10183
#define ID_MENU_CHECK_GEOM 10123
#define ID_MENU_CHECK_DANGLING 10048
#define ID_MENU_SELECT 10163
#define ID_MENU_SELECT_ALL 10162
#define ID_MENU_SELECT_NONE 10160
#define ID_MENU_SELECT_INVERSE 10161
#define ID_MENU_QUERIES 10019
#define ID_MENUITEM11 10046
#define ID_MENUITEM12 10047
#define ID_MENU_TOC_WINDOW 10037
#define ID_MENU_INFO_WINDOW 10140
#define ID_MENU_LOG_WINDOW 10207
#define ID_MENU_CHECK_UPDATE 10125
#define ID_MENUITEM17 10077
#define ID_MENUITEM14 10072
#define ID_MENUITEM20 10111
#define ID_MENU6 10075
#define ID_MENUITEM16 10076
#define ID_MENUITEM45 10181
#define ID_MENUITEM46 10182
#define ID_MENUITEM18 10078

#define ID_TOOLBAR1 10038
#define ID_TOOL1 10071
#define ID_TOOL2 10073
#define ID_TOOL3 10074
#define ID_TOOL4 10193
#define ID_COMBOBOX2 10252
#define ID_TOOL5 10253
#define ID_TOOL6 10254
#define ID_TOOL7 10255
#define ID_TOOL8 10256
#define ID_TOOL9 10257
#define ID_TOOL10 10258
const wxString g_ProgName = _T("ToolMap 2.0.");

#define wxGetBitmapFromMemory(name) _wxGetBitmapFromMemory(name ## _png, sizeof(name ## _png))


/***************************************************************************//**
 @brief Implement the Application
 @author Lucien Schreiber (c) CREALP 2007
 @date 04 December 2007
 *******************************************************************************/
class ToolMapApp : public wxApp
{
public:
	virtual bool OnInit();
	
};
DECLARE_APP(ToolMapApp);




/***************************************************************************//**
 @brief All frame's related stuff
 @details This implement the wxFrame class. This is where all stuff related to
 the graphical aspect of the main programme's frame is defined
 @author Lucien Schreiber (c) CREALP 2007
 @date 04 December 2007
 *******************************************************************************/
class ToolMapFrame: public wxFrame
{
public:
	ToolMapFrame();
	ToolMapFrame(wxFrame *frame, const wxString& title,
			 wxPoint  pos,  wxSize  size);
	~ToolMapFrame();
	void PostInit();
	
	static wxMenuBar* CreateToolMapMenu();
	static wxToolBar * CreateToolMapToolBar(wxWindow * parent);
	
	
	
private:
	//PrjDefMemManage m_PrjDefinition;
	//DataBaseTM * m_Database;
	wxFileConfig * m_pConfig;
	
	wxAuiManager * m_AuiManager;
	AttribObjType_PANEL * m_AttribObjPanel;
	Main_PANEL * m_MainPanel;
	
	ProjectManager * m_PManager;
	MenuManager * m_MManager;
	
	
	void OnQuit(wxCloseEvent & event);
	//void OnAbout(wxCommandEvent& event);
	void OnNewProject(wxCommandEvent & event);
	void OnOpenProject (wxCommandEvent & event);
	void OnOpenRecentProject (wxCommandEvent & event);
	void OnEditProjectObjects (wxCommandEvent & event);
	void OnEditProjectSettings (wxCommandEvent & event);
	void OnEditProject (wxCommandEvent & event);
	void OnNewProjectExisting (wxCommandEvent & event);
	void OnLogWindow(wxCommandEvent & event);
	void OnTocWindow (wxCommandEvent & event);
	void OnIdleTimeUpdate(wxIdleEvent & event);
	void OnShowObjectAttributionWindow (wxCommandEvent & event);
	void OnBackupProject (wxCommandEvent & event);
	void OnUpdateAttributionObjects(wxCommandEvent & event);
	
	void OnShowQueryManager (wxCommandEvent & event);
	
	ImportLogDLG * m_LogWindow;
//#if (__WXMAC__)
//	TocWindowDlgMac * m_TocWindow;
//#else
	TocWindowDlgGen * m_TocWindow;
//#endif
	
	DECLARE_EVENT_TABLE();
	DECLARE_DYNAMIC_CLASS(ToolMapFrame)
};




#endif
