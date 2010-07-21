/***************************************************************************
							project_def_fields_dlg.cpp
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

#include "project_def_fields_dlg.h"

/******************************  Field List *************************/
ProjectDefFieldList::ProjectDefFieldList(wxWindow * parent, wxWindowID  id, wxSize size) 
	: ListGenReportWithDialog(parent,id,size)
{

	
	// Create columns
	wxArrayString myColNames;
	wxArrayInt myColsWidths;
	
	myColNames.Add(_("Code"));
	myColNames.Add(_("Description"));
	
	myColsWidths.Add(70);
	myColsWidths.Add(280);
	
	CreateColumns(&myColNames, &myColsWidths);
	
	
	m_pPrjDefinition = NULL;
	m_CodedValueObj = NULL;
	
	m_ChoiceIndex = 0;
	
	

	
	
}


ProjectDefFieldList::~ProjectDefFieldList()
{

}



void ProjectDefFieldList::BeforeAdding ()
{
	// create the object dialog, will be destroyed in AfterAdding
	ProjectDefLayersEditObjectDlg * myEditObjDlg = new ProjectDefLayersEditObjectDlg(this);
	//wxLogDebug(_T("Creating Coded Value Dialog"));
	SetDialog(myEditObjDlg);
	
	// add object to array 
	m_CodedValueObj = m_pPrjDefinition->AddCodedValue();
	
	// now uses Transfert data process
	((ProjectDefLayersEditObjectDlg*)m_pDialog)->SetMemoryCodedValObject(m_CodedValueObj);
}



void ProjectDefFieldList::AfterAdding (bool bRealyAddItem)
{
	wxArrayString myListValues;
	
	if (bRealyAddItem)
	{
    //data allready in the array
	
		
		// add item to the list
		myListValues.Add(wxString::Format(_T("%d"), m_CodedValueObj->m_ValueCode));
		myListValues.Add( m_CodedValueObj->m_ValueName);
		EditDataToList(myListValues);
		
	}
	else
		m_pPrjDefinition->RemoveCodedValue(); // remove last object not used
	
	// delete the dialog
	//wxLogDebug(_T("Deleting Coded Value Dialog"));
	//delete m_pDialog;
}


void ProjectDefFieldList::BeforeEditing ()
{
	// create the dialog, will be deleted in AfterEditing.
	ProjectDefLayersEditObjectDlg * myCodedValueDlg = new ProjectDefLayersEditObjectDlg(this);
	//wxLogDebug(_T("Creating Coded Value Dialog"));
	SetDialog(myCodedValueDlg);
	
	// get selected item from the list
	long mySelectedListItem = GetSelectedItem();
	wxString myValueName = GetItemColText(mySelectedListItem, 1);
	

	m_CodedValueObj = m_pPrjDefinition->FindCodedValue(myValueName, m_ChoiceIndex);
	
	// find item selected and then call a new Dialog
	// for editing the existing Field
	if (m_CodedValueObj != NULL)
	{
		// transfert the data obj to the dialog, data will be 
		// filled during DataTransfer...
		((ProjectDefLayersEditObjectDlg*)m_pDialog)->SetMemoryCodedValObject(m_CodedValueObj);	
	}
	
}
	

void ProjectDefFieldList::AfterEditing (bool bRealyEdited)
{
	wxArrayString myListValues;
	
	if (bRealyEdited)
	{
		// data modified is allready stored in the project definition
		
		// prepare data for list representation
		myListValues.Add(wxString::Format(_T("%d"), m_CodedValueObj->m_ValueCode));
		myListValues.Add( m_CodedValueObj->m_ValueName);
		EditDataToList(myListValues, GetSelectedItem());
		
	}
	
	// delete dialog
    //wxLogDebug(_T("Deleting Coded value Dialog"));
	//delete m_pDialog;
}


