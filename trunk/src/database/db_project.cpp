/***************************************************************************
								db_project.h.h
		deals with the creation and modification of a database project
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


#include "db_project.h"


DatabaseNewPrj::DatabaseNewPrj()
{
	InitMembersValues();
}


DatabaseNewPrj::DatabaseNewPrj(PrjDefMemManage * myPrjDefinition)
{
	InitMembersValues();
	SetPrjDefMemory(myPrjDefinition);
}


DatabaseNewPrj::~DatabaseNewPrj()
{

}


void DatabaseNewPrj::InitMembersValues()
{
	pPrjDefinition = NULL;
}

bool DatabaseNewPrj::IsPrjDefMemorySet()
{
	if (pPrjDefinition != NULL)
		return TRUE;
	return FALSE;
}


bool DatabaseNewPrj::CreateProject()      
{
	// check for PrjDefMemory is stored in memory 
	wxASSERT_MSG(IsPrjDefMemorySet(), _T("Use SetPrjDefMemory first..."));
	if (IsPrjDefMemorySet())
	{
		
		
		// show the path and name...
		wxLogDebug(_T("Path : %s \n Name : %s"),pPrjDefinition->m_PrjPath.c_str(), pPrjDefinition->m_PrjName.c_str());
		
		// create the database
		if (DataBaseCreateNew(pPrjDefinition->m_PrjPath, 
							  pPrjDefinition->m_PrjName))
		{
			wxLogMessage(_("Creating the \"%s\" database"), pPrjDefinition->m_PrjName.c_str());
			
			
			// create the default tables
			if (CreateDefaultTables())
			{
				wxLogDebug(_("Default tables created in the database"));
				return TRUE;
			}
		
		
		}
		wxLogDebug(_T("Error creating the database"));
		
		
	}
	return FALSE; // no project created....
}

bool DatabaseNewPrj::CreateDefaultTables ()
{
	wxString myNewPrjSentence = _T("  CREATE TABLE GENERIC_LABELS ("
								   "  OBJECT_ID INTEGER UNSIGNED NOT NULL AUTO_INCREMENT,"
								   "  OBJECT_GEOMETRY MULTIPOINT NOT NULL,"
								   "  PRIMARY KEY(OBJECT_ID)"
								   ");"
								   "CREATE TABLE GENERIC_FRAME ("
								   "  OBJECT_ID INTEGER UNSIGNED NOT NULL AUTO_INCREMENT,"
								   "  OBJECT_GEOMETRY POLYGON NOT NULL,"
								   "  PRIMARY KEY(OBJECT_ID)"
								   ");"
								   "CREATE TABLE GENERIC_POINTS ("
								   "  OBJECT_ID INTEGER UNSIGNED NOT NULL AUTO_INCREMENT,"
								   "  OBJECT_GEOMETRY MULTIPOINT NOT NULL,"
								   "  PRIMARY KEY(OBJECT_ID)"
								   ");"
								   "CREATE TABLE GENERIC_LINES ("
								   "  OBJECT_ID INTEGER UNSIGNED NOT NULL AUTO_INCREMENT,"
								   "  OBJECT_GEOMETRY LINESTRING NOT NULL,"
								   "  PRIMARY KEY(OBJECT_ID)"
								   ");"
								   "CREATE TABLE GENERIC_DMN ("
								   "  CODE INTEGER UNSIGNED NOT NULL AUTO_INCREMENT,"
								   "  DESCRIPTION VARCHAR(255) NULL,"
								   "  PRIMARY KEY(CODE)"
								   ");"
								   "CREATE TABLE DMN_LAYER_TYPE ("
								   "  TYPE_CD INTEGER UNSIGNED NOT NULL AUTO_INCREMENT,"
								   "  TYPE_DESCRIPTION VARCHAR(20) NOT NULL,"
								   "  PRIMARY KEY(TYPE_CD)"
								   ");"
								   "CREATE TABLE DMN_SHORTCUT_KEY ("
								   "  SHORTCUT_CD INTEGER UNSIGNED NOT NULL AUTO_INCREMENT,"
								   "  SHORTCUT_KEY VARCHAR(20) NOT NULL,"
								   "  PRIMARY KEY(SHORTCUT_CD)"
								   ");"
								   "CREATE TABLE ZOOM_LEVEL ("
								   "  ZOOM_ID INTEGER UNSIGNED NOT NULL AUTO_INCREMENT,"
								   "  SCALE_VALUE INTEGER UNSIGNED NOT NULL,"
								   "  ZOOM_TYPE ENUM('SYS','USR') NULL,"
								   "  PRIMARY KEY(ZOOM_ID)"
								   ");"
								   "CREATE TABLE LAYER_ATX ("
								   "  LAYER_AT_ID INTEGER UNSIGNED NOT NULL AUTO_INCREMENT,"
								   "  OBJECT_ID INTEGER UNSIGNED NOT NULL,"
								   "  PRIMARY KEY(LAYER_AT_ID),"
								   "  INDEX LAYER_ATX_FKIndex1(OBJECT_ID),"
								   "  FOREIGN KEY(OBJECT_ID)"
								   "    REFERENCES GENERIC_POINTS(OBJECT_ID)"
								   "      ON DELETE CASCADE"
								   "      ON UPDATE NO ACTION"
								   ");"
								   "CREATE TABLE THEMATIC_LAYERS ("
								   "  LAYER_INDEX INTEGER UNSIGNED NOT NULL AUTO_INCREMENT,"
								   "  TYPE_CD INTEGER UNSIGNED NOT NULL,"
								   "  LAYER_NAME VARCHAR(20) NOT NULL,"
								   "  LAYER_DESC VARCHAR(255) NULL,"
								   "  REMARK VARCHAR(255) NULL,"
								   "  PRIMARY KEY(LAYER_INDEX),"
								   "  INDEX THEMATIC_LAYERS_FKIndex1(TYPE_CD),"
								   "  FOREIGN KEY(TYPE_CD)"
								   "    REFERENCES DMN_LAYER_TYPE(TYPE_CD)"
								   "      ON DELETE NO ACTION"
								   "      ON UPDATE NO ACTION"
								   ");"
								   "CREATE TABLE PROJECT_TOC ("
								   "  CONTENT_ID INTEGER UNSIGNED NOT NULL AUTO_INCREMENT,"
								   "  TYPE_CD INTEGER UNSIGNED NOT NULL,"
								   "  CONTENT_PATH VARCHAR(45) NULL,"
								   "  CONTENT_NAME VARCHAR(45) NULL,"
								   "  CONTENT_STATUS ENUM('ON','OFF') NULL,"
								   "  PRIMARY KEY(CONTENT_ID),"
								   "  INDEX PROJECT_TOC_FKIndex1(TYPE_CD),"
								   "  FOREIGN KEY(TYPE_CD)"
								   "    REFERENCES DMN_LAYER_TYPE(TYPE_CD)"
								   "      ON DELETE NO ACTION"
								   "      ON UPDATE NO ACTION"
								   ");"
								   "CREATE TABLE GENERIC_LAYERS ("
								   "  LAYER_INDEX INTEGER UNSIGNED NOT NULL AUTO_INCREMENT,"
								   "  TYPE_CD INTEGER UNSIGNED NOT NULL,"
								   "  LAYER_NAME VARCHAR(45) NULL,"
								   "  LAYER_DESCRIPTION VARCHAR(100) NULL,"
								   "  PRIMARY KEY(LAYER_INDEX),"
								   "  INDEX GENERIC_LAYERS_FKIndex1(TYPE_CD),"
								   "  FOREIGN KEY(TYPE_CD)"
								   "    REFERENCES DMN_LAYER_TYPE(TYPE_CD)"
								   "      ON DELETE NO ACTION"
								   "      ON UPDATE NO ACTION"
								   ");"
								   "CREATE TABLE DMN_LAYER_OBJECT ("
								   "  OBJECT_CD INTEGER UNSIGNED NOT NULL,"
								   "  THEMATIC_LAYERS_LAYER_INDEX INTEGER UNSIGNED NOT NULL,"
								   "  OBJECT_DESC VARCHAR(100) NOT NULL,"
								   "  SYMBOL_CD VARCHAR(20) NULL,"
								   "  REMARK VARCHAR(100) NULL,"
								   "  PRIMARY KEY(OBJECT_CD, THEMATIC_LAYERS_LAYER_INDEX),"
								   "  INDEX DMN_LAYER_OBJECT_FKIndex2(THEMATIC_LAYERS_LAYER_INDEX),"
								   "  FOREIGN KEY(THEMATIC_LAYERS_LAYER_INDEX)"
								   "    REFERENCES THEMATIC_LAYERS(LAYER_INDEX)"
								   "      ON DELETE NO ACTION"
								   "      ON UPDATE NO ACTION"
								   ");"
								   "CREATE TABLE GENERIC_AAT ("
								   "  OBJECT_ID INTEGER UNSIGNED NOT NULL,"
								   "  THEMATIC_LAYERS_LAYER_INDEX INTEGER UNSIGNED NOT NULL,"
								   "  OBJECT_CD INTEGER UNSIGNED NOT NULL,"
								   "  PRIMARY KEY(OBJECT_ID, THEMATIC_LAYERS_LAYER_INDEX, OBJECT_CD),"
								   "  INDEX GENERIC_LINES_has_DMN_LAYER_OBJECT_FKIndex1(OBJECT_ID),"
								   "  INDEX GENERIC_LINES_has_DMN_LAYER_OBJECT_FKIndex2(OBJECT_CD, THEMATIC_LAYERS_LAYER_INDEX),"
								   "  FOREIGN KEY(OBJECT_ID)"
								   "    REFERENCES GENERIC_LINES(OBJECT_ID)"
								   "      ON DELETE NO ACTION"
								   "      ON UPDATE NO ACTION,"
								   "  FOREIGN KEY(OBJECT_CD, THEMATIC_LAYERS_LAYER_INDEX)"
								   "    REFERENCES DMN_LAYER_OBJECT(OBJECT_CD, THEMATIC_LAYERS_LAYER_INDEX)"
								   "      ON DELETE NO ACTION"
								   "      ON UPDATE NO ACTION"
								   ");"
								   "CREATE TABLE SHORTCUT_LIST ("
								   "  OBJECT_CD INTEGER UNSIGNED NOT NULL,"
								   "  THEMATIC_LAYERS_LAYER_INDEX INTEGER UNSIGNED NOT NULL,"
								   "  SHORTCUT_CD INTEGER UNSIGNED NOT NULL,"
								   "  PRIMARY KEY(OBJECT_CD, THEMATIC_LAYERS_LAYER_INDEX, SHORTCUT_CD),"
								   "  INDEX SHORTCUT_KEYS_has_DMN_LAYER_OBJECT_FKIndex2(OBJECT_CD, THEMATIC_LAYERS_LAYER_INDEX),"
								   "  INDEX SHORTCUT_LIST_FKIndex2(SHORTCUT_CD),"
								   "  FOREIGN KEY(SHORTCUT_CD)"
								   "    REFERENCES DMN_SHORTCUT_KEY(SHORTCUT_CD)"
								   "      ON DELETE NO ACTION"
								   "      ON UPDATE NO ACTION,"
								   "  FOREIGN KEY(OBJECT_CD, THEMATIC_LAYERS_LAYER_INDEX)"
								   "    REFERENCES DMN_LAYER_OBJECT(OBJECT_CD, THEMATIC_LAYERS_LAYER_INDEX)"
								   "      ON DELETE NO ACTION"
								   "      ON UPDATE NO ACTION"
								   ");"
								   "CREATE TABLE GENERIC_PAT ("
								   "  OBJECT_ID INTEGER UNSIGNED NOT NULL,"
								   "  THEMATIC_LAYERS_LAYER_INDEX INTEGER UNSIGNED NOT NULL,"
								   "  OBJECT_CD INTEGER UNSIGNED NOT NULL,"
								   "  PRIMARY KEY(OBJECT_ID, THEMATIC_LAYERS_LAYER_INDEX, OBJECT_CD),"
								   "  INDEX GENERIC_POINTS_has_DMN_LAYER_OBJECT_FKIndex1(OBJECT_ID),"
								   "  INDEX GENERIC_POINTS_has_DMN_LAYER_OBJECT_FKIndex2(OBJECT_CD, THEMATIC_LAYERS_LAYER_INDEX),"
								   "  FOREIGN KEY(OBJECT_ID)"
								   "    REFERENCES GENERIC_POINTS(OBJECT_ID)"
								   "      ON DELETE NO ACTION"
								   "      ON UPDATE NO ACTION,"
								   "  FOREIGN KEY(OBJECT_CD, THEMATIC_LAYERS_LAYER_INDEX)"
								   "    REFERENCES DMN_LAYER_OBJECT(OBJECT_CD, THEMATIC_LAYERS_LAYER_INDEX)"
								   "      ON DELETE NO ACTION"
								   "      ON UPDATE NO ACTION"
								   ");"
								   "CREATE TABLE GENERIC_LAT ("
								   "  OBJECT_ID INTEGER UNSIGNED NOT NULL,"
								   "  THEMATIC_LAYERS_LAYER_INDEX INTEGER UNSIGNED NOT NULL,"
								   "  OBJECT_CD INTEGER UNSIGNED NOT NULL,"
								   "  PRIMARY KEY(OBJECT_ID, THEMATIC_LAYERS_LAYER_INDEX, OBJECT_CD),"
								   "  INDEX GENERIC_LABELS_has_DMN_LAYER_OBJECT_FKIndex1(OBJECT_ID),"
								   "  INDEX GENERIC_LABELS_has_DMN_LAYER_OBJECT_FKIndex2(OBJECT_CD, THEMATIC_LAYERS_LAYER_INDEX),"
								   "  FOREIGN KEY(OBJECT_ID)"
								   "    REFERENCES GENERIC_LABELS(OBJECT_ID)"
								   "      ON DELETE NO ACTION"
								   "      ON UPDATE NO ACTION,"
								   "  FOREIGN KEY(OBJECT_CD, THEMATIC_LAYERS_LAYER_INDEX)"
								   "    REFERENCES DMN_LAYER_OBJECT(OBJECT_CD, THEMATIC_LAYERS_LAYER_INDEX)"
								   "      ON DELETE NO ACTION"
								   "      ON UPDATE NO ACTION"
								   ");");
	
	int iRequestResult = 0;
	

	wxArrayString myArray = DataBaseCutRequest(myNewPrjSentence);
	wxLogDebug(_T("Request array size is %d sentence(s)"), myArray.GetCount());
	
	for (uint i= 0; i<myArray.GetCount(); i++)
	{
		iRequestResult += DataBaseQueryMultiple(myArray.Item(i));
	}
	wxLogDebug(_T("Number of errors during process of array : %d"), iRequestResult);
		
	
	return TRUE;
	
}


