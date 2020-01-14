/***************************************************************************
 projectdefmemory.cpp
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

#include "projectdefmemory.h"

/*************************   FIELDS CODED VALUES   *************************/
ProjectDefMemoryFieldsCodedVal::ProjectDefMemoryFieldsCodedVal() {
  InitMemberValues();
}

ProjectDefMemoryFieldsCodedVal::ProjectDefMemoryFieldsCodedVal(const wxString &code, const wxString &desc) {
  InitMemberValues();
  m_ValueCode = code;
  m_ValueName = desc;
}

ProjectDefMemoryFieldsCodedVal::~ProjectDefMemoryFieldsCodedVal() {}

void ProjectDefMemoryFieldsCodedVal::InitMemberValues() {
  m_ValueID = wxNOT_FOUND;
  m_ValueCode = NULL_LONG_STRING_VALUE;
  m_ValueName = _T("");
}

/***************************************************************************/ /**
  @brief Tokenize coded values
  @details Convert from a string containing concateneted coded values to a
  wxArrayString
  @param codedvalue the original string with all values concateneted
  @param results a wxArrayString containing all extracted coded values
  @return  the number of coded values found
  @author Lucien Schreiber (c) CREALP 2007
  @date 17 April 2008
  *******************************************************************************/
int ProjectDefMemoryFieldsCodedVal::ExtractCodedValuesFromString(const wxString &codedvalue, wxArrayString &results) {
  results.Clear();

  wxString myTempText = codedvalue.Mid(1, codedvalue.Len() - 2);
  myTempText.Replace(_T("','"), _T("\t"));

  wxStringTokenizer tkz(myTempText, wxT("\t"));
  while (tkz.HasMoreTokens()) {
    wxString token = tkz.GetNextToken();
    // Bug #260 Apostrophes
    // Strange Behavior in MySQL Enum
    // single quotes are returned double!
    token.Replace(_T("''"), _T("'"));

    results.Add(token);
  }

  return results.GetCount();
}

/***************************************************************************/ /**
  @brief Copy fields coded values
  @return  always true
  @author Lucien Schreiber (c) CREALP 2009
  @date 17 March 2009
  *******************************************************************************/
ProjectDefMemoryFieldsCodedVal &ProjectDefMemoryFieldsCodedVal::operator=(
    const ProjectDefMemoryFieldsCodedVal &source) {
  m_ValueID = source.m_ValueID;
  m_ValueCode = source.m_ValueCode;
  m_ValueName = source.m_ValueName;
  return *this;
}

#include <wx/arrimpl.cpp>
WX_DEFINE_OBJARRAY(PrjMemFieldCodedValArray);

/*************************         FIELDS          *************************/
ProjectDefMemoryFields::ProjectDefMemoryFields() {
  // set initial values to data members
  InitMemberValues();
}

ProjectDefMemoryFields::~ProjectDefMemoryFields() {
  unsigned int iCount = m_pCodedValueArray.GetCount();
  for (unsigned int i = 0; i < iCount; i++) {
    ProjectDefMemoryFieldsCodedVal *myVal = m_pCodedValueArray.Item(0);
    wxDELETE(myVal);
    m_pCodedValueArray.RemoveAt(0);
  }
  wxASSERT(m_pCodedValueArray.GetCount() == 0);
}

void ProjectDefMemoryFields::InitMemberValues() {
  m_FieldID = wxNOT_FOUND;
  m_Fieldname = _T("");
  m_FieldOldName = _T("");
  m_FieldType = TM_FIELD_ENUMERATION;
  m_FieldPrecision = 0;
  m_FieldScale = 0;
  m_FieldOrientation = FALSE;
  m_FieldConstrain = FALSE;
}

/***************************************************************************/ /**
  @brief Copy Field values (and array of coded values)
  @return  always true
  @author Lucien Schreiber (c) CREALP 2009
  @date 17 March 2009
  *******************************************************************************/
