/***************************************************************************
								database_tm.cpp
					extension of the database class for ToolMap
                             -------------------
    copyright            : (C) 2007 CREALP Lucien Schreiber 
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


#include "database_tm.h"


DataBaseTM::DataBaseTM()
{
	m_iDBLayerIndex = 0;

}


DataBaseTM::~DataBaseTM()
{


}
/*************************** GENERAL DATABASE FUNCTION ****************************/
bool DataBaseTM::CreateEmptyTMDatabase()
{
	wxString myNewPrjSentence = _T("CREATE  TABLE `dmn_layer_type` (")
	_T("  `TYPE_CD` INT UNSIGNED NOT NULL COMMENT 'Layer type code' ,")
	_T("  `TYPE_DESCRIPTION` VARCHAR(20) NOT NULL COMMENT 'Layer type description' ,")
	_T("  PRIMARY KEY (`TYPE_CD`) );")
	
	_T("CREATE  TABLE `thematic_layers` (")
	_T("  `LAYER_INDEX` INT UNSIGNED NOT NULL AUTO_INCREMENT COMMENT 'Theme index' ,")
	_T("  `TYPE_CD` INT UNSIGNED NOT NULL ,")
	_T("  `LAYER_NAME` VARCHAR(255) NOT NULL COMMENT 'Theme name' ,")
	_T("  `LAYER_DESC` VARCHAR(255) NULL COMMENT 'Theme description' ,")
	_T("  `REMARK` VARCHAR(255) NULL ,")
	_T("  PRIMARY KEY (`LAYER_INDEX`) ,")
	_T("  INDEX THEMATIC_LAYERS_FKIndex1 (`TYPE_CD` ASC) ,")
	_T("  CONSTRAINT `Rel_11`")
	_T("    FOREIGN KEY (`TYPE_CD` )")
	_T("    REFERENCES `dmn_layer_type` (`TYPE_CD` )")
	_T("    ON DELETE NO ACTION")
	_T("    ON UPDATE NO ACTION);")
	
	_T("CREATE  TABLE `dmn_layer_object` (")
	_T("  `OBJECT_ID` BIGINT NOT NULL AUTO_INCREMENT ,")
	_T("  `OBJECT_CD` INT UNSIGNED NOT NULL COMMENT 'Feature code' ,")
	_T("  `THEMATIC_LAYERS_LAYER_INDEX` INT UNSIGNED NOT NULL COMMENT 'Associated thematic layer' ,")
	_T("  `OBJECT_DESC` VARCHAR(100) NOT NULL COMMENT 'Feature description' ,")
	_T("  `OBJECT_ISFREQ` BOOLEAN NOT NULL DEFAULT FALSE ,")
	_T("  `SYMBOL_CD` VARCHAR(20) NULL COMMENT 'Feature symbology' ,")
	_T("  `RANK` INT NULL ,")
	_T("  `REMARK` VARCHAR(100) NULL ,")
	_T("  PRIMARY KEY (`OBJECT_ID`) ,")
	_T("  INDEX DMN_LAYER_OBJECT_FKIndex2 (`THEMATIC_LAYERS_LAYER_INDEX` ASC, `OBJECT_ID` ASC) ,")
	_T("  CONSTRAINT `Contain`")
	_T("    FOREIGN KEY (`THEMATIC_LAYERS_LAYER_INDEX` )")
	_T("    REFERENCES `thematic_layers` (`LAYER_INDEX` )")
	_T("    ON DELETE CASCADE")
	_T("    ON UPDATE RESTRICT);")
	
	_T("CREATE  TABLE `generic_points` (")
	_T("  `OBJECT_ID` INT UNSIGNED NOT NULL AUTO_INCREMENT COMMENT 'Feature identifier' ,")
	_T("  `OBJECT_GEOMETRY` MULTIPOINT NOT NULL COMMENT 'Feature geometry' ,")
	_T("  PRIMARY KEY (`OBJECT_ID`) );")
	
	_T("CREATE  TABLE `generic_labels` (")
	_T("  `OBJECT_ID` INT UNSIGNED NOT NULL AUTO_INCREMENT ,")
	_T("  `OBJECT_GEOMETRY` MULTIPOINT NOT NULL ,")
	_T("  PRIMARY KEY (`OBJECT_ID`) );")
	
	_T("CREATE  TABLE `generic_lines` (")
	_T("  `OBJECT_ID` INT UNSIGNED NOT NULL AUTO_INCREMENT ,")
	_T("  `OBJECT_GEOMETRY` LINESTRING NOT NULL ,")
	_T("  PRIMARY KEY (`OBJECT_ID`) );")
	
	_T("CREATE  TABLE `generic_pat` (")
	_T("  `OBJECT_ID` INT UNSIGNED NOT NULL ,")
	_T("  `THEMATIC_LAYERS_LAYER_INDEX` INT UNSIGNED NOT NULL ,")
	_T("  `OBJECT_CD` INT UNSIGNED NOT NULL ,")
	_T("  PRIMARY KEY (`OBJECT_ID`, `THEMATIC_LAYERS_LAYER_INDEX`, `OBJECT_CD`) ,")
	_T("  INDEX GENERIC_POINTS_has_DMN_LAYER_OBJECT_FKIndex1 (`OBJECT_ID` ASC) ,")
	_T("  INDEX GENERIC_POINTS_has_DMN_LAYER_OBJECT_FKIndex2 (`OBJECT_CD` ASC, `THEMATIC_LAYERS_LAYER_INDEX` ASC) ,")
	_T("  CONSTRAINT `Rel_19`")
	_T("    FOREIGN KEY (`OBJECT_ID` )")
	_T("    REFERENCES `generic_points` (`OBJECT_ID` ),")
	_T("  CONSTRAINT `Rel_20`")
	_T("    FOREIGN KEY (`OBJECT_CD` , `THEMATIC_LAYERS_LAYER_INDEX` )")
	_T("    REFERENCES `dmn_layer_object` (`OBJECT_CD` , `THEMATIC_LAYERS_LAYER_INDEX` ));")
	
	_T("CREATE  TABLE `generic_lat` (")
	_T("  `OBJECT_ID` INT UNSIGNED NOT NULL ,")
	_T("  `THEMATIC_LAYERS_LAYER_INDEX` INT UNSIGNED NOT NULL ,")
	_T("  `OBJECT_CD` INT UNSIGNED NOT NULL ,")
	_T("  PRIMARY KEY (`OBJECT_ID`, `THEMATIC_LAYERS_LAYER_INDEX`, `OBJECT_CD`) ,")
	_T("  INDEX GENERIC_LABELS_has_DMN_LAYER_OBJECT_FKIndex1 (`OBJECT_ID` ASC) ,")
	_T("  INDEX GENERIC_LABELS_has_DMN_LAYER_OBJECT_FKIndex2 (`OBJECT_CD` ASC, `THEMATIC_LAYERS_LAYER_INDEX` ASC) ,")
	_T("  CONSTRAINT `Rel_21`")
	_T("    FOREIGN KEY (`OBJECT_ID` )")
	_T("    REFERENCES `generic_labels` (`OBJECT_ID` ),")
	_T("  CONSTRAINT `Rel_22`")
	_T("    FOREIGN KEY (`OBJECT_CD` , `THEMATIC_LAYERS_LAYER_INDEX` )")
	_T("    REFERENCES `dmn_layer_object` (`OBJECT_CD` , `THEMATIC_LAYERS_LAYER_INDEX` ));")
	
	_T("CREATE  TABLE `generic_aat` (")
	_T("  `OBJECT_ID` INT UNSIGNED NOT NULL ,")
	_T("  `THEMATIC_LAYERS_LAYER_INDEX` INT UNSIGNED NOT NULL ,")
	_T("  `OBJECT_CD` INT UNSIGNED NOT NULL ,")
	_T("  PRIMARY KEY (`OBJECT_ID`, `THEMATIC_LAYERS_LAYER_INDEX`, `OBJECT_CD`) ,")
	_T("  INDEX GENERIC_LINES_has_DMN_LAYER_OBJECT_FKIndex1 (`OBJECT_ID` ASC) ,")
	_T("  INDEX GENERIC_LINES_has_DMN_LAYER_OBJECT_FKIndex2 (`OBJECT_CD` ASC, `THEMATIC_LAYERS_LAYER_INDEX` ASC) ,")
	_T("  CONSTRAINT `Rel_25`")
	_T("    FOREIGN KEY (`OBJECT_ID` )")
	_T("    REFERENCES `generic_lines` (`OBJECT_ID` ),")
	_T("  CONSTRAINT `Rel_26`")
	_T("    FOREIGN KEY (`OBJECT_CD` , `THEMATIC_LAYERS_LAYER_INDEX` )")
	_T("    REFERENCES `dmn_layer_object` (`OBJECT_CD` , `THEMATIC_LAYERS_LAYER_INDEX` ));")
	
	_T("CREATE  TABLE `generic_dmn` (")
	_T("  `CODE` INT UNSIGNED NOT NULL AUTO_INCREMENT COMMENT 'Code value' ,")
	_T("  `DESCRIPTION` VARCHAR(255) NULL COMMENT 'Code description' ,")
	_T("  PRIMARY KEY (`CODE`) );")
	
	_T("CREATE  TABLE `project_toc` (")
	_T("  `CONTENT_ID` INT UNSIGNED NOT NULL AUTO_INCREMENT ,")
	_T("  `TYPE_CD` INT UNSIGNED NOT NULL ,")
	_T("  `CONTENT_PATH` VARCHAR(255) NULL ,")
	_T("  `CONTENT_NAME` VARCHAR(255) NULL ,")
	_T("  `CONTENT_STATUS` BOOLEAN NULL DEFAULT 1 ,")
	_T("  `GENERIC_LAYERS` TINYINT NULL DEFAULT 100 ,")
	_T("  `RANK` INT NULL ,")
	_T("  `SYMBOLOGY` BINARY(255) NULL ,")
	_T("  `VERTEX_FLAGS` TINYINT NULL  ,")
	_T("  PRIMARY KEY (`CONTENT_ID`) ,")
	_T("  INDEX PROJECT_TOC_FKIndex1 (`TYPE_CD` ASC) ,")
	_T("  CONSTRAINT `Rel_10`")
	_T("    FOREIGN KEY (`TYPE_CD` )")
	_T("    REFERENCES `dmn_layer_type` (`TYPE_CD` ));")
	
	_T("CREATE  TABLE `dmn_shortcut_key` (")
	_T("  `SHORTCUT_CD` INT UNSIGNED NOT NULL AUTO_INCREMENT ,")
	_T("  `SHORTCUT_KEY` VARCHAR(20) NOT NULL ,")
	_T("  PRIMARY KEY (`SHORTCUT_CD`) );")
	
	_T("CREATE  TABLE `shortcut_list` (")
	_T("  `OBJECT_CD` INT UNSIGNED NOT NULL ,")
	_T("  `THEMATIC_LAYERS_LAYER_INDEX` INT UNSIGNED NOT NULL ,")
	_T("  `SHORTCUT_CD` INT UNSIGNED NOT NULL ,")
	_T("  PRIMARY KEY (`OBJECT_CD`, `THEMATIC_LAYERS_LAYER_INDEX`, `SHORTCUT_CD`) ,")
	_T("  INDEX SHORTCUT_KEYS_has_DMN_LAYER_OBJECT_FKIndex2 (`OBJECT_CD` ASC, `THEMATIC_LAYERS_LAYER_INDEX` ASC) ,")
	_T("  INDEX SHORTCUT_LIST_FKIndex2 (`SHORTCUT_CD` ASC) ,")
	_T("  CONSTRAINT `Rel_14`")
	_T("    FOREIGN KEY (`SHORTCUT_CD` )")
	_T("    REFERENCES `dmn_shortcut_key` (`SHORTCUT_CD` ),")
	_T("  CONSTRAINT `Rel_15`")
	_T("    FOREIGN KEY (`OBJECT_CD` , `THEMATIC_LAYERS_LAYER_INDEX` )")
	_T("    REFERENCES `dmn_layer_object` (`OBJECT_CD` , `THEMATIC_LAYERS_LAYER_INDEX` ));")
	
	_T("CREATE  TABLE `generic_frame` (")
	_T("  `OBJECT_ID` INT UNSIGNED NOT NULL AUTO_INCREMENT COMMENT 'Feature identifier' ,")
	_T("  `OBJECT_GEOMETRY` POLYGON NOT NULL COMMENT 'Feature geometry' ,")
	_T("  PRIMARY KEY (`OBJECT_ID`) );")
	
	_T("CREATE  TABLE `zoom_level` (")
	_T("  `ZOOM_ID` INT UNSIGNED NOT NULL AUTO_INCREMENT ,")
	_T("  `SCALE_VALUE` INT UNSIGNED NOT NULL ,")
	_T("  `RANK` INT NULL ,")
	_T("  PRIMARY KEY (`ZOOM_ID`) );")
	
	_T("CREATE  TABLE `prj_settings` (")
	_T("  `SETTING_DBK` INT NOT NULL AUTO_INCREMENT ,")
	_T("  `PRJ_UNIT` VARCHAR(10) NOT NULL ,")
	_T("  `PRJ_PROJECTION` VARCHAR(45) NOT NULL ,")
	_T("  `PRJ_NAME` VARCHAR(45) NOT NULL ,")
	_T("  `PRJ_VERSION` INT NOT NULL ,")
	_T("  `PRJ_EXPORT_PATH` VARCHAR(255) NULL ,")
	_T("  `PRJ_EXPORT_TYPE` INT NULL DEFAULT 0 ,")
	_T("  `PRJ_BACKUP_PATH` VARCHAR(255) NULL ,")
	_T("  `PRJ_AUTHORS` VARCHAR(255) NULL ,")
	_T("  `PRJ_SUMMARY` TEXT NULL ,")
	_T("  PRIMARY KEY (`SETTING_DBK`) );")
	
	_T("CREATE  TABLE `generic_notes` (")
	_T("  `OBJECT_ID` INT UNSIGNED NOT NULL AUTO_INCREMENT ,")
	_T("  `OBJECT_GEOMETRY` MULTIPOINT NOT NULL ,")
	_T("  `PRIORITY_CD` TINYINT NULL DEFAULT 0 ,")
	_T("  `CODE_CD` TINYINT NULL DEFAULT 0 ,")
	_T("  `AUTHOR` VARCHAR(255) NULL ,")
	_T("  `NOTE_DATE` TIMESTAMP NULL DEFAULT CURRENT_TIMESTAMP ,")
	_T("  `DESCRIPTION` TEXT NULL ,")
	_T("  `ISFIXED` BOOLEAN NULL DEFAULT 0 ,")
	_T("  PRIMARY KEY (`OBJECT_ID`) )");

	
	
	wxArrayString myArray = DataBaseCutRequest(myNewPrjSentence);
	
	for (unsigned int i= 0; i<myArray.GetCount(); i++)
	{
		if(!DataBaseQueryNoResult(myArray.Item(i)))
		{
			// request in not ok
			wxLogDebug(_T("Errors during process of Tables creation"));
			return FALSE;
		}
	}
	
	
	// pass field data to the database
	if (FillLayerTableTypeData()==FALSE)
		return FALSE;
	
	// pass scale data into the database
	if (FillDefaultScaleData()==FALSE)
		return FALSE;
	
	return TRUE;	
	
}





