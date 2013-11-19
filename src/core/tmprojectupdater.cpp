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
    if (myActualDBVersion == 221) {
        if(_221to222()==false){
			_SetVersion(221);
			return tmPRJ_UPD_ERROR_PROJECT;
		}
        else{
            myActualDBVersion = 222;
        }
        
    }
    
    // 222 -> 223
    if (myActualDBVersion == 222) {
        if (_222to223() == false) {
            _SetVersion(222);
            return tmPRJ_UPD_ERROR_PROJECT;
        }
        else{
            myActualDBVersion = 223;
        }
    }
	
    // 223 -> 224
    if (myActualDBVersion == 223) {
        if (_223to224() == false) {
            _SetVersion(223);
            return tmPRJ_UPD_ERROR_PROJECT;
        }
        else{
            myActualDBVersion = 224;
        }
    }
    
    // 224 -> 225
    if (myActualDBVersion == 224) {
        if (_224to225() == false) {
            _SetVersion(224);
            return tmPRJ_UPD_ERROR_PROJECT;
        }
        else{
            myActualDBVersion = 225;
        }
    }
    
    // 225 -> 226
    if (myActualDBVersion == 225) {
        if (_225to226() == false) {
            _SetVersion(225);
            return tmPRJ_UPD_ERROR_PROJECT;
        }
        else{
            myActualDBVersion = 226;
        }
    }
    
   
    // 226 -> 227
    if (myActualDBVersion == 226) {
        if (_226to227() == false) {
            _SetVersion(226);
            return tmPRJ_UPD_ERROR_PROJECT;
        }
        else{
            myActualDBVersion = 227;
        }
    }
    
    // 227 -> 228
    if (myActualDBVersion == 227) {
        if (_227to228() == false) {
            _SetVersion(227);
            return tmPRJ_UPD_ERROR_PROJECT;
        }
        else{
            myActualDBVersion = 228;
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
    wxStopWatch sw;
    
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
    _T("PRIMARY KEY (`ATTRIBUT_ID`, `CATALOG_ID`) );")
    
    _T(" ALTER TABLE prj_settings ADD COLUMN  `PRJ_LANG_ACTIVE` int(11) NOT NULL DEFAULT '0' AFTER `PRJ_SNAP_TOLERENCE`;");
    
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
        ProjectDefMemoryFields * myF = myFields.Item(f);
        if (myF->m_FieldType != TM_FIELD_ENUMERATION) {
            continue;
        }
        
        // add field to dmn_layer_attribut
        myQuery = _T("INSERT INTO %s (LAYER_INDEX, ATTRIBUT_NAME) VALUES (%ld, \"%s\");");
        if (m_pDB->DataBaseQueryNoResults(wxString::Format(myQuery, TABLE_NAME_AT_LIST, myLayerIndex.Item(f), myF->m_Fieldname)) == false) {
            wxLogError(_("Error converting field: '%s', project will be incomplete"), myF->m_Fieldname);
            continue;
        }
        myF->m_FieldID = m_pDB->DataBaseGetLastInsertedID();
        if (myF->m_FieldID == wxNOT_FOUND) {
            wxLogError(_("Unable to get ID for newly inserted field '%s', failed to convert project"), myF->m_Fieldname);
            continue;
        }
        
        // add enumeration into dmn_catalog
        for (unsigned int e = 0; e < myF->m_pCodedValueArray.GetCount(); e++) {
            myQuery = _T("INSERT INTO %s (CODE, DESCRIPTION_0) VALUES (\"%s\",\"%s\")");
            ProjectDefMemoryFieldsCodedVal * myCVal = myF->m_pCodedValueArray.Item(e);
            wxASSERT(myCVal);
            if (m_pDB->DataBaseQueryNoResults(wxString::Format(myQuery, TABLE_NAME_AT_CATALOG, myCVal->m_ValueCode, myCVal->m_ValueName))==false) {
                wxLogError(_("Adding ennumeration: '%s' from layer '%s' failed"), myCVal->m_ValueName, myF->m_Fieldname);
                continue;
            }
            myCVal->m_ValueID = m_pDB->DataBaseGetLastInsertedID();
            
            if (myCVal->m_ValueID == wxNOT_FOUND) {
                wxLogError(_("Unable to get ID for newly inserted enumeration '%s' from layer '%s'"), myCVal->m_ValueName, myF->m_Fieldname);
                continue;
            }
            // add enumeration into dmn_attribut_value
            myQuery = _T("INSERT INTO %s (ATTRIBUT_ID, CATALOG_ID) VALUES (%d,%ld)");
            if (m_pDB->DataBaseQueryNoResults(wxString::Format(myQuery, TABLE_NAME_AT_MIX, myF->m_FieldID, myCVal->m_ValueID))==false) {
                wxLogError(_("Inserting enumeration '%s' into mix table failed!"), myCVal->m_ValueName);
                continue;
            }
        }
    }
    
    
    // remove NULL values (Bug #263)
    for (unsigned int f = 0; f< myFields.GetCount(); f++) {
        ProjectDefMemoryFields * myF = myFields.Item(f);
        if (myF->m_FieldType != TM_FIELD_ENUMERATION) {
            continue;
        }
        
        myQuery = _T("DELETE from %s%ld WHERE %s = \"\"");
        if (m_pDB->DataBaseQueryNoResults(wxString::Format(myQuery, TABLE_NAME_LAYER_AT, myLayerIndex[f], myF->m_Fieldname),true)==false) {
            wxLogError(_("Unable to remove empty data from '%s' field"), myF->m_Fieldname);
            continue;
        }
    }
    
    
    // STEP 3. change existing attribution
    for (unsigned int f = 0; f< myFields.GetCount(); f++) {
        ProjectDefMemoryFields * myF = myFields.Item(f);
        if (myF->m_FieldType != TM_FIELD_ENUMERATION) {
            continue;
        }
        
        
        // convert columns from enumeration to string
        myQuery = _T("ALTER TABLE %s%ld MODIFY %s VARCHAR(500) NULL");
        if (m_pDB->DataBaseQueryNoResults(wxString::Format(myQuery, TABLE_NAME_LAYER_AT, myLayerIndex.Item(f), myF->m_Fieldname))==false) {
            wxLogError(_("Unable to convert '%s' field"), myF->m_Fieldname);
            continue;
        }
        
        // change all ennumeration into id (stored as string)
        for (unsigned int e = 0; e < myF->m_pCodedValueArray.GetCount(); e++) {
            ProjectDefMemoryFieldsCodedVal * myCVal = myF->m_pCodedValueArray.Item(e);
            wxASSERT(myCVal);
            myQuery = _T("UPDATE %s%ld SET %s=\"%ld\" WHERE %s=\"%s\"");
            if (m_pDB->DataBaseQueryNoResults(wxString::Format(myQuery,TABLE_NAME_LAYER_AT,  myLayerIndex.Item(f), myF->m_Fieldname, myCVal->m_ValueID, myF->m_Fieldname, myCVal->m_ValueName))==false) {
                wxLogError(_("Error setting enumeration value from '%s' to '%ld'"), myCVal->m_ValueName, myCVal->m_ValueID);
                continue;
            }
        }
        
        // convert columns from string to integer
        myQuery = _T("ALTER TABLE %s%ld MODIFY %s INT NULL COMMENT \"%s\"");
        if (m_pDB->DataBaseQueryNoResults(wxString::Format(myQuery, TABLE_NAME_LAYER_AT, myLayerIndex.Item(f), myF->m_Fieldname, TABLE_COMMENT_ENUMERATION))==false) {
            wxLogError(_("Converting %s to integer failed!"), myF->m_Fieldname);
            continue;
        }
    }
    
    // clean field array
    unsigned int fCount = myFields.GetCount();
	for (unsigned int i = 0; i<fCount; i++) {
		ProjectDefMemoryFields * myF = myFields.Item(0);
		wxDELETE(myF);
		myFields.RemoveAt(0);
	}
	wxASSERT(myFields.GetCount() == 0);
    wxLogMessage(_("Database updated in %ld [ms]"), sw.Time());
    return true;
}



