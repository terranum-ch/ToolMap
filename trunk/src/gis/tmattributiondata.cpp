/***************************************************************************
								tmattributiondata.h
                    Main class for attribution process
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

#include "tmattributiondata.h"



/***************************************************************************//**
 @brief Default constructor
 @details Use the tmAttributionData::Create() function after for two step
 construction
 @author Lucien Schreiber (c) CREALP 2008
 @date 06 November 2008
 *******************************************************************************/
tmAttributionData::tmAttributionData()
{
	InitMemberValues();
}



/***************************************************************************//**
 @brief Destructor
 @author Lucien Schreiber (c) CREALP 2008
 @date 06 November 2008
 *******************************************************************************/
tmAttributionData::~tmAttributionData()
{

}



/***************************************************************************//**
 @brief Member values initialisation
 @author Lucien Schreiber (c) CREALP 2008
 @date 06 November 2008
 *******************************************************************************/
void tmAttributionData::InitMemberValues()
{
	m_TableName = wxEmptyString;
	m_SelIDs = NULL;
}


void tmAttributionData::Create (wxArrayLong * selected,DataBaseTM * database)
{
	if (selected)
		m_SelIDs = selected;
	
	if (database)
		m_pDB = database;
	
}


tmAttributionData::tmAttributionData(wxArrayLong * selected,DataBaseTM * database)
{
	InitMemberValues();
	Create(selected, database);	
}



/***************************************************************************//**
 @brief Verify validity of member objects
 @param bool return true if all objects are valid and correctly initialized,
 false otherwise
 @author Lucien Schreiber (c) CREALP 2008
 @date 06 November 2008
 *******************************************************************************/
bool tmAttributionData::IsValid ()
{
	bool bRetVal = true;
	if (!m_SelIDs)
		bRetVal = false;
	
	if (!m_pDB)
		bRetVal = false;
	
	if (m_TableName == wxEmptyString)
		bRetVal = false;
	
	wxASSERT_MSG(bRetVal, _T("Error, tmAttributionData wasn't correctly initialized"));
	
	return bRetVal;
}
