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
		
		// add item to the list
		myListValues.Add(wxString::Format(_T("%d"), m_ObjectObj->m_ObjectCode));
		myListValues.Add(m_ObjectObj->m_ObjectName);
		EditDataToList(myListValues);
		
	}
	else
		m_pPrjDefinition->RemoveObject(); // remove last object not used
	
	// delete the dialog
	//wxLogDebug(_T("Deleting Object description Dialog"));
	delete m_pDialog;
	
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
	int iNbSelectedItems = GetAllSelectedItem(mySelectedListItems);
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
		myListValues.Add(wxString::Format(_T("%d"), m_ObjectObj->m_ObjectCode));
		myListValues.Add(m_ObjectObj->m_ObjectName);
		EditDataToList(myListValues, GetSelectedItem());
		
		
	}
		
	// delete dialog
    //wxLogDebug(_T("Deleting Object description Dialog"));
	delete m_pDialog;

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
	
	myColsWidths.Add(250);
	myColsWidths.Add(80);
	
	CreateColumns(&myColNames, &myColsWidths);
	
	m_ChoiceToChange = NULL;
	
	m_pPrjDefinition = NULL;
	m_FieldsObj = NULL;
	
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
	delete m_pDialog;	
}


void ProjectDefLayersFieldsList::BeforeDeleting ()
{
//	// remove item from array before removing it from the list
//	// because of the unknown position of item (may have been moved)
//	// if a corresponding item was found, remove it from the array
//	
//	// get selected item from the list
//	long mySelectedListItem = GetSelectedItem();
//	wxString myFieldName = GetItemColText(mySelectedListItem, 0);
//	
//	m_pPrjDefinition->RemoveField(myFieldName);
	
	wxString myFieldName;
	// remove item from array before removing it from the list
	// because of the unknown position of item (may have been moved)
	// if a corresponding item was found, remove it from the array
	
	// get selected items from the list
	wxArrayLong mySelectedListItems;
	int iNbSelectedItems = GetAllSelectedItem(mySelectedListItems);
	for (unsigned int i=0; i< mySelectedListItems.GetCount(); i++)
	{
		myFieldName = GetItemColText(mySelectedListItems[i], 0);		
		m_pPrjDefinition->RemoveField(myFieldName);
	}
	
	
	
	
}


void ProjectDefLayersFieldsList::BeforeEditing ()
{
	// create the dialog, will be deleted in AfterEditing.
	ProjectDefFieldDlg * myFieldDlg = new ProjectDefFieldDlg(this,m_pPrjDefinition);
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
	delete m_pDialog;
	
}





/******************************  Add object Dialog Class *************************/
IMPLEMENT_DYNAMIC_CLASS( ProjectDefLayersEditObjectDlg, wxDialog )

BEGIN_EVENT_TABLE (ProjectDefLayersEditObjectDlg, ListGenDialog)
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
	////@begin ProjectDefLayersEditObjectDlg content construction
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
	
	////@end ProjectDefLayersEditObjectDlg content construction
}


bool ProjectDefLayersEditObjectDlg::TransferDataFromWindow()
{
	// getting values
	long lCode = 0;
	m_DlgEO_Code->GetValue().ToLong(&lCode);
	wxString sValue = m_DlgEO_Value->GetValue();
	
	// passing values to the corresponding obj.
	if (m_ObjectObj != NULL)
	{
		m_ObjectObj->m_ObjectCode = lCode;
		m_ObjectObj->m_ObjectName = sValue;
	}
	
	if (m_CodedValObj != NULL)
	{
		m_CodedValObj->m_ValueCode = lCode;
		m_CodedValObj->m_ValueName = sValue;
	}
	
	//m_DlgEO_Code->GetValue().ToLong(&(m_ObjectObj->m_ObjectCode));
	//m_ObjectObj->m_ObjectName = m_DlgEO_Value->GetValue();
	
	
	return TRUE;
}

