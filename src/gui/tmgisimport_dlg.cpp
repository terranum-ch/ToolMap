/***************************************************************************
								tmgisimport_dlg.cpp
                    Dialog for importing gis data into project
                             -------------------
    copyright            : (C) 2009 CREALP Lucien Schreiber 
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

#include "tmgisimport_dlg.h"


BEGIN_EVENT_TABLE(tmGISImport_DLG, wxDialog)
	EVT_FILEPICKER_CHANGED(ID_DLGIG_IMPORT_BTN, tmGISImport_DLG::OnChangeFile)
END_EVENT_TABLE()


tmGISImport_DLG::tmGISImport_DLG( wxWindow* parent, tmGISImport * import, wxWindowID id, 
								 const wxString& title, const wxPoint& pos,
								 const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	m_Import = import;
	wxASSERT(m_Import);
	
	CreateCtrls(parent);
	GI_Enable(false);
}



tmGISImport_DLG::~tmGISImport_DLG()
{
}


void tmGISImport_DLG::GI_Enable(bool enable)
{
	m_ImportChoice->Enable(enable);
	m_Btn_Import->Enable(enable);
}



void tmGISImport_DLG::GI_SetInfo (TM_GIS_SPATIAL_TYPES type)
{
	wxString myPolyText = TM_GIS_SPATIAL_TYPES_STRING[LAYER_SPATIAL_POLYGON];
	myPolyText.Append(_(", importing not allowed"));
	
	wxString label[] = {TM_GIS_SPATIAL_TYPES_STRING[LAYER_SPATIAL_LINE],
	TM_GIS_SPATIAL_TYPES_STRING[LAYER_SPATIAL_POINT],
	myPolyText, _("Unable to acces file")};
	
	
	// error with file, file not open
	if (type == LAYER_SPATIAL_UNKNOWN)
	{
		m_Type_Value->SetLabel(label[3]);
		m_FeatureCount_Type->SetLabel(_T("0"));
		return;
	}
	else
	{
		m_Type_Value->SetLabel(label[type]);
		m_FeatureCount_Type->SetLabel(wxString::Format(_T("%d"), m_Import->GetFeatureCount()));
	}
}



void tmGISImport_DLG::GI_SetRedColor (bool red)
{
	if (red)
		m_Type_Value->SetForegroundColour(*wxRED);
	else
		m_Type_Value->SetForegroundColour(wxNullColour);
}


void tmGISImport_DLG::GI_SetChoice (bool clear)
{
	m_ImportChoice->Clear();
	if (clear)
		return;
	
	
	for (int i = 0; i<= (int) TOC_NAME_FRAME; i++)
	{
		if (m_Import->IsImportIntoAllowed((TOC_GENERIC_NAME)i))
			m_ImportChoice->Append(TOC_GENERIC_NAME_STRING[i]);
	}
	m_ImportChoice->Select(0);
	
}


void tmGISImport_DLG::OnChangeFile (wxFileDirPickerEvent & event)
{
	wxASSERT(m_Import);
	
	// getting import info
	bool bIsOpen = m_Import->Open(event.GetPath());
	TM_GIS_SPATIAL_TYPES type = LAYER_SPATIAL_UNKNOWN;
	bool bIsImportAllowed = false;
	
	if (bIsOpen)
	{
		bIsImportAllowed = m_Import->IsImportAllowed();
		type = m_Import->GetSpatialType();
	}
	
	// updating dialog
	if (bIsOpen && bIsImportAllowed)
	{
		GI_Enable(true);
		GI_SetRedColor(false);
		GI_SetChoice(false);
	}
	else
	{
		GI_Enable(false);
		GI_SetRedColor(true);
		GI_SetChoice(true);
	}
	GI_SetInfo(type);

	wxLogDebug(event.GetPath());
	
	
}


TOC_GENERIC_NAME tmGISImport_DLG::GetImportLayer()
{
	wxString myChoice = m_ImportChoice->GetString(m_ImportChoice->GetSelection());
	for (int i = 0; i<= (int) TOC_NAME_FRAME;i++)
	{
		if (myChoice == TOC_GENERIC_NAME_STRING[i])
			return (TOC_GENERIC_NAME) i;
	}
	
	return TOC_NAME_UNKNOWN;
}


void tmGISImport_DLG::CreateCtrls (wxWindow * parent)
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer32;
	bSizer32 = new wxBoxSizer( wxVERTICAL );
	
	bSizer32->SetMinSize( wxSize( 500,-1 ) ); 
	wxBoxSizer* bSizer33;
	bSizer33 = new wxBoxSizer( wxHORIZONTAL );
	
	wxStaticText* m_staticText13;
	m_staticText13 = new wxStaticText( this, wxID_ANY, _("File Name :"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText13->Wrap( -1 );
	bSizer33->Add( m_staticText13, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxFilePickerCtrl* m_filePicker1;
	m_filePicker1 = new wxFilePickerCtrl( this, ID_DLGIG_IMPORT_BTN, wxEmptyString, _("Select a file"),
										 tmGISDATA_VECTOR_TYPE_WILDCARDS[0], wxDefaultPosition, wxDefaultSize,
										 wxFLP_OPEN|wxFLP_USE_TEXTCTRL );
	bSizer33->Add( m_filePicker1, 1, wxALL, 5 );
	
	bSizer32->Add( bSizer33, 0, wxEXPAND, 5 );
	
	wxStaticBoxSizer* staticsizer;
	staticsizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("File informations") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer2;
	fgSizer2 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer2->SetFlexibleDirection( wxBOTH );
	fgSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxStaticText* m_Type_Label;
	m_Type_Label = new wxStaticText( this, wxID_ANY, _("Geometry type :"), wxDefaultPosition, wxDefaultSize, 0 );
	m_Type_Label->Wrap( -1 );
	fgSizer2->Add( m_Type_Label, 0, wxALL, 5 );
	
	m_Type_Value = new wxStaticText( this, wxID_ANY, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
	m_Type_Value->Wrap( -1 );
	
	fgSizer2->Add( m_Type_Value, 0, wxALL, 5 );
	
	wxStaticText* m_FeatureCount_Label;
	m_FeatureCount_Label = new wxStaticText( this, wxID_ANY, _("Number of features :"), wxDefaultPosition, wxDefaultSize, 0 );
	m_FeatureCount_Label->Wrap( -1 );
	fgSizer2->Add( m_FeatureCount_Label, 0, wxALL, 5 );
	
	m_FeatureCount_Type = new wxStaticText( this, wxID_ANY, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
	m_FeatureCount_Type->Wrap( -1 );
	fgSizer2->Add( m_FeatureCount_Type, 0, wxALL, 5 );
	
	staticsizer->Add( fgSizer2, 1, wxEXPAND, 5 );
	
	bSizer32->Add( staticsizer, 1,  wxLEFT | wxRIGHT | wxEXPAND, 5 );
	
	wxBoxSizer* bSizer34;
	bSizer34 = new wxBoxSizer( wxHORIZONTAL );
	
	wxStaticText* m_staticText18;
	m_staticText18 = new wxStaticText( this, wxID_ANY, _("Import into :"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText18->Wrap( -1 );
	bSizer34->Add( m_staticText18, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxArrayString m_ImportChoiceChoices;
	m_ImportChoice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_ImportChoiceChoices, 0 );
	m_ImportChoice->SetSelection( 0 );
	bSizer34->Add( m_ImportChoice, 1, wxALL|wxEXPAND, 5 );
	
	bSizer32->Add( bSizer34, 0, wxEXPAND, 5 );
	
	wxStaticLine* m_staticline4;
	m_staticline4 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer32->Add( m_staticline4, 0, wxEXPAND | wxALL, 5 );
	
	wxBoxSizer* bSizer35;
	bSizer35 = new wxBoxSizer( wxHORIZONTAL );
	
	wxButton* m_button28;
	m_button28 = new wxButton( this, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer35->Add( m_button28, 0, wxALL, 5 );
	
	m_Btn_Import = new wxButton( this, wxID_OK, _("&Import"), wxDefaultPosition, wxDefaultSize, 0 );
	m_Btn_Import->SetDefault(); 
	bSizer35->Add( m_Btn_Import, 0, wxALL, 5 );
	
	bSizer32->Add( bSizer35, 0, wxALIGN_RIGHT, 5 );
	
	this->SetSizer( bSizer32 );
	this->Layout();
	bSizer32->Fit( this );
	CenterOnParent(wxBOTH);
}

