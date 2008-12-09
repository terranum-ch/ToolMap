/***************************************************************************
							attribution_obj_type.cpp
					Display wxAui panel for attribution
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


#include "attribution_obj_type.h"


DEFINE_EVENT_TYPE (tmEVT_ATTRIBUTION_BTN_PRESSED)
DEFINE_EVENT_TYPE (tmEVT_INFO_BTN_PRESSED)

BEGIN_EVENT_TABLE(AttribObjType_PANEL, ManagedAuiWnd)
	EVT_BUTTON (ID_DLG_OBJ_ATTRIBUTION_BTN_ATTRIBUTE, AttribObjType_PANEL::OnAttributeBtn)
	EVT_BUTTON (ID_DLG_OBJ_ATTRIBUTION_BTN_INFO, AttribObjType_PANEL::OnInfoBtn)
END_EVENT_TABLE()


AttribObjType_PANEL::AttribObjType_PANEL(wxWindow * parent, wxAuiManager * AuiManager) : ManagedAuiWnd(AuiManager)
{
	m_AttribBtnLabel = _("Attribute");
	m_ParentEvt = parent;
	m_ParentEvt->PushEventHandler(this);
	
	wxPanel *  ContentFrame = new wxPanel (parent, wxID_ANY);
	CreateControls(ContentFrame);
	
	
	
	// define properties of Panel.
	mPaneInfo.Name(SYMBOL_ATTRIBOBJTYPE_PANEL_TITLE);
	mPaneInfo.Caption(SYMBOL_ATTRIBOBJTYPE_PANEL_TITLE);
	mPaneInfo.Right();
	mPaneInfo.Layer(2);
	mPaneInfo.Position(2);
	mPaneInfo.MinSize(SYMBOL_ATTRIBOBJTYPE_PANEL_SIZE);
	mPaneInfo.CloseButton(FALSE);
	mPaneInfo.Hide();
	
	// pass panel name to parent class.
	m_AuiPanelName = SYMBOL_ATTRIBOBJTYPE_PANEL_TITLE;
	
	//wxLogDebug(_T("m_auiMgr (child) %p"), AuiManager);
	
	// add the panel
	AddManagedPane(ContentFrame, mPaneInfo);
}

AttribObjType_PANEL::~AttribObjType_PANEL()
{
	m_ParentEvt->PopEventHandler(FALSE);
}


wxSizer * AttribObjType_PANEL::CreateControls(wxWindow * parent, bool call_fit, bool set_sizer)
{    
    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    //itemPanel1->SetSizer(itemBoxSizer2);
	
   m_AttribNotebook = new wxChoicebook( parent, ID_NOTEBOOK2, wxDefaultPosition, wxSize(300, -1), wxBK_DEFAULT );
	
    wxPanel* itemPanel4 = new wxPanel( m_AttribNotebook, ID_PANEL4, wxDefaultPosition, wxSize(300, 300), wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer5 = new wxBoxSizer(wxVERTICAL);
    itemPanel4->SetSizer(itemBoxSizer5);
	
    wxAuiNotebook* itemNotebook6 = new wxAuiNotebook( itemPanel4, ID_NOTEBOOK3, 
													 wxDefaultPosition, wxDefaultSize, wxAUI_NB_WINDOWLIST_BUTTON );
	
    wxPanel* itemPanel7 = new wxPanel( itemNotebook6, ID_PANEL5, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer8 = new wxBoxSizer(wxVERTICAL);
    itemPanel7->SetSizer(itemBoxSizer8);
	
    
	m_pObjList_L_Freq = new tmCheckListBoxRank (itemPanel7, ID_CHECKLISTBOX4, wxDefaultPosition,
											 wxDefaultSize, 0, NULL, wxLB_MULTIPLE | tmLB_MENU);
    itemBoxSizer8->Add(m_pObjList_L_Freq, 1, wxGROW, 0); //5);
	
    itemNotebook6->AddPage(itemPanel7, _("Frequent"));
	
    wxPanel* itemPanel10 = new wxPanel( itemNotebook6, ID_PANEL6, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer11 = new wxBoxSizer(wxVERTICAL);
    itemPanel10->SetSizer(itemBoxSizer11);
	
	m_pObjList_L_NoFreq = new tmCheckListBoxRank  (itemPanel10, ID_CHECKLISTBOX3, wxDefaultPosition,
											 wxDefaultSize, 0, NULL, wxLB_MULTIPLE | tmLB_MENU);
	
    itemBoxSizer11->Add(m_pObjList_L_NoFreq, 1, wxGROW,0);// 5);
	
    itemNotebook6->AddPage(itemPanel10, _("Less frequent"));
	
    itemBoxSizer5->Add(itemNotebook6, 1, wxGROW, 0);// 5);
	
    m_AttribNotebook->AddPage(itemPanel4, _("Lines"));
	
    wxPanel* itemPanel13 = new wxPanel( m_AttribNotebook, ID_PANEL7, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer14 = new wxBoxSizer(wxVERTICAL);
    itemPanel13->SetSizer(itemBoxSizer14);
	
	m_pObjList_PT = new tmCheckListBoxRank (itemPanel13, ID_CHECKLISTBOX2, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_MULTIPLE | tmLB_MENU);
	itemBoxSizer14->Add(m_pObjList_PT, 1, wxGROW|wxALL, 0);// 5);
	
    m_AttribNotebook->AddPage(itemPanel13, _("Point"));
	
    wxPanel* itemPanel21 = new wxPanel( m_AttribNotebook, ID_PANEL8, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer22 = new wxBoxSizer(wxVERTICAL);
    itemPanel21->SetSizer(itemBoxSizer22);
	
    m_pObjList_PLG = new tmCheckListBoxRank(itemPanel21, ID_CHECKLISTBOX1, 
										wxDefaultPosition, wxDefaultSize,
										0, NULL, wxLB_MULTIPLE | tmLB_MENU);
	
	itemBoxSizer22->Add(m_pObjList_PLG, 1, wxGROW | wxALL, 0);// 5);
	
    m_AttribNotebook->AddPage(itemPanel21, _("Polygons"));
	
    wxPanel* itemPanel24 = new wxPanel( m_AttribNotebook, ID_PANEL23, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer25 = new wxBoxSizer(wxVERTICAL);
    itemPanel24->SetSizer(itemBoxSizer25);
	
    wxFlexGridSizer* itemFlexGridSizer26 = new wxFlexGridSizer(4, 2, 0, 0);
    itemFlexGridSizer26->AddGrowableCol(1);
    itemBoxSizer25->Add(itemFlexGridSizer26, 0, wxGROW|wxLEFT|wxRIGHT, 5);
    wxStaticText* itemStaticText27 = new wxStaticText( itemPanel24, wxID_STATIC, _("Priority :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer26->Add(itemStaticText27, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
	
    wxArrayString itemChoice28Strings;
    itemChoice28Strings.Add(_("High"));
    itemChoice28Strings.Add(_("Medium"));
    itemChoice28Strings.Add(_("Low"));
    wxChoice* itemChoice28 = new wxChoice( itemPanel24, ID_CHOICE13, wxDefaultPosition, wxDefaultSize, itemChoice28Strings, 0 );
    itemChoice28->SetStringSelection(_("High"));
    itemFlexGridSizer26->Add(itemChoice28, 0, wxGROW|wxALIGN_CENTER_VERTICAL, 0);
	
    wxStaticText* itemStaticText29 = new wxStaticText( itemPanel24, wxID_STATIC, _("Author :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer26->Add(itemStaticText29, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxTOP, 5);
	
    wxTextCtrl* itemTextCtrl30 = new wxTextCtrl( itemPanel24, ID_TEXTCTRL19, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer26->Add(itemTextCtrl30, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxTOP, 5);
	
    wxStaticText* itemStaticText31 = new wxStaticText( itemPanel24, wxID_STATIC, _("Code :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer26->Add(itemStaticText31, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxTOP, 5);
	
    wxArrayString itemChoice32Strings;
    itemChoice32Strings.Add(_("Error"));
    itemChoice32Strings.Add(_("Note"));
    itemChoice32Strings.Add(_("Task"));
    wxChoice* itemChoice32 = new wxChoice( itemPanel24, ID_CHOICE14, wxDefaultPosition, wxDefaultSize, itemChoice32Strings, 0 );
    itemChoice32->SetStringSelection(_("Error"));
    itemFlexGridSizer26->Add(itemChoice32, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxTOP, 5);
	
    wxStaticText* itemStaticText33 = new wxStaticText( itemPanel24, wxID_STATIC, _("Date :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer26->Add(itemStaticText33, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	
    wxDatePickerCtrl* itemDatePickerCtrl34 = new wxDatePickerCtrl( itemPanel24, ID_DATEPICKERCTRL); //, wxDateTime(), wxDefaultPosition, wxDefaultSize, wxDP_DEFAULT );
    itemFlexGridSizer26->Add(itemDatePickerCtrl34, 0, wxGROW|wxALIGN_CENTER_VERTICAL, 5);
	
    wxStaticBox* itemStaticBoxSizer35Static = new wxStaticBox(itemPanel24, wxID_ANY, _("Description"));
    wxStaticBoxSizer* itemStaticBoxSizer35 = new wxStaticBoxSizer(itemStaticBoxSizer35Static, wxVERTICAL);
    itemBoxSizer25->Add(itemStaticBoxSizer35, 1, wxGROW, 0);
    wxTextCtrl* itemTextCtrl36 = new wxTextCtrl( itemPanel24, ID_TEXTCTRL21, _T(""), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE );
    itemStaticBoxSizer35->Add(itemTextCtrl36, 1, wxGROW, 0);
	
    wxCheckBox* itemCheckBox37 = new wxCheckBox( itemPanel24, ID_CHECKBOX7, _("fixed"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox37->SetValue(false);
    itemBoxSizer25->Add(itemCheckBox37, 0, wxGROW|wxTOP, 5);
	
    m_AttribNotebook->AddPage(itemPanel24, _("Notes"));
	
    itemBoxSizer2->Add(m_AttribNotebook, 1, wxGROW, 0);//|wxTOP|wxBOTTOM, 5);
	
    wxGridSizer* itemGridSizer38 = new wxGridSizer(2, 2, 0, 0);
    itemBoxSizer2->Add(itemGridSizer38, 0, wxGROW|wxALL, 5);
	
    wxSearchCtrl* itemSearchCtrl39 = new wxSearchCtrl( parent, ID_SEARCHCTRL1, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemSearchCtrl39->ShowCancelButton(TRUE);
	itemGridSizer38->Add(itemSearchCtrl39, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    wxCheckBox* itemCheckBox40 = new wxCheckBox( parent, ID_CHECKBOX9, _("full attribution"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox40->SetValue(true);
    itemGridSizer38->Add(itemCheckBox40, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    m_InfoBtn = new wxButton( parent, ID_DLG_OBJ_ATTRIBUTION_BTN_INFO, _("Info"),
							 wxDefaultPosition, wxDefaultSize, 0 );
    m_InfoBtn->Enable(false);
	itemGridSizer38->Add(m_InfoBtn, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    m_AttribBtn = new wxButton( parent, ID_DLG_OBJ_ATTRIBUTION_BTN_ATTRIBUTE,
							   m_AttribBtnLabel, wxDefaultPosition, wxDefaultSize, 0 );
    m_AttribBtn->Enable(false); // disable by default
	itemGridSizer38->Add(m_AttribBtn, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);
		
    m_AutoDisplayAttributes = new wxCheckBox( parent, ID_CHECKBOX5, _("Auto display attributes"), wxDefaultPosition, wxDefaultSize, 0 );
    m_AutoDisplayAttributes->SetValue(false);
    itemBoxSizer2->Add(m_AutoDisplayAttributes, 0, wxGROW|wxALL, 5);
	
	
	if (set_sizer)
    {
        parent->SetSizer( itemBoxSizer2 );
        if (call_fit)
            itemBoxSizer2->SetSizeHints( parent );
    }
    
    return itemBoxSizer2;
}



/***************************************************************************//**
 @brief Update items in the object point list
 @details This functions delete all items in the list and add items directly
 from the database
 @param pDB object of #DataBaseTM
 @return  TRUE if operations was a succes, false otherwise
 @author Lucien Schreiber (c) CREALP 2008
 @date 15 May 2008
 *******************************************************************************/
