/***************************************************************************
 queries_panel_dlg.cpp
 Display dialog for queries panel (simple version)
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

#include "queries_panel_dlg.h"

/***************************************************************************/ /**
  @brief Constructor
  @details Create all the controls
  @author Lucien Schreiber (c) CREALP 2008
  @date 09 November 2008
  *******************************************************************************/
QueriesListDLG::QueriesListDLG(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &pos,
                               const wxSize &size, long style)
    : wxDialog(parent, id, title, pos, size, style) {
  this->SetSizeHints(wxSize(300, 300), wxDefaultSize);

  wxBoxSizer *bSizer2;
  bSizer2 = new wxBoxSizer(wxVERTICAL);

  wxFlexGridSizer *fgSizer1;
  fgSizer1 = new wxFlexGridSizer(2, 2, 0, 0);
  fgSizer1->AddGrowableCol(1);
  fgSizer1->SetFlexibleDirection(wxHORIZONTAL);
  fgSizer1->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

  wxStaticText *m_staticText2;
  m_staticText2 = new wxStaticText(this, wxID_ANY, wxT("Description :"), wxDefaultPosition, wxDefaultSize, 0);
  m_staticText2->Wrap(-1);
  fgSizer1->Add(m_staticText2, 0, wxALL | wxALIGN_RIGHT, 5);

  m_QueriesName = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(200, -1), 0);
  m_QueriesName->SetMaxLength(255);
  fgSizer1->Add(m_QueriesName, 0, wxALL | wxEXPAND, 5);

  wxStaticText *m_staticText21;
  m_staticText21 = new wxStaticText(this, wxID_ANY, wxT("Target :"), wxDefaultPosition, wxDefaultSize, 0);
  m_staticText21->Wrap(-1);
  fgSizer1->Add(m_staticText21, 0, wxALL | wxALIGN_RIGHT, 5);

#ifdef USE_NOTES
  m_Target = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 4, TOC_GENERIC_NAME_STRING, 0);
#else
  m_Target = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 3, TOC_GENERIC_NAME_STRING, 0);
#endif

  m_Target->SetSelection(0);
  fgSizer1->Add(m_Target, 0, wxALL | wxEXPAND, 5);

  bSizer2->Add(fgSizer1, 0, wxEXPAND | wxALL, 5);

  wxStaticBoxSizer *sbSizer2;
  sbSizer2 = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, wxT("Queries (SQL Select statement)")), wxVERTICAL);

  m_QueriesDescription = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(300, 150),
                                        wxTE_MULTILINE | wxTE_BESTWRAP);  // wxSize( -1,100 )
  sbSizer2->Add(m_QueriesDescription, 1, wxALL | wxEXPAND, 5);

  bSizer2->Add(sbSizer2, 1, wxALL | wxEXPAND, 5);

  wxStaticLine *m_staticline1;
  m_staticline1 = new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL);
  bSizer2->Add(m_staticline1, 0, wxEXPAND | wxALL, 5);

  wxBoxSizer *bSizer4;
  bSizer4 = new wxBoxSizer(wxHORIZONTAL);

  wxButton *m_button3;
  m_button3 = new wxButton(this, wxID_OK, wxT("&Save"), wxDefaultPosition, wxDefaultSize, 0);
  m_button3->SetDefault();
  bSizer4->Add(m_button3, 0, wxALL, 5);

  wxButton *m_button4;
  m_button4 = new wxButton(this, wxID_CANCEL, wxT("&Close"), wxDefaultPosition, wxDefaultSize, 0);
  bSizer4->Add(m_button4, 0, wxALL, 5);

  bSizer2->Add(bSizer4, 0, wxALIGN_RIGHT | wxALL, 5);

  this->SetSizer(bSizer2);
  this->Layout();
  bSizer2->Fit(this);

  this->CenterOnParent();
  this->SetMinSize(wxSize(300, 200));
}

/***************************************************************************/ /**
  @brief Destructor
  @author Lucien Schreiber (c) CREALP 2008
  @date 09 November 2008
  *******************************************************************************/
QueriesListDLG::~QueriesListDLG() {}
