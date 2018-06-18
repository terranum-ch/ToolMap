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

#include "../database/database_tm.h"    // for database acces (updating lists)
#include "attribution_obj_type.h"
#include "wxflatbutton.h"


//DEFINE_EVENT_TYPE (tmEVT_ATTRIBUTION_BTN_PRESSED)
DEFINE_EVENT_TYPE (tmEVT_INFO_BTN_PRESSED)
DEFINE_EVENT_TYPE(tmEVT_ADD_BTN_PRESSED)
DEFINE_EVENT_TYPE(tmEVT_REMOVE_BTN_PRESSED)


BEGIN_EVENT_TABLE(AttribObjType_PANEL, ManagedAuiWnd)
                //EVT_FLATBUTTON (ID_DLG_OBJ_ATTRIBUTION_BTN_ATTRIBUTE, AttribObjType_PANEL::OnAttributeBtn)
                EVT_FLATBUTTON (ID_DLG_OBJ_ATTRIBUTION_BTN_INFO, AttribObjType_PANEL::OnInfoBtn)
                EVT_FLATBUTTON (ID_DLG_OBJ_ATTRIBUTION_BTN_ADD, AttribObjType_PANEL::OnAddBtn)
                EVT_FLATBUTTON (ID_DLG_OBJ_ATTRIBUTION_BTN_REMOVE, AttribObjType_PANEL::OnRemoveBtn)
                EVT_COMMAND (wxID_ANY, tmEVT_EM_EDIT_START, AttribObjType_PANEL::OnEditStart)
                EVT_COMMAND (wxID_ANY, tmEVT_EM_EDIT_STOP, AttribObjType_PANEL::OnEditStop)
END_EVENT_TABLE()


AttribObjType_PANEL::AttribObjType_PANEL(wxWindow *parent, wxAuiManager *AuiManager) : ManagedAuiWnd(AuiManager)
{
    m_AutoDisplayAttributes = false;
    m_EmptyListAfterAttributes = true;
    m_EnableFullAttribution = false;
    m_ParentEvt = parent;
    m_ParentEvt->PushEventHandler(this);
    m_NbFeaturesSelected = 0;

    wxPanel *ContentFrame = new wxPanel(parent, wxID_ANY);
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

    // add the panel
    AddManagedPane(ContentFrame, mPaneInfo);

    // Connect events
    m_textCtrlPoints->Connect(wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(AttribObjType_PANEL::FilterPointList), NULL, this);
    m_textCtrlPoly->Connect(wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(AttribObjType_PANEL::FilterPolyList), NULL, this);
    m_textCtrlLines->Connect(wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(AttribObjType_PANEL::FilterLineList), NULL, this);

}

AttribObjType_PANEL::~AttribObjType_PANEL()
{
    // Disconnect events
    m_textCtrlPoints->Disconnect(wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(AttribObjType_PANEL::FilterPointList), NULL, this);
    m_textCtrlPoly->Disconnect(wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(AttribObjType_PANEL::FilterPolyList), NULL, this);
    m_textCtrlLines->Disconnect(wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(AttribObjType_PANEL::FilterLineList), NULL, this);

    m_ParentEvt->PopEventHandler(FALSE);
}


