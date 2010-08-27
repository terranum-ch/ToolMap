/////////////////////////////////////////////////////////////////////////////
// Name:        selectbasicshortcutvalues.cpp
// Purpose:     
// Author:      Lucien Schreiber
// Modified by: 
// Created:     Fri 28 Sep 14:43:48 2007
// RCS-ID:      
// Copyright:   (c) CREALP 2007
// Licence:     
/////////////////////////////////////////////////////////////////////////////

// Generated by DialogBlocks (Personal Edition), Fri 28 Sep 14:43:48 2007

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

#include "selectbasicshortcutvalues.h"

////@begin XPM images
////@end XPM images


/*!
 * Selectbasicshortcutvalues type definition
 */

IMPLEMENT_DYNAMIC_CLASS( Selectbasicshortcutvalues, wxDialog )


/*!
 * Selectbasicshortcutvalues event table definition
 */

BEGIN_EVENT_TABLE( Selectbasicshortcutvalues, wxDialog )

////@begin Selectbasicshortcutvalues event table entries
////@end Selectbasicshortcutvalues event table entries

END_EVENT_TABLE()


/*!
 * Selectbasicshortcutvalues constructors
 */

Selectbasicshortcutvalues::Selectbasicshortcutvalues()
{
    Init();
}

Selectbasicshortcutvalues::Selectbasicshortcutvalues( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, caption, pos, size, style);
}


/*!
 * Selectbasicshortcutvalues creator
 */

bool Selectbasicshortcutvalues::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin Selectbasicshortcutvalues creation
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end Selectbasicshortcutvalues creation
    return true;
}


/*!
 * Selectbasicshortcutvalues destructor
 */

Selectbasicshortcutvalues::~Selectbasicshortcutvalues()
{
////@begin Selectbasicshortcutvalues destruction
////@end Selectbasicshortcutvalues destruction
}


/*!
 * Member initialisation
 */

void Selectbasicshortcutvalues::Init()
{
////@begin Selectbasicshortcutvalues member initialisation
////@end Selectbasicshortcutvalues member initialisation
}


/*!
 * Control creation for Selectbasicshortcutvalues
 */

void Selectbasicshortcutvalues::CreateControls()
{    
////@begin Selectbasicshortcutvalues content construction
    // Generated by DialogBlocks, Mon 12 Nov 13:58:11 2007 (Personal Edition)

    Selectbasicshortcutvalues* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxArrayString itemListBox3Strings;
    wxListBox* itemListBox3 = new wxListBox( itemDialog1, ID_LISTBOX, wxDefaultPosition, wxSize(-1, 200), itemListBox3Strings, wxLB_SINGLE );
    itemBoxSizer2->Add(itemListBox3, 1, wxGROW|wxALL, 5);

    wxGridSizer* itemGridSizer4 = new wxGridSizer(2, 2, 0, 0);
    itemBoxSizer2->Add(itemGridSizer4, 0, wxGROW|wxALL, 5);

    wxSearchCtrl* itemSearchCtrl5 = new wxSearchCtrl( itemDialog1, ID_SEARCHCTRL2, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemGridSizer4->Add(itemSearchCtrl5, 1, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxCheckBox* itemCheckBox6 = new wxCheckBox( itemDialog1, ID_CHECKBOX6, _("full attribution"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox6->SetValue(true);
    itemGridSizer4->Add(itemCheckBox6, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton7 = new wxButton( itemDialog1, ID_BUTTON29, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    itemGridSizer4->Add(itemButton7, 1, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton8 = new wxButton( itemDialog1, ID_BUTTON30, _("Continue"), wxDefaultPosition, wxDefaultSize, 0 );
    itemGridSizer4->Add(itemButton8, 1, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);

////@end Selectbasicshortcutvalues content construction
}


/*!
 * Should we show tooltips?
 */

bool Selectbasicshortcutvalues::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap Selectbasicshortcutvalues::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin Selectbasicshortcutvalues bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end Selectbasicshortcutvalues bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon Selectbasicshortcutvalues::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin Selectbasicshortcutvalues icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end Selectbasicshortcutvalues icon retrieval
}