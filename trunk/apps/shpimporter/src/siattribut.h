/***************************************************************************
 siattribut.h
 -------------------
 copyright            : (C) 2012 CREALP Lucien Schreiber 
 email                : lucien.schreiber at crealp dot vs dot ch
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef _SIATTRIBUT_H_
#define _SIATTRIBUT_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif


enum SIATTRIBUT_OPERATION {
  SIATTRIBUT_OPERATION_VALUE = 0,
  SIATTRIBUT_OPERATION_REPLACE,
  SIATTRIBUT_OPERATION_FIELD

};
class siAttributValue {
  public:
    siAttributValue();

    virtual ~siAttributValue();


  private:
    wxString m_ValueIn;

    wxString m_ValueOut;

    long m_ValueOutCode;

};
class siAttribut {
  public:
    siAttribut();

    virtual ~siAttribut();


  private:
    wxString m_AttributNameIn;

    wxString m_AttributNameOut;

    wxArrayLong m_AttributFilterIDs;

    SIATTRIBUT_OPERATION m_AttributOperation;

    siAttributValue m_Values;

};
#endif
