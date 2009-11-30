/***************************************************************************
								querieswizard.cpp
                    Main part of the new query system
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

#include "querieswizard.h"






void QueriesWizard::_Init() {
	m_QueryName = wxEmptyString;
	m_QueryType = QUERY_LAYERS;
	m_QuerySQL = wxEmptyString;
	m_QueryLayerID = wxNOT_FOUND;
	m_QueryLayerType = TOC_NAME_LINES;
	m_QueryObjectID = wxNOT_FOUND;
	m_QueryObjectGeomID = wxNOT_FOUND;
	//m_QueryFieldsLayers
	//m_QueryFieldsValues
	m_pDB = NULL;
	m_PageIntro = NULL;
}



int QueriesWizard::ShowWizard() {
	if (RunWizard(m_PageIntro)==true){
		return wxOK;
	}
	else {
		return wxCANCEL;
	}

}



QueriesWizard::QueriesWizard(wxWindow * parent, DataBaseTM * database, int id) :
wxWizard(parent, id, _("Add query Wizard"), wxNullBitmap, wxDefaultPosition){
	_Init();
	m_pDB = database;
	//wxASSERT(m_pDB);
	
	m_PageIntro = new QueriesPageIntro(this);
	
	GetPageAreaSizer()->Add(m_PageIntro);
	wxSize mySize = GetPageAreaSizer()->CalcMin();
	SetMinSize(mySize);
	SetPageSize(mySize);
}


QueriesWizard::~QueriesWizard() {
	this->Destroy();
}