wxSizer *AttribObjType_PANEL::CreateControls(wxWindow *parent, bool call_fit, bool set_sizer)
{
    m_AttribSizer = new wxBoxSizer(wxVERTICAL);
    //itemPanel1->SetSizer(itemBoxSizer2);

    m_AttribNotebook = new wxChoicebook(parent, wxID_ANY, wxDefaultPosition, wxSize(300, -1), wxCHB_DEFAULT);


    // Lines panel

    wxPanel *itemPanel1 = new wxPanel(m_AttribNotebook, wxID_ANY, wxDefaultPosition, wxSize(300, 300), wxTAB_TRAVERSAL);
    wxBoxSizer *itemBoxSizer1 = new wxBoxSizer(wxVERTICAL);
    itemPanel1->SetSizer(itemBoxSizer1);

    m_textCtrlLines = new wxSearchCtrl(itemPanel1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
    m_textCtrlLines->SetToolTip( wxT("Type to filter the list") );
    itemBoxSizer1->Add(m_textCtrlLines, 0, wxALL | wxEXPAND, 2);

    wxAuiNotebook *itemNotebook1 = new wxAuiNotebook(itemPanel1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_NB_TOP);

    wxPanel *itemPanel2 = new wxPanel(itemNotebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
    wxBoxSizer *itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemPanel2->SetSizer(itemBoxSizer2);

    m_pObjList_L_Freq = new tmCheckListBoxRank(itemPanel2, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL,
                                               tmLB_MENU);
    itemBoxSizer2->Add(m_pObjList_L_Freq, 1, wxGROW, 0);

    itemNotebook1->AddPage(itemPanel2, _("Frequent"));

    wxPanel *itemPanel3 = new wxPanel(itemNotebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
    wxBoxSizer *itemBoxSizer3 = new wxBoxSizer(wxVERTICAL);
    itemPanel3->SetSizer(itemBoxSizer3);

    m_pObjList_L_NoFreq = new tmCheckListBoxRank(itemPanel3, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL,
                                                 tmLB_MENU);
    itemBoxSizer3->Add(m_pObjList_L_NoFreq, 1, wxGROW, 0);

    itemNotebook1->AddPage(itemPanel3, _("Less frequent"));

    itemBoxSizer1->Add(itemNotebook1, 1, wxGROW, 0);

    m_AttribNotebook->AddPage(itemPanel1, _("Lines"));


    // Points panel

    wxPanel *itemPanel4 = new wxPanel(m_AttribNotebook, wxID_ANY, wxDefaultPosition, wxSize(300, 300),
                                      wxTAB_TRAVERSAL);
    wxBoxSizer *itemBoxSizer4 = new wxBoxSizer(wxVERTICAL);
    itemPanel4->SetSizer(itemBoxSizer4);

    m_textCtrlPoints = new wxTextCtrl(itemPanel4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
    m_textCtrlPoints->SetToolTip( wxT("Type to filter the list") );
    itemBoxSizer4->Add(m_textCtrlPoints, 0, wxALL | wxEXPAND, 2);

    wxAuiNotebook *itemNotebook2 = new wxAuiNotebook(itemPanel4, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                                                     wxAUI_NB_TOP);

    wxPanel *itemPanel5 = new wxPanel(itemNotebook2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
    wxBoxSizer *itemBoxSizer5 = new wxBoxSizer(wxVERTICAL);
    itemPanel5->SetSizer(itemBoxSizer5);
	
    m_pObjList_PT_Freq = new tmCheckListBoxRank(itemPanel5, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL,
                                                tmLB_MENU);
    itemBoxSizer5->Add(m_pObjList_PT_Freq, 1, wxGROW, 0);

    itemNotebook2->AddPage(itemPanel5, _("Frequent"));

    wxPanel *itemPanel6 = new wxPanel(itemNotebook2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
    wxBoxSizer *itemBoxSizer6 = new wxBoxSizer(wxVERTICAL);
    itemPanel6->SetSizer(itemBoxSizer6);

    m_pObjList_PT_NoFreq = new tmCheckListBoxRank(itemPanel6, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0,
                                                  NULL, tmLB_MENU);
    itemBoxSizer6->Add(m_pObjList_PT_NoFreq, 1, wxGROW, 0);

    itemNotebook2->AddPage(itemPanel6, _("Less frequent"));
	
    itemBoxSizer4->Add(itemNotebook2, 1, wxEXPAND, 0);

    m_AttribNotebook->AddPage(itemPanel4, _("Point"));


    // Polygons panel

    wxPanel *itemPanel7 = new wxPanel(m_AttribNotebook, wxID_ANY, wxDefaultPosition, wxSize(300, 300),
                                      wxTAB_TRAVERSAL);
    wxBoxSizer *itemBoxSizer7 = new wxBoxSizer(wxVERTICAL);
    itemPanel7->SetSizer(itemBoxSizer7);

    m_textCtrlPoly = new wxTextCtrl(itemPanel7, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
    m_textCtrlPoly->SetToolTip( wxT("Type to filter the list") );
    itemBoxSizer7->Add(m_textCtrlPoly, 0, wxALL | wxEXPAND, 2);

    wxAuiNotebook *itemNotebook3 = new wxAuiNotebook(itemPanel7, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                                                     wxAUI_NB_TOP);

    wxPanel *itemPanel8 = new wxPanel(itemNotebook3, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
    wxBoxSizer *itemBoxSizer8 = new wxBoxSizer(wxVERTICAL);
    itemPanel8->SetSizer(itemBoxSizer8);


    m_pObjList_PLG_Freq = new tmCheckListBoxRank(itemPanel8, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL,
                                                tmLB_MENU);
    itemBoxSizer8->Add(m_pObjList_PLG_Freq, 1, wxGROW, 0);

    itemNotebook3->AddPage(itemPanel8, _("Frequent"));

    wxPanel *itemPanel9 = new wxPanel(itemNotebook3, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
    wxBoxSizer *itemBoxSizer9 = new wxBoxSizer(wxVERTICAL);
    itemPanel9->SetSizer(itemBoxSizer9);

    m_pObjList_PLG_NoFreq = new tmCheckListBoxRank(itemPanel9, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0,
                                                  NULL, tmLB_MENU);
    itemBoxSizer9->Add(m_pObjList_PLG_NoFreq, 1, wxGROW, 0);

    itemNotebook3->AddPage(itemPanel9, _("Less frequent"));

    itemBoxSizer7->Add(itemNotebook3, 1, wxGROW, 0);

    m_AttribNotebook->AddPage(itemPanel7, _("Polygons"));

#ifdef USE_NOTES
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
#endif

    m_AttribSizer->Add(m_AttribNotebook, 1, wxGROW, 0);//|wxTOP|wxBOTTOM, 5);

    m_WarningMultiFeatureCtrl = new wxStaticText(parent, wxID_ANY, wxEmptyString);
    wxFont myFont = wxNORMAL_FONT->Bold();
    m_WarningMultiFeatureCtrl->SetFont(myFont);
    m_AttribSizer->Add(m_WarningMultiFeatureCtrl, 0, wxALL | wxEXPAND, 5);

    m_ButtonSizer = new wxBoxSizer(wxHORIZONTAL);

    m_AttribBtn = new wxFlatButton(parent, ID_DLG_OBJ_ATTRIBUTION_BTN_ADD, _("Add"), wxDefaultSize);
    m_AttribBtn->Enable(false);
    m_ButtonSizer->Add(m_AttribBtn, 0, 0, 5);

    m_AddBtnCtrl = new wxFlatButton(parent, ID_DLG_OBJ_ATTRIBUTION_BTN_REMOVE, _("Remove"), wxDefaultSize);
    m_AddBtnCtrl->Enable(false);
    m_ButtonSizer->Add(m_AddBtnCtrl, 0, wxLEFT, 5);

    m_ButtonSizer->Add(0, 0, 1, wxEXPAND, 5);

    m_InfoBtn = new wxFlatButton(parent, ID_DLG_OBJ_ATTRIBUTION_BTN_INFO, _("Info"), wxDefaultSize);
    m_InfoBtn->Enable(false);
    m_ButtonSizer->Add(m_InfoBtn, 0, wxLEFT, 5);

    //m_ButtonSizer->Add( 0, 0, 2, 0, 5 );
    m_AttribSizer->Add(m_ButtonSizer, 0, wxALL | wxEXPAND, 5);

    if (set_sizer) {
        parent->SetSizer(m_AttribSizer);
        if (call_fit)
            m_AttribSizer->SetSizeHints(parent);
    }

    return m_AttribSizer;
}


/***************************************************************************//**
 @brief Called when Auto Display Attributes is enabled
 @details Switch state of Auto dispaly attributes each time this is called
 @author Lucien Schreiber (c) CREALP 2008
 @date 09 December 2008
 *******************************************************************************/
void AttribObjType_PANEL::OnDisplayAttributesAuto(wxCommandEvent &event)
{
    if (m_AutoDisplayAttributes)
        m_AutoDisplayAttributes = false;
    else
        m_AutoDisplayAttributes = true;
}


/***************************************************************************//**
 @brief Switch status of cleaning attribution each time called
 @author Lucien Schreiber (c) CREALP 2009
 @date 13 February 2009
 *******************************************************************************/
void AttribObjType_PANEL::OnEmptyListAffterAttributes(wxCommandEvent &event)
{
    if (m_EmptyListAfterAttributes)
        m_EmptyListAfterAttributes = false;
    else
        m_EmptyListAfterAttributes = true;

}


void AttribObjType_PANEL::OnFullAttribution(wxCommandEvent &event)
{
    m_EnableFullAttribution = !m_EnableFullAttribution;
}


void AttribObjType_PANEL::OnEditStart(wxCommandEvent &event)
{
    SetAttributeBtn(m_NbFeaturesSelected, true);
    event.Skip();
}


void AttribObjType_PANEL::OnEditStop(wxCommandEvent &event)
{
    SetAttributeBtn(m_NbFeaturesSelected, false);
    event.Skip();
}

void AttribObjType_PANEL::FilterPointList(wxCommandEvent &event)
{
    wxString filter = event.GetString();
    m_pObjList_PT_Freq->Filter(filter);
    m_pObjList_PT_NoFreq->Filter(filter);
    event.Skip();
}

void AttribObjType_PANEL::FilterPolyList(wxCommandEvent &event)
{
    wxString filter = event.GetString();
    m_pObjList_PLG_Freq->Filter(filter);
    m_pObjList_PLG_NoFreq->Filter(filter);
    event.Skip();
}

void AttribObjType_PANEL::FilterLineList(wxCommandEvent &event)
{
    wxString filter = event.GetString();
    m_pObjList_L_Freq->Filter(filter);
    m_pObjList_L_NoFreq->Filter(filter);
    event.Skip();
}

void AttribObjType_PANEL::ResetFilterFields()
{
    m_textCtrlLines->Clear();
    m_textCtrlPoly->Clear();
    m_textCtrlPoints->Clear();
}


/***************************************************************************//**
 @brief Clear all attributes set when called
 @author Lucien Schreiber (c) CREALP 2009
 @date 13 February 2009
 *******************************************************************************/
void AttribObjType_PANEL::EmptyListValues()
{
    wxASSERT(m_EmptyListAfterAttributes);
    m_pObjList_PT_Freq->ClearCheckMarks();
    m_pObjList_PT_NoFreq->ClearCheckMarks();
    m_pObjList_PLG_Freq->ClearCheckMarks();
    m_pObjList_PLG_NoFreq->ClearCheckMarks();
    m_pObjList_L_Freq->ClearCheckMarks();
    m_pObjList_L_NoFreq->ClearCheckMarks();
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
bool AttribObjType_PANEL::UpdateObjectPointList(DataBaseTM *pDB)
{
    // clear and update
    m_pObjList_PT_Freq->ClearItems();
    m_pObjList_PT_NoFreq->ClearItems();

    if (!UpdateObjectList(pDB, m_pObjList_PT_Freq, LAYER_POINT, OBJECT_FREQUENT))
        return FALSE;

    if (!UpdateObjectList(pDB, m_pObjList_PT_NoFreq, LAYER_POINT, OBJECT_LESS_FREQUENT))
        return FALSE;

	return true;
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
bool AttribObjType_PANEL::UpdateObjectPolyList(DataBaseTM *pDB)
{
    m_pObjList_PLG_Freq->ClearItems();
    m_pObjList_PLG_NoFreq->ClearItems();

    if (!UpdateObjectList(pDB, m_pObjList_PLG_Freq, LAYER_POLYGON, OBJECT_FREQUENT))
        return FALSE;

    if (!UpdateObjectList(pDB, m_pObjList_PLG_NoFreq, LAYER_POLYGON, OBJECT_LESS_FREQUENT))
        return FALSE;

    return TRUE;
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
bool AttribObjType_PANEL::UpdateObjectLineList(DataBaseTM *pDB)
{
    m_pObjList_L_Freq->ClearItems();
    m_pObjList_L_NoFreq->ClearItems();

    if (!UpdateObjectList(pDB, m_pObjList_L_Freq, LAYER_LINE, OBJECT_FREQUENT))
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
bool AttribObjType_PANEL::UpdateObjectList(DataBaseTM *pDB, tmCheckListBox *pList,
                                           int type, short frequency)
{
    // check database
    if (!pDB) {
        wxLogDebug(_T("Pointer to the database not valid"));
        return FALSE;
    }



    // get objects from the database (all points)
    if (!pDB->GetObjectListByLayerType(type, TRUE)) {
        wxLogDebug(_T("Error getting object for layer : %d"), type);
        return FALSE;
    }


    // loop for all results
    ProjectDefMemoryObjects myTempObject;
    while (1) {
        if (pDB->DataBaseGetNextResultAsObject(&myTempObject, type)) {
                if (frequency == myTempObject.m_ObjectFreq) {
                    pList->AddItem(-1, myTempObject.m_ObjectID,
                                   myTempObject.m_ObjectName);
                }

        } else
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
void AttribObjType_PANEL::SetDataBaseToList(DataBaseTM *pDB)
{
    if (pDB == NULL)
        return;

    m_pObjList_L_Freq->SetDataBase(pDB);
    m_pObjList_L_NoFreq->SetDataBase(pDB);
    m_pObjList_PLG_Freq->SetDataBase(pDB);
    m_pObjList_PLG_NoFreq->SetDataBase(pDB);
    m_pObjList_PT_Freq->SetDataBase(pDB);
    m_pObjList_PT_NoFreq->SetDataBase(pDB);
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
 @param editmode set to true if we are in edit mode
 @author Lucien Schreiber (c) CREALP 2008
 @date 04 November 2008
 *******************************************************************************/
void AttribObjType_PANEL::SetAttributeBtn(int nbfeatures, bool editmode)
{
    m_NbFeaturesSelected = nbfeatures;

    m_AttribBtn->Enable(false);
    m_AddBtnCtrl->Enable(false);
    if (nbfeatures > 0 && editmode == true) {
        m_AttribBtn->Enable(true);
        m_AddBtnCtrl->Enable(true);
    }

    m_WarningMultiFeatureCtrl->SetLabel(wxEmptyString);
    if (nbfeatures > 1 && editmode == true) {
        m_WarningMultiFeatureCtrl->SetLabel(wxString::Format(_("Warning: %d features selected!"), nbfeatures));
    }
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
void AttribObjType_PANEL::SetInfoBtn(int nbfeatures)
{
    if (nbfeatures == 1) {
        m_InfoBtn->Enable(true);
    } else {
        m_InfoBtn->Enable(false);
    }
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
void AttribObjType_PANEL::SetVisibleNotebook(TOC_GENERIC_NAME notebooktype)
{
    switch (notebooktype) {
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
TOC_GENERIC_NAME AttribObjType_PANEL::GetVisibleNotebook()
{
    TOC_GENERIC_NAME myRetVal = TOC_NAME_UNKNOWN;
    switch (m_AttribNotebook->GetSelection()) {
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
/*void AttribObjType_PANEL::OnAttributeBtn (wxCommandEvent & event)
{
	wxCommandEvent evt (tmEVT_ATTRIBUTION_BTN_PRESSED, wxID_ANY);
	m_ParentEvt->GetEventHandler()->QueueEvent(evt.Clone());
}*/



void AttribObjType_PANEL::OnAddBtn(wxCommandEvent &event)
{
    wxCommandEvent evt(tmEVT_ADD_BTN_PRESSED, wxID_ANY);
    m_ParentEvt->GetEventHandler()->QueueEvent(evt.Clone());
}


void AttribObjType_PANEL::OnRemoveBtn(wxCommandEvent &event)
{
    wxCommandEvent evt(tmEVT_REMOVE_BTN_PRESSED, wxID_ANY);
    m_ParentEvt->GetEventHandler()->QueueEvent(evt.Clone());
}

/***************************************************************************//**
 @brief Called when user press Info
 @details This function only send an event of type tmEVT_INFO_BTN_PRESSED
 actually intercepted by the #tmAttributionManager
 @author Lucien Schreiber (c) CREALP 2008
 @date 07 November 2008
 *******************************************************************************/
void AttribObjType_PANEL::OnInfoBtn(wxCommandEvent &event)
{
    wxCommandEvent evt(tmEVT_INFO_BTN_PRESSED, wxID_ANY);
    m_ParentEvt->GetEventHandler()->QueueEvent(evt.Clone());
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
int AttribObjType_PANEL::GetSelectedValues(TOC_GENERIC_NAME panel_name,
                                           wxArrayLong &values,
                                           bool panel_freq)
{
    tmCheckListBoxRank *myList = NULL;
    switch (panel_name) {
        case TOC_NAME_LINES:
            if (panel_freq)
                myList = m_pObjList_L_Freq;
            else
                myList = m_pObjList_L_NoFreq;
            break;

        case TOC_NAME_POINTS:
            if (panel_freq)
                myList = m_pObjList_PT_Freq;
            else
                myList = m_pObjList_PT_NoFreq;
            break;

        case TOC_NAME_LABELS:
            if (panel_freq)
                myList = m_pObjList_PLG_Freq;
            else
                myList = m_pObjList_PLG_NoFreq;
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

    for (unsigned int i = 0; i < myList->GetCount(); i++) {
        if (myList->GetItem(i, myCheckedID, mytemp, myChecked)) {
            if (myChecked == true)
                values.Add(myCheckedID);
        } else
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
void AttribObjType_PANEL::SetSelectedValues(TOC_GENERIC_NAME panel_name,
                                            const wxArrayLong &values,
                                            bool panel_freq)
{
    tmCheckListBoxRank *myList = NULL;
    switch (panel_name) {
        case TOC_NAME_LINES:
            if (panel_freq)
                myList = m_pObjList_L_Freq;
            else
                myList = m_pObjList_L_NoFreq;
            break;

        case TOC_NAME_POINTS:
            if (panel_freq)
                myList = m_pObjList_PT_Freq;
            else
                myList = m_pObjList_PT_NoFreq;
            break;

        case TOC_NAME_LABELS:
            if (panel_freq)
                myList = m_pObjList_PLG_Freq;
            else
                myList = m_pObjList_PLG_NoFreq;
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
    for (unsigned int l = 0; l < myList->GetCount(); l++) {
        myList->GetItem(l, myCheckedID, mytemp, myChecked);
        myChecked = false;
        // search the values
        for (unsigned int v = 0; v < values.GetCount(); v++) {
            if (values.Item(v) == myCheckedID) {
                myChecked = true;
                break;
            }
        }

        myList->SetItemCheck(l, myChecked);
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
    return m_AutoDisplayAttributes;
}

