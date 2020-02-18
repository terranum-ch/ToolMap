/***************************************************************************
 tmcrash_dlg.cpp
 Display crash dialog
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

#include "tmcrash_dlg.h"

#include "../img/misc_bmp.h"

tmCrash_DLG::tmCrash_DLG(wxWindow *parent, const wxString &crashfilename, wxWindowID id, const wxString &title,
                         const wxPoint &pos, const wxSize &size, long style)
    : wxDialog(parent, id, title, pos, size, style) {
  CreateControls(parent);
  m_CrashFile->SetValue(crashfilename);
}

tmCrash_DLG::~tmCrash_DLG() {}

void tmCrash_DLG::CreateControls(wxWindow *parent) {
  this->SetSizeHints(wxDefaultSize, wxDefaultSize);

  wxBoxSizer *bSizer43;
  bSizer43 = new wxBoxSizer(wxVERTICAL);

  wxStaticBitmap *m_bitmap2;
  m_bitmap2 = new wxStaticBitmap(this, wxID_ANY, *_img_icon_bug, wxDefaultPosition, wxDefaultSize, 0);
  bSizer43->Add(m_bitmap2, 0, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 5);

  wxBoxSizer *bSizer45;
  bSizer45 = new wxBoxSizer(wxVERTICAL);

  wxStaticText *m_staticText22;
  m_staticText22 = new wxStaticText(this, wxID_ANY, _("ToolMap has encountered an unexpected problem and will close. "),
                                    wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE);
  m_staticText22->Wrap(-1);
  bSizer45->Add(m_staticText22, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);

  wxStaticText *m_staticText23;
  m_staticText23 =
      new wxStaticText(this, wxID_ANY, _("Informations about the crash has been successfully collected here :  "),
                       wxDefaultPosition, wxDefaultSize, 0);
  m_staticText23->Wrap(-1);
  bSizer45->Add(m_staticText23, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);

  m_CrashFile =
      new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_CENTRE | wxTE_READONLY);
  bSizer45->Add(m_CrashFile, 0, wxALL | wxEXPAND, 5);

  wxStaticText *m_staticText24;
  m_staticText24 = new wxStaticText(this, wxID_ANY,
                                    _("Please report this bug using the button bellow. \nReported informations will be "
                                      "used for improving ToolMap's quality.   "),
                                    wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE);
  m_staticText24->Wrap(-1);
  bSizer45->Add(m_staticText24, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);

  bSizer43->Add(bSizer45, 1, wxEXPAND, 5);

  wxStaticLine *m_staticline5;
  m_staticline5 = new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL);
  bSizer43->Add(m_staticline5, 0, wxEXPAND | wxALL, 5);

  wxBoxSizer *bSizer44;
  bSizer44 = new wxBoxSizer(wxHORIZONTAL);

  wxButton *m_button31;
  m_button31 = new wxButton(this, wxID_CANCEL, _("Don't report"), wxDefaultPosition, wxDefaultSize, 0);
  bSizer44->Add(m_button31, 0, wxALL, 5);

  wxButton *m_button32;
  m_button32 = new wxButton(this, wxID_OK, _("Report"), wxDefaultPosition, wxDefaultSize, 0);
  m_button32->SetDefault();
  m_button32->SetFocus();
  bSizer44->Add(m_button32, 0, wxALL, 5);

  bSizer43->Add(bSizer44, 0, wxALIGN_RIGHT, 5);

  this->SetSizer(bSizer43);
  this->Layout();
  bSizer43->Fit(this);
}
