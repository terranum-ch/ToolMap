/***************************************************************************
								objectattribution_dlg.cpp
					Display the dialog called when user press the
					Project->Edit->Object definition menu
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

#include "objectattribution_dlg.h"

IMPLEMENT_DYNAMIC_CLASS( ProjectEditObjectDefinitionDLG, wxDialog )


BEGIN_EVENT_TABLE( ProjectEditObjectDefinitionDLG, wxDialog )
	EVT_FLATBUTTON (ID_DLGPEO_BTN_ADD, ProjectEditObjectDefinitionDLG::OnAddObject)
	EVT_CHECKBOX (ID_DLGPEO_LINE_FRQ, ProjectEditObjectDefinitionDLG::OnChangeFrequency)
END_EVENT_TABLE()

/**************** EVENT FUNCTION ***********************************/
void ProjectEditObjectDefinitionDLG::OnAddObject (wxCommandEvent & event)
{
	// get the selected panel and call addItem for selected list
	int iSelectedPage = m_DLGPEO_Notebook->GetSelection();
	switch (iSelectedPage)
	{
		case LAYER_POINT:
			m_DLGPEO_List_Point->AddItem();
			break;
		case LAYER_POLYGON:
			m_DLGPEO_List_Poly->AddItem();
			break;
		default:
			m_DLGPEO_List_Line->AddItem();
			break;
	}
}


void ProjectEditObjectDefinitionDLG::OnChangeFrequency (wxCommandEvent & event)
{
	wxArrayLong mySelectedItems;
	int iFreq = 0;
	
	// get selected items
	if (m_DLGPEO_List_Line->GetAllSelectedItem(mySelectedItems) > 0)
	{
		// if checked
		if (m_DLGPEO_Choice_Lyr_Line_Freq->Get3StateValue() == wxCHK_CHECKED)
		{
			iFreq = OBJECT_FREQUENT ;
		}
		else
		{
			iFreq = OBJECT_LESS_FREQUENT;
		}
		
		m_DLGPEO_List_Line->SetFreqStatus(iFreq, &mySelectedItems);
	}
	
}


/**************** PUBLIC FUNCTIONS ***********************************/
ProjectEditObjectDefinitionDLG::ProjectEditObjectDefinitionDLG()
{
    Init();
}


ProjectEditObjectDefinitionDLG::ProjectEditObjectDefinitionDLG( wxWindow* parent, 
															   DataBaseTM * pDatabase,
															   wxWindowID id, 
															   const wxString& caption, 
															   const wxPoint& pos, 
															   const wxSize& size, long style )
{
    Init();
	
	// pass DB values
	m_DB = pDatabase;
	
    Create(parent, id, caption, pos, size, style);
	
	PostInit();
}


