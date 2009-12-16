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