bool tmProjectUpdater::_222to223(){
    wxString myQuery = _T("ALTER TABLE %s MODIFY SYMBOLOGY VARCHAR(65535) NULL");
    if (m_pDB->DataBaseQueryNoResults(wxString::Format(myQuery, TABLE_NAME_TOC),true)==false) {
        return false;
    }
    return true;
}



bool tmProjectUpdater::_223to224(){
    wxString myQuery =
    _T("CREATE  TABLE IF NOT EXISTS `export_poly` (")
    _T("   `LAYER_INDEX` INT NOT NULL ,")
    _T("   `RASTER_FACTOR` DOUBLE NULL ,")
    _T("   `NB_EXPORT` INT NULL ,")
    _T("   `PERCENT_SKIPPED` DOUBLE NULL ,")
    _T("   PRIMARY KEY (`LAYER_INDEX`));");
    if (m_pDB->DataBaseQueryNoResults(myQuery)==false) {
        return false;
    }
    return true;
}


bool tmProjectUpdater::_224to225(){
    wxString myQuery = _T("ALTER TABLE %s MODIFY OBJECT_CD VARCHAR(50) NULL COMMENT 'Feature code'");
    if (m_pDB->DataBaseQueryNoResults(wxString::Format(myQuery, TABLE_NAME_OBJECTS)) == false) {
        return false;
    }
    return true;
}



