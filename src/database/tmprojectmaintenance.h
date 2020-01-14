/***************************************************************************
 tmprojectmaintenance.h
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
#ifndef _TMPROJECTMAINTENANCE_H_
#define _TMPROJECTMAINTENANCE_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/filename.h>

class DataBase;

class tmProjectMaintenance
{
private:
    wxArrayString m_Messages;
    wxArrayString m_Errors;

    bool m_IsVerbose;
    DataBase *m_DB;
    bool m_DestroyDatabase;

    wxArrayString _GetAllTables();

    bool _DoCodeOnTables(const wxString &query, const wxString &errmsg, const wxString &sucessmsg);

    bool _CleanLayerOrphansKind(const wxString &tablegeom, const wxString &tablekind);

    bool _CleanLayerOrphansAttributes(int geomtype, const wxString &generictablename);

public:
    tmProjectMaintenance(const wxString &databasename, DataBase *database = NULL);

    virtual ~tmProjectMaintenance();

    //bool CheckTables();
    bool RepairTables();

    bool OptimizeTables();

    bool ClearOrphans();

    inline const bool IsVerbose() const;

    void SetVerbose(bool value);

    inline const wxArrayString GetErrors() const;

    inline const wxArrayString GetMessages() const;
};


inline const bool tmProjectMaintenance::IsVerbose() const
{
    return m_IsVerbose;
}


inline const wxArrayString tmProjectMaintenance::GetErrors() const
{
    return m_Errors;
}


inline const wxArrayString tmProjectMaintenance::GetMessages() const
{
    return m_Messages;
}

#endif
