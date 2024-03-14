/***************************************************************************
 tmprojectmerge.cpp
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

#include "tmprojectmerge.h"

#include "database_tm.h"
#include "database.h"
#include "databaseresult.h"

tmProjectMerge::tmProjectMerge(const wxString& masterprj, const wxString& slaveprj, DataBase* database) {
    m_MasterFileName = wxFileName(masterprj);
    m_SlaveFileName = wxFileName(slaveprj);
    SetVerbose(false);

    // open database if required.
    if (database != nullptr) {
        m_DB = database;
        m_manage_database = false;
        return;
    }

    m_DB = nullptr;
    m_manage_database = true;

    // opening master database
    m_DB = new DataBase(_T("./"));
    if (!m_DB->DataBaseOpen(m_MasterFileName.GetPath(), m_MasterFileName.GetFullName())) {
        m_Errors.Add(wxString::Format(_("Opening '%s' from '%s' Failed!"), m_MasterFileName.GetFullName(),
                                      m_MasterFileName.GetPath()));
        m_DB = nullptr;
    }
}

tmProjectMerge::~tmProjectMerge() {
    if (m_manage_database) {
        wxDELETE(m_DB);
    }
}

void tmProjectMerge::SetVerbose(bool value) {
    m_beVerbose = value;
}

wxString tmProjectMerge::GetLastError() {
    if (m_Errors.GetCount() > 0) {
        return m_Errors.Item(m_Errors.GetCount() - 1);
    }
    return wxEmptyString;
}

bool tmProjectMerge::_HasSameNumberRecords(DataBase* db, const wxString& tablename) {
    wxASSERT(db);
    long myNumMaster = wxNOT_FOUND;
    long myNumSlave = wxNOT_FOUND;
    wxString myQueryTemplate = _T("SELECT COUNT(*) FROM %s.%s c; ");

    wxString myQuery = wxString::Format(myQueryTemplate, m_MasterFileName.GetFullName(), tablename);
    if (!db->DataBaseQuery(myQuery)) {
        m_Errors.Add(
            wxString::Format(_("Counting record into '%s.%s' failed!"), m_MasterFileName.GetFullName(), tablename));
        return false;
    }

    db->DataBaseGetNextResult(myNumMaster);
    db->DataBaseClearResults();

    myQuery = wxString::Format(myQueryTemplate, m_SlaveFileName.GetFullName(), tablename);
    if (!db->DataBaseQuery(myQuery)) {
        m_Errors.Add(
            wxString::Format(_("Counting record into '%s.%s' failed!"), m_SlaveFileName.GetFullName(), tablename));
        return false;
    }

    db->DataBaseGetNextResult(myNumSlave);
    db->DataBaseClearResults();

    if (myNumMaster != myNumSlave) {
        m_Errors.Add(wxString::Format(_("Number of record into '%s' differs! (%ld vs %ld)"), tablename, myNumMaster,
                                      myNumSlave));
        return false;
    }

    if (IsVerbose()) {
        wxLogMessage(_("%ld records in '%s'"), myNumMaster, tablename);
    }

    return true;
}

bool tmProjectMerge::_HasSameRecords(DataBase* db, const wxString& query, long& errnumber, long& firstId,
                                     bool raiseError = true) {
    errnumber = 0;
    wxASSERT(db);

    if (!db->DataBaseQuery(query, true)) {
        return false;
    }

    wxArrayString myResults;
    while (db->DataBaseGetNextResult(myResults)) {
        if (firstId == 0) {
            myResults[0].ToLong(&firstId);
        }
        if (!raiseError) {
            continue;
        }
        errnumber++;
        wxString myErrorTxt = wxEmptyString;
        for (unsigned int i = 0; i < myResults.GetCount(); i++) {
            myErrorTxt.Append(myResults[i] + _T(" | "));
        }
        myErrorTxt.RemoveLast(3);
        m_Errors.Add(myErrorTxt);
    }
    db->DataBaseClearResults();
    if (errnumber > 0) {
        return false;
    }
    return true;
}

bool tmProjectMerge::_HasSimilarResults(DataBase* db, const wxString& query, long& errnumber) {
    // run query twice and compare results
    wxArrayString myMasterResults;
    wxArrayString mySlaveResults;

    // query 1
    if (!db->DataBaseQuery(wxString::Format(query, m_MasterFileName.GetFullName()), true)) {
        return false;
    }

    wxArrayString myResults;
    while (db->DataBaseGetNextResult(myResults)) {
        wxString myResultsAggregated = wxEmptyString;
        for (unsigned int i = 0; i < myResults.GetCount(); i++) {
            myResultsAggregated.Append(myResults[i] + _T(" "));
        }
        myMasterResults.Add(myResultsAggregated);
    }
    db->DataBaseClearResults();

    // query 2
    if (!db->DataBaseQuery(wxString::Format(query, m_SlaveFileName.GetFullName()), true)) {
        return false;
    }

    myResults.Clear();
    while (db->DataBaseGetNextResult(myResults)) {
        wxString myResultsAggregated = wxEmptyString;
        for (unsigned int i = 0; i < myResults.GetCount(); i++) {
            myResultsAggregated.Append(myResults[i] + _T(" "));
        }
        mySlaveResults.Add(myResultsAggregated);
    }
    db->DataBaseClearResults();

    // compare
    if (myMasterResults.GetCount() != mySlaveResults.GetCount()) {
        m_Errors.Add(wxString::Format(_("Number of results differs! (%d vs %d"), myMasterResults.GetCount(),
                                      mySlaveResults.GetCount()));
        return false;
    }

    errnumber = 0;
    for (unsigned int i = 0; i < myMasterResults.GetCount(); i++) {
        if (myMasterResults[i] != mySlaveResults[i]) {
            errnumber++;
            m_Errors.Add(myMasterResults[i] + _T(" || ") + mySlaveResults[i]);
        }
    }

    if (errnumber > 0) {
        return false;
    }

    return true;
}

bool tmProjectMerge::_CopyUpdateTable(const wxString& tablename, const wxString& keycol, wxArrayLong* oldids,
                                      wxArrayLong* newids) {
    wxASSERT(m_DB);
    wxASSERT(oldids);
    wxASSERT(newids);

    // check table existance
    if (!m_DB->DataBaseQuery(wxString::Format(_T("SHOW TABLES LIKE \"%s\""), tablename), true)) {
        return false;
    }
    if (!m_DB->DataBaseHasResults()) {
        if (IsVerbose()) {
            wxLogMessage(_("Table '%s' didn't exists and is ignored!"), tablename);
        }
        return true;
    }
    m_DB->DataBaseClearResults();

    if (!m_DB->DataBaseQuery(wxString::Format(_T("SHOW CREATE TABLE %s"), tablename), true)) {
        return false;
    }

    DataBaseResult* myResults = new DataBaseResult();
    if (!m_DB->DataBaseGetResults(myResults)) {
        m_Errors.Add(_("Error getting create information!"));
        return false;
    }
    wxString myCreateStmt = wxEmptyString;
    myResults->NextRow();
    myResults->GetValue(1, myCreateStmt);
    wxDELETE(myResults);
    if (myCreateStmt.IsEmpty()) {
        m_Errors.Add(_("Error getting create information!"));
        return false;
    }

    wxString myTempTableName = tablename + _T("_temp");
    myCreateStmt.Replace(tablename, myTempTableName);
    if (!m_DB->DataBaseQueryNoResults(myCreateStmt, true)) {
        return false;
    }

    wxSortedArrayString myUsedIds;
    // update object_kind (aka generic_aat)
    wxString myQuery = _T("INSERT INTO %s.%s SELECT * FROM %s.%s");
    if (!m_DB->DataBaseQueryNoResults(wxString::Format(myQuery, m_MasterFileName.GetFullName(), myTempTableName,
                                                       m_SlaveFileName.GetFullName(), tablename),
                                      true)) {
        return false;
    }

    for (int i = oldids->GetCount() - 1; i >= 0; i--) {
        if (i < oldids->GetCount() - 1 && newids->Item(i) == newids->Item(i + 1)) {
            wxLogMessage(_("Ignoring new ID: %ld, duplicate!"), newids->Item(i));
        }

        myQuery = wxString::Format(_T("UPDATE %s SET %s = %ld WHERE %s = %ld"), myTempTableName, keycol,
                                   newids->Item(i), keycol, oldids->Item(i));
        // wxLogDebug(myQuery);
        if (!m_DB->DataBaseQueryNoResults(myQuery, true)) {
            return false;
        }
    }
    if (IsVerbose()) {
        wxLogMessage(_("%d records updated into '%s'"), (int)oldids->GetCount(), tablename);
    }

    myQuery = _T("INSERT INTO %s SELECT * FROM %s");
    if (!m_DB->DataBaseQueryNoResults(wxString::Format(myQuery, tablename, myTempTableName), true)) {
        return false;
    }

    // drop temp table
    if (!m_DB->DataBaseQueryNoResults(wxString::Format(_T("DROP TABLE %s"), myTempTableName), true)) {
        return false;
    }

    return true;
}

bool tmProjectMerge::_MergeGeom(const wxString& geomtablename, const wxString& aatablename, int geomtype) {
    // get old ID's
    wxArrayLong myOldIds;

    wxString myQuery = _T("SELECT d.OBJECT_ID FROM %s.%s d ORDER BY d.OBJECT_ID");
    if (!m_DB->DataBaseQuery(wxString::Format(myQuery, m_SlaveFileName.GetFullName(), geomtablename), true)) {
        return false;
    }

    if (!m_DB->DataBaseGetResults(myOldIds)) {
        wxLogMessage(_("No data retrieved from table %s."), geomtablename);
        return true;
    }

    if (IsVerbose()) {
        wxLogMessage(_("%d Old ID retrieved in '%s'"), (int)myOldIds.GetCount(), geomtablename);
    }

    // Count objects in master
    if (!m_DB->DataBaseQuery(wxString::Format(_T("SELECT COUNT(OBJECT_ID) AS NumberOfObjects FROM %s.%s"),
                                              m_MasterFileName.GetFullName(), geomtablename),
                             true)) {
        return false;
    }

    long myMasterObjectsCount = 0;
    if (!m_DB->DataBaseGetNextResult(myMasterObjectsCount)) {
        m_DB->DataBaseClearResults();
        m_Errors.Add(_("Unable to get the number of objects!"));
        return false;
    }
    m_DB->DataBaseClearResults();

    long myMaxSlaveID = myOldIds[myOldIds.GetCount() - 1];
    long myMaxMasterID = wxNOT_FOUND;

    if (myMasterObjectsCount == 0) {
        myMaxMasterID = 0;
    } else {
        // Get highest ID in master
        if (!m_DB->DataBaseQuery(wxString::Format(_T("SELECT OBJECT_ID FROM %s.%s ORDER BY OBJECT_ID DESC LIMIT 1"),
                                                  m_MasterFileName.GetFullName(), geomtablename),
                                 true)) {
            return false;
        }

        if (!m_DB->DataBaseGetNextResult(myMaxMasterID)) {
            m_DB->DataBaseClearResults();
            m_Errors.Add(_("Unable to get max ID!"));
            return false;
        }
        m_DB->DataBaseClearResults();
    }

    // if slave ID is > master, we need to change autoincrement
    long myUsedMaxID = myMaxMasterID;
    if (myMaxSlaveID > myMaxMasterID) {
        myUsedMaxID = myMaxSlaveID;
        if (!m_DB->DataBaseQueryNoResults(
                wxString::Format(_T("ALTER TABLE %s AUTO_INCREMENT = %ld"), geomtablename, myUsedMaxID + 1), true)) {
            return false;
        }
    }
    if (IsVerbose()) {
        wxLogMessage(_("Changing slave ID's starting from: %ld"), myUsedMaxID);
    }

    // copy lines
    myQuery = _T("INSERT INTO %s.%s (OBJECT_GEOMETRY) SELECT d.OBJECT_GEOMETRY FROM %s.%s d ORDER BY d.OBJECT_ID");

    if (!m_DB->DataBaseQuery(wxString::Format(myQuery, m_MasterFileName.GetFullName(), geomtablename,
                                              m_SlaveFileName.GetFullName(), geomtablename),
                             true)) {
        return false;
    }

    // get new IDS
    wxArrayLong myNewIds;
    myQuery = _T("SELECT OBJECT_ID FROM %s.%s WHERE OBJECT_ID > %ld ORDER BY OBJECT_ID");
    if (!m_DB->DataBaseQuery(wxString::Format(myQuery, m_MasterFileName.GetFullName(), geomtablename, myUsedMaxID),
                             true)) {
        return false;
    }
    if (!m_DB->DataBaseGetResults(myNewIds)) {
        m_Errors.Add(_("Retrieving new ID Failed!"));
        return false;
    }

    if (myNewIds.GetCount() != myOldIds.GetCount()) {
        m_Errors.Add(
            wxString::Format(_("ID number mismatch! (%d vs %d)"), (int)myNewIds.GetCount(), (int)myOldIds.GetCount()));
        return false;
    }

    // copy object_kind from slave to temporary table into master
    if (!_CopyUpdateTable(aatablename, _T("OBJECT_GEOM_ID"), &myOldIds, &myNewIds)) {
        m_Errors.Add(_("Copying object kind failed!"));
        return false;
    }

    // copy and update layer_at
    myQuery = _T("SELECT LAYER_INDEX FROM thematic_layers WHERE TYPE_CD = %d ORDER BY LAYER_INDEX");
    if (!m_DB->DataBaseQuery(wxString::Format(myQuery, geomtype), true)) {
        return false;
    }
    wxArrayLong myLayersIndexIDs;
    if (!m_DB->DataBaseGetResults(myLayersIndexIDs)) {
        m_Errors.Add(_("Getting layer index Failed!"));
        return false;
    }

    bool hasError = false;
    for (unsigned int i = 0; i < myLayersIndexIDs.GetCount(); i++) {
        wxString myTableName = wxString::Format(_T("layer_at%ld"), myLayersIndexIDs[i]);
        if (!_CopyUpdateTable(myTableName, _T("OBJECT_ID"), &myOldIds, &myNewIds)) {
            m_Errors.Add(wxString::Format(_("Copying table: '%s' failed!"), myTableName));
            hasError = true;
            continue;
        }
    }

    return !hasError;
}

bool tmProjectMerge::_IsReady() {
    // some generic checks
    if (!m_MasterFileName.IsOk() || m_MasterFileName.GetFullName() == wxEmptyString) {
        m_Errors.Add(_("Master file name not correctely inited!"));
        return false;
    }

    if (!m_SlaveFileName.IsOk() || m_SlaveFileName.GetFullName() == wxEmptyString) {
        m_Errors.Add(_("Slave file name not correctely inited!"));
        return false;
    }

    // ensure in same directory
    if (m_MasterFileName.GetPath() != m_SlaveFileName.GetPath()) {
        m_Errors.Add(wxString::Format(
            _("Path from '%s' is different from '%s' path!\nChecking works only for projects in same directory"),
            m_MasterFileName.GetFullName(), m_SlaveFileName.GetFullName()));
        return false;
    }

    if (m_DB == nullptr) {
        m_Errors.Add(_("Database not initialized!"));
        return false;
    }

    return true;
}

bool tmProjectMerge::_GetIdMax(const wxString& dbName, const wxString& tableName, long& idMax) {
    idMax = wxNOT_FOUND;
    if (!m_DB->DataBaseQuery(
            wxString::Format(_T("SELECT OBJECT_ID FROM %s.%s ORDER BY OBJECT_ID DESC LIMIT 1"), dbName, tableName),
            true)) {
        return false;
    }
    if (!m_DB->DataBaseGetNextResult(idMax)) {
        m_DB->DataBaseClearResults();
        m_Errors.Add(wxString::Format(_("Unable to get max ID from %s (in %s)"), tableName, dbName));
        return true;
    }
    m_DB->DataBaseClearResults();

    return true;
}

bool tmProjectMerge::CheckSimilar() {
    // some generic checks
    if (!_IsReady()) {
        return false;
    }

    wxASSERT(m_DB);
    m_Errors.Clear();
    if (IsVerbose()) {
        wxLogMessage("Checking projects: '%s' with '%s'", m_MasterFileName.GetFullName(),
                     m_SlaveFileName.GetFullName());
    }

    // correct project version
    if (!m_DB->DataBaseQuery(_T("SELECT PRJ_VERSION FROM prj_settings"), true)) {
        return false;
    }
    long myVersion = wxNOT_FOUND;
    if (!m_DB->DataBaseGetNextResult(myVersion)) {
        return false;
    }
    m_DB->DataBaseClearResults();
    if (myVersion != TM_DATABASE_VERSION) {
        m_Errors.Add(
            wxString::Format(_("Wrong project version, works only for projects v.%d found (%ld)!"), 231, myVersion));
        return false;
    }

    // are layers similar ?
    if (!_HasSameNumberRecords(m_DB, _T("thematic_layers"))) {
        return false;
    }

    long myErrorLayers = 0;
    long firstFailingIdLayers = 0;
    wxString myQuery = wxString::Format(
        _T("SELECT c.LAYER_INDEX, c.LAYER_NAME, d.LAYER_NAME, c.TYPE_CD, d.TYPE_CD FROM %s.thematic_layers c, ")
        _T("%s.thematic_layers d WHERE c.LAYER_INDEX = d.LAYER_INDEX AND (c.LAYER_NAME <> d.LAYER_NAME OR c.TYPE_CD ")
        _T("<> ")
        _T("d.TYPE_CD)"),
        m_MasterFileName.GetFullName(), m_SlaveFileName.GetFullName());

    if (!_HasSameRecords(m_DB, myQuery, myErrorLayers, firstFailingIdLayers)) {
        m_Errors.Add(wxString::Format(_("%ld layers error found"), myErrorLayers));
        return false;
    }

    // Are objects similar ?
    long myErrorObjects = 0;
    long firstFailingIdObjects = 0;
    myQuery = wxString::Format(
        _T("SELECT c.OBJECT_ID, c.OBJECT_DESC_0, d.OBJECT_DESC_0, c.OBJECT_CD, d.OBJECT_CD, c.OBJECT_TYPE_CD, ")
        _T("d.OBJECT_TYPE_CD, c.THEMATIC_LAYERS_LAYER_INDEX, d.THEMATIC_LAYERS_LAYER_INDEX FROM %s.dmn_layer_object ")
        _T("c, ")
        _T("%s.dmn_layer_object d WHERE c.OBJECT_ID = d.OBJECT_ID AND (c.OBJECT_DESC_0 <> d.OBJECT_DESC_0 OR ")
        _T("c.OBJECT_CD <> d.OBJECT_CD OR c.OBJECT_TYPE_CD <> d.OBJECT_TYPE_CD OR c.THEMATIC_LAYERS_LAYER_INDEX <> ")
        _T("d.THEMATIC_LAYERS_LAYER_INDEX)"),
        m_MasterFileName.GetFullName(), m_SlaveFileName.GetFullName());

    if (!_HasSameNumberRecords(m_DB, _T("dmn_layer_object")) |
        !_HasSameRecords(m_DB, myQuery, myErrorObjects, firstFailingIdObjects, false)) {
        long myMaxMasterID = wxNOT_FOUND;
        if (!_GetIdMax(m_MasterFileName.GetFullName(), "dmn_layer_object", myMaxMasterID)) {
            return false;
        }

        long myMaxSlaveID = wxNOT_FOUND;
        if (!_GetIdMax(m_SlaveFileName.GetFullName(), "dmn_layer_object", myMaxSlaveID)) {
            return false;
        }

        // Get IDs of all entries with issues from the slave table
        myQuery = wxString::Format(_T("SELECT OBJECT_ID FROM %s.dmn_layer_object WHERE OBJECT_ID >= %ld"),
                                   m_SlaveFileName.GetFullName(), firstFailingIdObjects);

        if (!m_DB->DataBaseQuery(myQuery, true)) {
            return false;
        }

        wxArrayLong issuesIds;
        if (!m_DB->DataBaseGetResults(issuesIds)) {
            m_DB->DataBaseClearResults();
            return false;
        }
        m_DB->DataBaseClearResults();

        // Tables and entries to act on
        const wxString arrayTables[] = {"dmn_layer_object", "generic_pat", "generic_lat", "generic_aat",
                                        "shortcut_list"};
        wxArrayString tables = wxArrayString(sizeof(arrayTables), *arrayTables);
        const wxString arrayFields[] = {"OBJECT_ID", "OBJECT_VAL_ID", "OBJECT_VAL_ID", "OBJECT_VAL_ID", "OBJECT_ID"};
        wxArrayString fields = wxArrayString(sizeof(arrayFields), *arrayFields);

        // Change IDs to values out of range to avoid conflicts
        for (int i = 0; i < issuesIds.GetCount(); i++) {
            long originalId = issuesIds[i];
            long newId = wxMax(myMaxMasterID, myMaxSlaveID) + 1 + i;

            for (int j = 0; j < tables.GetCount(); j++) {
                myQuery = wxString::Format(_T("UPDATE %s.%s SET %s = %ld WHERE %s = %ld"),
                                           m_SlaveFileName.GetFullName(), tables.Item(j), fields.Item(j), newId,
                                           fields.Item(j), originalId);
                if (!m_DB->DataBaseQueryNoResults(myQuery, true)) {
                    return false;
                }
            }
        }

        // Look for equivalence in the main table
        for (int i = 0; i < issuesIds.GetCount(); i++) {
            long originalId = wxMax(myMaxMasterID, myMaxSlaveID) + 1 + i;
            long newId = 0;

            myQuery = wxString::Format(
                _T("SELECT c.OBJECT_ID, c.OBJECT_DESC_0, d.OBJECT_DESC_0, c.OBJECT_CD, d.OBJECT_CD, c.OBJECT_TYPE_CD, ")
                _T("d.OBJECT_TYPE_CD, c.THEMATIC_LAYERS_LAYER_INDEX, d.THEMATIC_LAYERS_LAYER_INDEX FROM ")
                _T("%s.dmn_layer_object c, ")
                _T("%s.dmn_layer_object d WHERE d.OBJECT_ID = %ld AND (c.OBJECT_DESC_0 = d.OBJECT_DESC_0 AND ")
                _T("c.OBJECT_CD = d.OBJECT_CD AND c.OBJECT_TYPE_CD = d.OBJECT_TYPE_CD AND ")
                _T("c.THEMATIC_LAYERS_LAYER_INDEX = ")
                _T("d.THEMATIC_LAYERS_LAYER_INDEX)"),
                m_MasterFileName.GetFullName(), m_SlaveFileName.GetFullName(), originalId);

            if (!m_DB->DataBaseQuery(myQuery, true)) {
                continue;
            }

            wxArrayString myResults;
            if (!m_DB->DataBaseGetNextResult(myResults)) {
                m_DB->DataBaseClearResults();
                continue;
            }
            myResults[0].ToLong(&newId);
            m_DB->DataBaseClearResults();

            for (int j = 0; j < tables.GetCount(); j++) {
                myQuery = wxString::Format(_T("UPDATE %s.%s SET %s = %ld WHERE %s = %ld"),
                                           m_SlaveFileName.GetFullName(), tables.Item(j), fields.Item(j), newId,
                                           fields.Item(j), originalId);
                if (!m_DB->DataBaseQueryNoResults(myQuery, true)) {
                    return false;
                }
            }
        }
    }

    // are layers_at similar
    long myErrorsLayerAt = 0;
    myQuery =
        _T("SELECT CAST(SUBSTR(TABLE_NAME FROM 9) AS UNSIGNED) LAYER_INDEX,COLUMN_NAME, COLUMN_TYPE, COLUMN_COMMENT ")
        _T("FROM INFORMATION_SCHEMA.COLUMNS WHERE table_schema = \"%s\" AND  table_name IN (SELECT TABLE_NAME FROM ")
        _T("INFORMATION_SCHEMA.TABLES WHERE TABLE_NAME LIKE \"layer_at%%\") AND COLUMN_NAME NOT IN ( 'OBJECT_ID', ")
        _T("'LAYER_AT_ID') ORDER BY LAYER_INDEX;");
    if (!_HasSimilarResults(m_DB, myQuery, myErrorsLayerAt)) {
        m_Errors.Add(wxString::Format(_("%ld layers_at error found"), myErrorsLayerAt));
        return false;
    }

    // are attributs and catalog similar ?
    if (!_HasSameNumberRecords(m_DB, _T("dmn_catalog"))) {
        return false;
    }

    long myErrorsCatalog = 0;
    long firstFailingIdCatalog = 0;
    myQuery = wxString::Format(
        _T("SELECT c.CATALOG_ID, c.CODE, c.DESCRIPTION_0, d.CATALOG_ID, d.CODE, d.DESCRIPTION_0 FROM %s.dmn_catalog ")
        _T("c, ")
        _T("%s.dmn_catalog d WHERE c.CATALOG_ID = d.CATALOG_ID AND (c.CODE <> d.CODE OR c.DESCRIPTION_0 <> ")
        _T("d.DESCRIPTION_0)"),
        m_MasterFileName.GetFullName(), m_SlaveFileName.GetFullName());
    if (!_HasSameRecords(m_DB, myQuery, myErrorsCatalog, firstFailingIdCatalog)) {
        m_Errors.Add(wxString::Format(_("%ld catalog error found"), myErrorsCatalog));
        return false;
    }

    // are dmn_layer_attribut similar ?
    if (!_HasSameNumberRecords(m_DB, _T("dmn_layer_attribut"))) {
        return false;
    }
    long myErrorLayerAttribute = 0;
    long firstFailingIdAttribute = 0;
    myQuery = wxString::Format(
        _T("SELECT c.ATTRIBUT_ID, c.LAYER_INDEX, c.ATTRIBUT_NAME, d.ATTRIBUT_ID, d.LAYER_INDEX, d.ATTRIBUT_NAME FROM ")
        _T("%s.dmn_layer_attribut c, %s.dmn_layer_attribut d WHERE c.ATTRIBUT_ID = d.ATTRIBUT_ID AND (c.LAYER_INDEX ")
        _T("<> ")
        _T("d.LAYER_INDEX or c.ATTRIBUT_NAME <> d.ATTRIBUT_NAME)"),
        m_MasterFileName.GetFullName(), m_SlaveFileName.GetFullName());
    if (!_HasSameRecords(m_DB, myQuery, myErrorLayerAttribute, firstFailingIdAttribute)) {
        m_Errors.Add(wxString::Format(_("%ld layer attributs error found"), myErrorLayerAttribute));
        return false;
    }

    if (!_HasSameNumberRecords(m_DB, _T("dmn_attribut_value"))) {
        return false;
    }
    return true;
}

bool tmProjectMerge::MergeIntoMaster() {
    // some generic checks
    if (!_IsReady()) {
        return false;
    }
    wxASSERT(m_DB);
    m_Errors.Clear();

    wxString myGeomTables[] = {_T("generic_lines"), _T("generic_points"), _T("generic_labels")};
    wxString myAATables[] = {_T("generic_aat"), _T("generic_pat"), _T("generic_lat")};
    int myGeomType[] = {0, 1, 2};
    wxString myDesc[] = {_("Lines"), _("Points"), _("Labels / Polygons")};

    for (int i = 0; i < (sizeof(myGeomType) / sizeof(int)); i++) {
        if (IsVerbose()) {
            wxLogMessage(_("Merging data from: ") + myDesc[i]);
        }
        if (!_MergeGeom(myGeomTables[i], myAATables[i], myGeomType[i])) {
            return false;
        }
    }

    // merge TOC
    wxString myQuery =
        _T("INSERT INTO %s.prj_toc (TYPE_CD, CONTENT_PATH, CONTENT_NAME, CONTENT_STATUS, GENERIC_LAYERS, RANK, ")
        _T("SYMBOLOGY, VERTEX_FLAGS) SELECT a.TYPE_CD, a.CONTENT_PATH, a.CONTENT_NAME, a.CONTENT_STATUS, ")
        _T("a.GENERIC_LAYERS, a.RANK, a.SYMBOLOGY, a.VERTEX_FLAGS FROM %s.prj_toc a, %s.prj_toc b WHERE ")
        _T("a.CONTENT_PATH <> b.CONTENT_PATH AND a.CONTENT_NAME <> b.CONTENT_NAME  AND a.CONTENT_ID > 5 GROUP BY ")
        _T("a.CONTENT_ID ");
    if (!m_DB->DataBaseQueryNoResults(wxString::Format(myQuery, m_MasterFileName.GetFullName(),
                                                       m_SlaveFileName.GetFullName(), m_MasterFileName.GetFullName()),
                                      true)) {
        return false;
    }

    // merge Frame
    myQuery = _T("INSERT INTO %s.generic_frame (OBJECT_GEOMETRY) SELECT s.OBJECT_GEOMETRY FROM %s.generic_frame s");
    if (!m_DB->DataBaseQueryNoResults(
            wxString::Format(myQuery, m_MasterFileName.GetFullName(), m_SlaveFileName.GetFullName()), true)) {
        return false;
    }

    // merge stats
    myQuery = _T(
        "INSERT INTO %s.prj_stats (DATE_START, CLICK, ATTRIBUTION, INTERSECTION, DATE_END) SELECT "
        "s.DATE_START, s.CLICK, s.ATTRIBUTION, s.INTERSECTION, s.DATE_END FROM %s.prj_stats s");
    if (!m_DB->DataBaseQueryNoResults(
            wxString::Format(myQuery, m_MasterFileName.GetFullName(), m_SlaveFileName.GetFullName()), true)) {
        return false;
    }

    return true;
}