void ProjectDefFieldList::BeforeDeleting ()
{
	wxString myValueName;
	// remove item from array before removing it from the list
	// because of the unknown position of item (may have been moved)
	// if a corresponding item was found, remove it from the array
	
	// get selected items from the list
	wxArrayLong mySelectedListItems;
	GetAllSelectedItem(mySelectedListItems);
	for (unsigned int i=0; i< mySelectedListItems.GetCount(); i++)
	{
		myValueName = GetItemColText(mySelectedListItems[i],1);		
		m_pPrjDefinition->FindCodedValue(myValueName, m_ChoiceIndex);
		m_pPrjDefinition->RemoveCodedValue(myValueName);
	}

}


void ProjectDefFieldList::AddingValueToArray (wxArrayString & myImportedValues)
{
	// create new Coded value obj in the array.
	m_CodedValueObj = m_pPrjDefinition->AddCodedValue();
	
	// pass value to this object
	myImportedValues.Item(0).ToLong(&(m_CodedValueObj->m_ValueCode));
	m_CodedValueObj->m_ValueName = myImportedValues.Item(1);
	
}




/******************************  Dialog Field definition *************************/
IMPLEMENT_DYNAMIC_CLASS( ProjectDefFieldDlg, wxDialog )



BEGIN_EVENT_TABLE( ProjectDefFieldDlg, wxDialog )
	//EVT_CHECKBOX(ID_DLGAFD_CONSTRAIN_VALUES, ProjectDefFieldDlg::OnShowConstrainValues)
	EVT_FLATBUTTON (ID_DLGAFD_VAL_ADD,ProjectDefFieldDlg::OnAddAllowedValue)
	EVT_FLATBUTTON (ID_DLGAFD_VAL_IMPORT, ProjectDefFieldDlg::OnImportAllowedValue)
	EVT_FLATBUTTON (ID_DLGAFD_VAL_REMOVE, ProjectDefFieldDlg::OnRemoveAllowedValue)
	EVT_FLATBUTTON (ID_DLGAFD_VAL_EXPORT, ProjectDefFieldDlg::OnExportAllowedValue)
	EVT_TEXT(ID_DLGAFD_FIELD_SCALE, ProjectDefFieldDlg::OnShowLiveResults)
	EVT_TEXT(ID_DLGAFD_FIELD_PRECISION, ProjectDefFieldDlg::OnShowLiveResults)
	//EVT_CHOICE (ID_DLGAFD_DATA_TYPE, ProjectDefFieldDlg::OnChangeFieldType)
	EVT_TEXT(ID_DLGAFD_FIELD_DEF,ProjectDefFieldDlg::OnFieldNameChange)
END_EVENT_TABLE()


void ProjectDefFieldDlg::OnFieldNameChange (wxCommandEvent & event)
{
	if (m_DlgAFD_Field_Def->GetValue().IsEmpty())
	{
		m_DlgAFD_Button_OK->Enable(FALSE);
	}
	else
		m_DlgAFD_Button_OK->Enable(TRUE);
	
}