ProjectDefMemoryFields &ProjectDefMemoryFields::operator=(const ProjectDefMemoryFields &source) {
  m_FieldID = source.m_FieldID;
  m_Fieldname = source.m_Fieldname;
  m_FieldOldName = source.m_FieldOldName;
  m_FieldType = source.m_FieldType;
  m_FieldPrecision = source.m_FieldPrecision;
  m_FieldScale = source.m_FieldScale;
  m_FieldOrientation = source.m_FieldOrientation;
  m_FieldConstrain = source.m_FieldConstrain;

  // copy enum values if existing
  for (unsigned int j = 0; j < m_pCodedValueArray.GetCount(); j++) {
    ProjectDefMemoryFieldsCodedVal *myVal = m_pCodedValueArray.Item(0);
    wxDELETE(myVal);
    m_pCodedValueArray.RemoveAt(0);
  }

  for (unsigned int i = 0; i < source.m_pCodedValueArray.GetCount(); i++) {
    ProjectDefMemoryFieldsCodedVal *CVal = source.m_pCodedValueArray.Item(i);
    ProjectDefMemoryFieldsCodedVal *myVal = new ProjectDefMemoryFieldsCodedVal();
    *myVal = *CVal;
    m_pCodedValueArray.Add(myVal);
  }

  return *this;
}

/***************************************************************************/ /**
  @brief Extract value(s) from string
  @details This may be used for getting a string containing only values
  indicating either field precision (for integer, float), field length (text) or
  values in case of enum.
  For exemple :
  @verbatim
  fielddef = "enum("hello","world")
  returned value = "hello","world"

  fielddef = "decimal(10,2)"
  returned value = 10,2
  @endverbatim
  @param fielddef a string containing the value to parse
  @return  the parsed string without field type and parenthesis
  @author Lucien Schreiber (c) CREALP 2007
  @date 17 April 2008
  *******************************************************************************/
wxString ProjectDefMemoryFields::ExtractValuesFromString(const wxString &fielddef) {
  wxString myTempValue = fielddef.AfterFirst('(');
  return myTempValue.RemoveLast(1);
}

/***************************************************************************/ /**
  @brief Assign values based on string
  @details This function may be used for assigning values to a
  #ProjectDefMemoryFields object from a string (for applications see string
  extracted from some SQL query like 'SHOW FULL COLUMNS FROM ...'
  @param fielddef an Array of String containing minimum 2 items (verified
  internally), the first item should be the field name and the second is a
  string with the width of the field or the values in case of enumerations
  @return  TRUE if values were assigned
  @author Lucien Schreiber (c) CREALP 2007
  @date 17 April 2008
  *******************************************************************************/
bool ProjectDefMemoryFields::SetValues(const wxString &fieldname, const wxString &fielddef,
                                       const wxString &fieldcomment) {
  wxString myFieldType = wxEmptyString;
  wxString myValues = wxEmptyString;
  wxString myTempConvert = wxEmptyString;
  wxArrayString myCodedValResults;
  int iCountCodedVal = 0;

  // assign a field id greather than 0
  m_FieldID = 1;

  // get the field name
  m_Fieldname = fieldname;
  m_FieldOldName = fieldname;

  // get the field type
  myFieldType = fielddef.BeforeFirst('(');
  if (!myFieldType.IsEmpty()) {
    // ENUMERATION //
    if (myFieldType == _T("enum")) {
      m_FieldType = TM_FIELD_ENUMERATION;

      // compute the enumerations values
      myTempConvert = ExtractValuesFromString(fielddef);
      iCountCodedVal = ProjectDefMemoryFieldsCodedVal::ExtractCodedValuesFromString(myTempConvert, myCodedValResults);

      for (int i = 0; i < iCountCodedVal; i++) {
        ProjectDefMemoryFieldsCodedVal *myVal = new ProjectDefMemoryFieldsCodedVal();
        myVal->m_ValueCode = wxString::Format(_T("%d"), i + 1);
        myVal->m_ValueName = myCodedValResults.Item(i);
        m_pCodedValueArray.Add(myVal);
      }
    }

    // INTEGER //
    else if (myFieldType == _T("int")) {
      m_FieldType = TM_FIELD_INTEGER;
      // compute nothing... :-) we know
      // that integer is 10 decimals
      // storing value up to 2147483648
    }

    // FLOAT //
    else if (myFieldType == _T("decimal")) {
      m_FieldType = TM_FIELD_FLOAT;
      // compute precision and scale
      myTempConvert = ExtractValuesFromString(fielddef);
      m_FieldPrecision = wxAtoi(myTempConvert.BeforeFirst(','));
      m_FieldScale = wxAtoi(myTempConvert.AfterFirst(','));
    }

    // DATE //
    else if (myFieldType == _T("date")) {
      m_FieldType = TM_FIELD_DATE;
    }

    // TEXT //
    else if (myFieldType == _T("varchar")) {
      m_FieldType = TM_FIELD_TEXT;
      // compute size (precision)
      m_FieldPrecision = wxAtoi(ExtractValuesFromString(fielddef));
    }
  }

  // get orientation (item number 8)
  if (fieldcomment == ProjectDefMemoryFields::GetOrientationName()) {
    m_FieldOrientation = true;
  }
  // otherwise, field orientation is false :-)

  return TRUE;
}

