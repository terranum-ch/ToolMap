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
	
	m_radioBtn1 = new wxRadioButton( m_wizPageFileType, wxID_ANY, wxT("ESRI's shapefile"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1->Add( m_radioBtn1, 0, wxALL, 5 );
	
	m_radioBtn2 = new wxRadioButton( m_wizPageFileType, wxID_ANY, wxT("CSV file"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1->Add( m_radioBtn2, 0, wxALL, 5 );
	
	
	m_wizPageFileType->SetSizer( bSizer1 );
	m_wizPageFileType->Layout();
	bSizer1->Fit( m_wizPageFileType );
	wxWizardPageSimple* m_wizPageFile = new wxWizardPageSimple( this );
	m_pages.Add( m_wizPageFile );
	
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText3 = new wxStaticText( m_wizPageFile, wxID_ANY, wxT("Select the file"), wxDefaultPosition, wxSize( -1,25 ), 0 );
	m_staticText3->Wrap( -1 );
	m_staticText3->SetFont( wxFont( 13, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );
	
	bSizer2->Add( m_staticText3, 0, wxALL, 5 );
	
	m_filePicker2 = new wxFilePickerCtrl( m_wizPageFile, wxID_ANY, wxEmptyString, wxT("Select a file"), wxT("*.*"), wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE );
	bSizer2->Add( m_filePicker2, 0, wxALL|wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer1;
	sbSizer1 = new wxStaticBoxSizer( new wxStaticBox( m_wizPageFile, wxID_ANY, wxT("File information") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer1->SetFlexibleDirection( wxBOTH );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_fileInfoProp1 = new wxStaticText( sbSizer1->GetStaticBox(), wxID_ANY, wxT("MyLabel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_fileInfoProp1->Wrap( -1 );
	fgSizer1->Add( m_fileInfoProp1, 0, wxALL, 5 );
	
	m_fileInfoVal1 = new wxStaticText( sbSizer1->GetStaticBox(), wxID_ANY, wxT("MyLabel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_fileInfoVal1->Wrap( -1 );
	fgSizer1->Add( m_fileInfoVal1, 0, wxALL, 5 );
	
	m_fileInfoProp2 = new wxStaticText( sbSizer1->GetStaticBox(), wxID_ANY, wxT("MyLabel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_fileInfoProp2->Wrap( -1 );
	fgSizer1->Add( m_fileInfoProp2, 0, wxALL, 5 );
	
	m_fileInfoVal2 = new wxStaticText( sbSizer1->GetStaticBox(), wxID_ANY, wxT("MyLabel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_fileInfoVal2->Wrap( -1 );
	fgSizer1->Add( m_fileInfoVal2, 0, wxALL, 5 );
	
	
	sbSizer1->Add( fgSizer1, 0, 0, 5 );
	
	
	bSizer2->Add( sbSizer1, 0, wxEXPAND|wxALL, 5 );
	
	
	m_wizPageFile->SetSizer( bSizer2 );
	m_wizPageFile->Layout();
	bSizer2->Fit( m_wizPageFile );
	wxWizardPageSimple* m_wizPage4 = new wxWizardPageSimple( this );
	m_pages.Add( m_wizPage4 );
	
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText9 = new wxStaticText( m_wizPage4, wxID_ANY, wxT("Select geometry"), wxDefaultPosition, wxSize( -1,25 ), 0 );
	m_staticText9->Wrap( -1 );
	m_staticText9->SetFont( wxFont( 13, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );
	
	bSizer3->Add( m_staticText9, 0, wxALL, 5 );
	
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText10 = new wxStaticText( m_wizPage4, wxID_ANY, wxT("Import into:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText10->Wrap( -1 );
	bSizer4->Add( m_staticText10, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxString m_choice1Choices[] = { wxT("Points"), wxT("Labels") };
	int m_choice1NChoices = sizeof( m_choice1Choices ) / sizeof( wxString );
	m_choice1 = new wxChoice( m_wizPage4, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choice1NChoices, m_choice1Choices, 0 );
	m_choice1->SetSelection( 0 );
	bSizer4->Add( m_choice1, 1, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	bSizer3->Add( bSizer4, 0, wxEXPAND|wxTOP|wxBOTTOM, 5 );
	
	wxStaticBoxSizer* sbSizer2;
	sbSizer2 = new wxStaticBoxSizer( new wxStaticBox( m_wizPage4, wxID_ANY, wxT("Coordinates in file") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer2;
	fgSizer2 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer2->SetFlexibleDirection( wxHORIZONTAL );
	fgSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText11 = new wxStaticText( sbSizer2->GetStaticBox(), wxID_ANY, wxT("X:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText11->Wrap( -1 );
	fgSizer2->Add( m_staticText11, 0, wxALL, 5 );
	
	wxArrayString m_choiceXChoices;
	m_choiceX = new wxChoice( sbSizer2->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceXChoices, 0 );
	m_choiceX->SetSelection( 0 );
	fgSizer2->Add( m_choiceX, 1, wxALL|wxEXPAND, 5 );
	
	m_staticText12 = new wxStaticText( sbSizer2->GetStaticBox(), wxID_ANY, wxT("Y:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText12->Wrap( -1 );
	fgSizer2->Add( m_staticText12, 0, wxALL, 5 );
	
	wxArrayString m_choiceYChoices;
	m_choiceY = new wxChoice( sbSizer2->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceYChoices, 0 );
	m_choiceY->SetSelection( 0 );
	fgSizer2->Add( m_choiceY, 0, wxALL, 5 );
	
	
	sbSizer2->Add( fgSizer2, 1, wxEXPAND, 5 );
	
	
	bSizer3->Add( sbSizer2, 0, wxALL|wxEXPAND, 5 );
	
	
	m_wizPage4->SetSizer( bSizer3 );
	m_wizPage4->Layout();
	bSizer3->Fit( m_wizPage4 );
	wxWizardPageSimple* m_wizPageLayer = new wxWizardPageSimple( this );
	m_pages.Add( m_wizPageLayer );
	
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText13 = new wxStaticText( m_wizPageLayer, wxID_ANY, wxT("Select thematic layer"), wxDefaultPosition, wxSize( -1,25 ), 0 );
	m_staticText13->Wrap( -1 );
	m_staticText13->SetFont( wxFont( 13, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );
	
	bSizer5->Add( m_staticText13, 0, wxALL, 5 );
	
	wxArrayString m_choice4Choices;
	m_choice4 = new wxChoice( m_wizPageLayer, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choice4Choices, 0 );
	m_choice4->SetSelection( 0 );
	bSizer5->Add( m_choice4, 0, wxALL|wxEXPAND, 5 );
	
	
	m_wizPageLayer->SetSizer( bSizer5 );
	m_wizPageLayer->Layout();
	bSizer5->Fit( m_wizPageLayer );
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
	m_fgSizerAttributes->SetFlexibleDirection( wxBOTH );
	m_fgSizerAttributes->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_textAttribute = new wxStaticText( m_scrolledWindow1, wxID_ANY, wxT("MyLabel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_textAttribute->Wrap( -1 );
	m_fgSizerAttributes->Add( m_textAttribute, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxArrayString m_choiceAttributeChoices;
	m_choiceAttribute = new wxChoice( m_scrolledWindow1, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceAttributeChoices, 0 );
	m_choiceAttribute->SetSelection( 0 );
	m_fgSizerAttributes->Add( m_choiceAttribute, 1, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
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
	
	m_scrolledWindow11 = new wxScrolledWindow( m_wizPageEnums, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	m_scrolledWindow11->SetScrollRate( 5, 5 );
	m_fgSizerEnums = new wxFlexGridSizer( 0, 2, 0, 0 );
	m_fgSizerEnums->SetFlexibleDirection( wxBOTH );
	m_fgSizerEnums->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_textEnum = new wxStaticText( m_scrolledWindow11, wxID_ANY, wxT("MyLabel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_textEnum->Wrap( -1 );
	m_fgSizerEnums->Add( m_textEnum, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxArrayString m_choiceEnumChoices;
	m_choiceEnum = new wxChoice( m_scrolledWindow11, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceEnumChoices, 0 );
	m_choiceEnum->SetSelection( 0 );
	m_fgSizerEnums->Add( m_choiceEnum, 1, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	m_scrolledWindow11->SetSizer( m_fgSizerEnums );
	m_scrolledWindow11->Layout();
	m_fgSizerEnums->Fit( m_scrolledWindow11 );
	bSizer61->Add( m_scrolledWindow11, 1, wxEXPAND, 5 );
	
	
	m_wizPageEnums->SetSizer( bSizer61 );
	m_wizPageEnums->Layout();
	bSizer61->Fit( m_wizPageEnums );
	
	this->Centre( wxBOTH );
	
	for ( unsigned int i = 1; i < m_pages.GetCount(); i++ )
	{
		m_pages.Item( i )->SetPrev( m_pages.Item( i - 1 ) );
		m_pages.Item( i - 1 )->SetNext( m_pages.Item( i ) );
	}
}

tmWizardImport::~tmWizardImport()
{
	m_pages.Clear();
}
