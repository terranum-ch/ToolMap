///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jan 23 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "wizard_import_data.h"

///////////////////////////////////////////////////////////////////////////

tmWizardImport::tmWizardImport( wxWindow* parent, wxWindowID id, const wxString& title, const wxBitmap& bitmap, const wxPoint& pos, long style ) 
{
	this->Create( parent, id, title, bitmap, pos, style );
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxWizardPageSimple* m_wizPageFileType = new wxWizardPageSimple( this );
	m_pages.Add( m_wizPageFileType );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText1 = new wxStaticText( m_wizPageFileType, wxID_ANY, wxT("Select the type of file"), wxDefaultPosition, wxSize( -1,25 ), 0 );
	m_staticText1->Wrap( -1 );
	m_staticText1->SetFont( wxFont( 13, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );
	
	bSizer1->Add( m_staticText1, 0, wxALL, 5 );
	
	m_radioBtnSHP = new wxRadioButton( m_wizPageFileType, wxID_ANY, wxT("ESRI's shapefile"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1->Add( m_radioBtnSHP, 0, wxALL, 5 );
	
	m_radioBtnCSV = new wxRadioButton( m_wizPageFileType, wxID_ANY, wxT("CSV file"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1->Add( m_radioBtnCSV, 0, wxALL, 5 );
	
	
	m_wizPageFileType->SetSizer( bSizer1 );
	m_wizPageFileType->Layout();
	bSizer1->Fit( m_wizPageFileType );
	wxWizardPageSimple* m_wizPageFilePath = new wxWizardPageSimple( this );
	m_pages.Add( m_wizPageFilePath );
	
	m_sizerFilePath = new wxBoxSizer( wxVERTICAL );
	
	m_staticText3 = new wxStaticText( m_wizPageFilePath, wxID_ANY, wxT("Select the file"), wxDefaultPosition, wxSize( -1,25 ), 0 );
	m_staticText3->Wrap( -1 );
	m_staticText3->SetFont( wxFont( 13, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );
	
	m_sizerFilePath->Add( m_staticText3, 0, wxALL, 5 );
	
	m_filePicker = new wxFilePickerCtrl( m_wizPageFilePath, wxID_ANY, wxEmptyString, wxT("Select a file"), wxT("*.*"), wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE );
	m_sizerFilePath->Add( m_filePicker, 0, wxALL|wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer1;
	sbSizer1 = new wxStaticBoxSizer( new wxStaticBox( m_wizPageFilePath, wxID_ANY, wxT("File information") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer1->SetFlexibleDirection( wxBOTH );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_infoLabelCtrl1 = new wxStaticText( sbSizer1->GetStaticBox(), wxID_ANY, wxT("MyLabel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_infoLabelCtrl1->Wrap( -1 );
	fgSizer1->Add( m_infoLabelCtrl1, 0, wxALL, 5 );
	
	m_infoValueCtrl1 = new wxStaticText( sbSizer1->GetStaticBox(), wxID_ANY, wxT("MyLabel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_infoValueCtrl1->Wrap( -1 );
	fgSizer1->Add( m_infoValueCtrl1, 0, wxALL, 5 );
	
	m_infoLabelCtrl2 = new wxStaticText( sbSizer1->GetStaticBox(), wxID_ANY, wxT("MyLabel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_infoLabelCtrl2->Wrap( -1 );
	fgSizer1->Add( m_infoLabelCtrl2, 0, wxALL, 5 );
	
	m_infoValueCtrl2 = new wxStaticText( sbSizer1->GetStaticBox(), wxID_ANY, wxT("MyLabel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_infoValueCtrl2->Wrap( -1 );
	fgSizer1->Add( m_infoValueCtrl2, 0, wxALL, 5 );
	
	
	sbSizer1->Add( fgSizer1, 0, 0, 5 );
	
	
	m_sizerFilePath->Add( sbSizer1, 0, wxEXPAND|wxALL, 5 );
	
	
	m_wizPageFilePath->SetSizer( m_sizerFilePath );
	m_wizPageFilePath->Layout();
	m_sizerFilePath->Fit( m_wizPageFilePath );
	wxWizardPageSimple* m_wizPageXY = new wxWizardPageSimple( this );
	m_pages.Add( m_wizPageXY );
	
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText9 = new wxStaticText( m_wizPageXY, wxID_ANY, wxT("Select X/Y columns"), wxDefaultPosition, wxSize( -1,25 ), 0 );
	m_staticText9->Wrap( -1 );
	m_staticText9->SetFont( wxFont( 13, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );
	
	bSizer3->Add( m_staticText9, 0, wxALL, 5 );
	
	wxStaticBoxSizer* sbSizer2;
	sbSizer2 = new wxStaticBoxSizer( new wxStaticBox( m_wizPageXY, wxID_ANY, wxT("Coordinates in file") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer2;
	fgSizer2 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer2->AddGrowableCol( 1 );
	fgSizer2->SetFlexibleDirection( wxHORIZONTAL );
	fgSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText11 = new wxStaticText( sbSizer2->GetStaticBox(), wxID_ANY, wxT("X:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText11->Wrap( -1 );
	fgSizer2->Add( m_staticText11, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxArrayString m_choiceXChoices;
	m_choiceX = new wxChoice( sbSizer2->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceXChoices, 0 );
	m_choiceX->SetSelection( 0 );
	fgSizer2->Add( m_choiceX, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_staticText12 = new wxStaticText( sbSizer2->GetStaticBox(), wxID_ANY, wxT("Y:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText12->Wrap( -1 );
	fgSizer2->Add( m_staticText12, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxArrayString m_choiceYChoices;
	m_choiceY = new wxChoice( sbSizer2->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceYChoices, 0 );
	m_choiceY->SetSelection( 0 );
	fgSizer2->Add( m_choiceY, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	sbSizer2->Add( fgSizer2, 1, wxEXPAND, 5 );
	
	
	bSizer3->Add( sbSizer2, 0, wxALL|wxEXPAND, 5 );
	
	
	m_wizPageXY->SetSizer( bSizer3 );
	m_wizPageXY->Layout();
	bSizer3->Fit( m_wizPageXY );
	wxWizardPageSimple* m_wizPageTarget = new wxWizardPageSimple( this );
	m_pages.Add( m_wizPageTarget );
	
	wxBoxSizer* bSizer31;
	bSizer31 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText91 = new wxStaticText( m_wizPageTarget, wxID_ANY, wxT("Select target"), wxDefaultPosition, wxSize( -1,25 ), 0 );
	m_staticText91->Wrap( -1 );
	m_staticText91->SetFont( wxFont( 13, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );
	
	bSizer31->Add( m_staticText91, 0, wxALL, 5 );
	
	wxBoxSizer* bSizer41;
	bSizer41 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText101 = new wxStaticText( m_wizPageTarget, wxID_ANY, wxT("Import into:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText101->Wrap( -1 );
	bSizer41->Add( m_staticText101, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxArrayString m_choiceTargetChoices;
	m_choiceTarget = new wxChoice( m_wizPageTarget, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceTargetChoices, 0 );
	m_choiceTarget->SetSelection( 0 );
	bSizer41->Add( m_choiceTarget, 1, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	bSizer31->Add( bSizer41, 0, wxEXPAND|wxTOP|wxBOTTOM, 5 );
	
	
	m_wizPageTarget->SetSizer( bSizer31 );
	m_wizPageTarget->Layout();
	bSizer31->Fit( m_wizPageTarget );
	wxWizardPageSimple* m_wizPageLayer = new wxWizardPageSimple( this );
	m_pages.Add( m_wizPageLayer );
	
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText13 = new wxStaticText( m_wizPageLayer, wxID_ANY, wxT("Select layer"), wxDefaultPosition, wxSize( -1,25 ), 0 );
	m_staticText13->Wrap( -1 );
	m_staticText13->SetFont( wxFont( 13, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );
	
	bSizer5->Add( m_staticText13, 0, wxALL, 5 );
	
	wxArrayString m_choiceLayerChoices;
	m_choiceLayer = new wxChoice( m_wizPageLayer, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceLayerChoices, 0 );
	m_choiceLayer->SetSelection( 0 );
	bSizer5->Add( m_choiceLayer, 0, wxALL|wxEXPAND, 5 );
	
	
	m_wizPageLayer->SetSizer( bSizer5 );
	m_wizPageLayer->Layout();
	bSizer5->Fit( m_wizPageLayer );
	wxWizardPageSimple* m_wizPageKindField = new wxWizardPageSimple( this );
	m_pages.Add( m_wizPageKindField );
	
	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText15 = new wxStaticText( m_wizPageKindField, wxID_ANY, wxT("Select object kind field in file"), wxDefaultPosition, wxSize( -1,25 ), 0 );
	m_staticText15->Wrap( -1 );
	m_staticText15->SetFont( wxFont( 13, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );
	
	bSizer9->Add( m_staticText15, 0, wxALL, 5 );
	
	wxArrayString m_choiceKindChoices;
	m_choiceKind = new wxChoice( m_wizPageKindField, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceKindChoices, 0 );
	m_choiceKind->SetSelection( 0 );
	bSizer9->Add( m_choiceKind, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText18 = new wxStaticText( m_wizPageKindField, wxID_ANY, wxT("OR"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText18->Wrap( -1 );
	bSizer9->Add( m_staticText18, 0, wxALL, 5 );
	
	m_checkBoxAllSameKind = new wxCheckBox( m_wizPageKindField, wxID_ANY, wxT("Assign all objects to the same kind"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer9->Add( m_checkBoxAllSameKind, 0, wxALL, 5 );
	
	
	m_wizPageKindField->SetSizer( bSizer9 );
	m_wizPageKindField->Layout();
	bSizer9->Fit( m_wizPageKindField );
	wxWizardPageSimple* m_wizPageKindMatch = new wxWizardPageSimple( this );
	m_pages.Add( m_wizPageKindMatch );
	
	wxBoxSizer* bSizer10;
	bSizer10 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText16 = new wxStaticText( m_wizPageKindMatch, wxID_ANY, wxT("Object kinds matching"), wxDefaultPosition, wxSize( -1,25 ), 0 );
	m_staticText16->Wrap( -1 );
	m_staticText16->SetFont( wxFont( 13, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );
	
	bSizer10->Add( m_staticText16, 0, wxALL, 5 );
	
	m_scrolledWindow3 = new wxScrolledWindow( m_wizPageKindMatch, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	m_scrolledWindow3->SetScrollRate( 5, 5 );
	m_fgSizerKinds = new wxFlexGridSizer( 0, 2, 0, 0 );
	m_fgSizerKinds->AddGrowableCol( 1 );
	m_fgSizerKinds->SetFlexibleDirection( wxBOTH );
	m_fgSizerKinds->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	
	m_scrolledWindow3->SetSizer( m_fgSizerKinds );
	m_scrolledWindow3->Layout();
	m_fgSizerKinds->Fit( m_scrolledWindow3 );
	bSizer10->Add( m_scrolledWindow3, 1, wxEXPAND, 5 );
	
	
	m_wizPageKindMatch->SetSizer( bSizer10 );
	m_wizPageKindMatch->Layout();
	bSizer10->Fit( m_wizPageKindMatch );
	wxWizardPageSimple* m_wizPageAttributes = new wxWizardPageSimple( this );
	m_pages.Add( m_wizPageAttributes );
	
	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText14 = new wxStaticText( m_wizPageAttributes, wxID_ANY, wxT("Attributes matching"), wxDefaultPosition, wxSize( -1,25 ), 0 );
	m_staticText14->Wrap( -1 );
	m_staticText14->SetFont( wxFont( 13, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );
	
	bSizer6->Add( m_staticText14, 0, wxALL, 5 );
	
	m_scrolledWindow1 = new wxScrolledWindow( m_wizPageAttributes, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	m_scrolledWindow1->SetScrollRate( 5, 5 );
	m_fgSizerAttributes = new wxFlexGridSizer( 0, 2, 0, 0 );
	m_fgSizerAttributes->AddGrowableCol( 1 );
	m_fgSizerAttributes->SetFlexibleDirection( wxBOTH );
	m_fgSizerAttributes->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	
	m_scrolledWindow1->SetSizer( m_fgSizerAttributes );
	m_scrolledWindow1->Layout();
	m_fgSizerAttributes->Fit( m_scrolledWindow1 );
	bSizer6->Add( m_scrolledWindow1, 1, wxEXPAND, 5 );
	
	
	m_wizPageAttributes->SetSizer( bSizer6 );
	m_wizPageAttributes->Layout();
	bSizer6->Fit( m_wizPageAttributes );
	wxWizardPageSimple* m_wizPageEnums = new wxWizardPageSimple( this );
	m_pages.Add( m_wizPageEnums );
	
	wxBoxSizer* bSizer61;
	bSizer61 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText141 = new wxStaticText( m_wizPageEnums, wxID_ANY, wxT("Enumerations matching"), wxDefaultPosition, wxSize( -1,25 ), 0 );
	m_staticText141->Wrap( -1 );
	m_staticText141->SetFont( wxFont( 13, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );
	
	bSizer61->Add( m_staticText141, 0, wxALL, 5 );
	
	m_scrolledWindow2 = new wxScrolledWindow( m_wizPageEnums, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	m_scrolledWindow2->SetScrollRate( 5, 5 );
	m_sizerEnums = new wxBoxSizer( wxVERTICAL );
	
	
	m_scrolledWindow2->SetSizer( m_sizerEnums );
	m_scrolledWindow2->Layout();
	m_sizerEnums->Fit( m_scrolledWindow2 );
	bSizer61->Add( m_scrolledWindow2, 1, wxEXPAND, 5 );
	
	
	m_wizPageEnums->SetSizer( bSizer61 );
	m_wizPageEnums->Layout();
	bSizer61->Fit( m_wizPageEnums );
	
	this->Centre( wxBOTH );
	
	for ( unsigned int i = 1; i < m_pages.GetCount(); i++ )
	{
		m_pages.Item( i )->SetPrev( m_pages.Item( i - 1 ) );
		m_pages.Item( i - 1 )->SetNext( m_pages.Item( i ) );
	}
	
	// Connect Events
	this->Connect( wxID_ANY, wxEVT_WIZARD_FINISHED, wxWizardEventHandler( tmWizardImport::OnWizardFinished ) );
	m_filePicker->Connect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( tmWizardImport::OnFileChanged ), NULL, this );
	m_checkBoxAllSameKind->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( tmWizardImport::ToggleKindFieldSelection ), NULL, this );
}

tmWizardImport::~tmWizardImport()
{
	// Disconnect Events
	this->Disconnect( wxID_ANY, wxEVT_WIZARD_FINISHED, wxWizardEventHandler( tmWizardImport::OnWizardFinished ) );
	m_filePicker->Disconnect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( tmWizardImport::OnFileChanged ), NULL, this );
	m_checkBoxAllSameKind->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( tmWizardImport::ToggleKindFieldSelection ), NULL, this );
	
	m_pages.Clear();
}