/*************************** PROJECT DATABASE FUNCTION ****************************/
bool DataBaseTM::FillLayerTableTypeData ()
{
	wxString sSentence = _T("");
	bool bReturnValue = TRUE;
	
	// fill the field from the layer type table
	// only if the table is empty...
	if (DataBaseIsTableEmpty(TABLE_NAME_LAYER_TYPE))
	{
		for (int i = 0; i< PRJDEF_LAYERS_TYPE_NUMBER; i++)
		{
			sSentence.Printf(_T("INSERT INTO %s VALUES (%d,\"%s\")"),
							 TABLE_NAME_LAYER_TYPE.c_str(),
							 i, PRJDEF_LAYERS_TYPE_STRING[i].c_str());
			
			// in case of error send debug message
			if(!DataBaseQueryNoResult(sSentence))
			{
				wxLogDebug(_T("Error filling data into the [%s] tables"),
						   TABLE_NAME_LAYER_TYPE.c_str());	
			}
		}
		
	}
	return bReturnValue;
}



/***************************************************************************//**
 @brief Fill default data into scale table
 @details This function fills default scales into the scale table (ZOOM_LEVEL) if
 the table is empty. Otherwise FALSE is returned. Default scales are :
 - 1:5000
 - 1:10000
 - 1:25000
 - 1:50000
 @return  FALSE if the table isn't empty
 @author Lucien Schreiber (c) CREALP 2007
 @date 27 March 2008
 *******************************************************************************/
bool DataBaseTM::FillDefaultScaleData ()
{
	bool bReturnValue = FALSE;
	wxString sValue   = _T("INSERT INTO ") + TABLE_NAME_SCALE +
						_T("(SCALE_VALUE) VALUES ");
	wxString sSentence = _T("");
	int iScale [] = {5000, 10000, 25000, 50000};
	int iScaleNum = (sizeof (iScale) / sizeof (int));
	
	// check that the zoom table is empty otherwise error
	if (DataBaseIsTableEmpty(TABLE_NAME_SCALE))
	{
		// prepare statement
		for (int i = 0; i<iScaleNum; i++)
		{
			sSentence.Append(sValue);
			sSentence.Append(wxString::Format(_T("(%d); "),iScale[i]));
		}
		
		// execute statement
		if (DataBaseQuery(sSentence))
		{
			bReturnValue = TRUE;
		}
		
	}
	
	if (bReturnValue == FALSE)
		wxLogError(_T("Error filling scale table. Already filled of request error ?"));
	
	return bReturnValue;
}


/***************************************************************************//**
 @brief Check if project data are defined
 @details This function check the existance of a line into the PRJ_SETTINGS
 table.
 @return  TRUE if a line exist containing project data (in the PRJ_SETTINGS
 table)
 @author Lucien Schreiber (c) CREALP 2007
 @date 25 March 2008
 *******************************************************************************/
