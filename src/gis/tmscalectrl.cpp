/***************************************************************************
 tmscalectrl.cpp
 Contain scale control component
 -------------------
 copyright            : (C) 2007 CREALP Lucien Schreiber
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "tmscalectrl.h"

DEFINE_EVENT_TYPE(tmEVT_SCALE_USER_CHANGED)

BEGIN_EVENT_TABLE(tmScaleCtrlCombo, wxComboBox)
EVT_TEXT_ENTER(wxID_ANY, tmScaleCtrlCombo::OnUserSetValue)
EVT_COMBOBOX(wxID_ANY, tmScaleCtrlCombo::OnUserSetValue)
END_EVENT_TABLE()

tmScaleCtrlCombo::tmScaleCtrlCombo(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size,
                                   const wxArrayString &arraystring)
    : wxComboBox(parent, id, _T(""), pos, size, arraystring, wxCB_DROPDOWN | wxTE_PROCESS_ENTER) {
  m_ParentEvent = parent;
}

tmScaleCtrlCombo::tmScaleCtrlCombo() {
  m_ParentEvent = NULL;
}

void tmScaleCtrlCombo::InitScaleFromDatabase(const wxArrayLong &scale_values) {
  // clear old values
  Clear();

  wxArrayString myStringScale;
  wxString tempScale = _T("");

  for (unsigned int i = 0; i < scale_values.GetCount(); i++) {
    ScaleTM::GetScaleFromLong(scale_values.Item(i), tempScale);
    myStringScale.Add(tempScale);
  }
  Append(myStringScale);
}

void tmScaleCtrlCombo::SetValueScale(const long &scale) {
  wxString myScaleString = _T("");
  ScaleTM::GetScaleFromLong(scale, myScaleString);
  SetValue(myScaleString);
}

void tmScaleCtrlCombo::OnUserSetValue(wxCommandEvent &event) {
  // get value and pass it to the layermanager
  wxString myScaleString = GetValue();
  long myScaleLong = 0;
  wxString myTempDisplayedString = _T("");

  if (myScaleString.IsEmpty()) return;

  // deal with scale of format 1:1000 or 1000
  if (myScaleString.GetChar(1) == ':') {
    myScaleLong = ScaleTM::GetScaleFromString(myScaleString);
  } else {
    myScaleString.ToLong(&myScaleLong);
    ScaleTM::GetScaleFromLong(myScaleLong, myTempDisplayedString);
    SetValue(myTempDisplayedString);
  }

  // check scale greather than zero
  if (myScaleLong <= 0) {
    return;
  }

  // all seems correct
  wxCommandEvent evt(tmEVT_SCALE_USER_CHANGED, wxID_ANY);
  evt.SetExtraLong(myScaleLong);
  m_ParentEvent->GetEventHandler()->QueueEvent(evt.Clone());
}
