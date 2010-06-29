/***************************************************************************
 tmaattribbatchmanager.cpp

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


#include "tmaattribbatchmanager.h"
#include "prjdefmemmanage.h"
#include "../database/database_tm.h"
#include "../database/databaseresult.h"
#include "../gis/tmaattribctrls.h"


tmAAttribBatchManager::tmAAttribBatchManager(PrjDefMemManage * project,
											 DataBaseTM * database,
											 tmSelectedDataMemory * selected,
											 PRJDEF_LAYERS_TYPE sellayertype) {
	m_Project = project;
	m_DB = database;
	m_Selected = selected;
	m_SelLayerType = sellayertype;
}


tmAAttribBatchManager::~tmAAttribBatchManager() {
}


bool tmAAttribBatchManager::GetTypes(PrjMemObjectsArray & objects, wxArrayInt & number, wxArrayLong & layerid) {
	objects.Clear();
	number.Clear();
	layerid.Clear();
	
	if (IsOk() == false) {
		return false;
	}
	
	// create list of ids
	wxString myIdsText = wxEmptyString;
	wxArrayLong * mySelValues = m_Selected->GetSelectedValues();
	wxASSERT(mySelValues);
	for (unsigned int i = 0; i<m_Selected->GetCount(); i++) {
		myIdsText.Append(wxString::Format(_T("%d,"),
										  mySelValues->Item(i)));
	}
	myIdsText.RemoveLast(1);
	
	// create query
	wxString mySentence = wxString::Format(_T("SELECT a.OBJECT_VAL_ID, o.OBJECT_DESC, COUNT(*), o.THEMATIC_LAYERS_LAYER_INDEX FROM ")
										   _T("%s a LEFT JOIN %s  o ON a.OBJECT_VAL_ID = o.OBJECT_ID")
										   _T("  WHERE a.OBJECT_GEOM_ID IN (%s) GROUP BY a.OBJECT_VAL_ID"),
										   TABLE_NAME_GIS_ATTRIBUTION[(int) m_SelLayerType].c_str(),
										   TABLE_NAME_OBJECTS.c_str(),
										   myIdsText.c_str());
	wxASSERT(m_DB);
	if (m_DB->DataBaseQuery(mySentence)==false) {
		return false;
	}
	
	
	if (m_DB->DataBaseHasResults() == false) {
		wxLogWarning(_("Selected data doesn't have any attribution"));
		return true;
	}
	
	// process results
	DataBaseResult myResults;
	m_DB->DataBaseGetResults(&myResults);
	for (int j = 0; j < myResults.GetRowCount(); j++) {
		ProjectDefMemoryObjects myObj;
		long myCount = 0;
		long myLayerid = 0;
		if(myResults.NextRow()==false){
			break;
		}
		myResults.GetValue(0, myObj.m_ObjectID);
		myResults.GetValue(1, myObj.m_ObjectName);
		objects.Add(myObj);
		myResults.GetValue(2, myCount);
		number.Add(myCount);
		myResults.GetValue(3, myLayerid);
		layerid.Add(myLayerid);
	}
	return true;
}


bool tmAAttribBatchManager::GetFields(long layerid, PrjMemFieldArray & fields) {
	
	fields.Clear();
	
	if (IsOk() == false) {
		return false;
	}
	
	if (layerid == wxNOT_FOUND) {
		wxLogError(_("Invalid layer ID (%d)"), layerid);
		return false;
	}
	
	
	ProjectDefMemoryLayers * myLayer = m_Project->FindLayerByRealID(layerid);
	if (myLayer == NULL) {
		wxLogError(_("No layer found with %d ID"), layerid);
		return false;
	}
	
	// check that layers are of similar spatial type
	if (m_SelLayerType != myLayer->m_LayerType) {
		return false;
	}
	
	for (int i = 0; i< m_Project->GetCountFields(); i++) {
		ProjectDefMemoryFields * mypField = m_Project->GetNextField();
		if (mypField == NULL) {
			wxLogError(_("Field %d returned NULL"), i);
			continue;
		}
		ProjectDefMemoryFields myField;
		myField = *mypField;
		fields.Add(myField);
	}
	return true;
}



tmAAttribCtrl * tmAAttribBatchManager::GetValueControl(const ProjectDefMemoryFields & field, wxWindow * wnd) {
	
	if (IsOk()==false) {
		return NULL;
	}
	
	
	tmAAttribCtrl * mypControl = NULL;
	tmAAttribCtrlInteger * mypControlInt;
	tmAAttribCtrlFloat * mypControlFloat;
	tmAAttribCtrlEnum * mypControlEnum;
	tmAAttribCtrlText * mypControlText;
#ifdef __WXOSX__
	tmAAttribCtrlSafeDate * mypControlSafeDate;
#else
	tmAAttribCtrlDate * mypControlDate;
#endif
	
	switch (field.m_FieldType)
	{
		case TM_FIELD_INTEGER:
			mypControlInt = new tmAAttribCtrlInteger(wnd, field);
			mypControl = mypControlInt;
			break;
			
		case TM_FIELD_FLOAT:
			mypControlFloat = new tmAAttribCtrlFloat(wnd, field);
			mypControl = mypControlFloat;
			break;
			
		case TM_FIELD_ENUMERATION:
			mypControlEnum = new tmAAttribCtrlEnum(wnd, field);
			mypControl = mypControlEnum;
			break;
			
		case TM_FIELD_DATE:
#ifdef __WXOSX__
			mypControlSafeDate = new tmAAttribCtrlSafeDate(wnd, field);
			mypControl = mypControlSafeDate;
#else
			mypControlDate = new tmAAttribCtrlDate(this, field);
			mypControl = mypControlDate;
#endif
			
			break;
			
		default: // tmfieldText
			mypControlText = new tmAAttribCtrlText(wnd, field);
			mypControl = mypControlText;
			break;
	}
	return mypControl;
}



bool tmAAttribBatchManager::IsOk() {
	if (m_Project == NULL) {
		wxLogError(_("Project isn't inited"));
		return false;
	}
	
	if (m_DB == NULL) {
		wxLogError(_("Database isn't inited"));
		return false;
	}
	
	if (m_Selected == NULL) {
		wxLogError(_("Selection isn't inited"));
		return false;
	}
	
	if (m_Selected->GetCount() == 0) {
		wxLogError(_("Nothing is selected, please select some objects first"));
		return false;
	}
	
	if (m_Selected->GetSelectedLayer() > TOC_NAME_LABELS) {
		wxLogError(_("Batch advanced attribution isn't availlable for selected layer (%d)"),
				   m_Selected->GetSelectedLayer());
		return false;
	}
	
	return true;
}