void ProjectDefFieldDlg::OnImportAllowedValue (wxCommandEvent & event)
{
	// create a new file selector dialog for getting the filename and filterindex
	// of the value file to import in the list.
	wxFileDialog myImportSelector (this, _("Import a file"), _T(""), _T(""),
								   TextParser::GetAllSupportedParserWildCards(), 
								   wxFD_CHANGE_DIR | wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if(myImportSelector.ShowModal() == wxID_OK)
	{
		m_DlgAFD_Coded_Val_List->ImportParsedFileToListCtrl(myImportSelector.GetPath(),
														 myImportSelector.GetFilterIndex());		
	}	
}


void ProjectDefFieldDlg::OnExportAllowedValue (wxCommandEvent & event)
{
	// create a new file selector dialog for setting the filename and filterindex
	// of the file that will be used for exporting the list into
	wxFileDialog myImportSelector (this, _("Export allowed value to file"), _T(""), _T(""),
								   TextParser::GetAllSupportedParserWildCards(), 
								   wxFD_CHANGE_DIR | wxFD_SAVE);
	if(myImportSelector.ShowModal() == wxID_OK)
	{
		m_DlgAFD_Coded_Val_List->ExportListParsedToFile(myImportSelector.GetPath(),
															myImportSelector.GetFilterIndex());		
	}	
	
}


void ProjectDefFieldDlg::OnRemoveAllowedValue (wxCommandEvent & event)
{
	m_DlgAFD_Coded_Val_List->DeleteItem();
}

void ProjectDefFieldDlg::OnShowLiveResults (wxCommandEvent & event)
{
	/// called when scale or precision controls changes
	wxString myTempResultString = _T(""); 
	int iPrecision = m_DlgAFD_Field_Precision->GetValue();
	int iScale = m_DlgAFD_Field_Scale->GetValue();
	int iLeft = 0;
	int i = 0;
	
	// check that precision is greather than 0 otherwise do
	// nothing
	if (iPrecision > 0 && iPrecision -iScale > 0)
	{
		iLeft = iPrecision - iScale;

		for (i =0;i<iLeft;i++)
			myTempResultString = myTempResultString + _T("0");
		myTempResultString.Append(_T("."));
		for (i =0;i<iScale;i++)
			myTempResultString = myTempResultString + _T("0");
		
		// update the result ctrl
		m_DlgAFD_Result->SetValue(myTempResultString);
	}
	// if precision is null we clean the result ctrl
	else
	{
		m_DlgAFD_Result->Clear();
	}
}



void ProjectDefFieldDlg::OnAddAllowedValue (wxCommandEvent & event)
{
	m_DlgAFD_Coded_Val_List->AddItem();
}


ProjectDefFieldDlg::ProjectDefFieldDlg()
{
    Init();
}

ProjectDefFieldDlg::ProjectDefFieldDlg( wxWindow* parent,
									    PrjDefMemManage * myPrjMemManage,
									   bool isEditingMode,
									   wxWindowID id, const wxString& caption, 
									   const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    
	m_bIsModeEditing = isEditingMode;
	
	Create(parent, id, caption, pos, size, style);
	
	// init prj definition and pass it to the list
	m_pPrjDefinition = myPrjMemManage;
	m_DlgAFD_Coded_Val_List->PassPrjDefToList(m_pPrjDefinition);
	//wxLogDebug(_T("Prj def address = %p"), m_pPrjDefinition);
}


bool ProjectDefFieldDlg::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );
	
    CreateControls();
	if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
	
	/// Set default behaviour for selection
	//FieldTextSelected();
	
	
    return true;
}


ProjectDefFieldDlg::~ProjectDefFieldDlg()
{
	
}


void ProjectDefFieldDlg::Init()
{
	////@begin ProjectDefFieldDlg member initialisation
	m_DlgAFD_Field_Def = NULL;
    m_DlgAFD_Field_Precision = NULL;
    m_DlgAFD_Field_Scale = NULL;
    m_DlgAFD_Result = NULL;
    m_DlgAFD_Coded_Val_List = NULL;
	
	m_MemoryField = NULL;
	m_FieldTypeStatus = TM_FIELD_TEXT;
	////@end ProjectDefFieldDlg member initialisation
}

/***************************************************************************//**
 @brief Transfering text data
 @details Send data from memory to the panel or get data from memory to controls
 @param bSendDataToDlg TRUE means DATA --> DIALOG and FALSE mean : DIALOG -->
 DATA
 @author Lucien Schreiber (c) CREALP 2007
 @date 10 April 2008
 *******************************************************************************/
void ProjectDefFieldDlg::TransfertTextValues(bool bSendDataToDlg)
{
	if (bSendDataToDlg == TRUE)	// DATA --> DIALOG	
	{
		m_DlgAFD_Text_Length->SetValue(m_MemoryField->m_FieldPrecision);
	}
	else	// DIALOG --> DATA
	{
		m_MemoryField->m_FieldType = TM_FIELD_TEXT;
		m_MemoryField->m_FieldPrecision = m_DlgAFD_Text_Length->GetValue();
	}
}