/***************************************************************************/ /**
  @brief Create string type
  @details This function create string type :
  - DATE
  - ENUM(item1,item2,...)
  - VARCHAR(Length)
  - DECIMAL(Precision, Scale)
  - INT
  from the stored field type ProjectDefMemoryFields::m_FieldType
  @param sResult Will be modified to contain the string type who could be used
  for adding or modifing columns
  @return  Always true for the moment
  @author Lucien Schreiber (c) CREALP 2007
  @date 24 April 2008
  *******************************************************************************/
bool ProjectDefMemoryFields::GetStringTypeFromValues(wxString &sResult) {
  wxString sValuesConcatTemp = wxEmptyString;
  wxString sValuesConcat = wxEmptyString;
  wxString myComment = wxEmptyString;

  switch (m_FieldType) {
    case TM_FIELD_ENUMERATION:
      sResult = _T("INT NULL");
      myComment = TABLE_COMMENT_ENUMERATION;
      break;
    case TM_FIELD_DATE:
      sResult = _T("DATE NULL");  // column date with null values
      break;
    case TM_FIELD_INTEGER:
      sResult = _T("INT NULL");
      break;
    case TM_FIELD_FLOAT:
      sResult = wxString::Format(_T("DECIMAL(%d,%d) NULL"), m_FieldPrecision, m_FieldScale);
      break;
    default:
      // default is text
      sResult = wxString::Format(_T("VARCHAR(%d) NULL"), m_FieldPrecision);
      break;
  }

  // deals with orientation :
  if (m_FieldOrientation) {
    myComment = GetOrientationName();
  }

  sResult.Append(wxString::Format(_T(" COMMENT \"%s\""), myComment));
  return true;
}

WX_DEFINE_OBJARRAY(PrjMemFieldArray);

/*************************         LAYERS          *************************/
ProjectDefMemoryLayers::ProjectDefMemoryLayers() {
  InitMemberValues();
}

ProjectDefMemoryLayers::~ProjectDefMemoryLayers() {
  /// todo implement way of clearing the array contained inside
  unsigned int fCount = m_pLayerFieldArray.GetCount();
  for (unsigned int i = 0; i < fCount; i++) {
    ProjectDefMemoryFields *myField = m_pLayerFieldArray.Item(0);
    wxDELETE(myField);
    m_pLayerFieldArray.RemoveAt(0);
  }
  wxASSERT(m_pLayerFieldArray.GetCount() == 0);

  unsigned int oCount = m_pLayerObjectArray.GetCount();
  for (unsigned int i = 0; i < oCount; i++) {
    ProjectDefMemoryObjects *myObj = m_pLayerObjectArray.Item(0);
    wxDELETE(myObj);
    m_pLayerObjectArray.RemoveAt(0);
  }
  wxASSERT(m_pLayerObjectArray.GetCount() == 0);
}

