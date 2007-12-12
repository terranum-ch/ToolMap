/////////////////////////////////////////////////////////////////////////////
// Name:        projectdeflayerseditobjectdlg.cpp
// Purpose:     
// Author:      Lucien Schreiber
// Modified by: 
// Created:     Mon 10 Dec 15:43:25 2007
// RCS-ID:      
// Copyright:   (c) CREALP 2007
// Licence:     
/////////////////////////////////////////////////////////////////////////////

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

#include "projectdeflayerseditobjectdlg.h"

////@begin XPM images
////@end XPM images


/*!
 * ProjectDefLayersEditObjectDlg type definition
 */

IMPLEMENT_DYNAMIC_CLASS( ProjectDefLayersEditObjectDlg, wxDialog )


/*!
 * ProjectDefLayersEditObjectDlg event table definition
 */

BEGIN_EVENT_TABLE( ProjectDefLayersEditObjectDlg, wxDialog )

////@begin ProjectDefLayersEditObjectDlg event table entries
////@end ProjectDefLayersEditObjectDlg event table entries

END_EVENT_TABLE()


/*!
 * ProjectDefLayersEditObjectDlg constructors
 */

ProjectDefLayersEditObjectDlg::ProjectDefLayersEditObjectDlg()
{
    Init();
}

ProjectDefLayersEditObjectDlg::ProjectDefLayersEditObjectDlg( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, caption, pos, size, style);
}


/*!
 * ProjectDefLayersEditObjectDlg creator
 */

bool ProjectDefLayersEditObjectDlg::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin ProjectDefLayersEditObjectDlg creation
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end ProjectDefLayersEditObjectDlg creation
    return true;
}


/*!
 * ProjectDefLayersEditObjectDlg destructor
 */

ProjectDefLayersEditObjectDlg::~ProjectDefLayersEditObjectDlg()
{
////@begin ProjectDefLayersEditObjectDlg destruction
////@end ProjectDefLayersEditObjectDlg destruction
}


/*!
 * Member initialisation
 */

void ProjectDefLayersEditObjectDlg::Init()
{
////@begin ProjectDefLayersEditObjectDlg member initialisation
    m_DlgEO_Code = NULL;
    m_DlgEO_Value = NULL;
    m_DlgEO_OK_Btn = NULL;
////@end ProjectDefLayersEditObjectDlg member initialisation
}


/*!
 * Control creation for ProjectDefLayersEditObjectDlg
 */

void ProjectDefLayersEditObjectDlg::CreateControls()
{    
////@begin ProjectDefLayersEditObjectDlg content construction
    ProjectDefLayersEditObjectDlg* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxFlexGridSizer* itemFlexGridSizer3 = new wxFlexGridSizer(2, 2, 0, 0);
    itemBoxSizer2->Add(itemFlexGridSizer3, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxStaticText* itemStaticText4 = new wxStaticText( itemDialog1, wxID_STATIC, _("Code :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer3->Add(itemStaticText4, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText5 = new wxStaticText( itemDialog1, wxID_STATIC, _("Description :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer3->Add(itemStaticText5, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_DlgEO_Code = new wxTextCtrl( itemDialog1, ID_DLGEO_CODE, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer3->Add(m_DlgEO_Code, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_DlgEO_Value = new wxTextCtrl( itemDialog1, ID_DLGEO_VALUE, _T(""), wxDefaultPosition, wxSize(300, -1), 0 );
    itemFlexGridSizer3->Add(m_DlgEO_Value, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticLine* itemStaticLine8 = new wxStaticLine( itemDialog1, ID_STATICLINE2, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
    itemBoxSizer2->Add(itemStaticLine8, 0, wxGROW|wxALL, 5);

    wxStdDialogButtonSizer* itemStdDialogButtonSizer9 = new wxStdDialogButtonSizer;

    itemBoxSizer2->Add(itemStdDialogButtonSizer9, 0, wxALIGN_RIGHT|wxALL, 5);
    m_DlgEO_OK_Btn = new wxButton( itemDialog1, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    m_DlgEO_OK_Btn->SetDefault();
    m_DlgEO_OK_Btn->Enable(false);
    itemStdDialogButtonSizer9->AddButton(m_DlgEO_OK_Btn);

    wxButton* itemButton11 = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer9->AddButton(itemButton11);

    itemStdDialogButtonSizer9->Realize();

////@end ProjectDefLayersEditObjectDlg content construction
}


/*!
 * Should we show tooltips?
 */

bool ProjectDefLayersEditObjectDlg::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap ProjectDefLayersEditObjectDlg::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin ProjectDefLayersEditObjectDlg bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end ProjectDefLayersEditObjectDlg bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon ProjectDefLayersEditObjectDlg::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin ProjectDefLayersEditObjectDlg icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end ProjectDefLayersEditObjectDlg icon retrieval
}
