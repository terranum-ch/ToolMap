/***************************************************************************
							project_def_layers_dlg.cpp
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

#include "project_def_layers_dlg.h"




/******************************  Object List *************************/
ProjectDefLayersObjectList::ProjectDefLayersObjectList(wxWindow * parent, wxWindowID  id, wxSize size) 
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
	
	m_ChoiceToChange = NULL;
	
	m_pPrjDefinition = NULL;
	m_SpatialType = LAYER_LINE;
}

ProjectDefLayersObjectList::~ProjectDefLayersObjectList()
{
	//if (!m_ObjectsArray->IsEmpty())
	//	m_ObjectsArray->Clear();
	//delete m_ObjectsArray;
}


void ProjectDefLayersObjectList::BeforeAdding()
{
	// create the dialog, will be destroyed in AfterAdding
	ProjectDefLayersEditObjectDlg * myEditObjDlg = new ProjectDefLayersEditObjectDlg(this);
	//wxLogDebug(_T("Creating Object description Dialog"));
	SetDialog(myEditObjDlg);
	
	m_ObjectObj = m_pPrjDefinition->AddObject();
		
	// now uses Transfert data process
	((ProjectDefLayersEditObjectDlg*)m_pDialog)->SetMemoryObjectObject(m_ObjectObj);
	
}


void ProjectDefLayersObjectList::AfterAdding (bool bRealyAddItem)
{
	wxArrayString myListValues;
	
	if (bRealyAddItem)
	{
		// data allready added to the array
		// set the spatial type
		m_ObjectObj->m_ObjectType = m_SpatialType;
		
		// add item to the list
		myListValues.Add(wxString::Format(_T("%ld"), m_ObjectObj->m_ObjectCode));
		myListValues.Add(m_ObjectObj->m_ObjectName);
		EditDataToList(myListValues);
		
	}
	else
		m_pPrjDefinition->RemoveObject(); // remove last object not used
	
	// delete the dialog
	//wxLogDebug(_T("Deleting Object description Dialog"));
	//delete m_pDialog;
	
}


void ProjectDefLayersObjectList::BeforeDeleting ()
{
//	// remove item from array before removing it from the list
//	// because of the unknown position of item (may have been moved)
//	// if a corresponding item was found, remove it from the array
//		
//	// get selected item from the list
//	long mySelectedListItem = GetSelectedItem();
//	wxString myObjectName = GetItemColText(mySelectedListItem, 1);
//	
//	m_pPrjDefinition->RemoveObject(myObjectName);
	
	wxString myObjectName;
	// remove item from array before removing it from the list
	// because of the unknown position of item (may have been moved)
	// if a corresponding item was found, remove it from the array
	
	// get selected items from the list
	wxArrayLong mySelectedListItems;
	GetAllSelectedItem(mySelectedListItems);
	for (unsigned int i=0; i< mySelectedListItems.GetCount(); i++)
	{
		myObjectName = GetItemColText(mySelectedListItems[i], 1);		
		m_pPrjDefinition->RemoveObject(myObjectName);
	}
	
	
}


void ProjectDefLayersObjectList::BeforeEditing ()
{
	// create the dialog, will be deleted in AfterEditing.
	ProjectDefLayersEditObjectDlg * myObjectDlg = new ProjectDefLayersEditObjectDlg(this);
	//wxLogDebug(_T("Creating Object description Dialog"));
	SetDialog(myObjectDlg);
	
	// get selected item from the list
	long mySelectedListItem = GetSelectedItem();
	wxString myObjectName = GetItemColText(mySelectedListItem, 1);
	
	m_ObjectObj = m_pPrjDefinition->FindObject(myObjectName);
	
	//	// find item selected and then call a new Dialog
	//	// for editing the existing Field
	//	int iItemIndex = FindObjInLayersArray(this, m_LayersArray);
	if (m_ObjectObj != NULL)
	{
		// transfert the data obj to the dialog, data will be 
		// filled during DataTransfer...
		((ProjectDefLayersEditObjectDlg*)m_pDialog)->SetMemoryObjectObject(m_ObjectObj);	
	}
	 	
}


void ProjectDefLayersObjectList::AfterEditing(bool bRealyEdited)
{
	wxArrayString myListValues;
	
	if (bRealyEdited)
	{
		// data modified is allready stored in the project definition
		
		// prepare data for list representation
		myListValues.Add(wxString::Format(_T("%ld"), m_ObjectObj->m_ObjectCode));
		myListValues.Add(m_ObjectObj->m_ObjectName);
		EditDataToList(myListValues, GetSelectedItem());
		
		
	}
		
	// delete dialog
    //wxLogDebug(_T("Deleting Object description Dialog"));
	//delete m_pDialog;

}


void ProjectDefLayersObjectList::AddingValueToArray (wxArrayString & myImportedValues)
{
	// create new Object obj in the array.
	m_ObjectObj = m_pPrjDefinition->AddObject();
	
	// pass value to this object
	myImportedValues.Item(0).ToLong(&(m_ObjectObj->m_ObjectCode));
	m_ObjectObj->m_ObjectName = myImportedValues.Item(1);
}