bool DataBaseTM::IsProjectDataDefined ()
{
	// check if the project line exist
	wxString sSentence = _T("SELECT * FROM ") + TABLE_NAME_PRJ_SETTINGS + _T(" WHERE SETTING_DBK =1");
	if (DataBaseQuery(sSentence))
	{
		// if ok the project settings exist
		if (DataBaseHasResult())
			return TRUE;
	}
	
	wxLogDebug(_T("No data found into the project settings table"));
	return FALSE;
}




bool DataBaseTM::SetProjectData (PrjDefMemManage * pPrjDefinition)
{
	// prepare data
	wxString sUnit = PRJDEF_UNIT_TYPE_STRING[pPrjDefinition->m_PrjUnitType];
	wxString sProj = PRJDEF_PROJ_TYPE_STRING[pPrjDefinition->m_PrjProjType];
	wxString sSentence = _T("");
	
	if (IsProjectDataDefined()==FALSE)	
	{
		sSentence = wxString::Format(_T("INSERT INTO %s (PRJ_UNIT, PRJ_PROJECTION,") 
									 _T("PRJ_NAME, PRJ_VERSION, PRJ_AUTHORS, PRJ_SUMMARY) ")
									 _T(" VALUES (\"%s\",\"%s\",\"%s\",%d,\"%s\",\"%s\")"),
									 TABLE_NAME_PRJ_SETTINGS.c_str(),
									 sUnit.c_str(),sProj.c_str(),
									 pPrjDefinition->m_PrjName.c_str(),TM_DATABASE_VERSION,
									 pPrjDefinition->m_PrjAuthors.c_str(),
									 pPrjDefinition->m_PrjSummary.c_str());
		
	}
	else
		sSentence = wxString::Format(_T("UPDATE %s SET ") 
									 _T("PRJ_UNIT = \"%s\", PRJ_PROJECTION = \"%s\", ")
									 _T("PRJ_NAME = \"%s\", PRJ_VERSION = %d, ")
									 _T("PRJ_AUTHORS = \"%s\", PRJ_SUMMARY = \"%s\""),
									 TABLE_NAME_PRJ_SETTINGS.c_str(),
									 sUnit.c_str(),sProj.c_str(),
									 pPrjDefinition->m_PrjName.c_str(),TM_DATABASE_VERSION,
									 pPrjDefinition->m_PrjAuthors.c_str(),
									 pPrjDefinition->m_PrjSummary.c_str());
	
	// processing request
	if (DataBaseQueryNoResult(sSentence))
		return TRUE;
	
	wxLogDebug(_T("Error while modifing the project settings in the database"));
	return FALSE;
}


/***************************************************************************//**
 @brief Retrieve basic project data
 @details This may be used for filling a #PrjDefMemManage object with basic data
 from project such as Project name, path, units,... In facts all stuff from the
 Project Definition dialog (except the spatial model)
 @param pPrjDefinition a pointer to an existing #PrjDefMemManage object, will be
 used for storing project data (path, name,...)
 @return bool return TRUE if data was found from the Database
 @author Lucien Schreiber (c) CREALP 2007
 @date 14 April 2008
 *******************************************************************************/
bool DataBaseTM::GetProjectData (PrjDefMemManage * pPrjDefinition)
{
	int i=0;
	wxString sSentence = wxString::Format(_T("SELECT PRJ_UNIT, PRJ_PROJECTION, PRJ_NAME, ")
										  _T("PRJ_AUTHORS, PRJ_SUMMARY FROM %s"), 
										  TABLE_NAME_PRJ_SETTINGS.c_str());
	if (DataBaseQuery(sSentence))
	{
		wxArrayString myResults = DataBaseGetNextResult();
		
		// UNITS
		for (i = 0; i<PRJDEF_UNIT_TYPE_NUMBER; i++)
		{
			if (myResults.Item(0) == PRJDEF_UNIT_TYPE_STRING[i])
			{
				pPrjDefinition->m_PrjUnitType = (PRJDEF_UNIT_TYPE) i;
				break;
			}
		}
		
		// PROJECTION
		for (i = 0; i< PRJDEF_PROJ_TYPE_NUMBER; i++)
		{
			if(myResults.Item(1) == PRJDEF_PROJ_TYPE_STRING[i])
			{
				pPrjDefinition->m_PrjProjType = (PRJDEF_PROJ_TYPE) i;
				break;
			}
			
		}
		
		// NAME
		pPrjDefinition->m_PrjName = DataBaseGetName();
		
		// PATH
		pPrjDefinition->m_PrjPath = DataBaseGetPath();
		
		// AUTHORS
		pPrjDefinition->m_PrjAuthors = myResults.Item(3);
		
		// COMMENT
		pPrjDefinition->m_PrjSummary = myResults.Item(4);
		
		return TRUE;
	}
	wxLogError(_T("Error getting project data from the DB"));
	return FALSE;
}



int	 DataBaseTM::GetDatabaseToolMapVersion ()
{
	wxString sSentence = _T("SELECT (PRJ_VERSION) FROM " + TABLE_NAME_PRJ_SETTINGS);
	if (DataBaseQuery(sSentence))
	{
		return DataBaseGetResultAsInt();
	}
	wxLogDebug(_T("Getting database version not passed"));
	return -1;
	
}



bool DataBaseTM::SetProjectExportData (int iExportType, const wxString & spath)
{
	wxString sSentence = wxString::Format(_T("UPDATE %s SET PRJ_EXPORT_PATH = \"%s\", ")
										  _T("PRJ_EXPORT_TYPE = %d; "),
										  TABLE_NAME_PRJ_SETTINGS.c_str(),
										  spath.c_str(),
										  iExportType);
	if (DataBaseQuery(sSentence))
		return TRUE;
	
	wxLogError(_T("Not able to update project export path : %s"), sSentence.c_str());
	return FALSE;
}


bool DataBaseTM::SetProjectBackupPath (const wxString & spath)
{
	wxString sSentence = wxString::Format(_T("UPDATE %s SET PRJ_BACKUP_PATH = \"%s\"; "),
										  TABLE_NAME_PRJ_SETTINGS.c_str(),
										  spath.c_str());
										  
	if (DataBaseQuery(sSentence))
		return TRUE;
	
	wxLogError(_T("Not able to update project backup path : %s"), sSentence.c_str());
	return FALSE;
	
}



/***************************************************************************//**
 @brief Get the export data from the database
 @details Retrive the Export path and the export type from the database
 @param iExportType This value will be modified with one of the
 #PRJDEF_EXPORT_TYPE.
 @param spath This variable will be filled with the spath if return value is
 different of PATH_EMPTY
 @return  the returned values is a flag of type #PATH_ERROR 
 @author Lucien Schreiber (c) CREALP 2007
 @date 28 March 2008
 *******************************************************************************/
int	DataBaseTM::GetProjectExportData (int & iExportType, wxString &spath)
{
	int iflagreturn = PATH_OK;
	wxArrayString myResult;
	
	wxString sSentence = _T("SELECT PRJ_EXPORT_TYPE, PRJ_EXPORT_PATH FROM ") + 
	TABLE_NAME_PRJ_SETTINGS;
	if (DataBaseQuery(sSentence) && DataBaseHasResult())
	{
		// get the first result as int
		myResult = DataBaseGetNextResult();
		iExportType = wxAtoi(myResult.Item(0));
		spath = myResult.Item(1);
		
		if (spath.IsEmpty())
			iflagreturn = PATH_EMPTY;
		else
		{
			// check for path validity
			if (!wxFileName::DirExists(spath))
				iflagreturn = PATH_INVALID;
		}
		
	}
	else // error querying the database
	{
		wxLogDebug(_T("Unable to query the database for project ")
				   _T("export data. or no results returned : %s"),
				   sSentence.c_str());
		iflagreturn = PATH_DATABASE_ERROR;
	}
	return iflagreturn;
}


/***************************************************************************//**
 @brief Get the backup path from the database
 @details Retrive the Backup path type from the database
 @param spath This variable will be filled with the backup path from the
 database  if return value is different of PATH_EMPTY
 @return  the returned values is a flag of type #PATH_ERROR
 @author Lucien Schreiber (c) CREALP 2007
 @date 28 March 2008
 *******************************************************************************/
int DataBaseTM::GetProjectBackupPath (wxString & spath)
{
	int iflagreturn = PATH_OK;
	
	wxString sSentence = _T("SELECT PRJ_BACKUP_PATH FROM ") + 
						TABLE_NAME_PRJ_SETTINGS;
	if (DataBaseQuery(sSentence) && DataBaseHasResult())
	{
		DataBaseGetNextResult(spath);
		
		if (spath.IsEmpty())
			iflagreturn = PATH_EMPTY;
		else
		{
			// check for path validity
			if (!wxFileName::DirExists(spath))
				iflagreturn = PATH_INVALID;
		}
		
	}
	else // error querying the database
	{
		wxLogDebug(_T("Unable to query the database for project ")
				   _T("backup path. or no results returned : %s"),
				   sSentence.c_str());
		iflagreturn = PATH_DATABASE_ERROR;
	}
	return iflagreturn;

}


