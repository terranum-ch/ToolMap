/***************************************************************************
 tmserialize.cpp
 for serializating object in a simple way
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

#include "tmserialize.h"

// serializing data out
tmSerialize::tmSerialize() {
  m_outdirection = TRUE;
  m_stream = _T("");
  m_divStream.SetString(wxEmptyString, tmSERIAL_MAINSEP);
}

// serializing data in
tmSerialize::tmSerialize(wxString stream) {
  m_outdirection = FALSE;
  m_stream = stream;
  m_divStream.SetString(m_stream, tmSERIAL_MAINSEP);
}

bool tmSerialize::CanRead() {
  if (m_outdirection) {
    wxLogDebug(_T("Error, stream is in writing mode"));
    return FALSE;
  }

  if (m_stream.IsEmpty()) {
    wxLogDebug(_T("Nothing to read, stream empty"));
    return FALSE;
  }
  return TRUE;
}

void tmSerialize::WriteInt(int value) {
  if (CanStore()) {
    m_stream.Append(wxString::Format(_T("%d"), value));
  }
}

int tmSerialize::ReadInt(const wxString &part) {
  long lvalue = 0;
  if (!part.ToLong(&lvalue)) {
    wxLogDebug(_T("Error trying to convert string to integer"));
  }

  return lvalue;
}

bool tmSerialize::ReadStream(wxString &part) {
  if (CanRead()) {
    if (m_divStream.HasMoreTokens()) {
      part = m_divStream.GetNextToken();
      return true;
    }
  }
  part = wxEmptyString;
  return false;
}

tmSerialize &tmSerialize::operator<<(bool value) {
  if (CanStore()) {
    WriteInt((int)value);
    AddSeparator();
  }
  return *this;
}

/*
tmSerialize & tmSerialize::operator << (wxString value)
{
 if (CanStore())
 {
 m_stream.Append(value);
 AddSeparator();
 }
 return *this;
}*/

tmSerialize &tmSerialize::operator<<(const wxString &value) {
  if (CanStore()) {
    m_stream.Append(value);
    AddSeparator();
  }
  return *this;
}

tmSerialize &tmSerialize::operator<<(const wxChar *pvalue) {
  wxString value(pvalue);
  if (CanStore()) {
    m_stream.Append(value);
    AddSeparator();
  }
  return *this;
}

tmSerialize &tmSerialize::operator<<(wxColour value) {
  if (CanStore()) {
    m_stream.Append(value.GetAsString(wxC2S_CSS_SYNTAX));
    AddSeparator();
  }
  return *this;
}

tmSerialize &tmSerialize::operator<<(int value) {
  if (CanStore()) {
    WriteInt(value);
    AddSeparator();
  }
  return *this;
}

tmSerialize &tmSerialize::operator<<(long value) {
  if (CanStore()) {
    m_stream.Append(wxString::Format(_T("%ld"), value));
    AddSeparator();
  }
  return *this;
}

tmSerialize &tmSerialize::operator>>(bool &value) {
  wxString partstream = wxEmptyString;
  if (ReadStream(partstream)) {
    value = (bool)ReadInt(partstream);
  }
  return *this;
}

tmSerialize &tmSerialize::operator>>(wxString &value) {
  wxString partstream = wxEmptyString;
  if (ReadStream(partstream)) {
    value = partstream;
  }
  return *this;
}

tmSerialize &tmSerialize::operator>>(wxColour &value) {
  wxString partstream = wxEmptyString;
  if (ReadStream(partstream)) {
    wxColour tmpcol(partstream);
    if (tmpcol.IsOk()) value = tmpcol;
  }
  return *this;
}

tmSerialize &tmSerialize::operator>>(int &value) {
  wxString partstream = wxEmptyString;
  if (ReadStream(partstream)) {
    value = ReadInt(partstream);
  }
  return *this;
}

tmSerialize &tmSerialize::operator>>(long &value) {
  wxString partstream = wxEmptyString;
  if (ReadStream(partstream)) {
    partstream.ToLong(&value);
  }
  return *this;
}

bool tmSerialize::IsStoring() {
  if (m_outdirection) return TRUE;
  return FALSE;
}
