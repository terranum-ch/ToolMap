/***************************************************************************
								toolmap.cpp
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



#include "toolmap.h"
#include "svn_version.h" // version number definition




// toolbar images
#include "../img/tmimgfunc.h"	// for image processing
#include "../img/img_tool1.cpp"
#include "../img/img_tool2.cpp"
#include "../img/img_tool3.cpp"
#include "../img/img_tool4.cpp"
#include "../img/img_tool5.cpp"
#include "../img/img_tool6.cpp"
#include "../img/img_tool7.cpp"
#include "../img/img_tool8.cpp"
#include "../img/img_tool9.cpp"
#include "../img/img_tool10.cpp"
// icon image
#include "../img/img_icon32.cpp"



IMPLEMENT_APP(ToolMapApp);
 
bool ToolMapApp::OnInit()
/* APPLICATION INITIALISATION */
{
	// add handler for PNG embedded images (toolbar)
	wxImage::AddHandler(new wxPNGHandler);

	
	//wxString myFrameName = _T("ToolMap 2.0.");
	// myFrameName.Append(SVN_VERSION);
	ToolMapFrame* frame = new ToolMapFrame(NULL, g_ProgName + SVN_VERSION,wxDefaultPosition, wxSize(900,500));
	
	
	frame->Show(true);
	
	return true;
}

IMPLEMENT_DYNAMIC_CLASS(ToolMapFrame, wxFrame)

BEGIN_EVENT_TABLE (ToolMapFrame, wxFrame)
	EVT_MENU (ID_MENU_NEW_PRJ_EMPTY, ToolMapFrame::OnNewProject)
	EVT_MENU (ID_MENU_NEW_PRJ_EXISTING, ToolMapFrame::OnNewProjectExisting )
	EVT_MENU (ID_MENU_LOG_WINDOW, ToolMapFrame::OnLogWindow)
	EVT_MENU (ID_MENU_TOC_WINDOW, ToolMapFrame::OnTocWindow)
	EVT_MENU (ID_MENU_OPEN_PRJ, ToolMapFrame::OnOpenProject)
	EVT_MENU (ID_MENU_OBJ_DEF, ToolMapFrame::OnEditProjectObjects)
	EVT_MENU (ID_MENU_PRJ_SETTINGS, ToolMapFrame::OnEditProjectSettings )
	EVT_MENU (ID_MENU_PRJ_DEF, ToolMapFrame::OnEditProject)
	EVT_MENU (ID_MENU_BACKUP_PRJ, ToolMapFrame::OnBackupProject)
	EVT_MENU (ID_MENU_COPY_PASTE_ATTRIB, ToolMapFrame::OnUpdateAttributionObjects)
	EVT_MENU_RANGE (wxID_FILE1, wxID_FILE5, ToolMapFrame::OnOpenRecentProject)
	EVT_MENU (ID_MENU_ADD_SPATIAL_DATA, ToolMapFrame::OnAddGisData)
	EVT_MENU (ID_MENU_SHORTCUTS, ToolMapFrame::OnShowShortcutWindow)
	EVT_MENU (ID_MENU_ADJUST_SNAPPING, ToolMapFrame::OnShowSnappingWindow)
	
	// TOOL EVENT
	EVT_MENU (ID_MENU_ZOOM_FIT, ToolMapFrame::OnToolChanged)
	EVT_MENU (ID_MENU_ZOOM, ToolMapFrame::OnToolChanged)
	EVT_MENU (ID_MENU_PAN, ToolMapFrame::OnToolChanged)
	EVT_MENU (ID_MENU_SELECT, ToolMapFrame::OnToolChanged)
	EVT_MENU (ID_MENU_DRAW, ToolMapFrame::OnToolChanged)
	EVT_MENU (ID_MENU_MODIFY, ToolMapFrame::OnToolChanged)
	EVT_MENU (ID_MENU_CUT_LINES, ToolMapFrame::OnToolChanged)

	//EDIT MENU
	EVT_MENU (ID_MENU_DELETE_OBJ, ToolMapFrame::OnEditDeleteSelected)
	EVT_MENU (ID_MENU_UNDO, ToolMapFrame::OnEditUndo)
	EVT_MENU (ID_MENU_CREATE_INTERSECTIONS, ToolMapFrame::OnCreateIntersections)
	EVT_MENU (ID_MENU_EDIT_VERTEX_POS, ToolMapFrame::OnEditVertexPosition)
	EVT_MENU (ID_MENU_MERGE_LINES, ToolMapFrame::OnMergeSelectedLines)

	//ATTRIBUTION MENU
	EVT_MENU (ID_MENU_ATTRIB_TYPES,ToolMapFrame::OnShowObjectAttributionWindow)
	EVT_MENU (ID_MENU_ATTRIB_ATTRIBUTES, ToolMapFrame::OnShowAAttributionWindow)

	// EXPORT MENU
	EVT_MENU (ID_MENU_EXPORT_LAYER, ToolMapFrame::OnExportSelected)
	EVT_MENU (ID_MENU_EXPORT_FULL, ToolMapFrame::OnExportAll)


	EVT_MENU (ID_MENU_QUERIES,  ToolMapFrame::OnShowQueriesWindow)

	//TODO: Temp function for testing purpose, remove.
	EVT_MENU (ID_MENU_FLIP_LINE, ToolMapFrame::OnTempBlockRefresh)

	// AUI EVENT WHEN PANE CLOSED
	EVT_AUI_PANE_CLOSE (ToolMapFrame::OnCloseManagedPane)

	EVT_CLOSE(ToolMapFrame::OnQuit)
	EVT_IDLE (ToolMapFrame::OnIdleTimeUpdate)

	// NOTIFICATION EVENT
	EVT_COMMAND (wxID_ANY, tmEVT_SHORTCUT_ATTRIBUTION_DONE, ToolMapFrame::OnShortcutAttributionDone)
	EVT_COMMAND (wxID_ANY, tmEVT_EM_EDIT_START, ToolMapFrame::OnEditSwitch)
	EVT_COMMAND (wxID_ANY, tmEVT_EM_EDIT_STOP, ToolMapFrame::OnEditSwitch)
	EVT_COMMAND (wxID_ANY, tmEVT_SELECTION_DONE, ToolMapFrame::OnUpdateSelection)
	
