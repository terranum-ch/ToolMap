/***************************************************************************
 wxdirpickerctrlbest.h
 Upgrade a wxDirPickerCtrl control for TM
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

#include "wxdirpickerctrlbest.h"

wxDirPickerCtrlBest::wxDirPickerCtrlBest(wxWindow *parent, wxWindowID id, const wxString &path, const wxString &message,
                                         const wxPoint &pos, const wxSize &size, long style)
    : wxDirPickerCtrl(parent, id, path, message, pos, size, style) {}

wxDirPickerCtrlBest::~wxDirPickerCtrlBest() {}

void wxDirPickerCtrlBest::SetPathWithError(PATH_ERROR flags, const wxString &spath) {
  wxTextCtrl *txtctrl = nullptr;

  switch (flags) {
    case PATH_OK:
      SetPath(spath);
      break;
    case PATH_INVALID:
      // check that we have a textctrl
      // otherwise we just set a empty path
      if (HasTextCtrl()) {
        txtctrl = GetTextCtrl();
        if (txtctrl != nullptr) {
          txtctrl->SetValue(_("INVALID PATH"));
        }
      }
      break;
    default:
      SetPath(wxEmptyString);
      break;
  }
}
