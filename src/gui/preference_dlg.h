/***************************************************************************
 preference_dlg.h
 Display Preference dialog
 -------------------
 copyright            : (C) 2011 CREALP Lucien Schreiber
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef _PREFERENCE_DLG_H_
#define _PREFERENCE_DLG_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

// Include wxWidgets' headers
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/clrpicker.h>
#include <wx/config.h>
#include <wx/notebook.h>

class PreferenceDLG : public wxDialog {
 private:
  wxCheckBox *m_UpdateCheckCtrl;
  wxTextCtrl *m_ProxyInfoCtrl;
  wxColourPickerCtrl *m_SelColourCtrl;
  wxCheckBox *m_SelOutlineCtrl;
  wxCheckBox *m_RelPathCtrl;
  wxCheckBox *m_BuildOverviewCtrl;
  wxCheckBox* m_ctrl_debug_error;
  wxCheckBox* m_ctrl_debug_query;

  wxSlider *m_WaitingTimeCtrl;
  wxRadioButton *m_RamCtrl;
  wxRadioButton *m_MemoryCtrl;

  void _CreateControls();

 public:
  PreferenceDLG(wxWindow *parent, wxWindowID id = wxID_ANY, const wxString &title = _("Preferences"),
                const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize,
                long style = wxDEFAULT_DIALOG_STYLE);

  ~PreferenceDLG();

  virtual bool TransferDataToWindow();

  virtual bool TransferDataFromWindow();
};

#endif
