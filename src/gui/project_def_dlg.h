/***************************************************************************
								project_def.h
						Project definition dialog
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
 @file project_def_dlg.h
 @brief Definition of the Project definition dialog
 @details This file provides the class needed for dealing with project
 definition (layers, object and optional field). This dialog is called either
 for a new project or changing the parameters of an existing one. The two class
 implemented are : ProjectDefDLG for the dialog and ProjectDefList for the list
 inside the dialog.
 @author Lucien Schreiber (c) CREALP 2007
 @date 04 December 2007
 *******************************************************************************/



#ifndef PROJECT_DEF_DLG_H
#define PROJECT_DEF_DLG_H

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

////@begin includes
#include "wx/listctrl.h"
#include "listgenreport.h"
#include "wxflatbutton.h"
#include "project_def_layers_dlg.h"
#include <wx/dirdlg.h>				// for directory selection dialog
////@end includes





/*!
 * Forward declarations
 */

////@begin forward declarations
class ProjectDefList;
class wxToggleButton;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_DLGPD 10000
#define ID_DLGPD_PROJ_PATH 10001
#define ID_DLGPD_PROJ_PATH_BTN 10002
#define ID_DLGPD_PROJ_NAME 10003
#define ID_DLGPD_PROJ_UNIT 10156
#define ID_DLGPD_PROJ_PROJECTION 10119
#define ID_DLGPD_SPAT_MODEL_LIST 10004
#define ID_DLGPD_SPAT_MDL_ADD 10219
#define ID_DLGPD_SPAT_MDL_DEL 10006
#define SYMBOL_PROJECTDEFDLG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_PROJECTDEFDLG_TITLE _("Project Definition")
#define SYMBOL_PROJECTDEFDLG_IDNAME ID_DLGPD
#define SYMBOL_PROJECTDEFDLG_SIZE wxSize(400, 300)
#define SYMBOL_PROJECTDEFDLG_POSITION wxDefaultPosition
////@end control identifiers


/***************************************************************************//**
 @brief Definition of the Project definition dialog
 @details This provides the class needed for dealing with project definition
 (layers, object and optional field). This dialog is called either for a new
 project or changing the parameters of an existing one. This dialog implement a
 ProjectDefList List. for displaying the spatial themes of the project
 @author Lucien Schreiber (c) CREALP 2007
 @date 04 December 2007
 *******************************************************************************/
class ProjectDefDLG: public wxDialog
	{    
	private:
		ProjectDefLayersDlg * m_LayerDialog;
		PrjMemLayersArray  m_LayersArray;
		
		void OnAddLayer(wxCommandEvent & event);
		void OnRemoveLayer (wxCommandEvent & event);
		void OnSelectProjectPath (wxCommandEvent & event);
		
		DECLARE_DYNAMIC_CLASS( ProjectDefDLG );
		DECLARE_EVENT_TABLE();
		
		
		
	public:
		/// Constructors
		ProjectDefDLG();
		ProjectDefDLG( wxWindow* parent, wxWindowID id = SYMBOL_PROJECTDEFDLG_IDNAME, 
					  const wxString& caption = SYMBOL_PROJECTDEFDLG_TITLE, 
					  const wxPoint& pos = SYMBOL_PROJECTDEFDLG_POSITION, 
					  const wxSize& size = SYMBOL_PROJECTDEFDLG_SIZE, 
					  long style = SYMBOL_PROJECTDEFDLG_STYLE );
		
		/// Creation
		bool Create( wxWindow* parent, wxWindowID id = SYMBOL_PROJECTDEFDLG_IDNAME,
					const wxString& caption = SYMBOL_PROJECTDEFDLG_TITLE,
					const wxPoint& pos = SYMBOL_PROJECTDEFDLG_POSITION, 
					const wxSize& size = SYMBOL_PROJECTDEFDLG_SIZE, 
					long style = SYMBOL_PROJECTDEFDLG_STYLE );
		
		/// Destructor
		~ProjectDefDLG();
		
		/// Initialises member variables
		void Init();
		
		/// Creates the controls and sizers
		void CreateControls();
		
		//void RemoveObjFromArray();
		
		
		////@begin ProjectDefDLG member variables
		wxTextCtrl* m_DlgPD_Proj_Path;
		wxTextCtrl* m_DlgPd_Proj_Name;
		wxChoice* m_DlgPd_Proj_Unit;
		wxChoice* m_DlgPd_Proj_Projection;
		ProjectDefList * m_DlgPd_Stat_Model_List;
		////@end ProjectDefDLG member variables
	};





/***************************************************************************//**
 @brief List of the Project Definition Dialog
 @details The list managed here deals with project's spatial layers and is part
 of the ProjectDefDLG.
 @author Lucien Schreiber (c) CREALP 2007
 @date 04 December 2007
 *******************************************************************************/
class ProjectDefList : public ListGenReportWithDialog
	{
	private:
		
		virtual void OnPressBackSpace (wxListEvent & event); 
//		void OnMySelectionChange (wxListEvent & event);
				
		wxChoice * m_ChoiceToChange;
		PrjMemLayersArray *  m_LayersArray;
		// create object for storing layers data
		ProjectDefMemoryLayers * m_LayersObj;
		
		virtual void BeforeAdding();
		virtual void AfterAdding (bool bRealyAddItem);
		virtual void BeforeDeleting ();
		virtual void BeforeEditing ();
		virtual void AfterEditing (bool bRealyEdited);
		
	public:
		static const int ID_PARAMLIST;
		
		ProjectDefList(wxWindow * parent, wxWindowID id, wxSize size,  ProjectDefDLG * myParentDlg);
		
		~ProjectDefList(); 
		
		//	int GetParamType (wxString myTextParam);
		//		
		//		wxString GetParamType (int imyType);
		//
		//		void AddItemToParamList(wxString myValue, wxString myComment, ACOMMENTPOSITION type);
		//		void SetChoiceList (wxChoice * myChoiceList);
		//		DECLARE_EVENT_TABLE();
	};



#endif
