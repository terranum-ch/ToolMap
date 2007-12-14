/***************************************************************************
							project_def_fields_dlg.cpp
                    Display dialog for adding new field
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

// comment doxygen

#include "project_def_fields_dlg.h"

/******************************  Field List *************************/
ProjectDefFieldList::ProjectDefFieldList(wxWindow * parent, wxWindowID  id, wxSize size) 
	: ListGenReport(parent,id,size)
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
	
}

ProjectDefFieldList::~ProjectDefFieldList()
{

}


void ProjectDefFieldList::OnDoubleClickItem(wxListEvent & event)
{
	wxArrayString myRowData;
	long myIndex = GetSelectedItem();
	ProjectDefLayersEditObjectDlg * myModifiyDialog = new ProjectDefLayersEditObjectDlg(this);
	
	// get the data from the list for selected line
	GetAllDataAsStringArray(myRowData, myIndex);
	
	// put the data in the dialog
	if (DataToList(myModifiyDialog,myRowData))
	{
		EditDataToList(myRowData, myIndex);
	}
	delete myModifiyDialog;	
}









/******************************  Dialog Field definition *************************/
IMPLEMENT_DYNAMIC_CLASS( ProjectDefFieldDlg, wxDialog )



BEGIN_EVENT_TABLE( ProjectDefFieldDlg, wxDialog )
	EVT_CHECKBOX(ID_DLGAFD_CONSTRAIN_VALUES, ProjectDefFieldDlg::OnShowConstrainValues)
	EVT_FLATBUTTON (ID_DLGAFD_VAL_ADD,ProjectDefFieldDlg::OnAddAllowedValue)
END_EVENT_TABLE()


void ProjectDefFieldDlg::OnShowConstrainValues(wxCommandEvent & event)
{
	if (event.IsChecked())
	{
		wxLogDebug(_T("Constrain values is checked"));
		m_DlgAFD_Notebook->Show(TRUE);
	}
	else 
	{
		m_DlgAFD_Notebook->Show(FALSE);
	}

	// fit the window to the minimal size 
	// responding to the check box state
	if (GetSizer())
	{
		GetSizer()->Fit(this);
		GetSizer()->Layout();
   }
	
}


void ProjectDefFieldDlg::OnAddAllowedValue (wxCommandEvent & event)
{
	wxArrayString myDlgValues;
	
	ProjectDefLayersEditObjectDlg * myEditObjDlg = new ProjectDefLayersEditObjectDlg(this);
	myEditObjDlg->SetName(_("Edit allowed value"));
	
	int iLastItemNumber = m_DlgAFD_Coded_Val_List->GetItemCount();
	
	// check if data transfert was OK
	if (m_DlgAFD_Coded_Val_List->DataToList(myEditObjDlg, myDlgValues))
	{
		// put data to the list
		m_DlgAFD_Coded_Val_List->EditDataToList(myDlgValues);
	}
	
	delete myEditObjDlg;
	
}

ProjectDefFieldDlg::ProjectDefFieldDlg()
{
    Init();
}

ProjectDefFieldDlg::ProjectDefFieldDlg( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, caption, pos, size, style);
}


bool ProjectDefFieldDlg::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
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


ProjectDefFieldDlg::~ProjectDefFieldDlg()
{

}


void ProjectDefFieldDlg::Init()
{
	////@begin ProjectDefFieldDlg member initialisation
    m_DlgAFD_Field_Def = NULL;
    m_DlgAFD_Field_Precision = NULL;
    m_DlgAFD_Field_Scale = NULL;
    m_DlgAFD_Result = NULL;
    m_DlgAFD_Notebook = NULL;
    m_DlgAFD_Panel_Coded_Values = NULL;
    m_DlgAFD_Coded_Val_List = NULL;
    m_DlgAFD_Default_Val = NULL;
    m_DlgAFD_Panel_Range = NULL;
    m_DlgAFD_Range_Default = NULL;
    m_DlgAFD_Range_Min = NULL;
    m_DlgAFD_Range_Max = NULL;
	////@end ProjectDefFieldDlg member initialisation
}


/*!
 * Control creation for wxDialog
 */