void ProjectDefMemoryLayers::InitMemberValues() {
  m_LayerID = 0;
  m_LayerType = LAYER_LINE;
  m_LayerName = _T("");
  m_LayerPolygonDefaultValue = wxEmptyString;
  m_StoreDeleteFields.Clear();
}

/***************************************************************************/ /**
  @brief Overloading = operator
  @author Lucien Schreiber (c) CREALP 2009
  @date 17 March 2009
  *******************************************************************************/
ProjectDefMemoryLayers &ProjectDefMemoryLayers::operator=(const ProjectDefMemoryLayers &source) {
  m_LayerID = source.m_LayerID;
  m_LayerType = source.m_LayerType;
  m_LayerName = source.m_LayerName;
  m_LayerPolygonDefaultValue = source.m_LayerName;

  /* clear fieldarray
  unsigned int fCount = m_pLayerFieldArray.GetCount();
  for (unsigned int i = 0; i<fCount; i++) {
      ProjectDefMemoryFields * myField = m_pLayerFieldArray.Item(0);
      wxDELETE(myField);
      m_pLayerFieldArray.RemoveAt(0);
  }

  // clear objarray
  unsigned int oCount = m_pLayerObjectArray.GetCount();
  for (unsigned int i = 0; i<oCount; i++) {
      ProjectDefMemoryObjects * myObj = m_pLayerObjectArray.Item(0);
      wxDELETE(myObj);
      m_pLayerObjectArray.RemoveAt(0);
  }*/

  // copy field
  for (unsigned int i = 0; i < source.m_pLayerFieldArray.GetCount(); i++) {
    ProjectDefMemoryFields *myField = new ProjectDefMemoryFields();
    ProjectDefMemoryFields *sourceField = source.m_pLayerFieldArray.Item(i);
    wxASSERT(sourceField);
    *myField = *sourceField;
    m_pLayerFieldArray.Add(myField);
  }

  // copy objects
  for (unsigned int j = 0; j < source.m_pLayerObjectArray.GetCount(); j++) {
    ProjectDefMemoryObjects *myObj = new ProjectDefMemoryObjects();
    ProjectDefMemoryObjects *mySourceObj = source.m_pLayerObjectArray.Item(j);
    wxASSERT(mySourceObj);
    *myObj = *mySourceObj;
    m_pLayerObjectArray.Add(myObj);
  }
  return *this;
}

WX_DEFINE_OBJARRAY(PrjMemLayersArray);

/*************************         OBJECTS          *************************/
ProjectDefMemoryObjects::ProjectDefMemoryObjects() {
  InitMemberValues();
}

ProjectDefMemoryObjects::~ProjectDefMemoryObjects() {}

void ProjectDefMemoryObjects::InitMemberValues() {
  m_ObjectID = 0;
  m_ObjectCode = wxEmptyString;
  m_ObjectType = LAYER_LINE;
  m_ObjectName = wxEmptyString;
  m_ObjectFreq = OBJECT_LESS_FREQUENT;
  m_ParentLayerName = wxEmptyString;
}

ProjectDefMemoryObjects &ProjectDefMemoryObjects::operator=(const ProjectDefMemoryObjects &source) {
  m_ObjectID = source.m_ObjectID;
  m_ObjectCode = source.m_ObjectCode;
  m_ObjectType = source.m_ObjectType;
  m_ObjectName = source.m_ObjectName;
  m_ObjectFreq = source.m_ObjectFreq;
  m_ParentLayerName = source.m_ParentLayerName;
  return *this;
}

WX_DEFINE_OBJARRAY(PrjMemObjectsArray);

/*************************         SCALE          *************************/
ProjectDefMemoryScale::ProjectDefMemoryScale() {
  InitMemberValues();
}

ProjectDefMemoryScale::~ProjectDefMemoryScale() {}

void ProjectDefMemoryScale::InitMemberValues() {
  m_ScaleValue = 1;
  m_DBScaleID = -1;
}

WX_DEFINE_OBJARRAY(PrjMemScaleArray);
