/***************************************************************************
 newtemplateprjwizard.cpp
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

#include "newtemplateprjwizard.h"

#include "../core/tmvalidator.h"

BEGIN_EVENT_TABLE(NewTemplatePrjWizard, wxWizard)
EVT_IDLE(NewTemplatePrjWizard::OnUpdateNextBtn)
EVT_FILEPICKER_CHANGED(ID_NEWPRJ_SOURCEDIRCTRL, NewTemplatePrjWizard::OnSourceDirChanged)
END_EVENT_TABLE()

void NewTemplatePrjWizard::OnUpdateNextBtn(wxIdleEvent &event) {
  wxWindow *myForwardBtn = FindWindowById(wxID_FORWARD);
  if (myForwardBtn == NULL) {
    return;
  }

  // source page validation
  if (GetCurrentPage() == m_SourcePage) {
    if (wxFileExists(m_SourceFileCtrl->GetFileName().GetFullPath()) == true) {
      myForwardBtn->Enable(true);
    } else {
      myForwardBtn->Enable(false);
    }
  }

  // destination page validation
  if (GetCurrentPage() == m_DestinationPage) {
    if (wxDirExists(m_DestDirCtrl->GetDirName().GetFullPath()) == true) {
      myForwardBtn->Enable(true);
    } else {
      myForwardBtn->Enable(false);
    }
  }
}

void NewTemplatePrjWizard::OnSourceDirChanged(wxFileDirPickerEvent &event) {
  m_DateCtrl->SetLabel(wxEmptyString);
  m_CommentCtrl->SetLabel(wxEmptyString);

  BackupFile myFileInfo;
  BackupManager myBkpManager(NULL);
  if (myBkpManager.GetFileInfo(m_SourceFileCtrl->GetFileName(), myFileInfo) == false) {
    return;
  }

  if (myFileInfo.IsUsingDate() == true) {
    m_DateCtrl->SetLabel(myFileInfo.GetDate().Format(_T("%d %b %Y")) + _T(" ") + myFileInfo.GetDate().FormatISOTime());
  }
  m_CommentCtrl->SetLabel(myFileInfo.GetComment());
}

void NewTemplatePrjWizard::_CreateControls() {
  // Source Page
  m_SourcePage = new wxWizardPageSimple(this);
  wxBoxSizer *bSizer1;
  bSizer1 = new wxBoxSizer(wxVERTICAL);

  wxStaticText *m_staticText1 =
      new wxStaticText(m_SourcePage, wxID_ANY, _("Select a template File: "), wxDefaultPosition, wxDefaultSize, 0);
  m_staticText1->Wrap(-1);
  bSizer1->Add(m_staticText1, 0, wxALL, 5);

  m_SourceFileCtrl =
      new wxFilePickerCtrl(m_SourcePage, ID_NEWPRJ_SOURCEDIRCTRL, wxEmptyString, _("Select a template file"),
                           _("ToolMap template files(*.tmtp)|*.tmtp|ToolMap backup files (*.tmbk)|*.tmbk"),
                           wxDefaultPosition, wxSize(200, -1), wxFLP_DEFAULT_STYLE);
  bSizer1->Add(m_SourceFileCtrl, 0, wxEXPAND | wxBOTTOM, 5);

  wxStaticBoxSizer *sbSizer2;
  sbSizer2 = new wxStaticBoxSizer(new wxStaticBox(m_SourcePage, wxID_ANY, _("Template information")), wxVERTICAL);

  wxFlexGridSizer *fgSizer2;
  fgSizer2 = new wxFlexGridSizer(2, 2, 0, 0);
  fgSizer2->AddGrowableCol(1);
  fgSizer2->SetFlexibleDirection(wxBOTH);
  fgSizer2->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

  wxStaticText *m_staticText5 =
      new wxStaticText(m_SourcePage, wxID_ANY, _("Date:"), wxDefaultPosition, wxDefaultSize, 0);
  m_staticText5->Wrap(-1);
  fgSizer2->Add(m_staticText5, 0, wxALL, 5);

  m_DateCtrl = new wxStaticText(m_SourcePage, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
  m_DateCtrl->Wrap(-1);
  fgSizer2->Add(m_DateCtrl, 0, wxALL, 5);

  wxStaticText *m_staticText4 =
      new wxStaticText(m_SourcePage, wxID_ANY, _("Comment:"), wxDefaultPosition, wxDefaultSize, 0);
  m_staticText4->Wrap(-1);
  fgSizer2->Add(m_staticText4, 0, wxALL, 5);

  m_CommentCtrl = new wxStaticText(m_SourcePage, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
  m_CommentCtrl->Wrap(-1);
  fgSizer2->Add(m_CommentCtrl, 0, wxALL, 5);

  sbSizer2->Add(fgSizer2, 1, wxEXPAND, 5);

  bSizer1->Add(sbSizer2, 0, wxEXPAND | wxALL, 5);

  m_SourcePage->SetSizer(bSizer1);
  m_SourcePage->Layout();

  // Destination page
  m_DestinationPage = new wxWizardPageSimple(this);

  wxBoxSizer *bSizer2;
  bSizer2 = new wxBoxSizer(wxVERTICAL);

  wxStaticBoxSizer *sbSizer1;
  sbSizer1 =
      new wxStaticBoxSizer(new wxStaticBox(m_DestinationPage, wxID_ANY, wxT("New project information ")), wxVERTICAL);

  wxFlexGridSizer *fgSizer1;
  fgSizer1 = new wxFlexGridSizer(2, 2, 0, 0);
  fgSizer1->AddGrowableCol(1);
  fgSizer1->SetFlexibleDirection(wxBOTH);
  fgSizer1->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

  fgSizer1->SetMinSize(wxSize(350, -1));
  wxStaticText *m_staticText2;
  m_staticText2 = new wxStaticText(m_DestinationPage, wxID_ANY, wxT("Path:"), wxDefaultPosition, wxDefaultSize, 0);
  m_staticText2->Wrap(-1);
  fgSizer1->Add(m_staticText2, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

  m_DestDirCtrl = new wxDirPickerCtrl(m_DestinationPage, ID_NEWPRJ_DESTDIRCTRL, wxEmptyString, wxT("Select a folder"),
                                      wxDefaultPosition, wxSize(-1, -1), wxDIRP_DEFAULT_STYLE);
  fgSizer1->Add(m_DestDirCtrl, 0, wxEXPAND | wxTOP | wxBOTTOM, 5);

  wxStaticText *m_staticText3;
  m_staticText3 = new wxStaticText(m_DestinationPage, wxID_ANY, wxT("Name:"), wxDefaultPosition, wxDefaultSize, 0);
  m_staticText3->Wrap(-1);
  fgSizer1->Add(m_staticText3, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

  m_DestNameCtrl =
      new wxTextCtrl(m_DestinationPage, ID_NEWPRJ_DESTNAMECTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
  fgSizer1->Add(m_DestNameCtrl, 0, wxALL | wxEXPAND, 5);

  sbSizer1->Add(fgSizer1, 1, wxEXPAND, 5);

  bSizer2->Add(sbSizer1, 0, wxEXPAND | wxALL, 5);

  m_DestNameCtrl->SetValidator(tmValidator(tmFILTER_EXCLUDE_CHAR_DATABASE));

  m_DestinationPage->SetSizer(bSizer2);
  m_DestinationPage->Layout();
}

NewTemplatePrjWizard::NewTemplatePrjWizard(wxWindow *parent, int id, const wxString &title)
    : wxWizard(parent, id, title) {
  m_SourcePage = NULL;
  m_DestinationPage = NULL;
  m_SourceFileCtrl = NULL;
  m_DestDirCtrl = NULL;
  m_DestNameCtrl = NULL;
  m_BackupFileData = new BackupFile();

  _CreateControls();
  wxWizardPageSimple::Chain(m_SourcePage, m_DestinationPage);
  GetPageAreaSizer()->Add(m_SourcePage);
  wxSize mySize = GetPageAreaSizer()->CalcMin();
  wxLogMessage("%d, %d, wizard size", mySize.GetWidth(), mySize.GetHeight());
  SetMinSize(mySize);
  SetPageSize(mySize);

  FindWindowById(wxID_FORWARD)->Enable(false);
}

NewTemplatePrjWizard::~NewTemplatePrjWizard() {
  wxDELETE(m_BackupFileData);
  this->Destroy();
}

int NewTemplatePrjWizard::ShowWizard() {
  if (RunWizard(m_SourcePage) == true) {
    TransferDataFromWindow();
    return wxID_OK;
  }
  return wxID_CANCEL;
}

bool NewTemplatePrjWizard::TransferDataFromWindow() {
  m_BackupFileData->SetOutputName(m_SourceFileCtrl->GetFileName());
  m_BackupFileData->SetInputDirectory(
      wxFileName(m_DestDirCtrl->GetDirName().GetFullPath(), m_DestNameCtrl->GetValue()));
  m_BackupFileData->SetDate(wxDateTime::Now());
  return true;
}
