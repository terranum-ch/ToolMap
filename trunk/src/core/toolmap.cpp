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



 inline wxBitmap _wxGetBitmapFromMemory(const unsigned char *data, int length) {
   wxMemoryInputStream is(data, length);
   return wxBitmap(wxImage(is, wxBITMAP_TYPE_ANY, -1), -1);
 }

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
	EVT_MENU (ID_MENU_ATTRIB_TYPES,ToolMapFrame::OnShowObjectAttributionWindow)
	EVT_MENU (ID_MENU_OPEN_PRJ, ToolMapFrame::OnOpenProject)
	EVT_MENU (ID_MENU_OBJ_DEF, ToolMapFrame::OnEditProjectObjects)
	EVT_MENU (ID_MENU_PRJ_SETTINGS, ToolMapFrame::OnEditProjectSettings )
	EVT_MENU (ID_MENU_PRJ_DEF, ToolMapFrame::OnEditProject)
	EVT_MENU (ID_MENU_BACKUP_PRJ, ToolMapFrame::OnBackupProject)
	EVT_MENU (ID_MENU_COPY_PASTE_ATTRIB, ToolMapFrame::OnUpdateAttributionObjects)
	EVT_MENU_RANGE (wxID_FILE1, wxID_FILE5, ToolMapFrame::OnOpenRecentProject)
	EVT_CLOSE(ToolMapFrame::OnQuit)
	EVT_IDLE (ToolMapFrame::OnIdleTimeUpdate)
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
	CreateStatusBar(2,0,wxID_ANY);
	
	// adding menubar
	SetMenuBar(CreateToolMapMenu());
	
	
	// adding toolbar
	SetToolBar(CreateToolMapToolBar(this));
	
	// dessin de l'interface
	//wxPanel * top_panel = new wxPanel (this, -1,wxDefaultPosition,wxDefaultSize );
	//INTERFACE(top_panel, FALSE);
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
	
	
	// create the database object 
	//m_Database = new DataBaseTM();
	
	m_pConfig = new wxFileConfig();
	// init the menu manager 
	m_MManager = new MenuManager(GetMenuBar(), m_pConfig);
		
	m_PManager = new ProjectManager(this);
	m_PManager->SetMenuManager(m_MManager);
	

	
	wxLogMessage(_T("MySQL embedded version is : %s"),DataBase::DatabaseGetVersion().c_str());

	
}


/* Frame destruction */
ToolMapFrame::~ToolMapFrame()
{
	m_AuiManager->UnInit();
	// don't delete managed windows but check for 
	// memory leak.
	
	delete m_LogWindow;
	delete m_AuiManager;
	
	// delete the project Manager
	delete m_PManager;
	
	// delete the menu manager
	delete m_MManager;
	
	// delete the config file
	
	delete m_pConfig;
}


void ToolMapFrame::OnQuit(wxCloseEvent & event)
{

//		if (m_Database->DataBaseIsOpen())
//		{
//			m_Database->DataBaseClose();
//		}
//		delete m_Database;
	
	wxLog::SetActiveTarget (NULL);
	this->Destroy();
}

