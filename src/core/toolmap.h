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

// comment doxygen


#ifndef TOOLMAP_H
#define TOOLMAP_H

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "../gui/project_def_dlg.h"
#include "../gui/logwindow_dlg.h"
#include "../gui/tocwindow_dlg.h"

#include <wx/mstream.h>

#define ID_MENU_NEW_PRJ_EMPTY 10019
#define ID_MENU_NEW_PRJ_EXISTING 10148
#define ID_MENU_OPEN_PRJ 10020
#define ID_MENU3 10126
#define ID_MENU_BACKUP_PRJ 10021
#define ID_MENU_RESTORE_PRJ 10110
#define ID_MENU_EXPORT_LAYER 10033
#define ID_MENU_EXPORT_FULL 10034
#define ID_MENU_PRJ_DEF 10229
#define ID_MENU_OBJ_DEF 10230
#define ID_MENU_PRJ_SETTINGS 10175
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

#define wxGetBitmapFromMemory(name) _wxGetBitmapFromMemory(name ## _png, sizeof(name ## _png))


class ToolMapApp : public wxApp
{
public:
	virtual bool OnInit();
	
};
IMPLEMENT_APP(ToolMapApp);


class ToolMapFrame: public wxFrame
{
public:
	ToolMapFrame(wxFrame *frame, const wxString& title,
			 wxPoint  pos,  wxSize  size);
	~ToolMapFrame();
	void PostInit();
	
	static wxMenuBar* CreateToolMapMenu();
	static wxToolBar * CreateToolMapToolBar(wxWindow * parent);
	
	
	
private:
	//void OnQuit(wxCommandEvent& event);
	//void OnAbout(wxCommandEvent& event);
	void OnNewProject(wxCommandEvent & event);
	void OnLogWindow(wxCommandEvent & event);
	void OnTocWindow (wxCommandEvent & event);
	
	ImportLog * m_LogWindow;
	TocWindowDlg * m_TocWindow;
	
	DECLARE_EVENT_TABLE();
};





#endif
