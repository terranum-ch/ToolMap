/***************************************************************************
 databaseresult.cpp
 
 -------------------
 copyright            : (C) 2010 CREALP Lucien Schreiber 
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


#include "databaseresult.h"

DataBaseResult::DataBaseResult() {
	m_ResultSet = NULL;
	m_RowIndex = wxNOT_FOUND;
}


DataBaseResult::DataBaseResult(MYSQL_RES ** results) {
	Create(results);
}

void DataBaseResult::Create(MYSQL_RES ** results) {
	m_RowIndex = wxNOT_FOUND;
	m_ResultSet = results;
}


DataBaseResult::~DataBaseResult() {
	if (m_ResultSet != NULL) {
		wxLogMessage(_T("Cleaning result set"));
		mysql_free_result(*m_ResultSet);
		*m_ResultSet = NULL;
		m_ResultSet = NULL;
	}
}


bool DataBaseResult::HasResults() {
	if (GetRowCount() <= 0) {
		return false;
	}
	
	return true;
}


int DataBaseResult::GetColCount() {
	if (m_ResultSet == NULL) {
		wxLogError(_("Result set not inited or not valid"));
		return wxNOT_FOUND;
	}
	
	return mysql_num_fields(*m_ResultSet);
}


long DataBaseResult::GetRowCount() {
	if (m_ResultSet == NULL) {
		wxLogError(_("Result set not inited or not valid"));
		return wxNOT_FOUND;
	}
	
	return mysql_num_rows(*m_ResultSet);
}


bool DataBaseResult::GetColName(wxArrayString & fields) {
	fields.Clear();
	if (m_ResultSet == NULL) {
		wxLogError(_("Result set not inited or not valid"));
		return false;
	}
	
	int num_fields = GetColCount();
	wxASSERT(num_fields != wxNOT_FOUND);
	
	MYSQL_FIELD *pfields = mysql_fetch_fields(*m_ResultSet);
	wxASSERT(pfields != NULL);
	for (int i = 0; i<num_fields; i++) {
		fields.Add(wxString(pfields[i].name, wxConvUTF8));
	}
	
	return true;
}


bool DataBaseResult::GetValue(int col, wxString & value) {
	if (m_ResultSet == NULL) {
		wxLogError(_("Result set not inited or not valid"));
		return false;
	}
	
	if (IsRowOk() == false) {
		wxLogError(_("No data to retrive"));
		return false;
	}
	
	wxASSERT(m_Row);
	wxASSERT(col >= 0 && col < GetColCount());
	
	value = wxString(m_Row[col], wxConvUTF8);
	return true;
}


bool DataBaseResult::GetValue(int col, long & value) {
	
	wxString myValue = wxEmptyString;
	if (GetValue(col, myValue)==false){
		return false;
	}
	
	if (myValue.ToLong(&value)==false) {
		wxLogError(_("Error converting '%s' to long"), myValue.c_str());
		return false;
	}
	
	return true;
}


bool DataBaseResult::NextRow() {
	if (m_ResultSet == NULL) {
		wxLogError(_("Result set not inited or not valid"));
		return false;
	}
	
	m_Row = mysql_fetch_row(*m_ResultSet);
	
	if (!m_Row) {
		return false;
	}
	
	m_RowIndex ++;
	
	return true;
}


bool DataBaseResult::IsRowOk() {
	if (m_RowIndex == wxNOT_FOUND) {
		wxLogError(_("Move to first row using NextRow()"));
		return false;
	}
	
	if (!m_Row) {
		return false;
	}
	
	return true;
}