END_EVENT_TABLE()



ToolMapFrame::ToolMapFrame()
{

}


/* Frame initialisation */
ToolMapFrame::ToolMapFrame(wxFrame *frame, const wxString& title,wxPoint pos, wxSize size)
			: wxFrame(frame, wxID_ANY, title,pos,size)
{
    // Generic list settings for using generic list under Mac
	// otherwise some problem may occur with dnd and inserting
	// item
	wxSystemOptions::SetOption( wxT("mac.listctrl.always_use_generic"), 1 );
	
	// Loading icon
	wxIcon icon;
	icon.CopyFromBitmap(wxGetBitmapFromMemory(toolmap32));
	SetIcon(icon);
	
    // adding status bar
	CreateStatusBar(3,0,wxID_ANY);
	
	// adding menubar
	SetMenuBar(CreateToolMapMenu());
	
	
	// adding toolbar
	SetToolBar(CreateToolMapToolBar(this));
	
	// dessin de l'interface
	PostInit();
	
}

void ToolMapFrame::PostInit()
{
	m_LogWindow = new ImportLogDLG (this);
	wxTextCtrl * myLogTextCtrl = (wxTextCtrl *) m_LogWindow->FindWindow(ID_DLGLOG_TXT);
	wxLog::SetActiveTarget (new wxLogTextCtrl (myLogTextCtrl));
	wxLogMessage(_("Program started"));
	wxLogDebug(_("Debug mode enabled"));
	
	// create the Aui manager
	m_AuiManager = new wxAuiManager(this);
	
	
//#if (__WXMAC__)
//	m_TocWindow = new TocWindowDlgMac (this);
//#else
	m_TocWindow = new TocWindowDlgGen(m_AuiManager, this);
//#endif
	
	// init object attribution panel
	m_MainPanel = new Main_PANEL(this, m_AuiManager);	
	m_AttribObjPanel = new AttribObjType_PANEL(this, m_AuiManager);
	m_QueriesPanel = new Queries_PANEL(this,wxID_ANY, m_AuiManager);
	m_ShortCutPanel = new Shortcuts_PANEL(this, wxID_ANY, m_AuiManager);
	m_SnappingPanel = new Snapping_PANEL(this, wxID_ANY, m_AuiManager);
	
	
	// create layer manager object
	m_LayerManager = new tmLayerManager(this, m_TocWindow->GetTOCCtrl(),
										m_MainPanel->GetGISRenderer(),
										GetStatusBar(),
										m_ScaleCombo);
	
	m_AttribManager = new tmAttributionManager(this,
											   m_TocWindow->GetTOCCtrl(),
											   m_AttribObjPanel,
											   m_LayerManager->GetSelectedDataMemory());
											   
	
	m_EditManager = new tmEditManager (this,
									   m_TocWindow->GetTOCCtrl(),
									   m_LayerManager->GetSelectedDataMemory(),
									   m_MainPanel->GetGISRenderer(),
									   m_LayerManager->GetScale());
	
									   

	m_pConfig = new wxFileConfig();
	// init the menu manager 
	m_MManager = new MenuManager(GetMenuBar(), m_pConfig);
	m_TManager = new ToolbarManager (GetToolBar());
		
	m_PManager = new ProjectManager(this);
	m_PManager->SetMenuManager(m_MManager);
	m_PManager->GetObjectManager()->SetPanel(m_AttribObjPanel);
	m_PManager->SetStatusBar(GetStatusBar());
	m_PManager->SetLayerManager(m_LayerManager);
	m_PManager->SetAttributionManager(m_AttribManager);
	m_PManager->SetQueriesPanel(m_QueriesPanel);
	m_PManager->SetShortcutPanel(m_ShortCutPanel);
	m_PManager->SetSnappingPanel(m_SnappingPanel);
	m_PManager->SetEditManager(m_EditManager);
	
	
		
	
	wxLogMessage(_T("MySQL embedded version is : %s"),DataBase::DatabaseGetVersion().c_str());
	wxLogMessage(_("wxWidgets version is : %s"), wxVERSION_STRING);
	wxLogMessage(_("Running under : %s"), wxGetOsDescription().c_str());
	
	
	// loading GIS drivers
	tmGISData::InitGISDrivers(TRUE, TRUE);

}


