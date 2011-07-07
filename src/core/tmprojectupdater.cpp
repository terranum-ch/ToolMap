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
	}
	
	_SetVersion(TM_DATABASE_VERSION);
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
	_T("`DATE` TIMESTAMP NOT NULL ,`CLICK` BIGINT UNSIGNED NULL DEFAULT 0 ,")  
	_T("`ATTRIBUTION` BIGINT UNSIGNED NULL DEFAULT 0 , `INTERSECTION` BIGINT UNSIGNED NULL DEFAULT 0 ,")
	_T(" `ELAPSED_TIME` DATETIME NULL , PRIMARY KEY (`STAT_ID`) );");
	return m_pDB->DataBaseQueryNoResults(myQuery);
}