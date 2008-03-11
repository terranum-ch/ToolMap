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
	wxString myNewPrjSentence = _T("CREATE  TABLE     `DMN_LAYER_TYPE` (")
	_T("  `TYPE_CD` INT UNSIGNED NOT NULL,")
	_T("  `TYPE_DESCRIPTION` VARCHAR(20) NOT NULL     ,")
	_T("  PRIMARY KEY (`TYPE_CD`) ) ENGINE = InnoDB;")
	
	_T("CREATE  TABLE     `THEMATIC_LAYERS` (")
	_T("  `LAYER_INDEX` INT UNSIGNED NOT NULL AUTO_INCREMENT     ,")
	_T("  `TYPE_CD` INT UNSIGNED NOT NULL ,")
	_T("  `LAYER_NAME` VARCHAR(20) NOT NULL     ,")
	_T("  `LAYER_DESC` VARCHAR(255) NULL     ,")
	_T("  `REMARK` VARCHAR(255) NULL ,")
	_T("  PRIMARY KEY (`LAYER_INDEX`) ,")
	_T("  INDEX THEMATIC_LAYERS_FKIndex1 (`TYPE_CD` ASC) ,")
	_T("  CONSTRAINT `Rel_11`")
	_T("    FOREIGN KEY (`TYPE_CD` )")
	_T("    REFERENCES  `DMN_LAYER_TYPE` (`TYPE_CD` ));")
	
	_T("CREATE  TABLE     `DMN_LAYER_OBJECT` (")
	_T("  `OBJECT_ID` BIGINT NOT NULL AUTO_INCREMENT ,")
	_T("  `OBJECT_CD` INT UNSIGNED NOT NULL     ,")
	_T("  `THEMATIC_LAYERS_LAYER_INDEX` INT UNSIGNED NOT NULL     ,")
	_T("  `OBJECT_DESC` VARCHAR(100) NOT NULL     ,")
	_T("  `SYMBOL_CD` VARCHAR(20) NULL     ,")
	_T("  `REMARK` VARCHAR(100) NULL ,")
	_T("  PRIMARY KEY (`OBJECT_ID`) ,")
	_T("  INDEX DMN_LAYER_OBJECT_FKIndex2 (`THEMATIC_LAYERS_LAYER_INDEX` ASC, `OBJECT_ID` ASC) ,")
	_T("  CONSTRAINT `Contain`")
	_T("    FOREIGN KEY (`THEMATIC_LAYERS_LAYER_INDEX` )")
	_T("    REFERENCES  `THEMATIC_LAYERS` (`LAYER_INDEX` ) ")
	_T(" ON DELETE CASCADE")
    _T(" ON UPDATE RESTRICT);")
	
	_T("CREATE  TABLE     `GENERIC_POINTS` (")
	_T("  `OBJECT_ID` INT UNSIGNED NOT NULL AUTO_INCREMENT     ,")
	_T("  `OBJECT_GEOMETRY` MULTIPOINT NOT NULL     ,")
	_T("  PRIMARY KEY (`OBJECT_ID`) );")
	
	_T("CREATE  TABLE     `GENERIC_LABELS` (")
	_T("  `OBJECT_ID` INT UNSIGNED NOT NULL AUTO_INCREMENT ,")
	_T("  `OBJECT_GEOMETRY` MULTIPOINT NOT NULL ,")
	_T("  PRIMARY KEY (`OBJECT_ID`) );")
	
	_T("CREATE  TABLE     `GENERIC_LINES` (")
	_T("  `OBJECT_ID` INT UNSIGNED NOT NULL AUTO_INCREMENT ,")
	_T("  `OBJECT_GEOMETRY` LINESTRING NOT NULL ,")
	_T("  PRIMARY KEY (`OBJECT_ID`) );")
	
	_T("CREATE  TABLE     `GENERIC_PAT` (")
	_T("  `OBJECT_ID` INT UNSIGNED NOT NULL ,")
	_T("  `THEMATIC_LAYERS_LAYER_INDEX` INT UNSIGNED NOT NULL ,")
	_T("  `OBJECT_CD` INT UNSIGNED NOT NULL ,")
	_T("  PRIMARY KEY (`OBJECT_ID`, `THEMATIC_LAYERS_LAYER_INDEX`, `OBJECT_CD`) ,")
	_T("  INDEX GENERIC_POINTS_has_DMN_LAYER_OBJECT_FKIndex1 (`OBJECT_ID` ASC) ,")
	_T("  INDEX GENERIC_POINTS_has_DMN_LAYER_OBJECT_FKIndex2 (`OBJECT_CD` ASC, `THEMATIC_LAYERS_LAYER_INDEX` ASC) ,")
	_T("  CONSTRAINT `Rel_19`")
	_T("    FOREIGN KEY (`OBJECT_ID` )")
	_T("    REFERENCES  `GENERIC_POINTS` (`OBJECT_ID` ),")
	_T("  CONSTRAINT `Rel_20`")
	_T("    FOREIGN KEY (`OBJECT_CD` , `THEMATIC_LAYERS_LAYER_INDEX` )")
	_T("    REFERENCES  `DMN_LAYER_OBJECT` (`OBJECT_CD` , `THEMATIC_LAYERS_LAYER_INDEX` ));")
	
	_T("CREATE  TABLE     `GENERIC_LAT` (")
	_T("  `OBJECT_ID` INT UNSIGNED NOT NULL ,")
	_T("  `THEMATIC_LAYERS_LAYER_INDEX` INT UNSIGNED NOT NULL ,")
	_T("  `OBJECT_CD` INT UNSIGNED NOT NULL ,")
	_T("  PRIMARY KEY (`OBJECT_ID`, `THEMATIC_LAYERS_LAYER_INDEX`, `OBJECT_CD`) ,")
	_T("  INDEX GENERIC_LABELS_has_DMN_LAYER_OBJECT_FKIndex1 (`OBJECT_ID` ASC) ,")
	_T("  INDEX GENERIC_LABELS_has_DMN_LAYER_OBJECT_FKIndex2 (`OBJECT_CD` ASC, `THEMATIC_LAYERS_LAYER_INDEX` ASC) ,")
	_T("  CONSTRAINT `Rel_21`")
	_T("    FOREIGN KEY (`OBJECT_ID` )")
	_T("    REFERENCES  `GENERIC_LABELS` (`OBJECT_ID` ),")
	_T("  CONSTRAINT `Rel_22`")
	_T("    FOREIGN KEY (`OBJECT_CD` , `THEMATIC_LAYERS_LAYER_INDEX` )")
	_T("    REFERENCES  `DMN_LAYER_OBJECT` (`OBJECT_CD` , `THEMATIC_LAYERS_LAYER_INDEX` ));")
	
	_T("CREATE  TABLE     `GENERIC_AAT` (")
	_T("  `OBJECT_ID` INT UNSIGNED NOT NULL ,")
	_T("  `THEMATIC_LAYERS_LAYER_INDEX` INT UNSIGNED NOT NULL ,")
	_T("  `OBJECT_CD` INT UNSIGNED NOT NULL ,")
	_T("  PRIMARY KEY (`OBJECT_ID`, `THEMATIC_LAYERS_LAYER_INDEX`, `OBJECT_CD`) ,")
	_T("  INDEX GENERIC_LINES_has_DMN_LAYER_OBJECT_FKIndex1 (`OBJECT_ID` ASC) ,")
	_T("  INDEX GENERIC_LINES_has_DMN_LAYER_OBJECT_FKIndex2 (`OBJECT_CD` ASC, `THEMATIC_LAYERS_LAYER_INDEX` ASC) ,")
	_T("  CONSTRAINT `Rel_25`")
	_T("    FOREIGN KEY (`OBJECT_ID` )")
	_T("    REFERENCES  `GENERIC_LINES` (`OBJECT_ID` ),")
	_T("  CONSTRAINT `Rel_26`")
	_T("    FOREIGN KEY (`OBJECT_CD` , `THEMATIC_LAYERS_LAYER_INDEX` )")
	_T("    REFERENCES  `DMN_LAYER_OBJECT` (`OBJECT_CD` , `THEMATIC_LAYERS_LAYER_INDEX` ));")
	
	_T("CREATE  TABLE     `GENERIC_DMN` (")
	_T("  `CODE` INT UNSIGNED NOT NULL AUTO_INCREMENT     ,")
	_T("  `DESCRIPTION` VARCHAR(255) NULL     ,")
	_T("  PRIMARY KEY (`CODE`) );")
	
	_T("CREATE  TABLE     `PROJECT_TOC` (")
	_T("  `CONTENT_ID` INT UNSIGNED NOT NULL AUTO_INCREMENT ,")
	_T("  `TYPE_CD` INT UNSIGNED NOT NULL ,")
	_T("  `CONTENT_PATH` VARCHAR(45) NULL ,")
	_T("  `CONTENT_NAME` VARCHAR(45) NULL ,")
	_T("  `CONTENT_STATUS` ENUM('ON','OFF') NULL ,")
	_T("  PRIMARY KEY (`CONTENT_ID`) ,")
	_T("  INDEX PROJECT_TOC_FKIndex1 (`TYPE_CD` ASC) ,")
	_T("  CONSTRAINT `Rel_10`")
	_T("    FOREIGN KEY (`TYPE_CD` )")
	_T("    REFERENCES  `DMN_LAYER_TYPE` (`TYPE_CD` ));")
	
	_T("CREATE  TABLE     `GENERIC_LAYERS` (")
	_T("  `LAYER_INDEX` INT UNSIGNED NOT NULL AUTO_INCREMENT     ,")
	_T("  `TYPE_CD` INT UNSIGNED NOT NULL ,")
	_T("  `LAYER_NAME` VARCHAR(45) NULL ,")
	_T("  `LAYER_DESCRIPTION` VARCHAR(100) NULL ,")
	_T("  PRIMARY KEY (`LAYER_INDEX`) ,")
	_T("  INDEX GENERIC_LAYERS_FKIndex1 (`TYPE_CD` ASC) ,")
	_T("  CONSTRAINT `Rel_13`")
	_T("    FOREIGN KEY (`TYPE_CD` )")
	_T("    REFERENCES  `DMN_LAYER_TYPE` (`TYPE_CD` ));")
	
	_T("CREATE  TABLE     `DMN_SHORTCUT_KEY` (")
	_T("  `SHORTCUT_CD` INT UNSIGNED NOT NULL AUTO_INCREMENT ,")
	_T("  `SHORTCUT_KEY` VARCHAR(20) NOT NULL ,")
	_T("  PRIMARY KEY (`SHORTCUT_CD`) );")
	
	_T("CREATE  TABLE     `SHORTCUT_LIST` (")
	_T("  `OBJECT_CD` INT UNSIGNED NOT NULL ,")
	_T("  `THEMATIC_LAYERS_LAYER_INDEX` INT UNSIGNED NOT NULL ,")
	_T("  `SHORTCUT_CD` INT UNSIGNED NOT NULL ,")
	_T("  PRIMARY KEY (`OBJECT_CD`, `THEMATIC_LAYERS_LAYER_INDEX`, `SHORTCUT_CD`) ,")
	_T("  INDEX SHORTCUT_KEYS_has_DMN_LAYER_OBJECT_FKIndex2 (`OBJECT_CD` ASC, `THEMATIC_LAYERS_LAYER_INDEX` ASC) ,")
	_T("  INDEX SHORTCUT_LIST_FKIndex2 (`SHORTCUT_CD` ASC) ,")
	_T("  CONSTRAINT `Rel_14`")
	_T("    FOREIGN KEY (`SHORTCUT_CD` )")
	_T("    REFERENCES  `DMN_SHORTCUT_KEY` (`SHORTCUT_CD` ),")
	_T("  CONSTRAINT `Rel_15`")
	_T("    FOREIGN KEY (`OBJECT_CD` , `THEMATIC_LAYERS_LAYER_INDEX` )")
	_T("    REFERENCES  `DMN_LAYER_OBJECT` (`OBJECT_CD` , `THEMATIC_LAYERS_LAYER_INDEX` ));")
	
	_T("CREATE  TABLE     `GENERIC_FRAME` (")
	_T("  `OBJECT_ID` INT UNSIGNED NOT NULL AUTO_INCREMENT     ,")
	_T("  `OBJECT_GEOMETRY` POLYGON NOT NULL     ,")
	_T("  PRIMARY KEY (`OBJECT_ID`) );")
	
	_T("CREATE  TABLE     `ZOOM_LEVEL` (")
	_T("  `ZOOM_ID` INT UNSIGNED NOT NULL AUTO_INCREMENT ,")
	_T("  `SCALE_VALUE` INT UNSIGNED NOT NULL ,")
	_T("  `ZOOM_TYPE` ENUM('SYS','USR') NULL ,")
	_T("  PRIMARY KEY (`ZOOM_ID`) );")
	
	_T("CREATE  TABLE     `PRJ_SETTINGS` (")
	_T("  `SETTING_DBK` INT NOT NULL AUTO_INCREMENT ,")
	_T("  `PRJ_UNIT` VARCHAR(10) NOT NULL ,")
	_T("  `PRJ_PROJECTION` VARCHAR(45) NOT NULL ,")
	_T("  `PRJ_NAME` VARCHAR(45) NOT NULL ,")
	_T("  `PRJ_VERSION` SMALLINT UNSIGNED NOT NULL ,")
	_T("  PRIMARY KEY (`SETTING_DBK`) )");
	
	
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
	
	return TRUE;	
	
