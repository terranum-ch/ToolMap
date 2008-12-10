/***************************************************************************
								shortcut_panel.cpp
							Display the Shortcut panel
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

#include "shortcut_panel.h"

BEGIN_EVENT_TABLE( Shortcuts_PANEL, ManagedAuiWnd )
	EVT_CHOICE( IDDLG_SHORT_TARGET, Shortcuts_PANEL::OnChangeTarget )
	EVT_BUTTON( IDDLG_SHORT_ADD_BTN, Shortcuts_PANEL::OnShortcutAdd )
	EVT_BUTTON( IDDLG_SHORT_DEL_BTN, Shortcuts_PANEL::OnShortcutDel )
	EVT_BUTTON( IDDLG_SHORT_EDIT_BTN, Shortcuts_PANEL::OnShortcutEdit )
END_EVENT_TABLE()


Shortcuts_PANEL::Shortcuts_PANEL( wxWindow* parent, wxWindowID id, wxAuiManager * auimanager) : 
ManagedAuiWnd(auimanager)
{
	InitMemberValues();
	wxASSERT(parent);
	m_ParentEvt = parent;
	m_ParentEvt->PushEventHandler(this);
	
	// create the controls
	wxPanel *  ContentFrame = new wxPanel (parent, wxID_ANY);
	CreateControls(ContentFrame);
	ContentFrame->Fit();
	
	// define properties of Panel.
	m_PaneInfo.Name(SHORTCUT_PANEL_TITLE);
	m_PaneInfo.Caption(SHORTCUT_PANEL_TITLE);
	m_PaneInfo.Float();
	m_PaneInfo.Right();
	m_PaneInfo.Layer(4);
	m_PaneInfo.Position(4);
	m_PaneInfo.MinSize(SHORTCUT_PANEL_SIZE);
	m_PaneInfo.CloseButton(true);
	m_PaneInfo.Hide();
	m_PaneInfo.FloatingPosition(100, 150);
	m_PaneInfo.FloatingSize(SHORTCUT_PANEL_SIZE + wxSize(100,100));
	
	m_AuiPanelName = SHORTCUT_PANEL_TITLE;
	
	AddManagedPane(ContentFrame, m_PaneInfo);

	
}

void Shortcuts_PANEL::InitMemberValues()
{
	 m_TargetChoice = NULL;
	 m_ListShortcuts= NULL;
	 m_ParentEvt= NULL;	
	
}


Shortcuts_PANEL::~Shortcuts_PANEL()
{
	m_ParentEvt->PopEventHandler(FALSE);
}



/***************************************************************************//**
 @brief Control creation
 @author Lucien Schreiber (c) CREALP 2008
 @date 08 November 2008
 *******************************************************************************/
wxSizer * Shortcuts_PANEL::CreateControls(wxWindow * parent,
										 bool call_fit,	 bool set_sizer)
{    
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxHORIZONTAL );
	
	wxStaticText* m_staticText1;
	m_staticText1 = new wxStaticText( parent, wxID_ANY, _("Target :"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1->Wrap( -1 );
	bSizer2->Add( m_staticText1, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_TargetChoice = new wxChoice( parent, IDDLG_SHORT_TARGET, wxDefaultPosition, wxDefaultSize,
								  3,TOC_GENERIC_NAME_STRING , 0 );
	m_TargetChoice->SetSelection( TOC_NAME_LINES );
	bSizer2->Add( m_TargetChoice, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 5 );
	
	bSizer1->Add( bSizer2, 0, wxEXPAND, 5 );
	
	wxArrayString myColName;
	myColName.Add(_("Key"));
	myColName.Add(_("Description"));
	
	wxArrayInt myColWidth;
	myColWidth.Add(80);
	myColWidth.Add(200);
	
	m_ListShortcuts = new ShortcutList( parent, IDDLG_SHORT_LIST,
									   &myColName,
									   &myColWidth,
									   wxDefaultSize);
	bSizer1->Add( m_ListShortcuts, 1, wxALL|wxEXPAND, 0 );
	
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxHORIZONTAL );
	
	wxFlatButton * myShortcutAdd = new wxFlatButton( parent, IDDLG_SHORT_ADD_BTN, wxFLATBUTTON_TEXT_ADD);
	bSizer3->Add( myShortcutAdd, 0, wxLEFT, 0 );
	
	wxFlatButton * myShortcutDel = new wxFlatButton( parent, IDDLG_SHORT_DEL_BTN, wxFLATBUTTON_TEXT_REMOVE);
	bSizer3->Add( myShortcutDel, 0, wxLEFT, 5 );
	
	wxFlatButton * myShortcutEdit = new wxFlatButton( parent, IDDLG_SHORT_EDIT_BTN, _("Edit selected..."), wxDefaultSize);
	bSizer3->Add( myShortcutEdit, 0, wxLEFT|wxRIGHT, 5 );
	
	bSizer1->Add( bSizer3, 0, wxALL, 5 );
	
	
	if (set_sizer)
    {
        parent->SetSizer( bSizer1 );
        if (call_fit)
		{
            bSizer1->SetSizeHints( parent );
		}
	}
    
    return bSizer1;
}






/***************************** SHORTCUT LIST *****************************/



/***************************************************************************//**
 @brief Constructor
 @author Lucien Schreiber (c) CREALP 2008
 @date 10 December 2008
 *******************************************************************************/
ShortcutList::ShortcutList (wxWindow * parent,
						  wxWindowID id,
						  wxArrayString * pColsName, 
						  wxArrayInt * pColsSize,
						  wxSize size) :
ListGenReportWithDialog(parent, id, pColsName, pColsSize, size)
{
	
}



/***************************************************************************//**
 @brief Destructor
 @author Lucien Schreiber (c) CREALP 2008
 @date 10 December 2008
 *******************************************************************************/
ShortcutList::~ShortcutList()
{
	
}