/* Frame destruction */
ToolMapFrame::~ToolMapFrame()
{
	// close project
	m_PManager->CloseProject();
	
	
	m_AuiManager->UnInit();
	// don't delete managed windows but check for 
	// memory leak.
	
	delete m_LogWindow;
	delete m_AuiManager;
	
	delete m_EditManager;
	delete m_AttribManager;
	delete m_LayerManager;
	
	
	
	// delete the project Manager
	delete m_PManager;
	
	// delete the menu manager
	delete m_MManager;
	delete m_TManager;
	
	// delete the config file
	
	delete m_pConfig;
}


void ToolMapFrame::OnQuit(wxCloseEvent & event)
{
	
	wxLog::SetActiveTarget (NULL);
	this->Destroy();
}

wxMenuBar* ToolMapFrame::CreateToolMapMenu()
{
 wxMenuBar* menuBar = new wxMenuBar;
    wxMenu* itemMenu2 = new wxMenu;
    wxMenu* itemMenu3 = new wxMenu;
    itemMenu3->Append(ID_MENU_NEW_PRJ_EMPTY, _("&Empty project...\tCtrl+N"), _T(""), wxITEM_NORMAL);
    itemMenu3->Append(ID_MENU_NEW_PRJ_EXISTING, _("From existing..."), _T(""), wxITEM_NORMAL);
    itemMenu2->Append(wxID_ANY, _("New Project"), itemMenu3);
    itemMenu2->Append(ID_MENU_OPEN_PRJ, _("&Open project...\tCtrl+O"), _T(""), wxITEM_NORMAL);
    wxMenu* itemMenu7 = new wxMenu;
    itemMenu2->Append(ID_MENU_RECENT, _("Recent"), itemMenu7);
    itemMenu2->AppendSeparator();
    itemMenu2->Append(ID_MENU_BACKUP_PRJ, _("Bac&kup project"), _T(""), wxITEM_NORMAL);
    itemMenu2->Append(ID_MENU_RESTORE_PRJ, _("Restore project..."), _T(""), wxITEM_NORMAL);
    wxMenu* itemMenu11 = new wxMenu;
    itemMenu11->Append(ID_MENU_EXPORT_LAYER, _("Export layer..."), _T(""), wxITEM_NORMAL);
    itemMenu11->AppendSeparator();
    itemMenu11->Append(ID_MENU_EXPORT_FULL, _("Export full project"), _T(""), wxITEM_NORMAL);
    itemMenu2->Append(ID_MENU_EXPORT, _("Export project"), itemMenu11);
    itemMenu2->AppendSeparator();
    wxMenu* itemMenu16 = new wxMenu;
    itemMenu16->Append(ID_MENU_PRJ_DEF, _("Project definition..."), _T(""), wxITEM_NORMAL);
    itemMenu16->Append(ID_MENU_OBJ_DEF, _("Object definition..."), _T(""), wxITEM_NORMAL);
    itemMenu16->Append(ID_MENU_PRJ_SETTINGS, _("Settings..."), _T(""), wxITEM_NORMAL);
    itemMenu2->Append(ID_MENU_PRJ_EDIT, _("Edit project"), itemMenu16);
    itemMenu2->AppendSeparator();
    itemMenu2->Append(wxID_PRINT, _("Print view...\tCtrl+P"), _T(""), wxITEM_NORMAL);
    itemMenu2->AppendSeparator();
    itemMenu2->Append(wxID_EXIT, _("Quit ToolMap"), _T(""), wxITEM_NORMAL);
    menuBar->Append(itemMenu2, _("&Project"));
    wxMenu* itemMenu24 = new wxMenu;
    itemMenu24->Append(ID_MENU_ADD_SPATIAL_DATA, _("Add spatial data..."), _T(""), wxITEM_NORMAL);
    itemMenu24->AppendSeparator();
    itemMenu24->Append(ID_MENU_IMPORT_GIS_DATA, _("Import GIS data to project..."), _T(""), wxITEM_NORMAL);
    menuBar->Append(itemMenu24, _("Data"));
    wxMenu* itemMenu28 = new wxMenu;
    itemMenu28->Append(ID_MENU_ZOOM_PREVIOUS_EXTEND, _("Zoom to previous extend"), _T(""), wxITEM_NORMAL);
    itemMenu28->AppendSeparator();
    itemMenu28->Append(ID_MENU_ZOOM, _("Zoom by rectangle\tZ"), _T(""), wxITEM_NORMAL);
    itemMenu28->Append(ID_MENU_PAN, _("Pan\tP"), _T(""), wxITEM_NORMAL);
    itemMenu28->Append(ID_MENU_ZOOM_FIT, _("Zoom to full extend"), _T(""), wxITEM_NORMAL);
    itemMenu28->Append(ID_MENU_ZOOM_GIVEN_SCALE, _("Zoom to a given scale..."), _T(""), wxITEM_NORMAL);
    itemMenu28->Append(ID_MENU_ZOOM_COORDINATE, _("Zoom to coordinates..."), _T(""), wxITEM_NORMAL);
    itemMenu28->Append(ID_MENU_ZOOM_NEXT_SCALE, _("Zoom to next defined scale\t+"), _T(""), wxITEM_NORMAL);
    itemMenu28->Append(ID_MENU_PREVIOUS_SCALE, _("Zoom to previous defined scale\t-"), _T(""), wxITEM_NORMAL);
    itemMenu28->AppendSeparator();
    itemMenu28->Append(ID_MENU_SELECTION, _("Zoom to selection"), _T(""), wxITEM_NORMAL);
    itemMenu28->Enable(ID_MENU_SELECTION, false);
    itemMenu28->Append(ID_MENU_SELECTED_LAYER, _("Zoom to selected layer"), _T(""), wxITEM_NORMAL);
    menuBar->Append(itemMenu28, _("View"));
    wxMenu* itemMenu41 = new wxMenu;
    itemMenu41->Append(ID_MENU_UNDO, _("Undo\tCtrl+Z"), _T(""), wxITEM_NORMAL);
    //itemMenu41->Append(ID_MENU_REDO, _("Redo\tCtrl+R"), _T(""), wxITEM_NORMAL);
    itemMenu41->AppendSeparator();
    itemMenu41->Append(ID_MENU_DRAW, _("Draw\tD"), _T(""), wxITEM_NORMAL); 
	itemMenu41->Enable(ID_MENU_DRAW, false);
    itemMenu41->Append(ID_MENU_MODIFY, _("Modify\tM"), _T(""), wxITEM_NORMAL);
	itemMenu41->Enable(ID_MENU_MODIFY, false);
    itemMenu41->Append(ID_MENU_CUT_LINES, _("Cut lines\tCtrl+X"), _T(""), wxITEM_NORMAL);
    itemMenu41->Append(ID_MENU_EDIT_VERTEX_POS, _("Edit vertex position\tCtrl+V"), _T(""), wxITEM_NORMAL);
    itemMenu41->AppendSeparator();
    itemMenu41->Append(ID_MENU_MERGE_LINES, _("Merge selected lines\tCtrl+M"), _T(""), wxITEM_NORMAL);
    itemMenu41->Append(ID_MENU_CREATE_INTERSECTIONS, _("Create intersections\tCtrl+I"), _T(""), wxITEM_NORMAL);
    itemMenu41->Append(ID_MENU_FLIP_LINE, _("Flip line\tCtrl+F"), _T(""), wxITEM_NORMAL);
    itemMenu41->Append(ID_MENU_DELETE_OBJ, _("Delete selected objects"), _T(""), wxITEM_NORMAL);
    itemMenu41->Append(ID_MENU_ADJUST_SNAPPING, _("Adjust snapping...\tCtrl+S"), _T(""), wxITEM_CHECK);
    menuBar->Append(itemMenu41, _("Edition"));
    wxMenu* itemMenu55 = new wxMenu;
    itemMenu55->Append(ID_MENU_ATTRIB_TYPES, _("Object types..."), _T(""), wxITEM_CHECK);
    itemMenu55->Append(ID_MENU_ATTRIB_ATTRIBUTES, _("Object attributes...\tCtrl+A"), _T(""), wxITEM_NORMAL);
    itemMenu55->Append(ID_MENU_ATTRIB_BATCH, _("Object attribute (batch)..."), _T(""), wxITEM_NORMAL);
    itemMenu55->AppendSeparator();
    itemMenu55->Append(ID_MENU_COPY_PASTE_ATTRIB, _("Copy-Paste Attribution\tCtrl+B"), _T(""), wxITEM_NORMAL);
    itemMenu55->AppendSeparator();
    itemMenu55->Append(ID_MENU_SHORTCUTS, _("Shortcuts..."), _T(""), wxITEM_CHECK);
    menuBar->Append(itemMenu55, _("Attribution"));
    wxMenu* itemMenu63 = new wxMenu;
    itemMenu63->Append(ID_MENU_CHECK_GEOM, _("Check Geometry..."), _T(""), wxITEM_CHECK);
    itemMenu63->Check(ID_MENU_CHECK_GEOM, true);
    itemMenu63->Append(ID_MENU_CHECK_DANGLING, _("Check dangling nodes"), _T(""), wxITEM_NORMAL);
    menuBar->Append(itemMenu63, _("Tools"));
    wxMenu* itemMenu66 = new wxMenu;
    itemMenu66->Append(ID_MENU_SELECT, _("Select\tS"), _T(""), wxITEM_NORMAL);
    itemMenu66->Append(ID_MENU_SELECT_ALL, _("Select all"), _T(""), wxITEM_NORMAL);
    itemMenu66->Append(ID_MENU_SELECT_NONE, _("Select none"), _T(""), wxITEM_NORMAL);
    itemMenu66->AppendSeparator();
    itemMenu66->Append(ID_MENU_SELECT_INVERSE, _("Invert selection"), _T(""), wxITEM_NORMAL);
    itemMenu66->Enable(ID_MENU_SELECT_INVERSE, false);
    menuBar->Append(itemMenu66, _("Selection"));
    wxMenu* itemMenu72 = new wxMenu;
    itemMenu72->Append(ID_MENU_QUERIES, _("Queries editor"), _T(""), wxITEM_CHECK);
    itemMenu72->AppendSeparator();
    itemMenu72->Append(ID_MENUITEM11, _("User query 1"), _T(""), wxITEM_NORMAL);
    itemMenu72->Append(ID_MENUITEM12, _("User query 2"), _T(""), wxITEM_NORMAL);
    menuBar->Append(itemMenu72, _("Queries"));
    wxMenu* itemMenu77 = new wxMenu;
    itemMenu77->Append(ID_MENU_TOC_WINDOW, _("Table of content"), _T(""), wxITEM_CHECK);
    itemMenu77->Check(ID_MENU_TOC_WINDOW, true);
    itemMenu77->Append(ID_MENU_INFO_WINDOW, _("Information window"), _T(""), wxITEM_CHECK);
    itemMenu77->Append(ID_MENU_LOG_WINDOW, _("Log window\tCtrl+L"), _T(""), wxITEM_CHECK);
    menuBar->Append(itemMenu77, _("Window"));
    wxMenu* itemMenu81 = new wxMenu;
    itemMenu81->Append(wxID_ABOUT, _("About..."), _T(""), wxITEM_NORMAL);
    itemMenu81->Append(ID_MENU_CHECK_UPDATE, _("Check for updates..."), _T(""), wxITEM_NORMAL);
    menuBar->Append(itemMenu81, _("&Help"));
    return menuBar;
}


