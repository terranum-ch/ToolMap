/***************************************************************************
								queriesbuilder.cpp
                   
                             -------------------
    copyright            : (C) 2009 CREALP Lucien Schreiber 
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

#include "queriesbuilder.h"


bool QueriesBuilder::_IsQueryNull() {
	if (m_QueryData == NULL) {
		wxLogError(_T("Query is null !"));
		return true;
	}
	return false;
}



bool QueriesBuilder::_IsQueryLayersCorrect() {
	wxASSERT(m_QueryData->m_QueryType == QUERY_LAYERS);
	
	if (m_QueryData->m_QueryName.IsEmpty()) {
		wxLogError(_T("No name specified for the query"));
		return false;
	}
	
	if (m_QueryData->m_QueryLayerID == wxNOT_FOUND) {
		wxLogError(_T("No layer ID specified for the query"));
		return false;
	}
			
	return true;
}

bool QueriesBuilder::_IsQuerySelectedCorrect() {
	wxASSERT(m_QueryData->m_QueryType == QUERY_SELECTED);
	
	if (m_QueryData->m_QueryName.IsEmpty()) {
		wxLogError(_T("No name specified for the query"));
		return false;
	}
	
	if (m_QueryData->m_QueryObjectID == wxNOT_FOUND) {
		wxLogError(_T("No object ID specified for the query"));
		return false;
	}
	
	if (m_QueryData->m_QueryObjectGeomID == wxNOT_FOUND) {
		wxLogError(_T("No object geometry ID specified for the query"));
		return false;
	}
	
	return true;
}

bool QueriesBuilder::_IsQueryGenericCorrect() {
	wxASSERT(m_QueryData->m_QueryType == QUERY_GENERIC);
	
	if (m_QueryData->m_QueryName.IsEmpty()) {
		wxLogError(_T("No name specified for the query"));
		return false;
	}
	
	if (m_QueryData->m_QueryLayerType > TOC_NAME_NOT_GENERIC || 
		m_QueryData->m_QueryLayerType < TOC_NAME_LINES) {
		wxLogError(_T("Wrong layer target selected."));
		return false;
	}
	
	if (m_QueryData->m_QuerySQL.IsEmpty()) {
		wxLogError(_T("No SQL code specified for the query"));
		return false;
	}
	
	return true;
}

bool QueriesBuilder::_IsQuerySQLCorrect() {
	wxASSERT(m_QueryData->m_QueryType == QUERY_SQL);
	
	if (m_QueryData->m_QueryName.IsEmpty()) {
		wxLogError(_T("No name specified for the query"));
		return false;
	}
	
	if (m_QueryData->m_QueryLayerType > TOC_NAME_NOT_GENERIC || 
		m_QueryData->m_QueryLayerType < TOC_NAME_LINES) {
		wxLogError(_T("Wrong layer target selected."));
		return false;
	}
	
	if (m_QueryData->m_QuerySQL.IsEmpty()) {
		wxLogError(_T("No SQL code specified for the query"));
		return false;
	}
	
	
	return true;
}


bool QueriesBuilder::_IsCreated() {
	if (m_IsCreated == false){
		wxLogError(_T("Query not created, call Create() first"));
		return false;
	}
	return true;
}



QueriesBuilder::QueriesBuilder(QueriesData * query) {
	wxASSERT(query);
	m_QueryData = query;
	m_IsCreated = false;
}

QueriesBuilder::~QueriesBuilder() {
}

bool QueriesBuilder::IsOk() {
	if (_IsQueryNull())
		return false;
	
	switch (m_QueryData->m_QueryType) {
		case QUERY_LAYERS:
			return _IsQueryLayersCorrect();
			break;
			
		case QUERY_SELECTED:
			return _IsQuerySelectedCorrect();
			break;
			
		case QUERY_GENERIC:
			return _IsQueryGenericCorrect();
			break;
			
		case QUERY_SQL:
			return _IsQuerySQLCorrect();
			break;
				
		default:
			wxLogError(_T("Query Type not supported !"));
			break;
	}
	return false;
}



bool QueriesBuilder::Save(DataBaseTM * database) {
	wxASSERT(database);
		
	if (_IsCreated()==false){
		return false;
	}
	
	
	return database->EditQueries((int) m_QueryData->m_QueryLayerType,
						  m_QueryData->m_QueryName,
						  m_QueryData->m_QuerySQL,
						  -1);
}


bool QueriesBuilder::Create(DataBaseTM * database) {
	
	if (IsOk()==false)
		return false;
	
	switch (m_QueryData->m_QueryType) {
		case QUERY_GENERIC:
			m_IsCreated = true;
			break;
			
		default:
			wxLogError(_T("Unsupported query type"));
			break;
	}
	
	return m_IsCreated;
}