/***************************************************************************//**
 @brief Transfering integer data
 @details Send data from memory to the panel or get data from memory to controls
 @param bSendDataToDlg TRUE means DATA --> DIALOG and FALSE mean : DIALOG -->
 DATA
 @author Lucien Schreiber (c) CREALP 2007
 @date 10 April 2008
 *******************************************************************************/
void ProjectDefFieldDlg::TransfertIntegerValues(bool bSendDataToDlg)
{
	if (bSendDataToDlg == TRUE)	// DATA --> DIALOG	
	{
		//m_DlgAFD_Orientation_Integer->SetValue(m_MemoryField->m_FieldOrientation);
	}
	else	// DIALOG --> DATA
	{
		m_MemoryField->m_FieldType = TM_FIELD_INTEGER;
		//m_MemoryField->m_FieldOrientation = m_DlgAFD_Orientation_Integer->IsChecked();
	}	
}


/***************************************************************************//**
 @brief Transfering float data
 @details Send data from memory to the panel or get data from memory to controls
 @param bSendDataToDlg TRUE means DATA --> DIALOG and FALSE mean : DIALOG -->
 DATA
 @author Lucien Schreiber (c) CREALP 2007
 @date 10 April 2008
 *******************************************************************************/
void ProjectDefFieldDlg::TransfertFloatValues(bool bSendDataToDlg)
{
	if (bSendDataToDlg == TRUE)	// DATA --> DIALOG	
	{
		m_DlgAFD_Field_Precision->SetValue(m_MemoryField->m_FieldPrecision);
		m_DlgAFD_Field_Scale->SetValue(m_MemoryField->m_FieldScale);
		//m_DlgAFD_Orientation_Float->SetValue(m_MemoryField->m_FieldOrientation);
	}
	else	// DIALOG --> DATA
	{
		m_MemoryField->m_FieldType = TM_FIELD_FLOAT;
		m_MemoryField->m_FieldPrecision = m_DlgAFD_Field_Precision->GetValue(); 
		m_MemoryField->m_FieldScale = m_DlgAFD_Field_Scale->GetValue();
		//m_MemoryField->m_FieldOrientation = m_DlgAFD_Orientation_Float->IsChecked();
	}		
}

/***************************************************************************//**
 @brief Transfering date data
 @details Send data from memory to the panel or get data from memory to
 controls. 
 @note If data is transfered from memory to the control nothing is
 done actually (don't need to call this function) but this is kept for
 compatibility.
 @param bSendDataToDlg TRUE means DATA --> DIALOG and FALSE mean : DIALOG -->
 DATA
 @author Lucien Schreiber (c) CREALP 2007
 @date 10 April 2008
 *******************************************************************************/
void ProjectDefFieldDlg::TransfertDateValues(bool bSendDataToDlg)
{
	if (bSendDataToDlg == TRUE)	// DATA --> DIALOG	
	{
		// nothing to be done here
	}
	else	// DIALOG --> DATA
	{
		m_MemoryField->m_FieldType = TM_FIELD_DATE;
	}		
}

/***************************************************************************//**
 @brief Transfering enum data
 @details Send data from memory to the panel or get data from memory to
 controls. 
 @note In case of enums, the data were already passed into the Value array
 @param bSendDataToDlg TRUE means DATA --> DIALOG and FALSE mean : DIALOG -->
 DATA
 @author Lucien Schreiber (c) CREALP 2007
 @date 10 April 2008
 *******************************************************************************/