wxToolBar * ToolMapFrame::CreateToolMapToolBar(wxWindow * parent)
{
	long style = wxTB_FLAT | wxTB_HORIZONTAL;

	// conditionnal compilation for better look under win32
#ifndef __WXMSW__
	style += wxTB_TEXT;
#endif

	wxToolBar* itemToolBar3 = new wxToolBar( parent, ID_TOOLBAR1, wxDefaultPosition, wxDefaultSize, style );
    itemToolBar3->SetToolBitmapSize(wxSize(32, 32));
    wxBitmap itemtool4Bitmap (wxGetBitmapFromMemory(tool1));
    wxBitmap itemtool4BitmapDisabled;
    itemToolBar3->AddTool(ID_MENU_SELECT, _("Select"), itemtool4Bitmap, wxNullBitmap, wxITEM_NORMAL, _T(""), wxEmptyString);
	wxBitmap itemtool5Bitmap(wxGetBitmapFromMemory(tool2));
    wxBitmap itemtool5BitmapDisabled;
    itemToolBar3->AddTool(ID_MENU_ZOOM_FIT, _T("Fit"), itemtool5Bitmap, itemtool5BitmapDisabled, wxITEM_NORMAL, _T(""), wxEmptyString);
    wxBitmap itemtool6Bitmap(wxGetBitmapFromMemory(tool3));
    wxBitmap itemtool6BitmapDisabled;
    itemToolBar3->AddTool(ID_MENU_ZOOM, _T("Zoom"), itemtool6Bitmap, itemtool6BitmapDisabled, wxITEM_NORMAL, _T(""), wxEmptyString);
    wxBitmap itemtool7Bitmap(wxGetBitmapFromMemory(tool4));
    wxBitmap itemtool7BitmapDisabled;
    itemToolBar3->AddTool(ID_MENU_PAN, _T("Pan"), itemtool7Bitmap, itemtool7BitmapDisabled, wxITEM_NORMAL, _T(""), wxEmptyString);
    wxArrayString itemComboBox8Strings;
    m_ScaleCombo = new tmScaleCtrlCombo ( itemToolBar3, ID_COMBOBOX2, wxDefaultPosition, wxDefaultSize );
	itemToolBar3->AddControl(m_ScaleCombo);
    wxBitmap itemtool9Bitmap(wxGetBitmapFromMemory(tool5));
    wxBitmap itemtool9BitmapDisabled;
    itemToolBar3->AddTool(ID_MENU_DRAW, _T("Draw"), itemtool9Bitmap, itemtool9BitmapDisabled, wxITEM_NORMAL, _T(""), wxEmptyString);
    wxBitmap itemtool10Bitmap(wxGetBitmapFromMemory(tool6));
    wxBitmap itemtool10BitmapDisabled;
	itemToolBar3->EnableTool (ID_MENU_DRAW, false);
    itemToolBar3->AddTool(ID_MENU_MODIFY, _T("Modify"), itemtool10Bitmap, itemtool10BitmapDisabled, wxITEM_NORMAL, _T(""), wxEmptyString);
    wxBitmap itemtool11Bitmap(wxGetBitmapFromMemory(tool7));
    wxBitmap itemtool11BitmapDisabled;
	itemToolBar3->EnableTool(ID_MENU_MODIFY, false);
    itemToolBar3->AddTool(ID_TOOL7, _T("Copy-paste attribution"), itemtool11Bitmap, itemtool11BitmapDisabled, wxITEM_NORMAL, _T(""), wxEmptyString);
	itemToolBar3->AddSeparator();
    wxBitmap itemtool13Bitmap(wxGetBitmapFromMemory(tool8));
    wxBitmap itemtool13BitmapDisabled;
    itemToolBar3->AddTool(ID_MENU_ATTRIB_TYPES, _T("Object type"), itemtool13Bitmap, itemtool13BitmapDisabled, wxITEM_NORMAL, _T(""), wxEmptyString);
    wxBitmap itemtool14Bitmap(wxGetBitmapFromMemory(tool9));
    wxBitmap itemtool14BitmapDisabled;
    itemToolBar3->AddTool(ID_MENU_ATTRIB_ATTRIBUTES, _T("Object attributes"), itemtool14Bitmap, itemtool14BitmapDisabled, wxITEM_NORMAL, _T(""), wxEmptyString);
    itemToolBar3->AddSeparator();
    wxBitmap itemtool16Bitmap(wxGetBitmapFromMemory(tool10));
    wxBitmap itemtool16BitmapDisabled;
    itemToolBar3->AddTool(ID_TOOL10, _T("Informations"), itemtool16Bitmap, itemtool16BitmapDisabled, wxITEM_NORMAL, _T(""), wxEmptyString);
    itemToolBar3->Realize();
	
	return itemToolBar3;
}


