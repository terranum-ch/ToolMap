/***************************************************************************
 queries_panel_dlg.h
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

#ifndef _TM_QUERIES_PANEL_DLG_H_
#define _TM_QUERIES_PANEL_DLG_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

// Include wxWidgets' headers
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/statline.h>  // for static_line

#include "../gis/tmlayerpropertiesdef.h"  // for target definition

/***************************************************************************/ /**
  @brief Queries panel dialog
  @details This is the first very simple version of the dialog. In a future
  version a better version will be used.
  @todo Implement a new more complete
  version of this dialog
  @author Lucien Schreiber (c) CREALP 2008
  @date 09 November 2008
  *******************************************************************************/
class QueriesListDLG : public wxDialog {
 private:
 protected:
  wxTextCtrl *m_QueriesName;
  wxTextCtrl *m_QueriesDescription;
  wxChoice *m_Target;

 public:
  QueriesListDLG(wxWindow *parent, wxWindowID id = wxID_ANY, const wxString &title = wxT("Edit queries"),
                 const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize,
                 long style = wxRESIZE_BORDER | wxDEFAULT_DIALOG_STYLE);

  ~QueriesListDLG();

  // query name
  wxString GetQueriesName() {
    return m_QueriesName->GetValue();
  }

  void SetQueriesName(const wxString &name) {
    m_QueriesName->SetValue(name);
  }

  // query description
  wxString GetQueriesDescription() {
    return m_QueriesDescription->GetValue();
  }

  void SetQueriesDescription(const wxString &desc) {
    m_QueriesDescription->SetValue(desc);
  }

  // query target
  int GetQueriesTarget() {
    return m_Target->GetSelection();
  }

  void SetQueriesTarget(int toc_generic_name) {
    m_Target->SetSelection(toc_generic_name);
  }
};

#endif
