/////////////////////////////////////////////////////////////////////////////
// Name:        editvertexposition.cpp
// Purpose:     
// Author:      Lucien Schreiber
// Modified by: 
// Created:     Mon  8 Oct 11:06:20 2007
// RCS-ID:      
// Copyright:   (c) CREALP 2007
// Licence:     
/////////////////////////////////////////////////////////////////////////////

// Generated by DialogBlocks (Personal Edition), Mon 8 Oct 11:06:20 2007

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

////@begin includes
////@end includes

#include "editvertexposition.h"

////@begin XPM images
////@end XPM images


/*!
 * Editvertexposition type definition
 */

IMPLEMENT_DYNAMIC_CLASS( Editvertexposition, wxDialog )


/*!
 * Editvertexposition event table definition
 */

BEGIN_EVENT_TABLE( Editvertexposition, wxDialog )

////@begin Editvertexposition event table entries
////@end Editvertexposition event table entries

END_EVENT_TABLE()


/*!
 * Editvertexposition constructors
 */

Editvertexposition::Editvertexposition()
{
    Init();
}

Editvertexposition::Editvertexposition( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, caption, pos, size, style);
}


/*!
 * Editvertexposition creator
 */

bool Editvertexposition::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin Editvertexposition creation
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end Editvertexposition creation
    return true;
}


/*!
 * Editvertexposition destructor
 */

Editvertexposition::~Editvertexposition()
{
////@begin Editvertexposition destruction
////@end Editvertexposition destruction
}


/*!
 * Member initialisation
 */

void Editvertexposition::Init()
{
////@begin Editvertexposition member initialisation
////@end Editvertexposition member initialisation
}


/*!
 * Control creation for Editvertexposition
 */

void Editvertexposition::CreateControls()
{    
////@begin Editvertexposition content construction
    // Generated by DialogBlocks, Tue 30 Oct 15:21:20 2007 (Personal Edition)

    Editvertexposition* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxStaticBox* itemStaticBoxSizer3Static = new wxStaticBox(itemDialog1, wxID_ANY, _("Informations"));
    wxStaticBoxSizer* itemStaticBoxSizer3 = new wxStaticBoxSizer(itemStaticBoxSizer3Static, wxVERTICAL);
    itemBoxSizer2->Add(itemStaticBoxSizer3, 0, wxGROW|wxALL, 5);

    wxFlexGridSizer* itemFlexGridSizer4 = new wxFlexGridSizer(2, 2, 0, 0);
    itemFlexGridSizer4->AddGrowableCol(1);
    itemStaticBoxSizer3->Add(itemFlexGridSizer4, 0, wxGROW, 5);

    wxStaticText* itemStaticText5 = new wxStaticText( itemDialog1, wxID_STATIC, _("Layer :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer4->Add(itemStaticText5, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText6 = new wxStaticText( itemDialog1, wxID_STATIC, _("Line"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer4->Add(itemStaticText6, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText7 = new wxStaticText( itemDialog1, wxID_STATIC, _("Selected vertex :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer4->Add(itemStaticText7, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText8 = new wxStaticText( itemDialog1, wxID_STATIC, _("2"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer4->Add(itemStaticText8, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer9Static = new wxStaticBox(itemDialog1, wxID_ANY, _("Vertex position"));
    wxStaticBoxSizer* itemStaticBoxSizer9 = new wxStaticBoxSizer(itemStaticBoxSizer9Static, wxVERTICAL);
    itemBoxSizer2->Add(itemStaticBoxSizer9, 1, wxGROW|wxALL, 5);

    wxGrid* itemGrid10 = new wxGrid( itemDialog1, ID_GRID3, wxDefaultPosition, wxSize(200, 150), wxHSCROLL|wxVSCROLL );
    itemGrid10->SetDefaultColSize(70);
    itemGrid10->SetDefaultRowSize(25);
    itemGrid10->SetColLabelSize(25);
    itemGrid10->SetRowLabelSize(50);
    itemGrid10->CreateGrid(2, 2, wxGrid::wxGridSelectCells);
    itemStaticBoxSizer9->Add(itemGrid10, 1, wxGROW, 5);

    wxBoxSizer* itemBoxSizer11 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer9->Add(itemBoxSizer11, 0, wxALIGN_LEFT|wxALL, 5);

    wxToggleButton* itemToggleButton12 = new wxToggleButton( itemDialog1, ID_TOGGLEBUTTON26, _("+"), wxDefaultPosition, wxSize(40, -1), 0 );
    itemToggleButton12->SetValue(false);
    itemBoxSizer11->Add(itemToggleButton12, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxTOP|wxBOTTOM, 5);

    wxToggleButton* itemToggleButton13 = new wxToggleButton( itemDialog1, ID_TOGGLEBUTTON27, _("-"), wxDefaultPosition, wxSize(40, -1), 0 );
    itemToggleButton13->SetValue(false);
    itemBoxSizer11->Add(itemToggleButton13, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxTOP|wxBOTTOM, 5);

    wxBoxSizer* itemBoxSizer14 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer14, 0, wxALIGN_RIGHT|wxALL, 5);

    wxButton* itemButton15 = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer14->Add(itemButton15, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton16 = new wxButton( itemDialog1, wxID_APPLY, _("&Apply"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer14->Add(itemButton16, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

////@end Editvertexposition content construction
}


/*!
 * Should we show tooltips?
 */

bool Editvertexposition::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap Editvertexposition::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin Editvertexposition bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end Editvertexposition bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon Editvertexposition::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin Editvertexposition icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end Editvertexposition icon retrieval
}