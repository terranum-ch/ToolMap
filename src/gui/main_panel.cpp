/***************************************************************************
 main_panel.cpp
 Display the main panel where drawing occurs
 -------------------
 copyright : (C) 2007 CREALP Lucien Schreiber
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

#include "main_panel.h"


Main_PANEL::Main_PANEL(wxWindow *parent, wxAuiManager *AuiManager) : ManagedAuiWnd(AuiManager)
{
    wxPanel *ContentFrame = new wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                                        wxTAB_TRAVERSAL | wxWANTS_CHARS);
    CreateControls(ContentFrame);



    // define properties of Panel.
    mPaneInfo.Name(SYMBOL_MAIN_PANEL_TITLE);
    mPaneInfo.CentrePane();

    // pass panel name to parent class.
    m_AuiPanelName = SYMBOL_MAIN_PANEL_TITLE;

    // add the panel
    AddManagedPane(ContentFrame, mPaneInfo, TRUE);

}


Main_PANEL::~Main_PANEL()
{
    delete m_GISRenderer;

}


wxSizer *Main_PANEL::CreateControls(wxWindow *parent, bool call_fit, bool set_sizer)
{
    wxBoxSizer *itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);

    m_GISRenderer = new tmRenderer(parent, wxID_ANY);


    m_GISRenderer->SetBackgroundColour(*wxWHITE);
    itemBoxSizer2->Add(m_GISRenderer, 1, wxGROW | wxALL, 0);


    if (set_sizer) {
        parent->SetSizer(itemBoxSizer2);
        if (call_fit)
            itemBoxSizer2->SetSizeHints(parent);
    }

    return itemBoxSizer2;
}

