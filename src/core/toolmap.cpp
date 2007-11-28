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


/* APPLICATION INITIALISATION */
bool ToolMapApp::OnInit()
{
	
	
	
	ToolMapFrame* frame = new ToolMapFrame(0L, _("ToolMap 2"),wxDefaultPosition, wxSize(620,480));
	
	
	
	frame->Show();
	
	return true;
}



//BEGIN_EVENT_TABLE (ToolMapFrame, wxFrame)
//EVT_MENU (ID_RUN, ToolMapFrame::OnAbout)
//END_EVENT_TABLE()


/* Frame initialisation */
ToolMapFrame::ToolMapFrame(wxFrame *frame, const wxString& title,wxPoint pos, wxSize size)
			: wxFrame(frame, -1, title,pos,size)
{
    // Loading icon
	// wxIcon icon(sirs_xpm);
    //SetIcon(icon);
	
    // adding status bar
	CreateStatusBar(2,0,wxID_ANY);
	
	// adding menubar
	SetMenuBar(CreateToolMapMenu());
	
	// dessin de l'interface
	wxPanel * top_panel = new wxPanel (this, -1,wxDefaultPosition,wxDefaultSize );
	//INTERFACE(top_panel, FALSE);

	
}


/* Frame destruction */
ToolMapFrame::~ToolMapFrame()
{
}