wxMenuBar* ToolMapFrame::CreateToolMapMenu()
{
 wxMenuBar* menuBar = new wxMenuBar;
    wxMenu* itemMenu2 = new wxMenu;
    wxMenu* itemMenu3 = new wxMenu;
    itemMenu3->Append(ID_MENU_NEW_PRJ_EMPTY, _("&Empty project...\tCtrl-N"), _T(""), wxITEM_NORMAL);
    itemMenu3->Append(ID_MENU_NEW_PRJ_EXISTING, _("From existing..."), _T(""), wxITEM_NORMAL);
    itemMenu2->Append(wxID_ANY, _("New Project"), itemMenu3);
    itemMenu2->Append(ID_MENU_OPEN_PRJ, _("&Open project...\tCtrl-O"), _T(""), wxITEM_NORMAL);
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
    itemMenu41->Append(ID_MENU_REDO, _("Redo\tCtrl+R"), _T(""), wxITEM_NORMAL);
    itemMenu41->AppendSeparator();
    itemMenu41->Append(ID_MENU_DRAW, _("Draw\tD"), _T(""), wxITEM_NORMAL);
    itemMenu41->Append(ID_MENU_MODIFY, _("Modify\tM"), _T(""), wxITEM_NORMAL);
    itemMenu41->Append(ID_MENU_CUT_LINES, _("Cut lines\tCtrl+X"), _T(""), wxITEM_NORMAL);
    itemMenu41->Append(ID_MENU_EDIT_VERTEX_POS, _("Edit vertex position\tCtrl+V"), _T(""), wxITEM_NORMAL);
    itemMenu41->AppendSeparator();
    itemMenu41->Append(ID_MENU_MERGE_LINES, _("Merge selected lines\tCtrl+M"), _T(""), wxITEM_NORMAL);
    itemMenu41->Append(ID_MENU_CREATE_INTERSECTIONS, _("Create intersections\tCtrl+I"), _T(""), wxITEM_NORMAL);
    itemMenu41->Append(ID_MENU_FLIP_LINE, _("Flip line\tCtrl+F"), _T(""), wxITEM_NORMAL);
    itemMenu41->Append(ID_MENU_DELETE_OBJ, _("Delete selected objects"), _T(""), wxITEM_NORMAL);
    itemMenu41->Append(ID_MENU_ADJUST_SNAPPING, _("Adjust snapping...\tCtrl+S"), _T(""), wxITEM_NORMAL);
    menuBar->Append(itemMenu41, _("Edition"));
    wxMenu* itemMenu55 = new wxMenu;
    itemMenu55->Append(ID_MENU_ATTRIB_TYPES, _("Object types..."), _T(""), wxITEM_CHECK);
    itemMenu55->Append(ID_MENU_ATTRIB_ATTRIBUTES, _("Object attributes...\tCtrl+A"), _T(""), wxITEM_NORMAL);
    itemMenu55->Append(ID_MENU_ATTRIB_BATCH, _("Object attribute (batch)..."), _T(""), wxITEM_NORMAL);
    itemMenu55->AppendSeparator();
    itemMenu55->Append(ID_MENU_COPY_PASTE_ATTRIB, _("Copy-Paste Attribution\tCtrl+B"), _T(""), wxITEM_NORMAL);
    itemMenu55->AppendSeparator();
    itemMenu55->Append(ID_MENU_SHORTCUTS, _("Shortcuts..."), _T(""), wxITEM_NORMAL);
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
    itemMenu72->Append(ID_MENU_QUERIES, _("Queries editor..."), _T(""), wxITEM_NORMAL);
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
    itemToolBar3->AddTool(ID_TOOL1, _("Select"), itemtool4Bitmap, wxNullBitmap, wxITEM_NORMAL, _T(""), wxEmptyString);
	wxBitmap itemtool5Bitmap(wxGetBitmapFromMemory(tool2));
    wxBitmap itemtool5BitmapDisabled;
    itemToolBar3->AddTool(ID_TOOL2, _T("Fit"), itemtool5Bitmap, itemtool5BitmapDisabled, wxITEM_NORMAL, _T(""), wxEmptyString);
    wxBitmap itemtool6Bitmap(wxGetBitmapFromMemory(tool3));
    wxBitmap itemtool6BitmapDisabled;
    itemToolBar3->AddTool(ID_TOOL3, _T("Zoom"), itemtool6Bitmap, itemtool6BitmapDisabled, wxITEM_NORMAL, _T(""), wxEmptyString);
    wxBitmap itemtool7Bitmap(wxGetBitmapFromMemory(tool4));
    wxBitmap itemtool7BitmapDisabled;
    itemToolBar3->AddTool(ID_TOOL4, _T("Pan"), itemtool7Bitmap, itemtool7BitmapDisabled, wxITEM_NORMAL, _T(""), wxEmptyString);
    wxArrayString itemComboBox8Strings;
    wxComboBox* itemComboBox8 = new wxComboBox( itemToolBar3, ID_COMBOBOX2, _T(""), wxDefaultPosition, wxDefaultSize, itemComboBox8Strings, wxCB_DROPDOWN );
    itemToolBar3->AddControl(itemComboBox8);
    wxBitmap itemtool9Bitmap(wxGetBitmapFromMemory(tool5));
    wxBitmap itemtool9BitmapDisabled;
    itemToolBar3->AddTool(ID_TOOL5, _T("Draw"), itemtool9Bitmap, itemtool9BitmapDisabled, wxITEM_NORMAL, _T(""), wxEmptyString);
    wxBitmap itemtool10Bitmap(wxGetBitmapFromMemory(tool6));
    wxBitmap itemtool10BitmapDisabled;
    itemToolBar3->AddTool(ID_TOOL6, _T("Modify"), itemtool10Bitmap, itemtool10BitmapDisabled, wxITEM_NORMAL, _T(""), wxEmptyString);
    wxBitmap itemtool11Bitmap(wxGetBitmapFromMemory(tool7));
    wxBitmap itemtool11BitmapDisabled;
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
	// display a dir dialog for selecting the project to open
	wxDirDialog * myDirDLG = new wxDirDialog(this, _("Choose a ToolMap project"),
											 _T(""), wxRESIZE_BORDER | wxDD_DIR_MUST_EXIST);
	if (myDirDLG->ShowModal()==wxID_OK)
	{
		// call the project manager and ask to open an
		// existing project. 
		if (m_PManager->OpenProject(myDirDLG->GetPath()))
		{
			// If we can open the project,set the name in the program bar.
			wxString myProgName = g_ProgName + SVN_VERSION + _T(" - ") + m_PManager->GetProjectName();
			SetTitle(myProgName);
			
		}
		else
		{
			wxMessageBox(_("The selected folder is not a ToolMap project,\nplease select a ToolMap project."),
						 _("Opening project error"), wxICON_ERROR | wxOK,
						 this);
		
			// If we can open the project,set the name in the program bar.
			wxString myProgName = g_ProgName + SVN_VERSION;
			SetTitle(myProgName);
			
		}
	}
	delete myDirDLG;
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
		if (m_PManager->OpenProject(myPath))
		{
			// If we can open the project,set the name in the program bar.
			wxString myProgName = g_ProgName + SVN_VERSION + _T(" - ") + m_PManager->GetProjectName();
			SetTitle(myProgName);
			
			// updates the menu using the menu manager
			m_MManager->SetStatus(MENU_DB_OPENED);
		}
		else
		{
			wxString sMessage = _("Unable to open the specified project.\n");
			sMessage.Append(_("The specified path doesen't exist or isn't\na ToolMap project.\n\n"));
			sMessage.Append( _( "This path is now removed from the file history."));
			
					
			wxMessageBox(sMessage,_("Opening project error"), wxICON_ERROR | wxOK,this);
			
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


void ToolMapFrame::OnBackupProject (wxCommandEvent & event)
{
	m_PManager->BackupProject();
}


void ToolMapFrame::OnUpdateAttributionObjects(wxCommandEvent & event)
{
	m_AttribObjPanel->m_pObjList_PT->AddItem(-1, -1, _T("coucou"));
}




