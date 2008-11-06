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



/***************************************************************************//**
 @brief Create MySQL statement for attribution
 @details This function may be used for preparing generic statement for
 attribution for TOC_NAME_LINES, TOC_NAME_LABELS and TOC_NAME_POINTS. For
 TOC_NAME_ANNOTATION it won't work.
 @param statement String for storing statement
 @param checkedVal Adress of an array containing all checked IDs
 @author Lucien Schreiber (c) CREALP 2008
 @date 06 November 2008
 *******************************************************************************/
void tmAttributionData::PrepareAttributionStatement (wxString & statement,
													const wxString & tablename,
													 wxArrayLong * checkedVal)
{
	statement.Clear();
	
	// clean before inserting
	PrepareCleaningStatement(statement, tablename);
	
	wxString sTmp = _T("INSERT INTO ") + tablename + _T(" VALUES (%d, %d); ");
	
	unsigned int geomNumber = m_SelIDs->GetCount();
	unsigned int valNumber = checkedVal->GetCount();

	// loop for all selected geometry
	for (unsigned int geom= 0; geom< geomNumber; geom++)
	{
		
		// loop for values 
		for (unsigned int val = 0; val < valNumber; val++)
				statement.Append(wxString::Format(sTmp, checkedVal->Item(val),
												  m_SelIDs->Item(geom)));
	
	}
	
}


/***************************************************************************//**
 @brief Create MySQL statement for attribution
 @details This function may be used for preparing generic statement for
 cleaning, a.k.a as removing attribution for TOC_NAME_LINES, TOC_NAME_LABELS
 and TOC_NAME_POINTS. For TOC_NAME_ANNOTATION it won't work.
 @param statement String for storing statement
 @param tablename name of the database table to insert into (generic_aat, etc.)
 @author Lucien Schreiber (c) CREALP 2008
 @date 06 November 2008
 *******************************************************************************/
void tmAttributionData::PrepareCleaningStatement (wxString & statement,
												  const wxString & tablename)
{
	statement.Clear();
	wxString sTmp = _T("DELETE FROM ") + tablename + _T(" WHERE OBJECT_GEOM_ID=%d; "); 
	
	for (unsigned int i = 0; i<m_SelIDs->GetCount(); i++)
		statement.Append(wxString::Format(sTmp, m_SelIDs->Item(i)));
	
}

