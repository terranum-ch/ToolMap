/////////////////////////////////////////////////////////////////////////////
// Name:        objectdefinitionlistdlg.cpp
// Purpose:
// Author:      Lucien Schreiber
// Modified by:
// Created:     Thu 13 Mar 11:22:07 2008
// RCS-ID:
// Copyright:   (c) CREALP 2007
// Licence:
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

////@begin includes
////@end includes

#include "objectdefinitionlistdlg.h"

////@begin XPM images
////@end XPM images

/*!
 * ObjectDefinitionListDlg type definition
 */

IMPLEMENT_DYNAMIC_CLASS(ObjectDefinitionListDlg, wxDialog)

/*!
 * ObjectDefinitionListDlg event table definition
 */

BEGIN_EVENT_TABLE(ObjectDefinitionListDlg, wxDialog)

////@begin ObjectDefinitionListDlg event table entries
////@end ObjectDefinitionListDlg event table entries

END_EVENT_TABLE()

/*!
 * ObjectDefinitionListDlg constructors
 */

ObjectDefinitionListDlg::ObjectDefinitionListDlg() {
  Init();
}

ObjectDefinitionListDlg::ObjectDefinitionListDlg(wxWindow* parent, wxWindowID id, const wxString& caption,
                                                 const wxPoint& pos, const wxSize& size, long style) {
  Init();
  Create(parent, id, caption, pos, size, style);
}

/*!
 * ObjectDefinitionListDlg creator
 */

bool ObjectDefinitionListDlg::Create(wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos,
                                     const wxSize& size, long style) {
  ////@begin ObjectDefinitionListDlg creation
  SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
  wxDialog::Create(parent, id, caption, pos, size, style);

  CreateControls();
  if (GetSizer()) {
    GetSizer()->SetSizeHints(this);
  }
  Centre();
  ////@end ObjectDefinitionListDlg creation
  return true;
}

/*!
 * ObjectDefinitionListDlg destructor
 */

ObjectDefinitionListDlg::~ObjectDefinitionListDlg() {
  ////@begin ObjectDefinitionListDlg destruction
  ////@end ObjectDefinitionListDlg destruction
}

/*!
 * Member initialisation
 */

void ObjectDefinitionListDlg::Init() {
  ////@begin ObjectDefinitionListDlg member initialisation
  m_DLGODD_Code = NULL;
  m_DLGODD_Description = NULL;
  m_DLGODD_List_Lyr_Name = NULL;
  m_DLGODD_Frequency = NULL;
  ////@end ObjectDefinitionListDlg member initialisation
}

/*!
 * Control creation for ObjectDefinitionListDlg
 */

