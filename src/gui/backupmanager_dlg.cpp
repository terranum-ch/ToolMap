/***************************************************************************
 backupmanager_dlg.cpp
 -------------------
 copyright            : (C) 2011 CREALP Lucien Schreiber
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "backupmanager_dlg.h"

#include "../core/backupmanager.h"
#include "../core/projectmanager.h"
#include "../gui/bitmaps.h"

BEGIN_EVENT_TABLE(BackupManagerDLG, wxDialog)
EVT_LIST_COL_CLICK(ID_LIST_BACKUPS, BackupManagerDLG::OnListColumnClick)
EVT_BUTTON(wxID_DELETE, BackupManagerDLG::OnButtonDelete)
EVT_BUTTON(ID_BTN_RESTORE, BackupManagerDLG::OnButtonRestore)
EVT_UPDATE_UI(wxID_DELETE, BackupManagerDLG::OnUpdateUIDelete)
EVT_UPDATE_UI(ID_BTN_RESTORE, BackupManagerDLG::OnUpdateUIRestore)
END_EVENT_TABLE()

void BackupManagerDLG::_CreateControls() {
    this->SetSizeHints(wxDefaultSize, wxDefaultSize);

    wxBoxSizer* bSizer3;
    bSizer3 = new wxBoxSizer(wxVERTICAL);

    int myListWidth = 650;
    int myBigColSize = wxRound((myListWidth - 26) / 3.0);
    m_ListBackup = new DataListReportCtrl(this, ID_LIST_BACKUPS, wxDefaultPosition, wxSize(myListWidth, 300));

    // insert columns
    m_ListBackup->InsertColumn(0, "");
    m_ListBackup->InsertColumn(1, _("File Name"));
    m_ListBackup->InsertColumn(2, _("Date"));
    m_ListBackup->InsertColumn(3, _("Hour"));
    m_ListBackup->InsertColumn(4, _("Comment"));

    // size columns
    m_ListBackup->SetColumnWidth(0, 26);
    m_ListBackup->SetColumnWidth(1, myBigColSize);
    m_ListBackup->SetColumnWidth(2, wxRound(myBigColSize / 2.0));
    m_ListBackup->SetColumnWidth(3, wxRound(myBigColSize / 2.0));
    m_ListBackup->SetColumnWidth(4, myBigColSize);
    bSizer3->Add(m_ListBackup, 1, wxEXPAND, 5);

    m_ImgList = new wxImageList(16, 16);
    m_ListBackup->SetImageList(m_ImgList, wxIMAGE_LIST_SMALL);
    m_ImgList->Add(m_ListBackup->CreateArrowDown(wxSize(16, 16)));
    m_ImgList->Add(m_ListBackup->CreateArrowUp(wxSize(16, 16)));
    m_ImgList->Add(Bitmaps::GetMiscBitmap(Bitmaps::ID_MISC::ZIP, wxSize(16, 16)));

    wxBoxSizer* bSizer4;
    bSizer4 = new wxBoxSizer(wxHORIZONTAL);

    m_BtnDelete = new wxButton(this, wxID_DELETE, _("Delete"), wxDefaultPosition, wxDefaultSize, 0);
    bSizer4->Add(m_BtnDelete, 0, wxALL | wxALIGN_CENTER_VERTICAL, 10);

    m_BtnRestore = new wxButton(this, ID_BTN_RESTORE, _("Restore"), wxDefaultPosition, wxDefaultSize, 0);
    bSizer4->Add(m_BtnRestore, 0, wxALL | wxALIGN_CENTER_VERTICAL, 10);

    bSizer4->Add(0, 0, 1, wxEXPAND, 5);

    wxButton* myButton;
    myButton = new wxButton(this, wxID_CANCEL, _("Close"), wxDefaultPosition, wxDefaultSize, 0);
    myButton->SetDefault();
    bSizer4->Add(myButton, 0, wxALL | wxALIGN_CENTER_VERTICAL, 10);
    bSizer3->Add(bSizer4, 0, wxEXPAND, 5);

    m_StatusBar = new wxStatusBar(this, wxID_ANY, wxST_SIZEGRIP | wxNO_BORDER);
    bSizer3->Add(m_StatusBar, 0, wxALL | wxGROW, 0);

    this->SetSizer(bSizer3);
    this->Layout();
    bSizer3->Fit(this);
    this->Centre(wxBOTH);
}

bool BackupManagerDLG::_LoadData() {
    wxString myDBName = m_BackupManager->GetDatabase()->DataBaseGetName();

    int myResult = m_BackupManager->GetDatabase()->GetProjectBackupPath(m_BackupPath);
    wxASSERT(myResult == PATH_OK);
    wxDir myBckDir(m_BackupPath);

    // list supported files based on extension and file name.
    wxArrayString mySupportedFiles;
    wxString myFileNameText = wxEmptyString;
    wxRegEx mySupportedExt(_T(".zip|.tmbk"));
    wxRegEx mySupportedName(myDBName);
    bool cont = myBckDir.GetFirst(&myFileNameText, wxEmptyString, wxDIR_FILES);
    while (cont) {
        if (mySupportedExt.Matches(myFileNameText)) {
            if (mySupportedName.Matches(myFileNameText)) {
                mySupportedFiles.Add(myFileNameText);
            }
        }
        cont = myBckDir.GetNext(&myFileNameText);
    }

    if (mySupportedFiles.GetCount() == 0) {
        return false;
    }

    wxWindowUpdateLocker noUpdates(m_ListBackup);
    wxFileName myRestoreName(m_BackupManager->GetDatabase()->DataBaseGetPath(), myDBName);
    // for every supported file, get info and populate the list.
    for (unsigned int i = 0; i < mySupportedFiles.GetCount(); i++) {
        BackupFile myFile;
        myFile.SetInputDirectory(myRestoreName);

        m_BackupManager->GetFileInfo(wxFileName(m_BackupPath, mySupportedFiles.Item(i)), myFile);

        long myIndex = m_ListBackup->InsertItem(m_ListBackup->GetItemCount(), wxEmptyString);
        m_ListBackup->SetItemImage(myIndex, 2);
        m_ListBackup->SetText(myIndex, mySupportedFiles.Item(i), 1);
        if (myFile.GetDate().IsValid()) {
            m_ListBackup->SetText(myIndex, myFile.GetDate().Format(_T("%d %b %Y")), 2);
            m_ListBackup->SetText(myIndex, myFile.GetDate().FormatISOTime(), 3);
        }
        m_ListBackup->SetText(myIndex, myFile.GetComment(), 4);
    }

    // inform the status bar
    _UpdateStatusbar(mySupportedFiles.GetCount());
    return true;
}

void BackupManagerDLG::_UpdateStatusbar(const unsigned int& bcknumber) {
    m_StatusBar->SetStatusText(wxString::Format(_("%d backup(s) in '%s'"), bcknumber, m_BackupPath), 0);
}

void BackupManagerDLG::OnButtonRestore(wxCommandEvent& event) {
    wxString myMsg = wxString::Format(_("Are you sure you want to restore backup: '%s'?"),
                                      m_ListBackup->GetText(m_ListBackup->GetSelectedFirst(), 1));
    myMsg.Append(_("\nThis will erase your actual project!"));
    int mySecurityAnswer = wxMessageBox(myMsg, _("Confirm restoring project"), wxYES_NO | wxNO_DEFAULT, this);
    if (mySecurityAnswer != wxYES) {
        return;
    }

    m_RestoreFileName = m_ListBackup->GetText(m_ListBackup->GetSelectedFirst(), 1);
    EndModal(wxID_OK);
}

void BackupManagerDLG::OnButtonDelete(wxCommandEvent& event) {
    if (m_ListBackup == nullptr || m_ListBackup->GetSelectedItemCount() == 0) {
        return;
    }

    // ask confirmation
    wxString myMessage = _("Confirm deleting ");
    if (m_ListBackup->GetSelectedItemCount() == 1) {
        myMessage.Append(_T("'") + m_ListBackup->GetText(m_ListBackup->GetSelectedFirst(), 1) + _T("'"));
    } else {
        myMessage.Append(wxString::Format(_("%d backups"), m_ListBackup->GetSelectedItemCount()));
    }
    int myAnswer = wxMessageBox(myMessage, _("Delete backup"), wxYES_NO | wxNO_DEFAULT, this);
    if (myAnswer == wxNO) {
        return;
    }

    // delete
    wxWindowUpdateLocker noUpdates(m_ListBackup);
    for (int i = m_ListBackup->GetItemCount() - 1; i >= 0; i--) {
        if (m_ListBackup->GetItemState(i, wxLIST_STATE_SELECTED) == wxLIST_STATE_SELECTED) {
            wxFileName myFile(m_BackupPath, m_ListBackup->GetText(i, 1));
            if (!wxRemoveFile(myFile.GetFullPath())) {
                wxLogError(_("Removing file : '%s' failed!"), myFile.GetFullName());
            } else {
                m_ListBackup->DeleteItem(i);
            }
        }
    }
    _UpdateStatusbar(m_ListBackup->GetItemCount());
}

void BackupManagerDLG::OnUpdateUIDelete(wxUpdateUIEvent& event) {
    if (m_ListBackup == nullptr || m_ListBackup->GetSelectedItemCount() == 0) {
        event.Enable(false);
        return;
    }
    event.Enable(true);
}

void BackupManagerDLG::OnUpdateUIRestore(wxUpdateUIEvent& event) {
    if (m_ListBackup == nullptr || m_ListBackup->GetSelectedItemCount() != 1) {
        event.Enable(false);
        return;
    }
    event.Enable(true);
}

void BackupManagerDLG::OnListColumnClick(wxListEvent& event) {
    int myCol = event.GetColumn();
    if (myCol == wxNOT_FOUND) {
        return;
    }

    // invert order
    if (myCol == m_ClassedCol) {
        if (m_AscendingOrder) {
            m_AscendingOrder = false;
        } else {
            m_AscendingOrder = true;
        }
    } else {
        m_AscendingOrder = true;
    }

    wxWindowUpdateLocker noUpdates(m_ListBackup);
    // remove triangle from last colum
    if (m_ClassedCol != wxNOT_FOUND) {
        m_ListBackup->SetColumnImage(wxNOT_FOUND, m_ClassedCol);
    }
    int myImageindex = 0;
    if (m_AscendingOrder) {
        myImageindex = 1;
    }
    m_ListBackup->SetColumnImage(myImageindex, myCol);

    switch (myCol) {
        case 1:
        case 4:
            m_ListBackup->SortList(myCol, 0, -1, DATALIST_COLTYPE_TEXT, m_AscendingOrder);
            break;

        case 2:
            m_ListBackup->SortList(myCol, 0, -1, DATALIST_COLTYPE_DATE, m_AscendingOrder);
            break;

        case 3:
            m_ListBackup->SortList(myCol, 0, -1, DATALIST_COLTYPE_TIME, m_AscendingOrder);
            break;

        default:
            m_AscendingOrder = true;
            m_ClassedCol = wxNOT_FOUND;
            return;
            break;
    }
    m_ClassedCol = myCol;
    event.Skip();
}

BackupManagerDLG::BackupManagerDLG(wxWindow* parent, wxWindowID id, const wxString& title, BackupManager* bckmanager)
    : wxDialog(parent, id, title, wxDefaultPosition, wxDefaultSize,
               wxDEFAULT_DIALOG_STYLE | wxMAXIMIZE_BOX | wxRESIZE_BORDER) {
    wxASSERT(bckmanager);
    m_BackupManager = bckmanager;
    m_ClassedCol = wxNOT_FOUND;
    m_AscendingOrder = true;
    m_ImgList = nullptr;
    m_BackupPath = wxEmptyString;
    m_RestoreFileName = wxEmptyString;

    _CreateControls();
    _LoadData();
}

BackupManagerDLG::~BackupManagerDLG() {
    wxDELETE(m_ImgList);
}

wxString BackupManagerDLG::GetRestoreFileName() {
    return m_RestoreFileName;
}
