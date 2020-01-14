/***************************************************************************
 listgenreport_status.cpp
 Enhance the listgenreport class with a status bar
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

#include "listgenreport_status.h"

/***************************************************************************/ /**
  @brief Constructor
  @details See doc of #ListGenReportWithDialog for parameters descriptions
  @author Lucien Schreiber (c) CREALP 2008
  @date 21 May 2008
  *******************************************************************************/
ListGenReportWithStatus::ListGenReportWithStatus(wxWindow *parent, wxWindowID id, wxArrayString *pColsName,
                                                 wxArrayInt *pColsSize, wxSize size)
    : ListGenReportWithDialog(parent, id, pColsName, pColsSize, size) {
  // init default values
  InitMembers();

  // connect event
  Connect(id, wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler(ListGenReportWithStatus::OnSelectionChange));
  Connect(id, wxEVT_COMMAND_LIST_ITEM_DESELECTED, wxListEventHandler(ListGenReportWithStatus::OnSelectionChange));
}

/***************************************************************************/ /**
  @brief Init the private members
  @author Lucien Schreiber (c) CREALP 2008
  @date 21 May 2008
  *******************************************************************************/
void ListGenReportWithStatus::InitMembers() {
  m_status = NULL;
  m_TextField1 = _("Number of item(s) : %d");
  m_TextField2 = _("%d item(s) selected");
}

/***************************************************************************/ /**
  @brief Set the status bar
  @details This function must be called after creating a #ListGenReportWithStatus
  object.
  @param status pointer to a valid wxStatusBar object
  @author Lucien Schreiber (c) CREALP 2008
  @date 21 May 2008
  *******************************************************************************/
void ListGenReportWithStatus::SetStatusBar(wxStatusBar *status) {
  m_status = status;
}

/***************************************************************************/ /**
  @brief Set the fields messages
  @details This function may be used for setting one or two personnalized message
  for the status bar fields.
  @note Strings must contain the %d character. First
  string will be used for displaying the number of items in the list while the
  second field is designed for the number of selected items.
  Passing empty fields mean that we dont want to modify that field.
  @param field1 Specifiy a text for field 1 of the status bar (see note above)
  @param field2 Specify a text for field 2 of the status bar (see note above)
  @author Lucien Schreiber (c) CREALP 2008
  @date 21 May 2008
  *******************************************************************************/
void ListGenReportWithStatus::SetTextFields(const wxString &field1, const wxString &field2) {
  if (!field1.IsEmpty()) {
    m_TextField1 = field1;
  }

  if (!field2.IsEmpty()) {
    m_TextField2 = field2;
  }
}

/***************************************************************************/ /**
  @brief Update the status bar text
  @param iField wich field would you like to update : Allowed values are
  described in #TM_STATUS_FIELD
  @return  Return TRUE if the text was modified, false otherwise
  @author Lucien Schreiber (c) CREALP 2008
  @date 23 May 2008
  *******************************************************************************/
bool ListGenReportWithStatus::UpdateStatus(int iField) {
  // check status pointer
  if (!GetStatusBar()) {
    wxLogDebug(_T("Status bar is not defined, please define status bar first"));
    return FALSE;
  }

  if (iField & STATUS_FIELD_ITEM_COUNT) m_status->SetStatusText(wxString::Format(m_TextField1, GetItemCount()), 0);

  // update number of selected items only if greater than 0
  if (iField & STATUS_FIELD_ITEM_SELECTED) {
    if (GetSelectedItemCount() > 0) {
      m_status->SetStatusText(wxString::Format(m_TextField2, GetSelectedItemCount()), 1);
    } else {
      m_status->SetStatusText(_T(""), 1);
    }
  }

  return TRUE;
}

/***************************************************************************/ /**
  @brief Re-implement the AddItem
  @details Adding call for updating status bar
  @author Lucien Schreiber (c) CREALP 2008
  @date 23 May 2008
  *******************************************************************************/
void ListGenReportWithStatus::AddItem() {
  BeforeAdding();

  // check that the dialog pointer is not null
  // otherwise no operations are allowed
  wxASSERT_MSG(m_pDialog, wxT("Pointer to the dialog not initialised, init this pointer first"));

  // show the dialog for edition,
  // the dialog must implement the TransfertDataToWindow
  // for beeing usable
  if (m_pDialog->ShowModal() == wxID_OK) {
    AfterAdding(TRUE);
    // update status
    UpdateStatus(STATUS_FIELD_ITEM_COUNT);
  } else
    AfterAdding(FALSE);

  wxASSERT(m_pDialog);
  delete m_pDialog;
  m_pDialog = NULL;
}

/***************************************************************************/ /**
  @brief Re-implement the DeleteItem
  @details Adding call for updating status bar
  @author Lucien Schreiber (c) CREALP 2008
  @date 23 May 2008
  *******************************************************************************/
void ListGenReportWithStatus::DeleteItem() {
  BeforeDeleting();
  DeleteSelectedItem();
  // update the status
  UpdateStatus(STATUS_FIELD_ITEM_BOTH);
}

/***************************************************************************/ /**
  @brief Respond to selection change event
  @author Lucien Schreiber (c) CREALP 2008
  @date 23 May 2008
  *******************************************************************************/
void ListGenReportWithStatus::OnSelectionChange(wxListEvent &event) {
  UpdateStatus(STATUS_FIELD_ITEM_SELECTED);
  event.Skip();
}