/*************************** LAYER DATABASE FUNCTION ****************************/
bool DataBaseTM::AddLayer(ProjectDefMemoryLayers * myLayer)
{
	wxString sSentence = _T("INSERT INTO ") + TABLE_NAME_LAYERS + _T(" (TYPE_CD, LAYER_NAME) VALUES ");
	wxString sValues = wxString::Format(_T("(%d,'%s')"), 
		myLayer->m_LayerType, 
		myLayer->m_LayerName.c_str());
	sSentence.Append(sValues);
	
	//wxLogDebug(sSentence);
	
	if (DataBaseQueryReal(sSentence)==0)//DataBaseQueryMultiple(sSentence) == 0)
	{
		SetActiveLayerId(myLayer);
		return TRUE;
	}
	return FALSE;
}

void DataBaseTM::SetActiveLayerId (ProjectDefMemoryLayers * myLayer)
{
	wxString sSentence = _T("SELECT LAYER_INDEX FROM ") + TABLE_NAME_LAYERS + _(" WHERE LAYER_NAME = ");
	wxString sValues = wxString::Format(_T("'%s'"), myLayer->m_LayerName.c_str());
	sSentence.Append(sValues);
	
	//wxLogDebug(sSentence);
	
	if (DataBaseQuery(sSentence)) // query OK
	{
		m_iDBLayerIndex = DataBaseGetResultAsInt();
		//wxLogDebug(_T("Actual database index is %d"), m_iDBLayerIndex);
	}
	else
		wxLogDebug(_T("Unable to find the layer [%s]"), myLayer->m_LayerName.c_str());
}



int DataBaseTM::GetNextLayer (ProjectDefMemoryLayers * myLayer)
{
	wxArrayString myResults;
	wxString sSentence = wxString::Format(_T("SELECT LAYER_INDEX, TYPE_CD, LAYER_NAME FROM %s"),
										  TABLE_NAME_LAYERS.c_str());
	
	// check if we have some results 
	if (DataBaseResultExists())
	{
		myResults = DataBaseGetNextResult();
		if (!myResults.IsEmpty())
		{
			wxASSERT (myResults.Count() == 3);
			
			// LAYER ID
			myLayer->m_LayerID = wxAtoi(myResults.Item(0));
			
			// LAYER TYPE
			myLayer->m_LayerType = (PRJDEF_LAYERS_TYPE) wxAtoi(myResults.Item(1));
			
			// LAYER NAME
			myLayer->m_LayerName = myResults.Item(2);
			return 1;
		}
		return -1;
		
	}
	else 
	{
		DataBaseQuery(sSentence);
	}
	return 0;
}


/***************************************************************************//**
 @brief Prepare statement for updating layer
 @details This function return the prepared SQL statement for updating or
 inserting data into the #TABLE_NAME_LAYERS. 
 @note the most important thing is
 to be sure that myLayer->m_LayerID is correctly set for function's good
 behaviour.
 @param myLayer pointer to a #ProjectDefMemoryLayers object with new data. In
 case of modification the m_LayerID member must be set correctly
 @param sSqlSentence reference to a wxString in which we are going to put the
 issued sentence
 @return TRUE if a modification statement was issued or FALSE if we
 were preparing an INSERT statement
 @author Lucien Schreiber (c) CREALP 2007
 @date 15 April 2008
 *******************************************************************************/
bool DataBaseTM::UpdateLayer (ProjectDefMemoryLayers * myLayer, wxString & sSqlSentence)
{
	wxString sInsertSentence = _T("");
	long myLastInsertedId = 0;
	
	// update or insert statement based on the m_layerID 
	// greater than 1 for update and 0 if insert is required
	if (myLayer->m_LayerID > 0)
	{
		sSqlSentence.Append(wxString::Format(_T(" UPDATE %s SET TYPE_CD = %d, LAYER_NAME = \"%s\"")
										  _T(" WHERE LAYER_INDEX = %d; "), TABLE_NAME_LAYERS.c_str(), 
										  myLayer->m_LayerType, 
										  myLayer->m_LayerName.c_str(),
										  myLayer->m_LayerID));
		return TRUE;
	}
	else 
		// we need to insert layer immediately to get the real layer_ID.
		// used later during the attribute table creation process.

	{
		sInsertSentence = wxString::Format(_T(" INSERT INTO %s (TYPE_CD, LAYER_NAME) ")
										_T("VALUES (%d,\"%s\"); "),
										TABLE_NAME_LAYERS.c_str(), 
										myLayer->m_LayerType, 
										myLayer->m_LayerName.c_str());
		if(DataBaseQueryNoResult(sInsertSentence))
		{
			// get the last inserted id
			myLastInsertedId = DataBaseGetLastInsertID();
			if (myLastInsertedId > 0)
			{
				myLayer->m_LayerID = myLastInsertedId;
			}
			else
				wxLogError(_T("Error, last inserted ID is not bigger than 0 : %d"),
						   myLastInsertedId);
		}
		else
			wxLogError(_T("Problem inserting layer : %s"), sInsertSentence.c_str());
		
		return FALSE;	
	}

	
}


/***************************************************************************//**
 @brief Prepare statement for deleting layer(s)
 @details This append to the sentence the SQL code for deleting all layers which
 have their IDs stored into the deletelist array. This function operate on the
 #TABLE_NAME_LAYERS table.
 @param deletelist A wxArrayLong containing all ID we want to delete
 @param sSqlSentence SQL sentence which will be modified with the new delete
 statement
 @return  allways true for the moment
 @author Lucien Schreiber (c) CREALP 2007
 @date 16 April 2008
 *******************************************************************************/
bool DataBaseTM::DeleteLayer (const wxArrayLong & deletelist, wxString & sSqlSentence)
{
	wxString sDeleteSentence = _T("");
	
	for (unsigned int i= 0; i<deletelist.GetCount();i++)
	{
		sSqlSentence.Append(wxString::Format(_T(" DELETE FROM %s WHERE LAYER_INDEX = %d; "),
											 TABLE_NAME_LAYERS.c_str(),
											 deletelist.Item(i)));
		
		// delete associated field table (if existing)
		if(!DeleteTableIfExist(TABLE_NAME_LAYER_AT + wxString::Format(_T("%d"),
																	  deletelist.Item(i))))
		{
			wxLogError(_T("Error deleting table : %s%d"),
					   TABLE_NAME_LAYER_AT.c_str(),
					   deletelist.Item(i));
		}
		
		// prepare delete attribution objects from the object table
		DeleteLayersObjects(deletelist.Item(i), sDeleteSentence);
		
		
		///@todo prepare delete GIS objects from the gis tables
		
		
		// execute prepared statement
		if(!DataBaseQueryNoResult(sDeleteSentence))
			wxLogError(_T("Error deleting objects (GIS or attribution) : %s"),
					   sDeleteSentence.c_str());
		
	}
	return TRUE;
}


/*************************** OBJECT DATABASE FUNCTION ****************************/
bool DataBaseTM::AddObject (ProjectDefMemoryObjects * myObject, int DBlayerIndex)
{
	// get the selected layer of take the actual one
	if (DBlayerIndex == -1)
	{
		DBlayerIndex = GetActiveLayerId(); 
	}
	
	wxString sSentence = _T("INSERT INTO ") + TABLE_NAME_OBJECTS + _T(" (OBJECT_CD, THEMATIC_LAYERS_LAYER_INDEX, OBJECT_DESC) VALUES ");
	wxString sValues = wxString::Format(_T("(%d,%d,\"%s\")"),myObject->m_ObjectCode,
										DBlayerIndex, myObject->m_ObjectName.c_str());
	sSentence.Append(sValues);
	
	//wxLogDebug(sSentence);
	
	if (DataBaseQueryNoResult(sSentence))
	{
		return TRUE;
	}
	
	wxLogDebug(_T("Unable to AddObject to the database"));
	return FALSE;
}



/***************************************************************************//**
 @brief Insert or update an object
 @details This function insert or update an object in the database depending of
 the myObject->m_Object_ID value : bigger or egal to 0 means to update and
 smaller than 0 means to insert
 @param myObject parameter of the object to modify into the DB
 @return  TRUE if all went OK or FALSE if the query or something else dosen't
 work as espected
 @author Lucien Schreiber (c) CREALP 2007
 @date 18 March 2008
 *******************************************************************************/
bool DataBaseTM::EditObject (ProjectDefMemoryObjects * myObject )
{
	// get layer id
	ProjectDefMemoryLayers myLayer;
	myLayer.m_LayerName = myObject->m_ParentLayerName;
	
	// if layer name isn't empty
	if (!myLayer.m_LayerName.IsEmpty())
	{
		SetActiveLayerId(&myLayer);
		
		
		// prepare the sentence for insert or update
		//sentence for insert
		wxString sInsert = wxString::Format(_T("INSERT INTO %s ")
											_T("(OBJECT_CD, THEMATIC_LAYERS_LAYER_INDEX, OBJECT_DESC, OBJECT_ISFREQ) ")
											_T("VALUES (%d, %d, \"%s\", %d)"),
											TABLE_NAME_OBJECTS.c_str(),
											myObject->m_ObjectCode,
											GetActiveLayerId(),
											myObject->m_ObjectName.c_str(),
											(int) myObject->m_ObjectFreq);
		// sentence for update
		wxString sUpdate = wxString::Format(_T("UPDATE %s ")
											_T("SET OBJECT_CD = %d, THEMATIC_LAYERS_LAYER_INDEX = %d,")
											_T("OBJECT_DESC = \"%s\", OBJECT_ISFREQ = %d ")
											_T("WHERE OBJECT_ID = %d"),
											TABLE_NAME_OBJECTS.c_str(),
											myObject->m_ObjectCode,
											GetActiveLayerId(),
											myObject->m_ObjectName.c_str(),
											(int) myObject->m_ObjectFreq,
											myObject->m_ObjectID);
		
		// if id >= 0 we update (item exist)
		// otherwise we insert (item dosen't exist)
		if (myObject->m_ObjectID >= 0)
		{
			wxLogDebug(sUpdate);
			if (DataBaseQueryNoResult(sUpdate))
				return TRUE;
		}
		else
		{
			if (DataBaseQueryNoResult(sInsert))
				return TRUE;
		}
		
	}
	
	
	wxLogDebug(_T("Error editing object into the database"));
	return FALSE;
}




