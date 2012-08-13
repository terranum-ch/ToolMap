/***************************************************************************
 tmmergeprojects.cpp
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

#include "tmmergeprojects.h"
#include "database.h"

tmMergeProjects::tmMergeProjects(const wxString & masterprj, const wxString & slaveprj) {
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



tmMergeProjects::~tmMergeProjects() {
    wxDELETE(m_DB);
}




void tmMergeProjects::SetVerbose(bool value) {
  m_beVerbose = value;
}



wxString tmMergeProjects::GetLastError() {
    if (m_Errors.GetCount() > 0) {
        return  m_Errors.Item(m_Errors.GetCount() -1);
    }
    return wxEmptyString;
}


bool tmMergeProjects::_HasSameNumberRecords(DataBase * db, const wxString & tablename) {
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



bool tmMergeProjects::_HasDifferenceResults(DataBase * db, const wxString & query, long & errnumber) {
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



bool tmMergeProjects::_HasSimilarResults(DataBase * db, const wxString & query, long & errnumber) {
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



bool tmMergeProjects::_IsReady(){
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



bool tmMergeProjects::CheckSimilar() {
    
    // some generic checks
    if (_IsReady() == false) {
        return false;
    }
    
    wxASSERT(m_DB);
    m_Errors.Clear();
    wxLogDebug("Checking projects: '%s' with '%s'", m_MasterFileName.GetFullName(), m_SlaveFileName.GetFullName());
    
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



bool tmMergeProjects::MergeIntoMaster() {    
    // some generic checks
    if (_IsReady() == false) {
        return false;
    }
    wxASSERT(m_DB);
    m_Errors.Clear();

    // get old ID's
    wxArrayLong myOldIds;
    wxArrayLong myNewIds;
    
    wxString myQuery = _T("SELECT d.OBJECT_ID FROM %s.generic_lines d ORDER BY d.OBJECT_ID");
    if (m_DB->DataBaseQuery(wxString::Format(myQuery, m_SlaveFileName.GetFullName()), true)==false) {
        return false;
    }
    
    if(m_DB->DataBaseGetResults(myOldIds)==false){
        m_Errors.Add(_("regaining Old ID Failed!"));
        return false;
    }
    
    if (IsVerbose()) {
        wxLogDebug(_("%ld Old ID regained"), myOldIds.GetCount());
    }
    
    // copy lines and save id into new ID
    myQuery = _T("INSERT INTO %s.generic_lines (OBJECT_GEOMETRY) SELECT d.OBJECT_GEOMETRY FROM %s.generic_lines d ORDER BY d.OBJECT_ID LIMIT 1 OFFSET %d");
    
    for (unsigned int i = 0; i< myOldIds.GetCount(); i++) {
        if (m_DB->DataBaseQuery(wxString::Format(myQuery, m_MasterFileName.GetFullName(), m_SlaveFileName.GetFullName(), i), true)==false) {
            return false;
        }
        
        myNewIds.Add(m_DB->DataBaseGetLastInsertedID());
        if (IsVerbose()) {
            if (i % 100 == 0) {
                wxLogDebug(_("%d record copied!"), i);
            }
        }
        
    }
    
    
    return true;
}