void ProjectDefFieldDlg::TransfertEnumValues(bool bSendDataToDlg)
{
	wxArrayString myListValues;
	
	if (bSendDataToDlg == TRUE)	// DATA --> DIALOG	
	{
		// fill the list
		for (int i = 0; i< m_pPrjDefinition->GetCountCodedValue(); i++)
		{
			ProjectDefMemoryFieldsCodedVal * myCodedValObj = m_pPrjDefinition->
												GetNextCodedValue();
			
			// fit things returned in the list
			myListValues.Add(wxString::Format(_T("%d"), myCodedValObj->m_ValueCode));
			myListValues.Add(myCodedValObj->m_ValueName);
			m_DlgAFD_Coded_Val_List->EditDataToList(myListValues);
			myListValues.Clear();
		}
	}
	else	// DIALOG --> DATA
	{
		m_MemoryField->m_FieldType = TM_FIELD_ENUMERATION;
	}		
}


/***************************************************************************//**
 @brief disable some controls if in edit mode
 @details This function disable some GUI controls if we are in edit mode (if
 ProjectDefFieldDlg::m_bIsModeEditing is TRUE)
 @author Lucien Schreiber (c) CREALP 2007
 @date 17 April 2008
 *******************************************************************************/
void ProjectDefFieldDlg::DisableControlsForEdition()
{
	if (m_bIsModeEditing)
	{
		// nothing to do ???
		//m_DlgAFD_Choicebook->Disable();
	}
}


/***************************************************************************//**
 @brief Hide panel not usable
 @details During edit mode, we don't want user to be allowed to change field
 type so we delete all panels except the actual used one.
 @param iIndexShow Zero based index of the selected panel (the only panel not
 destroyed)
 @author Lucien Schreiber (c) CREALP 2007
 @date 18 April 2008
 *******************************************************************************/
void ProjectDefFieldDlg::HideUnusedTabs (int iIndexShow)
{
	for (int i=PRJDEF_FIELD_TYPE_NUMBER-1; i>=0; i--)
	{
		if (i != iIndexShow)
			m_DlgAFD_Choicebook->DeletePage(i);
	}
}



bool ProjectDefFieldDlg::TransferDataFromWindow()
{
	// some checks, have we called
	// SetMemoryFieldObject function
	// before ?
	wxASSERT (m_MemoryField != NULL);

	m_MemoryField->m_Fieldname = m_DlgAFD_Field_Def->GetValue();
	
	// transfert values based on the actual selected field type in
	// the choicebook (use the text value not the index, because
	// when in edit mode only one field type is availlable).
	wxString mySelectedTextValue = m_DlgAFD_Choicebook->
					GetPageText(m_DlgAFD_Choicebook->GetSelection());

	if (mySelectedTextValue == PRJDEF_FIELD_TYPE_STRING[TM_FIELD_INTEGER])
	{
		TransfertIntegerValues(FALSE);
	}
	else if (mySelectedTextValue == PRJDEF_FIELD_TYPE_STRING[TM_FIELD_FLOAT])
	{
		TransfertFloatValues(FALSE);
	}
	else if (mySelectedTextValue == PRJDEF_FIELD_TYPE_STRING[TM_FIELD_DATE])
	{
		TransfertDateValues(FALSE);
	}
	else if (mySelectedTextValue == PRJDEF_FIELD_TYPE_STRING[TM_FIELD_ENUMERATION])
	{
		TransfertEnumValues(FALSE);
	}
	else if (mySelectedTextValue == PRJDEF_FIELD_TYPE_STRING[TM_FIELD_TEXT])
	{
		TransfertTextValues(FALSE);
	}
	return TRUE;
}


