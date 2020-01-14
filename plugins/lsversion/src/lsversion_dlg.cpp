/***************************************************************************
 lsversion_dlg.cpp
 -------------------
 copyright            : (C) 2010 CREALP Lucien Schreiber
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "lsversion_dlg.h"

#include "lsversion_core.h"

lsVersionDlg::lsVersionDlg(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &pos,
                           const wxSize &size, long style)
    : wxDialog(parent, id, title, pos, size, style) {
  _CreateControls();
  SetTitleAndCaption(title);
  wxString myName = wxString::Format("%s (%s)", lsVersion::GetSoftName().c_str(), lsVersion::GetSoftGIT().c_str());
  SetSoftName(myName);
  SetModulesName(lsVersion::GetAllModules());

  wxString myCopy = wxString::Format("(c) CREALP Lucien Schreiber, %d", wxDateTime::Now().GetYear());
  SetCopyright(myCopy);
}

lsVersionDlg::~lsVersionDlg() {}

void lsVersionDlg::SetBitmapLogo(const wxBitmap &bmp) {
  m_BmpCtrl->SetBitmap(bmp);
}

void lsVersionDlg::SetTitleAndCaption(wxString title) {
  m_TitleCtrl->SetLabel(title);
  this->SetTitle(title);
}

void lsVersionDlg::SetSoftName(wxString name) {
  m_ProgNameCtrl->SetLabel(name);
}

void lsVersionDlg::SetModulesName(wxString name) {
  m_ModulesCtrl->SetValue(name);
}

void lsVersionDlg::SetCopyright(wxString name) {
#ifdef wxUSE_UNICODE
  wxString myCopyText = wxString::FromUTF8("\xc2\xa9");
  name.Replace(_T("(c)"), myCopyText);
  name.Replace(_T("(C)"), myCopyText);
#endif
  m_CopyRightCtrl->SetLabel(name);
}

void lsVersionDlg::_CreateControls() {
  this->SetSizeHints(wxDefaultSize, wxDefaultSize);
  this->SetFont(wxNullFont);

  wxBoxSizer *bSizer1;
  bSizer1 = new wxBoxSizer(wxVERTICAL);

  m_BmpCtrl = new wxStaticBitmap(this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0);
  bSizer1->Add(m_BmpCtrl, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);

  m_TitleCtrl = new wxStaticText(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
  m_TitleCtrl->Wrap(-1);

  // wxFont myFont = m_TitleCtrl->GetFont();
  // myFont.SetWeight(wxFONTWEIGHT_BOLD);
  // m_TitleCtrl->SetFont(myFont);
  // m_TitleCtrl->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );

  bSizer1->Add(m_TitleCtrl, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);

  m_ProgNameCtrl = new wxStaticText(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
  m_ProgNameCtrl->Wrap(-1);
  bSizer1->Add(m_ProgNameCtrl, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);

  m_ModulesCtrl =
      new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(250, 150), wxTE_MULTILINE | wxTE_LEFT);
  bSizer1->Add(m_ModulesCtrl, 1, wxALL | wxEXPAND, 5);

  m_CopyRightCtrl = new wxStaticText(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
  m_CopyRightCtrl->Wrap(-1);
  bSizer1->Add(m_CopyRightCtrl, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);

#ifdef __WXOSX__
  m_TitleCtrl->SetFont(wxNullFont);
  m_ProgNameCtrl->SetFont(wxNullFont);
  m_CopyRightCtrl->SetFont(wxNullFont);
#endif

  this->SetSizer(bSizer1);
}

int lsVersionDlg::ShowModal() {
  this->Layout();
  wxSizer *mySizer = this->GetSizer();
  wxASSERT(mySizer);
  mySizer->Fit(this);
  CenterOnParent(wxBOTH);
  return wxDialog::ShowModal();
}
