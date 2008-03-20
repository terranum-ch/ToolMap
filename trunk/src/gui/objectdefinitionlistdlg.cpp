/***************************************************************************
						objectdefinitionlistdlg.cpp
	Contain description of class used for ProjectEditObjectDefinitionDLG
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

#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "objectdefinitionlistdlg.h"


/******************************** EDIT OBJECT DEFINITION DLG ************************/
IMPLEMENT_DYNAMIC_CLASS( ObjectDefinitionListDlg, wxDialog )


BEGIN_EVENT_TABLE( ObjectDefinitionListDlg, wxDialog )
END_EVENT_TABLE()


ObjectDefinitionListDlg::ObjectDefinitionListDlg()
{
    Init();
}


ObjectDefinitionListDlg::ObjectDefinitionListDlg( wxWindow* parent,
												 int iListlayerType,
												 DataBaseTM * DBHandler,
												 wxWindowID id, const wxString& caption,
												 const wxPoint& pos, const wxSize& size, long style )
{
    Init();
	// should we use the frequency control
	m_ParentListType = iListlayerType;
	m_pDatabase = DBHandler;
	
    Create(parent, id, caption, pos, size, style);
	
	
	
	
}


bool ObjectDefinitionListDlg::Create( wxWindow* parent,
									 wxWindowID id, const wxString& caption, 
									 const wxPoint& pos, const wxSize& size, long style )
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


ObjectDefinitionListDlg::~ObjectDefinitionListDlg()
{
}


void ObjectDefinitionListDlg::Init()
{
    m_DLGODD_Code = NULL;
    m_DLGODD_Description = NULL;
    m_DLGODD_List_Lyr_Name = NULL;
    m_DLGODD_Frequency = NULL;
}


