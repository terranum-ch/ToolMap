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
		m_DLGODD_FrequencyStrings.Add(_("&Frequent"));
		m_DLGODD_FrequencyStrings.Add(_("&Not Frequent"));
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
	return TRUE;
}




/******************************** LIST OF OBJECT DEFINITION  ************************/
ObjectDefinitionList::ObjectDefinitionList(wxWindow * parent,
										   wxWindowID id,
										   PRJDEF_LAYERS_TYPE paneltype,
										   DataBaseTM * database,
										   wxArrayString * pColsName, 
										   wxArrayInt * pColsSize,
										   wxSize size) : ListGenReportWithDialog(parent, id, pColsName, pColsSize, size)
{
	m_CheckBox = NULL;
	m_layertype = paneltype;
	m_DBHandler = database;
	
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
	
	if(m_DBHandler->GetObjectListByLayerType(ilayertype))
	{
		// loop for all results 
		while (1)
		{
			myResults = m_DBHandler->DataBaseGetNextResult();
			if (myResults.GetCount() > 0)
			{
				// if ilayertype is LINE we
				// have to modify the bool
				// frequency into a text
				if (ilayertype == LAYER_LINE)
				{
					myResults.Item(3).ToLong(&lFrequency);
					if (lFrequency == 0) // not frequent
					{
						myResults.Item(3) = PRJDEF_OBJECTS_FREQ_STRING[OBJECT_LESS_FREQUENT];
					}
					else 
					{
						myResults.Item(3) = PRJDEF_OBJECTS_FREQ_STRING[OBJECT_FREQUENT];
					}
				}
				
				//put the results in the list
				EditDataToList(myResults);
				myResults.Clear();
			}
			else 
			{
				break;
			}
			
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


void ObjectDefinitionList::SetFreqStatus (int frequency, wxArrayLong * iIndexes)
{
	for (unsigned int i = 0; i<iIndexes->GetCount();i++)
	{
		SetItemText(iIndexes->Item(i), 3, PRJDEF_OBJECTS_FREQ_STRING[frequency]);
	}
	
}


/***************************************************************************//**
 @brief Pass data to the list
 @details This function is inspired from the ListGenReport::EditDataToList()
 one, but in this case we are storing the OBJECT_ID into the item (not
 visible). This way we can update or delete the good item
 @param myValue a wxArrayString containing all columns item plus the number to
 associate with the item
 @param index index item to modify or -1 for adding a new item
 @param bool return TRUE if the number of items in the array is correct
 @author Lucien Schreiber (c) CREALP 2007
 @date 13 March 2008
 *******************************************************************************/
bool ObjectDefinitionList::EditDataToList (const wxArrayString & myValue, int index)
{
	// check that data are stored in the array to fill the list.
	// the last item will be used for adding not visible data (OBJECT_ID) to the list 

	int iArrayItemCount = myValue.GetCount();
	int iRunNb = 0;
	//iArrayItemCount > GetColumnCount() ? iRunNb = GetColumnCount() : iRunNb = iArrayItemCount;
	
	
	if (iArrayItemCount - 1 == GetColumnCount())
	{
		// add the first line in the list if index is = -1
		if (index == -1)
		{
			AddItemToList(myValue.Item(0));
			index = GetItemCount()-1;
		}
		else
			SetItemText(index, 0, myValue.Item(0));
		
		for (int i=1; i<GetColumnCount(); i++)
		{
			SetItemText(index,i, myValue.Item(i));
		}
		// associate data to the item
		long myData = 0;
		myValue.Item(GetColumnCount()).ToLong(&myData);
		SetItemData(index,myData);
		
		return TRUE;
	}
	wxLogDebug(_T("Error using EditdatatoList function, not enrough item in array"));
	return FALSE;
	
	
}



void ObjectDefinitionList::BeforeAdding()
{
	// create the dialog
	ObjectDefinitionListDlg * myDlg = new ObjectDefinitionListDlg(this, m_layertype, m_DBHandler);
	SetDialog(myDlg);
}



void ObjectDefinitionList::AfterAdding (bool bRealyAddItem)
{
	
	delete m_pDialog;
}



void ObjectDefinitionList::BeforeEditing ()
{
	// create the dialog
	ObjectDefinitionListDlg * myDlg = new ObjectDefinitionListDlg(this, m_layertype, m_DBHandler);
	SetDialog(myDlg);
	
}

void ObjectDefinitionList::AfterEditing (bool bRealyEdited)
{
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
			wxLogDebug(_T("Multiple Selected items = %d"), event.GetIndex());
		}
		else
		{
			m_CheckBox->Set3StateValue(GetFreqStatus(event.GetIndex()));
			wxLogDebug(_T("Single Selected items = %d"), event.GetIndex());
		}
	}
}