bool DataBaseTM::DataBaseGetNextResultAsObject(ProjectDefMemoryObjects * object,  int ilayertype)
{
	MYSQL_ROW record;
	bool bReturnValue = FALSE;
	int iCol = 3;
	long lFreq = 0;
	//wxArrayString myRowResult;
	
	if (m_resultNumber > 0 && pResults != NULL)
	{
		record = mysql_fetch_row(pResults);
		if(record != NULL)
		{
			// check object validity
			if (object != NULL) // && m_resultNumber)
			{
				wxString (record[0], wxConvUTF8).ToLong( &(object->m_ObjectCode));
				object->m_ObjectName = wxString (record[1], wxConvUTF8);
				object->m_ParentLayerName = wxString (record[2], wxConvUTF8);
				
				// if we search the frequency
				if (ilayertype == LAYER_LINE)
				{
					wxString (record[iCol], wxConvUTF8).ToLong (&lFreq);
					object->m_ObjectFreq = (PRJDEF_OBJECTS_FREQ) lFreq;
					iCol ++;
				}
				
				// get the id
				wxString (record[iCol], wxConvUTF8).ToLong (&(object->m_ObjectID)); 
				bReturnValue = TRUE;
			}
			
		}
		else 
		{
			// clean
			m_resultNumber=0;
			mysql_free_result(pResults);
			pResults = NULL;
		}		
	}
	
	return bReturnValue;

}




int DataBaseTM::DeleteMultipleObjects (PrjDefMemManage * pProjet)
{
	wxString sSentence = _T("");
	wxString sPattern = _T("");
	
	// prepare statement for multiple delete
	for (unsigned int i=0; i< pProjet->m_StoreDeleteIDObj.GetCount(); i++)
	{
		sSentence.Append(wxString::Format(_T("DELETE FROM %s WHERE OBJECT_ID = %d; "),
										  TABLE_NAME_OBJECTS.c_str(),
										  pProjet->m_StoreDeleteIDObj.Item(i)));
	}
	
	// execute statement
	if(DataBaseQueryNoResult(sSentence))
		return pProjet->m_StoreDeleteIDObj.GetCount();
	
	return -1;
}

/***************************************************************************//**
 @brief Delete all objects from a layer
 @details Delete all objects belonging to a layers. May be used when one try to
 delete a layer.
 This function dosen't delete the table but issued a sentence
 for this purpose.
 @param iLayer Index of the layer (Starts at 1 in MySQL)
 @param sSqlSentence The sentence for storing the SQL sentence issued in the
 function
 @author Lucien Schreiber (c) CREALP 2007
 @date 28 April 2008
 *******************************************************************************/
void DataBaseTM::DeleteLayersObjects (int iLayer, wxString & sSqlSentence)
{
	sSqlSentence.Append(wxString::Format(_T(" DELETE FROM %s WHERE THEMATIC_LAYERS_LAYER_INDEX = %d; "),
										 TABLE_NAME_OBJECTS.c_str(), iLayer));
}

/***************************************************************************//**
 @brief Delete a Table
 @details Delete a table if this table exists
 @param TableName Name of the table to delete
 @return  Return True if the table was deleted
 @author Lucien Schreiber (c) CREALP 2007
 @date 28 April 2008
 *******************************************************************************/
bool DataBaseTM::DeleteTableIfExist (const wxString & TableName)
{
	wxString sSentence = wxString::Format(_T("DROP TABLE IF EXISTS %s; "),
										  TableName.c_str());
	if (DataBaseQueryNoResult(sSentence))
		return TRUE;
	return FALSE;
}

/*************************** FIELD DATABASE FUNCTION [ PRIVATE ] *******************/
bool DataBaseTM::AddTableIfNotExist (const wxString & TableName)
{
	wxString sCreateTable1 = _T("CREATE  TABLE IF NOT EXISTS `");
	wxString sValues = wxString::Format(_T("%s` ("), TableName.c_str());
	wxString sCreateTable2 = _T("  `LAYER_AT_ID` INT UNSIGNED NOT NULL AUTO_INCREMENT ,")
	_T("  `OBJECT_ID` INT UNSIGNED NOT NULL     ,")
	_T("  PRIMARY KEY (`LAYER_AT_ID`) ,")
	_T("  INDEX LAYER_ATX_FKIndex1 (`OBJECT_ID` ASC) ,")
	_T("  CONSTRAINT `Rel_09`")
	_T("    FOREIGN KEY (`OBJECT_ID` )")
	_T("    REFERENCES  `generic_points` (`OBJECT_ID` ));");
	sCreateTable1.Append(sValues + sCreateTable2);
	
	
	if (DataBaseQueryNoResult(sCreateTable1))
	{
		//wxLogDebug(_T("Table Creation [%s]... DONE"), TableName.c_str());	
		return TRUE;
	}
	wxLogDebug(_T("Table Creation [%s]... FAILED"), TableName.c_str());
	return FALSE;
}



/*************************** FIELD DATABASE FUNCTION ****************************/
bool DataBaseTM::AddField (ProjectDefMemoryFields * myField, int DBlayerIndex)
{
	bool bReturnValue = TRUE;
	wxString sSentence = _T("");
	
	// get the selected layer of take the actual one
	if (DBlayerIndex == -1)
	{
		DBlayerIndex = GetActiveLayerId(); 
	}
	
	// create the table name
	wxString sTableName = wxString::Format(TABLE_NAME_LAYER_AT + _("%d"), DBlayerIndex);
	
	// first we must create the table if not exist
	AddTableIfNotExist(sTableName);
	
	// check that the table exists.
	bReturnValue = DataBaseTableExist(sTableName);
	if (bReturnValue)
	{
		// then create the fields based upon the fields type
		// use the same function for updating 
		UpdateField(myField, DBlayerIndex, sSentence);
		bReturnValue = DataBaseQueryNoResult(sSentence);
	}
	
	if (bReturnValue == FALSE)
		wxLogError(_T("Unable to add field to the database : %s"), sSentence.c_str());
	
	return bReturnValue;
}


/***************************************************************************//**
 @brief Fill an existing project with fields
 @details The myPrj parameter must point to a existing project in memory.
 containing all layers. This function will add all fields into this project
 @param myPrj Pointer to an existing #PrjDefMemManage containing all layers
 @return  return the number of fields added, or -1 if something goes wrong
 @author Lucien Schreiber (c) CREALP 2008
 @date 19 June 2008
 *******************************************************************************/
int DataBaseTM::GetFieldsFromDB (PrjDefMemManage * myPrj)
{
	// some checks 
	if (!myPrj)
	{
		wxLogDebug(_T("Pointer to project in memory is empty"));
		return -1;
	}
	
	// getting data from database
	wxString sSentence = _T("SELECT CAST(SUBSTR(TABLE_NAME FROM 9) AS UNSIGNED) LAYER_INDEX,")
	_T("COLUMN_NAME, COLUMN_TYPE, COLUMN_COMMENT FROM INFORMATION_SCHEMA.COLUMNS")
	_T(" WHERE table_schema = \"") + DataBaseGetName() + _T("\"")
	_T(" AND  table_name IN (SELECT TABLE_NAME FROM")
	_T(" INFORMATION_SCHEMA.TABLES WHERE TABLE_NAME LIKE \"") +TABLE_NAME_LAYER_AT +
	_T("\%\") AND COLUMN_NAME NOT IN ( 'OBJECT_ID', 'LAYER_AT_ID') ORDER BY LAYER_INDEX");
	
	if(DataBaseQuery(sSentence) == FALSE)
	{
		wxLogDebug(_T("Error gettings fields from database : %s"),sSentence.c_str());
		return -1;
	}
	
	
	// results must contain some results
	if (!DataBaseHasResult())
	{
		wxLogDebug(_T("No results found for the fields"));
		return -1;
	}
	
	int iNbFields = 0;
	long iRealLayerID = 0;
	wxArrayString myResults;
	ProjectDefMemoryLayers * myLayer = NULL;
	ProjectDefMemoryFields * myField = NULL;
	while (1) // loop until no more results
	{
		myResults = DataBaseGetNextResult();
		// if no more results : break
		if (myResults.IsEmpty())
			break;
		
		// find layer by real id
		myResults.Item(0).ToLong(&iRealLayerID);
		myLayer = myPrj->FindLayerByRealID(iRealLayerID);
		if (!myLayer)
		{
			wxLogDebug(_T("Layer error, layer %d not found"), iRealLayerID);
			break;
		}
		
		// add field and set values to this field
		iNbFields ++;
		myField = myPrj->AddField();
		myField->SetValues(myResults);
		
		myResults.Clear();
	}
	
	
	return iNbFields; 

}


