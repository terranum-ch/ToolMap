/***************************************************************************
								queriesdata.h
                   
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

#include "queriesdata.h"


bool QueriesData::_IsQueryLayersCorrect() {
	wxASSERT(m_QueryType == QUERY_LAYERS);
	
	if (m_QueryName.IsEmpty()) {
		wxLogError(_T("No name specified for the query"));
		return false;
	}
	
	if (m_QueryLayerID == wxNOT_FOUND) {
		wxLogError(_T("No layer ID specified for the query"));
		return false;
	}
	
	return true;
}

bool QueriesData::_IsQuerySelectedCorrect() {
	wxASSERT(m_QueryType == QUERY_SELECTED);
	
	if (m_QueryName.IsEmpty()) {
		wxLogError(_T("No name specified for the query"));
		return false;
	}
	
	if (m_QueryObjectID == wxNOT_FOUND) {
		wxLogError(_T("No object ID specified for the query"));
		return false;
	}
	
	if (m_QueryObjectGeomID == wxNOT_FOUND) {
		wxLogError(_T("No object geometry ID specified for the query"));
		return false;
	}
	
	return true;
}

bool QueriesData::_IsQueryGenericCorrect() {
	wxASSERT(m_QueryType == QUERY_GENERIC);
	
	if (m_QueryName.IsEmpty()) {
		wxLogError(_T("No name specified for the query"));
		return false;
	}
	
	if (m_QueryLayerType > TOC_NAME_NOT_GENERIC || 
		m_QueryLayerType < TOC_NAME_LINES) {
		wxLogError(_T("Wrong layer target selected."));
		return false;
	}
	
	if (m_QuerySQL.IsEmpty()) {
		wxLogError(_T("No SQL code specified for the query"));
		return false;
	}
	
	return true;
}

bool QueriesData::_IsQuerySQLCorrect() {
	wxASSERT(m_QueryType == QUERY_SQL);
	
	if (m_QueryName.IsEmpty()) {
		wxLogError(_T("No name specified for the query"));
		return false;
	}
	
	if (m_QueryLayerType > TOC_NAME_NOT_GENERIC || 
		m_QueryLayerType < TOC_NAME_LINES) {
		wxLogError(_T("Wrong layer target selected."));
		return false;
	}
	
	if (m_QuerySQL.IsEmpty()) {
		wxLogError(_T("No SQL code specified for the query"));
		return false;
	}
	
	
	return true;
}



QueriesData::QueriesData() {
	m_QueryName = wxEmptyString;
	m_QueryType = QUERY_LAYERS;
	m_QuerySQL = wxEmptyString;
	m_QueryLayerID = wxNOT_FOUND;
	m_QueryLayerType = TOC_NAME_LINES;
	m_QueryObjectID = wxNOT_FOUND;
	m_QueryObjectGeomID = wxNOT_FOUND;
	//m_QueryFieldsLayers
	//m_QueryFieldsValues
}

QueriesData::~QueriesData() {
}


bool QueriesData::IsOk() {
	switch (m_QueryType) {
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


bool QueriesData::GetLayers(DataBaseTM * database, PrjMemLayersArray & layers) {
	wxASSERT(database);
	wxASSERT(layers.GetCount() == 0);
	
	while (1){
		ProjectDefMemoryLayers * myLayer = new ProjectDefMemoryLayers();
		
		int iRet = database->GetNextLayer(myLayer);
		if(iRet == 1){
			wxASSERT(myLayer);
			layers.Add(myLayer);
		}
		else{
			delete myLayer;
		}	
		
		
		if (iRet == -1) {
			break;
		}
		

	}
	
	if (layers.GetCount() > 0) {
		return true;
	}
	
	
	return false;
}

