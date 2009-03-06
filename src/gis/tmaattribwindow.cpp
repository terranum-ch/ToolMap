/***************************************************************************
								tmaattribwindow.cpp
                    Display the Advanced Attribution Window
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

// comment doxygen

#include "tmaattribwindow.h"

// init static member
wxRect tmAAttribWindow::m_WndPos = wxRect(wxDefaultPosition, wxDefaultSize);


/***************************************************************************//**
 @brief Constructor
 @author Lucien Schreiber (c) CREALP 2009
 @date 05 March 2009
 *******************************************************************************/
tmAAttribWindow::tmAAttribWindow( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	CreateControls();
}



/***************************************************************************//**
 @brief Init member values with default values
 @author Lucien Schreiber (c) CREALP 2009
 @date 05 March 2009
 *******************************************************************************/
void tmAAttribWindow::InitMemberValue ()
{
	m_AAttribTree = NULL;
}



/***************************************************************************//**
 @brief Create default controls for UI
 @author Lucien Schreiber (c) CREALP 2009
 @date 05 March 2009
 *******************************************************************************/
void tmAAttribWindow::CreateControls ()
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer20;
	bSizer20 = new wxBoxSizer( wxVERTICAL );
	
	m_AAttribTree = new wxTreeCtrl( this, wxID_ANY, wxDefaultPosition,
								   wxSize( 300,200 ),wxTR_HIDE_ROOT);
	wxTreeItemId rootid = m_AAttribTree->AddRoot(_T("Coucou"));
	m_AAttribTree->AppendItem(rootid, _T("Salut"));
	bSizer20->Add( m_AAttribTree, 1, wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* bSizer21;
	bSizer21 = new wxBoxSizer( wxHORIZONTAL );
	
	wxButton* m_button18;
	m_button18 = new wxButton( this, wxID_CANCEL, _("&Close"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer21->Add( m_button18, 0, wxALL, 5 );
	
	wxButton* m_button19;
	m_button19 = new wxButton( this, wxID_OK, _("&Attribute"), wxDefaultPosition, wxDefaultSize, 0 );
	m_button19->SetDefault(); 
	bSizer21->Add( m_button19, 0, wxALL, 5 );
	
	bSizer20->Add( bSizer21, 0, wxALIGN_RIGHT, 5 );
	
	this->SetSizer( bSizer20 );
	this->Layout();
	bSizer20->Fit( this );
	
	SetWindowPosition();
}



/***************************************************************************//**
 @brief Destructor
 @author Lucien Schreiber (c) CREALP 2009
 @date 05 March 2009
 *******************************************************************************/
tmAAttribWindow::~tmAAttribWindow()
{
	GetWindowPosition();
}



/***************************************************************************//**
 @brief Get Window position
 @author Lucien Schreiber (c) CREALP 2009
 @date 06 March 2009
 *******************************************************************************/
void tmAAttribWindow::GetWindowPosition()
{
	m_WndPos.SetPosition(this->GetPosition());
	m_WndPos.SetSize(this->GetSize());
}



/***************************************************************************//**
 @brief Set Window position
 @details Checks that we are inside the visible part of the screen
 @author Lucien Schreiber (c) CREALP 2009
 @date 06 March 2009
 *******************************************************************************/
void tmAAttribWindow::SetWindowPosition()
{
	if (m_WndPos.IsEmpty())
	{
		this->Center(wxVERTICAL | wxHORIZONTAL);
	}
	else
	{
		// check that we are inside the screen
		wxSize myScreenSize = wxGetDisplaySize();
		wxRect myScreenRect (wxPoint (0,0), myScreenSize);
		if (myScreenRect.Contains(m_WndPos))
		{
			this->SetSize(m_WndPos);
		}
		else
		{
			this->Center(wxVERTICAL | wxHORIZONTAL);
		}
	}
}

