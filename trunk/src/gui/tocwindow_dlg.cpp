/***************************************************************************
							TocWindowDlg.cpp
				Display the TOC, try using the wxDrawerWindow class
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

#include "tocwindow_dlg.h"





/**************** TOC WINDOW CONTENT (BASE CLASS) ****************************/
TocWindowContent::TocWindowContent()
{

}

TocWindowContent::~TocWindowContent()
{

}

IMPLEMENT_DYNAMIC_CLASS( TocWindowContent, wxEvtHandler)





/*!
 * Control creation for TocWindowDlg
 */
wxSizer * TocWindowContent::CreateControls(wxWindow * parent, bool call_fit, bool set_sizer)
{    
    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);

    wxTreeCtrl* itemTreeCtrl3 = new wxTreeCtrl( parent, ID_TREECTRL1, wxDefaultPosition, wxSize(200, -1), wxTR_SINGLE );
    itemBoxSizer2->Add(itemTreeCtrl3, 1, wxGROW|wxALL, 5);

    wxBoxSizer* itemBoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer4, 0, wxALIGN_LEFT|wxALL, 5);

    //wxButton * itemToggleButton5 = new wxButton(parent, ID_DLGTOC_ADD, _T("+"), wxDefaultPosition, wxSize(40, -1),);
	wxToggleButton* itemToggleButton5 = new wxToggleButton( parent, ID_DLGTOC_ADD, _("+"), wxDefaultPosition, wxSize(40, -1), 0 );
    itemToggleButton5->SetValue(false);
    itemBoxSizer4->Add(itemToggleButton5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxToggleButton* itemToggleButton6 = new wxToggleButton( parent, ID_DLGTOC_REMOVE, _("-"), wxDefaultPosition, wxSize(40, -1), 0 );
    itemToggleButton6->SetValue(false);
    itemBoxSizer4->Add(itemToggleButton6, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	if (set_sizer)
    {
        parent->SetSizer( itemBoxSizer2 );
        if (call_fit)
            itemBoxSizer2->SetSizeHints( parent );
    }
    
    return itemBoxSizer2;
}









/*************************** TOC WINDOW DLG MAC *********************************/

#if ( __WXMAC__ )
TocWindowDlgMac::TocWindowDlgMac(wxWindow * parent, wxWindowID id, const wxString & title) 
	//: wxDrawerWindow(parent,id,title,SYMBOL_TOCWINDOW_DLG_SIZE)
{
	Init();
	m_DrawerWindow = new wxDrawerWindow(parent,id,title,SYMBOL_TOCWINDOW_DLG_SIZE);
	CreateControls(m_DrawerWindow);
}

TocWindowDlgMac::TocWindowDlgMac()
{
	Init();
}

TocWindowDlgMac::~TocWindowDlgMac()
{
	delete m_DrawerWindow;
}

IMPLEMENT_DYNAMIC_CLASS( TocWindowDlgMac, TocWindowContent)

/*!
 * Member initialisation
 */

void TocWindowDlgMac::Init()
{
	m_DrawerWindow = NULL;
}

void TocWindowDlgMac::Show()
{
	m_DrawerWindow->Open();
}

void TocWindowDlgMac::Hide()
{
	m_DrawerWindow->Close();
}

bool TocWindowDlgMac::IsShown()
{
	return m_DrawerWindow->IsOpen();
}
#endif


/*************************** TOC WINDOW DLG GEN *********************************/
TocWindowDlgGen::TocWindowDlgGen()
{
	Init();
}

TocWindowDlgGen::TocWindowDlgGen(wxWindow * parent, wxWindowID id,const wxString & title)
{
	Init();

	m_TocAui = new wxAuiManager(parent);
	
	m_ContentFrame = new wxPanel(parent, wxID_ANY);
	CreateControls(m_ContentFrame);
	
	m_TocAui->AddPane(m_ContentFrame, wxAuiPaneInfo().Name(SYMBOL_TOCWINDOW_DLG_TITLE).
    	Caption(SYMBOL_TOCWINDOW_DLG_TITLE).
    	Left().Layer(1).Position(1).MinSize(SYMBOL_TOCWINDOW_DLG_SIZE));
	m_TocAui->GetArtProvider()->SetMetric(wxAUI_DOCKART_GRADIENT_TYPE, wxAUI_GRADIENT_NONE);


	m_TocAui->Update();
}

TocWindowDlgGen::~TocWindowDlgGen()
{
	delete m_TocAui;
	delete m_ContentFrame;
	m_TocAui->UnInit();
}

void TocWindowDlgGen::Init()
{
	m_TocAui = NULL;
	m_ContentFrame = NULL;
}

IMPLEMENT_DYNAMIC_CLASS( TocWindowDlgGen, TocWindowContent)

void TocWindowDlgGen::Show()
{
	m_TocAui->GetPane(SYMBOL_TOCWINDOW_DLG_TITLE).Show();
	m_TocAui->Update();
}

void TocWindowDlgGen::Hide()
{
	m_TocAui->GetPane(SYMBOL_TOCWINDOW_DLG_TITLE).Hide();
	m_TocAui->Update();
}

bool TocWindowDlgGen::IsShown()
{
	return m_TocAui->GetPane(SYMBOL_TOCWINDOW_DLG_TITLE).IsShown();
}
