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
END_EVENT_TABLE()

/**************** EVENT FUNCTION ***********************************/





/**************** PUBLIC FUNCTIONS ***********************************/
ProjectEditObjectDefinitionDLG::ProjectEditObjectDefinitionDLG()
{
    Init();
}


ProjectEditObjectDefinitionDLG::ProjectEditObjectDefinitionDLG( wxWindow* parent, 
															   wxWindowID id, 
															   const wxString& caption, 
															   const wxPoint& pos, 
															   const wxSize& size, long style )
{
    Init();
    Create(parent, id, caption, pos, size, style);
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
	m_DLGPEO_List_Line = new ListGenReport( m_DLGPEO_Panel_Line, ID_DLGPEO_LISTLINE, 
										   &mylistcolname, &mylistWidth, wxSize (500,260) );
    itemBoxSizer5->Add(m_DLGPEO_List_Line, 1, wxGROW|wxALL, 5);

    
	wxFlexGridSizer* itemFlexGridSizer7 = new wxFlexGridSizer(2, 2, 0, 0);
    itemFlexGridSizer7->AddGrowableCol(1);
    itemBoxSizer5->Add(itemFlexGridSizer7, 0, wxGROW|wxALL, 5);
    wxStaticText* itemStaticText8 = new wxStaticText( m_DLGPEO_Panel_Line, wxID_STATIC, _("Layer name :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer7->Add(itemStaticText8, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

    wxArrayString m_DLGPEO_Choice_Lyr_Line_NameStrings;
    m_DLGPEO_Choice_Lyr_Line_NameStrings.Add(_("Polygons_TS"));
    m_DLGPEO_Choice_Lyr_Line_Name = new wxChoice( m_DLGPEO_Panel_Line, ID_DLGPEO_LYR_NAME_LINE, wxDefaultPosition, wxDefaultSize, m_DLGPEO_Choice_Lyr_Line_NameStrings, 0 );
    m_DLGPEO_Choice_Lyr_Line_Name->SetStringSelection(_("Polygons_TS"));
    itemFlexGridSizer7->Add(m_DLGPEO_Choice_Lyr_Line_Name, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

    wxStaticText* itemStaticText10 = new wxStaticText( m_DLGPEO_Panel_Line, wxID_STATIC, _("Frequency :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer7->Add(itemStaticText10, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

    wxArrayString m_DLGPEO_Choice_Lyr_Line_FreqStrings;
    m_DLGPEO_Choice_Lyr_Line_FreqStrings.Add(_("Frequent"));
    m_DLGPEO_Choice_Lyr_Line_FreqStrings.Add(_("Less frequent"));
    m_DLGPEO_Choice_Lyr_Line_Freq = new wxChoice( m_DLGPEO_Panel_Line, ID_DLGPEO_LINE_FRQ, wxDefaultPosition, wxDefaultSize, m_DLGPEO_Choice_Lyr_Line_FreqStrings, 0 );
    m_DLGPEO_Choice_Lyr_Line_Freq->SetStringSelection(_("Frequent"));
    itemFlexGridSizer7->Add(m_DLGPEO_Choice_Lyr_Line_Freq, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

    m_DLGPEO_Notebook->AddPage(m_DLGPEO_Panel_Line, _("Lines"));

    m_DLGPEO_Panel_Point = new wxPanel( m_DLGPEO_Notebook, ID_DLGPEO_PANEL_POINT, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer13 = new wxBoxSizer(wxVERTICAL);
    m_DLGPEO_Panel_Point->SetSizer(itemBoxSizer13);

	// LIST FOR POINT
	m_DLGPEO_List_Point = new ListGenReport( m_DLGPEO_Panel_Point, ID_DLGPEO_LISTPOINT, 
										   &mylistcolname2, &mylistWidth2 );
    itemBoxSizer13->Add(m_DLGPEO_List_Point, 1, wxGROW|wxALL, 5);

    
	wxFlexGridSizer* itemFlexGridSizer15 = new wxFlexGridSizer(1, 2, 0, 0);
    itemFlexGridSizer15->AddGrowableCol(1);
    itemBoxSizer13->Add(itemFlexGridSizer15, 0, wxGROW|wxALL, 5);
    wxStaticText* itemStaticText16 = new wxStaticText( m_DLGPEO_Panel_Point, wxID_STATIC, _("Layer name :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer15->Add(itemStaticText16, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

    wxArrayString m_DLGPEO_Choice_Lyr_Point_NameStrings;
    m_DLGPEO_Choice_Lyr_Point_NameStrings.Add(_("Polygons_TS"));
    m_DLGPEO_Choice_Lyr_Point_Name = new wxChoice( m_DLGPEO_Panel_Point, ID_DLGPEO_LYR_NAME_POINT, wxDefaultPosition, wxDefaultSize, m_DLGPEO_Choice_Lyr_Point_NameStrings, 0 );
    m_DLGPEO_Choice_Lyr_Point_Name->SetStringSelection(_("Polygons_TS"));
    itemFlexGridSizer15->Add(m_DLGPEO_Choice_Lyr_Point_Name, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

    m_DLGPEO_Notebook->AddPage(m_DLGPEO_Panel_Point, _("Points"));

    m_DLGPEO_Panel_Poly = new wxPanel( m_DLGPEO_Notebook, ID_DLGPEO_PANEL_POLY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer19 = new wxBoxSizer(wxVERTICAL);
    m_DLGPEO_Panel_Poly->SetSizer(itemBoxSizer19);

	// LIST FOR POLY
    m_DLGPEO_List_Poly = new ListGenReport( m_DLGPEO_Panel_Poly, ID_DLGPEO_LISTPOLY,
										   &mylistcolname2, & mylistWidth2);
    itemBoxSizer19->Add(m_DLGPEO_List_Poly, 1, wxGROW|wxALL, 5);

    wxFlexGridSizer* itemFlexGridSizer21 = new wxFlexGridSizer(1, 2, 0, 0);
    itemFlexGridSizer21->AddGrowableCol(1);
    itemBoxSizer19->Add(itemFlexGridSizer21, 0, wxGROW|wxALL, 5);
    wxStaticText* itemStaticText22 = new wxStaticText( m_DLGPEO_Panel_Poly, wxID_STATIC, _("Layer name :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer21->Add(itemStaticText22, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

    wxArrayString m_DLGPEO_Choice_Lyr_Poly_NameStrings;
    m_DLGPEO_Choice_Lyr_Poly_NameStrings.Add(_("Polygons_TS"));
    m_DLGPEO_Choice_Lyr_Poly_Name = new wxChoice( m_DLGPEO_Panel_Poly, ID_DLGPEO_LYR_NAME_POLY, wxDefaultPosition, wxDefaultSize, m_DLGPEO_Choice_Lyr_Poly_NameStrings, 0 );
    m_DLGPEO_Choice_Lyr_Poly_Name->SetStringSelection(_("Polygons_TS"));
    itemFlexGridSizer21->Add(m_DLGPEO_Choice_Lyr_Poly_Name, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

    m_DLGPEO_Notebook->AddPage(m_DLGPEO_Panel_Poly, _("Polygons"));

    itemBoxSizer2->Add(m_DLGPEO_Notebook, 1, wxGROW|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer24Static = new wxStaticBox(itemDialog1, wxID_ANY, _("Operations"));
    wxStaticBoxSizer* itemStaticBoxSizer24 = new wxStaticBoxSizer(itemStaticBoxSizer24Static, wxHORIZONTAL);
    itemBoxSizer2->Add(itemStaticBoxSizer24, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);

    wxToggleButton* itemToggleButton25 = new wxToggleButton( itemDialog1, ID_TOGGLEBUTTON8, _("+"), wxDefaultPosition, wxSize(40, -1), 0 );
    itemToggleButton25->SetValue(false);
    itemStaticBoxSizer24->Add(itemToggleButton25, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxToggleButton* itemToggleButton26 = new wxToggleButton( itemDialog1, ID_TOGGLEBUTTON9, _("-"), wxDefaultPosition, wxSize(40, -1), 0 );
    itemToggleButton26->SetValue(false);
    itemStaticBoxSizer24->Add(itemToggleButton26, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxToggleButton* itemToggleButton27 = new wxToggleButton( itemDialog1, ID_TOGGLEBUTTON10, _("Import..."), wxDefaultPosition, wxDefaultSize, 0 );
    itemToggleButton27->SetValue(false);
    itemStaticBoxSizer24->Add(itemToggleButton27, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxToggleButton* itemToggleButton28 = new wxToggleButton( itemDialog1, ID_TOGGLEBUTTON11, _("Export..."), wxDefaultPosition, wxDefaultSize, 0 );
    itemToggleButton28->SetValue(false);
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