void ObjectDefinitionListDlg::CreateControls() {
  ////@begin ObjectDefinitionListDlg content construction
  ObjectDefinitionListDlg* itemDialog1 = this;

  wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
  itemDialog1->SetSizer(itemBoxSizer2);

  wxStaticBox* itemStaticBoxSizer3Static = new wxStaticBox(itemDialog1, wxID_ANY, _("Object properties"));
  wxStaticBoxSizer* itemStaticBoxSizer3 = new wxStaticBoxSizer(itemStaticBoxSizer3Static, wxVERTICAL);
  itemBoxSizer2->Add(itemStaticBoxSizer3, 0, wxGROW | wxALL, 5);

  wxFlexGridSizer* itemFlexGridSizer4 = new wxFlexGridSizer(3, 2, 0, 0);
  itemFlexGridSizer4->AddGrowableCol(1);
  itemStaticBoxSizer3->Add(itemFlexGridSizer4, 0, wxGROW, 5);

  wxStaticText* itemStaticText5 =
      new wxStaticText(itemDialog1, wxID_STATIC, _("Code :"), wxDefaultPosition, wxDefaultSize, 0);
  itemFlexGridSizer4->Add(itemStaticText5, 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL | wxALL, 5);

  m_DLGODD_Code = new wxTextCtrl(itemDialog1, wxID_ANY, _T(""), wxDefaultPosition, wxSize(250, -1), 0);
  itemFlexGridSizer4->Add(m_DLGODD_Code, 0, wxGROW | wxALIGN_CENTER_VERTICAL | wxALL, 5);

  wxStaticText* itemStaticText7 =
      new wxStaticText(itemDialog1, wxID_STATIC, _("Description :"), wxDefaultPosition, wxDefaultSize, 0);
  itemFlexGridSizer4->Add(itemStaticText7, 0, wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL | wxALL, 5);

  m_DLGODD_Description = new wxTextCtrl(itemDialog1, wxID_ANY, _T(""), wxDefaultPosition, wxDefaultSize, 0);
  m_DLGODD_Description->SetMaxLength(255);
  itemFlexGridSizer4->Add(m_DLGODD_Description, 0, wxGROW | wxALIGN_CENTER_VERTICAL | wxALL, 5);

  wxStaticText* itemStaticText9 =
      new wxStaticText(itemDialog1, wxID_STATIC, _("Layer name :"), wxDefaultPosition, wxDefaultSize, 0);
  itemFlexGridSizer4->Add(itemStaticText9, 0, wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL | wxALL, 5);

  wxArrayString m_DLGODD_List_Lyr_NameStrings;
  m_DLGODD_List_Lyr_Name = new wxChoice(itemDialog1, ID_DLGODD_LIST_LYR_NAME, wxDefaultPosition, wxDefaultSize,
                                        m_DLGODD_List_Lyr_NameStrings, 0);
  itemFlexGridSizer4->Add(m_DLGODD_List_Lyr_Name, 0, wxGROW | wxALIGN_CENTER_VERTICAL | wxALL, 5);

  wxArrayString m_DLGODD_FrequencyStrings;
  m_DLGODD_FrequencyStrings.Add(_("&Frequent"));
  m_DLGODD_FrequencyStrings.Add(_("&Not Frequent"));
  m_DLGODD_Frequency = new wxRadioBox(itemDialog1, ID_DLGODD_FREQUENCY, _("Frequency"), wxDefaultPosition,
                                      wxDefaultSize, m_DLGODD_FrequencyStrings, 1, wxRA_SPECIFY_ROWS);
  m_DLGODD_Frequency->SetSelection(0);
  itemStaticBoxSizer3->Add(m_DLGODD_Frequency, 0, wxGROW | wxALL, 5);

  itemBoxSizer2->Add(5, 5, 1, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);

  wxStaticLine* itemStaticLine13 =
      new wxStaticLine(itemDialog1, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL);
  itemBoxSizer2->Add(itemStaticLine13, 0, wxGROW | wxALL, 5);

  wxStdDialogButtonSizer* itemStdDialogButtonSizer14 = new wxStdDialogButtonSizer;

  itemBoxSizer2->Add(itemStdDialogButtonSizer14, 0, wxALIGN_RIGHT | wxALL, 5);
  wxButton* itemButton15 = new wxButton(itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0);
  itemStdDialogButtonSizer14->AddButton(itemButton15);

  wxButton* itemButton16 = new wxButton(itemDialog1, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0);
  itemButton16->SetDefault();
  itemStdDialogButtonSizer14->AddButton(itemButton16);

  itemStdDialogButtonSizer14->Realize();

  // Set validators
  m_DLGODD_Description->SetValidator(wxTextValidator(wxFILTER_NONE, &m_Description));
  ////@end ObjectDefinitionListDlg content construction
}

/*!
 * Should we show tooltips?
 */

bool ObjectDefinitionListDlg::ShowToolTips() {
  return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap ObjectDefinitionListDlg::GetBitmapResource(const wxString& name) {
  // Bitmap retrieval
  ////@begin ObjectDefinitionListDlg bitmap retrieval
  wxUnusedVar(name);
  return wxNullBitmap;
  ////@end ObjectDefinitionListDlg bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon ObjectDefinitionListDlg::GetIconResource(const wxString& name) {
  // Icon retrieval
  ////@begin ObjectDefinitionListDlg icon retrieval
  wxUnusedVar(name);
  return wxNullIcon;
  ////@end ObjectDefinitionListDlg icon retrieval
}
