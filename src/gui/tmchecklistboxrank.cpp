/***************************************************************************
 tmchecklistboxrank.cpp
 Deals with checklistbox but add the ability to save the order
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

#include "tmchecklistboxrank.h"

/***************************************************************************/ /**
  @brief Constructor for one steps creation
  @details see documentation for wxCheckListBox for parameters description
  @author Lucien Schreiber (c) CREALP 2008
  @date 20 May 2008
  *******************************************************************************/
tmCheckListBoxRank::tmCheckListBoxRank(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size,
                                       int nStrings, const wxString *choices, long style, const wxValidator &validator,
                                       const wxString &name) {
  Init();
  Create(parent, id, pos, size, nStrings, choices, style, validator, name);
}

/***************************************************************************/ /**
  @brief Constructor for two steps creation
  @details see documentation for wxCheckListBox for parameters description
  @author Lucien Schreiber (c) CREALP 2008
  @date 20 May 2008
  *******************************************************************************/
bool tmCheckListBoxRank::Create(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size, int nStrings,
                                const wxString *choices, long style, const wxValidator &validator,
                                const wxString &name) {
  if (!tmCheckListBox::Create(parent, id, pos, size, nStrings, choices, style, validator, name)) {
    return FALSE;
  }

  if (style & tmLB_MENU) {
    // add menu entry and attach them to functions
    if (!AddToMenu()) {
      return FALSE;
    }

    // all seems correct, connect the menu now
    Connect(tmCHECK_MENU_SAVE_RANK, wxEVT_COMMAND_MENU_SELECTED,
            wxCommandEventHandler(tmCheckListBoxRank::OnSaveOrder));

  } else  // tmLB_MENU not defined, this may cause problems
  {
    wxLogDebug(_T("Style menu not defined for tmCheckListboxRank, no enhanced functions actives"));
    return FALSE;
  }

  return TRUE;
}

/***************************************************************************/ /**
  @brief Destructor
  Do nothing actually
  @author Lucien Schreiber (c) CREALP 2008
  @date 20 May 2008
  *******************************************************************************/
tmCheckListBoxRank::~tmCheckListBoxRank() {}

/***************************************************************************/ /**
  @brief Init member values
  @author Lucien Schreiber (c) CREALP 2008
  @date 20 May 2008
  *******************************************************************************/
void tmCheckListBoxRank::Init() {
  m_RankColName = _T("RANK");
  m_TableName = TABLE_NAME_OBJECTS;
  m_pDB = NULL;
}

/***************************************************************************/ /**
  @brief Add entry to the popup menu
  @details Following entry are added to the popup menu if the menu exists
  - Saving list order
  @return  TRUE if the menu exists
  @author Lucien Schreiber (c) CREALP 2008
  @date 20 May 2008
  *******************************************************************************/
bool tmCheckListBoxRank::AddToMenu() {
  if (GetPopupMenu() == NULL) {
    wxFAIL;
    wxLogError(_T("Pointer to the popup menu is NULL"));
    return false;
  }

  GetPopupMenu()->AppendSeparator();
  GetPopupMenu()->Append(tmCHECK_MENU_SAVE_RANK, _("Save order"));
  return TRUE;
}

/***************************************************************************/ /**
  @brief Set the rank column name
  @details If your rank column has default name "RANK", no need to call this
  function
  @param colrank The name of the rank column (default is RANK)
  @author Lucien Schreiber (c) CREALP 2008
  @date 20 May 2008
  *******************************************************************************/
void tmCheckListBoxRank::SetRankColName(const wxString &colrank) {
  m_RankColName = colrank;
}

/***************************************************************************/ /**
  @brief Set the table name
  @details The table name will be used for setting order when user press the
  "Save order" popup menu.
  @param tabname The name of the table. Default is the value of
  #TABLE_NAME_OBJECTS
  @author Lucien Schreiber (c) CREALP 2008
  @date 20 May 2008
  *******************************************************************************/
void tmCheckListBoxRank::SetTableName(const wxString &tabname) {
  m_TableName = tabname;
}

/***************************************************************************/ /**
  @brief Save the list order
  @details Called when user press the "Save order" button in the popup menu
  @author Lucien Schreiber (c) CREALP 2008
  @date 20 May 2008
  *******************************************************************************/
void tmCheckListBoxRank::OnSaveOrder(wxCommandEvent &event) {
  // check that the database is correct
  if (m_pDB == NULL) {
    wxLogDebug(_T("Error : pointer to database not defined"));
    return;
  }

  // preparing statement
  wxString sqlstatement = _T("");
  if (!PrepareOrderStatement(sqlstatement)) return;

  // do the query
  if (!m_pDB->DataBaseQueryNoResults(sqlstatement)) {
    wxLogError(_("Error saving order to project"));
    wxLogDebug(_T("Order statement is : ") + sqlstatement);
  }

  wxLogDebug(_T("Saving rank"));
}

/***************************************************************************/ /**
  @brief Get the actual order from the list
  @details Every item's id from the list is returned in the same order as they
  appear in the list now
  @param id The array of long containing id values in the same order as in the
  list
  @return  TRUE if the list isn't empty
  @author Lucien Schreiber (c) CREALP 2008
  @date 20 May 2008
  *******************************************************************************/
bool tmCheckListBoxRank::GetOrder(wxArrayLong &id) {
  return TRUE;
}

/***************************************************************************/ /**
  @brief Prepare the statement for setting order
  @details Create the sql statement for setting the item's order in the database
  @param sqlstatement string will be filled with the issued statement for setting
  the item's order
  @return  TRUE if the list isn't empty
  @author Lucien Schreiber (c) CREALP 2008
  @date 20 May 2008
  *******************************************************************************/
bool tmCheckListBoxRank::PrepareOrderStatement(wxString &sqlstatement) {
  unsigned int iListItemCount = GetCount();
  if (iListItemCount == 0) {
    wxLogDebug(_T("List is empty, not able to set the order"));
    return FALSE;
  }

  // get the items
  wxString itemname = wxEmptyString;
  long itemid = 0;
  bool itemchecked = FALSE;
  for (unsigned i = 0; i < iListItemCount; i++) {
    if (!GetItem(i, itemid, itemname, itemchecked)) {
      wxLogDebug(_T("Getting item n. %d error"), i);
      return FALSE;
    }

    sqlstatement.Append(wxString::Format(_T("UPDATE %s SET %s=%d WHERE %s=%ld; "), GetTableName().c_str(),
                                         GetRankColName().c_str(), i, _T("OBJECT_ID"), itemid));
  }
  return TRUE;
}
