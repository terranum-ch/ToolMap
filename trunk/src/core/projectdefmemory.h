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

#ifndef PROJECTDEFMEMORY_H
#define PROJECTDEFMEMORY_H

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "../gui/listgenreport.h"


// Definition for the project export type
/**
 @brief Type of export we want
 @todo Maybe this  must be defined elsewhere ?
 */
enum PRJDEF_EXPORT_TYPE
{
	EXPORT_SHAPEFILE = 0, ///<  shapefile is selected (default)
	EXPORT_GRAPHIC	///<  we specify export to graphical format
};

static wxString PRJDEF_EXPORT_TYPE_STRING[] =
{
	_("Shapefiles"),
	_("Graphics (EPS)")
};



// Definitions for the project unit type, we define the text and  the id of 
// each string. The number of items in the list is also defined here.
static wxString PRJDEF_UNIT_TYPE_STRING[] = 
{
	_("Meters"),
	_("Kilometers"),
	_("Lat / Long")
};
enum PRJDEF_UNIT_TYPE
{
	UNIT_METERS = 0,
	UNIT_KILOMETERS,
	UNIT_LATLONG
};
static const int PRJDEF_UNIT_TYPE_NUMBER = 3;


// Definitions for the Projection  type, we define the text and  the id of 
// each string. The number of items in the list is also defined here.
static wxString PRJDEF_PROJ_TYPE_STRING[] = 
{
	_("No projection"),
	_("Swiss projection")
};
enum PRJDEF_PROJ_TYPE
{
	PROJ_NOPROJ = 0,
	PROJ_SWISSPROJ
};
static const int PRJDEF_PROJ_TYPE_NUMBER = 2;


// Definitions for the field type, we define the text and  the id of 
// each string. The number of items in the list is also defined here.
static wxString PRJDEF_FIELD_TYPE_STRING[] = 
{
	_("Text"),
	_("Integer"),
	_("Float"),
	_("Date"),
	_("Enumeration")
};
enum PRJDEF_FIELD_TYPE
{	
	TM_FIELD_TEXT = 0,
	TM_FIELD_INTEGER,
	TM_FIELD_FLOAT,
	TM_FIELD_DATE,
	TM_FIELD_ENUMERATION
};
static const int PRJDEF_FIELD_TYPE_NUMBER = 5;

static wxString PRJDEF_FIELD_ORIENTATION_STRING[] =
{
	_("Yes"),
	_T("")
};

enum PRJDEF_FIELD_ORIENTATION
{
	TM_FIELD_ORIENT_YES = 0,
	TM_FIELD_ORIENT_NO
};

// Definition for the constrain value type
/*enum PRJDEF_FIELD_CONSTAIN_VALUE_TYPE
{
	TM_FIELD_CONSTRAIN_CODED = 0,
	TM_FIELD_CONSTRAIN_RANGE,
	TM_FIELD_NOT_CONSTRAIN
};*/


// Definitions for the layers type, we define the text and  the id of 
// each string. The number of items in the list is also defined here.
static wxString PRJDEF_LAYERS_TYPE_STRING[] = 
{	
	_("Line"),
	_("Point"),
	_("Polygon")
};
enum PRJDEF_LAYERS_TYPE
{
	LAYER_LINE = 0,
	LAYER_POINT,
	LAYER_POLYGON
};
static const int PRJDEF_LAYERS_TYPE_NUMBER = 3; // only takes the 3 first


// Definitions for the objects type, we define the text and  the id of 
// each string. The number of items in the list is also defined here.
static wxString PRJDEF_OBJECTS_FREQ_STRING[] = 
{
	_(""),
	_("Frequent")
};
enum PRJDEF_OBJECTS_FREQ
{
	OBJECT_LESS_FREQUENT = 0,
	OBJECT_FREQUENT
};
static const int PRJDEF_OBJECTSS_FREQ_NUMBER = 2;

static const long NULL_LONG_VALUE = -9999;



/***************************************************************************//**
 @brief Storing coded val linked to a field in memory
 @details This class is used for storing coded values allowed in a field. It is
 used inside a PrjMemFieldCodedValArray.
 @author Lucien Schreiber (c) CREALP 2007
 @date 11 January 2008
 *******************************************************************************/
class ProjectDefMemoryFieldsCodedVal
	{
	private:
		void InitMemberValues();
		
	public:
		long m_ValueID;
		long m_ValueCode;
		wxString m_ValueName;
		
		ProjectDefMemoryFieldsCodedVal();
		~ProjectDefMemoryFieldsCodedVal();
		
		static int ExtractCodedValuesFromString (const wxString & codedvalue, 
										  wxArrayString & results);
		
	};

// Creating a list of MemoryObjects
WX_DECLARE_OBJARRAY(ProjectDefMemoryFieldsCodedVal, PrjMemFieldCodedValArray);

/***************************************************************************//**
 @brief Storing object properties in memory
 @details This class is used for storing in memory the objects (in a
 PrjMemLayersArray)
 @author Lucien Schreiber (c) CREALP 2007
 @date 07 January 2008
 *******************************************************************************/
class ProjectDefMemoryObjects
	{
	private:
		
		
	public:
		long m_ObjectID;
		long m_ObjectCode;
		short m_ObjectType; // spatial type
		wxString m_ObjectName;
		PRJDEF_OBJECTS_FREQ m_ObjectFreq;
		
		wxString m_ParentLayerName;
		
		ProjectDefMemoryObjects();
		~ProjectDefMemoryObjects();
		
		void InitMemberValues();
		
	};

// Creating a list of MemoryObjects
WX_DECLARE_OBJARRAY(ProjectDefMemoryObjects, PrjMemObjectsArray);

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
		
		wxString ExtractValuesFromString(const wxString & fielddef);
		
		
	public:
		int m_FieldID;
		wxString m_Fieldname;
		wxString m_FieldOldName; // used for modifications
		PRJDEF_FIELD_TYPE m_FieldType;
		int m_FieldPrecision;
		int m_FieldScale;
		bool m_FieldOrientation;
		bool m_FieldConstrain;
				
		// for storing coded values
		PrjMemFieldCodedValArray * m_pCodedValueArray; 
		
		
		ProjectDefMemoryFields();
		~ProjectDefMemoryFields();
		
		bool SetValues(const wxArrayString & fielddef);
		bool GetStringTypeFromValues (wxString & sResult);
		static wxString GetOrientationName(){return _T("ORIENTATION");}
		
	
	};

// Creating a list of MemoryFields
WX_DECLARE_OBJARRAY(ProjectDefMemoryFields, PrjMemFieldArray);


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
		// for special case : polygons and default border value
		wxString m_LayerPolygonDefaultValue;
		
		/// list of fields related to a layer
		PrjMemFieldArray * m_pLayerFieldArray;
		/// list of objects related to a layer
		PrjMemObjectsArray * m_pLayerObjectArray;
		
		// for storing layers to delete
		wxArrayString m_StoreDeleteFields;
		
		ProjectDefMemoryLayers();
		~ProjectDefMemoryLayers();
	};

// Creating a list of MemoryLayers
WX_DECLARE_OBJARRAY(ProjectDefMemoryLayers, PrjMemLayersArray);



class ProjectDefMemoryScale
	{
	private:
		void InitMemberValues();
	public:
		long m_DBScaleID;
		long m_ScaleValue;
		
		ProjectDefMemoryScale();
		~ProjectDefMemoryScale();
	
	};

// Creating a list of MemoryLayers
WX_DECLARE_OBJARRAY(ProjectDefMemoryScale, PrjMemScaleArray);

#endif