void ToolMapFrame::OnNewProject(wxCommandEvent & event)
{
	// call the project manager and ask for
	// creating a new project.
	if (m_PManager->CreateNewProject())
	{
	// add name to the program bar
	wxString myProgName = g_ProgName + SVN_VERSION + _T(" - ") + m_PManager->GetProjectName();
	SetTitle(myProgName);
	

	}
	
}


void ToolMapFrame::OnOpenProject (wxCommandEvent & event)
{
	
	
	
	tmProgressIndicator * testtm = new tmProgressIndicator(this, GetStatusBar());
	testtm->DisplayProgress();
		
	
	
	// display a dir dialog for selecting the project to open
	wxDirDialog * myDirDLG = new wxDirDialog(this, _("Choose a ToolMap project"),
											 _T(""), wxRESIZE_BORDER | wxDD_DIR_MUST_EXIST);
	if (myDirDLG->ShowModal()==wxID_OK)
	{
		// call the project manager and ask to open an
		// existing project. 
		int iActError = m_PManager->OpenProject(myDirDLG->GetPath());
		if (iActError == OPEN_OK)
		{
			// If we can open the project,set the name in the program bar.
			wxString myProgName = g_ProgName + SVN_VERSION + _T(" - ") + m_PManager->GetProjectName();
			SetTitle(myProgName);
			
		}
		else
		{
			OpenErrorDlg dlg (this, iActError, TM_DATABASE_VERSION, myDirDLG->GetPath());
			dlg.ShowModal();
			
			/*wxMessageBox(_("The selected folder is not a ToolMap project,\nplease select a ToolMap project."),
						 _("Opening project error"), wxICON_ERROR | wxOK,
						 this);*/
		
			// If we can open the project,set the name in the program bar.
			wxString myProgName = g_ProgName + SVN_VERSION;
			SetTitle(myProgName);
			
		}
	}
	delete myDirDLG;
	
	delete testtm;

}


