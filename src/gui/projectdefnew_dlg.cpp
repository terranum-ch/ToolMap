/***************************************************************************
							projectdefnew_dlg.cpp
					New Project creation first dialog
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


#include "projectdefnew_dlg.h"


IMPLEMENT_DYNAMIC_CLASS( ProjectDefNew, wxDialog )




BEGIN_EVENT_TABLE( ProjectDefNew, wxDialog )
	EVT_IDLE(ProjectDefNew::OnIdleWait)
END_EVENT_TABLE()


/***************************************************************************//**
 @brief Called during idle time
 @details Use the ProjectDefNew::CheckIdleRules() function for checking that the
 path and name aren't empty. Otherwise we discard the Continue button
 @author Lucien Schreiber (c) CREALP 2008
 @date 19 May 2008
 *******************************************************************************/
void ProjectDefNew::OnIdleWait(wxIdleEvent & event)
{
	if (!CheckIdleRules())
	{
		m_DlgPd_Button_Ok->Enable(FALSE);
	}
	else
		m_DlgPd_Button_Ok->Enable(TRUE);
}



ProjectDefNew::ProjectDefNew()
{
    Init();
}


ProjectDefNew::ProjectDefNew( wxWindow* parent,
							  PrjDefMemManage * PrjDefinition,
							 wxWindowID id,
							 const wxString& caption, const wxPoint& pos,
							 const wxSize& size, long style )
{
    Init();
	
	m_PrjDefinition = PrjDefinition;
	
    Create(parent, id, caption, pos, size, style);
}



bool ProjectDefNew::Create( wxWindow* parent, wxWindowID id, const wxString& caption,
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



ProjectDefNew::~ProjectDefNew()
{

}




void ProjectDefNew::Init()
{
	m_PrjDefinition = NULL;
	
	m_DlgPD_Proj_Path = NULL;
	m_DlgPd_Proj_Name = NULL;
	m_DlgPd_Proj_Unit = NULL;
	m_DlgPd_Proj_Projection = NULL;
	m_DlgPd_Button_Ok = NULL;
	
}

bool ProjectDefNew::TransferDataFromWindow()
{
	m_PrjDefinition->m_PrjName = m_DlgPd_Proj_Name->GetValue();
	m_PrjDefinition->m_PrjPath = m_DlgPD_Proj_Path->GetPath();
	m_PrjDefinition->m_PrjUnitType = (PRJDEF_UNIT_TYPE) m_DlgPd_Proj_Unit->GetSelection();
	m_PrjDefinition->m_PrjProjType = (PRJDEF_PROJ_TYPE) m_DlgPd_Proj_Projection->GetSelection();
	return TRUE;
}


bool ProjectDefNew::CheckIdleRules()
{
	// check the rules to be verified during the Idle time

	// project name
	if (m_DlgPd_Proj_Name->GetValue().IsEmpty())
		return FALSE;
	
	// project path
	if (!wxDirExists(m_DlgPD_Proj_Path->GetPath()))
		return FALSE;
	
	return TRUE; // check passed
}



/***************************************************************************//**
 @brief Create the controls
 @author Lucien Schreiber (c) CREALP 2008
 @date 16 May 2008
 *******************************************************************************/
void ProjectDefNew::CreateControls()
{    

    ProjectDefNew* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxFlexGridSizer* itemFlexGridSizer3 = new wxFlexGridSizer(4, 2, 0, 0);
    itemFlexGridSizer3->AddGrowableCol(1);
    itemBoxSizer2->Add(itemFlexGridSizer3, 0, wxGROW|wxALL, 5);
	
	wxStaticText* itemStaticText7 = new wxStaticText( itemDialog1, wxID_STATIC,
													 _("Project path :"), wxDefaultPosition, 
													 wxDefaultSize, 0 );
    itemFlexGridSizer3->Add(itemStaticText7, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_DlgPD_Proj_Path = new wxDirPickerCtrlBest(itemDialog1, ID_DLGPD_PROJ_PATH_NEW, wxEmptyString,
												_("Select the database folder"));

	///@bug alignement bug, only under mac ??
	// error with mac... alignement bug ???
#if defined(__WXMAC__)
	itemFlexGridSizer3->Add(m_DlgPD_Proj_Path, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL,0);
#else
	itemFlexGridSizer3->Add(m_DlgPD_Proj_Path, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL,5);
#endif
	
	wxStaticText* itemStaticText6 = new wxStaticText( itemDialog1, wxID_STATIC, _("Project name :"), 
													 wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer3->Add(itemStaticText6, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    m_DlgPd_Proj_Name = new wxTextCtrl( itemDialog1, ID_DLGPD_PROJ_NAME_NEW,
									   _T(""), wxDefaultPosition, wxSize(300,-1), 0 );
	m_DlgPd_Proj_Name->SetMaxLength(50);
    itemFlexGridSizer3->Add(m_DlgPd_Proj_Name, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
	wxStaticText* itemStaticText11 = new wxStaticText( itemDialog1, wxID_STATIC, _("Units"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer3->Add(itemStaticText11, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
 	m_DlgPd_Proj_Unit = new wxChoice( itemDialog1, ID_DLGPD_PROJ_UNIT_NEW, wxDefaultPosition,
									 wxDefaultSize,PRJDEF_UNIT_TYPE_NUMBER , PRJDEF_UNIT_TYPE_STRING);
	m_DlgPd_Proj_Unit->SetSelection(UNIT_METERS);
	itemFlexGridSizer3->Add(m_DlgPd_Proj_Unit, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    wxStaticText* itemStaticText13 = new wxStaticText( itemDialog1, wxID_STATIC, _("Projection :"), 
													  wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer3->Add(itemStaticText13, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
	m_DlgPd_Proj_Projection = new wxChoice( itemDialog1, ID_DLGPD_PROJ_PROJECTION_NEW,
										   wxDefaultPosition, wxDefaultSize,
										   PRJDEF_PROJ_TYPE_NUMBER,PRJDEF_PROJ_TYPE_STRING);
	m_DlgPd_Proj_Projection->SetSelection(PROJ_SWISSPROJ);
    itemFlexGridSizer3->Add(m_DlgPd_Proj_Projection, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
	
	

    itemBoxSizer2->Add(5, 5, 1, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxStaticLine* itemStaticLine9 = new wxStaticLine( itemDialog1, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
    itemBoxSizer2->Add(itemStaticLine9, 0, wxGROW|wxALL, 5);

    wxStdDialogButtonSizer* itemStdDialogButtonSizer10 = new wxStdDialogButtonSizer;

    itemBoxSizer2->Add(itemStdDialogButtonSizer10, 0, wxALIGN_RIGHT|wxALL, 5);
    m_DlgPd_Button_Ok = new wxButton( itemDialog1, wxID_OK, _("&Create new project"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer10->AddButton(m_DlgPd_Button_Ok);
	m_DlgPd_Button_Ok->SetDefault();

    wxButton* itemButton12 = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer10->AddButton(itemButton12);

    itemStdDialogButtonSizer10->Realize();
	
	
	// validators
	m_DlgPd_Proj_Name->SetValidator( tmValidator(tmFILTER_EXCLUDE_CHAR_DATABASE));

}