/******************************  Field List *************************/
ProjectDefLayersFieldsList::ProjectDefLayersFieldsList(wxWindow * parent, wxWindowID  id, wxSize size, ProjectDefLayersDlg * myDlg) 
	: ListGenReportWithDialog(parent,id,size)
{
	m_DlgParent = myDlg;
	
	// Create columns
	wxArrayString myColNames;
	wxArrayInt myColsWidths;
	
	myColNames.Add(_("Name"));
	myColNames.Add(_("Type"));
	myColNames.Add(_("Orientation"));
	
	myColsWidths.Add(150);
	myColsWidths.Add(80);
	myColsWidths.Add(85);
	
	CreateColumns(&myColNames, &myColsWidths);
	
	m_ChoiceToChange = NULL;
	
	m_pPrjDefinition = NULL;
	m_FieldsObj = NULL;
	m_ChkOrientation = NULL;
	
	m_bIsModeEditing = myDlg->IsEditMode();
	
}

BEGIN_EVENT_TABLE( ProjectDefLayersFieldsList, ListGenReportWithDialog)
	EVT_LIST_ITEM_SELECTED(wxID_ANY, ProjectDefLayersFieldsList::OnItemSelectChange)
END_EVENT_TABLE()


/***************************************************************************//**
 @brief Called when item is selected
 @details This functions update the checkbox
 (ProjectDefLayersFieldsList::m_ChkOrientation) based on the Field orientation
 status
 @author Lucien Schreiber (c) CREALP 2007
 @date 08 May 2008
 *******************************************************************************/
void ProjectDefLayersFieldsList::OnItemSelectChange (wxListEvent & event)
{
	// check validity of checkbox
	if (m_ChkOrientation == NULL)
	{
		wxLogError(_T("Checkbox pointer is null, use SetListCtrl() to pass")
				   _T(" pointer adress !"));
		return;
	}
	
	int iIndex = GetSelectedItem();
	// works on the first item only
	if (iIndex >= 0)
	{
		wxString chkvalue = GetItemColText(iIndex, 2);
		if (chkvalue == PRJDEF_FIELD_ORIENTATION_STRING[TM_FIELD_ORIENT_YES])
			m_ChkOrientation->SetValue(TRUE);
		else
			m_ChkOrientation->SetValue(FALSE);
	}

}


ProjectDefLayersFieldsList::~ProjectDefLayersFieldsList()
{
	m_DlgParent = NULL;
}



void ProjectDefLayersFieldsList::BeforeAdding ()
{
	// create the dialog, will be destroyed in AfterAdding
	ProjectDefFieldDlg * myFieldDlg = new ProjectDefFieldDlg(this,m_pPrjDefinition);
	//wxLogDebug(_T("Creating Field Dialog"));
	SetDialog(myFieldDlg);
	
	m_FieldsObj =  m_pPrjDefinition->AddField();
	
	// now uses Transfert data process
	((ProjectDefFieldDlg*)m_pDialog)->SetMemoryFieldObject(m_FieldsObj);
}



void ProjectDefLayersFieldsList::AfterAdding (bool bRealyAddItem)
{
	wxArrayString myListValues;
	
	if (bRealyAddItem)
	{
		// data allready added to the array
		
		
		// add item to the list
		myListValues.Add(m_FieldsObj->m_Fieldname);
		myListValues.Add(PRJDEF_FIELD_TYPE_STRING[m_FieldsObj->m_FieldType]);
		EditDataToList(myListValues);
		
	}
	else
		m_pPrjDefinition->RemoveField(); // remove last field not used
	
	// delete the dialog
	//wxLogDebug(_T("Deleting Field Dialog"));
	//delete m_pDialog;	
}


void ProjectDefLayersFieldsList::BeforeDeleting ()
{
	wxString myFieldName;
	ProjectDefMemoryFields * field = NULL;
	// remove item from array before removing it from the list
	// because of the unknown position of item (may have been moved)
	// if a corresponding item was found, remove it from the array
	
	// get selected items from the list
	wxArrayLong mySelectedListItems;
	GetAllSelectedItem(mySelectedListItems);
	for (unsigned int i=0; i< mySelectedListItems.GetCount(); i++)
	{
		myFieldName = GetItemColText(mySelectedListItems[i], 0);		

		// if we are in editing mode we must save the items to delete
		// in the delete field array.
		if (m_bIsModeEditing == TRUE)
		{
			field = m_pPrjDefinition->FindField(myFieldName);
			if (field && field->m_FieldID > 0)
			{
				// if field exists in the database we mark it for deleting
				wxLogDebug(_T("Marqued field for deleting : %s"), field->m_FieldOldName.c_str());
				(m_pPrjDefinition->GetActiveLayer())->m_StoreDeleteFields.Add(field->m_FieldOldName);
			}
		}
		
		m_pPrjDefinition->RemoveField(myFieldName);
	}

}