bool AttribObjType_PANEL::UpdateObjectPointList(DataBaseTM * pDB)
{
	// clear and update
	m_pObjList_PT->ClearItems();
	return UpdateObjectList(pDB, m_pObjList_PT, LAYER_POINT);
}


/***************************************************************************//**
 @brief Update items in the object polygon list
 @details This functions delete all items in the list and add items directly
 from the database
 @param pDB object of #DataBaseTM
 @return  TRUE if operations was a succes, false otherwise
 @author Lucien Schreiber (c) CREALP 2008
 @date 16 May 2008
 *******************************************************************************/
bool AttribObjType_PANEL::UpdateObjectPolyList (DataBaseTM * pDB)
{
	m_pObjList_PLG->ClearItems();
	return UpdateObjectList(pDB, m_pObjList_PLG, LAYER_POLYGON);
}


/***************************************************************************//**
 @brief Update items in the object line list
 @details This functions delete all items in the list and add items directly
 from the database
 @param pDB object of #DataBaseTM
 @return  TRUE if operations was a succes, false otherwise
 @author Lucien Schreiber (c) CREALP 2008
 @date 16 May 2008
 *******************************************************************************/
bool AttribObjType_PANEL::UpdateObjectLineList (DataBaseTM * pDB)
{
	m_pObjList_L_Freq->ClearItems();
	m_pObjList_L_NoFreq->ClearItems();
	
	if(!UpdateObjectList(pDB, m_pObjList_L_Freq, LAYER_LINE, OBJECT_FREQUENT))
		return FALSE;
	
	if (!UpdateObjectList(pDB, m_pObjList_L_NoFreq, LAYER_LINE, OBJECT_LESS_FREQUENT))
		return FALSE;
		
	return TRUE;
}

