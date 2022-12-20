/***************************************************************************
 sikind.h
 -------------------
 copyright            : (C) 2012 CREALP Lucien Schreiber
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
#include "database.h"
#include "databaseresult.h"

class siKind {
  private:
    wxString m_KindNameIn;
    wxArrayLong m_CodesIn;
    wxArrayString m_CodesOut2;
    wxArrayLong m_CodesRealOut;
    DataBase* m_Database;

    bool _LoadKindValue(const wxString& kindtxt);

  public:
    siKind();
    virtual ~siKind();

    bool LoadFromArray(const wxArrayString& kindtxt, DataBase* database);
    void Reset();

    long GetRealKind(long kindin);
    inline const wxString GetKindNameIn() const;
};

inline const wxString siKind::GetKindNameIn() const {
    return m_KindNameIn;
}
#endif
