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


/*************************   FIELDS CODED VALUES   *************************/
ProjectDefMemoryFieldsCodedVal::ProjectDefMemoryFieldsCodedVal()
{
	InitMemberValues();
}

ProjectDefMemoryFieldsCodedVal::~ProjectDefMemoryFieldsCodedVal()
{
	
}

void ProjectDefMemoryFieldsCodedVal::InitMemberValues()
{
	m_ValueID = 0;
	m_ValueCode = NULL_LONG_VALUE;
	m_ValueName = _T("");	
}

#include <wx/arrimpl.cpp>
WX_DEFINE_OBJARRAY (PrjMemFieldCodedValArray);

/*************************         FIELDS          *************************/
ProjectDefMemoryFields::ProjectDefMemoryFields()
{
	// set initial values to data members
	InitMemberValues();
}



ProjectDefMemoryFields::~ProjectDefMemoryFields()
{
	/// todo implement way of clearing the array contained inside
}




void ProjectDefMemoryFields::InitMemberValues()
{
	m_FieldID = 0;
	m_Fieldname = _T("");
	m_FieldType = TM_FIELD_TEXT;
	m_FieldPrecision = 0;
	m_FieldScale = 0;
	m_FieldOrientation = FALSE;
	m_FieldConstrain = TM_FIELD_NOT_CONSTRAIN;
	
	m_FieldRangeDefault = 0;
	m_FieldRangeMin = 0;
	m_FieldRangeMax = 0;
	
	m_pCodedValueArray = new PrjMemFieldCodedValArray();
	
	int m_CodedDefaultIndex = wxNOT_FOUND;
}


WX_DEFINE_OBJARRAY (PrjMemFieldArray);






/*************************         LAYERS          *************************/
ProjectDefMemoryLayers::ProjectDefMemoryLayers()
{
	InitMemberValues();
}

ProjectDefMemoryLayers::~ProjectDefMemoryLayers()
{
	/// todo implement way of clearing the array contained inside
}


void ProjectDefMemoryLayers::InitMemberValues()
{
	m_LayerID = 0;
	m_LayerType = LAYER_POINT;
	m_pLayerFieldArray = NULL;
	m_pLayerObjectArray = NULL;
	m_LayerName = _T("");
	
	m_pLayerFieldArray = new PrjMemFieldArray();
	m_pLayerObjectArray = new PrjMemObjectsArray();
}

WX_DEFINE_OBJARRAY (PrjMemLayersArray);




/*************************         OBJECTS          *************************/
ProjectDefMemoryObjects::ProjectDefMemoryObjects()
{
	InitMemberValues();
}

ProjectDefMemoryObjects::~ProjectDefMemoryObjects()
{
	
}

void ProjectDefMemoryObjects::InitMemberValues()
{
	m_ObjectID = 0;
	m_ObjectCode = NULL_LONG_VALUE;
	m_ObjectName = _T("");
	m_ObjectFreq = OBJECT_FREQUENT;
	m_ParentLayerName = _T("");
}

WX_DEFINE_OBJARRAY (PrjMemObjectsArray);

