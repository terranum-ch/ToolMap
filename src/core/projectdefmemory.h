/***************************************************************************
								projectdefmemory.h
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

/***************************************************************************//**
 @file projectdefmemory.h
 @brief Storing project properties in memory
 @details During the process of creating a new project or editing an existing
 one, we have to keep all layers, objects, fields, in memory to allow an undo
 process.
 @author Lucien Schreiber (c) CREALP 2007
 @date 18 December 2007
 *******************************************************************************/
/*!
 \page Prj_def Project definition
 
 \section Introduction
 The project definition is stored in the embedded database but during the process
 of creating a new project or edition of an existing one, all will be temporally
 stored into the ProjectDefMemory class for allowing user to cancel any dialogs
 operation.
 
 \section ProjectDefMemory
 
 Hello
 
 
 
*/


#ifndef PROJECTDEFMEMORY_H
#define PROJECTDEFMEMORY_H

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "listgenreport.h"



// Definitions for the field type, we define the text and  the id of 
// each string. The number of items in the list is also defined here.
static wxString PRJDEF_FIELD_TYPE_STRING[] = 
{
	_("Integer"),
	_("Float"),
	_("Text"),
	_("Date")
};
enum PRJDEF_FIELD_TYPE
{
	FIELD_INTEGER = 0,
	FIELD_FLOAT,
	FIELD_TEXT,
	FIELD_DATE
};
static const int PRJDEF_FIELD_TYPE_NUMBER = 4;

// Definition for the constrain value type
enum PRJDEF_FIELD_CONSTAIN_VALUE_TYPE
{
	FIELD_CONSTRAIN_CODED = 0,
	FIELD_CONSTRAIN_RANGE,
	FIELD_NOT_CONSTRAIN
};


// Definitions for the layers type, we define the text and  the id of 
// each string. The number of items in the list is also defined here.
static wxString PRJDEF_LAYERS_TYPE_STRING[] = 
{
	_("Point"),
	_("Line"),
	_("Polygon")
};
enum PRJDEF_LAYERS_TYPE
{
	LAYER_POINT = 0,
	LAYER_LINE,
	LAYER_POLYGON
};
static const int PRJDEF_LAYERS_TYPE_NUMBER = 3;



/***************************************************************************//**
 @brief Storing Fields property in memory
 @details This class is used in PrjMemFieldArray, a wxArray type class for
 storing all fields related to a layer.
 @author Lucien Schreiber (c) CREALP 2007
 @date 18 December 2007
 *******************************************************************************/
class ProjectDefMemoryFields
	{
	private:
		void InitMemberValues();
		
	public:
		int m_FieldID;
		wxString m_Fieldname;
		PRJDEF_FIELD_TYPE m_FieldType;
		int m_FieldPrecision;
		int m_FieldScale;
		bool m_FieldOrientation;
		PRJDEF_FIELD_CONSTAIN_VALUE_TYPE m_FieldConstrain;
		
		ProjectDefMemoryFields();
		~ProjectDefMemoryFields();
		
	
	};

// Creating a list of MemoryFields
WX_DECLARE_OBJARRAY(ProjectDefMemoryFields, PrjMemFieldArray);

/// find function in field array
int FindObjInFieldArray(ListGenReport * myList, const PrjMemFieldArray & myArray);





/***************************************************************************//**
 @brief Storing layers properties in memory
 @details This class represent a layer. Objects of this class will be stored in
 a wxArray : PrjMemLayersArray.
 @author Lucien Schreiber (c) CREALP 2007
 @date 18 December 2007
 *******************************************************************************/
class ProjectDefMemoryLayers
	{
	private:
		void InitMemberValues();
		
	public:
		int m_LayerID;
		PRJDEF_LAYERS_TYPE m_LayerType;
		wxString m_LayerName;
		
		/// list of fields related to a layer
		PrjMemFieldArray * m_pLayerFieldArray;
		
		ProjectDefMemoryLayers();
		~ProjectDefMemoryLayers();
	};

// Creating a list of MemoryLayers
WX_DECLARE_OBJARRAY(ProjectDefMemoryLayers, PrjMemLayersArray);

/// find function in layer array
int FindObjInLayersArray(ListGenReport * myList, const PrjMemLayersArray & myArray);

#endif
