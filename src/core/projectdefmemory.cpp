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

/*************************         FIND FUNCTION IN ARRAY     *************************/
int FindObjInFieldArray(ListGenReport * myList, const PrjMemFieldArray & myArray)
{
	// get selected item from the list
	long mySelectedListItem = myList->GetSelectedItem();
	
	// search this item in the list
	for (unsigned int i=0; i < myArray.GetCount(); i++)
	{
		if (myArray.Item(i).m_Fieldname == myList->GetItemColText(mySelectedListItem, 0))
		{
			wxLogDebug(_T("Object found in Field array in position : %d"), i);
			return i;
		}
	}
	return -1;
}


int FindObjInLayersArray(ListGenReport * myList, PrjMemLayersArray * myArray)
{
	// get selected item from the list
	long mySelectedListItem = myList->GetSelectedItem();
	
	// search this item in the list
	for (unsigned int i=0; i < myArray->GetCount(); i++)
	{
		if (myArray->Item(i).m_LayerName == myList->GetItemColText(mySelectedListItem, 0))
		{
			wxLogDebug(_T("Object found in Layer array in position : %d"), i);
			return i;
		}
	}
	return -1;
	
}


int FindObjInObjectArray(ListGenReport * myList, PrjMemObjectsArray * myArray)
{
	// get selected item from the list
	long mySelectedListItem = myList->GetSelectedItem();
	
	// search this item in the list
	for (unsigned int i=0; i < myArray->GetCount(); i++)
	{
		
				
		if (myArray->Item(i).m_ObjectName == myList->GetItemColText(mySelectedListItem, 1))
		{
			wxLogDebug(_T("Object found in Layer array in position : %d"), i);
			return i;
		}
	}
	return -1;
	
}


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

#include <wx/arrimpl.cpp>
WX_DEFINE_OBJARRAY (PrjMemFieldArray);






/*************************         LAYERS          *************************/
ProjectDefMemoryLayers::ProjectDefMemoryLayers()
{
	InitMemberValues();
}

ProjectDefMemoryLayers::~ProjectDefMemoryLayers()
{
	
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
}

WX_DEFINE_OBJARRAY (PrjMemObjectsArray);

