/***************************************************************************
 projectdefmemory.h
 Manage all the project definition in memory before
 Creating the database (to allow cancel operations)
 -------------------
 copyright : (C) 2007 CREALP Lucien Schreiber
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/***************************************************************************/ /**
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
#include <wx/wxprec.h>

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
enum PRJDEF_EXPORT_TYPE {
    EXPORT_SHAPEFILE = 0,  ///<  shapefile is selected (default)
    EXPORT_GRAPHIC         ///<  we specify export to graphical format
};

static wxString PRJDEF_EXPORT_TYPE_STRING[] = {wxTRANSLATE("Shapefiles"), wxTRANSLATE("Graphics (EPS)")};

// Definitions for the project unit type, we define the text and  the id of
// each string. The number of items in the list is also defined here.
static wxString PRJDEF_UNIT_TYPE_STRING[] = {wxTRANSLATE("Meters"), wxTRANSLATE("Degrees (DD)"),
                                             wxTRANSLATE("Degrees (DMS)")};
enum PRJDEF_UNIT_TYPE {
    UNIT_METERS = 0,
    UNIT_DD,
    UNIT_DMS
};
static const int PRJDEF_UNIT_TYPE_NUMBER = 3;

const wxString TABLE_COMMENT_ENUMERATION = _T("ENUMERATION");

// Definitions for the Projection  type, we define the text and  the id of
// each string. The number of items in the list is also defined here.
static wxString PRJDEF_PROJ_TYPE_STRING[] = {
    wxTRANSLATE("Swiss projection (CH1903)"),
    wxTRANSLATE("Swiss projection (CH1903+ / LV95)"),
    wxTRANSLATE("World (WGS84)"),
};

static wxString PRJDEF_PROJ_TYPE_EPSG[] = {
    wxTRANSLATE("EPSG:21781"), //"Swiss projection (CH1903)"
    wxTRANSLATE("EPSG:2056"), // (CH1903+ / LV95)")
    wxTRANSLATE("EPSG:3857") //"World (WGS84)"),
};

enum PRJDEF_PROJ_TYPE {
    PROJ_SWISS_CH1903 = 0,
    PROJ_SWISS_CH1903PLUS,
    PROJ_WORLD_WGS84
};
static const int PRJDEF_PROJ_TYPE_NUMBER = 3;

// Definitions for the field type, we define the text and  the id of
// each string. The number of items in the list is also defined here.
static wxString PRJDEF_FIELD_TYPE_STRING[] = {wxTRANSLATE("Enumeration"), wxTRANSLATE("Text"), wxTRANSLATE("Integer"),
                                              wxTRANSLATE("Float"), wxTRANSLATE("Date")};
enum PRJDEF_FIELD_TYPE {
    TM_FIELD_ENUMERATION = 0,
    TM_FIELD_TEXT,
    TM_FIELD_INTEGER,
    TM_FIELD_FLOAT,
    TM_FIELD_DATE
};
static const int PRJDEF_FIELD_TYPE_NUMBER = 5;

static wxString PRJDEF_FIELD_ORIENTATION_STRING[] = {wxTRANSLATE("Yes"), wxTRANSLATE("")};

enum PRJDEF_FIELD_ORIENTATION {
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
static wxString PRJDEF_LAYERS_TYPE_STRING[] = {wxTRANSLATE("Line"), wxTRANSLATE("Point"), wxTRANSLATE("Polygon")};
enum PRJDEF_LAYERS_TYPE {
    LAYER_LINE = 0,
    LAYER_POINT,
    LAYER_POLYGON
};
static const int PRJDEF_LAYERS_TYPE_NUMBER = 3;  // only takes the 3 first

// Definitions for the objects type, we define the text and  the id of
// each string. The number of items in the list is also defined here.
static wxString PRJDEF_OBJECTS_FREQ_STRING[] = {_T(""), wxTRANSLATE("Frequent")};
enum PRJDEF_OBJECTS_FREQ {
    OBJECT_LESS_FREQUENT = 0,
    OBJECT_FREQUENT
};
static const int PRJDEF_OBJECTSS_FREQ_NUMBER = 2;

static const long NULL_LONG_VALUE = -9999;
static const wxString NULL_LONG_STRING_VALUE = _T("-9999");

/***************************************************************************/ /**
  @brief Storing coded val linked to a field in memory
  @details This class is used for storing coded values allowed in a field. It is
  used inside a PrjMemFieldCodedValArray.
  @author Lucien Schreiber (c) CREALP 2007
  @date 11 January 2008
  *******************************************************************************/
