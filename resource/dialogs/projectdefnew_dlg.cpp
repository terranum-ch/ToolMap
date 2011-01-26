/////////////////////////////////////////////////////////////////////////////
// Name:        projectdefnew_dlg.cpp
// Purpose:     
// Author:      Lucien Schreiber
// Modified by: 
// Created:     Fri 16 May 22:18:04 2008
// RCS-ID:      
// Copyright:   (c) CREALP 2007
// Licence:     
/////////////////////////////////////////////////////////////////////////////

// Generated by DialogBlocks (Personal Edition), Fri 16 May 22:18:04 2008

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

#include "projectdefnew_dlg.h"

////@begin XPM images
////@end XPM images


/*!
 * ProjectDefNew type definition
 */

IMPLEMENT_DYNAMIC_CLASS( ProjectDefNew, wxDialog )


/*!
 * ProjectDefNew event table definition
 */

BEGIN_EVENT_TABLE( ProjectDefNew, wxDialog )

////@begin ProjectDefNew event table entries
////@end ProjectDefNew event table entries

END_EVENT_TABLE()


/*!
 * ProjectDefNew constructors
 */

ProjectDefNew::ProjectDefNew()
{
    Init();
}

ProjectDefNew::ProjectDefNew( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, caption, pos, size, style);
}


/*!
 * ProjectDefNew creator
 */

bool ProjectDefNew::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin ProjectDefNew creation
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end ProjectDefNew creation
    return true;
}


/*!
 * ProjectDefNew destructor
 */

ProjectDefNew::~ProjectDefNew()
{
////@begin ProjectDefNew destruction
////@end ProjectDefNew destruction
}


/*!
 * Member initialisation
 */

void ProjectDefNew::Init()
{
////@begin ProjectDefNew member initialisation
////@end ProjectDefNew member initialisation
}


/*!
 * Control creation for ProjectDefNew
 */

void ProjectDefNew::CreateControls()
{    
////@begin ProjectDefNew content construction
    // Generated by DialogBlocks, Fri 16 May 22:33:11 2008 (Personal Edition)

    ProjectDefNew* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxFlexGridSizer* itemFlexGridSizer3 = new wxFlexGridSizer(2, 2, 0, 0);
    itemFlexGridSizer3->AddGrowableCol(1);
    itemBoxSizer2->Add(itemFlexGridSizer3, 0, wxGROW|wxALL, 5);

    wxStaticText* itemStaticText4 = new wxStaticText( itemDialog1, wxID_STATIC, _("Static text"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer3->Add(itemStaticText4, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxTextCtrl* itemTextCtrl5 = new wxTextCtrl( itemDialog1, ID_TEXTCTRL1, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer3->Add(itemTextCtrl5, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText6 = new wxStaticText( itemDialog1, wxID_STATIC, _("Static text"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer3->Add(itemStaticText6, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxTextCtrl* itemTextCtrl7 = new wxTextCtrl( itemDialog1, ID_TEXTCTRL2, _T(""), wxDefaultPosition, wxSize(300, -1), 0 );
    itemTextCtrl7->SetMaxLength(50);
    itemFlexGridSizer3->Add(itemTextCtrl7, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemBoxSizer2->Add(5, 5, 1, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxStaticLine* itemStaticLine9 = new wxStaticLine( itemDialog1, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
    itemBoxSizer2->Add(itemStaticLine9, 0, wxGROW|wxALL, 5);

    wxStdDialogButtonSizer* itemStdDialogButtonSizer10 = new wxStdDialogButtonSizer;

    itemBoxSizer2->Add(itemStdDialogButtonSizer10, 0, wxALIGN_RIGHT|wxALL, 5);
    wxButton* itemButton11 = new wxButton( itemDialog1, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer10->AddButton(itemButton11);

    wxButton* itemButton12 = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer10->AddButton(itemButton12);

    itemStdDialogButtonSizer10->Realize();

////@end ProjectDefNew content construction
}


/*!
 * Should we show tooltips?
 */

bool ProjectDefNew::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap ProjectDefNew::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin ProjectDefNew bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end ProjectDefNew bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon ProjectDefNew::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin ProjectDefNew icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end ProjectDefNew icon retrieval
}