int DataBaseTM::GetNextField (ProjectDefMemoryFields * myField, int DBlayerIndex)
{
	// if no result execute the sentence
	// otherwise parse the results
	if (!DataBaseHasResult())
	{
		wxLogDebug(_T("No results found for the fields"));
		return -2;
	}
	
	
	
	wxArrayString myResults;
	myResults = DataBaseGetNextResult();
	if (!myResults.IsEmpty())
	{
		myField->SetValues(myResults);
		
		return 1; // ok continue
	}
	return -1; // no more results 
	
	
	/*wxString sSentence = wxString::Format(_T("SHOW FULL COLUMNS FROM %s%d"),
										  TABLE_NAME_LAYER_AT.c_str(),
										  DBlayerIndex);
	
	// check if we have some results 
	if (DataBaseHasResult())
	{
		myResults = DataBaseGetNextResult();
		if (!myResults.IsEmpty())
		{
			myField->SetValues(myResults);
			
			return 1; // ok continue
		}
		return -1; // no more results 
		
		
	}
	else
	{
			// get all fields
			if (DataBaseQuery(sSentence))
			{
				// skip two first lines 
				DataBaseGetNextResult(); DataBaseGetNextResult(); 

				return 0; // ok query done
			}
			
	}
	return -2; // error*/
}


/***************************************************************************//**
 @brief Prepare statement for updating fields
 @details This function prepare the SQL sentence used for updating the fields.
 The main concept is the ProjectDefMemoryFields::m_FieldID :
 - if this integer is greather than 0, we update the field
 - if this integer is egal to 0, we prepare a sentence for adding a new field
 @param myField a #ProjectDefMemoryFields object containing all the field data
 @param iLayer The layer index the field belong to. 
 @param sSqlSentence the string containing the SQL sentence we want to modify
 @return  Allways true for the moment
 @author Lucien Schreiber (c) CREALP 2007
 @date 23 April 2008
 *******************************************************************************/
bool DataBaseTM::UpdateField(ProjectDefMemoryFields * myField,int iLayer, wxString & sSqlSentence)
{
	wxString myTypeFieldTemp = _T("");
	
	switch (myField->m_FieldID)
	{
		case 0: // we insert
			myField->GetStringTypeFromValues(myTypeFieldTemp);
			sSqlSentence.Append(wxString::Format(_T("ALTER TABLE %s%d ADD COLUMN %s %s; "),
								TABLE_NAME_LAYER_AT.c_str(),
								iLayer,
								myField->m_Fieldname.c_str(),
								myTypeFieldTemp.c_str()));
			break;

		default: // modifiy the field
			myField->GetStringTypeFromValues(myTypeFieldTemp);
			sSqlSentence.Append(wxString::Format(_T("ALTER TABLE %s%d CHANGE COLUMN %s %s %s; "),
												 TABLE_NAME_LAYER_AT.c_str(),
												 iLayer,
												 myField->m_FieldOldName.c_str(),
												 myField->m_Fieldname.c_str(),
												 myTypeFieldTemp.c_str()));
			
			break;
	}
	return TRUE;
}


/***************************************************************************//**
 @brief Prepare statement for deleting
 @details This prepare the SQL statement for deleting fields stored into the
 PrjDefMemManage::m_StoreDeleteFields.
 @param myFields the array containing the name of the fields we want to delete
 @param iLayer the zero based layer index of the fields (Number used for table
 name)
 @param sSqlSentence The string we filled with the issued SQL sentence
 @return  TRUE if the passed Array (myFields) is greater than 0
 @author Lucien Schreiber (c) CREALP 2007
 @date 28 April 2008
 *******************************************************************************/
bool DataBaseTM::DeleteField (wxArrayString & myFields, int iLayer, wxString & sSqlSentence)
{
	sSqlSentence.Clear();
	int myNumberOfItems = myFields.GetCount();
	for (unsigned int i = myNumberOfItems ;i>0 ; i--)
	{
		sSqlSentence.Append(wxString::Format(_T("ALTER TABLE %s%d DROP COLUMN %s; "),
											 TABLE_NAME_LAYER_AT.c_str(),
											 iLayer,
											 myFields.Item(i-1).c_str()));
	}
	
	if (myNumberOfItems > 0)
		return TRUE;
	return FALSE;
}




/*************************** DATABASE QUERY FUNCTION ****************************/
/***************************************************************************//**
 @brief Retrive objects from the object table
 @details This functions may be called for getting all objects from within a
 layer type. This is used by the "Object definition dialog" but also by the
 "Attribution window".
 @param ilayertype the zero based type of layer, see #PRJDEF_LAYERS_TYPE
 @param bOrder Return the result in the "RANK" order if set to TRUE
 @return  FALSE if the query wasn't valid
 @author Lucien Schreiber (c) CREALP 2008
 @date 21 May 2008
 *******************************************************************************/
bool DataBaseTM::GetObjectListByLayerType(int ilayertype, bool bOrder)
{
	// different request based on the ilayertype (4 fields for LAYER_LINE and
	// 3 fields in other cases
	wxString sWantFrequencyField = _T("");
	if (ilayertype == LAYER_LINE)
		 sWantFrequencyField = _T("OBJECT_ISFREQ, ");
	
	
	
	wxString sSentence =  wxString::Format(
										   _T("SELECT OBJECT_CD, OBJECT_DESC, thematic_layers.LAYER_NAME, %s OBJECT_ID ")
										   _T("FROM dmn_layer_object LEFT JOIN (thematic_layers) ")
										   _T("ON (thematic_layers.LAYER_INDEX=dmn_layer_object.THEMATIC_LAYERS_LAYER_INDEX)")
										   _T("WHERE thematic_layers.TYPE_CD = %d"),
										   sWantFrequencyField.c_str(),
										   ilayertype);
	
	if (bOrder)
		sSentence.Append(_T(" ORDER BY RANK "));
	
	if (DataBaseQuery(sSentence))
	{
		return TRUE;
	}
	wxLogDebug(_T("Error getting the list of object by layertype : %s"), sSentence.c_str());
	return FALSE;
	
}


wxArrayString DataBaseTM::GetLayerNameByType (int ilayertype)
{
	wxArrayString myThematicResult;
	wxString myTempResult;
	
	wxString sSentence = wxString::Format(_T("SELECT LAYER_NAME FROM %s WHERE TYPE_CD = %d"),
										  TABLE_NAME_LAYERS.c_str(), ilayertype);
	
	if (DataBaseQuery(sSentence))
	{
		wxLogDebug(_T("Number of results found : %d"), DatabaseGetCountResults());	
		
		while (DataBaseGetNextResult(myTempResult))
		{
			myThematicResult.Add(myTempResult);
		}

	}
	else 
	{
		wxLogDebug(_T("Error getting list of layer for specified listtype : %d"), ilayertype);
	}

	return myThematicResult;
}


/********************************** SCALE OPERATIONS **********************************/
long DataBaseTM::GetNextScaleValue (long & DBindex, bool bFirst)
{
	long myResultScale = -1;
	wxArrayLong myResults;
	
	// no result, we process the sentence
	if (!DataBaseHasResult() || bFirst == TRUE)
	{
	
		wxString sSentence = wxString::Format(_T("SELECT ZOOM_ID, SCALE_VALUE FROM %s ORDER BY RANK"),
											  TABLE_NAME_SCALE.c_str());
		
		if (!DataBaseQuery(sSentence))
		{
			wxLogDebug(_T("Error getting scale data from the database : %s"), sSentence.c_str());
		}	
	}
	
	DataBaseGetNextResultAsLong(myResults);
	if (myResults.GetCount() > 0)
	{
		DBindex = myResults.Item(0);
		myResultScale = myResults.Item(1);
	}
	
	if (myResultScale == -1)
		DataBaseDestroyResults();
	
	return myResultScale;
}


bool DataBaseTM::EditScale (ProjectDefMemoryScale * myScaleObj)
{
	// prepare the sentence for insert or update
	//sentence for insert
	wxString sInsert = wxString::Format(_T("INSERT INTO %s ")
										_T("(SCALE_VALUE) ")
										_T("VALUES (%d)"),
										TABLE_NAME_SCALE.c_str(),
										myScaleObj->m_ScaleValue);
	// sentence for update
	wxString sUpdate = wxString::Format(_T("UPDATE %s ")
										_T("SET SCALE_VALUE = %d ")
										_T("WHERE ZOOM_ID = %d"),
										TABLE_NAME_SCALE.c_str(),
										myScaleObj->m_ScaleValue,
										myScaleObj->m_DBScaleID);
										
	// if id >= 0 we update (item exist)
	// otherwise we insert (item dosen't exist)
	if (myScaleObj->m_DBScaleID >= 0)
	{
		wxLogDebug(sUpdate);
		if (DataBaseQueryNoResult(sUpdate))
			return TRUE;
	}
	else
	{
		if (DataBaseQueryNoResult(sInsert))
			return TRUE;
	}
	
	
	wxLogDebug(_T("Error editing scale into the database"));
	return FALSE;
	
	
}


