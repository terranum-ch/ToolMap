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
	int iErrCode = 0;
	wxString myNewPrjSentence = _T("CREATE TABLE GENERIC_LABELS (")
	_T("  OBJECT_ID INTEGER UNSIGNED NOT NULL AUTO_INCREMENT,")
	_T("  OBJECT_GEOMETRY MULTIPOINT NOT NULL,")
	_T("  PRIMARY KEY(OBJECT_ID)")
	_T(");")
	_T("CREATE TABLE GENERIC_FRAME (")
	_T("  OBJECT_ID INTEGER UNSIGNED NOT NULL AUTO_INCREMENT,")
	_T("  OBJECT_GEOMETRY POLYGON NOT NULL,")
	_T("  PRIMARY KEY(OBJECT_ID)")
	_T(");")
	_T("CREATE TABLE GENERIC_POINTS (")
	_T("  OBJECT_ID INTEGER UNSIGNED NOT NULL AUTO_INCREMENT,")
	_T("  OBJECT_GEOMETRY MULTIPOINT NOT NULL,")
	_T("  PRIMARY KEY(OBJECT_ID)")
	_T(");")
	_T("CREATE TABLE GENERIC_LINES (")
	_T("  OBJECT_ID INTEGER UNSIGNED NOT NULL AUTO_INCREMENT,")
	_T("  OBJECT_GEOMETRY LINESTRING NOT NULL,")
	_T("  PRIMARY KEY(OBJECT_ID)")
	_T(");")
	_T("CREATE TABLE GENERIC_DMN (")
	_T("  CODE INTEGER UNSIGNED NOT NULL AUTO_INCREMENT,")
	_T("  DESCRIPTION VARCHAR(255) NULL,")
	_T("  PRIMARY KEY(CODE)")
	_T(");")
	_T("CREATE TABLE DMN_LAYER_TYPE (")
	_T("  TYPE_CD INTEGER UNSIGNED NOT NULL AUTO_INCREMENT,")
	_T("  TYPE_DESCRIPTION VARCHAR(20) NOT NULL,")
	_T("  PRIMARY KEY(TYPE_CD)")
	_T(");")
	_T("CREATE TABLE DMN_SHORTCUT_KEY (")
	_T("  SHORTCUT_CD INTEGER UNSIGNED NOT NULL AUTO_INCREMENT,")
	_T("  SHORTCUT_KEY VARCHAR(20) NOT NULL,")
	_T("  PRIMARY KEY(SHORTCUT_CD)")
	_T(");")
	_T("CREATE TABLE ZOOM_LEVEL (")
	_T("  ZOOM_ID INTEGER UNSIGNED NOT NULL AUTO_INCREMENT,")
	_T("  SCALE_VALUE INTEGER UNSIGNED NOT NULL,")
	_T("  ZOOM_TYPE ENUM('SYS','USR') NULL,")
	_T("  PRIMARY KEY(ZOOM_ID)")
	_T(");")
	_T("CREATE TABLE LAYER_ATX (")
	_T("  LAYER_AT_ID INTEGER UNSIGNED NOT NULL AUTO_INCREMENT,")
	_T("  OBJECT_ID INTEGER UNSIGNED NOT NULL,")
	_T("  PRIMARY KEY(LAYER_AT_ID),")
	_T("  INDEX LAYER_ATX_FKIndex1(OBJECT_ID),")
	_T("  FOREIGN KEY(OBJECT_ID)")
	_T("    REFERENCES GENERIC_POINTS(OBJECT_ID)")
	_T("      ON DELETE CASCADE")
	_T("      ON UPDATE NO ACTION")
	_T(");")
	_T("CREATE TABLE THEMATIC_LAYERS (")
	_T("  LAYER_INDEX INTEGER UNSIGNED NOT NULL AUTO_INCREMENT,")
	_T("  TYPE_CD INTEGER UNSIGNED NOT NULL,")
	_T("  LAYER_NAME VARCHAR(20) NOT NULL,")
	_T("  LAYER_DESC VARCHAR(255) NULL,")
	_T("  REMARK VARCHAR(255) NULL,")
	_T("  PRIMARY KEY(LAYER_INDEX),")
	_T("  INDEX THEMATIC_LAYERS_FKIndex1(TYPE_CD),")
	_T("  FOREIGN KEY(TYPE_CD)")
	_T("    REFERENCES DMN_LAYER_TYPE(TYPE_CD)")
	_T("      ON DELETE NO ACTION")
	_T("      ON UPDATE NO ACTION")
	_T(");")
	_T("CREATE TABLE PROJECT_TOC (")
	_T("  CONTENT_ID INTEGER UNSIGNED NOT NULL AUTO_INCREMENT,")
	_T("  TYPE_CD INTEGER UNSIGNED NOT NULL,")
	_T("  CONTENT_PATH VARCHAR(45) NULL,")
	_T("  CONTENT_NAME VARCHAR(45) NULL,")
	_T("  CONTENT_STATUS ENUM('ON','OFF') NULL,")
	_T("  PRIMARY KEY(CONTENT_ID),")
	_T("  INDEX PROJECT_TOC_FKIndex1(TYPE_CD),")
	_T("  FOREIGN KEY(TYPE_CD)")
	_T("    REFERENCES DMN_LAYER_TYPE(TYPE_CD)")
	_T("      ON DELETE NO ACTION")
	_T("      ON UPDATE NO ACTION")
	_T(");")
	_T("CREATE TABLE GENERIC_LAYERS (")
	_T("  LAYER_INDEX INTEGER UNSIGNED NOT NULL AUTO_INCREMENT,")
	_T("  TYPE_CD INTEGER UNSIGNED NOT NULL,")
	_T("  LAYER_NAME VARCHAR(45) NULL,")
	_T("  LAYER_DESCRIPTION VARCHAR(100) NULL,")
	_T("  PRIMARY KEY(LAYER_INDEX),")
	_T("  INDEX GENERIC_LAYERS_FKIndex1(TYPE_CD),")
	_T("  FOREIGN KEY(TYPE_CD)")
	_T("    REFERENCES DMN_LAYER_TYPE(TYPE_CD)")
	_T("      ON DELETE NO ACTION")
	_T("      ON UPDATE NO ACTION")
	_T(");")
	_T("CREATE TABLE DMN_LAYER_OBJECT (")
	_T("  OBJECT_CD INTEGER UNSIGNED NOT NULL,")
	_T("  THEMATIC_LAYERS_LAYER_INDEX INTEGER UNSIGNED NOT NULL,")
	_T("  OBJECT_DESC VARCHAR(100) NOT NULL,")
	_T("  SYMBOL_CD VARCHAR(20) NULL,")
	_T("  REMARK VARCHAR(100) NULL,")
	_T("  PRIMARY KEY(OBJECT_CD, THEMATIC_LAYERS_LAYER_INDEX),")
	_T("  INDEX DMN_LAYER_OBJECT_FKIndex2(THEMATIC_LAYERS_LAYER_INDEX),")
	_T("  FOREIGN KEY(THEMATIC_LAYERS_LAYER_INDEX)")
	_T("    REFERENCES THEMATIC_LAYERS(LAYER_INDEX)")
	_T("      ON DELETE NO ACTION")
	_T("      ON UPDATE NO ACTION")
	_T(");")
	_T("CREATE TABLE GENERIC_AAT (")
	_T("  OBJECT_ID INTEGER UNSIGNED NOT NULL,")
	_T("  THEMATIC_LAYERS_LAYER_INDEX INTEGER UNSIGNED NOT NULL,")
	_T("  OBJECT_CD INTEGER UNSIGNED NOT NULL,")
	_T("  PRIMARY KEY(OBJECT_ID, THEMATIC_LAYERS_LAYER_INDEX, OBJECT_CD),")
	_T("  INDEX GENERIC_LINES_has_DMN_LAYER_OBJECT_FKIndex1(OBJECT_ID),")
	_T("  INDEX GENERIC_LINES_has_DMN_LAYER_OBJECT_FKIndex2(OBJECT_CD, THEMATIC_LAYERS_LAYER_INDEX),")
	_T("  FOREIGN KEY(OBJECT_ID)")
	_T("    REFERENCES GENERIC_LINES(OBJECT_ID)")
	_T("      ON DELETE NO ACTION")
	_T("      ON UPDATE NO ACTION,")
	_T("  FOREIGN KEY(OBJECT_CD, THEMATIC_LAYERS_LAYER_INDEX)")
	_T("    REFERENCES DMN_LAYER_OBJECT(OBJECT_CD, THEMATIC_LAYERS_LAYER_INDEX)")
	_T("      ON DELETE NO ACTION")
	_T("      ON UPDATE NO ACTION")
	_T(");")
	_T("CREATE TABLE SHORTCUT_LIST (")
	_T("  OBJECT_CD INTEGER UNSIGNED NOT NULL,")
	_T("  THEMATIC_LAYERS_LAYER_INDEX INTEGER UNSIGNED NOT NULL,")
	_T("  SHORTCUT_CD INTEGER UNSIGNED NOT NULL,")
	_T("  PRIMARY KEY(OBJECT_CD, THEMATIC_LAYERS_LAYER_INDEX, SHORTCUT_CD),")
	_T("  INDEX SHORTCUT_KEYS_has_DMN_LAYER_OBJECT_FKIndex2(OBJECT_CD, THEMATIC_LAYERS_LAYER_INDEX),")
	_T("  INDEX SHORTCUT_LIST_FKIndex2(SHORTCUT_CD),")
	_T("  FOREIGN KEY(SHORTCUT_CD)")
	_T("    REFERENCES DMN_SHORTCUT_KEY(SHORTCUT_CD)")
	_T("      ON DELETE NO ACTION")
	_T("      ON UPDATE NO ACTION,")
	_T("  FOREIGN KEY(OBJECT_CD, THEMATIC_LAYERS_LAYER_INDEX)")
	_T("    REFERENCES DMN_LAYER_OBJECT(OBJECT_CD, THEMATIC_LAYERS_LAYER_INDEX)")
	_T("      ON DELETE NO ACTION")
	_T("      ON UPDATE NO ACTION")
	_T(");")
	_T("CREATE TABLE GENERIC_PAT (")
	_T("  OBJECT_ID INTEGER UNSIGNED NOT NULL,")
	_T("  THEMATIC_LAYERS_LAYER_INDEX INTEGER UNSIGNED NOT NULL,")
	_T("  OBJECT_CD INTEGER UNSIGNED NOT NULL,")
	_T("  PRIMARY KEY(OBJECT_ID, THEMATIC_LAYERS_LAYER_INDEX, OBJECT_CD),")
	_T("  INDEX GENERIC_POINTS_has_DMN_LAYER_OBJECT_FKIndex1(OBJECT_ID),")
	_T("  INDEX GENERIC_POINTS_has_DMN_LAYER_OBJECT_FKIndex2(OBJECT_CD, THEMATIC_LAYERS_LAYER_INDEX),")
	_T("  FOREIGN KEY(OBJECT_ID)")
	_T("    REFERENCES GENERIC_POINTS(OBJECT_ID)")
	_T("      ON DELETE NO ACTION")
	_T("      ON UPDATE NO ACTION,")
	_T("  FOREIGN KEY(OBJECT_CD, THEMATIC_LAYERS_LAYER_INDEX)")
	_T("    REFERENCES DMN_LAYER_OBJECT(OBJECT_CD, THEMATIC_LAYERS_LAYER_INDEX)")
	_T("      ON DELETE NO ACTION")
	_T("      ON UPDATE NO ACTION")
	_T(");")
	_T("CREATE TABLE GENERIC_LAT (")
	_T("  OBJECT_ID INTEGER UNSIGNED NOT NULL,")
	_T("  THEMATIC_LAYERS_LAYER_INDEX INTEGER UNSIGNED NOT NULL,")
	_T("  OBJECT_CD INTEGER UNSIGNED NOT NULL,")
	_T("  PRIMARY KEY(OBJECT_ID, THEMATIC_LAYERS_LAYER_INDEX, OBJECT_CD),")
	_T("  INDEX GENERIC_LABELS_has_DMN_LAYER_OBJECT_FKIndex1(OBJECT_ID),")
	_T("  INDEX GENERIC_LABELS_has_DMN_LAYER_OBJECT_FKIndex2(OBJECT_CD, THEMATIC_LAYERS_LAYER_INDEX),")
	_T("  FOREIGN KEY(OBJECT_ID)")
	_T("    REFERENCES GENERIC_LABELS(OBJECT_ID)")
	_T("      ON DELETE NO ACTION")
	_T("      ON UPDATE NO ACTION,")
	_T("  FOREIGN KEY(OBJECT_CD, THEMATIC_LAYERS_LAYER_INDEX)")
	_T("    REFERENCES DMN_LAYER_OBJECT(OBJECT_CD, THEMATIC_LAYERS_LAYER_INDEX)")
	_T("      ON DELETE NO ACTION")
	_T("      ON UPDATE NO ACTION")
	_T(")");

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

//bool DatabaseNewPrj::SetProjectName()
//{
//	int iErrCode = 0;
//	return TRUE;
//}


