/***************************************************************************
 sikind.h
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
#ifndef _SIKIND_H_
#define _SIKIND_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif


class siKind {
  public:
    siKind();

    virtual ~siKind();


  private:
    wxString m_KindNameIn;

    wxArrayLong m_CodesIn;

    wxArrayLong m_CodesOut;

    wxArrayLong m_CodesRealOut;


  public:
    bool LoadFromTxt(const wxString & kindtxt);

};
#endif