/***************************************************************************//**
 @brief Open file based on recent
 @details Event function called when user try to open a file from the recent
 menu.
 @author Lucien Schreiber (c) CREALP 2007
 @date 07 April 2008
 *******************************************************************************/
void ToolMapFrame::OnOpenRecentProject(wxCommandEvent & event)
{
	wxString myPath = _T("");
	
	// get the file to open (the one clicked...)
	if (m_MManager->GetRecentFile(myPath, event.GetId() - wxID_FILE1))
	{
		int iActError = m_PManager->OpenProject(myPath);
		if (iActError == OPEN_OK)
		{
			// If we can open the project,set the name in the program bar.
			wxString myProgName = g_ProgName + SVN_VERSION + _T(" - ") + m_PManager->GetProjectName();
			SetTitle(myProgName);
			
			// updates the menu using the menu manager
			m_MManager->SetStatus(MENU_DB_OPENED);
		}
		else
		{
			OpenErrorDlg dlg (this, iActError, TM_DATABASE_VERSION, myPath);
			dlg.ShowModal();
			
			// If we can't open the project,set the name in the program bar.
			wxString myProgName = g_ProgName + SVN_VERSION;
			SetTitle(myProgName);
			
			// remove the non valid history from the recent
			m_MManager->RemoveFileFromRecent(event.GetId() - wxID_FILE1);
			
			// updates the menu using the menu manager
			m_MManager->SetStatus(MENU_DB_CLOSED);
		}
	}
	
}



void ToolMapFrame::OnEditProjectObjects (wxCommandEvent & event)
{
	// call the project manager
	m_PManager->EditProjectObjectDefinition();
}



void ToolMapFrame::OnEditProjectSettings (wxCommandEvent & event)
{
	// call the project manager.
	m_PManager->EditProjectSettings();
}



void ToolMapFrame::OnEditProject (wxCommandEvent & event)
{
	// call the project manager
	m_PManager->EditProject();
	
}



void ToolMapFrame::OnNewProjectExisting (wxCommandEvent & event)
{
	ProjectNewExistDLG * myNewPrjExistDLG = new ProjectNewExistDLG(this);
	if (myNewPrjExistDLG->ShowModal() == wxID_OK)
	{
		wxLogDebug(myNewPrjExistDLG->m_New_Prj_Name);
		
		//// temp for testing 
		
		
		 DirOperation mydirOp (myNewPrjExistDLG->m_Old_Prj_Name, myNewPrjExistDLG->m_New_Prj_Name);
		double myPrjSize = mydirOp.GetDirectorySize();
		wxLogDebug(_T("%.*f [MB]"),3,myPrjSize);
		
		wxArrayString myFiles;
		long NumOfFileToCopy = mydirOp.GetAllDirectoryFiles(myFiles);
		
		wxLogDebug(_T("Number of files to copy : %d"),NumOfFileToCopy);
		
		// create the destination path
		if (mydirOp.IsPathWritable(DIROP_PATH_DESTINATION))
		{
			mydirOp.HasEnoughFreeSpace(myPrjSize, DIROP_PATH_DESTINATION);
		}
		
		// copy the files 
		if (mydirOp.CopyDirectory(myFiles,TRUE))
			wxLogDebug(_T("Directory copy finished..."));
		
		
		//// end of testing
		
		
		
	}
	delete myNewPrjExistDLG;
}