void ProjectDefLayersFieldsList::BeforeEditing ()
{
	// create the dialog, will be deleted in AfterEditing.
	ProjectDefFieldDlg * myFieldDlg = new ProjectDefFieldDlg(this,m_pPrjDefinition, m_bIsModeEditing);
	//wxLogDebug(_T("Creating Field Dialog"));
	SetDialog(myFieldDlg);
	
	// get selected item from the list
	long mySelectedListItem = GetSelectedItem();
	wxString myFieldName = GetItemColText(mySelectedListItem, 0);
	
	m_FieldsObj = m_pPrjDefinition->FindField(myFieldName);
	
	//	// find item selected and then call a new Dialog
	//	// for editing the existing Field
	//	int iItemIndex = FindObjInLayersArray(this, m_LayersArray);
	if (m_FieldsObj != NULL)
	{
		// transfert the data obj to the dialog, data will be 
		// filled during DataTransfer...
		((ProjectDefFieldDlg*)m_pDialog)->SetMemoryFieldObject(m_FieldsObj);	
	}
	
}


void ProjectDefLayersFieldsList::AfterEditing (bool bRealyEdited)
{
	wxArrayString myListValues;
	
	if (bRealyEdited)
	{
		// data modified is allready stored in the project definition
		
		// prepare data for list representation
		myListValues.Add(m_FieldsObj->m_Fieldname);
		myListValues.Add(PRJDEF_FIELD_TYPE_STRING[m_FieldsObj->m_FieldType]);
		EditDataToList(myListValues, GetSelectedItem());
		
	}
	
	// delete dialog
    //wxLogDebug(_T("Deleting Field Dialog"));
	//delete m_pDialog;
	
}



/***************************************************************************//**
 @brief Set orientation for field
 @details This function set or clear orientation for specified field.
 @param orientation value of orientation to set. Allowed values are : 
 - TM_FIELD_ORIENT_YES 
 - TM_FIELD_ORIENT_NO
 @param index Zero based index of the item to modify.
 @author Lucien Schreiber (c) CREALP 2008
 @date 08 May 2008
 *******************************************************************************/
void ProjectDefLayersFieldsList::SetOrientation (int orientation, const int & index)
{
	wxString myFieldName;
	ProjectDefMemoryFields * field = NULL;
	
			
	myFieldName = GetItemColText(index,0);
	field = m_pPrjDefinition->FindField(myFieldName);
	
	// if field not found -> Error
	if (field == NULL)
	{
		wxLogError(_T("Field with name : %s not found in memory array"),
				   myFieldName.c_str());
		return;
	}
	

	// update UI
	SetItemText(index, 2, PRJDEF_FIELD_ORIENTATION_STRING[orientation]);
	
	// finally change the frequency to the specified value
	if (orientation == TM_FIELD_ORIENT_YES)
		field->m_FieldOrientation =  TRUE;
	else
		field->m_FieldOrientation = FALSE;

}


/***************************************************************************//**
 @brief Check if a field may be used for orientation
 @details Only following fields could be set as orientation fields :
 - INTEGER
 - FLOAT
 @param index Zero based item index
 @return  TRUE if fields may be used for orientation
 @author Lucien Schreiber (c) CREALP 2008
 @date 08 May 2008
 *******************************************************************************/
bool ProjectDefLayersFieldsList::IsOrientationAllowed(int index)
{
	wxString fieldtype = GetItemColText(index, 1);
	if (fieldtype == PRJDEF_FIELD_TYPE_STRING[TM_FIELD_INTEGER] ||
		fieldtype == PRJDEF_FIELD_TYPE_STRING[TM_FIELD_FLOAT])
	{
		return TRUE;
	}
	return FALSE;
}




/******************************  Add object Dialog Class *************************/
IMPLEMENT_DYNAMIC_CLASS( ProjectDefLayersEditObjectDlg, wxDialog )

BEGIN_EVENT_TABLE (ProjectDefLayersEditObjectDlg, wxDialog)
	EVT_TEXT (ID_DLGEO_CODE, ProjectDefLayersEditObjectDlg::OnTextChange)
	EVT_TEXT (ID_DLGEO_VALUE, ProjectDefLayersEditObjectDlg::OnTextChange)
END_EVENT_TABLE ()


void ProjectDefLayersEditObjectDlg::OnTextChange(wxCommandEvent & event)
{
	if (m_DlgEO_Code->GetValue().IsEmpty() || m_DlgEO_Value->GetValue().IsEmpty())
	{
		m_DlgEO_OK_Btn->Enable(FALSE);
	}
	else
		m_DlgEO_OK_Btn->Enable(TRUE);
}

ProjectDefLayersEditObjectDlg::ProjectDefLayersEditObjectDlg()
{
    Init();
}

ProjectDefLayersEditObjectDlg::ProjectDefLayersEditObjectDlg( wxWindow* parent, wxWindowID id, 
															const wxString& caption, const wxPoint& pos, 
															 const wxSize& size, long style )
{
    Init();
    Create(parent, id, caption, pos, size, style);
}

bool ProjectDefLayersEditObjectDlg::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );
	
    CreateDlgControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
    return true;
}


ProjectDefLayersEditObjectDlg::~ProjectDefLayersEditObjectDlg()
{
	
}


void ProjectDefLayersEditObjectDlg::Init()
{

	m_DlgEO_Code = NULL;
    m_DlgEO_Value = NULL;
    m_DlgEO_OK_Btn = NULL;
	
	m_ObjectObj = NULL;
	m_CodedValObj = NULL;
}


