/***************************************************************************
							project_def_layers_dlg.h
	Display Project Definition layers dialog (step 2 of a project definition)
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


#ifndef PROJECT_DEF_LAYERS_DLG_H
#define PROJECT_DEF_LAYERS_DLG_H

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif


#include "listgenreport.h"
#include "wx/notebook.h"
#include "wxflatbutton.h"
#include "project_def_fields_dlg.h"
#include "wx/statline.h" // for static line in ProjectDefLayersEditObjectDlg


#define ID_DLGPDL 10007
#define ID_DLGPDL_LAYER_TYPE 10000
#define ID_DLGPDL_LAYER_NAME 10001
#define ID_NOTEBOOK 10002
#define ID_DLGPDL_PANEL_OBJ 10003
#define ID_DLGPDL_OBJ_LIST 10009
#define ID_DLGPDL_OBJECT_ADD 10010
#define ID_DLGPDL_OBJECT_REMOVE 10225
#define ID_DLGPDL_OBJECT_IMPORT 10251
#define ID_DLGPDL_PANEL_FIELDS 10008
#define ID_DLGPDL_FIELDS_LIST 10220
#define ID_DLGPDL_FIELD_ADD 10005
#define ID_DLGPDL_FIELD_REMOVE 10006
#define SYMBOL_PROJECTDEFLAYERSDLG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_PROJECTDEFLAYERSDLG_TITLE _("Thematic layer definition")
#define SYMBOL_PROJECTDEFLAYERSDLG_IDNAME ID_DLGPDL
#define SYMBOL_PROJECTDEFLAYERSDLG_SIZE wxSize(400, 300)
#define SYMBOL_PROJECTDEFLAYERSDLG_POSITION wxDefaultPosition

class ProjectDefLayersObjectList : public ListGenReport
	{
	private:
		//virtual void OnDoubleClickItem(wxListEvent & event);
//		void OnMySelectionChange (wxListEvent & event);
		
		wxChoice * m_ChoiceToChange;
		
	public:
		static const int ID_PARAMLIST;

		ProjectDefLayersObjectList(wxWindow * parent, wxWindowID id, wxSize size);

		~ProjectDefLayersObjectList(); 

	//	int GetParamType (wxString myTextParam);
//		
//		wxString GetParamType (int imyType);
//
//		void AddItemToParamList(wxString myValue, wxString myComment, ACOMMENTPOSITION type);
//		void SetChoiceList (wxChoice * myChoiceList);
//		DECLARE_EVENT_TABLE();
};

class ProjectDefLayersFieldsList : public ListGenReport
	{
	private:
		//virtual void OnDoubleClickItem(wxListEvent & event);
//		void OnMySelectionChange (wxListEvent & event);
		
		wxChoice * m_ChoiceToChange;
		
	public:
		static const int ID_PARAMLIST;

		ProjectDefLayersFieldsList(wxWindow * parent, wxWindowID id, wxSize size);

		~ProjectDefLayersFieldsList(); 

	//	int GetParamType (wxString myTextParam);
//		
//		wxString GetParamType (int imyType);
//
//		void AddItemToParamList(wxString myValue, wxString myComment, ACOMMENTPOSITION type);
//		void SetChoiceList (wxChoice * myChoiceList);
//		DECLARE_EVENT_TABLE();
};


////@begin control identifiers
#define ID_PROJECTDEFLAYERSEDITOBJECTDLG 10117
#define ID_DLGEO_CODE 10259
#define ID_DLGEO_VALUE 10260
#define ID_STATICLINE2 10261
#define SYMBOL_PROJECTDEFLAYERSEDITOBJECTDLG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_PROJECTDEFLAYERSEDITOBJECTDLG_TITLE _("Edit Object")
#define SYMBOL_PROJECTDEFLAYERSEDITOBJECTDLG_IDNAME ID_PROJECTDEFLAYERSEDITOBJECTDLG
#define SYMBOL_PROJECTDEFLAYERSEDITOBJECTDLG_SIZE wxSize(400, 300)
#define SYMBOL_PROJECTDEFLAYERSEDITOBJECTDLG_POSITION wxDefaultPosition
////@end control identifiers



class ProjectDefLayersEditObjectDlg: public wxDialog
	{    
		DECLARE_DYNAMIC_CLASS( ProjectDefLayersEditObjectDlg )
		//DECLARE_EVENT_TABLE()
		
	public:
		/// Constructors
		ProjectDefLayersEditObjectDlg();
		ProjectDefLayersEditObjectDlg( wxWindow* parent, wxWindowID id = SYMBOL_PROJECTDEFLAYERSEDITOBJECTDLG_IDNAME, const wxString& caption = SYMBOL_PROJECTDEFLAYERSEDITOBJECTDLG_TITLE, const wxPoint& pos = SYMBOL_PROJECTDEFLAYERSEDITOBJECTDLG_POSITION, const wxSize& size = SYMBOL_PROJECTDEFLAYERSEDITOBJECTDLG_SIZE, long style = SYMBOL_PROJECTDEFLAYERSEDITOBJECTDLG_STYLE );
		
		/// Creation
		bool Create( wxWindow* parent, wxWindowID id = SYMBOL_PROJECTDEFLAYERSEDITOBJECTDLG_IDNAME, const wxString& caption = SYMBOL_PROJECTDEFLAYERSEDITOBJECTDLG_TITLE, const wxPoint& pos = SYMBOL_PROJECTDEFLAYERSEDITOBJECTDLG_POSITION, const wxSize& size = SYMBOL_PROJECTDEFLAYERSEDITOBJECTDLG_SIZE, long style = SYMBOL_PROJECTDEFLAYERSEDITOBJECTDLG_STYLE );
		
		/// Destructor
		~ProjectDefLayersEditObjectDlg();
		
		/// Initialises member variables
		void Init();
		
		/// Creates the controls and sizers
		void CreateControls();
		
		////@begin ProjectDefLayersEditObjectDlg member variables
		wxTextCtrl* m_DlgEO_Code;
		wxTextCtrl* m_DlgEO_Value;
		////@end ProjectDefLayersEditObjectDlg member variables
	};





class ProjectDefLayersDlg: public wxDialog
	{    
		void OnAddField (wxCommandEvent & event);
		void OnAddObject (wxCommandEvent & event);
		
		DECLARE_DYNAMIC_CLASS( ProjectDefLayersDlg )
		DECLARE_EVENT_TABLE()
		
	public:
		/// Constructors
		ProjectDefLayersDlg();
		ProjectDefLayersDlg( wxWindow* parent, 
							wxWindowID id = SYMBOL_PROJECTDEFLAYERSDLG_IDNAME,
							const wxString& caption = SYMBOL_PROJECTDEFLAYERSDLG_TITLE,
							const wxPoint& pos = SYMBOL_PROJECTDEFLAYERSDLG_POSITION, 
							const wxSize& size = SYMBOL_PROJECTDEFLAYERSDLG_SIZE, 
							long style = SYMBOL_PROJECTDEFLAYERSDLG_STYLE );
		
		/// Creation
		bool Create( wxWindow* parent, wxWindowID id = SYMBOL_PROJECTDEFLAYERSDLG_IDNAME, 
					const wxString& caption = SYMBOL_PROJECTDEFLAYERSDLG_TITLE,
					const wxPoint& pos = SYMBOL_PROJECTDEFLAYERSDLG_POSITION,
					const wxSize& size = SYMBOL_PROJECTDEFLAYERSDLG_SIZE, 
					long style = SYMBOL_PROJECTDEFLAYERSDLG_STYLE );
		
		/// Destructor
		~ProjectDefLayersDlg();
		
		/// Initialises member variables
		void Init();
		
		/// Creates the controls and sizers
		void CreateControls();
		
		/// controls variables
		wxChoice* m_DlgPDL_Layer_Type;
		wxTextCtrl* m_DlgPDL_Layer_Name;
		wxPanel* m_DlgPDL_Panel_Obj;
		ProjectDefLayersObjectList* m_DlgPDL_Object_List;
		wxPanel* m_DlgPDL_Panel_Fields;
		ProjectDefLayersFieldsList* m_DlgPDL_Fields_List;
		
	};


#endif