bool ProjectDefLayersEditObjectDlg::TransferDataToWindow()
{
	if (m_ObjectObj != NULL)
	{
		// check that the numeric value is not null (-9999)
		if (m_ObjectObj->m_ObjectCode != NULL_LONG_VALUE)
		{
			m_DlgEO_Code->SetValue(wxString::Format(_T("%d"), m_ObjectObj->m_ObjectCode));
		}
		m_DlgEO_Value->SetValue(m_ObjectObj->m_ObjectName);
	}
	
	
	if (m_CodedValObj != NULL)
	{
		// check that the numeric value is not null (-9999)
		if (m_CodedValObj->m_ValueCode != NULL_LONG_VALUE)
		{
			m_DlgEO_Code->SetValue(wxString::Format(_T("%d"), m_CodedValObj->m_ValueCode));
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






bool ProjectDefLayersDlg::TransferDataFromWindow()
{
	// function automaticaly called when the dialog
	// is closed using the wxID_OK button
	wxASSERT_MSG(m_LayersObj, wxT("Init m_LayersObj First, not initialised."));
	m_LayersObj->m_LayerName = m_DlgPDL_Layer_Name->GetValue();
	m_LayersObj->m_LayerType = (PRJDEF_LAYERS_TYPE) m_DlgPDL_Layer_Type->GetSelection();
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
		m_DlgPDL_Layer_Name->SetValue(m_LayersObj->m_LayerName); 
		m_DlgPDL_Layer_Type->SetSelection((PRJDEF_LAYERS_TYPE) m_LayersObj->m_LayerType);
		
		//wxLogDebug(_T("Prj def address = %p"), m_pPrjDefinition);
		
		// fill the object list
		for (int i = 0; i<m_pPrjDefinition->GetCountObject(); i++)
		{
			ProjectDefMemoryObjects * myObjectObj = m_pPrjDefinition->GetNextObjects();
			
			// fit things returned in the list
			myObjListValues.Add(wxString::Format(_T("%d"), myObjectObj->m_ObjectCode));
			myObjListValues.Add(myObjectObj->m_ObjectName);
			m_DlgPDL_Object_List->EditDataToList(myObjListValues);
			myObjListValues.Clear();
		}
		
		// fill the field list
		for (int j=0; j < m_pPrjDefinition->GetCountFields(); j++)
		{
			ProjectDefMemoryFields * myFieldObj = m_pPrjDefinition->GetNextField();
			
			// fit things returned in the list
			myFieldListValues.Add(myFieldObj->m_Fieldname);
			myFieldListValues.Add(PRJDEF_FIELD_TYPE_STRING[myFieldObj->m_FieldType]);
			m_DlgPDL_Fields_List->EditDataToList(myFieldListValues);
			myFieldListValues.Clear();

		}
		
	}
	return TRUE;
}




ProjectDefLayersDlg::ProjectDefLayersDlg()
{
    Init();
}

ProjectDefLayersDlg::ProjectDefLayersDlg( wxWindow* parent, PrjDefMemManage *pPrjDef,
										  bool isEditingMode,
										 wxWindowID id, const wxString& caption, 
										 const wxPoint& pos, const wxSize& size,
										 long style )
{
    Init();
	
	m_bIsModeEditing = isEditingMode;
	
    Create(parent, id, caption, pos, size, style);
	
	
	// init prj definition and pass it to the list
	m_pPrjDefinition = pPrjDef;
	m_DlgPDL_Object_List->PassPrjDefToList(m_pPrjDefinition);
	m_DlgPDL_Fields_List->PassPrjDefToList(m_pPrjDefinition);
	//wxLogDebug(_T("Prj def address = %p"), m_pPrjDefinition);
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
		wxStaticBox* itemStaticBoxSizer11Static = new wxStaticBox(m_DlgPDL_Panel_Obj, wxID_ANY, _("Objects description"));
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
		
		itemNotebook9->AddPage(m_DlgPDL_Panel_Obj, _("Objects"));
	}

    m_DlgPDL_Panel_Fields = new wxPanel( itemNotebook9, ID_DLGPDL_PANEL_FIELDS, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxStaticBox* itemStaticBoxSizer18Static = new wxStaticBox(m_DlgPDL_Panel_Fields, wxID_ANY, _("Optional layer attributes"));
    wxStaticBoxSizer* itemStaticBoxSizer18 = new wxStaticBoxSizer(itemStaticBoxSizer18Static, wxVERTICAL);
    m_DlgPDL_Panel_Fields->SetSizer(itemStaticBoxSizer18);

    m_DlgPDL_Fields_List = new ProjectDefLayersFieldsList( m_DlgPDL_Panel_Fields, ID_DLGPDL_FIELDS_LIST, wxSize(100, 100), this);
    itemStaticBoxSizer18->Add(m_DlgPDL_Fields_List, 1, wxGROW|wxALL, 5);

    wxBoxSizer* itemBoxSizer20 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer18->Add(itemBoxSizer20, 0, wxALIGN_LEFT|wxALL, 5);
    wxFlatButton* itemFlatButton21 = new wxFlatButton( m_DlgPDL_Panel_Fields, ID_DLGPDL_FIELD_ADD, wxFLATBUTTON_TEXT_ADD);
    itemBoxSizer20->Add(itemFlatButton21, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxTOP|wxBOTTOM, 5);

    wxFlatButton* itemFlatButton22 = new wxFlatButton( m_DlgPDL_Panel_Fields, ID_DLGPDL_FIELD_REMOVE, wxFLATBUTTON_TEXT_REMOVE);
    itemBoxSizer20->Add(itemFlatButton22, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxTOP|wxBOTTOM, 5);

    itemNotebook9->AddPage(m_DlgPDL_Panel_Fields, _("Attributes"));

    itemBoxSizer2->Add(itemNotebook9, 1, wxGROW|wxALL, 5);

    wxStdDialogButtonSizer* itemStdDialogButtonSizer23 = new wxStdDialogButtonSizer;

    itemBoxSizer2->Add(itemStdDialogButtonSizer23, 0, wxALIGN_RIGHT|wxALL, 5);
    wxButton* itemButton24 = new wxButton( itemDialog1, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    itemButton24->SetDefault();
	itemStdDialogButtonSizer23->AddButton(itemButton24);

    wxButton* itemButton25 = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer23->AddButton(itemButton25);

    itemStdDialogButtonSizer23->Realize();
	
	if (m_bIsModeEditing)
		DisableControlsForEdition();

}


