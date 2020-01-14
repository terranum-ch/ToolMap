/***************************************************************************
 tmprojectmerge.h
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
#ifndef _tmProjectMerge_H_
#define _tmProjectMerge_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include "database.h"
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/filename.h>

class DataBase;

class tmProjectMerge {
 private:
  wxFileName m_MasterFileName;
  wxFileName m_SlaveFileName;
  DataBase *m_DB;
  bool m_manage_database;

  bool m_beVerbose;
  wxArrayString m_Errors;

  bool _HasSameNumberRecords(DataBase *db, const wxString &tablename);

  bool _HasDifferenceResults(DataBase *db, const wxString &query, long &errnumber);

  bool _HasSimilarResults(DataBase *db, const wxString &query, long &errnumber);

  bool _CopyUpdateTable(const wxString &tablename, const wxString &keycol, wxArrayLong *oldids, wxArrayLong *newids);

  bool _MergeGeom(const wxString &geomtablename, const wxString &aatablename, int geomtype);

  bool _IsReady();

 public:
  tmProjectMerge(const wxString &masterprj, const wxString &slaveprj, DataBase *database = NULL);

  virtual ~tmProjectMerge();

  inline const bool IsVerbose() const;

  void SetVerbose(bool value);

  inline const wxArrayString GetErrors() const;

  wxString GetLastError();

  bool CheckSimilar();

  bool MergeIntoMaster();

  DataBase *GetDatabaseRef() {
    return m_DB;
  }
};

inline const bool tmProjectMerge::IsVerbose() const {
  return m_beVerbose;
}

inline const wxArrayString tmProjectMerge::GetErrors() const {
  return m_Errors;
}

#endif
