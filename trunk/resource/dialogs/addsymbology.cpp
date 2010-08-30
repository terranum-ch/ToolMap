/////////////////////////////////////////////////////////////////////////////
// Name:        addsymbology.cpp
// Purpose:     
// Author:      Lucien Schreiber
// Modified by: 
// Created:     Fri 24 Aug 11:02:21 2007
// RCS-ID:      
// Copyright:   (c) CREALP 2007
// Licence:     
/////////////////////////////////////////////////////////////////////////////

// Generated by DialogBlocks (Personal Edition), Fri 24 Aug 11:02:21 2007

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

#include "addsymbology.h"

////@begin XPM images
////@end XPM images


/*!
 * AddSymbology type definition
 */

IMPLEMENT_DYNAMIC_CLASS( AddSymbology, wxDialog )


/*!
 * AddSymbology event table definition
 */

BEGIN_EVENT_TABLE( AddSymbology, wxDialog )

////@begin AddSymbology event table entries
////@end AddSymbology event table entries

END_EVENT_TABLE()


/*!
 * AddSymbology constructors
 */

AddSymbology::AddSymbology()
{
    Init();
}

AddSymbology::AddSymbology( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, caption, pos, size, style);
}


/*!
 * AddSymbology creator
 */

bool AddSymbology::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin AddSymbology creation
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end AddSymbology creation
    return true;
}


/*!
 * AddSymbology destructor
 */

AddSymbology::~AddSymbology()
{
////@begin AddSymbology destruction
////@end AddSymbology destruction
}


/*!
 * Member initialisation
 */

void AddSymbology::Init()
{
////@begin AddSymbology member initialisation
////@end AddSymbology member initialisation
}


/*!
 * Control creation for AddSymbology
 */

void AddSymbology::CreateControls()
{    
////@begin AddSymbology content construction
    AddSymbology* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxFlexGridSizer* itemFlexGridSizer3 = new wxFlexGridSizer(2, 2, 0, 0);
    itemFlexGridSizer3->AddGrowableCol(1);
    itemBoxSizer2->Add(itemFlexGridSizer3, 0, wxGROW|wxALL, 5);

    wxStaticText* itemStaticText4 = new wxStaticText( itemDialog1, wxID_STATIC, _("Value :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer3->Add(itemStaticText4, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxArrayString itemComboBox5Strings;
    wxComboBox* itemComboBox5 = new wxComboBox( itemDialog1, ID_COMBOBOX1, _T(""), wxDefaultPosition, wxDefaultSize, itemComboBox5Strings, wxCB_DROPDOWN );
    itemFlexGridSizer3->Add(itemComboBox5, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText6 = new wxStaticText( itemDialog1, wxID_STATIC, _("Color :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer3->Add(itemStaticText6, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton7 = new wxButton( itemDialog1, ID_BUTTON17, _("Button"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer3->Add(itemButton7, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxCheckBox* itemCheckBox8 = new wxCheckBox( itemDialog1, ID_CHECKBOX1, _("Transparent color"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox8->SetValue(false);
    itemBoxSizer2->Add(itemCheckBox8, 0, wxGROW|wxALL, 5);

    wxStdDialogButtonSizer* itemStdDialogButtonSizer9 = new wxStdDialogButtonSizer;

    itemBoxSizer2->Add(itemStdDialogButtonSizer9, 0, wxALIGN_CENTER_HORIZONTAL|wxLEFT|wxTOP|wxBOTTOM, 5);
    wxButton* itemButton10 = new wxButton( itemDialog1, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer9->AddButton(itemButton10);

    wxButton* itemButton11 = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer9->AddButton(itemButton11);

    itemStdDialogButtonSizer9->Realize();

////@end AddSymbology content construction
}


/*!
 * Should we show tooltips?
 */

bool AddSymbology::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap AddSymbology::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin AddSymbology bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end AddSymbology bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon AddSymbology::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin AddSymbology icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end AddSymbology icon retrieval
}