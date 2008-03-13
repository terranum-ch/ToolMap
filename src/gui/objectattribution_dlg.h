/***************************************************************************
								objectattribution_dlg.h
					Display the dialog called when user press the
					Project->Edit->Object definition menu
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

#ifndef _OBJECT_ATTRIBUTION_H_
#define _OBJECT_ATTRIBUTION_H_

#include "wx/notebook.h"
#include "wx/listctrl.h"
#include "wx/tglbtn.h"
#include "wx/statline.h"
#include "objectdefinitionlistdlg.h" // For list and edition dialog definition
#include "wxflatbutton.h"
class wxNotebook;


#define ID_DLG_ATTRIBUTION 10025
#define ID_DLGPEO_NOTEBOOK 10026
#define ID_DLGPEO_PANEL_LINE 10027
#define ID_DLGPEO_LISTLINE 10264
#define ID_DLGPEO_LYR_NAME_LINE 10234
#define ID_DLGPEO_LINE_FRQ 10235
#define ID_DLGPEO_PANEL_POINT 10028
#define ID_DLGPEO_LISTPOINT 10000
#define ID_DLGPEO_LYR_NAME_POINT 10234
#define ID_DLGPEO_PANEL_POLY 10029
#define ID_DLGPEO_LISTPOLY 10000
#define ID_DLGPEO_LYR_NAME_POLY 10234
#define ID_DLGPEO_BTN_ADD 10031
#define ID_DLGPEO_BTN_DEL 10032
#define ID_DLGPEO_BTN_IMPORT 10232
#define ID_DLGPEO_BTN_EXPORT 10233
#define SYMBOL_PROJECTEDITOBJECTDEFINITIONDLG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxMAXIMIZE_BOX
#define SYMBOL_PROJECTEDITOBJECTDEFINITIONDLG_TITLE _("Objects definition")
#define SYMBOL_PROJECTEDITOBJECTDEFINITIONDLG_IDNAME ID_DLG_ATTRIBUTION
#define SYMBOL_PROJECTEDITOBJECTDEFINITIONDLG_SIZE wxDefaultSize
#define SYMBOL_PROJECTEDITOBJECTDEFINITIONDLG_POSITION wxDefaultPosition



class ProjectEditObjectDefinitionDLG: public wxDialog
{    
private:
	DataBaseTM * m_DB;
	
	// EVENT FUNCTION
	void OnAddObject (wxCommandEvent & event);
	
	// PRIVATE DATABASE FUNCTION
	bool SetChoiceListText (wxChoice * choice, int listtype); 
	//bool SetListText (int ilayertype, ListGenReport * liste);
	
	
	DECLARE_DYNAMIC_CLASS( ProjectEditObjectDefinitionDLG )
    DECLARE_EVENT_TABLE()
	
public:
    /// Constructors
    ProjectEditObjectDefinitionDLG();
    ProjectEditObjectDefinitionDLG( wxWindow* parent,DataBaseTM * pDatabase, 
								   wxWindowID id = SYMBOL_PROJECTEDITOBJECTDEFINITIONDLG_IDNAME,
								   const wxString& caption = SYMBOL_PROJECTEDITOBJECTDEFINITIONDLG_TITLE,
								   const wxPoint& pos = SYMBOL_PROJECTEDITOBJECTDEFINITIONDLG_POSITION,
								   const wxSize& size = SYMBOL_PROJECTEDITOBJECTDEFINITIONDLG_SIZE,
								   long style = SYMBOL_PROJECTEDITOBJECTDEFINITIONDLG_STYLE );

    /// Creation
    bool Create( wxWindow* parent, 
				wxWindowID id = SYMBOL_PROJECTEDITOBJECTDEFINITIONDLG_IDNAME, 
				const wxString& caption = SYMBOL_PROJECTEDITOBJECTDEFINITIONDLG_TITLE, 
				const wxPoint& pos = SYMBOL_PROJECTEDITOBJECTDEFINITIONDLG_POSITION, 
				const wxSize& size = SYMBOL_PROJECTEDITOBJECTDEFINITIONDLG_SIZE, 
				long style = SYMBOL_PROJECTEDITOBJECTDEFINITIONDLG_STYLE );

    /// Destructor
    ~ProjectEditObjectDefinitionDLG();

    void Init();
	void PostInit();

    void CreateControls();

    wxNotebook* m_DLGPEO_Notebook;
    wxPanel* m_DLGPEO_Panel_Line;
    ObjectDefinitionList* m_DLGPEO_List_Line;
    wxChoice* m_DLGPEO_Choice_Lyr_Line_Name;
    wxChoice* m_DLGPEO_Choice_Lyr_Line_Freq;
    wxPanel* m_DLGPEO_Panel_Point;
    ObjectDefinitionList* m_DLGPEO_List_Point;
    wxChoice* m_DLGPEO_Choice_Lyr_Point_Name;
    wxPanel* m_DLGPEO_Panel_Poly;
    ObjectDefinitionList* m_DLGPEO_List_Poly;
    wxChoice* m_DLGPEO_Choice_Lyr_Poly_Name;
};

#endif

