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