/***************************************************************************//**
 @brief Update lists
 @details This functions is called internally by
 AttribObj_PANEL::UpdateObjectPointList() for exemple and is used for updating
 all lists
 @param pDB Pointer to a valid #DataBaseTM instance
 @param pList Pointer to a valid #tmCheckListBox instance
 @param type The type of the list to update. may be one of the following : 
 - #LAYER_LINE 
 - #LAYER_POINT 
 - #LAYER_POLYGON
 @param frequency @todo describe frequency here
 @return  TRUE if list updated, FALSE otherwise
 @author Lucien Schreiber (c) CREALP 2008
 @date 16 May 2008
 *******************************************************************************/
bool AttribObjType_PANEL::UpdateObjectList(DataBaseTM * pDB,tmCheckListBox * pList,
										   int type, short frequency)
{
	// check database
	if(!pDB)
	{
		wxLogDebug(_T("Pointer to the database not valid"));
		return FALSE;
	}
	
	
	
	// get objects from the database (all points)
	if(!pDB->GetObjectListByLayerType(type, TRUE))
	{
		wxLogDebug(_T("Error getting object for layer : %d"), type);
		return FALSE;
	}
		
		
	// loop for all results 
	ProjectDefMemoryObjects myTempObject;
	while (1)
	{
		if (pDB->DataBaseGetNextResultAsObject(&myTempObject, type))
		{
			if (type != LAYER_LINE)
			{
					pList->AddItem(-1, myTempObject.m_ObjectID,
						   myTempObject.m_ObjectName);
			}
			else // layer line
			{
				if (frequency == myTempObject.m_ObjectFreq)
				{
					pList->AddItem(-1, myTempObject.m_ObjectID,
								   myTempObject.m_ObjectName);
				}
			}

		}
			
		else
			break;
	}
	
	// pass database pointer to list
	SetDataBaseToList(pDB);
		
	
	return TRUE;
}


