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


AttribObjType_PANEL::AttribObjType_PANEL(wxWindow * parent, wxAuiManager * AuiManager) : ManagedAuiWnd(AuiManager)
{
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
	
}


wxSizer * AttribObjType_PANEL::CreateControls(wxWindow * parent, bool call_fit, bool set_sizer)
{    
    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    //itemPanel1->SetSizer(itemBoxSizer2);
	
    wxNotebook* itemNotebook3 = new wxNotebook( parent, ID_NOTEBOOK2, wxDefaultPosition, wxSize(300, -1), wxBK_DEFAULT );
	
    wxPanel* itemPanel4 = new wxPanel( itemNotebook3, ID_PANEL4, wxDefaultPosition, wxSize(300, 300), wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer5 = new wxBoxSizer(wxVERTICAL);
    itemPanel4->SetSizer(itemBoxSizer5);
	
    wxNotebook* itemNotebook6 = new wxNotebook( itemPanel4, ID_NOTEBOOK3, wxDefaultPosition, wxDefaultSize, wxBK_DEFAULT );
	
    wxPanel* itemPanel7 = new wxPanel( itemNotebook6, ID_PANEL5, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer8 = new wxBoxSizer(wxVERTICAL);
    itemPanel7->SetSizer(itemBoxSizer8);
	
    wxArrayString itemCheckListBox9Strings;
    wxCheckListBox* itemCheckListBox9 = new wxCheckListBox( itemPanel7, ID_CHECKLISTBOX4, wxDefaultPosition, wxDefaultSize, itemCheckListBox9Strings, wxLB_SINGLE );
    itemBoxSizer8->Add(itemCheckListBox9, 1, wxGROW, 5);
	
    itemNotebook6->AddPage(itemPanel7, _("Frequent"));
	
    wxPanel* itemPanel10 = new wxPanel( itemNotebook6, ID_PANEL6, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer11 = new wxBoxSizer(wxVERTICAL);
    itemPanel10->SetSizer(itemBoxSizer11);
	
    wxArrayString itemCheckListBox12Strings;
    wxCheckListBox* itemCheckListBox12 = new wxCheckListBox( itemPanel10, ID_CHECKLISTBOX3, wxDefaultPosition, wxDefaultSize, itemCheckListBox12Strings, wxLB_SINGLE );
    itemBoxSizer11->Add(itemCheckListBox12, 1, wxGROW, 5);
	
    itemNotebook6->AddPage(itemPanel10, _("Less frequent"));
	
    itemBoxSizer5->Add(itemNotebook6, 1, wxGROW|wxALL, 5);
	
    itemNotebook3->AddPage(itemPanel4, _("Lines"));
	
    wxPanel* itemPanel13 = new wxPanel( itemNotebook3, ID_PANEL7, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer14 = new wxBoxSizer(wxVERTICAL);
    itemPanel13->SetSizer(itemBoxSizer14);
	
	m_pObjList_PT = new tmCheckListBox (itemPanel13, ID_CHECKLISTBOX2, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_MULTIPLE | tmLB_MENU);
    //wxArrayString itemCheckListBox15Strings;
    //wxCheckListBox* itemCheckListBox15 = new wxCheckListBox( itemPanel13, ID_CHECKLISTBOX2, wxDefaultPosition, wxDefaultSize, itemCheckListBox15Strings, wxLB_SINGLE );
    itemBoxSizer14->Add(m_pObjList_PT, 1, wxGROW|wxALL, 5);
	
    /*wxStaticBox* itemStaticBoxSizer16Static = new wxStaticBox(itemPanel13, wxID_ANY, _("Orientation"));
    wxStaticBoxSizer* itemStaticBoxSizer16 = new wxStaticBoxSizer(itemStaticBoxSizer16Static, wxVERTICAL);
    itemBoxSizer14->Add(itemStaticBoxSizer16, 0, wxGROW|wxALL, 5);
    wxBoxSizer* itemBoxSizer17 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer16->Add(itemBoxSizer17, 0, wxGROW|wxALL, 5);
    wxStaticText* itemStaticText18 = new wxStaticText( itemPanel13, wxID_STATIC, _("Angle :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer17->Add(itemStaticText18, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    wxTextCtrl* itemTextCtrl19 = new wxTextCtrl( itemPanel13, ID_TEXTCTRL6, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer17->Add(itemTextCtrl19, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    wxButton* itemButton20 = new wxButton( itemPanel13, ID_BUTTON9, _(" Interactive orientation"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticBoxSizer16->Add(itemButton20, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);*/
	
    itemNotebook3->AddPage(itemPanel13, _("Point"));
	
    wxPanel* itemPanel21 = new wxPanel( itemNotebook3, ID_PANEL8, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer22 = new wxBoxSizer(wxVERTICAL);
    itemPanel21->SetSizer(itemBoxSizer22);
	
    wxArrayString itemCheckListBox23Strings;
    wxCheckListBox* itemCheckListBox23 = new wxCheckListBox( itemPanel21, ID_CHECKLISTBOX1, wxDefaultPosition, wxDefaultSize, itemCheckListBox23Strings, wxLB_SINGLE );
    itemBoxSizer22->Add(itemCheckListBox23, 1, wxGROW | wxALL, 5);
	
    itemNotebook3->AddPage(itemPanel21, _("Polygons"));
	
    wxPanel* itemPanel24 = new wxPanel( itemNotebook3, ID_PANEL23, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer25 = new wxBoxSizer(wxVERTICAL);
    itemPanel24->SetSizer(itemBoxSizer25);
	
    wxFlexGridSizer* itemFlexGridSizer26 = new wxFlexGridSizer(4, 2, 0, 0);
    itemFlexGridSizer26->AddGrowableCol(1);
    itemBoxSizer25->Add(itemFlexGridSizer26, 0, wxGROW|wxALL, 5);
    wxStaticText* itemStaticText27 = new wxStaticText( itemPanel24, wxID_STATIC, _("Priority :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer26->Add(itemStaticText27, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    wxArrayString itemChoice28Strings;
    itemChoice28Strings.Add(_("High"));
    itemChoice28Strings.Add(_("Medium"));
    itemChoice28Strings.Add(_("Low"));
    wxChoice* itemChoice28 = new wxChoice( itemPanel24, ID_CHOICE13, wxDefaultPosition, wxDefaultSize, itemChoice28Strings, 0 );
    itemChoice28->SetStringSelection(_("High"));
    itemFlexGridSizer26->Add(itemChoice28, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    wxStaticText* itemStaticText29 = new wxStaticText( itemPanel24, wxID_STATIC, _("Author :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer26->Add(itemStaticText29, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    wxTextCtrl* itemTextCtrl30 = new wxTextCtrl( itemPanel24, ID_TEXTCTRL19, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer26->Add(itemTextCtrl30, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    wxStaticText* itemStaticText31 = new wxStaticText( itemPanel24, wxID_STATIC, _("Code :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer26->Add(itemStaticText31, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    wxArrayString itemChoice32Strings;
    itemChoice32Strings.Add(_("Error"));
    itemChoice32Strings.Add(_("Note"));
    itemChoice32Strings.Add(_("Task"));
    wxChoice* itemChoice32 = new wxChoice( itemPanel24, ID_CHOICE14, wxDefaultPosition, wxDefaultSize, itemChoice32Strings, 0 );
    itemChoice32->SetStringSelection(_("Error"));
    itemFlexGridSizer26->Add(itemChoice32, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    wxStaticText* itemStaticText33 = new wxStaticText( itemPanel24, wxID_STATIC, _("Date :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer26->Add(itemStaticText33, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    //wxDatePickerCtrl* itemDatePickerCtrl34 = new wxDatePickerCtrl( itemPanel24, ID_DATEPICKERCTRL); //, wxDateTime(), wxDefaultPosition, wxDefaultSize, wxDP_DEFAULT );
    //itemFlexGridSizer26->Add(itemDatePickerCtrl34, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    wxStaticBox* itemStaticBoxSizer35Static = new wxStaticBox(itemPanel24, wxID_ANY, _("Description"));
    wxStaticBoxSizer* itemStaticBoxSizer35 = new wxStaticBoxSizer(itemStaticBoxSizer35Static, wxVERTICAL);
    itemBoxSizer25->Add(itemStaticBoxSizer35, 1, wxGROW|wxALL, 5);
    wxTextCtrl* itemTextCtrl36 = new wxTextCtrl( itemPanel24, ID_TEXTCTRL21, _T(""), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE );
    itemStaticBoxSizer35->Add(itemTextCtrl36, 1, wxGROW, 5);
	
    wxCheckBox* itemCheckBox37 = new wxCheckBox( itemPanel24, ID_CHECKBOX7, _("fixed"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox37->SetValue(false);
    itemBoxSizer25->Add(itemCheckBox37, 0, wxGROW|wxALL, 5);
	
    itemNotebook3->AddPage(itemPanel24, _("Notes"));
	
    itemBoxSizer2->Add(itemNotebook3, 1, wxGROW|wxALL, 5);
	
    wxGridSizer* itemGridSizer38 = new wxGridSizer(2, 2, 0, 0);
    itemBoxSizer2->Add(itemGridSizer38, 0, wxGROW|wxALL, 5);
	
    wxSearchCtrl* itemSearchCtrl39 = new wxSearchCtrl( parent, ID_SEARCHCTRL1, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemGridSizer38->Add(itemSearchCtrl39, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    wxCheckBox* itemCheckBox40 = new wxCheckBox( parent, ID_CHECKBOX9, _("full attribution"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox40->SetValue(true);
    itemGridSizer38->Add(itemCheckBox40, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    wxButton* itemButton41 = new wxButton( parent, ID_BUTTON7, _("Info"), wxDefaultPosition, wxDefaultSize, 0 );
    itemGridSizer38->Add(itemButton41, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    wxButton* itemButton42 = new wxButton( parent, ID_BUTTON8, _("Attribute"), wxDefaultPosition, wxDefaultSize, 0 );
    itemGridSizer38->Add(itemButton42, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    wxCheckBox* itemCheckBox43 = new wxCheckBox( parent, ID_CHECKBOX5, _("Clear value list after attribution"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox43->SetValue(true);
    itemBoxSizer2->Add(itemCheckBox43, 0, wxGROW|wxALL, 5);
	
	
	if (set_sizer)
    {
        parent->SetSizer( itemBoxSizer2 );
        if (call_fit)
            itemBoxSizer2->SetSizeHints( parent );
    }
    
    return itemBoxSizer2;
}



/***************************************************************************//**
 @brief Update items in the object list
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
	if(!pDB->GetObjectListByLayerType(type))
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
			pList->AddItem(-1, myTempObject.m_ObjectID,
						   myTempObject.m_ObjectName);
		}
			
		else
			break;
	}
	
		
	
	return TRUE;
}
