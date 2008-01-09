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
//BEGIN_EVENT_TABLE (ProjectDefLayersObjectList, ListGenReport)
//	EVT_
//END_EVENT_TABLE()

void ProjectDefLayersObjectList::OnDoubleClickItem(wxListEvent & event)
{
//	wxArrayString myRowData;
//	long myIndex = GetSelectedItem();
//	ProjectDefLayersEditObjectDlg * myModifiyDialog = new ProjectDefLayersEditObjectDlg(this);
//
//	// get the data from the list for selected line
//	GetAllDataAsStringArray(myRowData, myIndex);
//	
//	// put the data in the dialog
//	if (DataToList(myModifiyDialog,myRowData))
//	{
//		EditDataToList(myRowData, myIndex);
//	}
//	delete myModifiyDialog;
	EditItem();
}

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
	
	// create an array for storing theme objects
	m_ObjectsArray = new PrjMemObjectsArray();
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
	wxLogDebug(_T("Creating Object description Dialog"));
	SetDialog(myEditObjDlg);
	
	// create a new item for the array if we cancel the dialog
	// this item will be destroyed, otherwise it will be attached
	// to the array
	m_ObjectObj = new ProjectDefMemoryObjects();
	
	// now uses Transfert data process
	((ProjectDefLayersEditObjectDlg*)m_pDialog)->SetMemoryObjectObject(m_ObjectObj);
	
}


void ProjectDefLayersObjectList::AfterAdding (bool bRealyAddItem)
{
	wxArrayString myListValues;
	
	if (bRealyAddItem)
	{
		// add item to the array
		m_ObjectsArray->Add(m_ObjectObj);
		wxLogDebug(_T("Size of the Object array : %d"), m_ObjectsArray->GetCount());
		
		// add item to the list
		myListValues.Add(wxString::Format(_T("%d"), m_ObjectObj->m_ObjectCode));
		myListValues.Add(m_ObjectObj->m_ObjectName);
		EditDataToList(myListValues);
		
	}
	else
		delete m_ObjectObj; // not used
	
	// delete the dialog
	wxLogDebug(_T("Deleting Object description Dialog"));
	delete m_pDialog;
	
}


void ProjectDefLayersObjectList::BeforeDeleting ()
{
	// remove item from array before removing it from the list
	// because of the unknown position of item (may have been moved)
	// if a corresponding item was found, remove it from the array
	int iItemIndex = FindObjInObjectArray(this, m_ObjectsArray);
	if ( iItemIndex != -1)
	{
		m_ObjectsArray->RemoveAt(iItemIndex);
	}	
}


void ProjectDefLayersObjectList::BeforeEditing ()
{
	// create the dialog, will be deleted in AfterEditing.
	ProjectDefLayersEditObjectDlg * myObjectDlg = new ProjectDefLayersEditObjectDlg(this);
	wxLogDebug(_T("Creating Object description Dialog"));
	SetDialog(myObjectDlg);
	
	// find item selected and then call a new Dialog
	// for editing the existing Field
	int iItemIndex = FindObjInObjectArray(this, m_ObjectsArray);
	if (iItemIndex != -1)
	{
		// get the object from the array
		m_ObjectObj = &(m_ObjectsArray->Item(iItemIndex));
		
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
		// modify data to the array is automatically done using DataTransfert
		wxLogDebug(_T("Size of the object array : %d"), m_ObjectsArray->GetCount());
		
		// modify data to the list
		// prepare data for list representation
		myListValues.Add(wxString::Format(_T("%d"), m_ObjectObj->m_ObjectCode));
		myListValues.Add(m_ObjectObj->m_ObjectName);
		EditDataToList(myListValues, GetSelectedItem());
		
	}
	
	// delete dialog
    wxLogDebug(_T("Deleting Object description Dialog"));
	delete m_pDialog;

}


void ProjectDefLayersObjectList::AddingValueToArray (wxArrayString & myImportedValues)
{
	// called during import process after each import
	// myImportedValues is modified and return a pointer
	// to a string array containing the stuff to put in
	// our array
	ProjectDefMemoryObjects * myObjectObj = new ProjectDefMemoryObjects();
	myImportedValues.Item(0).ToLong(&(myObjectObj->m_ObjectCode));
	myObjectObj->m_ObjectName = myImportedValues.Item(1);
	
	// add to array
	m_ObjectsArray->Add(myObjectObj);
	wxLogDebug(_T("%s, added to array size is : %d"), 
			   myObjectObj->m_ObjectName.c_str(), m_ObjectsArray->GetCount());
	
}


