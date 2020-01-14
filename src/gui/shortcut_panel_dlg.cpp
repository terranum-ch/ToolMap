/***************************************************************************
   shortcut_panel_dlg.cpp
 Dialog box for editing shortcuts
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

// comment doxygen

#include "shortcut_panel_dlg.h"
#include "../database/database_tm.h"    // for databse acces


Shortcut_Panel_DLG::Shortcut_Panel_DLG(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &pos,
                                       const wxSize &size, long style) : wxDialog(parent, id, title, pos, size, style)
{
    CreateControls();
}


Shortcut_Panel_DLG::~Shortcut_Panel_DLG()
{

}


void Shortcut_Panel_DLG::CreateControls()
{

    this->SetSizeHints(wxDefaultSize, wxDefaultSize);

    wxBoxSizer *bSizer10;
    bSizer10 = new wxBoxSizer(wxVERTICAL);

    wxFlexGridSizer *fgSizer1;
    fgSizer1 = new wxFlexGridSizer(2, 2, 0, 0);
    fgSizer1->AddGrowableCol(1);
    fgSizer1->SetFlexibleDirection(wxBOTH);
    fgSizer1->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

    wxStaticText *m_staticText2;
    m_staticText2 = new wxStaticText(this, wxID_ANY, _("Function Key :"), wxDefaultPosition, wxDefaultSize, 0);
    m_staticText2->Wrap(-1);
    fgSizer1->Add(m_staticText2, 0, wxALL, 5);

    m_ShortcutKey = new wxChoice(this, ID_DLG_SHORTCUT_LIST,
                                 wxDefaultPosition, wxDefaultSize,
                                 (sizeof(tmShortCutKeys) / sizeof(wxString)),
                                 tmShortCutKeys);
    m_ShortcutKey->SetSelection(0);
    fgSizer1->Add(m_ShortcutKey, 0, wxBOTTOM | wxEXPAND | wxRIGHT | wxTOP, 5);

    wxStaticText *m_staticText3;
    m_staticText3 = new wxStaticText(this, wxID_ANY, _("Description :"), wxDefaultPosition, wxDefaultSize, 0);
    m_staticText3->Wrap(-1);
    fgSizer1->Add(m_staticText3, 0, wxBOTTOM | wxLEFT | wxRIGHT, 5);

    m_ShortcutDescription = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
    m_ShortcutDescription->SetMaxLength(255);
    fgSizer1->Add(m_ShortcutDescription, 0, wxBOTTOM | wxEXPAND | wxRIGHT, 5);

    bSizer10->Add(fgSizer1, 0, wxEXPAND, 5);

    m_TypeList = new tmCheckListBox(this, wxID_ANY,
                                    wxDefaultPosition,
                                    wxDefaultSize);
    bSizer10->Add(m_TypeList, 1, wxEXPAND | wxLEFT | wxRIGHT, 5);

    wxStaticLine *m_staticline1;
    m_staticline1 = new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL);
    bSizer10->Add(m_staticline1, 0, wxEXPAND | wxALL, 5);

    wxBoxSizer *bSizer11;
    bSizer11 = new wxBoxSizer(wxHORIZONTAL);


    m_SaveButton = new wxButton(this, wxID_OK, _("&Save"), wxDefaultPosition, wxDefaultSize, 0);
    m_SaveButton->SetDefault();
    bSizer11->Add(m_SaveButton, 0, wxALL, 5);

    wxButton *m_button8;
    m_button8 = new wxButton(this, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0);
    bSizer11->Add(m_button8, 0, wxBOTTOM | wxRIGHT | wxTOP, 5);

    bSizer10->Add(bSizer11, 0, wxALIGN_RIGHT, 5);

    this->SetSizer(bSizer10);
    this->Layout();
    bSizer10->Fit(this);

    // specify a default size
    this->SetSize(300, 350);
    this->SetMinSize(wxSize(200, 200));

    this->Centre(wxBOTH);

}


/***************************************************************************//**
 @brief Called just before closing the dialog
 @details Using for data tranfert
 @author Lucien Schreiber (c) CREALP 2008
 @date 15 December 2008
 *******************************************************************************/
bool Shortcut_Panel_DLG::TransferDataFromWindow()
{
    m_SelectedKey = wxAtoi(m_ShortcutKey->GetStringSelection().AfterFirst('F'));
    m_Description = m_ShortcutDescription->GetValue();
    m_CheckedTypes.Clear();

    long myCheckedID = 0;
    wxString mytemp = _T("");
    bool myChecked = false;

    for (unsigned int i = 0; i < m_TypeList->GetCount(); i++) {
        m_TypeList->GetItem(i, myCheckedID, mytemp, myChecked);
        if (myChecked == true)
            m_CheckedTypes.Add(myCheckedID);
    }


    return true;
}


/***************************************************************************//**
 @brief Called just before displaying the dialog
 @details Using for data tranfert. Lot of data transfert was allready done in
 ShortcutList::BeforeAdding() and in ShortcutList::BeforeEditing ()
 @author Lucien Schreiber (c) CREALP 2008
 @date 15 December 2008
 *******************************************************************************/
bool Shortcut_Panel_DLG::TransferDataToWindow()
{
    m_ShortcutDescription->SetValue(m_Description);
    return true;
}


/***************************************************************************//**
 @brief Set values for keys
 @details This function set the list of keys. If the wxArrayString is empty, the
 Save button is disabled
 @param keys An array of string containing the new choices values
 @author Lucien Schreiber (c) CREALP 2008
 @date 12 December 2008
 *******************************************************************************/
void Shortcut_Panel_DLG::SetKeyList(const wxArrayString &keys)
{
    if (keys.GetCount() == 0) {
        m_SaveButton->Enable(false);
    } else {
        m_ShortcutKey->Clear();
        m_ShortcutKey->Append(keys);
        m_ShortcutKey->SetSelection(0);
    }
}


bool Shortcut_Panel_DLG::SetTypeList(DataBaseTM *database,
                                     int layer_type, int key)
{
    SetDataBase(database);

    // get objects from the database (all objects)
    if (!m_pDB->GetObjectListByLayerType(layer_type, TRUE)) {
        wxLogDebug(_T("Error getting object for layer : %d"), layer_type);
        return false;
    }


    // loop for all results
    ProjectDefMemoryObjects myTempObject;
    while (1) {
        if (m_pDB->DataBaseGetNextResultAsObject(&myTempObject, layer_type)) {

            m_TypeList->AddItem(-1, myTempObject.m_ObjectID,
                                myTempObject.m_ObjectName);
        } else
            break;
    }

    // check result if needed (key != 0)
    long myShortcutID = 0;
    bool bFirstLoop = true;
    unsigned int myListCountItem = m_TypeList->GetCount();
    long myCheckedID = 0;
    wxString mytemp = _T("");
    bool myChecked = false;
    if (key) {
        while (1) {
            if (m_pDB->GetNextShortCutObject(myShortcutID, key, bFirstLoop)) {
                bFirstLoop = false;

                // search into list
                for (unsigned int i = 0; i < myListCountItem; i++) {
                    m_TypeList->GetItem(i, myCheckedID, mytemp, myChecked);
                    if (myCheckedID == myShortcutID)
                        m_TypeList->Check(i, true);
                }

            } else
                break;
        }


    }


    return true;
}

