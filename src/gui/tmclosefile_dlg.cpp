/***************************************************************************
 tmclosefile_dlg.cpp
 -------------------
 copyright            : (C) 2013 CREALP Lucien Schreiber
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "tmclosefile_dlg.h"

BEGIN_EVENT_TABLE(tmCloseFile_DLG, wxDialog)
EVT_BUTTON(ID_BTN_ALL, tmCloseFile_DLG::OnBtnAll)
EVT_UPDATE_UI(ID_BTN_ALL, tmCloseFile_DLG::OnUpdateUIAll)
EVT_BUTTON(ID_BTN_INVERT, tmCloseFile_DLG::OnBtnInvert)
EVT_BUTTON(ID_BTN_NONE, tmCloseFile_DLG::OnBtnNone)
EVT_UPDATE_UI(ID_BTN_NONE, tmCloseFile_DLG::OnUpdateUINone)
END_EVENT_TABLE()

tmCloseFile_DLG::tmCloseFile_DLG(wxWindow *parent, const wxArrayString &choices, wxWindowID id, const wxString &title,
                                 const wxPoint &pos, const wxSize &size, long style)
    : wxDialog(parent, id, title, pos, size, style) {
  _CreateControls();
  m_FileListCtrl->Append(choices);
}

tmCloseFile_DLG::~tmCloseFile_DLG() {}

void tmCloseFile_DLG::OnBtnAll(wxCommandEvent &event) {
  for (unsigned int i = 0; i < m_FileListCtrl->GetCount(); i++) {
    m_FileListCtrl->Check(i);
  }
}

void tmCloseFile_DLG::OnUpdateUIAll(wxUpdateUIEvent &event) {
  wxArrayInt myCheckedArray;
  int myCount = m_FileListCtrl->GetCheckedItems(myCheckedArray);
  if (myCount == m_FileListCtrl->GetCount()) {
    event.Enable(false);
    return;
  }
  event.Enable(true);
}

void tmCloseFile_DLG::OnBtnInvert(wxCommandEvent &event) {
  for (unsigned int i = 0; i < m_FileListCtrl->GetCount(); i++) {
    if (m_FileListCtrl->IsChecked(i)) {
      m_FileListCtrl->Check(i, false);
    } else {
      m_FileListCtrl->Check(i, true);
    }
  }
}

void tmCloseFile_DLG::OnBtnNone(wxCommandEvent &event) {
  for (unsigned int i = 0; i < m_FileListCtrl->GetCount(); i++) {
    m_FileListCtrl->Check(i, false);
  }
}

void tmCloseFile_DLG::OnUpdateUINone(wxUpdateUIEvent &event) {
  wxArrayInt myCheckedArray;
  int myCount = m_FileListCtrl->GetCheckedItems(myCheckedArray);
  if (myCount == 0) {
    event.Enable(false);
    return;
  }
  event.Enable(true);
}

wxArrayInt tmCloseFile_DLG::GetSelections() {
  wxArrayInt mySelectedIndex;
  m_FileListCtrl->GetCheckedItems(mySelectedIndex);
  return mySelectedIndex;
}

void tmCloseFile_DLG::_CreateControls() {
  this->SetSizeHints(wxDefaultSize, wxDefaultSize);

  wxBoxSizer *bSizer1;
  bSizer1 = new wxBoxSizer(wxVERTICAL);

  wxStaticBoxSizer *sbSizer1;
  sbSizer1 = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, _("Layers")), wxVERTICAL);

  wxArrayString m_FileListCtrlChoices;
  m_FileListCtrl = new wxCheckListBox(this, wxID_ANY, wxDefaultPosition, wxSize(300, 300), m_FileListCtrlChoices, 0);
  sbSizer1->Add(m_FileListCtrl, 0, wxALL, 5);

  wxBoxSizer *bSizer2;
  bSizer2 = new wxBoxSizer(wxHORIZONTAL);

  m_BtnAllCtrl = new wxButton(this, ID_BTN_ALL, _("All"), wxDefaultPosition, wxDefaultSize, 0);
  bSizer2->Add(m_BtnAllCtrl, 0, wxALL, 5);

  m_BtnInvertCtrl = new wxButton(this, ID_BTN_INVERT, _("Invert"), wxDefaultPosition, wxDefaultSize, 0);
  bSizer2->Add(m_BtnInvertCtrl, 0, wxALL, 5);

  m_BtnNoneCtrl = new wxButton(this, ID_BTN_NONE, _("None"), wxDefaultPosition, wxDefaultSize, 0);
  bSizer2->Add(m_BtnNoneCtrl, 0, wxALL, 5);

  sbSizer1->Add(bSizer2, 0, 0, 5);

  bSizer1->Add(sbSizer1, 1, wxEXPAND | wxALL, 5);

  wxStdDialogButtonSizer *m_sdbSizer1;
  wxButton *m_sdbSizer1OK;
  wxButton *m_sdbSizer1Cancel;
  m_sdbSizer1 = new wxStdDialogButtonSizer();
  m_sdbSizer1OK = new wxButton(this, wxID_OK);
  m_sdbSizer1->AddButton(m_sdbSizer1OK);
  m_sdbSizer1Cancel = new wxButton(this, wxID_CANCEL);
  m_sdbSizer1->AddButton(m_sdbSizer1Cancel);
  m_sdbSizer1->Realize();

  bSizer1->Add(m_sdbSizer1, 0, wxEXPAND | wxALL, 5);

  this->SetSizer(bSizer1);
  this->Layout();
  bSizer1->Fit(this);

  this->Centre(wxBOTH);
}