/******************************  Layers List *************************/
ProjectDefLayersFieldsList::ProjectDefLayersFieldsList(wxWindow * parent, wxWindowID  id, wxSize size, ProjectDefLayersDlg * myDlg) 
	: ListGenReport(parent,id,size)
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
	
}

ProjectDefLayersFieldsList::~ProjectDefLayersFieldsList()
{
	m_DlgParent = NULL;
}


void ProjectDefLayersFieldsList::OnPressBackSpace (wxListEvent & event)
{
	// delete selected item but also in the array
	m_DlgParent->RemoveObjFromArray();
	DeleteSelectedItem();
}


void ProjectDefLayersFieldsList::OnDoubleClickItem(wxListEvent & event)
{
	wxArrayString myListValues;
	
	// find item selected and then call a new ProjectDefFieldDlg
	// for editing the existing Field
	int iItemIndex = FindObjInFieldArray(this, m_DlgParent->m_FieldArray);
	if (iItemIndex != -1)
	{
		ProjectDefFieldDlg * myTempFieldDialog = new ProjectDefFieldDlg (this);
		
		// transfert the data obj to the dialog, data will be 
		// filled during DataTransfer...
		ProjectDefMemoryFields * myMemFieldValue = &(m_DlgParent->m_FieldArray.Item(iItemIndex));
		myTempFieldDialog->SetMemoryFieldObject(myMemFieldValue);
		
		if (myTempFieldDialog->ShowModal() == wxID_OK)
		{
			
			// prepare data for list representation
			myListValues.Add(myMemFieldValue->m_Fieldname);
			myListValues.Add(PRJDEF_FIELD_TYPE_STRING[myMemFieldValue->m_FieldType]);
			EditDataToList(myListValues, GetSelectedItem());
			
		}
		
		
		wxLogDebug(_T("Size of Field array %d"), m_DlgParent->m_FieldArray.GetCount());
		delete myTempFieldDialog;
	}	
	
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

ProjectDefLayersEditObjectDlg::ProjectDefLayersEditObjectDlg( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
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
	
	//m_ObjectObj = NULL;
	
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
	m_DlgEO_Code->GetValue().ToLong(&(m_ObjectObj->m_ObjectCode));
	m_ObjectObj->m_ObjectName = m_DlgEO_Value->GetValue();
	return TRUE;
}

bool ProjectDefLayersEditObjectDlg::TransferDataToWindow()
{
	// check that the numeric value is not null (-9999)
	if (m_ObjectObj->m_ObjectCode != NULL_LONG_VALUE)
	{
		m_DlgEO_Code->SetValue(wxString::Format(_T("%d"), m_ObjectObj->m_ObjectCode));
	}
	m_DlgEO_Value->SetValue(m_ObjectObj->m_ObjectName);
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
	// create a new object for storing fields value
	ProjectDefMemoryFields * myMemFieldValue = new ProjectDefMemoryFields();
	wxArrayString myListValues;

	m_FieldDialog = new ProjectDefFieldDlg (this);
	
	// transfert the data obj to the dialog, data will be 
	// filled during DataTransfer...
	m_FieldDialog->SetMemoryFieldObject(myMemFieldValue);
	
	if (m_FieldDialog->ShowModal() == wxID_OK)
	{
		
		// retrive data from the dialog and then strore
		// this object to the list.
		m_FieldArray.Add(myMemFieldValue);
		
		// prepare data for list representation
		myListValues.Add(myMemFieldValue->m_Fieldname);
		myListValues.Add(PRJDEF_FIELD_TYPE_STRING[myMemFieldValue->m_FieldType]);
		m_DlgPDL_Fields_List->EditDataToList(myListValues);
		
	}
	else 
	{
		delete myMemFieldValue;
		wxLogDebug(_T("Deleting Field Memory object not used"));
	}

	wxLogDebug(_T("Size of Field array %d"), m_FieldArray.GetCount());
	delete m_FieldDialog;
	
}




void ProjectDefLayersDlg::OnRemoveField (wxCommandEvent & event)
{
	// remove the object from the field array
	// if found and then remove the object from the list
	RemoveObjFromArray();
	m_DlgPDL_Fields_List->DeleteSelectedItem();
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




void ProjectDefLayersDlg::RemoveObjFromArray()
{
	// if a corresponding item was found, remove it from the array
	int iItemIndex = FindObjInFieldArray(m_DlgPDL_Fields_List, m_FieldArray);
	if ( iItemIndex != -1)
	{
		m_FieldArray.RemoveAt(iItemIndex);
	}
}


bool ProjectDefLayersDlg::TransferDataFromWindow()
{
	// function automaticaly called when the dialog
	// is closed using the wxID_OK button
	wxASSERT_MSG(m_LayersObj, wxT("Init m_LayersObj First, not initialised."));
	m_LayersObj->m_LayerName = m_DlgPDL_Layer_Name->GetValue();
	m_LayersObj->m_LayerType = (PRJDEF_LAYERS_TYPE) m_DlgPDL_Layer_Type->GetSelection();
	//m_LayersObj->m_pLayerObjectArray = m_DlgPDL_Object_List->m_ObjectsArray;
	return TRUE;
	
}


bool ProjectDefLayersDlg::TransferDataToWindow()
{
	// function automaticaly called when the dialog
	// is showed 
	wxASSERT_MSG(m_LayersObj, wxT("Init m_LayersObj First, not initialised."));
	m_DlgPDL_Layer_Name->SetValue(m_LayersObj->m_LayerName); 
	m_DlgPDL_Layer_Type->SetSelection((PRJDEF_LAYERS_TYPE) m_LayersObj->m_LayerType);
	
	// if object array isn't null then we fill the list
	if (m_LayersObj->m_pLayerObjectArray)
	{
		wxLogDebug(_T("number of item passed : %d"), m_LayersObj->m_pLayerObjectArray->GetCount());
	}
	
	return TRUE;
}


ProjectDefLayersDlg::ProjectDefLayersDlg()
{
    Init();
}

ProjectDefLayersDlg::ProjectDefLayersDlg( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, caption, pos, size, style);
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

    wxStaticText* itemStaticText5 = new wxStaticText( itemDialog1, wxID_STATIC, _("Layer type :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer4->Add(itemStaticText5, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);


    m_DlgPDL_Layer_Type = new wxChoice( itemDialog1, ID_DLGPDL_LAYER_TYPE, wxDefaultPosition, wxDefaultSize,PRJDEF_LAYERS_TYPE_NUMBER, PRJDEF_LAYERS_TYPE_STRING, 0 );
    m_DlgPDL_Layer_Type->SetStringSelection(PRJDEF_LAYERS_TYPE_STRING[LAYER_POINT]);
    itemFlexGridSizer4->Add(m_DlgPDL_Layer_Type, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText7 = new wxStaticText( itemDialog1, wxID_STATIC, _("Layer name :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer4->Add(itemStaticText7, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_DlgPDL_Layer_Name = new wxTextCtrl( itemDialog1, ID_DLGPDL_LAYER_NAME, _T(""), wxDefaultPosition, wxSize(200, -1), 0 );
    itemFlexGridSizer4->Add(m_DlgPDL_Layer_Name, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxNotebook* itemNotebook9 = new wxNotebook( itemDialog1, ID_NOTEBOOK, wxDefaultPosition, wxDefaultSize, wxBK_DEFAULT );

    m_DlgPDL_Panel_Obj = new wxPanel( itemNotebook9, ID_DLGPDL_PANEL_OBJ, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxStaticBox* itemStaticBoxSizer11Static = new wxStaticBox(m_DlgPDL_Panel_Obj, wxID_ANY, _("Object description"));
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

    m_DlgPDL_Panel_Fields = new wxPanel( itemNotebook9, ID_DLGPDL_PANEL_FIELDS, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxStaticBox* itemStaticBoxSizer18Static = new wxStaticBox(m_DlgPDL_Panel_Fields, wxID_ANY, _("Optional layer fileds"));
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

    itemNotebook9->AddPage(m_DlgPDL_Panel_Fields, _("Fields"));

    itemBoxSizer2->Add(itemNotebook9, 1, wxGROW|wxALL, 5);

    wxStdDialogButtonSizer* itemStdDialogButtonSizer23 = new wxStdDialogButtonSizer;

    itemBoxSizer2->Add(itemStdDialogButtonSizer23, 0, wxALIGN_RIGHT|wxALL, 5);
    wxButton* itemButton24 = new wxButton( itemDialog1, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    itemButton24->SetDefault();
	itemStdDialogButtonSizer23->AddButton(itemButton24);

    wxButton* itemButton25 = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer23->AddButton(itemButton25);

    itemStdDialogButtonSizer23->Realize();}
