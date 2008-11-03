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

/***************************************************************************//**
 @brief Tokenize coded values
 @details Convert from a string containing concateneted coded values to a
 wxArrayString
 @param codedvalue the original string with all values concateneted
 @param results a wxArrayString containing all extracted coded values
 @return  the number of coded values found
 @author Lucien Schreiber (c) CREALP 2007
 @date 17 April 2008
 *******************************************************************************/
int ProjectDefMemoryFieldsCodedVal::ExtractCodedValuesFromString (const wxString & codedvalue, 
								  wxArrayString & results)
{
	wxStringTokenizer tkz(codedvalue, wxT(",")); 
	while ( tkz.HasMoreTokens()) 
	{ 
		wxString token = tkz.GetNextToken(); 
		token = token.Mid(1, token.Length()-2);
		results.Add(token);
	}
	
	return results.GetCount();
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
	///@todo implement way of clearing the array contained inside
}




void ProjectDefMemoryFields::InitMemberValues()
{
	m_FieldID = 0;
	m_Fieldname = _T("");
	m_FieldOldName = _T("");
	m_FieldType = TM_FIELD_TEXT;
	m_FieldPrecision = 0;
	m_FieldScale = 0;
	m_FieldOrientation = FALSE;
	m_FieldConstrain = FALSE;
	
	m_pCodedValueArray = new PrjMemFieldCodedValArray();
	
}


/***************************************************************************//**
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
wxString ProjectDefMemoryFields::ExtractValuesFromString(const wxString & fielddef)
{
	wxString myTempValue = _T("");
	myTempValue = fielddef.AfterFirst('(');
	return myTempValue.RemoveLast(1);
}


/***************************************************************************//**
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
bool ProjectDefMemoryFields::SetValues(const wxArrayString & fielddef)
{
	wxString myFieldType = _T("");
	wxString myValues = _T("");
	wxString myTempConvert = _T("");
	wxArrayString myCodedValResults;
	int iCountCodedVal = 0;
	
	if(fielddef.GetCount() != 3)
	{
		// assign a field id greather than 0
		m_FieldID = 1;
		
		// get the field name
		m_Fieldname = fielddef.Item(1);
		m_FieldOldName = fielddef.Item(1);
		
		// get the field type
		myFieldType = fielddef.Item(2).BeforeFirst('(');
		if (!myFieldType.IsEmpty())
		{
			// ENUMERATION //
			if(myFieldType == _T("enum"))
			{
				m_FieldType = TM_FIELD_ENUMERATION;
				
				// compute the enumerations values
				myTempConvert = ExtractValuesFromString(fielddef.Item(2));
				iCountCodedVal = ProjectDefMemoryFieldsCodedVal::
					ExtractCodedValuesFromString(myTempConvert, myCodedValResults);
				for (int i = 0; i < iCountCodedVal ; i++)
				{
					ProjectDefMemoryFieldsCodedVal * myVal = new ProjectDefMemoryFieldsCodedVal();
					myVal->m_ValueCode = i+1;
					myVal->m_ValueName = myCodedValResults.Item(i);
					m_pCodedValueArray->Add(myVal);
				}
			}
			
			
			// INTEGER //
			else if (myFieldType == _T("int"))
			{
				m_FieldType = TM_FIELD_INTEGER;
				// compute nothing... :-) we know
				// that integer is 10 decimals 
				// storing value up to 2147483648
			}
			
			
			// FLOAT //
			else if (myFieldType == _T("decimal"))
			{
				m_FieldType = TM_FIELD_FLOAT;
				// compute precision and scale
				myTempConvert = ExtractValuesFromString(fielddef.Item(2));
				m_FieldPrecision	= wxAtoi(myTempConvert.BeforeFirst(','));
				m_FieldScale		= wxAtoi(myTempConvert.AfterFirst(','));
			}
			
			
			// DATE //
			else if (myFieldType == _T("date"))
			{
				m_FieldType = TM_FIELD_DATE;
			}
			
			
			// TEXT //
			else if (myFieldType == _T("varchar"))
			{
				m_FieldType = TM_FIELD_TEXT;
				// compute size (precision)
				m_FieldPrecision = wxAtoi(ExtractValuesFromString(fielddef.Item(2)));
			}

		}
		
		// get orientation (item number 8)
		if (fielddef.Item(3) == ProjectDefMemoryFields::GetOrientationName())
			m_FieldOrientation = TRUE;
		// otherwise, field orientation is false :-)
			
			
		return TRUE;
	}
	
	wxLogError(_T("Text passed for field conversion seems to be false..."));
	return FALSE;
}


/***************************************************************************//**
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
bool ProjectDefMemoryFields::GetStringTypeFromValues (wxString & sResult)
{
	wxString sValuesConcatTemp = _T("");
	wxString sValuesConcat = _T("");
	
	switch (m_FieldType)
	{
		case TM_FIELD_ENUMERATION:
			// get all coded values from array and concatenate them
			for (unsigned int i = 0; i< m_pCodedValueArray->GetCount(); i++)
			{
				sValuesConcatTemp = m_pCodedValueArray->Item(i).m_ValueName;
				sValuesConcat.Append(wxString::Format(_T("\"%s\","), sValuesConcatTemp.c_str()));
			}
			// remove last comma
			sValuesConcat = sValuesConcat.BeforeLast(',');
			if (!sValuesConcat.IsEmpty())
			{
				sResult=wxString::Format(_T("ENUM(%s)"),sValuesConcat.c_str());
			}
			break;
		case TM_FIELD_DATE:
			sResult = _T("DATE NULL"); // column date with null values
			break;
		case TM_FIELD_INTEGER :
			sResult = _T("INT NULL");
			break;
		case TM_FIELD_FLOAT:
			sResult = wxString::Format(_T("DECIMAL(%d,%d) NULL"), m_FieldPrecision,
									   m_FieldScale);
			break;
		default:
			// default is text
			sResult = wxString::Format(_T("VARCHAR(%d) NULL"),m_FieldPrecision);
			break;
	}
	
	// deals with orientation :
	if (m_FieldOrientation)
		sResult.Append(_T(" COMMENT \"") + GetOrientationName() + _T("\""));
	else
		sResult.Append(_T(" COMMENT ''"));

	return TRUE;
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
	m_LayerType = LAYER_LINE;
	m_pLayerFieldArray = NULL;
	m_pLayerObjectArray = NULL;
	m_LayerName = _T("");
	m_LayerPolygonDefaultValue = wxEmptyString;
	
	m_pLayerFieldArray = new PrjMemFieldArray();
	m_pLayerObjectArray = new PrjMemObjectsArray();
	m_StoreDeleteFields.Clear();
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
	m_ObjectType = LAYER_LINE;
	m_ObjectName = _T("");
	m_ObjectFreq = OBJECT_LESS_FREQUENT;
	m_ParentLayerName = _T("");
}

WX_DEFINE_OBJARRAY (PrjMemObjectsArray);




/*************************         SCALE          *************************/
ProjectDefMemoryScale::ProjectDefMemoryScale()
{
	InitMemberValues();
}

ProjectDefMemoryScale::~ProjectDefMemoryScale()
{
	
}

void ProjectDefMemoryScale::InitMemberValues()
{
	m_ScaleValue = 1;
	m_DBScaleID = -1;
}

WX_DEFINE_OBJARRAY (PrjMemScaleArray);



