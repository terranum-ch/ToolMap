/***************************************************************************
 projectproperties_dlg.cpp
 Contain description of class used for ProjectPropertiesDLG called 
 when user press Project->Edit->Project Settings menu
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

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif


#include "projectproperties_dlg.h"


/************************************* PROJECT PROPERTIES DIALOG***********************************/
IMPLEMENT_DYNAMIC_CLASS( ProjectPropertiesDLG, wxDialog )


BEGIN_EVENT_TABLE( ProjectPropertiesDLG, wxDialog )
	EVT_BUTTON (wxID_SAVE, ProjectPropertiesDLG::OnSaveButton)
END_EVENT_TABLE()

/********************************** PROJECT PROPERTIES EVENT FUNCTION *****************************/
void ProjectPropertiesDLG::OnSaveButton (wxCommandEvent & event)
{
	// get the data from ctrls
	wxString myBackupPath = m_DLGPS_Backup_Path->GetPath();
	wxString myExportPath = m_DLGPS_Export_Path->GetPath();
	int iSelExportType = m_DLGPS_Export_Type_Choice->GetSelection();
	
	// update path for windows, does nothing if path is unix
	DataBase::DataBaseConvertWindowsPath(myBackupPath);
	DataBase::DataBaseConvertWindowsPath(myExportPath);
	
	// check that the project data is defined 
	// otherwise we have a big problem :-)
	if (m_DBHandler->IsProjectDataDefined())
	{
		// save export and backup path 
		m_DBHandler->SetProjectExportData(iSelExportType, myExportPath);
		m_DBHandler->SetProjectBackupPath(myBackupPath);
		
	}
	else
		wxLogError(_T("Project data not defined : critical error"));
	
	// propagate event for closing the dialog
	event.Skip();
}

/************************************ PROJECT PROPERTIES FUNCTION**********************************/
ProjectPropertiesDLG::ProjectPropertiesDLG()
{
    Init();
}


ProjectPropertiesDLG::ProjectPropertiesDLG( wxWindow* parent,
										   DataBaseTM * database,
										   wxWindowID id, const wxString& caption,
										   const wxPoint& pos, const wxSize& size, long style )
{
    Init();
	m_DBHandler = database;
    Create(parent, id, caption, pos, size, style);
	
	// init the list with scale
	
}


