/***************************************************************************
								tmexportdata.cpp
                    main class for the process of exporting data
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

// comment doxygen

#include "tmexportdata.h"


/***************************************************************************//**
 @brief Init class member values
 @author Lucien Schreiber (c) CREALP 2008
 @date 14 November 2008
 *******************************************************************************/
void tmExportData::InitMemberValues()
{
	m_pDB = NULL;
	m_Extension = wxEmptyString;
}



/***************************************************************************//**
 @brief Constructor
 @details Use tmExportData::Create() for two step initialization
 @author Lucien Schreiber (c) CREALP 2008
 @date 14 November 2008
 *******************************************************************************/
tmExportData::tmExportData()
{
	InitMemberValues();
}



/***************************************************************************//**
 @brief Constructor
 @param database A valid #DataBaseTM object (an assert is done in debug mode)
 @author Lucien Schreiber (c) CREALP 2008
 @date 14 November 2008
 *******************************************************************************/
tmExportData::tmExportData (DataBaseTM * database)
{
	InitMemberValues();
	Create(database);
}



/***************************************************************************//**
 @brief Creator
 @details For two step initialization. In case of use of the default constructor
 @param database A valid #DataBaseTM object (an assert is done in debug mode)
 @author Lucien Schreiber (c) CREALP 2008
 @date 14 November 2008
 *******************************************************************************/
void tmExportData::Create (DataBaseTM * database)
{
	wxASSERT(database);
	m_pDB = database;
}


/***************************************************************************//**
 @brief destructor
 @author Lucien Schreiber (c) CREALP 2008
 @date 14 November 2008
 *******************************************************************************/
tmExportData::~tmExportData()
{

}


/***************************************************************************//**
 @brief Create a file name
 @details Create a file name from ProjectDefMemoryLayers informations
 @param myLayer valid object of type #ProjectDefMemoryLayers
 @param path The path (with trailling separator) 
 @return  A valid wxFileName or Null if an error occur. If wxFileName is
 returned, caller must destroy the object once finished
 @author Lucien Schreiber (c) CREALP 2008
 @date 14 November 2008
 *******************************************************************************/
wxFileName * tmExportData::GetFileName (ProjectDefMemoryLayers * myLayer,  const wxString & path)
{
	wxASSERT(myLayer);
	wxFileName * myReturn = new wxFileName(path, myLayer->m_LayerName + m_Extension);
	if (myReturn->IsOk())
		return myReturn;
	else
	{
		wxASSERT_MSG(0,_T("Error constructing file name"));
		return NULL;
	}
}



/***************************************************************************//**
 @brief Get the max size of the items stored into array
 @details This function may be used for getting the max size we should plan for
 storring all enum values
 @param mCodedVal Contain all enum values
 @return  size of the field for storing those enum
 @author Lucien Schreiber (c) CREALP 2008
 @date 15 November 2008
 *******************************************************************************/
int tmExportData::GetSizeOfEnum (PrjMemFieldCodedValArray * mCodedVal)
{
	wxASSERT (mCodedVal);
	int iRetour = 0;
	int iTemp = 0;
	for (unsigned int i = 0; i<mCodedVal->GetCount();i++)
	{
		iTemp = mCodedVal->Item(i).m_ValueName.Len();
		if (iTemp > iRetour)
			iRetour = iTemp;
	}
	
	return iRetour;
		
}



/***************************************************************************//**
 @brief Getting size of Object Desc column
 @details This function return an array of int corresponding to the minimum size
 of the column needed for containing the OBJECT_DESC text
 @param layerindex the Database layerindex
 @return the size of the column Object_Description for the specified layers
 @author Lucien Schreiber (c) CREALP 2008
 @date 16 November 2008
 *******************************************************************************/
int tmExportData::GetSizeOfObjDesc (int layerindex)
{
	wxASSERT(m_pDB);
	
	wxString sStemp = _T("SELECT OBJECT_DESC FROM ") +
	TABLE_NAME_OBJECTS + _T(" WHERE THEMATIC_LAYERS_LAYER_INDEX = %d;");
	wxString sSentence = wxString::Format(sStemp, layerindex);
	
	
	if (!m_pDB->DataBaseQuery(sSentence))
		return 0;
	
	wxString myResult;
	int iActualCharCount = 0;
	int iTempCharCount = 0;
	while (1)
	{
		if(!m_pDB->DataBaseGetNextResult(myResult))
			break;
		
		iTempCharCount = myResult.Len();
		if (iTempCharCount > iActualCharCount)
			iActualCharCount = iTempCharCount;
	}
	
	return iActualCharCount + 1;
	
}



/***************************************************************************//**
 @brief Process query for getting simple attributs
 @param layertype type of the layer (point, line, polygon)
 @param layerindex layer index of the layer in the database
 @return  true if the query succeed, false otherwise
 @author Lucien Schreiber (c) CREALP 2008
 @date 17 November 2008
 *******************************************************************************/
bool tmExportData::GetSimpleAttribData (PRJDEF_LAYERS_TYPE layertype, long layerindex)
{
	wxString sGeomTable = wxEmptyString;
	wxString sValTable = wxEmptyString;
	switch (layertype)
	{
			
		case LAYER_LINE:
			sGeomTable = TABLE_NAME_GIS_GENERIC[0]; // lines
			sValTable = TABLE_NAME_GIS_ATTRIBUTION[0];
			break;
			
		case LAYER_POINT:
			sGeomTable = TABLE_NAME_GIS_GENERIC[1];
			sValTable = TABLE_NAME_GIS_ATTRIBUTION[1];
			break;
			
		case LAYER_POLYGON:
			sGeomTable = TABLE_NAME_GIS_GENERIC[2];
			sValTable = TABLE_NAME_GIS_ATTRIBUTION[2];
			break;
			
		default:
			break;
	}
	
	
	wxString sTemp = _T("SELECT %s.OBJECT_CD,")
	_T(" %s.OBJECT_DESC FROM %s ")
	_T(" LEFT JOIN %s ON (%s.OBJECT_ID = %s.OBJECT_GEOM_ID) ") 
	_T(" LEFT JOIN %s ON %s.OBJECT_VAL_ID = %s.OBJECT_ID WHERE")
	_T(" %s.THEMATIC_LAYERS_LAYER_INDEX = %d");
	wxString sSentence = wxString::Format(sTemp,
										  TABLE_NAME_OBJECTS.c_str(),
										  TABLE_NAME_OBJECTS.c_str(),
										  sGeomTable.c_str(),
										  sValTable.c_str(),
										  sGeomTable.c_str(),
										  sValTable.c_str(),
										  TABLE_NAME_OBJECTS.c_str(),
										  sValTable.c_str(),
										  TABLE_NAME_OBJECTS.c_str(),
										  TABLE_NAME_OBJECTS.c_str(),
										  layerindex);
	if (m_pDB->DataBaseQuery(sSentence))
		return true;
	
	return false;
		
}
