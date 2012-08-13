/***************************************************************************
 tmmergeprojects.h
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
#ifndef _TMMERGEPROJECTS_H_
#define _TMMERGEPROJECTS_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <wx/filename.h>

class DataBase;

class tmMergeProjects {
  private:
    wxFileName m_MasterFileName;
    wxFileName m_SlaveFileName;
    DataBase * m_DB;

    bool m_beVerbose;
    wxArrayString m_Errors;

    bool _HasSameNumberRecords(DataBase * db, const wxString & tablename);
    bool _HasDifferenceResults(DataBase * db, const wxString & query, long & errnumber);
    bool _HasSimilarResults(DataBase * db, const wxString & query, long & errnumber);
    bool _IsReady();

    
  public:
    tmMergeProjects(const wxString & masterprj, const wxString & slaveprj);
    virtual ~tmMergeProjects();

    inline const bool IsVerbose() const;
    void SetVerbose(bool value);

    inline const wxArrayString GetErrors() const;
    wxString GetLastError();

    bool CheckSimilar();
    bool MergeIntoMaster();

};



inline const bool tmMergeProjects::IsVerbose() const {
  return m_beVerbose;
}



inline const wxArrayString tmMergeProjects::GetErrors() const {
  return m_Errors;
}

#endif