void ProjectDefFieldDlg::CreateControls()
{    
	////@begin ProjectDefFieldDlg content construction
    ProjectDefFieldDlg* itemDialog1 = this;
	
    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);
	
    wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer3, 0, wxGROW|wxALL, 5);
	
    wxStaticText* itemStaticText4 = new wxStaticText( itemDialog1, wxID_STATIC, _("Field name :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer3->Add(itemStaticText4, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    m_DlgAFD_Field_Def = new wxTextCtrl( itemDialog1, ID_DLGAFD_FIELD_DEF, _T(""), wxDefaultPosition, wxSize(200, -1), 0 );
    itemBoxSizer3->Add(m_DlgAFD_Field_Def, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    wxStaticBox* itemStaticBoxSizer6Static = new wxStaticBox(itemDialog1, wxID_ANY, _("Field properties"));
    wxStaticBoxSizer* itemStaticBoxSizer6 = new wxStaticBoxSizer(itemStaticBoxSizer6Static, wxVERTICAL);
    itemBoxSizer2->Add(itemStaticBoxSizer6, 0, wxGROW|wxALL, 5);
	
    wxFlexGridSizer* itemFlexGridSizer7 = new wxFlexGridSizer(4, 2, 0, 0);
    itemFlexGridSizer7->AddGrowableCol(1);
    itemStaticBoxSizer6->Add(itemFlexGridSizer7, 0, wxGROW, 5);
	
    wxStaticText* itemStaticText8 = new wxStaticText( itemDialog1, wxID_STATIC, _("Data Type :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer7->Add(itemStaticText8, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    wxArrayString itemChoice9Strings;
    itemChoice9Strings.Add(_("Integer"));
    itemChoice9Strings.Add(_("Float"));
    itemChoice9Strings.Add(_("Text"));
    itemChoice9Strings.Add(_("Date"));
    wxChoice* itemChoice9 = new wxChoice( itemDialog1, ID_DLGAFD_DATA_TYPE, wxDefaultPosition, wxDefaultSize, itemChoice9Strings, 0 );
    itemChoice9->SetStringSelection(_("Text"));
    itemFlexGridSizer7->Add(itemChoice9, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    wxStaticText* itemStaticText10 = new wxStaticText( itemDialog1, wxID_STATIC, _("Field precision :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer7->Add(itemStaticText10, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    m_DlgAFD_Field_Precision = new wxSpinCtrl( itemDialog1, ID_DLGAFD_FIELD_PRECISION, _T("0"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100, 0 );
    itemFlexGridSizer7->Add(m_DlgAFD_Field_Precision, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    wxStaticText* itemStaticText12 = new wxStaticText( itemDialog1, wxID_STATIC, _("Field scale :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer7->Add(itemStaticText12, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    m_DlgAFD_Field_Scale = new wxSpinCtrl( itemDialog1, ID_DLGAFD_FIELD_SCALE, _T("0"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100, 0 );
    itemFlexGridSizer7->Add(m_DlgAFD_Field_Scale, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    wxStaticText* itemStaticText14 = new wxStaticText( itemDialog1, wxID_STATIC, _("Result sample :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer7->Add(itemStaticText14, 0, wxALIGN_LEFT|wxALIGN_TOP|wxALL, 5);
	
    m_DlgAFD_Result = new wxTextCtrl( itemDialog1, ID_DLGAFD_RESULT, _T(""), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE );
    itemFlexGridSizer7->Add(m_DlgAFD_Result, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    wxCheckBox* itemCheckBox16 = new wxCheckBox( itemDialog1, ID_DLGAFD_ORIENTATION_FIELD, _("Orientation field"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox16->SetValue(false);
    itemFlexGridSizer7->Add(itemCheckBox16, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);
	
    wxCheckBox* itemCheckBox17 = new wxCheckBox( itemDialog1, ID_DLGAFD_CONSTRAIN_VALUES, _("Constrain allowed values"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox17->SetValue(false);
    itemBoxSizer2->Add(itemCheckBox17, 0, wxGROW|wxALL, 5);
	
    m_DlgAFD_Notebook = new wxNotebook( itemDialog1, ID_DLGAFD_NOTEBOOK, wxDefaultPosition, wxDefaultSize, wxBK_DEFAULT );
    m_DlgAFD_Notebook->Show(false);
	
    m_DlgAFD_Panel_Coded_Values = new wxPanel( m_DlgAFD_Notebook, ID_DLGAFD_PANEL_CODED_VALUES, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxStaticBox* itemStaticBoxSizer20Static = new wxStaticBox(m_DlgAFD_Panel_Coded_Values, wxID_ANY, _("Allowed values"));
    wxStaticBoxSizer* itemStaticBoxSizer20 = new wxStaticBoxSizer(itemStaticBoxSizer20Static, wxVERTICAL);
    m_DlgAFD_Panel_Coded_Values->SetSizer(itemStaticBoxSizer20);
	
    m_DlgAFD_Coded_Val_List = new ProjectDefFieldList( m_DlgAFD_Panel_Coded_Values, ID_DLGAFD_CODED_VAL_LIST,  wxSize(100, 200) );
    itemStaticBoxSizer20->Add(m_DlgAFD_Coded_Val_List, 1, wxGROW|wxLEFT|wxRIGHT, 5);
	
    wxBoxSizer* itemBoxSizer22 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer20->Add(itemBoxSizer22, 0, wxGROW|wxALL, 5);
    wxStaticText* itemStaticText23 = new wxStaticText( m_DlgAFD_Panel_Coded_Values, wxID_STATIC, _("Default value :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer22->Add(itemStaticText23, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    wxArrayString m_DlgAFD_Default_ValStrings;
    m_DlgAFD_Default_Val = new wxChoice( m_DlgAFD_Panel_Coded_Values, ID_DLGAFD_DEFAULT_VAL, wxDefaultPosition, wxDefaultSize, m_DlgAFD_Default_ValStrings, 0 );
    itemBoxSizer22->Add(m_DlgAFD_Default_Val, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    wxBoxSizer* itemBoxSizer25 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer20->Add(itemBoxSizer25, 0, wxGROW|wxALL, 5);
    wxFlatButton* itemFlatButton26 = new wxFlatButton( m_DlgAFD_Panel_Coded_Values, ID_DLGAFD_VAL_ADD, wxFLATBUTTON_TEXT_ADD);
    itemBoxSizer25->Add(itemFlatButton26, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxTOP|wxBOTTOM, 5);
	
    wxFlatButton* itemFlatButton27 = new wxFlatButton( m_DlgAFD_Panel_Coded_Values, ID_DLGAFD_VAL_REMOVE, wxFLATBUTTON_TEXT_REMOVE);
    itemBoxSizer25->Add(itemFlatButton27, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxTOP|wxBOTTOM, 5);
	
    wxFlatButton* itemFlatButton28 = new wxFlatButton( m_DlgAFD_Panel_Coded_Values, ID_DLGAFD_VAL_IMPORT, _("Import..."),  wxDefaultSize );
    itemBoxSizer25->Add(itemFlatButton28, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxTOP|wxBOTTOM, 5);
	
    wxFlatButton* itemFlatButton29 = new wxFlatButton( m_DlgAFD_Panel_Coded_Values, ID_DLGAFD_VAL_EXPORT, _("Export..."),  wxDefaultSize );
    itemBoxSizer25->Add(itemFlatButton29, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxTOP|wxBOTTOM, 5);
	
    m_DlgAFD_Notebook->AddPage(m_DlgAFD_Panel_Coded_Values, _("Coded values"));
	
    m_DlgAFD_Panel_Range = new wxPanel( m_DlgAFD_Notebook, ID_DLGAFD_PANEL_RANGE, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxFlexGridSizer* itemFlexGridSizer31 = new wxFlexGridSizer(3, 2, 0, 0);
    itemFlexGridSizer31->AddGrowableCol(1);
    m_DlgAFD_Panel_Range->SetSizer(itemFlexGridSizer31);
	
    wxStaticText* itemStaticText32 = new wxStaticText( m_DlgAFD_Panel_Range, wxID_STATIC, _("Default value :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer31->Add(itemStaticText32, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    m_DlgAFD_Range_Default = new wxTextCtrl( m_DlgAFD_Panel_Range, ID_DLGAFD_RANGE_DEFAULT, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer31->Add(m_DlgAFD_Range_Default, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    wxStaticText* itemStaticText34 = new wxStaticText( m_DlgAFD_Panel_Range, wxID_STATIC, _("Minimum value :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer31->Add(itemStaticText34, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    m_DlgAFD_Range_Min = new wxTextCtrl( m_DlgAFD_Panel_Range, ID_DLGAFD_RANGE_MIN, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer31->Add(m_DlgAFD_Range_Min, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    wxStaticText* itemStaticText36 = new wxStaticText( m_DlgAFD_Panel_Range, wxID_STATIC, _("Maximum value :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer31->Add(itemStaticText36, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    m_DlgAFD_Range_Max = new wxTextCtrl( m_DlgAFD_Panel_Range, ID_DLGAFD_RANGE_MAX, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer31->Add(m_DlgAFD_Range_Max, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    m_DlgAFD_Notebook->AddPage(m_DlgAFD_Panel_Range, _("Range"));
	
    itemBoxSizer2->Add(m_DlgAFD_Notebook, 1, wxGROW|wxLEFT|wxRIGHT, 5);
	
    wxStdDialogButtonSizer* itemStdDialogButtonSizer38 = new wxStdDialogButtonSizer;
	
    itemBoxSizer2->Add(itemStdDialogButtonSizer38, 0, wxALIGN_RIGHT|wxALL, 5);
    wxButton* itemButton39 = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer38->AddButton(itemButton39);
	
    wxButton* itemButton40 = new wxButton( itemDialog1, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer38->AddButton(itemButton40);
	
    itemStdDialogButtonSizer38->Realize();
}