int DataBaseTM::DeleteMultipleScales (PrjDefMemManage * pProjet)
{
	wxString sSentence = _T("");
	wxString sPattern = _T("");
	
	// prepare statement for multiple delete
	for (unsigned int i=0; i< pProjet->m_StoreDeleteScale.GetCount(); i++)
	{
		sSentence.Append(wxString::Format(_T("DELETE FROM %s WHERE ZOOM_ID = %d; "),
										  TABLE_NAME_SCALE.c_str(),
										  pProjet->m_StoreDeleteScale.Item(i)));
	}
	
	// execute statement
	if(DataBaseQueryNoResult(sSentence))
		return pProjet->m_StoreDeleteIDObj.GetCount();
	
	wxLogDebug(_T("Error trying to delete scale from the database : %s"), sSentence.c_str());
	
	return -1;
}

/********************************** RANK DATABASE OPERATIONS **********************************/
/***************************************************************************//**
 @brief Update a rank field
 @details This may be used to update a rank field using items from a
 #ListGenReport. This way we can store the order of items wanted by the end
 user
 @param list a pointer to a #ListGenReport object containing items
 @param icol specify the column of interest in the list (zero based index)
 @param stable the name of the table in which we want to add rank values
 @param sfield the field name for the field containing values of the selected
 column (icol)
 @param bStringType TRUE if the column store textual values, FALSE otherwise. In
 fact this function prepend brackets if TRUE for dealing with strings with
 spaces (Default is FALSE)
 @param rankfield Name of the field where we want to update the rank (Default is
 field "RANK") this field must be of type integer
 @return  Return TRUE if the query pass without problem and FALSE otherwise
 @author Lucien Schreiber (c) CREALP 2007
 @date 01 April 2008
 *******************************************************************************/
bool DataBaseTM::SetRank (ListGenReport * list, 
						  int icol, 
						  const wxString & stable,
						  const wxString & sfield,
						  bool bStringType,
						  const wxString & rankfield)
{
	wxString myText = _T("");
	wxString sSentence = _T("");
	
	// loop all items from the list and update the rank in one pass...
	for (int i= 0; i< list->GetItemCount(); i++)
	{
		myText = list->GetItemColText(i, icol);
		
		// if this is a string value we add brackets...
		if (bStringType)
		{
			myText.Prepend(_T("\""));
			myText.Append(_T("\""));
		}
		
		// prepare the sentence
		sSentence.Append(wxString::Format(_T("UPDATE %s SET %s = %d WHERE %s = %s; "),
										  stable.c_str(),
										  rankfield.c_str(),
										  i,
										  sfield.c_str(),
										  myText.c_str()));
	
	}
	
	// process the sentence
	if (DataBaseQueryNoResult(sSentence))
	{
		return TRUE;
	}
	
	wxLogDebug(_T("Error updating RANK : %s"), sSentence.c_str());
	return FALSE;
}



/***************************************************************************//**
 @brief Update a rank field for scale table
 @details This function is a specialized function of DataBaseTM::SetRank() used
 to update rank for scale table (because of scale table store only long values
 but displays 1: before the scale value
 @param list a pointer to a #ScaleList object containing items
 @param icol specify the column of interest in the list (zero based index)
 @param stable the name of the table in which we want to add rank values
 @param sfield the field name for the field containing values of the selected
 column (icol)
 @param rankfield Name of the field where we want to update the rank (Default is
 field "RANK") this field must be of type integer
 @return  Return TRUE if the query pass without problem and FALSE otherwise
 @author Lucien Schreiber (c) CREALP 2007
 @date 01 April 2008
 *******************************************************************************/
bool DataBaseTM::SetScaleRank (ScaleList * list, int icol, 
							   const wxString & stable,
							   const wxString & sfield,
							   const wxString & rankfield)
{
	long myScale = 0;
	wxString sSentence = _T("");
	
	// loop all items from the list and update the rank in one pass...
	for (int i= 0; i< list->GetItemCount(); i++)
	{
		myScale = list->GetScaleFromList(i);
		
		// prepare the sentence
		sSentence.Append(wxString::Format(_T("UPDATE %s SET %s = %d WHERE %s = %d; "),
										  stable.c_str(),
										  rankfield.c_str(),
										  i,
										  sfield.c_str(),
										  myScale));
		
	}
	
	// process the sentence
	if (DataBaseQueryNoResult(sSentence))
	{
		return TRUE;
	}
	
	wxLogDebug(_T("Error updating scale RANK : %s"), sSentence.c_str());
	return FALSE;
}


/********************************** PROJECT DATABASE OPERATIONS **********************************/
PrjDefMemManage * DataBaseTM::GetProjectDataFromDB ()
{
	PrjDefMemManage * myPrjDef = new PrjDefMemManage();
	ProjectDefMemoryLayers * mypLayer = NULL;
	//ProjectDefMemoryFields * mypField = NULL;
	
	int iLayerAdded = 0, iReturnValue = 0; // iReturnFieldValue=0; //  iFieldAdded = 0
	//wxArrayInt myLayerIDArray;
	
	// Load General project data (path, name,...)
	if (GetProjectData(myPrjDef))
	{
		// clear database results
		DataBaseDestroyResults();

		
		
		// STEP 1
		// get all layers 
		while (1)
		{
			mypLayer = myPrjDef->AddLayer();
			iReturnValue = GetNextLayer(mypLayer);
	
			// item not found 
			if (iReturnValue != 1)
			{
				// remove last layer
				myPrjDef->RemoveLayer();
				iLayerAdded--;
			}
			//else	// add layer id to array
			//	myLayerIDArray.Add(mypLayer->m_LayerID);
			
			iLayerAdded++;
			
			// no more results 
			if (iReturnValue == -1)
				break;
			
		}
		
		
		int iNumFieldAdded = GetFieldsFromDB(myPrjDef);
		
		wxLogDebug(_T("Number of fields parsed %d"), iNumFieldAdded);
		wxLogDebug(_T("Nb of layers found in db : %d"), iLayerAdded);
		
		return myPrjDef;
	}
	
	wxLogError(_T("Error loading basic project data"));
	return NULL;
}


/***************************************************************************//**
 @brief Update database
 @details This function update the project's data. This is a "clever" function,
 only modified data is changed
 @param pProjDef Object of type #PrjDefMemManage containing new project values.
 @return  return TRUE if project was updated successfully
 @author Lucien Schreiber (c) CREALP 2007
 @date 14 April 2008
 *******************************************************************************/
bool DataBaseTM::UpdateDataBaseProject (PrjDefMemManage * pProjDef)
{
	wxString sSentence = _T("");
	wxString sFieldSentence = _T("");
	wxString sDeleteString = _T("");
	ProjectDefMemoryLayers * pLayers = NULL;
	
	/********* PART 1 : BASIC PROJECT UPDATING *********************/
	if (SetProjectData(pProjDef))
	{
		
		/********* PART 2 : DELETING AND CLEANING *********************/ 
		// we prepare the sentence for deleting layers
		// based on the delete array
		DeleteLayer(pProjDef->m_StoreDeleteLayers, sSentence);
		
		// execute the sentence for deleting
		if (!sSentence.IsEmpty())
			if (!DataBaseQueryNoResult(sSentence))
				wxLogError(_T("Error deleting layers : %s"), sSentence.c_str());
		
		// clean the delete sentence
		sSentence.Clear();
		
		
		/********* PART 3 : INSERT AND UPDATE *********************/
		
		// prepare sentence for updating layers (insert or modify)
		for (int i = 0; i< pProjDef->GetCountLayers(); i++)
		{
			pLayers = pProjDef->GetNextLayer();
			UpdateLayer(pLayers, sSentence);
			
			// check if a table exists for fields 
			if(DataBaseTableExist(wxString::Format(_T("%s%d"),
												   TABLE_NAME_LAYER_AT.c_str(),
												   pLayers->m_LayerID)) )
			{
				//wxLogDebug(_T("Fields Table exist for layer : %s, [%d]"),
				//		   pLayers->m_LayerName.c_str(),
				//		   pLayers->m_LayerID);
				
				// check if we have some fields 
				if (pProjDef->GetCountFields() > 0)
				{
					
					// fields exists, we update, insert them
					sFieldSentence.Clear();
					for (int j = 0; j < pProjDef->GetCountFields(); j++)
					{
						UpdateField(pProjDef->GetNextField(), pLayers->m_LayerID, sFieldSentence);
					}
					
					
					// modfiy the fields in the database
					if(!DataBaseQueryNoResult(sFieldSentence))
						wxLogError(_T("Error modifing field data in the database : %s"),
								   sFieldSentence.c_str());
				}
				
				
				// process for deleting fields // normally the table should exists 
				sDeleteString.Clear();
				if (pLayers->m_StoreDeleteFields.GetCount() > 0)
				{
					DeleteField(pLayers->m_StoreDeleteFields, pLayers->m_LayerID,
								sDeleteString);
					
					if(!DataBaseQueryNoResult(sDeleteString))
						wxLogError(_T("Unable to delete Fields from table : %s%d, %s"),
								   TABLE_NAME_LAYER_AT.c_str(),
								   pLayers->m_LayerID,
								   sDeleteString.c_str());
				}
				
				
			}
			
			
			
			
			
			else // the table dosen't exist, need to create the table if fields > 0
			{
				if (pProjDef->GetCountFields() > 0)
				{
					// create the table 
					if(!AddTableIfNotExist(wxString::Format(TABLE_NAME_LAYER_AT + _T("%d"),
															pLayers->m_LayerID)))
					{
						wxLogError(_T("Unable to create the Field's table : %d"),
								   pLayers->m_LayerID);
					}
					
					// update, insert fields
					sFieldSentence.Clear();
					for (int j = 0; j < pProjDef->GetCountFields(); j++)
					{
						UpdateField(pProjDef->GetNextField(), pLayers->m_LayerID, sFieldSentence);
					}
					
					
					// modfiy the fields in the database
					if(!DataBaseQueryNoResult(sFieldSentence))
						wxLogError(_T("Error modifing field data in the database : %s"),
								   sFieldSentence.c_str());
				}
			}
			
			
			
		}
		
	
		
		// execute the sentence for layers.
		if (DataBaseQueryNoResult(sSentence))
		{
			return TRUE;
		}	
		
	}
	
	wxLogError(_T("Error updating project in the database"));
	return FALSE;	
}



