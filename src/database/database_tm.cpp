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

bool DataBaseTM::CreateEmptyTMDatabase()
{
	int iErrCode = 0;
	wxString myNewPrjSentence = _T("CREATE  TABLE     `DMN_LAYER_TYPE` (")
	_T("  `TYPE_CD` INT UNSIGNED NOT NULL AUTO_INCREMENT     ,")
	_T("  `TYPE_DESCRIPTION` VARCHAR(20) NOT NULL     ,")
	_T("  PRIMARY KEY (`TYPE_CD`) );")
	
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
	_T("  `PRJ_VERSION` VARCHAR(45) NOT NULL ,")
	_T("  PRIMARY KEY (`SETTING_DBK`) )");
	
	
	wxArrayString myArray = DataBaseCutRequest(myNewPrjSentence);
	wxLogDebug(_T("Request array size is %d sentence(s)"), myArray.GetCount());
	
	for (unsigned int i= 0; i<myArray.GetCount(); i++)
	{
		iErrCode += DataBaseQueryMultiple(myArray.Item(i));
	}
	wxLogDebug(_T("Number of errors during process of array : %d"), iErrCode);
	if (iErrCode != 0)
		return FALSE;
	
	return TRUE;	
}



/*************************** LAYER DATABASE FUNCTION ****************************/
bool DataBaseTM::AddLayer(ProjectDefMemoryLayers * myLayer)
{
	wxString sSentence = _T("INSERT INTO ") + TABLE_NAME_LAYERS + _T(" (TYPE_CD, LAYER_NAME) VALUES ");
	wxString sValues = wxString::Format(_T("(%d,'%s')"), 
		myLayer->m_LayerType, 
		myLayer->m_LayerName.c_str());
	sSentence.Append(sValues);
	
	wxLogDebug(sSentence);
	
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
	
	wxLogDebug(sSentence);
	
	if (DataBaseQuery(sSentence) == 0) // query OK
	{
		m_iDBLayerIndex = DataBaseGetResultAsInt();
		wxLogDebug(_T("Actual database index is %d"), m_iDBLayerIndex);
	}
	
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
	
	wxLogDebug(sSentence);
	
	if (DataBaseQueryMultiple(sSentence) == 0)
	{
		return TRUE;
	}
	return FALSE;
}



/*************************** FIELD DATABASE FUNCTION [ PRIVATE ] *******************/
bool DataBaseTM::AddTableIfNotExist (const int & iLayerIndex)
{
	wxString sCreateTable1 = _T("CREATE  TABLE IF NOT EXISTS `");
	wxString sValues = wxString::Format(TABLE_NAME_LAYER_AT + _T("%d` ("), iLayerIndex);
	wxString sCreateTable2 = _T("  `LAYER_AT_ID` INT UNSIGNED NOT NULL AUTO_INCREMENT ,")
	_T("  `OBJECT_ID` INT UNSIGNED NOT NULL     ,")
	_T("  PRIMARY KEY (`LAYER_AT_ID`) ,")
	_T("  INDEX LAYER_ATX_FKIndex1 (`OBJECT_ID` ASC) ,")
	_T("  CONSTRAINT `Rel_09`")
	_T("    FOREIGN KEY (`OBJECT_ID` )")
	_T("    REFERENCES  `GENERIC_POINTS` (`OBJECT_ID` ));");
	sCreateTable1.Append(sValues + sCreateTable2);
	
	
	if (DataBaseQueryMultiple(sCreateTable1) == 0)
	{
		wxLogDebug(_T("Table Creation for layer [%i]... DONE"), iLayerIndex);	
		return TRUE;
	}
	wxLogDebug(_T("Table Creation for layer [%i]... FAILED"), iLayerIndex);
	return FALSE;
}


bool DataBaseTM::CreateFieldInteger (ProjectDefMemoryFields * myField, wxString TableName)
{
	wxString sSentence = wxString::Format(
										  _T("ALTER TABLE `%s` ADD COLUMN `%s` INT NULL"),
										  TableName.c_str(), myField->m_Fieldname.c_str());
	if (DataBaseQueryMultiple(sSentence)==0)
	{
		wxLogDebug(_T("Creating integer field : %s DONE"), myField->m_Fieldname.c_str());
		return TRUE;
	}
	
	return FALSE;
}



bool DataBaseTM::CreateFieldText (ProjectDefMemoryFields * myField, wxString TableName)
{
	wxString sSentence = wxString::Format(
										  _T("ALTER TABLE `%s` ADD COLUMN `%s` VARCHAR(%d) NULL"),
										  TableName.c_str(), 
										  myField->m_Fieldname.c_str(),
										  myField->m_FieldPrecision);
	if (DataBaseQueryMultiple(sSentence)==0)
	{
		wxLogDebug(_T("Creating text field : %s DONE"), myField->m_Fieldname.c_str());
		return TRUE;
	}
	
	return FALSE;	
}


bool DataBaseTM::CreateFieldDouble (ProjectDefMemoryFields * myField, wxString TableName)
{
	wxString sSentence = wxString::Format(
										  _T("ALTER TABLE `%s` ADD COLUMN `%s` DECIMAL(%d,%d) NULL"),
										  TableName.c_str(), 
										  myField->m_Fieldname.c_str(),
										  myField->m_FieldPrecision,
										  myField->m_FieldScale);
	if (DataBaseQueryMultiple(sSentence)==0)
	{
		wxLogDebug(_T("Creating double field : %s DONE"), myField->m_Fieldname.c_str());
		return TRUE;
	}
	
	return FALSE;	
}


bool DataBaseTM::CreateFieldDate (ProjectDefMemoryFields * myField, wxString TableName)
{
	wxString sSentence = wxString::Format(
										  _T("ALTER TABLE `%s` ADD COLUMN `%s` DATE NULL"),
										  TableName.c_str(), 
										  myField->m_Fieldname.c_str());
	if (DataBaseQueryMultiple(sSentence)==0)
	{
		wxLogDebug(_T("Creating date field : %s DONE"), myField->m_Fieldname.c_str());
		return TRUE;
	}
	
	return FALSE;	
}




/*************************** FIELD DATABASE FUNCTION ****************************/
bool DataBaseTM::AddField (ProjectDefMemoryFields * myField, int DBlayerIndex)
{
	// get the selected layer of take the actual one
	if (DBlayerIndex == -1)
	{
		DBlayerIndex = GetActiveLayerId(); 
	}
	
	// first we must create the table if not exist
	bool bCreateTable = AddTableIfNotExist(DBlayerIndex);
	
	wxString sTableName = wxString::Format(TABLE_NAME_LAYER_AT + _("%d"), DBlayerIndex);
	
	// check that the table exists.
	if (DataBaseTableExist(sTableName))
	{
		// then create the fields based upon the fields type
		switch (myField->m_FieldType)
		{
			case TM_FIELD_INTEGER:
				CreateFieldInteger(myField, sTableName);
				break;
			case TM_FIELD_FLOAT:
				CreateFieldDouble(myField, sTableName);
				break;
			case  TM_FIELD_DATE:
				CreateFieldDate(myField, sTableName);
				break;
			default:
				CreateFieldText(myField, sTableName);
				break;
		}
		
		return TRUE;	
	}
	return FALSE;
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
