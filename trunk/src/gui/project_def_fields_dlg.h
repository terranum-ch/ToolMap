/***************************************************************************
							project_def_fields_dlg.h
                    Display dialog for adding new field
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


#ifndef PROJECT_DEF_FIELDS_DLG_H
#define PROJECT_DEF_FIELDS_DLG_H

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "listgenreport.h"
#include "wx/notebook.h"
#include "wxflatbutton.h"
#include "wx/spinctrl.h"
#include <wx/choicebk.h>	// for the choicebook, sort of notebook
#include "project_def_layers_dlg.h" // for using the dialog defined there
#include "../core/textparser.h" // for text parsing during importation
#include "../core/prjdefmemmanage.h" // for different project specification.

#define ID_DLGAFD 10011
#define ID_DLGAFD_FIELD_DEF 10012
#define ID_DLGAFD_DATA_TYPE 10013
#define ID_DLGAFD_FIELD_PRECISION 10015
#define ID_DLGAFD_FIELD_SCALE 10014
#define ID_DLGAFD_RESULT 10016
#define ID_DLGAFD_CONSTRAIN_PANEL 10017
#define ID_DLGAFD_ORIENTATION_FIELD 10118
#define ID_DLGAFD_CONSTRAIN_VALUES 10050
#define ID_DLGAFD_CODED_VAL_LIST 10274
#define ID_DLGAFD_VAL_ADD 10276
#define ID_DLGAFD_VAL_REMOVE 10277
#define ID_DLGAFD_VAL_IMPORT 10278
#define ID_DLGAFD_VAL_EXPORT 10279
#define SYMBOL_PROJECTDEFFIELDDLG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_PROJECTDEFFIELDDLG_TITLE _("Attribute field definition")
#define SYMBOL_PROJECTDEFFIELDDLG_IDNAME ID_DLGAFD
#define SYMBOL_PROJECTDEFFIELDDLG_SIZE wxSize(400, -1)
#define SYMBOL_PROJECTDEFFIELDDLG_POSITION wxDefaultPosition


class ProjectDefFieldList : public ListGenReportWithDialog
	{
	private:
		ProjectDefMemoryFieldsCodedVal * m_CodedValueObj;
		PrjDefMemManage *  m_pPrjDefinition;
		int	m_ChoiceIndex;

		//		void OnMySelectionChange (wxListEvent & event);
		
		virtual void	AfterAdding (bool bRealyAddItem);
		virtual void 	AfterEditing (bool bRealyEdited);
		virtual void 	BeforeAdding ();
		virtual void 	BeforeDeleting ();
		virtual void 	BeforeEditing ();
		virtual void	AddingValueToArray (wxArrayString & myImportedValues);
		

		
	public:
		static const int ID_PARAMLIST;
		
		ProjectDefFieldList(wxWindow * parent, wxWindowID id, wxSize size);
		
		~ProjectDefFieldList(); 
		
		void PassPrjDefToList (PrjDefMemManage *  myPrjMemManage) {m_pPrjDefinition = myPrjMemManage;}
		
		//	int GetParamType (wxString myTextParam);
		//		
		//		wxString GetParamType (int imyType);
		//
		//		void AddItemToParamList(wxString myValue, wxString myComment, ACOMMENTPOSITION type);
		//void SetChoiceList (wxChoice * myChoiceList) {m_ChoiceToDefault = myChoiceList;}
		//		DECLARE_EVENT_TABLE();
	};


class ProjectDefFieldDlg: public wxDialog
	{    
		PRJDEF_FIELD_TYPE m_FieldTypeStatus;
		
		ProjectDefMemoryFields * m_MemoryField;
		PrjDefMemManage * m_pPrjDefinition;
		
		void OnShowLiveResults (wxCommandEvent & event);
		//void OnShowConstrainValues(wxCommandEvent & event);
		void OnAddAllowedValue (wxCommandEvent & event);
		void OnImportAllowedValue (wxCommandEvent & event);
		void OnRemoveAllowedValue (wxCommandEvent & event);
		void OnExportAllowedValue (wxCommandEvent & event);
		//void OnChangeFieldType (wxCommandEvent & event);
		
		
		/// graphical function for updating fields options
		/*void EnableAllCtrls (bool bUngray = TRUE);
		void FieldIntegerSelected ();
		void FieldTextSelected();
		void FieldDoubleSelected();
		void FieldDateSelected();
		void FieldContrainSelected();*/
		
		
		DECLARE_DYNAMIC_CLASS( ProjectDefFieldDlg );
		DECLARE_EVENT_TABLE();
		
	public:
		/// Constructors
		ProjectDefFieldDlg();
		ProjectDefFieldDlg( wxWindow* parent,
						   PrjDefMemManage * myPrjMemManage,
						   wxWindowID id = SYMBOL_PROJECTDEFFIELDDLG_IDNAME, 
						   const wxString& caption = SYMBOL_PROJECTDEFFIELDDLG_TITLE, 
						   const wxPoint& pos = SYMBOL_PROJECTDEFFIELDDLG_POSITION, 
						   const wxSize& size = SYMBOL_PROJECTDEFFIELDDLG_SIZE, 
						   long style = SYMBOL_PROJECTDEFFIELDDLG_STYLE );
		
		/// Creation
		bool Create( wxWindow* parent, 
					wxWindowID id = SYMBOL_PROJECTDEFFIELDDLG_IDNAME, 
					const wxString& caption = SYMBOL_PROJECTDEFFIELDDLG_TITLE, 
					const wxPoint& pos = SYMBOL_PROJECTDEFFIELDDLG_POSITION, 
					const wxSize& size = SYMBOL_PROJECTDEFFIELDDLG_SIZE, 
					long style = SYMBOL_PROJECTDEFFIELDDLG_STYLE );
		
		/// Destructor
		~ProjectDefFieldDlg();
		
		/// Initialises member variables
		void Init();
		
		/// Creates the controls and sizers
		void CreateControls();
		
		void SetMemoryFieldObject (ProjectDefMemoryFields * myMemField) {m_MemoryField = myMemField;}
		ProjectDefMemoryFields * GetMemoryFieldObject (){return m_MemoryField;}
		/// Transfer data, is called automaticaly
		/// when the dialog is disabled
		virtual bool TransferDataFromWindow();
		virtual bool TransferDataToWindow();
		
		

	/*	wxChoice* m_DlgAFD_Field_Type;
		wxCheckBox* m_DlgAFD_Field_Orientation;
		wxCheckBox* m_DlgAFD_Constrain_Values;
		wxPanel* m_DlgAFD_Constrain_Panel;*/
		ProjectDefFieldList* m_DlgAFD_Coded_Val_List;
		
		
		wxTextCtrl* m_DlgAFD_Field_Def;
		wxChoicebook* m_DlgAFD_Choicebook;
		wxSpinCtrl* m_DlgAFD_Text_Length;
		wxCheckBox* m_DlgAFD_Orientation_Integer;
		wxSpinCtrl* m_DlgAFD_Field_Precision;
		wxSpinCtrl* m_DlgAFD_Field_Scale;
		wxTextCtrl* m_DlgAFD_Result;
		wxCheckBox* m_DlgAFD_Orientation_Float;
		wxButton* m_DlgAFD_Button_OK;
		
	
		
	};


#endif