/***************************************************************************//**
 @brief Pass database to check box list
 @param pDB pointer to a valid database instance
 @author Lucien Schreiber (c) CREALP 2008
 @date 21 May 2008
 *******************************************************************************/
void AttribObjType_PANEL::SetDataBaseToList (DataBaseTM * pDB)
{
	if (pDB == NULL)
		return;
	
	m_pObjList_L_Freq->SetDataBase(pDB);
	m_pObjList_L_NoFreq->SetDataBase(pDB);
	m_pObjList_PLG->SetDataBase(pDB);
	m_pObjList_PT->SetDataBase(pDB);
}



/***************************************************************************//**
 @brief Set the text for the button
 @details This function allows a visual indication about the numer of item
 selected. Bellow are the possible case based on the number of selected
 features :
 - 0 : Button is disabled
 - 1 : Button is normal
 - 1+ : Button is bold and number of features are specified next to the label
 @param nbfeatures Number of features selected
 @author Lucien Schreiber (c) CREALP 2008
 @date 04 November 2008
 *******************************************************************************/
void AttribObjType_PANEL::SetAttributeBtn (int nbfeatures)
{
	wxString myButtonLabel = m_AttribBtnLabel;
	bool myEnable = true;
	wxFont myBtnFont (*wxNORMAL_FONT);
	
	
	if (nbfeatures == 0) // 0 item selected
		myEnable = false;
	
	if (nbfeatures > 1) // more than one feature selected
	{
		myButtonLabel.Append(wxString::Format(_T(" (%d)"),nbfeatures));
		myBtnFont.SetWeight(wxFONTWEIGHT_BOLD);
	}
	
	m_AttribBtn->SetLabel(myButtonLabel);
	m_AttribBtn->Enable(myEnable);
	m_AttribBtn->SetFont(myBtnFont); 

}


