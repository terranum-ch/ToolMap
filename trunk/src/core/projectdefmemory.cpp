/***************************************************************************
							projectdefmemory.cpp
				Manage all the project definition in memory before
				Creating the database (to allow cancel operations)
 
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


#include "projectdefmemory.h"

/*************************         FIELDS          *************************/
ProjectDefMemoryFields::ProjectDefMemoryFields()
{
	// set initial values to data members
	InitMemberValues();
}



ProjectDefMemoryFields::~ProjectDefMemoryFields()
{
	
}




void ProjectDefMemoryFields::InitMemberValues()
{
	m_FieldID = 0;
	m_Fieldname = _T("");
	m_FieldType = FIELD_TEXT;
	m_FieldPrecision = 0;
	m_FieldScale = 0;
	m_FieldOrientation = FALSE;
	m_FieldConstrain = FIELD_NOT_CONSTRAIN;
}