bool ProjectEditObjectDefinitionDLG::Create( wxWindow* parent,
											wxWindowID id, const wxString& caption, 
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


void ProjectEditObjectDefinitionDLG::PostInit()
{
	// load data into the choice controls and 
	// enable panel where choice item != 0
	bool bLine = SetChoiceListText(m_DLGPEO_Choice_Lyr_Line_Name, LAYER_LINE);
	bool bPoint = SetChoiceListText(m_DLGPEO_Choice_Lyr_Point_Name, LAYER_POINT);
	bool bPoly = SetChoiceListText(m_DLGPEO_Choice_Lyr_Poly_Name, LAYER_POLYGON);
	
	// activate or not the panel
	m_DLGPEO_Panel_Line->Enable(bLine);
	m_DLGPEO_Panel_Point->Enable(bPoint);
	m_DLGPEO_Panel_Poly->Enable(bPoly);
	
	// pass controls pointer to the list
	m_DLGPEO_List_Line->SetListCtrls(m_DLGPEO_Choice_Lyr_Line_Freq);
		
}


ProjectEditObjectDefinitionDLG::~ProjectEditObjectDefinitionDLG()
{
}


void ProjectEditObjectDefinitionDLG::Init()
{
    m_DLGPEO_Notebook = NULL;
    m_DLGPEO_Panel_Line = NULL;
    m_DLGPEO_List_Line = NULL;
    m_DLGPEO_Choice_Lyr_Line_Name = NULL;
    m_DLGPEO_Choice_Lyr_Line_Freq = NULL;
    m_DLGPEO_Panel_Point = NULL;
    m_DLGPEO_List_Line = NULL;
    m_DLGPEO_Choice_Lyr_Point_Name = NULL;
    m_DLGPEO_Panel_Poly = NULL;
    m_DLGPEO_List_Line = NULL;
    m_DLGPEO_Choice_Lyr_Poly_Name = NULL;
	m_DB = NULL;
}


void ProjectEditObjectDefinitionDLG::CreateControls()
{    
    ProjectEditObjectDefinitionDLG* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    m_DLGPEO_Notebook = new wxNotebook( itemDialog1, ID_DLGPEO_NOTEBOOK, wxDefaultPosition, wxDefaultSize, wxBK_DEFAULT );

    m_DLGPEO_Panel_Line = new wxPanel( m_DLGPEO_Notebook, ID_DLGPEO_PANEL_LINE, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer5 = new wxBoxSizer(wxVERTICAL);
    m_DLGPEO_Panel_Line->SetSizer(itemBoxSizer5);

   
	// size and cols name of the list
	wxArrayString mylistcolname;
	mylistcolname.Add(_("Code"));
	mylistcolname.Add(_("Description"));
	mylistcolname.Add(_("Layer name"));
	mylistcolname.Add(_("Frequency"));
	
	wxArrayInt mylistWidth;
	mylistWidth.Add(80);
	mylistWidth.Add(200);
	mylistWidth.Add(120);
	mylistWidth.Add(100);
	
	wxArrayString mylistcolname2;
	mylistcolname2.Add(mylistcolname.Item(0));
	mylistcolname2.Add(mylistcolname.Item(1));
	mylistcolname2.Add(mylistcolname.Item(2));
	
	wxArrayInt mylistWidth2;
	mylistWidth2.Add(mylistWidth.Item(0));
	mylistWidth2.Add(mylistWidth.Item(1)+50);
	mylistWidth2.Add(mylistWidth.Item(2)+50);
	

	// LIST FOR LINES
	m_DLGPEO_List_Line = new ObjectDefinitionList( m_DLGPEO_Panel_Line, ID_DLGPEO_LISTLINE,
												  LAYER_LINE, m_DB,
										   &mylistcolname, &mylistWidth, wxSize (500,260) );
	//m_DLGPEO_List_Line->SetWindowStyleFlag(wxLC_REPORT | wxLC_SINGLE_SEL);
    itemBoxSizer5->Add(m_DLGPEO_List_Line, 1, wxGROW|wxALL, 5);

    
	wxFlexGridSizer* itemFlexGridSizer7 = new wxFlexGridSizer(1, 2, 0, 0);
    itemFlexGridSizer7->AddGrowableCol(1);
    itemBoxSizer5->Add(itemFlexGridSizer7, 0, wxGROW|wxALL, 5);
    wxStaticText* itemStaticText8 = new wxStaticText( m_DLGPEO_Panel_Line, wxID_STATIC, _("Layer name :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer7->Add(itemStaticText8, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

    //wxArrayString m_DLGPEO_Choice_Lyr_Line_NameStrings;
    //m_DLGPEO_Choice_Lyr_Line_NameStrings.Add(_("Polygons_TS"));
    m_DLGPEO_Choice_Lyr_Line_Name = new wxChoice( m_DLGPEO_Panel_Line, ID_DLGPEO_LYR_NAME_LINE, wxDefaultPosition, wxDefaultSize); //, m_DLGPEO_Choice_Lyr_Line_NameStrings, 0 );
   // m_DLGPEO_Choice_Lyr_Line_Name->SetStringSelection(_("Polygons_TS"));
    itemFlexGridSizer7->Add(m_DLGPEO_Choice_Lyr_Line_Name, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

	
    m_DLGPEO_Choice_Lyr_Line_Freq = new wxCheckBox( m_DLGPEO_Panel_Line, ID_DLGPEO_LINE_FRQ, _("This object is frequently used"), wxDefaultPosition, wxDefaultSize, wxCHK_3STATE);
    m_DLGPEO_Choice_Lyr_Line_Freq->SetValue(false);
    itemBoxSizer5->Add(m_DLGPEO_Choice_Lyr_Line_Freq, 0, wxGROW|wxALL, 5);
	
    m_DLGPEO_Notebook->AddPage(m_DLGPEO_Panel_Line, _("Lines"));
	

    m_DLGPEO_Panel_Point = new wxPanel( m_DLGPEO_Notebook, ID_DLGPEO_PANEL_POINT, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer13 = new wxBoxSizer(wxVERTICAL);
    m_DLGPEO_Panel_Point->SetSizer(itemBoxSizer13);

	// LIST FOR POINT
	m_DLGPEO_List_Point = new ObjectDefinitionList( m_DLGPEO_Panel_Point, ID_DLGPEO_LISTPOINT,
												   LAYER_POINT, m_DB,
										   &mylistcolname2, &mylistWidth2 );
    itemBoxSizer13->Add(m_DLGPEO_List_Point, 1, wxGROW|wxALL, 5);

    
	wxFlexGridSizer* itemFlexGridSizer15 = new wxFlexGridSizer(1, 2, 0, 0);
    itemFlexGridSizer15->AddGrowableCol(1);
    itemBoxSizer13->Add(itemFlexGridSizer15, 0, wxGROW|wxALL, 5);
    wxStaticText* itemStaticText16 = new wxStaticText( m_DLGPEO_Panel_Point, wxID_STATIC, _("Layer name :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer15->Add(itemStaticText16, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

   // wxArrayString m_DLGPEO_Choice_Lyr_Point_NameStrings;
   // m_DLGPEO_Choice_Lyr_Point_NameStrings.Add(_("Polygons_TS"));
    m_DLGPEO_Choice_Lyr_Point_Name = new wxChoice( m_DLGPEO_Panel_Point, ID_DLGPEO_LYR_NAME_POINT, wxDefaultPosition, wxDefaultSize); //, m_DLGPEO_Choice_Lyr_Point_NameStrings, 0 );
    m_DLGPEO_Choice_Lyr_Point_Name->SetStringSelection(_("Polygons_TS"));
    itemFlexGridSizer15->Add(m_DLGPEO_Choice_Lyr_Point_Name, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

    m_DLGPEO_Notebook->AddPage(m_DLGPEO_Panel_Point, _("Points"));

    m_DLGPEO_Panel_Poly = new wxPanel( m_DLGPEO_Notebook, ID_DLGPEO_PANEL_POLY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer19 = new wxBoxSizer(wxVERTICAL);
    m_DLGPEO_Panel_Poly->SetSizer(itemBoxSizer19);

	// LIST FOR POLY
    m_DLGPEO_List_Poly = new ObjectDefinitionList( m_DLGPEO_Panel_Poly, ID_DLGPEO_LISTPOLY,
												  LAYER_POLYGON,m_DB,
										   &mylistcolname2, & mylistWidth2);
    itemBoxSizer19->Add(m_DLGPEO_List_Poly, 1, wxGROW|wxALL, 5);

    wxFlexGridSizer* itemFlexGridSizer21 = new wxFlexGridSizer(1, 2, 0, 0);
    itemFlexGridSizer21->AddGrowableCol(1);
    itemBoxSizer19->Add(itemFlexGridSizer21, 0, wxGROW|wxALL, 5);
    wxStaticText* itemStaticText22 = new wxStaticText( m_DLGPEO_Panel_Poly, wxID_STATIC, _("Layer name :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer21->Add(itemStaticText22, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

    //wxArrayString m_DLGPEO_Choice_Lyr_Poly_NameStrings;
    //m_DLGPEO_Choice_Lyr_Poly_NameStrings.Add(_("Polygons_TS"));
    m_DLGPEO_Choice_Lyr_Poly_Name = new wxChoice( m_DLGPEO_Panel_Poly, ID_DLGPEO_LYR_NAME_POLY, wxDefaultPosition, wxDefaultSize); //, m_DLGPEO_Choice_Lyr_Poly_NameStrings, 0 );
    m_DLGPEO_Choice_Lyr_Poly_Name->SetStringSelection(_("Polygons_TS"));
    itemFlexGridSizer21->Add(m_DLGPEO_Choice_Lyr_Poly_Name, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

    m_DLGPEO_Notebook->AddPage(m_DLGPEO_Panel_Poly, _("Polygons"));

    itemBoxSizer2->Add(m_DLGPEO_Notebook, 1, wxGROW|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer24Static = new wxStaticBox(itemDialog1, wxID_ANY, _("Operations"));
    wxStaticBoxSizer* itemStaticBoxSizer24 = new wxStaticBoxSizer(itemStaticBoxSizer24Static, wxHORIZONTAL);
    itemBoxSizer2->Add(itemStaticBoxSizer24, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);

    wxFlatButton* itemToggleButton25 = new wxFlatButton( itemDialog1, ID_DLGPEO_BTN_ADD, _("+"),  wxSize(40, -1));
    itemStaticBoxSizer24->Add(itemToggleButton25, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxFlatButton* itemToggleButton26 = new wxFlatButton( itemDialog1, ID_DLGPEO_BTN_DEL, _("-"),  wxSize(40, -1));
    itemStaticBoxSizer24->Add(itemToggleButton26, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxFlatButton* itemToggleButton27 = new wxFlatButton( itemDialog1, ID_DLGPEO_BTN_IMPORT, _("Import..."),  wxDefaultSize);
    itemStaticBoxSizer24->Add(itemToggleButton27, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxFlatButton* itemToggleButton28 = new wxFlatButton( itemDialog1, ID_DLGPEO_BTN_EXPORT, _("Export..."),  wxDefaultSize );
    itemStaticBoxSizer24->Add(itemToggleButton28, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticLine* itemStaticLine29 = new wxStaticLine( itemDialog1, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
    itemBoxSizer2->Add(itemStaticLine29, 0, wxGROW|wxALL, 5);

    wxStdDialogButtonSizer* itemStdDialogButtonSizer30 = new wxStdDialogButtonSizer;

    itemBoxSizer2->Add(itemStdDialogButtonSizer30, 0, wxALIGN_RIGHT|wxALL, 5);
    wxButton* itemButton31 = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer30->AddButton(itemButton31);

    wxButton* itemButton32 = new wxButton( itemDialog1, wxID_SAVE, _("&Save"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer30->AddButton(itemButton32);
	SetAffirmativeId(wxID_SAVE);

    itemStdDialogButtonSizer30->Realize();
}


// PRIVATE DATABASE FUNCTION
bool  ProjectEditObjectDefinitionDLG::SetChoiceListText (wxChoice * choice,
														 int listtype)
{
	wxArrayString myThematicResult; 
//	wxString sSentence = wxString::Format(_T("SELECT (LAYER_NAME) FROM %s WHERE TYPE_CD = %d"),
//										  table.c_str(), listtype);
//	if (m_DB->DataBaseQuery(sSentence))
//	{
//		myThematicResult = m_DB->DataBaseGetNextResult();
//		
		myThematicResult = m_DB->GetLayerNameByType(listtype);
		// append item only if they are items !
		if (myThematicResult.GetCount() > 0)
		{
			choice->Append(myThematicResult);
			return TRUE;
		}
		return FALSE;
		
}


							  