bool ProjectPropertiesDLG::Create( wxWindow* parent, wxWindowID id, const wxString& caption, 
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

ProjectPropertiesDLG::~ProjectPropertiesDLG()
{
}


void ProjectPropertiesDLG::Init()
{
    m_DLGPS_Notebook = NULL;
    m_DLGPS_Export_Type_Choice = NULL;
    m_DLGPS_Export_Path = NULL;
    m_DLGPS_Backup_Path = NULL;
    m_DLGPS_Scale_List = NULL;
    m_DLGPS_Scale_Order_Choice = NULL;
}

void ProjectPropertiesDLG::CreateControls()
{    
    ProjectPropertiesDLG* itemDialog1 = this;
	
    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);
	
    m_DLGPS_Notebook = new wxNotebook( itemDialog1, ID_DLGPS_NOTEBOOK, wxDefaultPosition, wxDefaultSize, wxBK_DEFAULT );
	
    wxPanel* itemPanel4 = new wxPanel( m_DLGPS_Notebook, ID_DLGPS_PANEL_SETTINGS, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer5 = new wxBoxSizer(wxVERTICAL);
    itemPanel4->SetSizer(itemBoxSizer5);
	
    wxStaticBox* itemStaticBoxSizer6Static = new wxStaticBox(itemPanel4, wxID_ANY, _("Export project"));
    wxStaticBoxSizer* itemStaticBoxSizer6 = new wxStaticBoxSizer(itemStaticBoxSizer6Static, wxVERTICAL);
    itemBoxSizer5->Add(itemStaticBoxSizer6, 0, wxGROW|wxALL, 5);
    wxFlexGridSizer* itemFlexGridSizer7 = new wxFlexGridSizer(2, 2, 0, 0);
    itemFlexGridSizer7->AddGrowableCol(1);
    itemStaticBoxSizer6->Add(itemFlexGridSizer7, 1, wxGROW|wxALL, 0);
    wxStaticText* itemStaticText8 = new wxStaticText( itemPanel4, wxID_STATIC, _("Export as :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer7->Add(itemStaticText8, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    wxArrayString m_DLGPS_Export_Type_ChoiceStrings;
    m_DLGPS_Export_Type_ChoiceStrings.Add(PRJDEF_EXPORT_TYPE_STRING[EXPORT_SHAPEFILE]);
	m_DLGPS_Export_Type_ChoiceStrings.Add(PRJDEF_EXPORT_TYPE_STRING[EXPORT_GRAPHIC]);
	m_DLGPS_Export_Type_Choice = new wxChoice( itemPanel4, ID_DLGPS_EXPORT_TYPE_CHOICE, wxDefaultPosition, wxSize(300, -1), m_DLGPS_Export_Type_ChoiceStrings, 0 );
    m_DLGPS_Export_Type_Choice->SetSelection(0);
    itemFlexGridSizer7->Add(m_DLGPS_Export_Type_Choice, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    wxStaticText* itemStaticText10 = new wxStaticText( itemPanel4, wxID_STATIC, _("Export Path :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer7->Add(itemStaticText10, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    m_DLGPS_Export_Path = new wxDirPickerCtrlBest( itemPanel4, ID_DLGPS_EXPORT_PATH, wxEmptyString,
												  _("Select an export path")); 
    itemFlexGridSizer7->Add(m_DLGPS_Export_Path, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    wxStaticBox* itemStaticBoxSizer12Static = new wxStaticBox(itemPanel4, wxID_ANY, _("Incremental backup directory"));
    wxStaticBoxSizer* itemStaticBoxSizer12 = new wxStaticBoxSizer(itemStaticBoxSizer12Static, wxVERTICAL);
    itemBoxSizer5->Add(itemStaticBoxSizer12, 0, wxGROW|wxALL, 5);
    wxBoxSizer* itemBoxSizer13 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer12->Add(itemBoxSizer13, 0, wxGROW|wxALL, 0);
    wxStaticText* itemStaticText14 = new wxStaticText( itemPanel4, wxID_STATIC, _("Path :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer13->Add(itemStaticText14, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
	
    m_DLGPS_Backup_Path = new wxDirPickerCtrlBest( itemPanel4, ID_DLGPS_BACKUP_PATH, wxEmptyString,
												  _("Select a backup folder")); 
    itemBoxSizer13->Add(m_DLGPS_Backup_Path, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    m_DLGPS_Notebook->AddPage(itemPanel4, _("Project settings"));
	
    wxPanel* itemPanel16 = new wxPanel( m_DLGPS_Notebook, ID_DLGPS_PANEL_SCALE, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxStaticBox* itemStaticBoxSizer17Static = new wxStaticBox(itemPanel16, wxID_ANY, _("Define usables scales"));
    wxStaticBoxSizer* itemStaticBoxSizer17 = new wxStaticBoxSizer(itemStaticBoxSizer17Static, wxVERTICAL);
    itemPanel16->SetSizer(itemStaticBoxSizer17);
	
    wxArrayString m_DLGPS_Scale_ListStrings;
    //m_DLGPS_Scale_List = new wxListBox( itemPanel16, ID_DLGPS_SCALE_LIST, wxDefaultPosition, wxDefaultSize, m_DLGPS_Scale_ListStrings, wxLB_SINGLE );
    m_DLGPS_Scale_List = new ScaleList( itemPanel16, ID_DLGPS_SCALE_LIST,
									   m_DBHandler);
	
	
	
	itemStaticBoxSizer17->Add(m_DLGPS_Scale_List, 1, wxGROW|wxALL, 0);
	
    wxBoxSizer* itemBoxSizer19 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer17->Add(itemBoxSizer19, 0, wxGROW|wxALL, 5);
    wxFlatButton* itemFlatButton20 = new wxFlatButton( itemPanel16, ID_DLGPS_BTN_SCALE_ADD, wxFLATBUTTON_TEXT_ADD);
    //itemFlatButton20->SetValue(false);
    itemBoxSizer19->Add(itemFlatButton20, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    wxFlatButton* itemFlatButton21 = new wxFlatButton( itemPanel16, ID_DLGPS_BTN_SCALE_DEL,wxFLATBUTTON_TEXT_REMOVE);
    //itemFlatButton21->SetValue(false);
    itemBoxSizer19->Add(itemFlatButton21, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    wxStaticText* itemStaticText22 = new wxStaticText( itemPanel16, wxID_STATIC, _("Order :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer19->Add(itemStaticText22, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    wxArrayString m_DLGPS_Scale_Order_ChoiceStrings;
    m_DLGPS_Scale_Order_ChoiceStrings.Add(_("User defined"));
    m_DLGPS_Scale_Order_ChoiceStrings.Add(_("Sort ascending"));
    m_DLGPS_Scale_Order_ChoiceStrings.Add(_("Sort descending"));
    m_DLGPS_Scale_Order_Choice = new wxChoice( itemPanel16, ID_DLGPS_SCALE_ORDER_CHOICE, wxDefaultPosition, wxDefaultSize, m_DLGPS_Scale_Order_ChoiceStrings, 0 );
    m_DLGPS_Scale_Order_Choice->SetSelection(0);  //SetStringSelection(_("User defined"));
    itemBoxSizer19->Add(m_DLGPS_Scale_Order_Choice, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    m_DLGPS_Notebook->AddPage(itemPanel16, _("Scale"));
	
    itemBoxSizer2->Add(m_DLGPS_Notebook, 1, wxGROW|wxALL, 5);
	
    wxStaticLine* itemStaticLine24 = new wxStaticLine( itemDialog1, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
    itemBoxSizer2->Add(itemStaticLine24, 0, wxGROW|wxALL, 5);
	
    wxStdDialogButtonSizer* itemStdDialogButtonSizer25 = new wxStdDialogButtonSizer;
	
    itemBoxSizer2->Add(itemStdDialogButtonSizer25, 0, wxALIGN_RIGHT|wxALL, 5);
    wxButton* itemButton26 = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer25->AddButton(itemButton26);
	
    wxButton* itemButton27 = new wxButton( itemDialog1, wxID_SAVE, _("&Save"), wxDefaultPosition, wxDefaultSize, 0 );
    itemButton27->SetDefault();
	SetAffirmativeId(wxID_SAVE);
    itemStdDialogButtonSizer25->AddButton(itemButton27);
	
    itemStdDialogButtonSizer25->Realize();
	
}


/***************************************************************************//**
 @brief Transfert data to dialog
 @details This is called automatically just before displaying the
 #ProjectPropertiesDLG for passing values to the dialog
 @return  TRUE if data transfert OK
 @author Lucien Schreiber (c) CREALP 2007
 @date 28 March 2008
 *******************************************************************************/
bool ProjectPropertiesDLG::TransferDataToWindow()
{
	int iResultflag;
	int iExportType = 0;
	wxString myPath = _T("");

	
	// get the export data
	// get the data from the database
	iResultflag = m_DBHandler->GetProjectExportData(iExportType, myPath);
	// put the data into the dialog
	m_DLGPS_Export_Path->SetPathWithError((PATH_ERROR) iResultflag, myPath);
	m_DLGPS_Export_Type_Choice->SetSelection(iExportType);
	
	// get the backup data
	iResultflag = m_DBHandler->GetProjectBackupPath(myPath);
	m_DLGPS_Backup_Path->SetPathWithError((PATH_ERROR) iResultflag,myPath );
	
	return TRUE;
}


/*************************************** SCALE LIST *****************************************/
BEGIN_EVENT_TABLE( ScaleList, ListGenReportWithDialog )
END_EVENT_TABLE()


ScaleList::ScaleList(wxWindow * parent,
					 wxWindowID id,
					 DataBaseTM * database,
					 wxSize size) : ListGenReportWithDialog(parent, id, size)
{
	m_ChoiceOrder = NULL;
	m_DBHandler = database;
	
	wxArrayString myColName;
	myColName.Add(_("Scale"));
	wxArrayInt myColSize;
	myColSize.Add(300);
	
	CreateColumns(&myColName, &myColSize);
	
	// init list values
	LoadValueIntoList();
	
}



ScaleList::~ScaleList()
{
	
}


void ScaleList::LoadValueIntoList ()
{
	long myScale = 0;
	while (1)
	{
		myScale = m_DBHandler->GetNextScaleValue();
		if (myScale == -1)
			break;
		else
		{
			// adding scale in the list
			SetScaleToList(myScale);
		}

	}
	
}



/***************************************************************************//**
 @brief Set the scale value
 @details Insert a new scale item or change an existing one adding the 1: text
 in front
 of the scale value
 @param lscale the scale value (without the 1:)
 @param index zero based index of the item to modify or -1 if we want to add an
 item (the default)
 @author Lucien Schreiber (c) CREALP 2007
 @date 25 March 2008
 *******************************************************************************/
void ScaleList::SetScaleToList (long lscale, int index)
{
	wxString myScaleText = _T("");
	if(!ScaleTM::GetScaleFromLong(lscale, myScaleText))
		wxLogDebug(_T("Error converting scale"));
	
	// if index == -1 then we add otherwise we insert
	if (index == -1)
		AddItemToList(myScaleText, -1);
	else 
		SetItemText(index, 0, myScaleText);
	
}


/***************************************************************************//**
 @brief Get the scale value
 @details Return the long value of the scale for a specified item (without the
 1:)
 @param index the zero based item index we want the scale for
 @return  the scale value without the 1: text
 @author Lucien Schreiber (c) CREALP 2007
 @date 25 March 2008
 *******************************************************************************/
long ScaleList::GetScaleFromList (int index)
{
	wxString myScaleText = GetItemColText(index, 0);
	return ScaleTM::GetScaleFromString(myScaleText);
}