void ToolMapFrame::OnLogWindow(wxCommandEvent & event)
{
	///@bug the Check methode won't work under mac os X in the window menu.
	if (m_LogWindow->IsShown())
	{
		
		GetMenuBar()->Check(ID_MENU_LOG_WINDOW, FALSE);
		m_LogWindow->Hide();
	}
	else
	{
		m_LogWindow->Show();
		GetMenuBar()->Check(ID_MENU_LOG_WINDOW, TRUE);
	}
}

void ToolMapFrame::OnTocWindow (wxCommandEvent & event)
{
	if (m_TocWindow->IsShown())
	{
		m_TocWindow->Hide();
		GetMenuBar()->Check(ID_MENU_TOC_WINDOW, FALSE);
	}
	else 
	{
		m_TocWindow->Show();
		GetMenuBar()->Check(ID_MENU_TOC_WINDOW, TRUE);

	}

}


void ToolMapFrame::OnIdleTimeUpdate(wxIdleEvent & event)
{
	m_MManager->UpdateMenusStatus();
}


void ToolMapFrame::OnShowObjectAttributionWindow (wxCommandEvent & event)
{
	if(m_AttribObjPanel->IsPanelShown())
	{
		m_AttribObjPanel->HidePanel();
		GetMenuBar()->Check(ID_MENU_ATTRIB_TYPES, FALSE);
	}
	else
	{
		m_AttribObjPanel->ShowPanel();
		GetMenuBar()->Check(ID_MENU_ATTRIB_TYPES, TRUE);
	}
	
}


/***************************************************************************//**
 @brief Display or hide the #Queries_PANEL
 @author Lucien Schreiber (c) CREALP 2008
 @date 08 November 2008
 *******************************************************************************/
void ToolMapFrame::OnShowQueriesWindow (wxCommandEvent & event)
{
	if (m_QueriesPanel->IsPanelShown())
	{
		m_QueriesPanel->HidePanel();
		GetMenuBar()->Check(ID_MENU_QUERIES, false);
	}
	else
	{
		m_QueriesPanel->ShowPanel();
		GetMenuBar()->Check(ID_MENU_QUERIES, true);
	}
	
}



/***************************************************************************//**
 @brief Display or hide the #Shortcuts_PANEL
 @author Lucien Schreiber (c) CREALP 2008
 @date 04 December 2008
 *******************************************************************************/
void ToolMapFrame::OnShowShortcutWindow (wxCommandEvent & event)
{
	if (m_ShortCutPanel->IsPanelShown())
	{
		m_ShortCutPanel->HidePanel();
		GetMenuBar()->Check(ID_MENU_SHORTCUTS, false);
	}
	else
	{
		m_ShortCutPanel->ShowPanel();
		GetMenuBar()->Check(ID_MENU_SHORTCUTS, true);
	}
	
}



/***************************************************************************//**
 @brief Display or hide the #Snapping_PANEL
 @author Lucien Schreiber (c) CREALP 2009
 @date 19 January 2009
 *******************************************************************************/
void ToolMapFrame::OnShowSnappingWindow (wxCommandEvent & event)
{
	if (m_SnappingPanel->IsPanelShown())
	{
		m_SnappingPanel->HidePanel();
		GetMenuBar()->Check(ID_MENU_ADJUST_SNAPPING, false);
	}
	else
	{
		m_SnappingPanel->ShowPanel();
		GetMenuBar()->Check(ID_MENU_ADJUST_SNAPPING, true);
	}
	
}




void ToolMapFrame::OnEditSwitch (wxCommandEvent & event)
{
	bool bEditStart = false;
	if (event.GetEventType() == tmEVT_EM_EDIT_START)
		bEditStart = true;
	else
		m_LayerManager->OnSelect(); // set the select cursor
	
	m_MManager->EditingStatus(bEditStart);
	m_TManager->EditingStatus(bEditStart);

	
}


/***************************************************************************//**
 @brief User press the "Delete selected object" menu
 @image html editdeletemenu.png
 @author Lucien Schreiber (c) CREALP 2009
 @date 05 February 2009
 *******************************************************************************/
void ToolMapFrame::OnEditDeleteSelected (wxCommandEvent & event)
{
	m_EditManager->DeleteSelected(true);
}



/***************************************************************************//**
 @brief Called when user press "Undo"
 @author Lucien Schreiber (c) CREALP 2009
 @date 05 February 2009
 *******************************************************************************/
void ToolMapFrame::OnEditUndo (wxCommandEvent & event)
{
	m_EditManager->UndoLastVertex();
}



/***************************************************************************//**
 @brief Called when user try to create intersections
 @author Lucien Schreiber (c) CREALP 2009
 @date 09 February 2009
 *******************************************************************************/
void ToolMapFrame::OnCreateIntersections (wxCommandEvent & event)
{
	m_EditManager->CreateIntersections();
}



/***************************************************************************//**
 @brief Display the edit vertex position dialog
 @author Lucien Schreiber (c) CREALP 2009
 @date 23 February 2009
 *******************************************************************************/
void ToolMapFrame::OnEditVertexPosition (wxCommandEvent & event)
{
	m_EditManager->EditVertexPosition();
}



/***************************************************************************//**
 @brief Called when line merge menu is pressed
 @author Lucien Schreiber (c) CREALP 2009
 @date 27 February 2009
 *******************************************************************************/