// CODE FOR PASSING MULTIPLE STATEMENT NOT WORKING (BUG) WITH VERSION 5.1.23-rc	
//	if (DataBaseQueryMultiple(myNewPrjSentence) == 0)
//		return TRUE;
//	
//	wxLogDebug(_T("Errors creating the database..."));
//	return FALSE;
	
}


/*************************** PROJECT DATABASE FUNCTION ****************************/
bool DataBaseTM::FillLayerTableTypeData ()
{
	wxString sSentence = _T("");
	int iReturnValue = 0;
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


bool DataBaseTM::SetProjectData (PrjDefMemManage * pPrjDefinition)
{
	// prepare data
	wxString sUnit = PRJDEF_UNIT_TYPE_STRING[pPrjDefinition->m_PrjUnitType];
	wxString sProj = PRJDEF_PROJ_TYPE_STRING[pPrjDefinition->m_PrjProjType];
	
	
	// check if the project line exist
	wxString sSentence = _T("SELECT * FROM ") + TABLE_NAME_PRJ_SETTINGS + _T(" WHERE SETTING_DBK =1");
	if (DataBaseQuery(sSentence))
	{
		// if ok the project settings exist and we 
		// must update it, otherwise insert
		if (DataBaseHasResult()==FALSE)
		{
			sSentence = wxString::Format(_T("INSERT INTO %s (PRJ_UNIT, PRJ_PROJECTION,") 
										 _T("PRJ_NAME, PRJ_VERSION) VALUES (\"%s\",\"%s\",\"%s\",%d)"),
										 TABLE_NAME_PRJ_SETTINGS.c_str(),
										 sUnit.c_str(),sProj.c_str(),
										 pPrjDefinition->m_PrjName.c_str(),TM_DATABASE_VERSION);
						
		}
		else
			sSentence = wxString::Format(_T("UPDATE %s SET ") 
										 _T("PRJ_UNIT = \"%s\", PRJ_PROJECTION = \"%s\", ")
										 _T("PRJ_NAME = \"%s\", PRJ_VERSION = %d"),
										 TABLE_NAME_PRJ_SETTINGS.c_str(),
										 sUnit.c_str(),sProj.c_str(),
										 pPrjDefinition->m_PrjName.c_str(),TM_DATABASE_VERSION);
		
		// processing request
		if (DataBaseQueryNoResult(sSentence))
			return TRUE;
	}
	wxLogDebug(_T("Error while modifing the project settings in the database"));
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



/*************************** OBJECT DATABASE FUNCTION ****************************/
bool DataBaseTM::AddObject (ProjectDefMemoryObjects * myObject, int DBlayerIndex)
{
	// get the selected layer of take the actual one
	if (DBlayerIndex == -1)
	{
		DBlayerIndex = GetActiveLayerId(); 
	}
	
	wxString sSentence = _T("INSERT INTO ") + TABLE_NAME_OBJECTS + _T(" (OBJECT_CD, THEMATIC_LAYERS_LAYER_INDEX, OBJECT_DESC) VALUES ");
	wxString sValues = wxString::Format(_T("(%d,%d,'%s')"),myObject->m_ObjectCode,
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
	_T("    REFERENCES  `GENERIC_POINTS` (`OBJECT_ID` ));");
	sCreateTable1.Append(sValues + sCreateTable2);
	
	
	if (DataBaseQueryNoResult(sCreateTable1))
	{
		//wxLogDebug(_T("Table Creation [%s]... DONE"), TableName.c_str());	
		return TRUE;
	}
	wxLogDebug(_T("Table Creation [%s]... FAILED"), TableName.c_str());
	return FALSE;
}


bool DataBaseTM::CreateFieldInteger (ProjectDefMemoryFields * myField, const wxString & TableName)
{
	wxString sSentence = wxString::Format(
										  _T("ALTER TABLE `%s` ADD COLUMN `%s` INT NULL"),
										  TableName.c_str(), myField->m_Fieldname.c_str());
	if (DataBaseQueryNoResult(sSentence))
	{
		return TRUE;
	}
	
	wxLogDebug(_T("Creating integer field : %s FAILED"), myField->m_Fieldname.c_str());
	return FALSE;
}



bool DataBaseTM::CreateFieldText (ProjectDefMemoryFields * myField, const wxString & TableName)
{
	wxString sSentence = wxString::Format(
										  _T("ALTER TABLE `%s` ADD COLUMN `%s` VARCHAR(%d) NULL"),
										  TableName.c_str(), 
										  myField->m_Fieldname.c_str(),
										  myField->m_FieldPrecision);
	if (DataBaseQueryNoResult(sSentence))
	{
		return TRUE;
	}
	
	wxLogDebug(_T("Creating text field : %s FAILED"), myField->m_Fieldname.c_str());
	return FALSE;	
}


bool DataBaseTM::CreateFieldDouble (ProjectDefMemoryFields * myField, const wxString & TableName)
{
	wxString sSentence = wxString::Format(
										  _T("ALTER TABLE `%s` ADD COLUMN `%s` DECIMAL(%d,%d) NULL"),
										  TableName.c_str(), 
										  myField->m_Fieldname.c_str(),
										  myField->m_FieldPrecision,
										  myField->m_FieldScale);
	if (DataBaseQueryNoResult(sSentence))
	{
		return TRUE;
	}
	
	wxLogDebug(_T("Creating double field : %s FAILED"), myField->m_Fieldname.c_str());
	return FALSE;	
}


bool DataBaseTM::CreateFieldDate (ProjectDefMemoryFields * myField, const wxString & TableName)
{
	wxString sSentence = wxString::Format(
										  _T("ALTER TABLE `%s` ADD COLUMN `%s` DATE NULL"),
										  TableName.c_str(), 
										  myField->m_Fieldname.c_str());
	if (DataBaseQueryNoResult(sSentence))
	{
		return TRUE;
	}
	wxLogDebug(_T("Creating date field : %s FAILED"), myField->m_Fieldname.c_str());
	return FALSE;	
}


bool DataBaseTM::AddFieldConstrain (ProjectDefMemoryFields * myField, const wxString & TableName)
{
	//bReturnValue = TRUE;
	
	// first get all coded values from the array and concanete them into
	// a wxString.
	wxString sValues = _T("");
	wxString sValueTemp = _T("");
	for (unsigned int i = 0; i< myField->m_pCodedValueArray->GetCount(); i++)
	{
		sValueTemp = myField->m_pCodedValueArray->Item(i).m_ValueName;
		sValues.Append(wxString::Format(_T("\"%s\","), sValueTemp.c_str()));
	}
	// remove last comma
	sValues = sValues.BeforeLast(',');
	
	
	if (sValues.IsEmpty() == FALSE)
	{
		
		wxString sSentence = wxString::Format(_T("ALTER TABLE `%s` MODIFY `%s` ENUM (%s)"),
											  TableName.c_str(),
											  myField->m_Fieldname.c_str(),
											  sValues.c_str());
		
		if (DataBaseQueryNoResult(sSentence))
		{
			return TRUE;
		}
	}
	wxLogDebug(_T("Unable to add constrain to field [%s]"), myField->m_Fieldname.c_str());
	return FALSE;
}


/*************************** FIELD DATABASE FUNCTION ****************************/
bool DataBaseTM::AddField (ProjectDefMemoryFields * myField, int DBlayerIndex)
{
	bool bReturnValue = TRUE;
	
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
		switch (myField->m_FieldType)
		{
			case TM_FIELD_INTEGER:
				bReturnValue = CreateFieldInteger(myField, sTableName);
				break;
			case TM_FIELD_FLOAT:
				bReturnValue = CreateFieldDouble(myField, sTableName);
				break;
			case  TM_FIELD_DATE:
				bReturnValue = CreateFieldDate(myField, sTableName);
				break;
			default:
				bReturnValue = CreateFieldText(myField, sTableName);
				break;
		}
		// add constrain on field if needed. 
		// actually only support enum contrain for text field.
		if (myField->m_FieldConstrain == TM_FIELD_CONSTRAIN_CODED)
		{
			bReturnValue = AddFieldConstrain(myField, sTableName);
		}

	}
	return bReturnValue;
}

/// FIELD CREATION ::

//_T("CREATE  TABLE     `LAYER_AT3` (")
//_T("  `LAYER_AT_ID` INT UNSIGNED NOT NULL AUTO_INCREMENT ,")
//_T("  `OBJECT_ID` INT UNSIGNED NOT NULL     ,")
//_T("  PRIMARY KEY (`LAYER_AT_ID`) ,")
//_T("  INDEX LAYER_ATX_FKIndex1 (`OBJECT_ID` ASC) ,")
//_T("  CONSTRAINT `Rel_09`")
//_T("    FOREIGN KEY (`OBJECT_ID` )")
//_T("    REFERENCES  `GENERIC_POINTS` (`OBJECT_ID` ));")