class ProjectDefMemoryFieldsCodedVal {
  private:
    void InitMemberValues();

  public:
    long m_ValueID;
    wxString m_ValueCode;
    wxString m_ValueName;

    ProjectDefMemoryFieldsCodedVal();

    ProjectDefMemoryFieldsCodedVal(const wxString& code, const wxString& desc);

    ~ProjectDefMemoryFieldsCodedVal();

    static int ExtractCodedValuesFromString(const wxString& codedvalue, wxArrayString& results);

    // copy
    ProjectDefMemoryFieldsCodedVal& operator=(const ProjectDefMemoryFieldsCodedVal& source);
};

// Creating a list of MemoryObjects
WX_DECLARE_OBJARRAY(ProjectDefMemoryFieldsCodedVal*, PrjMemFieldCodedValArray);

/***************************************************************************/ /**
  @brief Storing object properties in memory
  @details This class is used for storing in memory the objects (in a
  PrjMemLayersArray)
  @author Lucien Schreiber (c) CREALP 2007
  @date 07 January 2008
  *******************************************************************************/
class ProjectDefMemoryObjects {
  private:
  public:
    long m_ObjectID;
    wxString m_ObjectCode;
    short m_ObjectType;  // spatial type
    wxString m_ObjectName;
    PRJDEF_OBJECTS_FREQ m_ObjectFreq;

    wxString m_ParentLayerName;

    ProjectDefMemoryObjects();

    ~ProjectDefMemoryObjects();

    void InitMemberValues();

    ProjectDefMemoryObjects& operator=(const ProjectDefMemoryObjects& source);
};

// Creating a list of MemoryObjects
WX_DECLARE_OBJARRAY(ProjectDefMemoryObjects*, PrjMemObjectsArray);

/***************************************************************************/ /**
  @brief Storing Fields property in memory
  @details This class is used in PrjMemFieldArray, a wxArray type class for
  storing all fields related to a layer.
  @author Lucien Schreiber (c) CREALP 2007
  @date 18 December 2007
  *******************************************************************************/
class ProjectDefMemoryFields {
  private:
    void InitMemberValues();

    wxString ExtractValuesFromString(const wxString& fielddef);

  public:
    int m_FieldID;
    wxString m_Fieldname;
    wxString m_FieldOldName;  // used for modifications
    PRJDEF_FIELD_TYPE m_FieldType;
    int m_FieldPrecision;
    int m_FieldScale;
    bool m_FieldOrientation;
    bool m_FieldConstrain;

    // for storing layers to delete
    wxArrayLong m_StoreDeleteCodedValues;

    // for storing coded values
    PrjMemFieldCodedValArray m_pCodedValueArray;

    ProjectDefMemoryFields();

    ~ProjectDefMemoryFields();

    bool SetValues(const wxString& fieldname, const wxString& fielddef, const wxString& fieldcomment = _T(""));

    bool GetStringTypeFromValues(wxString& sResult);

    static wxString GetOrientationName() {
        return _T("ORIENTATION");
    }

    // copy function
    ProjectDefMemoryFields& operator=(const ProjectDefMemoryFields& source);
};

// Creating a list of MemoryFields
WX_DECLARE_OBJARRAY(ProjectDefMemoryFields*, PrjMemFieldArray);

/***************************************************************************/ /**
  @brief Storing layers properties in memory
  @details This class represent a layer. Objects of this class will be stored in
  a wxArray : PrjMemLayersArray.
  @author Lucien Schreiber (c) CREALP 2007
  @date 18 December 2007
  *******************************************************************************/
class ProjectDefMemoryLayers {
  private:
    void InitMemberValues();

  public:
    int m_LayerID;
    PRJDEF_LAYERS_TYPE m_LayerType;
    wxString m_LayerName;
    // for special case : polygons and default border value
    wxString m_LayerPolygonDefaultValue;

    /// list of fields related to a layer
    PrjMemFieldArray m_pLayerFieldArray;
    /// list of objects related to a layer
    PrjMemObjectsArray m_pLayerObjectArray;

    // for storing layers to delete
    wxArrayString m_StoreDeleteFields;

    ProjectDefMemoryLayers();

    ~ProjectDefMemoryLayers();

    // copy operator
    ProjectDefMemoryLayers& operator=(const ProjectDefMemoryLayers& source);
};

// Creating a list of MemoryLayers
WX_DECLARE_OBJARRAY(ProjectDefMemoryLayers*, PrjMemLayersArray);

class ProjectDefMemoryScale {
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