/***************************************************************************//**
 @brief Enable or disable the Info button
 @details Based on the number of features passed, this function enable or not
 the info button :
 - 0 features, button disabled
 - 1 feature, button enabled
 - 1+ feature, button disabled
 @param nbfeatures Number of selected features
 @author Lucien Schreiber (c) CREALP 2008
 @date 07 November 2008
 *******************************************************************************/
void AttribObjType_PANEL::SetInfoBtn (int nbfeatures)
{
	if (nbfeatures == 1)
		m_InfoBtn->Enable(true);
	else
		m_InfoBtn->Enable(false);
}



/***************************************************************************//**
 @brief Set visible notebook
 @details Activate the correct notebook depending on the selected object type.
 Case supported are : 
 - TOC_NAME_LINES
 - TOC_NAME_POINTS
 - TOC_NAME_LABELS
 - TOC_NAME_ANNOTATIONS
 for other case, nothing is changed
 @param notebooktype one of the #TOC_GENERIC_NAME values
 @author Lucien Schreiber (c) CREALP 2008
 @date 05 November 2008
 *******************************************************************************/
void AttribObjType_PANEL::SetVisibleNotebook (TOC_GENERIC_NAME notebooktype)
{
	switch (notebooktype)
	{
		case TOC_NAME_LINES:
			m_AttribNotebook->ChangeSelection(0);
			break;
			
		case TOC_NAME_POINTS:
			m_AttribNotebook->ChangeSelection(1);
			break;
			
		case TOC_NAME_LABELS:
			m_AttribNotebook->ChangeSelection(2);
			break;
			
		case TOC_NAME_ANNOTATIONS:
			m_AttribNotebook->ChangeSelection(3);
			break;
			
		default:
			break;
	}
}



/***************************************************************************//**
 @brief Get visible notebook page
 @details Return the selected notebook page.
 @return  One of the following values :
 - TOC_NAME_LINES 
 - TOC_NAME_POINTS
 - TOC_NAME_LABELS
 - TOC_NAME_ANNOTATIONS
 - TOC_NAME_UNKNOWN in case of error
 @author Lucien Schreiber (c) CREALP 2008
 @date 05 November 2008
 *******************************************************************************/
TOC_GENERIC_NAME AttribObjType_PANEL::GetVisibleNotebook ()
{
	TOC_GENERIC_NAME myRetVal = TOC_NAME_UNKNOWN;
	switch (m_AttribNotebook->GetSelection())
	{
		case 0:
			myRetVal = TOC_NAME_LINES;
			break;
			
		case 1:
			myRetVal = TOC_NAME_POINTS;
			break;
			
		case 2:
			myRetVal = TOC_NAME_LABELS;
			break;
			
		case 3:
			myRetVal = TOC_NAME_ANNOTATIONS;
			break;
			
		default:
			myRetVal = TOC_NAME_UNKNOWN;
			break;
	}
	return myRetVal;
}



/***************************************************************************//**
 @brief Called when user press attribute
 @details This function only send an event of type tmEVT_ATTRIBUTION_BTN_PRESSED
 actually intercepted by the #tmAttributionManager
 @author Lucien Schreiber (c) CREALP 2008
 @date 05 November 2008
 *******************************************************************************/
void AttribObjType_PANEL::OnAttributeBtn (wxCommandEvent & event)
{
	wxCommandEvent evt (tmEVT_ATTRIBUTION_BTN_PRESSED, wxID_ANY);
	m_ParentEvt->GetEventHandler()->AddPendingEvent(evt);
}