void ObjectDefinitionListDlg::CreateControls()
{    
    ObjectDefinitionListDlg* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxStaticBox* itemStaticBoxSizer3Static = new wxStaticBox(itemDialog1, wxID_ANY, _("Object properties"));
    wxStaticBoxSizer* itemStaticBoxSizer3 = new wxStaticBoxSizer(itemStaticBoxSizer3Static, wxVERTICAL);
    itemBoxSizer2->Add(itemStaticBoxSizer3, 0, wxGROW|wxALL, 5);

    wxFlexGridSizer* itemFlexGridSizer4 = new wxFlexGridSizer(3, 2, 0, 0);
    itemFlexGridSizer4->AddGrowableCol(1);
    itemStaticBoxSizer3->Add(itemFlexGridSizer4, 0, wxGROW, 5);

    wxStaticText* itemStaticText5 = new wxStaticText( itemDialog1, wxID_STATIC, _("Code :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer4->Add(itemStaticText5, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_DLGODD_Code = new wxTextCtrl( itemDialog1, wxID_ANY, _T(""), wxDefaultPosition, wxSize(250, -1), 0 );
    itemFlexGridSizer4->Add(m_DLGODD_Code, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText7 = new wxStaticText( itemDialog1, wxID_STATIC, _("Description :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer4->Add(itemStaticText7, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_DLGODD_Description = new wxTextCtrl( itemDialog1, wxID_ANY, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    m_DLGODD_Description->SetMaxLength(255);
    itemFlexGridSizer4->Add(m_DLGODD_Description, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText9 = new wxStaticText( itemDialog1, wxID_STATIC, _("Layer name :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer4->Add(itemStaticText9, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxArrayString m_DLGODD_List_Lyr_NameStrings;
    m_DLGODD_List_Lyr_Name = new wxChoice( itemDialog1, ID_DLGODD_LIST_LYR_NAME, wxDefaultPosition, wxDefaultSize, m_DLGODD_List_Lyr_NameStrings, 0 );
    itemFlexGridSizer4->Add(m_DLGODD_List_Lyr_Name, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);

	if (m_ParentListType == LAYER_LINE)
	{
		wxArrayString m_DLGODD_FrequencyStrings;
		m_DLGODD_FrequencyStrings.Add(_("&Not Frequent"));
		m_DLGODD_FrequencyStrings.Add(_("&Frequent"));
		m_DLGODD_Frequency = new wxRadioBox( itemDialog1, ID_DLGODD_FREQUENCY, _("Frequency"), wxDefaultPosition, wxDefaultSize, m_DLGODD_FrequencyStrings, 1, wxRA_SPECIFY_ROWS );
		m_DLGODD_Frequency->SetSelection(0);
		itemStaticBoxSizer3->Add(m_DLGODD_Frequency, 0, wxGROW|wxALL, 5);
		
		itemBoxSizer2->Add(5, 5, 1, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	}

    wxStaticLine* itemStaticLine13 = new wxStaticLine( itemDialog1, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
    itemBoxSizer2->Add(itemStaticLine13, 0, wxGROW|wxALL, 5);

    wxStdDialogButtonSizer* itemStdDialogButtonSizer14 = new wxStdDialogButtonSizer;

    itemBoxSizer2->Add(itemStdDialogButtonSizer14, 0, wxALIGN_RIGHT|wxALL, 5);
    wxButton* itemButton15 = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer14->AddButton(itemButton15);

    wxButton* itemButton16 = new wxButton( itemDialog1, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    itemButton16->SetDefault();
    itemStdDialogButtonSizer14->AddButton(itemButton16);

    itemStdDialogButtonSizer14->Realize();

    // Set validators
    m_DLGODD_Description->SetValidator( wxTextValidator(wxFILTER_NONE, & m_Description) );
}



bool ObjectDefinitionListDlg::TransferDataToWindow()
{
	// fill choice with themes
	m_DLGODD_List_Lyr_Name->Append(m_pDatabase->GetLayerNameByType(m_ParentListType));
	
	if (m_ObjectObj->m_ObjectCode != NULL_LONG_VALUE)
		m_DLGODD_Code->SetValue(wxString::Format(_T("%d"),m_ObjectObj->m_ObjectCode));
	
	m_DLGODD_Description->SetValue(m_ObjectObj->m_ObjectName);
	
	// if we use the frequency control
	if (m_ParentListType == LAYER_LINE)
		m_DLGODD_Frequency->SetSelection(m_ObjectObj->m_ObjectFreq);
	
	// set parent layer
	if (!m_ObjectObj->m_ParentLayerName.IsEmpty())
		m_DLGODD_List_Lyr_Name->SetStringSelection(m_ObjectObj->m_ParentLayerName);
	
	return TRUE;
}



bool ObjectDefinitionListDlg::TransferDataFromWindow()
{
	// fill ProjectDefMemoryObject with list values
	(m_DLGODD_Code->GetValue()).ToLong (&(m_ObjectObj->m_ObjectCode));
	m_ObjectObj->m_ObjectName = m_DLGODD_Description->GetValue();
	m_ObjectObj->m_ParentLayerName = m_DLGODD_List_Lyr_Name->GetStringSelection();
	// get the frequency item (FREQUENT must be 0)
	if (m_ParentListType == LAYER_LINE)
		m_ObjectObj->m_ObjectFreq = (PRJDEF_OBJECTS_FREQ) m_DLGODD_Frequency->GetSelection();
	return TRUE;
}




/******************************** LIST OF OBJECT DEFINITION  ************************/
int ObjectDefinitionList::iAddItems = -1;

ObjectDefinitionList::ObjectDefinitionList(wxWindow * parent,
										   wxWindowID id,
										   PrjDefMemManage * memory,
										   PRJDEF_LAYERS_TYPE paneltype,
										   DataBaseTM * database,
										   wxArrayString * pColsName, 
										   wxArrayInt * pColsSize,
										   wxSize size) : ListGenReportWithDialog(parent, id, pColsName, pColsSize, size)
{
	m_CheckBox = NULL;
	m_ChoiceLayer = NULL;
	m_layertype = paneltype;
	m_DBHandler = database;
	m_MemoryObject = memory;
	
	
	// init list with database values
	SetListText(m_layertype);
}



ObjectDefinitionList::~ObjectDefinitionList()
{
	
}


BEGIN_EVENT_TABLE( ObjectDefinitionList, ListGenReportWithDialog)
	EVT_LIST_ITEM_SELECTED(wxID_ANY, ObjectDefinitionList::OnItemSelectChange)
END_EVENT_TABLE()



bool ObjectDefinitionList::SetListText (int ilayertype)
{
	wxArrayString myResults;
	long lFrequency = 0;
	ProjectDefMemoryObjects myTempObject;
	
	if(m_DBHandler->GetObjectListByLayerType(ilayertype))
	{
		// loop for all results 
		while (1)
		{
			if (m_DBHandler->DataBaseGetNextResultAsObject(&myTempObject, ilayertype))
				SetObjectToList(&myTempObject);
			else
				break;
		}
		return TRUE;
		
	}
	
	return FALSE;
}


/***************************************************************************//**
 @brief Retrive the frequency status of a selected item
 @details This function returns a wxCheckBoxState item indicating if the iIndex
 item is Frequent (wxCHK_CHECKED) or less frequent (wxCHK_UNCHECKED). If iIndex
 is -1 (the default) then this function returns the frequency for all selected
 items
 @param int The index value for the item we want to interogate. If -1 this
 function returns the checked state for all selected items
 @return  indicate if (iIndex) should be selected / unselected or undefined
 @author Lucien Schreiber (c) CREALP 2007
 @date 17 March 2008
 *******************************************************************************/
wxCheckBoxState ObjectDefinitionList::GetFreqStatus (int iIndex)
{
	wxString myValue = _T("");
	wxArrayLong myAllIndex;
	long iNbOfSelectedItems = 0;
	wxArrayString myAllValues;
	int i = 0, iCompteFreq = 0;
	
	
	// retrun value based on the frequency of selected items
	// if index == -1 return value for all selected items
	if (iIndex == -1)
	{
		// get all selected items
		iNbOfSelectedItems = GetAllSelectedItem(myAllIndex);
		for (i = 0; i<iNbOfSelectedItems; i++)
		{
			myAllValues.Add(GetItemColText(myAllIndex[i], 3));
		}
		
		// compare text for selected items
		for (i=0;i<iNbOfSelectedItems;i++)
		{
			if ( (myAllValues[i]).IsSameAs(PRJDEF_OBJECTS_FREQ_STRING[OBJECT_FREQUENT]))
				iCompteFreq++;
		}
		
		// return values based on selected texts
		if (iCompteFreq == iNbOfSelectedItems)
		{
			return wxCHK_CHECKED;
		}
		else if (iCompteFreq == 0)
		{
			return wxCHK_UNCHECKED;
		}
		else
		{
			return wxCHK_UNDETERMINED;
		}

	}
	else // return freq state for iIndex item
	{
		myValue = GetItemColText(iIndex, 3);
		if (myValue == PRJDEF_OBJECTS_FREQ_STRING[OBJECT_FREQUENT])
		{
			return wxCHK_CHECKED;
		}
	}
	
	return wxCHK_UNCHECKED;
}


// iIndex not used but here for compatibility with GetFreqStatus()
wxString ObjectDefinitionList::GetLayerStatus (int iIndex)
{
	// we get the values for the first selected 
	int iFirstSelected = GetSelectedItem();
	return GetItemColText(iFirstSelected, 2);
}



void ObjectDefinitionList::SetListCtrls (wxChoice * layerchoice ,wxCheckBox * checkboxfreq)
{
	m_CheckBox = checkboxfreq;
	m_ChoiceLayer = layerchoice;
}




/***************************************************************************//**
 @brief Update layer freqency in response to event
 @details May be used to change the layer frequency text from one or more item
 in the list
 @param frequency the new frequency to change may be OBJECT_FREQUENT or
 OBJECT_LESS_FREQUENT
 @param iIndexes an array of long containing index of items to be changed
 @author Lucien Schreiber (c) CREALP 2007
 @date 17 March 2008
 *******************************************************************************/
void ObjectDefinitionList::SetFreqStatus (int frequency, wxArrayLong * iIndexes)
{
	wxString myObjectName;
	
	for (unsigned int i = 0; i<iIndexes->GetCount();i++)
	{
		// update UI
		SetItemText(iIndexes->Item(i), 3, PRJDEF_OBJECTS_FREQ_STRING[frequency]);
		
		// update memory objects, we first search if items exists in the objects to
		// modify
		myObjectName = GetItemColText(iIndexes->Item(i), 1);
		m_ObjectObj = m_MemoryObject->FindObject(myObjectName);
		
		// if object not found in the array, we create a new object
		if (m_ObjectObj == NULL)
		{
			m_ObjectObj = m_MemoryObject->AddObject();
			
			// load object with list data
			GetObjectFromList(m_ObjectObj, iIndexes->Item(i));
		}
		
		// finally change the frequency to the specified value
		m_ObjectObj->m_ObjectFreq = (PRJDEF_OBJECTS_FREQ) frequency;
		
		
	}
	
}


/***************************************************************************//**
 @brief Update layer name in response to event
 @details May be used to change the layer name text from one or more item in the
 list
 @param layer The new name for the layer name
 @param iIndexes an array of long containing index of items to be changed
 (should not be null)
 @author Lucien Schreiber (c) CREALP 2007
 @date 17 March 2008
 *******************************************************************************/
void ObjectDefinitionList::SetLayerStatus (const wxString & layer, wxArrayLong * iIndexes)
{
	for (unsigned int i = 0; i<iIndexes->GetCount(); i++)
	{
		SetItemText(iIndexes->Item(i), 2, layer);
	}
}




/***************************************************************************//**
 @brief Update list values with #ProjectDefMemoryObjects
 @details This function update the list (or add item into) with values specified
 in the #ProjectDefMemoryObjects pointer
 @param object pointer to a #ProjectDefMemoryObjects containing values to set
 into the list
 @param iIndex the zero based index of the item to modify if iIndex is -1 (the
 default) then a new item is added
 @author Lucien Schreiber (c) CREALP 2007
 @date 20 March 2008
 *******************************************************************************/
void ObjectDefinitionList::SetObjectToList (ProjectDefMemoryObjects * object, int iIndex)
{
	// prepare default frequency 
	wxString sfrequency = PRJDEF_OBJECTS_FREQ_STRING[OBJECT_LESS_FREQUENT];
	
	
	// if we want to add an item
	if (iIndex == -1)
	{
		AddItemToList(wxString::Format(_T("%d"), object->m_ObjectCode));
		iIndex = GetItemCount()-1;
	}
	else
		SetItemText(iIndex, 0, wxString::Format(_T("%d"), object->m_ObjectCode));
	
	
	// normal operations for others columns
	SetItemText(iIndex, 1, object->m_ObjectName);
	SetItemText(iIndex, 2, object->m_ParentLayerName);
	
	// if we use the frequency
	if (m_layertype == LAYER_LINE)
	{
		if (object->m_ObjectFreq == OBJECT_FREQUENT)
			sfrequency = PRJDEF_OBJECTS_FREQ_STRING[OBJECT_FREQUENT];
		SetItemText(iIndex, 3, sfrequency);
	}

	// attach data to the item
	SetItemData(iIndex, object->m_ObjectID);
}



/***************************************************************************//**
 @brief Get list values into #ProjectDefMemoryObjects
 @details This function gets the list values into a #ProjectDefMemoryObjects
 pointer for the specified list index
 @param object pointer to a #ProjectDefMemoryObjects used for returning the
 values
 @param iIndex the zero based index of the item we want to get values
 @author Lucien Schreiber (c) CREALP 2007
 @date 20 March 2008
 *******************************************************************************/
void ObjectDefinitionList::GetObjectFromList (ProjectDefMemoryObjects * object, int iIndex)
{
	// prepare default frequency 
	wxString sfrequency = PRJDEF_OBJECTS_FREQ_STRING[OBJECT_LESS_FREQUENT];

	GetItemColText(iIndex, 0).ToLong(&(object->m_ObjectCode));
	object->m_ObjectName = GetItemColText(iIndex, 1);
	object->m_ParentLayerName = GetItemColText(iIndex, 2);
	
	// if we use the frequency control
	if (m_layertype == LAYER_LINE)
	{
		if (GetItemColText(iIndex, 3) == sfrequency)
			object->m_ObjectFreq = OBJECT_LESS_FREQUENT;
		else
			object->m_ObjectFreq = OBJECT_FREQUENT;
	}
	
	// attach data to the item
	object->m_ObjectID = GetItemData(iIndex);
}




void ObjectDefinitionList::BeforeAdding()
{
	// create the dialog
	ObjectDefinitionListDlg * myDlg = new ObjectDefinitionListDlg(this, m_layertype, m_DBHandler);
	SetDialog(myDlg);
	
	m_ObjectObj = m_MemoryObject->AddObject();
	
	// now uses Transfert data process
	((ObjectDefinitionListDlg*)m_pDialog)->SetMemoryObjectObject(m_ObjectObj);
	
}



void ObjectDefinitionList::AfterAdding (bool bRealyAddItem)
{
	wxArrayString sResultToList;
	
	
	if (bRealyAddItem == TRUE)
	{
		// add an id for the new added item
		// needed if we modify it later
		m_ObjectObj->m_ObjectID = iAddItems;
		iAddItems--;
		
		// update list ctrl
		SetObjectToList(m_ObjectObj);
	}
	else 
	{
		m_MemoryObject->RemoveObject();
	}

	
	delete m_pDialog;
}



void ObjectDefinitionList::BeforeEditing ()
{
	long iSelected = 0;
	long lTemp = 0;
	int iIndex = 0;
	wxArrayString mySelValues;
	
	// create the dialog
	ObjectDefinitionListDlg * myDlg = new ObjectDefinitionListDlg(this, m_layertype, m_DBHandler);
	SetDialog(myDlg);
	
	
	// get selected item from the list
	long mySelectedListItem = GetSelectedItem();
	wxString myObjectName = GetItemColText(mySelectedListItem, 1);
	
	// try to find if the object exist in the array
	m_ObjectObj = m_MemoryObject->FindObject(myObjectName);
	
	// if the object wasn't found we create a new object
	// for storing the modifications
	if (m_ObjectObj == NULL)
	{
		// create an empty object and fill it with actual selection values
		m_ObjectObj = m_MemoryObject->AddObject();
		
		
		// fill the Object with list values 
		GetObjectFromList(m_ObjectObj, GetSelectedItem());
	}
	
	// now uses Transfert data process
	((ObjectDefinitionListDlg*)m_pDialog)->SetMemoryObjectObject(m_ObjectObj);

	
}

void ObjectDefinitionList::AfterEditing (bool bRealyEdited)
{
	wxArrayString sResultToList;
	
	if (bRealyEdited == TRUE)
	{
		// the object was modified we show the modif in the list
		SetObjectToList(m_ObjectObj, GetSelectedItem());
	}
	else
		m_MemoryObject->RemoveObject();
	
	delete m_pDialog;
}






/**************************************** EVENT FUNCTION TO IMPLEMENT ***************************/
/***************************************************************************//**
 @brief Called when selection changes (event function)
 @details This function is called when selection changes and update the parent
 controls (checkbox and choice) for reflecting the selected items status
 @author Lucien Schreiber (c) CREALP 2007
 @date 17 March 2008
 *******************************************************************************/
void ObjectDefinitionList::OnItemSelectChange (wxListEvent & event)
{
	// if we have a checkbox for Frequency
	if (m_CheckBox != NULL)
	{
		// multiple selection ?
		if (GetSelectedItemCount() > 1)
		{
			m_CheckBox->Set3StateValue(GetFreqStatus(-1));
		}
		else
		{
			m_CheckBox->Set3StateValue(GetFreqStatus(event.GetIndex()));
		}
	}
	
	// if we have a choice layer ctrl defined
	if (m_ChoiceLayer != NULL )
	{
		m_ChoiceLayer->SetStringSelection(GetLayerStatus());
	}
	else
	{
		wxLogDebug(_T("Pointer to the wxChoice not passed to listctrl..."));	
	}
}