void ProjectDefLayersEditObjectDlg::CreateDlgControls()
{    

    ProjectDefLayersEditObjectDlg* itemDialog1 = this;
	
    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);
	
    wxFlexGridSizer* itemFlexGridSizer3 = new wxFlexGridSizer(2, 2, 0, 0);
    itemBoxSizer2->Add(itemFlexGridSizer3, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	
    wxStaticText* itemStaticText4 = new wxStaticText( itemDialog1, wxID_STATIC, _("Code :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer3->Add(itemStaticText4, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    wxStaticText* itemStaticText5 = new wxStaticText( itemDialog1, wxID_STATIC, _("Description :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer3->Add(itemStaticText5, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    m_DlgEO_Code = new wxTextCtrl( itemDialog1, ID_DLGEO_CODE, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer3->Add(m_DlgEO_Code, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    m_DlgEO_Value = new wxTextCtrl( itemDialog1, ID_DLGEO_VALUE, _T(""), wxDefaultPosition, wxSize(300, -1), 0 );
    itemFlexGridSizer3->Add(m_DlgEO_Value, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    wxStaticLine* itemStaticLine8 = new wxStaticLine( itemDialog1, ID_STATICLINE2, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
    itemBoxSizer2->Add(itemStaticLine8, 0, wxGROW|wxALL, 5);
	
    wxStdDialogButtonSizer* itemStdDialogButtonSizer9 = new wxStdDialogButtonSizer;
	
	itemBoxSizer2->Add(itemStdDialogButtonSizer9, 0, wxALIGN_RIGHT|wxALL, 5);
    m_DlgEO_OK_Btn = new wxButton( itemDialog1, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    m_DlgEO_OK_Btn->SetDefault();
    m_DlgEO_OK_Btn->Enable(false);
    itemStdDialogButtonSizer9->AddButton(m_DlgEO_OK_Btn);
	
	
    wxButton* itemButton11 = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer9->AddButton(itemButton11);
	
    itemStdDialogButtonSizer9->Realize();
	
	// set the validator for the controls
	_SetValidator();
}

/***************************************************************************//**
 @brief Set the validator for the controls
 @author Lucien Schreiber (c) CREALP 2008
 @date 20 May 2008
 *******************************************************************************/
void ProjectDefLayersEditObjectDlg::_SetValidator()
{
	m_DlgEO_Code->SetValidator(tmValidator(tmFILTER_EXCLUDE_CHAR_NUMERIC_STRICT));
	m_DlgEO_Value->SetValidator(tmValidator(tmFILTER_EXCLUDE_CHAR_COMPLEX));
}


bool ProjectDefLayersEditObjectDlg::TransferDataFromWindow(){
	// getting values
    wxString sCode = m_DlgEO_Code->GetValue();
	wxString sValue = m_DlgEO_Value->GetValue();

#ifdef __WXMSW__
	sValue.Replace(_T("’"), _T("'"));
#endif
	// passing values to the corresponding obj.
	if (m_ObjectObj != NULL)
	{
        long myCode = 0;
        sCode.ToLong(&myCode);
		m_ObjectObj->m_ObjectCode = myCode;
		m_ObjectObj->m_ObjectName = sValue;
	}
	
	if (m_CodedValObj != NULL)
	{
		m_CodedValObj->m_ValueCode = sCode;
		m_CodedValObj->m_ValueName = sValue;
	}
	return true;
}


bool ProjectDefLayersEditObjectDlg::TransferDataToWindow()
{
	if (m_ObjectObj != NULL)
	{
		// check that the numeric value is not null (-9999)
		if (m_ObjectObj->m_ObjectCode != NULL_LONG_VALUE)
		{
			m_DlgEO_Code->SetValue(wxString::Format(_T("%ld"), m_ObjectObj->m_ObjectCode));
		}
		m_DlgEO_Value->SetValue(m_ObjectObj->m_ObjectName);
	}
	
	
	if (m_CodedValObj != NULL)
	{
		// check that the numeric value is not null (-9999)
		if (m_CodedValObj->m_ValueCode != NULL_LONG_STRING_VALUE)
		{
			m_DlgEO_Code->SetValue(m_CodedValObj->m_ValueCode);
		}
		m_DlgEO_Value->SetValue(m_CodedValObj->m_ValueName);
		
	}
	
	return TRUE;
}

/******************************  Dialog Class *************************/
IMPLEMENT_DYNAMIC_CLASS( ProjectDefLayersDlg, wxDialog )

BEGIN_EVENT_TABLE( ProjectDefLayersDlg, wxDialog )
	EVT_FLATBUTTON (ID_DLGPDL_FIELD_ADD,  ProjectDefLayersDlg::OnAddField)
	EVT_FLATBUTTON (ID_DLGPDL_OBJECT_ADD, ProjectDefLayersDlg::OnAddObject )
	EVT_FLATBUTTON (ID_DLGPDL_OBJECT_REMOVE, ProjectDefLayersDlg::OnRemoveObject)
	EVT_FLATBUTTON (ID_DLGPDL_OBJECT_IMPORT, ProjectDefLayersDlg::OnImportObject)
	EVT_FLATBUTTON (ID_DLGPDL_FIELD_REMOVE, ProjectDefLayersDlg::OnRemoveField)
	EVT_CHECKBOX(ID_DLGPDL_CHK_ORIENTATION,ProjectDefLayersDlg::OnChangeOrientation)
	EVT_CHOICE(ID_DLGPDL_LAYER_TYPE,  ProjectDefLayersDlg::OnSelectLayerType)
	EVT_TEXT (ID_DLGPDL_LAYER_NAME, ProjectDefLayersDlg::OnLayerNameChange)
END_EVENT_TABLE()



void ProjectDefLayersDlg::OnAddField (wxCommandEvent & event)
{	
	m_DlgPDL_Fields_List->AddItem();
}



void ProjectDefLayersDlg::OnRemoveField (wxCommandEvent & event)
{
	m_DlgPDL_Fields_List->DeleteItem();
}



void ProjectDefLayersDlg::OnAddObject (wxCommandEvent & event)
{
	m_DlgPDL_Object_List->AddItem();
}


void ProjectDefLayersDlg::OnRemoveObject (wxCommandEvent & event)
{
	// call deletion process from the listclass
	m_DlgPDL_Object_List->DeleteItem();
}


void ProjectDefLayersDlg::OnImportObject (wxCommandEvent & event)
{
	
	
	// create a new file selector dialog
	wxFileDialog myImportSelector (this, _("Import a file"), _T(""), _T(""),
								   TextParser::GetAllSupportedParserWildCards(), 
								   wxFD_CHANGE_DIR | wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if(myImportSelector.ShowModal() == wxID_OK)
	{
		m_DlgPDL_Object_List->ImportParsedFileToListCtrl(myImportSelector.GetPath(),
														 myImportSelector.GetFilterIndex());		
	}
	
}


/***************************************************************************//**
 @brief Called when checkbox is pressed
This functions does the following things  :
 - Set the parameter orientation if field type is Integer or Float
 - Remove every other orientation fields.
 @author Lucien Schreiber (c) CREALP 2008
 @date 08 May 2008
 *******************************************************************************/
void ProjectDefLayersDlg::OnChangeOrientation (wxCommandEvent & event)
{
	int iIndex = 0;
	
	// check : one item must be selected, otherwise return
	iIndex = m_DlgPDL_Fields_List->GetSelectedItem();
	if (iIndex < 0)
	{
		m_DlgPDL_Orientation_FLD->SetValue(FALSE);
		//wxLogDebug(_T("TEMP : no item selected, index is %d"), iIndex);
		return;
	}
	
	// check : field must be of type integer or float
	if (!m_DlgPDL_Fields_List->IsOrientationAllowed(iIndex))
	{
		m_DlgPDL_Orientation_FLD->SetValue(FALSE);
		wxLogMessage(_("Field \"%s\" is not of the Integer or Float type"),
				   m_DlgPDL_Fields_List->GetItemColText(iIndex, 1).c_str());
		return;
	}
	
	
	// if checkbox is checked, check this one and uncheck all others
	if(m_DlgPDL_Orientation_FLD->IsChecked())
	{		
		int iNbItem = m_DlgPDL_Fields_List->GetItemCount();
		//wxString fieldname  = _T("");
		for (int i=0; i<iNbItem; i++)
		{
			if (i!=iIndex)
			{	
				if(m_DlgPDL_Fields_List->IsOrientationAllowed(i))
					m_DlgPDL_Fields_List->SetOrientation(TM_FIELD_ORIENT_NO, i);
			}
			else
				m_DlgPDL_Fields_List->SetOrientation(TM_FIELD_ORIENT_YES, i);
		}
		
		
	}
	else // just uncheck this one
	{
		m_DlgPDL_Fields_List->SetOrientation(TM_FIELD_ORIENT_NO, iIndex);
	}
}

/***************************************************************************//**
 @brief Called every time the layer type is selected
 @details This is called every time when an item on the layer type choice is
 selected.
 @author Lucien Schreiber (c) CREALP 2008
 @date 18 June 2008
 *******************************************************************************/
void ProjectDefLayersDlg::OnSelectLayerType (wxCommandEvent & event)
{
	int iActualSpatType = m_DlgPDL_Layer_Type->GetSelection();
	if (iActualSpatType >= LAYER_LINE && iActualSpatType <= LAYER_POLYGON)
		if (m_DlgPDL_Object_List)
			m_DlgPDL_Object_List->SetSpatialType(iActualSpatType);
	
	bool bShowPolyCtrl = false;
	if (iActualSpatType == LAYER_POLYGON)
		bShowPolyCtrl = true;
	
	if (m_DlgPDL_Contour_Name)
	{
		m_DlgPDL_Contour_Name->Show(bShowPolyCtrl);
		m_DlgPDL_Contour_Static->Show(bShowPolyCtrl);
		
		
		UpdateDefaultPolygonBorderName(m_DlgPDL_Layer_Name->GetValue());
		this->Layout();
	}
	
	
	
	
	ActivateOrientation();
}



/***************************************************************************//**
 @brief Called when layer name is changing
 @details This function updates the Polygon contour value with a
 #m_DlgPDL_Contour_Prefix
 @author Lucien Schreiber (c) CREALP 2008
 @date 03 November 2008
 *******************************************************************************/
void ProjectDefLayersDlg::OnLayerNameChange (wxCommandEvent & event)
{
	UpdateDefaultPolygonBorderName(event.GetString());
	
	event.Skip();
	
}



/***************************************************************************//**
 @brief Update polygon default contour name
 @author Lucien Schreiber (c) CREALP 2008
 @date 03 November 2008
 *******************************************************************************/
void ProjectDefLayersDlg::UpdateDefaultPolygonBorderName (const wxString & name)
{
	if (m_bIsModeEditing == false)
		if (m_DlgPDL_Contour_Name)
			m_DlgPDL_Contour_Name->SetValue(m_DlgPDL_Contour_Prefix + name);
}



/***************************************************************************//**
 @brief Activate / desactivate the Orientation check box ctrl
 @author Lucien Schreiber (c) CREALP 2008
 @date 18 June 2008
 *******************************************************************************/
void ProjectDefLayersDlg::ActivateOrientation()
{
	bool bActivated = FALSE;
	if (m_DlgPDL_Layer_Type->GetSelection() == LAYER_POINT)
		bActivated = TRUE;
	m_DlgPDL_Orientation_FLD->Enable(bActivated);
}


bool ProjectDefLayersDlg::TransferDataFromWindow()
{
	// function automaticaly called when the dialog
	// is closed using the wxID_OK button
	wxASSERT_MSG(m_LayersObj, wxT("Init m_LayersObj First, not initialised."));
	m_LayersObj->m_LayerName = m_DlgPDL_Layer_Name->GetValue();
	m_LayersObj->m_LayerType = (PRJDEF_LAYERS_TYPE) m_DlgPDL_Layer_Type->GetSelection();
	if (m_LayersObj->m_LayerType ==  LAYER_POLYGON &&  m_DlgPDL_Contour_Name)
		m_LayersObj->m_LayerPolygonDefaultValue = m_DlgPDL_Contour_Name->GetValue();
	else
		m_LayersObj->m_LayerPolygonDefaultValue = wxEmptyString;
	return TRUE;
	
}




bool ProjectDefLayersDlg::TransferDataToWindow()
{
	wxArrayString myObjListValues;
	wxArrayString myFieldListValues;
	
	// function automaticaly called when the dialog
	// is showed 
	wxASSERT_MSG(m_LayersObj, wxT("Init m_LayersObj First, Problem in transfert data"));
	
	if (m_LayersObj != NULL)
	{
		// GENERIC VALUES FOR LAYER
		m_DlgPDL_Layer_Name->SetValue(m_LayersObj->m_LayerName); 
		m_DlgPDL_Layer_Type->SetSelection((PRJDEF_LAYERS_TYPE) m_LayersObj->m_LayerType);
		
		if (m_LayersObj->m_LayerType == LAYER_POLYGON && m_bIsModeEditing == false)
		{
			m_DlgPDL_Contour_Name->Show(true);
			m_DlgPDL_Contour_Static->Show(true);
			m_DlgPDL_Contour_Name->SetValue(m_LayersObj->m_LayerPolygonDefaultValue);
			this->Layout();
		}
		
		// fill the object list (but only if panel exists)
		if (m_DlgPDL_Panel_Obj != NULL)
		{
			for (int i = 0; i<m_pPrjDefinition->GetCountObject(); i++)
			{
				ProjectDefMemoryObjects * myObjectObj = m_pPrjDefinition->GetNextObjects();
				
				// fit things returned in the list
				myObjListValues.Add(wxString::Format(_T("%ld"), myObjectObj->m_ObjectCode));
				myObjListValues.Add(myObjectObj->m_ObjectName);
				m_DlgPDL_Object_List->EditDataToList(myObjListValues);
				myObjListValues.Clear();
			}
		}
		
		
		// fill the field list (but only if panel exists)
		if (m_DlgPDL_Panel_Fields != NULL)
		{
			for (int j=0; j < m_pPrjDefinition->GetCountFields(); j++)
			{
				ProjectDefMemoryFields * myFieldObj = m_pPrjDefinition->GetNextField();
				
				// fit things returned in the list
				myFieldListValues.Add(myFieldObj->m_Fieldname);
				myFieldListValues.Add(PRJDEF_FIELD_TYPE_STRING[myFieldObj->m_FieldType]);
				
				if (myFieldObj->m_FieldOrientation)
					myFieldListValues.Add(PRJDEF_FIELD_ORIENTATION_STRING[TM_FIELD_ORIENT_YES]);
				else
					myFieldListValues.Add(PRJDEF_FIELD_ORIENTATION_STRING[TM_FIELD_ORIENT_NO]);
				
				m_DlgPDL_Fields_List->EditDataToList(myFieldListValues);
				myFieldListValues.Clear();
				
			}
		}
	
		// activate or desactivate the checkbox for orientation
		// based on the selection made in the layer type choice ctrl
		ActivateOrientation();
	}
	return TRUE;
}




ProjectDefLayersDlg::ProjectDefLayersDlg()
{
    Init();
}

ProjectDefLayersDlg::ProjectDefLayersDlg( wxWindow* parent, PrjDefMemManage *pPrjDef,
										  bool isEditingMode, bool isAddingMode,
										 wxWindowID id, const wxString& caption, 
										 const wxPoint& pos, const wxSize& size,
										 long style )
{
    Init();
	
	m_bIsModeEditing = isEditingMode;
	m_bIsModeAddingEditing = isAddingMode;
	
    Create(parent, id, caption, pos, size, style);
	
	
	// init prj definition and pass it to the list
	m_pPrjDefinition = pPrjDef;
	if (m_DlgPDL_Panel_Obj != NULL)
		m_DlgPDL_Object_List->PassPrjDefToList(m_pPrjDefinition);
	if (m_DlgPDL_Panel_Fields != NULL)
	m_DlgPDL_Fields_List->PassPrjDefToList(m_pPrjDefinition);
	//wxLogDebug(_T("Prj def address = %p"), m_pPrjDefinition);
	
	// pass address of checkbox to the list
	m_DlgPDL_Fields_List->SetListCtrls(m_DlgPDL_Orientation_FLD);
}



bool ProjectDefLayersDlg::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
    return true;
}




ProjectDefLayersDlg::~ProjectDefLayersDlg()
{
	// clear the array
	/// @todo remove this code if the array dosen't need to be deleted
	if (!m_FieldArray.IsEmpty())
		m_FieldArray.Clear();
}



void ProjectDefLayersDlg::Init()
{
	m_FieldDialog = NULL;
	m_bIsModeEditing = FALSE;

	 m_DlgPDL_Layer_Type = NULL;
	 m_DlgPDL_Layer_Name = NULL;
	 m_DlgPDL_Panel_Obj = NULL;
	 m_DlgPDL_Object_List = NULL;
	 m_DlgPDL_Panel_Fields = NULL;
	 m_DlgPDL_Fields_List = NULL;
	m_DlgPDL_Orientation_FLD = NULL;
	m_DlgPDL_Contour_Prefix = _("Border of ");
	m_DlgPDL_Contour_Name = NULL;
	m_DlgPDL_Contour_Static = NULL;
}


void ProjectDefLayersDlg::DisableControlsForEdition()
{
	m_DlgPDL_Layer_Type->Enable(FALSE);
	
	// see also CreateControls for notebook panel deleting
}


void ProjectDefLayersDlg::CreateControls()
{    
    ProjectDefLayersDlg* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxStaticBox* itemStaticBoxSizer3Static = new wxStaticBox(itemDialog1, wxID_ANY, _("Layer options"));
    wxStaticBoxSizer* itemStaticBoxSizer3 = new wxStaticBoxSizer(itemStaticBoxSizer3Static, wxVERTICAL);
    itemBoxSizer2->Add(itemStaticBoxSizer3, 0, wxGROW|wxALL, 5);

    wxFlexGridSizer* itemFlexGridSizer4 = new wxFlexGridSizer(2, 2, 0, 0);
    itemFlexGridSizer4->AddGrowableCol(1);
    itemStaticBoxSizer3->Add(itemFlexGridSizer4, 0, wxGROW|wxALL, 5);

    wxStaticText* itemStaticText7 = new wxStaticText( itemDialog1, wxID_STATIC, _("Layer name :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer4->Add(itemStaticText7, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_DlgPDL_Layer_Name = new wxTextCtrl( itemDialog1, ID_DLGPDL_LAYER_NAME, _T(""), wxDefaultPosition, wxSize(200, -1), 0 );
    itemFlexGridSizer4->Add(m_DlgPDL_Layer_Name, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);

	
	wxStaticText* itemStaticText5 = new wxStaticText( itemDialog1, wxID_STATIC, _("Layer type :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer4->Add(itemStaticText5, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
	
    m_DlgPDL_Layer_Type = new wxChoice( itemDialog1, ID_DLGPDL_LAYER_TYPE, wxDefaultPosition, wxDefaultSize,PRJDEF_LAYERS_TYPE_NUMBER, PRJDEF_LAYERS_TYPE_STRING, 0 );
    m_DlgPDL_Layer_Type->SetSelection(LAYER_LINE);
    itemFlexGridSizer4->Add(m_DlgPDL_Layer_Type, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
   
	wxNotebook* itemNotebook9 = new wxNotebook( itemDialog1, ID_NOTEBOOK, wxDefaultPosition, wxDefaultSize, wxBK_DEFAULT );
	
	if (m_bIsModeEditing == FALSE)
	{
		m_DlgPDL_Panel_Obj = new wxPanel( itemNotebook9, ID_DLGPDL_PANEL_OBJ, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
		wxStaticBox* itemStaticBoxSizer11Static = new wxStaticBox(m_DlgPDL_Panel_Obj, wxID_ANY, _("Object Kind"));
		wxStaticBoxSizer* itemStaticBoxSizer11 = new wxStaticBoxSizer(itemStaticBoxSizer11Static, wxVERTICAL);
		m_DlgPDL_Panel_Obj->SetSizer(itemStaticBoxSizer11);
		
		m_DlgPDL_Object_List = new ProjectDefLayersObjectList( m_DlgPDL_Panel_Obj, ID_DLGPDL_OBJ_LIST, wxSize(100, 200) );
		itemStaticBoxSizer11->Add(m_DlgPDL_Object_List, 1, wxGROW|wxALL, 5);
		
		wxBoxSizer* itemBoxSizer13 = new wxBoxSizer(wxHORIZONTAL);
		itemStaticBoxSizer11->Add(itemBoxSizer13, 0, wxALIGN_LEFT|wxALL, 5);
		wxFlatButton* itemFlatButton14 = new wxFlatButton( m_DlgPDL_Panel_Obj, ID_DLGPDL_OBJECT_ADD, wxFLATBUTTON_TEXT_ADD);
		itemBoxSizer13->Add(itemFlatButton14, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxTOP|wxBOTTOM, 5);
		
		wxFlatButton* itemFlatButton15 = new wxFlatButton( m_DlgPDL_Panel_Obj, ID_DLGPDL_OBJECT_REMOVE, wxFLATBUTTON_TEXT_REMOVE);
		itemBoxSizer13->Add(itemFlatButton15, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxTOP|wxBOTTOM, 5);
		
		wxFlatButton* itemFlatButton16 = new wxFlatButton( m_DlgPDL_Panel_Obj, ID_DLGPDL_OBJECT_IMPORT, _("Import"), wxDefaultSize);
		itemBoxSizer13->Add(itemFlatButton16, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxTOP|wxBOTTOM, 5);
		
		itemNotebook9->AddPage(m_DlgPDL_Panel_Obj, _("Object Kind"));
	}

    m_DlgPDL_Panel_Fields = new wxPanel( itemNotebook9, ID_DLGPDL_PANEL_FIELDS, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxStaticBox* itemStaticBoxSizer18Static = new wxStaticBox(m_DlgPDL_Panel_Fields, wxID_ANY, _("Object Attribute"));
    wxStaticBoxSizer* itemStaticBoxSizer18 = new wxStaticBoxSizer(itemStaticBoxSizer18Static, wxVERTICAL);
    m_DlgPDL_Panel_Fields->SetSizer(itemStaticBoxSizer18);

    m_DlgPDL_Fields_List = new ProjectDefLayersFieldsList( m_DlgPDL_Panel_Fields, 
														  ID_DLGPDL_FIELDS_LIST, wxSize(100, 100),
														  this);
    itemStaticBoxSizer18->Add(m_DlgPDL_Fields_List, 1, wxGROW|wxALL, 5);
	
	
    wxBoxSizer* itemBoxSizer20 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer18->Add(itemBoxSizer20, 0, wxALIGN_LEFT|wxALL, 5);
    wxFlatButton* itemFlatButton21 = new wxFlatButton( m_DlgPDL_Panel_Fields, ID_DLGPDL_FIELD_ADD, wxFLATBUTTON_TEXT_ADD);
    itemBoxSizer20->Add(itemFlatButton21, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxTOP|wxBOTTOM, 5);

    wxFlatButton* itemFlatButton22 = new wxFlatButton( m_DlgPDL_Panel_Fields, ID_DLGPDL_FIELD_REMOVE, wxFLATBUTTON_TEXT_REMOVE);
    itemBoxSizer20->Add(itemFlatButton22, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxTOP|wxBOTTOM, 5);
	
	m_DlgPDL_Orientation_FLD = new wxCheckBox(m_DlgPDL_Panel_Fields,ID_DLGPDL_CHK_ORIENTATION,
										 _("Orientation field"));
	itemBoxSizer20->Add(m_DlgPDL_Orientation_FLD,0,wxALIGN_CENTER_VERTICAL | wxALL,5);
	
    itemNotebook9->AddPage(m_DlgPDL_Panel_Fields, _("Object Attribute"));

    itemBoxSizer2->Add(itemNotebook9, 1, wxGROW|wxALL, 5);
	
	// textctrl for polygon border name
	if (m_bIsModeEditing == false)
	{
		m_DlgPDL_Contour_Static = new wxStaticBoxSizer( new wxStaticBox( itemDialog1,wxID_ANY, _("Polygon contour value") ),
													   wxVERTICAL );
		
		m_DlgPDL_Contour_Name = new wxTextCtrl( itemDialog1, wxID_ANY, wxEmptyString,
											   wxDefaultPosition, wxDefaultSize, 0 );
		m_DlgPDL_Contour_Static->Add( m_DlgPDL_Contour_Name, 0, wxEXPAND|wxRIGHT|wxLEFT, 5 );
		itemBoxSizer2->Add(m_DlgPDL_Contour_Static, 0, wxGROW|wxALL, 5);
		m_DlgPDL_Contour_Name->Show(false);
		m_DlgPDL_Contour_Static->Show(false);
	}
	
    wxStdDialogButtonSizer* itemStdDialogButtonSizer23 = new wxStdDialogButtonSizer;

    itemBoxSizer2->Add(itemStdDialogButtonSizer23, 0, wxALIGN_RIGHT|wxALL, 5);
    wxButton* itemButton24 = new wxButton( itemDialog1, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    itemButton24->SetDefault();
	itemStdDialogButtonSizer23->AddButton(itemButton24);

    wxButton* itemButton25 = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer23->AddButton(itemButton25);

    itemStdDialogButtonSizer23->Realize();
	
	// disable the list only if in edding mode but not in adding mode
	// user want to select the type of layers :-)
	if (m_bIsModeEditing && !m_bIsModeAddingEditing)
		DisableControlsForEdition();

}