wxMenuBar* ToolMapFrame::CreateToolMapMenu()
{
    wxMenuBar* menuBar = new wxMenuBar;
    wxMenu* itemMenu2 = new wxMenu;
    wxMenu* itemMenu3 = new wxMenu;
    itemMenu3->Append(ID_MENUITEM, _("&Empty project...\tCtrl-N"), _T(""), wxITEM_NORMAL);
    itemMenu3->Append(ID_MENUITEM30, _("From existing..."), _T(""), wxITEM_NORMAL);
    itemMenu2->Append(ID_MENU4, _("New Project"), itemMenu3);
    itemMenu2->Append(ID_MENUITEM1, _("&Open project...\tCtrl-O"), _T(""), wxITEM_NORMAL);
    wxMenu* itemMenu7 = new wxMenu;
    itemMenu2->Append(ID_MENU3, _("Recent"), itemMenu7);
    itemMenu2->AppendSeparator();
    itemMenu2->Append(ID_MENUITEM2, _("Bac&kup project"), _T(""), wxITEM_NORMAL);
    itemMenu2->Append(ID_MENUITEM19, _("Restore project..."), _T(""), wxITEM_NORMAL);
    wxMenu* itemMenu11 = new wxMenu;
    itemMenu11->Append(ID_MENUITEM6, _("Export layer..."), _T(""), wxITEM_NORMAL);
    itemMenu11->AppendSeparator();
    itemMenu11->Append(ID_MENUITEM7, _("Export full project"), _T(""), wxITEM_NORMAL);
    itemMenu2->Append(ID_MENU1, _("Export project"), itemMenu11);
    itemMenu2->AppendSeparator();
    wxMenu* itemMenu16 = new wxMenu;
    itemMenu16->Append(ID_MENUITEM55, _("Project definition..."), _T(""), wxITEM_NORMAL);
    itemMenu16->Append(ID_MENUITEM56, _("Object definition..."), _T(""), wxITEM_NORMAL);
    itemMenu16->Append(ID_MENUITEM39, _("Settings..."), _T(""), wxITEM_NORMAL);
    itemMenu2->Append(ID_MENU8, _("Edit project"), itemMenu16);
    itemMenu2->AppendSeparator();
    itemMenu2->Append(ID_MENUITEM28, _("Print view...\tCtrl+P"), _T(""), wxITEM_NORMAL);
    itemMenu2->AppendSeparator();
    itemMenu2->Append(wxID_EXIT, _("Quit ToolMap"), _T(""), wxITEM_NORMAL);
    menuBar->Append(itemMenu2, _("&Project"));
    wxMenu* itemMenu24 = new wxMenu;
    itemMenu24->Append(ID_MENUITEM3, _("Add spatial data..."), _T(""), wxITEM_NORMAL);
    itemMenu24->AppendSeparator();
    itemMenu24->Append(ID_MENUITEM21, _("Import GIS data to project..."), _T(""), wxITEM_NORMAL);
    menuBar->Append(itemMenu24, _("Data"));
    wxMenu* itemMenu28 = new wxMenu;
    itemMenu28->Append(ID_MENUITEM4, _("Zoom to previous extend"), _T(""), wxITEM_NORMAL);
    itemMenu28->AppendSeparator();
    itemMenu28->Append(ID_MENUITEM35, _("Zoom by rectangle\tZ"), _T(""), wxITEM_NORMAL);
    itemMenu28->Append(ID_MENUITEM36, _("Pan\tP"), _T(""), wxITEM_NORMAL);
    itemMenu28->Append(ID_MENUITEM22, _("Zoom to full extend"), _T(""), wxITEM_NORMAL);
    itemMenu28->Append(ID_MENUITEM23, _("Zoom to a given scale..."), _T(""), wxITEM_NORMAL);
    itemMenu28->Append(ID_MENUITEM52, _("Zoom to coordinates..."), _T(""), wxITEM_NORMAL);
    itemMenu28->Append(ID_MENUITEM48, _("Zoom to next defined scale\t+"), _T(""), wxITEM_NORMAL);
    itemMenu28->Append(ID_MENUITEM49, _("Zoom to previous defined scale\t-"), _T(""), wxITEM_NORMAL);
    itemMenu28->AppendSeparator();
    itemMenu28->Append(ID_MENUITEM8, _("Zoom to selection"), _T(""), wxITEM_NORMAL);
    itemMenu28->Enable(ID_MENUITEM8, false);
    itemMenu28->Append(ID_MENUITEM51, _("Zoom to selected layer"), _T(""), wxITEM_NORMAL);
    menuBar->Append(itemMenu28, _("View"));
    wxMenu* itemMenu41 = new wxMenu;
    itemMenu41->Append(ID_MENUITEM43, _("Undo\tCtrl+Z"), _T(""), wxITEM_NORMAL);
    itemMenu41->Append(ID_MENUITEM44, _("Redo\tCtrl+R"), _T(""), wxITEM_NORMAL);
    itemMenu41->AppendSeparator();
    itemMenu41->Append(ID_MENUITEM40, _("Draw\tD"), _T(""), wxITEM_NORMAL);
    itemMenu41->Append(ID_MENUITEM41, _("Modify\tM"), _T(""), wxITEM_NORMAL);
    itemMenu41->Append(ID_MENUITEM42, _("Cut lines\tCtrl+X"), _T(""), wxITEM_NORMAL);
    itemMenu41->Append(ID_MENUITEM53, _("Edit vertex position\tCtrl+V"), _T(""), wxITEM_NORMAL);
    itemMenu41->AppendSeparator();
    itemMenu41->Append(ID_MENUITEM14, _("Merge selected lines\tCtrl+M"), _T(""), wxITEM_NORMAL);
    itemMenu41->Append(ID_MENUITEM26, _("Create intersections\tCtrl+I"), _T(""), wxITEM_NORMAL);
    itemMenu41->Append(ID_MENUITEM15, _("Flip line\tCtrl+F"), _T(""), wxITEM_NORMAL);
    itemMenu41->Append(ID_MENUITEM24, _("Delete selected objects"), _T(""), wxITEM_NORMAL);
    itemMenu41->Append(ID_MENUITEM5, _("Adjust snapping...\tCtrl+S"), _T(""), wxITEM_NORMAL);
    menuBar->Append(itemMenu41, _("Edition"));
    wxMenu* itemMenu55 = new wxMenu;
    itemMenu55->Append(ID_MENUITEM37, _("Object types..."), _T(""), wxITEM_CHECK);
    itemMenu55->Append(ID_MENUITEM39, _("Object attributes...\tCtrl+A"), _T(""), wxITEM_NORMAL);
    itemMenu55->Append(ID_MENUITEM38, _("Object attribute (batch)..."), _T(""), wxITEM_NORMAL);
    itemMenu55->AppendSeparator();
    itemMenu55->Append(ID_MENUITEM54, _("Copy-Paste Attribution\tCtrl+B"), _T(""), wxITEM_NORMAL);
    itemMenu55->AppendSeparator();
    itemMenu55->Append(ID_MENUITEM47, _("Shortcuts..."), _T(""), wxITEM_NORMAL);
    menuBar->Append(itemMenu55, _("Attribution"));
    wxMenu* itemMenu63 = new wxMenu;
    itemMenu63->Append(ID_MENUITEM25, _("Check Geometry..."), _T(""), wxITEM_NORMAL);
    itemMenu63->Append(ID_MENUITEM13, _("Check dangling nodes"), _T(""), wxITEM_NORMAL);
    menuBar->Append(itemMenu63, _("Tools"));
    wxMenu* itemMenu66 = new wxMenu;
    itemMenu66->Append(ID_MENUITEM34, _("Select\tS"), _T(""), wxITEM_NORMAL);
    itemMenu66->Append(ID_MENUITEM33, _("Select all"), _T(""), wxITEM_NORMAL);
    itemMenu66->Append(ID_MENUITEM31, _("Select none"), _T(""), wxITEM_NORMAL);
    itemMenu66->AppendSeparator();
    itemMenu66->Append(ID_MENUITEM32, _("Invert selection"), _T(""), wxITEM_NORMAL);
    itemMenu66->Enable(ID_MENUITEM32, false);
    menuBar->Append(itemMenu66, _("Selection"));
    wxMenu* itemMenu72 = new wxMenu;
    itemMenu72->Append(ID_MENUITEM, _("Queries editor..."), _T(""), wxITEM_NORMAL);
    itemMenu72->AppendSeparator();
    itemMenu72->Append(ID_MENUITEM11, _("User query 1"), _T(""), wxITEM_NORMAL);
    itemMenu72->Append(ID_MENUITEM12, _("User query 2"), _T(""), wxITEM_NORMAL);
    menuBar->Append(itemMenu72, _("Queries"));
    wxMenu* itemMenu77 = new wxMenu;
    itemMenu77->Append(ID_MENUITEM9, _("Table of content"), _T(""), wxITEM_CHECK);
    itemMenu77->Check(ID_MENUITEM9, true);
    itemMenu77->Append(ID_MENUITEM29, _("Information window"), _T(""), wxITEM_CHECK);
    itemMenu77->Append(ID_MENUITEM50, _("Log window"), _T(""), wxITEM_CHECK);
    menuBar->Append(itemMenu77, _("Window"));
    wxMenu* itemMenu81 = new wxMenu(wxID_HELP);
    itemMenu81->Append(wxID_ABOUT); //, _("About..."), _T(""), wxITEM_NORMAL);
    itemMenu81->Append(ID_MENUITEM27, _("Check for updates..."), _T(""), wxITEM_NORMAL);
    menuBar->Append(itemMenu81, _("&Help"));
    return menuBar;
}