/***************************************************************************//**
 @brief Called when user press Info
 @details This function only send an event of type tmEVT_INFO_BTN_PRESSED
 actually intercepted by the #tmAttributionManager
 @author Lucien Schreiber (c) CREALP 2008
 @date 07 November 2008
 *******************************************************************************/
void AttribObjType_PANEL::OnInfoBtn (wxCommandEvent & event)
{
	wxCommandEvent evt (tmEVT_INFO_BTN_PRESSED, wxID_ANY);
	m_ParentEvt->GetEventHandler()->AddPendingEvent(evt);
}


/***************************************************************************//**
 @brief Get checked values from lists
 @param panel_name one of the #TOC_GENERIC_NAME. Allowed values are :
 - TOC_NAME_LINES 
 - TOC_NAME_POINTS 
 - TOC_NAME_LABELS 
 @param values return all ids of checked values
 @param panel_freq if true, get values for Frequent lines. Usefull only if
 panel_name = TOC_NAME_LINES
 @return  number of values checked
 @author Lucien Schreiber (c) CREALP 2008
 @date 06 November 2008
 *******************************************************************************/
int AttribObjType_PANEL::GetSelectedValues (TOC_GENERIC_NAME panel_name,
					   wxArrayLong & values,
					   bool panel_freq)
{
	tmCheckListBoxRank * myList = NULL;
	switch (panel_name)
	{
		case TOC_NAME_LINES:
			if (panel_freq)
				myList = m_pObjList_L_Freq;
			else
				myList = m_pObjList_L_NoFreq;
			break;
			
		case TOC_NAME_POINTS:
			myList = m_pObjList_PT;
			break;
			
		case TOC_NAME_LABELS:
			myList = m_pObjList_PLG;
			break;
			
		default:
			myList = NULL;
			break;
	}
	
	if (!myList)
		return 0;
	
	
	long myCheckedID = 0;
	wxString mytemp = _T("");
	bool myChecked = false;
	
	for (unsigned int i = 0; i< myList->GetCount(); i++)
	{
		if (myList->GetItem(i, myCheckedID, mytemp, myChecked))
		{
			if (myChecked == true)
				values.Add(myCheckedID);
		}
		else
			break;
	}
	
	return values.GetCount();
}


/***************************************************************************//**
 @brief Set checked values to the lists
 @param panel_name one of the #TOC_GENERIC_NAME. Allowed values are :
 - TOC_NAME_LINES 
 - TOC_NAME_POINTS 
 - TOC_NAME_LABELS 
 @param values contain all values to check
 @param panel_freq if true, set values for Frequent lines. Usefull only if
 panel_name = TOC_NAME_LINES
 @author Lucien Schreiber (c) CREALP 2008
 @date 06 November 2008
 *******************************************************************************/
void AttribObjType_PANEL::SetSelectedValues (TOC_GENERIC_NAME panel_name,
											 const wxArrayLong & values,
											 bool panel_freq)
{
	tmCheckListBoxRank * myList = NULL;
	switch (panel_name)
	{
		case TOC_NAME_LINES:
			if (panel_freq)
				myList = m_pObjList_L_Freq;
			else
				myList = m_pObjList_L_NoFreq;
			break;
			
		case TOC_NAME_POINTS:
			myList = m_pObjList_PT;
			break;
			
		case TOC_NAME_LABELS:
			myList = m_pObjList_PLG;
			break;
			
		default:
			myList = NULL;
			break;
	}
	
	if (!myList)
		return;
	
	
	long myCheckedID = 0;
	wxString mytemp = _T("");
	bool myChecked = false;
	
	// for each item in the list 
	for (unsigned int l = 0; l < myList->GetCount(); l++)
	{
		myList->GetItem(l, myCheckedID, mytemp, myChecked);
		myChecked = false;
		// search the values
		for (unsigned int v = 0; v< values.GetCount(); v++)
		{
			if (values.Item(v) == myCheckedID)
			{
				myChecked = true;
				break;
			}
		}
		
		myList->EditItem(l, -1, wxEmptyString, myChecked);
	}

}



/***************************************************************************//**
 @brief Get Status of Auto display attributes checkbox
 @return  true if "Auto Display Attribute" check box is checked, false otherwise
 @author Lucien Schreiber (c) CREALP 2008
 @date 07 November 2008
 *******************************************************************************/
bool AttribObjType_PANEL::IsAutoDisplayAttributeChecked()
{
	return m_AutoDisplayAttributes->IsChecked();
}

