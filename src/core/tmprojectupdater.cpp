/***************************************************************************
 tmprojectupdater.cpp
 -------------------
 copyright            : (C) 2011 CREALP Lucien Schreiber
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

#include "tmprojectupdater.h"
#include "../database/databaseresult.h"



tmProjectUpdater::tmProjectUpdater(DataBaseTM * database) {
	m_pDB = database;
	wxASSERT(m_pDB);
}



tmProjectUpdater::~tmProjectUpdater() {
	
}



bool tmProjectUpdater::IsCorrectVersion() {
	int myActualDBVersion = m_pDB->GetDatabaseToolMapVersion();
	wxASSERT(myActualDBVersion != wxNOT_FOUND);
	
	if (myActualDBVersion != TM_DATABASE_VERSION) {
		return false;
	}
	return true;
}



tmPRJ_UPD_ERROR tmProjectUpdater::DoUpdate(){
	int myActualDBVersion = m_pDB->GetDatabaseToolMapVersion();
	wxASSERT(myActualDBVersion != wxNOT_FOUND);
	if (myActualDBVersion == TM_DATABASE_VERSION) {
		return tmPRJ_UPD_ERROR_OK;
	}
	
	// security check
	if (myActualDBVersion < 220) {
		wxFAIL; // Update bellow 220 isn't supported!
	}
	if (myActualDBVersion > TM_DATABASE_VERSION) {
		return tmPRJ_UPD_ERROR_TM;
	}
	
	
	// 220 -> 221
	if (myActualDBVersion == 220) {
		if(_220to221()==false){
			_SetVersion(220);
			return tmPRJ_UPD_ERROR_PROJECT;
		}
        else{
            myActualDBVersion = 221;
        }
	}
    
    // 221 -> 222
    // TODO: Implement database migration
    if (myActualDBVersion == 221) {
        if(_221to222()==false){
			_SetVersion(221);
			return tmPRJ_UPD_ERROR_PROJECT;
		}
        else{
            myActualDBVersion = 222;
        }
        
    }
	
	_SetVersion(myActualDBVersion);
	return tmPRJ_UPD_ERROR_OK;
}



bool tmProjectUpdater::_SetVersion(int versionnumber) {
	wxString myQuery = wxString::Format(_T("UPDATE `%s` SET PRJ_VERSION = %d"),
										TABLE_NAME_PRJ_SETTINGS,
										versionnumber);
	return m_pDB->DataBaseQueryNoResults(myQuery);
}


bool tmProjectUpdater::_220to221() {
	// adding statistics table
	wxString myQuery = _T("CREATE  TABLE IF NOT EXISTS `prj_stats` (`STAT_ID` INT NOT NULL AUTO_INCREMENT ,")
	_T("`DATE_START` DATETIME NOT NULL ,`CLICK` BIGINT UNSIGNED NULL DEFAULT 0 ,")
	_T("`ATTRIBUTION` BIGINT UNSIGNED NULL DEFAULT 0 , `INTERSECTION` BIGINT UNSIGNED NULL DEFAULT 0 ,")
	_T(" `DATE_END` TIMESTAMP NOT NULL default current_timestamp on update current_timestamp, PRIMARY KEY (`STAT_ID`) );");
	return m_pDB->DataBaseQueryNoResults(myQuery);
}


bool tmProjectUpdater::_221to222(){
    // STEP 1. modify database
    // add tables
    wxString myQuery =
    _T("CREATE  TABLE IF NOT EXISTS `lang_def` (")
    _T("`LANG_ID` INT NOT NULL,")
    _T("`LANG_NAME` VARCHAR(50) NULL ,")
    _T("PRIMARY KEY (`LANG_ID`) );")
    
    _T("CREATE  TABLE IF NOT EXISTS `dmn_layer_attribut` (")
    _T("`ATTRIBUT_ID` INT NOT NULL AUTO_INCREMENT ,")
    _T("`LAYER_INDEX` INT NOT NULL ,")
    _T("`ATTRIBUT_NAME` VARCHAR(100) NULL ,")
    _T("PRIMARY KEY (`ATTRIBUT_ID`, `LAYER_INDEX`) );")
    
    _T("CREATE  TABLE IF NOT EXISTS `dmn_catalog` (")
    _T("`CATALOG_ID` INT NOT NULL AUTO_INCREMENT ,")
    _T("`CODE` VARCHAR(50) NULL ,")
    _T("`DESCRIPTION_0` VARCHAR(255) NULL ,")
    _T("`DESCRIPTION_1` VARCHAR(255) NULL ,")
    _T("`DESCRIPTION_2` VARCHAR(255) NULL ,")
    _T("`DESCRIPTION_3` VARCHAR(255) NULL ,")
    _T("`DESCRIPTION_4` VARCHAR(255) NULL ,")
    _T("PRIMARY KEY (`CATALOG_ID`) );")
    
    _T("CREATE  TABLE IF NOT EXISTS `dmn_attribut_value` (")
    _T("`ATTRIBUT_ID` INT NOT NULL ,")
    _T("`CATALOG_ID` INT NOT NULL ,")
    _T("PRIMARY KEY (`ATTRIBUT_ID`, `CATALOG_ID`) );");
    
    if (m_pDB->DataBaseQueryNoResults(myQuery) == false) {
        return false;
    }
    
    // Add data into lang_def
    if (m_pDB->CreateLangDefData() == false){
        return false;
    }
    
    // Modify OBJECT_DESC into OBJECT_DESC_0 -5
    myQuery =
    _T("ALTER TABLE dmn_layer_object CHANGE OBJECT_DESC")
    _T(" OBJECT_DESC_0 varchar(255) NOT NULL COMMENT 'Feature description'");
    if (m_pDB->DataBaseQueryNoResults(myQuery) == false) {
        return false;
    }
    
    for (int i = 1; i<5; i++){
        myQuery = _T("ALTER TABLE dmn_layer_object ADD OBJECT_DESC_%d varchar(255) DEFAULT NULL AFTER OBJECT_DESC_%d");
        if (m_pDB->DataBaseQueryNoResults(wxString::Format(myQuery, i, i-1))==false){
            return false;
        }
    }
    
    // STEP 2. Add Enumeration, Modify Enumeration values and column type
    // Get all layer_at tables
    wxString sSentence = _T("SELECT CAST(SUBSTR(TABLE_NAME FROM 9) AS UNSIGNED) LAYER_INDEX,")
	_T("COLUMN_NAME, COLUMN_TYPE, COLUMN_COMMENT FROM INFORMATION_SCHEMA.COLUMNS")
	_T(" WHERE table_schema = \"") + m_pDB->DataBaseGetName() + _T("\"")
	_T(" AND  table_name IN (SELECT TABLE_NAME FROM")
	_T(" INFORMATION_SCHEMA.TABLES WHERE TABLE_NAME LIKE \"") + TABLE_NAME_LAYER_AT +
	_T("\%\") AND COLUMN_NAME NOT IN ( 'OBJECT_ID', 'LAYER_AT_ID') ORDER BY LAYER_INDEX");
	if(m_pDB->DataBaseQuery(sSentence) == false){
        return false;
    }
    
    
	DataBaseResult * myResults = new DataBaseResult();
    if(m_pDB->DataBaseGetResults(myResults)==false){
        wxDELETE(myResults);
        return false;
    }
    
    wxArrayLong myLayerIndex;
    PrjMemFieldArray myFields;
	while (myResults->NextRow()){
        long myLayerID = wxNOT_FOUND;
        myResults->GetValue(0, myLayerID);
        myLayerIndex.Add(myLayerID);
        
        wxString myFieldName = wxEmptyString;
        wxString myFieldDesc = wxEmptyString;
        wxString myFieldComment = wxEmptyString;
        
        myResults->GetValue(1, myFieldName);
        myResults->GetValue(2, myFieldDesc);
        myResults->GetValue(3, myFieldComment);
         
        ProjectDefMemoryFields * myF = new ProjectDefMemoryFields();
        myF->SetValues(myFieldName, myFieldDesc, myFieldComment);
        myFields.Add(myF);
	}
    wxDELETE(myResults);
    
    // process every enumeration fields
    wxASSERT(myLayerIndex.GetCount() == myFields.GetCount());
    for (unsigned int f = 0; f< myFields.GetCount(); f++) {
        if (myFields.Item(f)->m_FieldType != TM_FIELD_ENUMERATION) {
            continue;
        }
        
        // add field
        
        
    }
    
    
    
    
    
    // clean field array
    unsigned int fCount = myFields.GetCount();
	for (unsigned int i = 0; i<fCount; i++) {
		ProjectDefMemoryFields * myF = myFields.Item(0);
		wxDELETE(myF);
		myFields.RemoveAt(0);
	}
	wxASSERT(myFields.GetCount() == 0);
    return true;
}