/***************************************************************************//**
 @brief Init the TOC
 @details This function fill the TOC with generic layers which are :
 - <B>generic_points</B> : For storing all points
 - <B>generic_lines</B> : For storing all lines and limits of polygons
 - <B>generic_labels</B> : For storing all polygon's labels
 - <B>generic_notes</B> : For storing all notes, errors, questions. 
 @note User could'nt remove those layers. Value of column GENERIC_LAYERS is linked to
 the #TOC_GENERIC_NAME
 @return  TRUE if table was succesfully filled, FALSE if an error occur (see log)
 @author Lucien Schreiber (c) CREALP 2008
 @date 26 June 2008
 *******************************************************************************/
bool DataBaseTM::InitTOCGenericLayers()
{
	// check that a database is opened
	if (!DataBaseIsOpen())
	{
		wxLogDebug(_T("Database not opened, open database first"));
		return FALSE;
	}
	
	
	int iTypes [] = {LAYER_LINE, LAYER_POINT, LAYER_POINT, LAYER_POINT, LAYER_POLYGON};

	wxString sSentence = _T("INSERT INTO ") + TABLE_NAME_TOC + _T(" (TYPE_CD, ")
	_T("CONTENT_NAME, GENERIC_LAYERS) VALUES (%d, \"%s\", %d); ");
	wxString sRealSentence = _T("");
	
	for (int i = TOC_NAME_LINES; i<= TOC_NAME_FRAME; i++)
	{
		sRealSentence.Append(wxString::Format(sSentence,iTypes[i],
											  TOC_GENERIC_NAME_STRING[i].c_str(),
											  TOC_NAME_LINES+i));
	}
	
	if (!DataBaseQueryNoResult(sRealSentence))
	{
		wxLogDebug(_T("Error in TOC init : ") + sRealSentence);
		return FALSE;
	}
	
	return TRUE;
}


/***************************************************************************//**
 @brief Get entry from the TOC
 @details This function returns all entry from the TOC one by one. Only one
 request is issued and then each time this function is called the next layer is
 returned
 @return  a #tmLayerProperties pointer, or NULL if no more layer to proceed
 @note if result isn't NULL, you need to delete pointer after use.
 @author Lucien Schreiber (c) CREALP 2008
 @date 07 July 2008
 *******************************************************************************/
tmLayerProperties * DataBaseTM::GetNextTOCEntry()
{
	wxArrayString myTempResults;
	
	if(!DataBaseHasResult())
	{
		
		wxString sSentence = _T("SELECT CONTENT_ID, TYPE_CD, CONTENT_PATH, ")
		_T("CONTENT_NAME, CONTENT_STATUS, GENERIC_LAYERS, SYMBOLOGY, VERTEX_FLAGS FROM ") + TABLE_NAME_TOC +
		_T(" ORDER by RANK");
		
		if (!DataBaseQuery(sSentence))
		{
			wxLogDebug(_T("Error getting layers from the TOC : %s"), sSentence.c_str());
			return NULL;
		}
	}
	
	if (DataBaseHasResult())
	{
		myTempResults = DataBaseGetNextResult();
		
		if (myTempResults.GetCount() == 0)
		{
			return NULL;
		}
				
		if (myTempResults.GetCount() != 8)
		{
			wxLogDebug(_T("Error with the results : attended 8 results, got %d"),
					   myTempResults.GetCount());
			return NULL;
		}
		
		// parsing results
		tmLayerProperties * myLayerProp = new tmLayerProperties();
		myLayerProp->InitFromArray(myTempResults);
		myTempResults.Clear();
		
		
		return myLayerProp;
		
		
		
	}
	
	return NULL;
}



/***************************************************************************//**
 @brief Add TOC item to the database
 @details Called when a new layer is added.
 @param item The item to add to the database
 @return  The database ID of the added item or -1 if an error occur
 @author Lucien Schreiber (c) CREALP 2008
 @date 14 July 2008
 *******************************************************************************/
long DataBaseTM::AddTOCLayer (tmLayerProperties * item)
{
	DataBaseDestroyResults();

	// converting path to windows path
	// do nothing if not a windows path.
	wxString myPath = item->m_LayerPathOnly;
	DataBase::DataBaseConvertWindowsPath(myPath);

	wxString sSentence = wxString::Format(_T("INSERT INTO ") + TABLE_NAME_TOC +
										  _T(" (TYPE_CD, CONTENT_PATH, CONTENT_NAME,")
										  _T("CONTENT_STATUS, GENERIC_LAYERS) ") +
										  _T(" VALUES ( %d, \"%s\", \"%s\", %d, %d ); "),
										  item->m_LayerSpatialType,
										  myPath.c_str(),
										  item->m_LayerNameExt.c_str(),
										  item->m_LayerVisible,
										  item->m_LayerType);
		
	if(!DataBaseQueryNoResult(sSentence))
	{
		wxLogDebug(_T("Error inserting layer into database : %s"), sSentence.c_str());
		return -1;
	}
	
	// getting ID back
	sSentence = (_T("SELECT CONTENT_ID FROM ") + TABLE_NAME_TOC +
				 _T(" ORDER BY CONTENT_ID DESC LIMIT 1; "));
	
	if (!DataBaseQuery(sSentence))
	{
		wxLogDebug(_T("Error getting the last layer id"));
		return -1;
		
	}		
	if(!DataBaseHasResult())
	{
		wxLogDebug(_T("Error no results returned, not able to know the layer ID"));
		return -1;
	}
	
	long lRetVal = DataBaseGetNextResultAsLong();
	DataBaseDestroyResults();
	
	return lRetVal;
}


/***************************************************************************//**
 @brief Delete a TOC item from the database
 @details Called when a layer is removed.
 @param itemid database ID of the item to remove
 @return  TRUE if item sucessfully removed, FALSE otherwise
 @author Lucien Schreiber (c) CREALP 2008
 @date 14 July 2008
 *******************************************************************************/
bool DataBaseTM::RemoveTOCLayer (const long & itemid)
{
	wxString sSentence = wxString::Format(_T("DELETE FROM ") + TABLE_NAME_TOC +
										  _T(" WHERE CONTENT_ID = %d"), itemid);
	if (!DataBaseQueryNoResult(sSentence))
	{
		wxLogDebug(_T("Not able to remove item from DB : %s"), sSentence.c_str());
		return FALSE;
	}
	
	return TRUE;
}



void DataBaseTM::PrepareTOCStatusUpdate(wxString & sentence, tmLayerProperties * item, 
										int itemRank,
										const wxString & symbology)
{
	sentence.Append(wxString::Format(_T("UPDATE ")+ TABLE_NAME_TOC +
									 _T(" SET CONTENT_STATUS = %d, RANK=%d, SYMBOLOGY=\"%s\",")
									 _T(" VERTEX_FLAGS = %d ")
									 _T("WHERE CONTENT_ID = %d; "),
									 item->m_LayerVisible,
									 itemRank,
									 symbology.c_str(),
									 item->m_DrawFlags,
									 item->m_LayerID));
	
}


/// FIELD CREATION ::

//_T("CREATE  TABLE     `LAYER_AT3` (")
//_T("  `LAYER_AT_ID` INT UNSIGNED NOT NULL AUTO_INCREMENT ,")
//_T("  `OBJECT_ID` INT UNSIGNED NOT NULL     ,")
//_T("  PRIMARY KEY (`LAYER_AT_ID`) ,")
//_T("  INDEX LAYER_ATX_FKIndex1 (`OBJECT_ID` ASC) ,")
//_T("  CONSTRAINT `Rel_09`")
//_T("    FOREIGN KEY (`OBJECT_ID` )")
//_T("    REFERENCES  `generic_points` (`OBJECT_ID` ));")
