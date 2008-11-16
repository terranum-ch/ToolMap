/***************************************************************************
								tmexportdatashp.cpp
                    Class for the process of exporting data as SHP
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

#include "tmexportdatashp.h"



/***************************************************************************//**
 @brief Init class member values
 @author Lucien Schreiber (c) CREALP 2008
 @date 14 November 2008
 *******************************************************************************/
void tmExportDataSHP::InitMemberValues()
{
	m_Extension = _T(".shp");
}



/***************************************************************************//**
 @brief Constructor
 @details Use tmExportDataSHP::Create() for two step initialization
 @author Lucien Schreiber (c) CREALP 2008
 @date 14 November 2008
 *******************************************************************************/
tmExportDataSHP::tmExportDataSHP()
{
	InitMemberValues();
}



/***************************************************************************//**
 @brief Constructor
 @param database A valid #DataBaseTM object (an assert is done in debug mode)
 @author Lucien Schreiber (c) CREALP 2008
 @date 14 November 2008
 *******************************************************************************/
tmExportDataSHP::tmExportDataSHP (DataBaseTM * database)
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
void tmExportDataSHP::Create (DataBaseTM * database)
{
	tmExportData::Create(database);
}


/***************************************************************************//**
 @brief destructor
 @author Lucien Schreiber (c) CREALP 2008
 @date 14 November 2008
 *******************************************************************************/
tmExportDataSHP::~tmExportDataSHP()
{
	
}



/***************************************************************************//**
 @brief Create empty file
 @details Create a file  ProjectDefMemoryLayers informations, path and extension
 specified in class
 @param myLayer contain the name of the file (without extension) to create
 @param path path where we want the file to be created
 @return  true if file created with sucess, false otherwise
 @author Lucien Schreiber (c) CREALP 2008
 @date 14 November 2008
 *******************************************************************************/
bool tmExportDataSHP::CreateEmptyExportFile (ProjectDefMemoryLayers * myLayer, 
											 const wxString & path)
{
	bool bReturn = true;
	wxASSERT(myLayer);
	wxFileName * myShpFileName = GetFileName(myLayer, path);
	if (!myShpFileName)
	{
		wxASSERT_MSG(0, _T("Unable to create the file name"));
		return false;
	}
	
	
	
	if(!m_Shp.CreateFile(myShpFileName->GetFullPath(), (int) myLayer->m_LayerType))
		bReturn =  false;
	
	// create the shp
	//TODO: Add error to report indicating that export file wasn't created
	
	delete myShpFileName;
	return bReturn;
	
}



/***************************************************************************//**
 @brief Add optional fields to the Shp
 @param myfields array of fields for this shapefile
 @return  true if fields were added successfully, false otherwise
 @author Lucien Schreiber (c) CREALP 2008
 @date 15 November 2008
 *******************************************************************************/
bool tmExportDataSHP::AddOptFields (PrjMemFieldArray * myfields)
{
	wxASSERT (myfields);
	
	bool bReturn = true; // if adding fields failed, stop
	for (unsigned int i = 0; i< myfields->GetCount(); i++)
	{
		ProjectDefMemoryFields field = myfields->Item(i);
		int iSize = 0;
		
		switch (field.m_FieldType)
		{
			case TM_FIELD_TEXT:
				bReturn = m_Shp.AddFieldText(field.m_Fieldname, field.m_FieldPrecision);
				break;
				
			case TM_FIELD_INTEGER:
				bReturn = m_Shp.AddFieldNumeric(field.m_Fieldname, false);
				break;
				
			case TM_FIELD_FLOAT:
				bReturn = m_Shp.AddFieldNumeric(field.m_Fieldname, true);
				break;
				
			case TM_FIELD_ENUMERATION:
				// compute max size for enum
				iSize = GetSizeOfEnum(field.m_pCodedValueArray);
				bReturn = m_Shp.AddFieldText(field.m_Fieldname, iSize);
				break;
				
			case TM_FIELD_DATE:
				bReturn = m_Shp.AddFieldDate(field.m_Fieldname);
				break;
				
			default:
				bReturn = false;
				break;
		}
		
		if (!bReturn)
			break;
		
	}
	
	return bReturn;
}



/***************************************************************************//**
 @brief Adding generic fields
 @details Adding field :
 - OBJECT_CD
 - OBJECT_DESC
 @return  true if fields added successfully
 @author Lucien Schreiber (c) CREALP 2008
 @date 16 November 2008
 *******************************************************************************/
bool tmExportDataSHP::AddGenericFields (int iObjeDescSize)
{
	wxASSERT (iObjeDescSize);
	
	if (m_Shp.AddFieldNumeric(_T("OBJ_CD")) &&
		m_Shp.AddFieldText(_T("OBJ_DESC"), iObjeDescSize))
		return true;
	
	return false;
	
}
