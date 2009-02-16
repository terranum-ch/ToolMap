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
 @return true if all objects are valid and correctly initialized,
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
 @param tablename name of the database table to insert into (generic_aat, etc.)
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



/***************************************************************************//**
 @brief Create MySQL statement for getting info
 @details Returning info works only for one selected feature so we get selected
 values only for the first selected values.
 @param statement a place for storing the statement
 @param tablename Name of the table : see #TABLE_NAME_GIS_ATTRIBUTION
 @author Lucien Schreiber (c) CREALP 2008
 @date 07 November 2008
 *******************************************************************************/
void tmAttributionData::PrepareGetInfoStatement (wxString & statement, const wxString & tablename)
{
	statement.Clear();
	wxString sTmp = _T("SELECT OBJECT_VAL_ID FROM ") + tablename + 
					_T(" WHERE OBJECT_GEOM_ID=%d; ");
	
	statement.Append(wxString::Format(sTmp, m_SelIDs->Item(0)));
}



/***************************************************************************//**
 @brief Set Basic attribution
 @details This function attributes the selected object with the specified
 values. This works in two steps : First we delete all values for selected
 object and then we insert specified values for selected objects
 This function is mainly used for attribution by shortcuts
 @param values An array of long containing the values for attribution
 @return  Return true if all works correctly, otherwise false
 @author Lucien Schreiber (c) CREALP 2008
 @date 18 December 2008
 *******************************************************************************/
bool tmAttributionData::SetAttributeBasicValues(wxArrayLong * values)
{
	wxASSERT (!m_TableName.IsEmpty());
	wxString myClearStatement;
	wxString myAttribStatement;
	
	PrepareCleaningStatement(myClearStatement, m_TableName);
	PrepareAttributionStatement(myAttribStatement, m_TableName, values);
	
	wxASSERT (m_pDB);
	if (!m_pDB->DataBaseQueryNoResult(myClearStatement + myAttribStatement))
	{
		wxLogDebug(_T("Error trying to attributes objects : %s"),
				   m_pDB->DataBaseGetLastError().c_str());
		return false;
		
	}
	
	return true;
}



/***************************************************************************//**
 @brief Copy attribution between objects
 @details Use tmAttributionData::Create for setting database and the 
 Multiples OID of the objects we copy attributes to
 @param copyfrom OID of the object we copy attributes from
 @return  true if copy succeed, false otherwise
 @author Lucien Schreiber (c) CREALP 2009
 @date 16 February 2009
 *******************************************************************************/
bool tmAttributionData::CopyAttributesBasic (const long & copyfrom)
{
	
	wxASSERT (!m_TableName.IsEmpty());
	wxASSERT (IsValid());
	if (IsValid()==false)
		return false;
	
	wxArrayLong myAttribValues;
	if (GetInfoBasicValues(copyfrom, myAttribValues)== false)
		return false;
	
	if (myAttribValues.GetCount() == 0)
	{
		wxLogDebug(_T("No attribution to copy"));
		return false;
	}
	
	if (SetAttributeBasicValues(&myAttribValues)== false)
		return false;
	
	return true;
}


/***************************************************************************//**
 @brief Get basic attribution values for passed ID
 @param selected The feature ID we want attribution for
 @param values The returned attribution values
 @return true if attributions where returned, false otherwise
 @author Lucien Schreiber (c) CREALP 2009
 @date 13 February 2009
 *******************************************************************************/
bool tmAttributionData::GetInfoBasicValues (const long & selected, 
												wxArrayLong & values)
{
	// checking
	wxASSERT (!m_TableName.IsEmpty());
	wxASSERT (IsValid());
	if (!IsValid())
		return false;
	
	// getting values
	wxString sStatement = _T("");
	
	m_SelIDs->Insert(selected,0);
	PrepareGetInfoStatement(sStatement, m_TableName);
	m_SelIDs->RemoveAt(0);
	
	if (!m_pDB->DataBaseQuery(sStatement))
	{
		wxLogDebug(_T("Error getting info : %s"),
				   m_pDB->DataBaseGetLastError().c_str());
		return false;
	}
	
	
	long mySelTemp = wxNOT_FOUND;
	while (1)
	{
		mySelTemp = m_pDB->DataBaseGetNextResultAsLong();
		if (mySelTemp == wxNOT_FOUND)
			break;
		values.Add(mySelTemp);
	}
	
	return true;
}


