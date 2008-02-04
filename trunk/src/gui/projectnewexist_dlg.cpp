/***************************************************************************
								projectnewexist_dlg.cpp
						Display New Project from existing dialog
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

#include "projectnewexist_dlg.h"


IMPLEMENT_DYNAMIC_CLASS( ProjectNewExistDLG, wxDialog )


BEGIN_EVENT_TABLE( ProjectNewExistDLG, wxDialog )
	EVT_BUTTON (ID_DLGNPE_NEW_PRJ_PATH_BTN,ProjectNewExistDLG::OnButtonPathNew)
	EVT_BUTTON (ID_DLGNPE_OLD_PRJ_PATH_BTN,ProjectNewExistDLG::OnButtonPathOld)
	EVT_IDLE (ProjectNewExistDLG::OnIdleWait)
END_EVENT_TABLE()



void ProjectNewExistDLG::OnButtonPathNew (wxCommandEvent & event)
{
	wxDirDialog myDirDialog (this);
	if( myDirDialog.ShowModal() == wxID_OK)
	{
		m_DlgPNE_New_Prj_Path->SetValue(myDirDialog.GetPath());
	}
}

void ProjectNewExistDLG::OnButtonPathOld (wxCommandEvent & event)
{
	wxDirDialog myDirDialog (this);
	if( myDirDialog.ShowModal() == wxID_OK)
	{
		m_DlgPNE_Old_Prj_Path->SetValue(myDirDialog.GetPath());
	}
}


void ProjectNewExistDLG::OnIdleWait(wxIdleEvent & event)
{
	wxString myNewPath = m_DlgPNE_New_Prj_Path->GetValue();
	wxString myNewName = m_DlgPNE_New_Prj_Name->GetValue();
	wxString myOldPath = m_DlgPNE_Old_Prj_Path->GetValue();
	
	if (wxDir::Exists(myNewPath) && !myNewName.IsEmpty() && wxDir::Exists(myOldPath))
	{
		m_DlgPNE_Btn_OK->Enable(TRUE);
	}
	else
		m_DlgPNE_Btn_OK->Disable();
	
}



ProjectNewExistDLG::ProjectNewExistDLG()
{
    Init();
}

ProjectNewExistDLG::ProjectNewExistDLG( wxWindow* parent, wxWindowID id, 
									   const wxString& caption, const wxPoint& pos, 
									   const wxSize& size, long style )
{
    Init();
    Create(parent, id, caption, pos, size, style);
}




bool ProjectNewExistDLG::Create( wxWindow* parent, wxWindowID id, 
								const wxString& caption, const wxPoint& pos,
								const wxSize& size, long style )
{
	////@begin ProjectNewExistDLG creation
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );
	
    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
	////@end ProjectNewExistDLG creation
    return true;
}



ProjectNewExistDLG::~ProjectNewExistDLG()
{
	////@begin ProjectNewExistDLG destruction
	////@end ProjectNewExistDLG destruction
}


void ProjectNewExistDLG::Init()
{
    m_DlgPNE_New_Prj_Path = NULL;
    m_DlgPNE_New_Prj_Name = NULL;
    m_DlgPNE_Old_Prj_Path = NULL;
	 m_DlgPNE_Options = NULL;
	m_DlgPNE_Btn_OK = NULL;

	
	m_New_Prj_Path = _T("");
	m_New_Prj_Name = _T("");
	m_Old_Prj_Path = _T("");
	m_sOption_Type = 0;
	
}


void ProjectNewExistDLG::CreateControls()
{    
    ProjectNewExistDLG* itemDialog1 = this;
	
    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);
	
    wxStaticBox* itemStaticBoxSizer3Static = new wxStaticBox(itemDialog1, wxID_ANY, _("New project properties"));
    wxStaticBoxSizer* itemStaticBoxSizer3 = new wxStaticBoxSizer(itemStaticBoxSizer3Static, wxVERTICAL);
    itemBoxSizer2->Add(itemStaticBoxSizer3, 0, wxGROW|wxALL, 5);
	
    wxFlexGridSizer* itemFlexGridSizer4 = new wxFlexGridSizer(2, 3, 0, 0);
    itemFlexGridSizer4->AddGrowableCol(1);
    itemStaticBoxSizer3->Add(itemFlexGridSizer4, 0, wxGROW|wxALL, 0);
	
    wxStaticText* itemStaticText5 = new wxStaticText( itemDialog1, wxID_STATIC, _("New project path :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer4->Add(itemStaticText5, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    m_DlgPNE_New_Prj_Path = new wxTextCtrl( itemDialog1, ID_TEXTCTRL15, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer4->Add(m_DlgPNE_New_Prj_Path, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    wxButton* itemButton7 = new wxButton( itemDialog1, ID_DLGNPE_NEW_PRJ_PATH_BTN, _("..."), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer4->Add(itemButton7, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    wxStaticText* itemStaticText8 = new wxStaticText( itemDialog1, wxID_STATIC, _("New project name :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer4->Add(itemStaticText8, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    m_DlgPNE_New_Prj_Name = new wxTextCtrl( itemDialog1, ID_TEXTCTRL16, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer4->Add(m_DlgPNE_New_Prj_Name, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    wxStaticLine* itemStaticLine10 = new wxStaticLine( itemDialog1, ID_STATICLINE1, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
    itemBoxSizer2->Add(itemStaticLine10, 0, wxGROW|wxALL, 5);
	
    wxStaticBox* itemStaticBoxSizer11Static = new wxStaticBox(itemDialog1, wxID_ANY, _("Existing project path"));
    wxStaticBoxSizer* itemStaticBoxSizer11 = new wxStaticBoxSizer(itemStaticBoxSizer11Static, wxVERTICAL);
    itemBoxSizer2->Add(itemStaticBoxSizer11, 0, wxGROW|wxALL, 5);
	
    wxBoxSizer* itemBoxSizer12 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer11->Add(itemBoxSizer12, 0, wxGROW|wxALL, 0);
	
    wxStaticText* itemStaticText13 = new wxStaticText( itemDialog1, wxID_STATIC, _("Existing project path :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer12->Add(itemStaticText13, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    m_DlgPNE_Old_Prj_Path = new wxTextCtrl( itemDialog1, ID_TEXTCTRL17, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer12->Add(m_DlgPNE_Old_Prj_Path, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    wxButton* itemButton15 = new wxButton( itemDialog1, ID_DLGNPE_OLD_PRJ_PATH_BTN, _("..."), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer12->Add(itemButton15, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    wxArrayString m_DlgPNE_OptionsStrings;
    m_DlgPNE_OptionsStrings.Add(_("&Keep only project structure"));
    m_DlgPNE_OptionsStrings.Add(_("&Keep project structure and data"));
    m_DlgPNE_Options = new wxRadioBox( itemDialog1, ID_DLGNPE_RADIO_OPTIONS, _("New project options"), wxDefaultPosition, wxDefaultSize, m_DlgPNE_OptionsStrings, 1, wxRA_SPECIFY_COLS );
    m_DlgPNE_Options->SetSelection(0);
    itemBoxSizer2->Add(m_DlgPNE_Options, 1, wxGROW|wxALL, 5);
	
    wxStdDialogButtonSizer* itemStdDialogButtonSizer17 = new wxStdDialogButtonSizer;
	
    itemBoxSizer2->Add(itemStdDialogButtonSizer17, 0, wxALIGN_RIGHT|wxALL, 5);
    m_DlgPNE_Btn_OK = new wxButton( itemDialog1, wxID_OK, _("&Create new project"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer17->AddButton(m_DlgPNE_Btn_OK);
	
    wxButton* itemButton19 = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer17->AddButton(itemButton19);
	
    itemStdDialogButtonSizer17->Realize();
}



bool ProjectNewExistDLG::TransferDataFromWindow()
{
	m_New_Prj_Path = m_DlgPNE_New_Prj_Path->GetValue();
	m_New_Prj_Name = m_DlgPNE_New_Prj_Name->GetValue();
	m_Old_Prj_Path = m_DlgPNE_Old_Prj_Path->GetValue();
	
	m_sOption_Type = m_DlgPNE_Options->GetSelection();
	return TRUE;
}





