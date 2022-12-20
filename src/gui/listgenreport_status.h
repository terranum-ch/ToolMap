/***************************************************************************
 listgenreport_status.h
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

#ifndef NAME_H
#define NAME_H

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

// Include wxWidgets' headers
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "listgenreport.h"

enum TM_STATUS_FIELD {
    /// only first status field will be modified
    STATUS_FIELD_ITEM_COUNT = 1,
    /// only second status field will be modified
    STATUS_FIELD_ITEM_SELECTED = 2,
    /// both status fields will be modified
    STATUS_FIELD_ITEM_BOTH = 3
};

/***************************************************************************/ /**
  @brief Enhance the #ListGenReportWithDialog for supporting status bar
  @details This class is able to automaticaly deals with a status bar updating
  number of elements status and number of selected items. String displayed is
  customizable
  @todo add a code sample for using this class
  @author Lucien Schreiber (c) CREALP 2008
  @date 21 May 2008
  *******************************************************************************/
class ListGenReportWithStatus : public ListGenReportWithDialog {
  private:
    // private members
    wxStatusBar* m_status;

    wxString m_TextField1;
    wxString m_TextField2;

    // private functions
    void InitMembers();

    // event functions
    void OnSelectionChange(wxListEvent& event);

  protected:
  public:
    // constructor
    ListGenReportWithStatus(wxWindow* parent, wxWindowID id, wxArrayString* pColsName, wxArrayInt* pColsSize = nullptr,
                            wxSize size = wxDefaultSize);

    // status setter and getter
    void SetStatusBar(wxStatusBar* status);

    wxStatusBar* GetStatusBar() {
        return m_status;
    }

    // set the status text
    void SetTextFields(const wxString& field1 = wxEmptyString, const wxString& field2 = wxEmptyString);

    // update the status
    bool UpdateStatus(int iField = STATUS_FIELD_ITEM_BOTH);

    // re-implement for adding / deleting update process
    virtual void AddItem();

    virtual void DeleteItem();
};

#endif
