/***************************************************************************
								tmgisdatavectormysql.cpp
                    For dealing with spatial data from a mysql database
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

#include "tmgisdatavectormysql.h"


// init static member
DataBaseTM * tmGISDataVectorMYSQL::m_DB = NULL;


tmGISDataVectorMYSQL::tmGISDataVectorMYSQL()
{

}




tmGISDataVectorMYSQL::~tmGISDataVectorMYSQL()
{

	
}



bool tmGISDataVectorMYSQL:: CheckGeometryFields(const wxString & tablename)
{
	wxASSERT(m_DB);
	wxString sSentence = _T("");
	sSentence = wxString::Format(_T("SELECT COLUMN_NAME from INFORMATION_SCHEMA.COLUMNS WHERE ")
								 _T("table_schema=\"%s\" AND table_name='%s' ")
								 _T("AND COLUMN_NAME IN ('%s','%s');"), m_DB->DataBaseGetName().c_str(),
								 tablename.c_str(), tmGISMYSQL_FIELD1.c_str(), tmGISMYSQL_FIELD2.c_str());
	
	if (!m_DB->DataBaseQuery(sSentence))
	{
		wxLogDebug(_T("Error checking geometry fields : %s"), sSentence.c_str());
		return FALSE;
	}
	
	if (m_DB->DatabaseGetCountResults() != 2)
	{
		wxLogDebug(_T("The table : %s dosen't contain needed geometry fields"), tablename.c_str());
		return FALSE;
	}
	
	return TRUE;
}




bool tmGISDataVectorMYSQL::Open (const wxString & filename, bool bReadWrite)
{
	// ensure that we have set a valid pointer to the database
	wxASSERT(m_DB);
	if (!m_DB)
	{
		wxLogDebug(_T("Pointer to database invalid, open database first"));
		return FALSE;
	}
	
	// does the table exists ?
	if(!m_DB->DataBaseTableExist(filename))
	{
		wxLogDebug(_T("Table '%s' dosen't exists in the database"), filename.c_str());
		return FALSE;
	}
	
	// does the fields exists ?
	if (!CheckGeometryFields(filename))
		return FALSE;

	return TRUE;
}




tmRealRect tmGISDataVectorMYSQL::GetMinimalBoundingRectangle()
{
	
	
	return tmRealRect(0,0,0,0);
}