void ToolMapFrame::OnMergeSelectedLines (wxCommandEvent & event)
{
	m_EditManager->MergeSelectedLines();
}


/***************************************************************************//**
 @details Called when Display Advanced attribution is pressed
 @author Lucien Schreiber (c) CREALP 2009
 @date 06 March 2009
 *******************************************************************************/
void ToolMapFrame::OnShowAAttributionWindow (wxCommandEvent & event)
{
	m_AttribManager->AAttributionButtonShow();
}

/***************************************************************************//**
 @brief Called earch time the selection is updated
 @details Modifies menu according to number of selected item
 @author Lucien Schreiber (c) CREALP 2009
 @date 18 February 2009
 *******************************************************************************/
void ToolMapFrame::OnUpdateSelection (wxCommandEvent & event)
{
	m_MManager->UpdateSelection(m_AttribManager->GetSelectionCount());
	event.Skip();
}


void ToolMapFrame::OnBackupProject (wxCommandEvent & event)
{
	m_PManager->BackupProject();
}


void ToolMapFrame::OnUpdateAttributionObjects(wxCommandEvent & event)
{
	//m_AttribObjPanel->UpdateObjectPointList(m_Database);
	//m_AttribObjPanel->m_pObjList_PT->AddItem(-1, -1, _T("coucou"));
}


void ToolMapFrame::OnAddGisData (wxCommandEvent & event)
{
	m_LayerManager->AddLayer(event);
}


void ToolMapFrame::OnShortcutAttributionDone (wxCommandEvent & event)
{
	wxString myStatusInfo =  event.GetString();
	myStatusInfo.Append(_(" : Attribution DONE"));
	
	GetStatusBar()->SetStatusText(myStatusInfo, 0);
}



/***************************************************************************//**
 @brief This is called when a pane was closed using the close button
 @details In this case, we should remove the tick near the window name in the
 menu
 @author Lucien Schreiber (c) CREALP 2009
 @date 23 January 2009
 *******************************************************************************/
void ToolMapFrame::OnCloseManagedPane(wxAuiManagerEvent & event)
{
	int iId = wxNOT_FOUND;
	wxAuiPaneInfo * myPaneInfo = event.GetPane();
	
	if (myPaneInfo->name == SHORTCUT_PANEL_TITLE)
		iId = ID_MENU_SHORTCUTS;
	
	if (myPaneInfo->name == SNAPPING_PANEL_TITLE)
		iId = ID_MENU_ADJUST_SNAPPING;
	
	if (iId != wxNOT_FOUND)
		GetMenuBar()->Check(iId, false);
}




void ToolMapFrame::OnToolChanged (wxCommandEvent & event)
{
	// check that a project is open
	if (!m_PManager->IsProjectOpen())
		return;
	
	switch (event.GetId())
	{
		case ID_MENU_SELECT:
			m_LayerManager->OnSelect();
			break;
			
		case ID_MENU_ZOOM_FIT:
			// execute command imediatly
			m_LayerManager->OnZoomToFit();
			break;
			
		case ID_MENU_ZOOM:
			m_LayerManager->OnZoomRectangle();
			break;
			
		case ID_MENU_PAN:
			m_LayerManager->OnPan();
			break;
			
		case ID_MENU_DRAW:
			m_EditManager->OnToolEdit();
			break;
			
		case ID_MENU_MODIFY:
			m_EditManager->OnToolModify();
			break;
			
		case ID_MENU_CUT_LINES:
			m_EditManager->OnToolCutLines();
			break;
			
		default:
			m_LayerManager->OnSelect();
			wxLogDebug(_T("Tool Not supported now : %d"), event.GetId());
			break;
	}
	
	
}


/***************************************************************************//**
 @brief Called when export selected is pressed
 @details This function create an object of type #tmExportManager
 @author Lucien Schreiber (c) CREALP 2008
 @date 13 November 2008
 *******************************************************************************/
void ToolMapFrame::OnExportSelected (wxCommandEvent & event)
{
	if (m_PManager->IsProjectOpen())
	{
		// get project def from memory
		PrjDefMemManage * memProj = m_PManager->GetMemoryProjectDefinition();
		PrjDefMemManage myCopyProj;
		myCopyProj = *memProj;
		
		
		tmExportManager myExport(this, m_PManager->GetDatabase());
		if (myExport.ExportSelected(&myCopyProj))
			wxLogDebug(_T("Exporting layer(s) success"));
		else
			wxLogDebug(_T("Exporting layer(s) failed"));
	}
		
}



/***************************************************************************//**
 @brief Called when export all is pressed
 @details Export all layers without any further user intervention
 @author Lucien Schreiber (c) CREALP 2008
 @date 13 November 2008
 *******************************************************************************/
void ToolMapFrame::OnExportAll (wxCommandEvent & event)
{
	if (m_PManager->IsProjectOpen())
	{
		// get project def from memory
		PrjDefMemManage * memProj = m_PManager->GetMemoryProjectDefinition();
		PrjDefMemManage myCopyProj;
		myCopyProj = *memProj;
		
		tmExportManager myExport(this, m_PManager->GetDatabase());
		if (myExport.ExportAll(&myCopyProj))
			wxLogDebug(_T("Exporting all project success"));
		else
			wxLogDebug(_T("Exporting all project failed"));
	}
	
}