bool ProjectDefFieldDlg::TransferDataToWindow()
{
	// some checks, have we called
	// SetMemoryFieldObject function
	// before ?
	wxASSERT (m_MemoryField != NULL);
	
	m_DlgAFD_Field_Def->SetValue(m_MemoryField->m_Fieldname);

	
	// delete all fields but the one we need
	if (m_bIsModeEditing)
	{
		HideUnusedTabs(m_MemoryField->m_FieldType);
		m_DlgAFD_Choicebook->SetSelection(0);
	}
	else
	// we select the field type if not in edit mode
	{
		m_DlgAFD_Choicebook->SetSelection(m_MemoryField->m_FieldType);
	}

	// transfert values based on the field type
	// and select good panel
	switch (m_MemoryField->m_FieldType)
	{
		case TM_FIELD_INTEGER:
			TransfertIntegerValues(TRUE);
			//m_DlgAFD_Choicebook->SetSelection(TM_FIELD_INTEGER);
			break;
		case TM_FIELD_FLOAT:
			TransfertFloatValues(TRUE);
			//m_DlgAFD_Choicebook->SetSelection(TM_FIELD_FLOAT);
			break;
		case TM_FIELD_DATE:
			// nothing is done here but keeped for compatibility
			TransfertDateValues(TRUE);
			//m_DlgAFD_Choicebook->SetSelection(TM_FIELD_DATE);
			break;
		case TM_FIELD_ENUMERATION:
			TransfertEnumValues(TRUE);
			//m_DlgAFD_Choicebook->SetSelection(TM_FIELD_ENUMERATION);
			break;
			
		default: // default is field text
			TransfertTextValues(TRUE);
			//m_DlgAFD_Choicebook->SetSelection(TM_FIELD_TEXT);
			break;
		}
	

	
	return TRUE;
}


/*!
 * Control creation for wxDialog
 */
