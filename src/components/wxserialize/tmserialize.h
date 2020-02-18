/***************************************************************************
 tmserialize.h
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

#ifndef _TM_SERIALIZE_H_
#define _TM_SERIALIZE_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

// Include wxWidgets' headers
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/tokenzr.h>  // for string tokenizer

const int TMSERIALIZE_VERSION = 1;  // increment each time something change
const wxString tmSERIAL_MAINSEP = _T("|");
const wxString tmSERIAL_AUXSEP = _T("§");

class tmSerialize {
 private:
  bool m_outdirection;
  wxString m_stream;
  wxStringTokenizer m_divStream;

  bool CanStore() {
    return m_outdirection;
  }

  bool CanRead();

  void AddSeparator() {
    m_stream.Append(tmSERIAL_MAINSEP);
  }
  // void AddAuxSeparator() {m_stream.Append(tmSERIAL_AUXSEP);}

  void WriteInt(int value);

  int ReadInt(const wxString &part);

  bool ReadStream(wxString &part);

 protected:
 public:
  tmSerialize();

  tmSerialize(wxString stream);

  virtual ~tmSerialize() {
    ;
  }

  virtual tmSerialize &operator<<(bool value);

  // virtual tmSerialize &operator << (wxString value);
  virtual tmSerialize &operator<<(const wxString &value);

  virtual tmSerialize &operator<<(const wxChar *pvalue);

  virtual tmSerialize &operator<<(wxColour value);

  virtual tmSerialize &operator<<(int value);

  virtual tmSerialize &operator<<(long value);

  virtual tmSerialize &operator>>(bool &value);

  virtual tmSerialize &operator>>(wxString &value);

  virtual tmSerialize &operator>>(wxColour &value);

  virtual tmSerialize &operator>>(int &value);

  virtual tmSerialize &operator>>(long &value);

  wxString GetString() {
    return m_stream;
  }

  bool IsStoring();

  bool IsOk() {
    return TRUE;
  }

  // for compatibility with wxSerialize
  void EnterObject() {
    ;
  }

  void LeaveObject() {
    ;
  }
};

#endif
