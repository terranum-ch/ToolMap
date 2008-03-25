/***************************************************************************
						projectproperties_dlg.h
	Contain description of class used for ProjectPropertiesDLG called 
			when user press Project->Edit->Project Settings menu
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

#ifndef _PROJECTPROPERTIES_DLG_H_
#define _PROJECTPROPERTIES_DLG_H_


#include "wx/notebook.h"
#include "wx/tglbtn.h"
#include "wx/statline.h"
#include <wx/filepicker.h>
#include "wxflatbutton.h"
#include "listgenreport.h"				// for base listgenreport definition
#include "../database/database_tm.h"	// for database manipulation
#include "../core/scale.h"				// for scale static conversion

class wxNotebook;
class ScaleList;

#define ID_PROJECTPROPERTIES 10132
#define ID_DLGPS_NOTEBOOK 10141
#define ID_DLGPS_PANEL_SETTINGS 10142
#define ID_DLGPS_EXPORT_TYPE_CHOICE 10000
#define ID_DLGPS_EXPORT_PATH 10001
#define ID_DLGPS_BACKUP_PATH 10003
#define ID_DLGPS_PANEL_SCALE 10133
#define ID_DLGPS_SCALE_LIST 10134
#define ID_DLGPS_BTN_SCALE_ADD 10239
#define ID_DLGPS_BTN_SCALE_DEL 10240
#define ID_DLGPS_SCALE_ORDER_CHOICE 10135
#define SYMBOL_PROJECTPROPERTIESDLG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_PROJECTPROPERTIESDLG_TITLE _("Project settings")
#define SYMBOL_PROJECTPROPERTIESDLG_IDNAME ID_PROJECTPROPERTIES
#define SYMBOL_PROJECTPROPERTIESDLG_SIZE wxSize(400, 300)
#define SYMBOL_PROJECTPROPERTIESDLG_POSITION wxDefaultPosition


class ProjectPropertiesDLG: public wxDialog
{   
	DataBaseTM * m_DBHandler;
	// event function
	
	
	
	// controls
	wxNotebook* m_DLGPS_Notebook;
    wxChoice* m_DLGPS_Export_Type_Choice;
    wxDirPickerCtrl * m_DLGPS_Export_Path;
    wxDirPickerCtrl * m_DLGPS_Backup_Path;
    ScaleList* m_DLGPS_Scale_List;
    wxChoice* m_DLGPS_Scale_Order_Choice;
	
	
    DECLARE_DYNAMIC_CLASS( ProjectPropertiesDLG )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    ProjectPropertiesDLG();
    ProjectPropertiesDLG( wxWindow* parent,
						 DataBaseTM * database,
						 wxWindowID id = SYMBOL_PROJECTPROPERTIESDLG_IDNAME, 
						 const wxString& caption = SYMBOL_PROJECTPROPERTIESDLG_TITLE, 
						 const wxPoint& pos = SYMBOL_PROJECTPROPERTIESDLG_POSITION,
						 const wxSize& size = SYMBOL_PROJECTPROPERTIESDLG_SIZE, 
						 long style = SYMBOL_PROJECTPROPERTIESDLG_STYLE );

    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_PROJECTPROPERTIESDLG_IDNAME,
				const wxString& caption = SYMBOL_PROJECTPROPERTIESDLG_TITLE,
				const wxPoint& pos = SYMBOL_PROJECTPROPERTIESDLG_POSITION,
				const wxSize& size = SYMBOL_PROJECTPROPERTIESDLG_SIZE,
				long style = SYMBOL_PROJECTPROPERTIESDLG_STYLE );

    ~ProjectPropertiesDLG();

    void Init();

    void CreateControls();
};



class ScaleList : public ListGenReportWithDialog
	{
	private:
		DataBaseTM * m_DBHandler;
		wxChoice   * m_ChoiceOrder;
		
		
		
		// derived functions to implement 
		virtual void BeforeAdding(){;}
		virtual void AfterAdding (bool bRealyAddItem){;}
		virtual void BeforeDeleting (){;}
		virtual void BeforeEditing (){;}
		virtual void AfterEditing (bool bRealyEdited){;}
		
		DECLARE_EVENT_TABLE()
		
	public:
		/// Constructor
		ScaleList(wxWindow * parent,
							 wxWindowID id,
							 DataBaseTM * database,
							 wxSize size = wxDefaultSize);
		
		/// Destructor
		~ScaleList(); 
		
		// pass pointer to ctrls to the list
		void SetListCtrls (wxChoice * orderchoice = NULL){m_ChoiceOrder =  orderchoice;}
		
		void SetScaleToList (long lscale, int index = -1);
				
	};



#endif
