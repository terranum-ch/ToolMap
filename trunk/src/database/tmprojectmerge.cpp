/***************************************************************************
 tmprojectmerge.cpp
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

#include "tmprojectmerge.h"
#include "database.h"
#include "databaseresult.h"

tmProjectMerge::tmProjectMerge(const wxString & masterprj, const wxString & slaveprj) {
    m_MasterFileName = wxFileName(masterprj);
    m_SlaveFileName = wxFileName(slaveprj);
    SetVerbose(false);
    m_DB = NULL;
    
    // opening master database
    m_DB = new DataBase(_T("./"));
    if(m_DB->DataBaseOpen(m_MasterFileName.GetPath(), m_MasterFileName.GetFullName())==false){
        m_Errors.Add(wxString::Format(_("Opening '%s' from '%s' Failed!"), m_MasterFileName.GetFullName(), m_MasterFileName.GetPath()));
        m_DB = NULL;
    }    
}



tmProjectMerge::~tmProjectMerge() {
    wxDELETE(m_DB);
}




void tmProjectMerge::SetVerbose(bool value) {
  m_beVerbose = value;
}



wxString tmProjectMerge::GetLastError() {
    if (m_Errors.GetCount() > 0) {
        return  m_Errors.Item(m_Errors.GetCount() -1);
    }
    return wxEmptyString;
}


bool tmProjectMerge::_HasSameNumberRecords(DataBase * db, const wxString & tablename) {
    wxASSERT(db);
    long myNumMaster = wxNOT_FOUND;
    long myNumSlave = wxNOT_FOUND;
    wxString myQueryTemplate= _T("SELECT COUNT(*) FROM %s.%s c; ");
    
    wxString myQuery = wxString::Format(myQueryTemplate, m_MasterFileName.GetFullName(), tablename);
    if (db->DataBaseQuery(myQuery) == false) {
        m_Errors.Add(wxString::Format(_("Counting record into '%s.%s' failed!"),m_MasterFileName.GetFullName(), tablename));
        return false;
    }
    
    db->DataBaseGetNextResult(myNumMaster);
    db->DataBaseClearResults();
    
    myQuery = wxString::Format(myQueryTemplate, m_SlaveFileName.GetFullName(), tablename);
    if (db->DataBaseQuery(myQuery) == false) {
        m_Errors.Add(wxString::Format(_("Counting record into '%s.%s' failed!"),m_SlaveFileName.GetFullName(), tablename));
        return false;
    }
    
    db->DataBaseGetNextResult(myNumSlave);
    db->DataBaseClearResults();
    
    if (myNumMaster != myNumSlave) {
        m_Errors.Add(wxString::Format(_("Number of record into '%s' differs! (%ld vs %ld)"), tablename, myNumMaster, myNumSlave));
        return false;
    }
    
    if (IsVerbose()) {
        wxLogMessage(_("%ld records in '%s'"), myNumMaster, tablename);
    }
    
    return true;
}



bool tmProjectMerge::_HasDifferenceResults(DataBase * db, const wxString & query, long & errnumber) {
    errnumber = 0;
    wxASSERT(db);
    
    if (db->DataBaseQuery(query, true)==false) {
        return false;
    }
    
    wxArrayString myResults;
    while (db->DataBaseGetNextResult(myResults) == true) {
        errnumber++;
        wxString myErrorTxt = wxEmptyString;
        for (unsigned int i = 0; i< myResults.GetCount(); i++) {
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



bool tmProjectMerge::_HasSimilarResults(DataBase * db, const wxString & query, long & errnumber) {
    // run query twice and compare results
    wxArrayString myMasterResults;
    wxArrayString mySlaveResults;
    
    // query 1
    if (db->DataBaseQuery(wxString::Format(query, m_MasterFileName.GetFullName()), true)==false) {
        return false;
    }
        
    wxArrayString myResults;
    while (db->DataBaseGetNextResult(myResults)==true) {
        wxString myResultsAggregated = wxEmptyString;
        for (unsigned int i = 0; i< myResults.GetCount(); i++) {
            myResultsAggregated.Append(myResults[i] + _T(" "));
        }
        myMasterResults.Add(myResultsAggregated);
    }
    db->DataBaseClearResults();
    
    // query 2
    if (db->DataBaseQuery(wxString::Format(query, m_SlaveFileName.GetFullName()), true)==false) {
        return false;
    }
    
    myResults.Clear();
    while (db->DataBaseGetNextResult(myResults)==true) {
        wxString myResultsAggregated = wxEmptyString;
        for (unsigned int i = 0; i< myResults.GetCount(); i++) {
            myResultsAggregated.Append(myResults[i] + _T(" "));
        }
        mySlaveResults.Add(myResultsAggregated);
    }
    db->DataBaseClearResults();
    
    // compare
    if (myMasterResults.GetCount() != mySlaveResults.GetCount()) {
        m_Errors.Add(wxString::Format(_("Number of results differs! (%d vs %d"), myMasterResults.GetCount(), mySlaveResults.GetCount()));
        return false;
    }
    
    errnumber = 0;
    for (unsigned int i = 0; i<myMasterResults.GetCount(); i++) {
        if (myMasterResults[i] != mySlaveResults[i]) {
            errnumber ++;
            m_Errors.Add(myMasterResults[i] + _T(" || ") + mySlaveResults[i]);
        }
    }
    
    if (errnumber > 0) {
        return false;
    }
    
    return true;
}



bool tmProjectMerge::_CopyUpdateTable(const wxString & tablename, const wxString & keycol, wxArrayLong * oldids, wxArrayLong * newids){
    wxASSERT(m_DB);
    wxASSERT(oldids);
    wxASSERT(newids);
    
    // check table existance
    if (m_DB->DataBaseQuery(wxString::Format(_T("SHOW TABLES LIKE \"%s\""),tablename),true)==false) {
        return false;
    }
    if (m_DB->DataBaseHasResults() == false) {
        if (IsVerbose()) {
            wxLogMessage(_("Table '%s' didn't exists and is ignored!"), tablename);
        }
        return true;
    }
    m_DB->DataBaseClearResults();
    
        
    if (m_DB->DataBaseQuery(wxString::Format(_T("SHOW CREATE TABLE %s"), tablename),true)==false) {
        return false;
    }
    
    DataBaseResult * myResults = new DataBaseResult();
    if (m_DB->DataBaseGetResults(myResults) == false) {
        m_Errors.Add(_("Error getting create information!"));
        return false;
    }
    wxString myCreateStmt = wxEmptyString;
    myResults->NextRow();
    myResults->GetValue(1, myCreateStmt);
    wxDELETE(myResults);
    if (myCreateStmt.IsEmpty() == true) {
        m_Errors.Add(_("Error getting create information!"));
        return false;
    }
    
    wxString myTempTableName = tablename + _T("_temp");
    myCreateStmt.Replace(tablename, myTempTableName);
    if (m_DB->DataBaseQueryNoResults(myCreateStmt,true)==false) {
        return false;
    }
    
    wxSortedArrayString myUsedIds;
    // update object_kind (aka generic_aat)
    wxString myQuery = _T("INSERT INTO %s.%s SELECT * FROM %s.%s");
    if (m_DB->DataBaseQueryNoResults(wxString::Format(myQuery, m_MasterFileName.GetFullName(),myTempTableName, m_SlaveFileName.GetFullName(), tablename),true)==false) {
        return false;
    }
    
    for (int i = oldids->GetCount()-1; i >= 0; i--) {
        if (i<oldids->GetCount() - 1  && newids->Item(i) == newids->Item(i+1)) {
            wxLogMessage(_("Ignoring new ID: %ld, duplicate!"), newids->Item(i));
        }
        
        myQuery = wxString::Format(_T("UPDATE %s SET %s = %ld WHERE %s = %ld"), myTempTableName, keycol, newids->Item(i), keycol, oldids->Item(i));
        //wxLogDebug(myQuery);
        if (m_DB->DataBaseQueryNoResults(myQuery,true)==false){
            return false;
        }
    }
    if (IsVerbose()) {
        wxLogMessage(_("%ld records updated into '%s'"), oldids->GetCount(), tablename);
    }

    myQuery = _T("INSERT INTO %s SELECT * FROM %s");
    if (m_DB->DataBaseQueryNoResults(wxString::Format(myQuery, tablename, myTempTableName), true)==false) {
        return false;
    }
    
    // drop temp table
    if (m_DB->DataBaseQueryNoResults(wxString::Format(_T("DROP TABLE %s"), myTempTableName), true)==false) {
        return false;
    }

    return true;
}



bool tmProjectMerge::_MergeGeom (const wxString & geomtablename, const wxString & aatablename, int geomtype){
    // get old ID's
    wxArrayLong myOldIds;
    
    wxString myQuery = _T("SELECT d.OBJECT_ID FROM %s.%s d ORDER BY d.OBJECT_ID");
    if (m_DB->DataBaseQuery(wxString::Format(myQuery, m_SlaveFileName.GetFullName(), geomtablename), true)==false) {
        return false;
    }
    
    if(m_DB->DataBaseGetResults(myOldIds)==false){
        m_Errors.Add(_("regaining Old ID Failed!"));
        return false;
    }
    
    if (IsVerbose()) {
        wxLogMessage(_("%ld Old ID regained in '%s'"), myOldIds.GetCount(), geomtablename);
    }
    
    // get highest ID in master
    if (m_DB->DataBaseQuery(wxString::Format(_T("SELECT OBJECT_ID FROM %s.%s ORDER BY OBJECT_ID DESC LIMIT 1"), m_MasterFileName.GetFullName(), geomtablename),true)==false) {
        return false;
    }
    
    long myMaxSlaveID = myOldIds[myOldIds.GetCount()-1];
    long myMaxMasterID = wxNOT_FOUND;
    if (m_DB->DataBaseGetNextResult(myMaxMasterID)==false) {
        m_DB->DataBaseClearResults();
        m_Errors.Add(_("Unable to get max ID!"));
        return false;
    }
    m_DB->DataBaseClearResults();
    
    
    // if slave ID is > master, we need to change autoincrement
    long myUsedMaxID = myMaxMasterID;
    if (myMaxSlaveID > myMaxMasterID) {
        myUsedMaxID = myMaxSlaveID;
        if (m_DB->DataBaseQueryNoResults(wxString::Format(_T("ALTER TABLE %s AUTO_INCREMENT = %ld"), geomtablename, myUsedMaxID + 1),true)==false) {
            return false;
        }
    }
    if (IsVerbose()) {
        wxLogMessage(_("Changing slave ID's starting from: %ld"), myUsedMaxID);
    }
    
    // copy lines
    myQuery = _T("INSERT INTO %s.%s (OBJECT_GEOMETRY) SELECT d.OBJECT_GEOMETRY FROM %s.%s d ORDER BY d.OBJECT_ID");
    
    if (m_DB->DataBaseQuery(wxString::Format(myQuery, m_MasterFileName.GetFullName(), geomtablename, m_SlaveFileName.GetFullName(), geomtablename), true)==false) {
        return false;
    }
    
    // get new IDS
    wxArrayLong myNewIds;
    myQuery = _T("SELECT OBJECT_ID FROM %s.%s WHERE OBJECT_ID > %ld ORDER BY OBJECT_ID");
    if (m_DB->DataBaseQuery(wxString::Format(myQuery, m_MasterFileName.GetFullName(), geomtablename, myUsedMaxID),true)==false) {
        return false;
    }
    if(m_DB->DataBaseGetResults(myNewIds)==false){
        m_Errors.Add(_("regaining New ID Failed!"));
        return false;
    }
    
    if (myNewIds.GetCount() != myOldIds.GetCount()) {
        m_Errors.Add(wxString::Format(_("ID number mismatch! (%ld vs %ld)"), myNewIds.GetCount(), myOldIds.GetCount()));
        return false;
    }
    
    
    // copy object_kind from slave to temporary table into master
    if (_CopyUpdateTable(aatablename, _T("OBJECT_GEOM_ID"), &myOldIds, &myNewIds)==false) {
        m_Errors.Add(_("Copying object kind failed!"));
        return false;
    }
    
    // copy and update layer_at
    myQuery = _T("SELECT LAYER_INDEX FROM thematic_layers WHERE TYPE_CD = %d ORDER BY LAYER_INDEX");
    if (m_DB->DataBaseQuery(wxString::Format(myQuery, geomtype),true)==false) {
        return false;
    }
    wxArrayLong myLayersIndexIDs;
    if (m_DB->DataBaseGetResults(myLayersIndexIDs)==false) {
        m_Errors.Add(_("Getting layer index Failed!"));
        return false;
    }
    
    bool hasError = false;
    for (unsigned int i = 0; i< myLayersIndexIDs.GetCount(); i++) {
        wxString myTableName = wxString::Format(_T("layer_at%ld"), myLayersIndexIDs[i]);
        if(_CopyUpdateTable(myTableName, _T("OBJECT_ID"), &myOldIds, &myNewIds)==false){
            m_Errors.Add(wxString::Format(_("Copying table: '%s' failed!"),myTableName));
            hasError = true;
            continue;
        }
    }
    
    if (hasError) {
        return false;
    }
    return true;
}




bool tmProjectMerge::_IsReady(){
    // some generic checks
    if (m_MasterFileName.IsOk() == false || m_MasterFileName.GetFullName() == wxEmptyString) {
        m_Errors.Add(_("Master file name not correctely inited!"));
        return false;
    }
    
    if (m_SlaveFileName.IsOk() == false || m_SlaveFileName.GetFullName() == wxEmptyString) {
        m_Errors.Add(_("Slave file name not correctely inited!"));
        return false;
    }
    
    // ensure in same directory
    if (m_MasterFileName.GetPath() != m_SlaveFileName.GetPath()) {
        m_Errors.Add(wxString::Format(_("Path from '%s' is different from '%s' path!\nChecking works only for projects in same directory"),m_MasterFileName.GetFullName(), m_SlaveFileName.GetFullName()));
        return false;
    }
    
    if (m_DB == NULL) {
        m_Errors.Add(_("Database not inited!"));
        return false;
    }
    
    return true;
}



bool tmProjectMerge::CheckSimilar() {
    
    // some generic checks
    if (_IsReady() == false) {
        return false;
    }
    
    wxASSERT(m_DB);
    m_Errors.Clear();
    if (IsVerbose()){
        wxLogMessage("Checking projects: '%s' with '%s'", m_MasterFileName.GetFullName(), m_SlaveFileName.GetFullName());
    }
    
    // correct project version
    if (m_DB->DataBaseQuery(_T("SELECT PRJ_VERSION FROM prj_settings"),true)==false) {
        return false;
    }
    long myVersion = wxNOT_FOUND;
    if (m_DB->DataBaseGetNextResult(myVersion)==false) {
        return false;
    }
    m_DB->DataBaseClearResults();
    if (myVersion != 221) {
        m_Errors.Add(wxString::Format(_("Wrong Database version, works only for database: %d found (%ld)!"), 221, myVersion));
        return false;
    }
    
    
    // are layers similar ?
    if (_HasSameNumberRecords(m_DB, _T("thematic_layers"))==false) {
        return false;
    }
    
    long myErrorLayers = 0;
    wxString myQuery = wxString::Format(_T("SELECT c.LAYER_INDEX, c.LAYER_NAME, d.LAYER_NAME, c.TYPE_CD, d.TYPE_CD FROM %s.thematic_layers c, %s.thematic_layers d WHERE c.LAYER_INDEX = d.LAYER_INDEX AND (c.LAYER_NAME <> d.LAYER_NAME OR c.TYPE_CD <> d.TYPE_CD)"), m_MasterFileName.GetFullName(), m_SlaveFileName.GetFullName());
    
    if (_HasDifferenceResults(m_DB, myQuery, myErrorLayers) == false) {
        m_Errors.Add(wxString::Format(_("%ld layers error found"), myErrorLayers));
        return false;
    }
    
     
    
    // are objects similar ?
    if (_HasSameNumberRecords(m_DB, _T("dmn_layer_object"))==false) {
        return false;
    }
    
    long myErrorObjects = 0;
    myQuery = wxString::Format(_T("SELECT c.OBJECT_ID, c.OBJECT_DESC, d.OBJECT_DESC, c.OBJECT_CD, d.OBJECT_CD, c.OBJECT_TYPE_CD, d.OBJECT_TYPE_CD, c.THEMATIC_LAYERS_LAYER_INDEX, d.THEMATIC_LAYERS_LAYER_INDEX FROM %s.dmn_layer_object c, %s.dmn_layer_object d WHERE c.OBJECT_ID = d.OBJECT_ID AND (c.OBJECT_DESC <> d.OBJECT_DESC OR c.OBJECT_CD <> d.OBJECT_CD OR c.OBJECT_TYPE_CD <> d.OBJECT_TYPE_CD OR c.THEMATIC_LAYERS_LAYER_INDEX <> d.THEMATIC_LAYERS_LAYER_INDEX)"), m_MasterFileName.GetFullName(), m_SlaveFileName.GetFullName());
    
    if (_HasDifferenceResults(m_DB, myQuery, myErrorObjects) == false) {
        m_Errors.Add(wxString::Format(_("%ld object error found"), myErrorObjects));
        return false;
    }

    
    // are layers_at similar
    long myErrorsLayerAt = 0;
    myQuery = _T("SELECT CAST(SUBSTR(TABLE_NAME FROM 9) AS UNSIGNED) LAYER_INDEX,COLUMN_NAME, COLUMN_TYPE, COLUMN_COMMENT FROM INFORMATION_SCHEMA.COLUMNS WHERE table_schema = \"%s\" AND  table_name IN (SELECT TABLE_NAME FROM                                     INFORMATION_SCHEMA.TABLES WHERE TABLE_NAME LIKE \"layer_at%\") AND COLUMN_NAME NOT IN ( 'OBJECT_ID', 'LAYER_AT_ID') ORDER BY LAYER_INDEX;");
    if (_HasSimilarResults(m_DB, myQuery, myErrorsLayerAt) == false) {
        m_Errors.Add(wxString::Format(_("%ld layers_at error found"), myErrorsLayerAt));
        return false;
    }
    
    return true;
}



bool tmProjectMerge::MergeIntoMaster() {    
    // some generic checks
    if (_IsReady() == false) {
        return false;
    }
    wxASSERT(m_DB);
    m_Errors.Clear();

    wxString myGeomTables[] = {_T("generic_lines"), _T("generic_points"), _T("generic_labels")};
    wxString myAATables[] = {_T("generic_aat"), _T("generic_pat"), _T("generic_lat")};
    int myGeomType[] = {0, 1, 2};
    wxString myDesc [] = {_("Lines"), _("Points"), _("Labels / Polygons")};
    
    for (int i = 0; i<(sizeof(myGeomType) / sizeof(int)); i++) {
        if (IsVerbose()) {
            wxLogMessage(_("Merging data from: ") + myDesc[i]);
        }
        if(_MergeGeom(myGeomTables[i], myAATables[i], myGeomType[i]) == false){
            return false;
        }
    }
    
    
    // merge TOC
    wxString myQuery = _T("INSERT INTO %s.prj_toc (TYPE_CD, CONTENT_PATH, CONTENT_NAME, CONTENT_STATUS, GENERIC_LAYERS, RANK, SYMBOLOGY, VERTEX_FLAGS) SELECT a.TYPE_CD, a.CONTENT_PATH, a.CONTENT_NAME, a.CONTENT_STATUS, a.GENERIC_LAYERS, a.RANK, a.SYMBOLOGY, a.VERTEX_FLAGS FROM %s.prj_toc a, %s.prj_toc b WHERE a.CONTENT_PATH <> b.CONTENT_PATH AND a.CONTENT_NAME <> b.CONTENT_NAME  AND a.CONTENT_ID > 5 GROUP BY a.CONTENT_ID ");
    if (m_DB->DataBaseQueryNoResults(wxString::Format(myQuery, m_MasterFileName.GetFullName(), m_SlaveFileName.GetFullName(), m_MasterFileName.GetFullName()), true)==false) {
        return false;
    }
    
    // merge Frame
    myQuery = _T("INSERT INTO %s.generic_frame (OBJECT_GEOMETRY) SELECT s.OBJECT_GEOMETRY FROM %s.generic_frame s");
    if (m_DB->DataBaseQueryNoResults(wxString::Format(myQuery, m_MasterFileName.GetFullName(), m_SlaveFileName.GetFullName()), true)==false) {
        return false;
    }
    
    // merge stats
    myQuery = _T("INSERT INTO %s.prj_stats (DATE_START, CLICK, ATTRIBUTION, INTERSECTION, DATE_END) SELECT s.DATE_START, s.CLICK, s.ATTRIBUTION, s.INTERSECTION, s.DATE_END FROM %s.prj_stats s");
    if (m_DB->DataBaseQueryNoResults(wxString::Format(myQuery, m_MasterFileName.GetFullName(), m_SlaveFileName.GetFullName()), true)==false) {
        return false;
    }
    return true;
}