bool tmProjectUpdater::_225to226(){
    // ensure column PRJ_LANG_ACTIVE exists ?
    wxString myQuery = _T(" SELECT * FROM information_schema.COLUMNS WHERE TABLE_SCHEMA = \"%s\" AND TABLE_NAME = 'prj_settings' AND COLUMN_NAME = 'PRJ_LANG_ACTIVE'");
    if (m_pDB->DataBaseQuery(wxString::Format(myQuery, m_pDB->DataBaseGetName()))==false) {
        return false;
    }
    
    wxString myAlterQuery = wxEmptyString;
    if (m_pDB->DataBaseHasResults() == false) {
        // Column PRJ_LANG_ACTIVE didn't exists! This was a bug in _221to222
        // it's now corrected.
        myAlterQuery =   _T(" ALTER TABLE prj_settings ADD COLUMN  `PRJ_LANG_ACTIVE` int(11) NOT NULL DEFAULT '0' AFTER `PRJ_SNAP_TOLERENCE`;");
    }
    m_pDB->DataBaseClearResults();

    myAlterQuery.Append( _T(" ALTER TABLE prj_settings ADD COLUMN  `PRJ_BEZIER_APPROX` FLOAT NOT NULL DEFAULT 0.5 AFTER `PRJ_LANG_ACTIVE`;"));
    if (m_pDB->DataBaseQueryNoResults(myAlterQuery) == false) {
        return false;
    }
    return true;
}



bool tmProjectUpdater::_226to227(){
    wxString myAlterQuery = _T("ALTER TABLE prj_settings ADD COLUMN `PRJ_BEZIER_WIDTH` FLOAT NOT NULL DEFAULT 1 AFTER `PRJ_BEZIER_APPROX`; ")
    _T("ALTER TABLE prj_settings ADD COLUMN `PRJ_BEZIER_NB_VERTEX` INT NOT NULL DEFAULT 10 ; ")
    _T("ALTER TABLE prj_settings ADD COLUMN `PRJ_BEZIER_METHOD` INT NOT NULL DEFAULT 0; ");
    if (m_pDB->DataBaseQueryNoResults(myAlterQuery) == false) {
        return false;
    }    
    return true;
}



bool tmProjectUpdater::_227to228(){
    wxString myAlterQuery = _T("ALTER TABLE prj_settings MODIFY COLUMN `PRJ_UNIT` VARCHAR(45) NOT NULL;");
    if (m_pDB->DataBaseQueryNoResults(myAlterQuery) == false) {
        return false;
    }
    return true;
}