void ProjectDefFieldDlg::CreateControls()
{    
	wxButton* m_sdbSizer1Cancel;
	wxStaticText* m_statictext;
	wxStaticText* m_staticText2;
	wxPanel* m_panel6;
	wxStaticText* m_staticText11;
	wxPanel* m_panel7;
	wxStaticText* m_staticText6;
	wxStaticText* m_staticText7;
	wxStaticText* m_staticText8;
	wxPanel* m_panel8;
	wxPanel* m_panel5;
	wxStaticText* m_staticText111;
	wxPanel* m_panel9;
	wxStdDialogButtonSizer* m_sdbSizer1;
	
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxHORIZONTAL );
	
	m_statictext = new wxStaticText( this, wxID_ANY, _("Attribute name :"), wxDefaultPosition, wxDefaultSize, 0 );
	m_statictext->Wrap( -1 );
	bSizer3->Add( m_statictext, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_DlgAFD_Field_Def = new wxTextCtrl( this, ID_DLGAFD_FIELD_DEF, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer3->Add( m_DlgAFD_Field_Def, 1, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	bSizer2->Add( bSizer3, 0, wxEXPAND|wxLEFT|wxRIGHT, 5 );
	
	wxStaticBoxSizer* sbSizer12;
	sbSizer12 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Attributes type and options") ), wxVERTICAL );
	
	m_DlgAFD_Choicebook = new wxChoicebook( this, ID_DLGAFD_DATA_TYPE, wxDefaultPosition, wxDefaultSize, wxCHB_DEFAULT );
	m_panel5 = new wxPanel( m_DlgAFD_Choicebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxVERTICAL );
	
	
	
	m_panel9 = new wxPanel( m_DlgAFD_Choicebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer6211;
	bSizer6211 = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* sbSizer2211;
	sbSizer2211 = new wxStaticBoxSizer( new wxStaticBox( m_panel9, wxID_ANY, _("Allowed values") ), wxVERTICAL );
	
	
	
	
	
	m_DlgAFD_Coded_Val_List = new ProjectDefFieldList( m_panel9, ID_DLGAFD_CODED_VAL_LIST,  wxSize(100, 200));
	sbSizer2211->Add(m_DlgAFD_Coded_Val_List, 1, wxGROW|wxLEFT|wxRIGHT, 5);
	
	wxBoxSizer* itemBoxSizer21 = new wxBoxSizer(wxHORIZONTAL);
	sbSizer2211->Add(itemBoxSizer21, 0, wxGROW|wxALL, 5);
	
	wxFlatButton* itemFlatButton22 = new wxFlatButton( m_panel9, ID_DLGAFD_VAL_ADD, wxFLATBUTTON_TEXT_ADD );
	itemBoxSizer21->Add(itemFlatButton22, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxTOP|wxBOTTOM, 5);
	
	wxFlatButton* itemFlatButton23 = new wxFlatButton( m_panel9, ID_DLGAFD_VAL_REMOVE, wxFLATBUTTON_TEXT_REMOVE );
	itemBoxSizer21->Add(itemFlatButton23, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxTOP|wxBOTTOM, 5);
	
	wxFlatButton* itemFlatButton24 = new wxFlatButton( m_panel9, ID_DLGAFD_VAL_IMPORT, _("Import..."), wxDefaultSize);
	itemBoxSizer21->Add(itemFlatButton24, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxTOP|wxBOTTOM, 5);
	
	wxFlatButton* itemFlatButton25 = new wxFlatButton( m_panel9, ID_DLGAFD_VAL_EXPORT, _("Export..."), wxDefaultSize);
	itemBoxSizer21->Add(itemFlatButton25, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxTOP|wxBOTTOM, 5);
	
	
	bSizer6211->Add( sbSizer2211, 1, wxEXPAND, 5 );
	
	m_panel9->SetSizer( bSizer6211 );
	m_panel9->Layout();
	bSizer6211->Fit( m_panel9 );
	m_DlgAFD_Choicebook->AddPage( m_panel9, PRJDEF_FIELD_TYPE_STRING[TM_FIELD_ENUMERATION], false );
	
	
	
	
	
	
	
	
	wxStaticBoxSizer* sbSizer2;
	sbSizer2 = new wxStaticBoxSizer( new wxStaticBox( m_panel5, wxID_ANY, _("Options") ), wxVERTICAL );
	
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText2 = new wxStaticText( m_panel5, wxID_ANY, _("Length :"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	bSizer5->Add( m_staticText2, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_DlgAFD_Text_Length = new wxSpinCtrl( m_panel5, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 1000, 0 );
	bSizer5->Add( m_DlgAFD_Text_Length, 1, wxALL, 5 );
	
	sbSizer2->Add( bSizer5, 0, wxEXPAND, 5 );
	
	bSizer6->Add( sbSizer2, 1, wxEXPAND, 5 );
	
	m_panel5->SetSizer( bSizer6 );
	m_panel5->Layout();
	bSizer6->Fit( m_panel5 );
	m_DlgAFD_Choicebook->AddPage( m_panel5,PRJDEF_FIELD_TYPE_STRING[TM_FIELD_TEXT], false );
	m_panel6 = new wxPanel( m_DlgAFD_Choicebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer62;
	bSizer62 = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* sbSizer22;
	sbSizer22 = new wxStaticBoxSizer( new wxStaticBox( m_panel6, wxID_ANY, _("Options") ), wxVERTICAL );
	
	m_staticText11 = new wxStaticText( m_panel6, wxID_ANY, _("Integer fields may store numeric values :\n\nfrom -2147483647\nto 2147483647"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
	m_staticText11->Wrap( -1 );
	sbSizer22->Add( m_staticText11, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );
		
	bSizer62->Add( sbSizer22, 1, wxEXPAND, 5 );
	
	m_panel6->SetSizer( bSizer62 );
	m_panel6->Layout();
	bSizer62->Fit( m_panel6 );
	m_DlgAFD_Choicebook->AddPage( m_panel6,PRJDEF_FIELD_TYPE_STRING[TM_FIELD_INTEGER], false );
	m_panel7 = new wxPanel( m_DlgAFD_Choicebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer61;
	bSizer61 = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* sbSizer21;
	sbSizer21 = new wxStaticBoxSizer( new wxStaticBox( m_panel7, wxID_ANY, _("Options") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer( 3, 2, 0, 0 );
	fgSizer1->AddGrowableCol( 1 );
	fgSizer1->SetFlexibleDirection( wxBOTH );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText6 = new wxStaticText( m_panel7, wxID_ANY, _("Field Precision :"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText6->Wrap( -1 );
	fgSizer1->Add( m_staticText6, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_DlgAFD_Field_Precision = new wxSpinCtrl( m_panel7, ID_DLGAFD_FIELD_PRECISION, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100, 0 );
	fgSizer1->Add( m_DlgAFD_Field_Precision, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText7 = new wxStaticText( m_panel7, wxID_ANY, _("Field scale :"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText7->Wrap( -1 );
	fgSizer1->Add( m_staticText7, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_DlgAFD_Field_Scale = new wxSpinCtrl( m_panel7, ID_DLGAFD_FIELD_SCALE, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100, 0 );
	fgSizer1->Add( m_DlgAFD_Field_Scale, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText8 = new wxStaticText( m_panel7, wxID_ANY, _("Result Sample :"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText8->Wrap( -1 );
	fgSizer1->Add( m_staticText8, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_DlgAFD_Result = new wxTextCtrl( m_panel7, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 150,-1 ), 0 );
	fgSizer1->Add( m_DlgAFD_Result, 1, wxALL|wxEXPAND, 5 );
	
	sbSizer21->Add( fgSizer1, 0, wxEXPAND, 5 );
		
	bSizer61->Add( sbSizer21, 1, wxEXPAND, 5 );
	
	m_panel7->SetSizer( bSizer61 );
	m_panel7->Layout();
	bSizer61->Fit( m_panel7 );
	m_DlgAFD_Choicebook->AddPage( m_panel7, PRJDEF_FIELD_TYPE_STRING[TM_FIELD_FLOAT], true );
	m_panel8 = new wxPanel( m_DlgAFD_Choicebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer621;
	bSizer621 = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* sbSizer221;
	sbSizer221 = new wxStaticBoxSizer( new wxStaticBox( m_panel8, wxID_ANY, _("Options") ), wxVERTICAL );
	
	m_staticText111 = new wxStaticText( m_panel8, wxID_ANY, _("No options availlable for this type"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
	m_staticText111->Wrap( -1 );
	sbSizer221->Add( m_staticText111, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 5 );
	
	bSizer621->Add( sbSizer221, 1, wxEXPAND, 5 );
	
	m_panel8->SetSizer( bSizer621 );
	m_panel8->Layout();
	bSizer621->Fit( m_panel8 );
	m_DlgAFD_Choicebook->AddPage( m_panel8, PRJDEF_FIELD_TYPE_STRING[TM_FIELD_DATE], false );
	
	sbSizer12->Add( m_DlgAFD_Choicebook, 1, wxALL|wxGROW, 5 );
	
	bSizer2->Add( sbSizer12, 1, wxALL|wxEXPAND, 5 );
	
	
	m_DlgAFD_Choicebook->SetSelection(0);
	
	m_sdbSizer1 = new wxStdDialogButtonSizer();
	m_DlgAFD_Button_OK = new wxButton( this, wxID_OK );
	m_DlgAFD_Button_OK->Enable(FALSE);
	m_sdbSizer1->AddButton( m_DlgAFD_Button_OK );
	m_sdbSizer1Cancel = new wxButton( this, wxID_CANCEL );
	m_sdbSizer1->AddButton( m_sdbSizer1Cancel );
	m_sdbSizer1->Realize();
	bSizer2->Add( m_sdbSizer1, 0, wxALL|wxEXPAND, 5 );
	
	// disable controls if in edit mode
	DisableControlsForEdition();
	
	this->SetSizer( bSizer2 );
	this->Layout();
	bSizer2->Fit( this );
	
	// validators
	SetValidator();
	
	
	
}

/***************************************************************************//**
 @brief Set the validator for the controls
 @author Lucien Schreiber (c) CREALP 2008
 @date 20 May 2008
 *******************************************************************************/
void ProjectDefFieldDlg::SetValidator()
{
	m_DlgAFD_Field_Def->SetValidator(tmValidator(tmFILTER_EXCLUDE_CHAR_DATABASE));
}


