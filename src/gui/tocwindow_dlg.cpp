/***************************************************************************
 TocWindowDlg.cpp
 Display the TOC, try using the wxDrawerWindow class
 -------------------
 copyright : (C) 2007 CREALP Lucien Schreiber
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
    m_TOCCtrl = NULL;

}

TocWindowContent::~TocWindowContent()
{
    delete m_TOCCtrl;
}

IMPLEMENT_DYNAMIC_CLASS(TocWindowContent, wxEvtHandler)


/*!
 * Control creation for TocWindowDlg
 */
wxSizer *TocWindowContent::CreateControls(wxWindow *parent, bool call_fit, bool set_sizer)
{
    wxBoxSizer *itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);

    m_TOCCtrl = new tmTOCCtrl(parent, ID_TREECTRL1, wxSize(200, -1),
                              wxTR_DEFAULT_STYLE | wxTR_HIDE_ROOT);// | wxTR_MULTIPLE);
    itemBoxSizer2->Add(m_TOCCtrl, 1, wxGROW | wxALL, 0);


    /*wxBoxSizer* itemBoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer4, 0, wxALIGN_LEFT|wxALL, 0);
   wxFlatButton * itemToggleButton5 = new wxFlatButton (parent, ID_DLGTOC_ADD, wxFLATBUTTON_TEXT_ADD);
   itemBoxSizer4->Add(itemToggleButton5, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM|wxLEFT, 5);

    wxFlatButton * itemToggleButton6 = new wxFlatButton( parent, ID_DLGTOC_REMOVE, wxFLATBUTTON_TEXT_REMOVE);
    itemBoxSizer4->Add(itemToggleButton6, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);*/

    if (set_sizer) {
        parent->SetSizer(itemBoxSizer2);
        if (call_fit)
            itemBoxSizer2->SetSizeHints(parent);
    }

    return itemBoxSizer2;
}


/*************************** TOC WINDOW DLG GEN *********************************/
TocWindowDlgGen::TocWindowDlgGen()
{
    Init();
}

TocWindowDlgGen::TocWindowDlgGen(wxAuiManager *myAuiManager, wxWindow *parent, wxWindowID id, const wxString &title)
{
    Init();

    m_TocAui = myAuiManager;

    m_ParentEvt = parent;
    m_ParentEvt->PushEventHandler(this);

    m_ContentFrame = new wxPanel(parent, wxID_ANY);
    CreateControls(m_ContentFrame);

    m_TocAui->AddPane(m_ContentFrame, wxAuiPaneInfo().Name(SYMBOL_TOCWINDOW_DLG_TITLE).
            Caption(SYMBOL_TOCWINDOW_DLG_TITLE).
            Left().Layer(1).Position(1).MinSize(SYMBOL_TOCWINDOW_DLG_SIZE).CloseButton(FALSE));
    //m_TocAui->GetArtProvider()->SetMetric(wxAUI_DOCKART_GRADIENT_TYPE, wxAUI_GRADIENT_NONE);

    wxAuiDockArt *myDockArt = m_TocAui->GetArtProvider();
    wxASSERT(myDockArt);
    myDockArt->SetMetric(wxAUI_DOCKART_PANE_BORDER_SIZE, 1);
    myDockArt->SetMetric(wxAUI_DOCKART_SASH_SIZE, 2);
    //myDockArt->SetColor(wxAUI_DOCKART_BORDER_COLOUR, wxColour(*wxBLACK));
    //myDockArt->SetColor(wxAUI_DOCKART_SASH_COLOUR, wxColour(*wxBLACK));


    m_TocAui->Update();
}

TocWindowDlgGen::~TocWindowDlgGen()
{
    //delete m_TocAui;
    //delete m_ContentFrame;
    m_ParentEvt->PopEventHandler(FALSE);
}

void TocWindowDlgGen::Init()
{
    m_TocAui = NULL;
    m_ContentFrame = NULL;
    m_ParentEvt = NULL;
}

IMPLEMENT_DYNAMIC_CLASS(TocWindowDlgGen, TocWindowContent)

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


/*BEGIN_EVENT_TABLE(TocWindowDlgGen, TocWindowContent)
 EVT_FLATBUTTON(ID_DLGTOC_REMOVE, TocWindowDlgGen::OnPressRemoveLayers)
 EVT_FLATBUTTON(ID_DLGTOC_ADD, TocWindowDlgGen::OnPressAddLayers)
END_EVENT_TABLE()



void TocWindowDlgGen::OnPressRemoveLayers(wxCommandEvent & event)
{
 m_TOCCtrl->OnRemoveItem(event);
}


void TocWindowDlgGen::OnPressAddLayers(wxCommandEvent & event)
{
 wxCommandEvent evt(tmEVT_LM_ADD, wxID_ANY);
 m_ParentEvt->GetEventHandler()->QueueEvent(evt.Clone());
}*/

