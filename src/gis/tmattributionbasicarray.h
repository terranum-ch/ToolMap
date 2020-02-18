/***************************************************************************
 tmattributionbasicarray.cpp
 An array for storing basic attributions values
 -------------------
 copyright : (C) 2009 CREALP Lucien Schreiber
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

#ifndef _TM_ATTRIBUTION_BASIC_ARRAY_H_
#define _TM_ATTRIBUTION_BASIC_ARRAY_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

// Include wxWidgets' headers
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

class tmAttributionBasic : public wxObject {
 private:
  void InitMemberValues();

 public:
  // ctor - dtor
  tmAttributionBasic();

  ~tmAttributionBasic();

  long m_Oid;
  wxArrayLong m_Values;
};

WX_DECLARE_OBJARRAY(tmAttributionBasic, tmAttributionBasicArray);

class tmLayerValue : public wxObject {
 private:
  void InitMemberValues();

 public:
  tmLayerValue();

  ~tmLayerValue();

  long m_Oid;
  wxString m_Value;
};

WX_DECLARE_OBJARRAY(tmLayerValue, tmLayerValueArray);

#endif
