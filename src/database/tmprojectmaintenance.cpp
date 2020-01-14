/***************************************************************************
 tmprojectmaintenance.cpp
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

#include "tmprojectmaintenance.h"

#include "database.h"

wxArrayString tmProjectMaintenance::_GetAllTables() {
  wxArrayString myTableNames;
  wxASSERT(m_DB);
  if (m_DB->DataBaseQuery(_T("SHOW TABLES"), true) == false) {
    return myTableNames;
  }

  if (m_DB->DataBaseGetResults(myTableNames) == false) {
    myTableNames.Clear();
  }
  return myTableNames;
}

bool tmProjectMaintenance::_DoCodeOnTables(const wxString &query, const wxString &errmsg, const wxString &sucessmsg) {
  wxASSERT(m_DB);
  m_Errors.Clear();
  m_Messages.Clear();

  wxArrayString myTableNames = _GetAllTables();
  for (unsigned int i = 0; i < myTableNames.GetCount(); i++) {
    if (m_DB->DataBaseQueryNoResults(wxString::Format(query, myTableNames[i]), true) == false) {
      m_Errors.Add(wxString::Format(errmsg, myTableNames[i]));
      continue;
    }
  }

  if (m_Errors.GetCount() > 0) {
    return false;
  }

  if (IsVerbose()) {
    m_Messages.Add(wxString::Format(sucessmsg, myTableNames.GetCount()));
  }

  return true;
}

bool tmProjectMaintenance::_CleanLayerOrphansKind(const wxString &tablegeom, const wxString &tablekind) {
  wxString myQuery = _T("DELETE FROM %s WHERE OBJECT_GEOM_ID NOT IN (SELECT OBJECT_ID FROM %s)");
  if (m_DB->DataBaseQueryNoResults(wxString::Format(myQuery, tablekind, tablegeom), true) == false) {
    return false;
  }
  long myDeletedRecords = m_DB->DataBaseGetAffectedRows();
  if (myDeletedRecords > 0) {
    m_Messages.Add(wxString::Format(_("%ld Record deleted from '%s'"), myDeletedRecords, tablekind));
  }
  return true;
}

bool tmProjectMaintenance::_CleanLayerOrphansAttributes(int geomtype, const wxString &generictablename) {
  wxString myQuery = _T("SELECT LAYER_INDEX FROM thematic_layers WHERE TYPE_CD = %d ORDER BY LAYER_INDEX");
  if (m_DB->DataBaseQuery(wxString::Format(myQuery, geomtype), true) == false) {
    return false;
  }

  if (m_DB->DataBaseHasResults() == false) {
    m_Messages.Add(wxString::Format(_("Nothing to clean for type code: %d"), geomtype));
    return true;
  }

  wxArrayLong myLayersIndexIDs;
  if (m_DB->DataBaseGetResults(myLayersIndexIDs) == false) {
    m_Errors.Add(_("Getting layer index Failed!"));
    return false;
  }

  for (unsigned int i = 0; i < myLayersIndexIDs.GetCount(); i++) {
    wxString myTablename = wxString::Format(_T("layer_at%ld"), myLayersIndexIDs[i]);
    if (m_DB->DataBaseQuery(wxString::Format(_T("SHOW TABLES LIKE \"%s\""), myTablename), true) == false) {
      return false;
    }
    if (m_DB->DataBaseHasResults() == false) {
      continue;
    }
    m_DB->DataBaseClearResults();

    wxString myQuery = _T("DELETE FROM %s WHERE OBJECT_ID NOT IN (SELECT OBJECT_ID FROM %s)");
    if (m_DB->DataBaseQueryNoResults(wxString::Format(myQuery, myTablename, generictablename), true) == false) {
      return false;
    }
    long myDeletedRecords = m_DB->DataBaseGetAffectedRows();
    if (myDeletedRecords > 0) {
      m_Messages.Add(wxString::Format(_("%ld Record deleted from '%s'"), myDeletedRecords, myTablename));
    }
  }
  return true;
}

tmProjectMaintenance::tmProjectMaintenance(const wxString &databasename, DataBase *database) {
  m_DB = NULL;
  m_DestroyDatabase = true;
  m_IsVerbose = false;
  if (database != NULL) {
    m_DB = database;
    m_DestroyDatabase = false;
  } else {
    wxFileName myDatabaseName(databasename);

    // opening master database
    m_DB = new DataBase(_T("./"));
    if (m_DB->DataBaseOpen(myDatabaseName.GetPath(), myDatabaseName.GetFullName()) == false) {
      m_Errors.Add(wxString::Format(_("Opening '%s' from '%s' Failed!"), myDatabaseName.GetFullName(),
                                    myDatabaseName.GetPath()));
      m_DB = NULL;
    }
  }
  wxASSERT(m_DB);
}

tmProjectMaintenance::~tmProjectMaintenance() {
  if (m_DestroyDatabase == true) {
    wxDELETE(m_DB);
  }
}

bool tmProjectMaintenance::RepairTables() {
  return _DoCodeOnTables(_T("REPAIR TABLE %s"), _("Repairing table '%s' failed"), _("Repairing %ld tables succeed!"));
}

bool tmProjectMaintenance::OptimizeTables() {
  return _DoCodeOnTables(_T("OPTIMIZE TABLE %s"), _("Optimizing table '%s' failed!"),
                         _("Optimizing %ld tables succeed!"));
}

bool tmProjectMaintenance::ClearOrphans() {
  bool bReturn = true;
  // cleaning in kind
  wxString myTablesGeom[] = {_T("generic_lines"), _T("generic_points"), _T("generic_labels")};
  wxString myTablesKind[] = {_T("generic_aat"), _T("generic_pat"), _T("generic_lat")};
  for (int i = 0; i < (sizeof(myTablesGeom) / sizeof(wxString)); i++) {
    if (_CleanLayerOrphansKind(myTablesGeom[i], myTablesKind[i]) == false) {
      m_Errors.Add(wxString::Format(_("Error cleaning orphans (kind, %s)!"), myTablesKind[i]));
      bReturn = false;
    }
  }

  // cleaning in layer_at tables (attributs)
  int myGeomtypes[] = {0, 1, 2};
  wxString myGenericTables[] = {_T("generic_lines"), _T("generic_points"), _T("generic_labels")};
  for (int i = 0; i < (sizeof(myGenericTables) / sizeof(wxString)); i++) {
    if (_CleanLayerOrphansAttributes(myGeomtypes[i], myGenericTables[i]) == false) {
      m_Errors.Add(wxString::Format(_("Error cleaning orphans (attributs, %s)!"), myGenericTables[i]));
      bReturn = false;
    }
  }
  return bReturn;
}

void tmProjectMaintenance::SetVerbose(bool value) {
  m_IsVerbose = value;
}
