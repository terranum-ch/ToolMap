/***************************************************************************
 queriesdata.cpp
 -------------------
 copyright : (C) 2009 CREALP Lucien Schreiber
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

#include "../database/database_tm.h"

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

    switch (m_QueryFieldsStatus) {
        case AATTRIBUTION_NO:
            // no special check
            break;

        case AATTRIBUTION_EMPTY:
            if (m_QueryLayerID == wxNOT_FOUND) {
                wxLogError(_T("No layer ID specified"));
                return false;
            }

            break;

        case AATTRIBUTION_YES:
            if (!HasFieldsValues()) {
                wxLogError(_T("No fields value. This mode isn't allowed"));
                return false;
            }

            if (m_QueryLayerID == wxNOT_FOUND) {
                wxLogError(_T("No layer ID specified"));
                return false;
            }

            wxASSERT(m_QueryFields.GetCount() == m_QueryFieldsValues.GetCount());
            if (m_QueryFields.GetCount() == 0) {
                wxLogError(_T("No advanced values for selected query"));
                return false;
            }
            break;

        default:
            wxLogError(_T("This case isn't supported"));
            return false;
            break;
    }

    return true;
}

bool QueriesData::_IsQueryGenericCorrect() {
    wxASSERT(m_QueryType == QUERY_GENERIC);

    if (m_QueryName.IsEmpty()) {
        wxLogError(_T("No name specified for the query"));
        return false;
    }

    if (m_QueryLayerType > TOC_NAME_NOT_GENERIC || m_QueryLayerType < TOC_NAME_LINES) {
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

    if (m_QueryLayerType > TOC_NAME_NOT_GENERIC || m_QueryLayerType < TOC_NAME_LINES) {
        wxLogError(_T("Wrong layer target selected."));
        return false;
    }

    if (m_QuerySQL.IsEmpty()) {
        wxLogError(_T("No SQL code specified for the query"));
        return false;
    }

    return true;
}

bool QueriesData::_IsQueryObjectCorrect() {
    wxASSERT(m_QueryType == QUERY_OBJECTS);

    if (m_QueryName.IsEmpty()) {
        wxLogError(_T("No name specified for the query"));
        return false;
    }

    if (m_QueryObjectID == wxNOT_FOUND) {
        wxLogError(_T("No object ID specified for the query"));
        return false;
    }

    if (m_QueryLayerType < TOC_NAME_LINES || m_QueryLayerType > TOC_NAME_LABELS) {
        wxLogError(_T("No layer ID specified"));
        return false;
    }

    return true;
}

bool QueriesData::_IsQueryLineSizeCorrect() {
    wxASSERT(m_QueryType == QUERY_LINES);

    if (m_QueryName.IsEmpty()) {
        wxLogError(_T("No name specified for the query"));
        return false;
    }

    if (m_QueryLineSize <= 0) {
        wxLogError(_T("Specified size for line is too small %d"), m_QueryLineSize);
        return false;
    }
    return true;
}

bool QueriesData::_IsQueryNodeNumberCorrect() {
    wxASSERT(m_QueryType == QUERY_NODES);

    if (m_QueryName.IsEmpty()) {
        wxLogError(_T("No name specified for the query"));
        return false;
    }

    if (m_QueryNodeNumber <= 1) {
        wxLogError(_T("Number of nodes specified isn't supported : %d"), m_QueryNodeNumber);
        return false;
    }

    return true;
}

bool QueriesData::_IsQueryDuplicateCorrect() {
    wxASSERT(m_QueryType == QUERY_DUPLICATE);

    if (m_QueryName.IsEmpty()) {
        wxLogError(_T("No name specified for the query"));
        return false;
    }

    if (m_QueryLayerType < TOC_NAME_LINES || m_QueryLayerType > TOC_NAME_ANNOTATIONS) {
        wxLogError(_T("No layer ID specified"));
        return false;
    }
    return true;
}

bool QueriesData::_IsQueryCrossingCorrect() {
    wxASSERT(m_QueryType == QUERY_CROSSING);

    if (m_QueryName.IsEmpty()) {
        wxLogError(_T("No name specified for the query"));
        return false;
    }

    if (m_QueryLayerType < TOC_NAME_LINES || m_QueryLayerType > TOC_NAME_ANNOTATIONS) {
        wxLogError(_T("No layer ID specified"));
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
    m_QueryFields.Clear();
    m_QueryFieldsValues.Clear();
    m_QueryFieldsStatus = AATTRIBUTION_NO;
    m_QueryLineSize = 10;
    m_QueryNodeNumber = 3;
    m_QueryRun = true;
}

QueriesData::~QueriesData() {}

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

        case QUERY_OBJECTS:
            return _IsQueryObjectCorrect();
            break;

        case QUERY_LINES:
            return _IsQueryLineSizeCorrect();
            break;

        case QUERY_NODES:
            return _IsQueryNodeNumberCorrect();
            break;

        case QUERY_DUPLICATE:
            return _IsQueryDuplicateCorrect();
            break;

        case QUERY_CROSSING:
            return _IsQueryCrossingCorrect();
            break;

        default:
            wxLogError(_T("Query Type not supported !"));
            break;
    }
    return false;
}

bool QueriesData::IsGenericLayer(DataBaseTM* database, long dblayerID) {
    wxASSERT(database);

    m_QueryLayerType = TOC_NAME_LINES;

    wxString myPrepQuery = _T("SELECT GENERIC_LAYERS FROM %s WHERE CONTENT_ID=%ld");
    wxString myQuery = wxString::Format(myPrepQuery, TABLE_NAME_TOC.c_str(), dblayerID);
    if (!database->DataBaseQuery(myQuery)) {
        database->DataBaseClearResults();
        return false;
    }

    long iLayerType = TOC_NAME_LINES;
    if (!database->DataBaseGetNextResult(iLayerType)) {
        database->DataBaseClearResults();
        return false;
    }
    database->DataBaseClearResults();
    m_QueryLayerType = (TOC_GENERIC_NAME)iLayerType;

    if (m_QueryLayerType < TOC_NAME_FRAME) {
        return true;
    }

    m_QueryLayerType = TOC_NAME_LINES;
    return false;
}

bool QueriesData::GetLayers(DataBaseTM* database, PrjMemLayersArray& layers) {
    wxASSERT(database);
    wxASSERT(layers.GetCount() == 0);

    while (1) {
        ProjectDefMemoryLayers* myLayer = new ProjectDefMemoryLayers();

        int iRet = database->GetNextLayer(myLayer);
        if (iRet == 1) {
            layers.Add(myLayer);
        }

        if (iRet == -1) {
            wxDELETE(myLayer);
            break;
        }
    }

    if (layers.GetCount() > 0) {
        return true;
    }

    return false;
}

bool QueriesData::GetObjectsForSelection(DataBaseTM* database, PrjMemObjectsArray& objects) {
    wxASSERT(database);
    wxASSERT(objects.GetCount() == 0);

    // get object types
    wxString myPrepQuery =
        _T("SELECT OBJECT_CD, o.OBJECT_DESC_0, o.OBJECT_TYPE_CD, l.LAYER_NAME, o.OBJECT_ISFREQ, o.OBJECT_ID ")
        _T("FROM %s o LEFT JOIN (%s l, %s a) ON l.LAYER_INDEX=o.THEMATIC_LAYERS_LAYER_INDEX AND")
        _T(" o.OBJECT_ID = a.OBJECT_VAL_ID WHERE a.OBJECT_GEOM_ID = %ld");
    wxString myQuery = wxString::Format(myPrepQuery, TABLE_NAME_OBJECTS.c_str(), TABLE_NAME_LAYERS.c_str(),
                                        TABLE_NAME_GIS_ATTRIBUTION[m_QueryLayerType].c_str(), m_QueryObjectGeomID);

    if (!database->DataBaseQuery(myQuery)) {
        return false;
    }

    long myRow = 0;
    if (!database->DataBaseGetResultSize(nullptr, &myRow)) {
        return false;
    }

    for (int i = 0; i < myRow; i++) {
        ProjectDefMemoryObjects* myObj = new ProjectDefMemoryObjects();
        if (!database->DataBaseGetNextResultAsObject(myObj, -1)) {
            database->DataBaseClearResults();
            delete myObj;
            return false;
        }
        objects.Add(myObj);
    }
    database->DataBaseClearResults();

    if (objects.GetCount() > 0) {
        return true;
    }

    return false;
}

bool QueriesData::GetObjectsForTypes(DataBaseTM* database, PrjMemObjectsArray& objects) {
    wxASSERT(database);
    wxASSERT(m_QueryLayerType >= TOC_NAME_LINES && m_QueryLayerType <= TOC_NAME_LABELS);
    objects.Clear();

    if (!database->GetObjectListByLayerType(m_QueryLayerType, true)) {
        wxASSERT(!database->DataBaseHasResults());
        return false;
    }

    // process results
    long myRow = 0;
    if (!database->DataBaseGetResultSize(nullptr, &myRow)) {
        wxLogError(_T("No results returned for %s"), PRJDEF_LAYERS_TYPE_STRING[m_QueryLayerType].c_str());
        return false;
    }

    for (int i = 0; i < myRow; i++) {
        ProjectDefMemoryObjects* myObj = new ProjectDefMemoryObjects();
        if (!database->DataBaseGetNextResultAsObject(myObj, -1)) {
            database->DataBaseClearResults();
            delete myObj;
            return false;
        }
        objects.Add(myObj);
    }
    database->DataBaseClearResults();

    if (objects.GetCount() > 0) {
        return true;
    }

    return false;
}

bool QueriesData::GetParentLayer(DataBaseTM* database, long& layerid) {
    layerid = wxNOT_FOUND;
    m_QueryLayerID = wxNOT_FOUND;

    if (m_QueryObjectID == wxNOT_FOUND) {
        wxLogError(_("No object specified"));
        return false;
    }

    wxString myPreparedQuery(_T("SELECT THEMATIC_LAYERS_LAYER_INDEX FROM %s WHERE OBJECT_ID = %ld"));
    wxString mySentence = wxString::Format(myPreparedQuery, TABLE_NAME_OBJECTS.c_str(), m_QueryObjectID);
    if (!database->DataBaseQuery(mySentence)) {
        wxLogError(_("Error getting parent layer for object : %ld"), m_QueryObjectID);
        return false;
    }

    bool bReturn = true;
    if (!database->DataBaseGetNextResult(layerid)) {
        wxLogError(_("Error getting results for object : %ld"), m_QueryObjectID);
        bReturn = false;
    }
    database->DataBaseClearResults();

    if (bReturn) {
        m_QueryLayerID = layerid;
    }

    return bReturn;
}

bool QueriesData::GetFieldsValues(DataBaseTM* database, long layerid, PrjMemFieldArray& fieldsdef,
                                  wxArrayString& fieldsvalue) {
    wxASSERT(database);
    wxASSERT(layerid > 0);
    fieldsdef.Clear();
    fieldsvalue.Clear();

    // m_QueryLayerID = layerid;

    ProjectDefMemoryLayers myTempLayer;
    myTempLayer.m_LayerID = layerid;

    // get fields for specified layer;
    if (!database->GetFields(fieldsdef, &myTempLayer)) {
        wxLogError(_("Error getting fields for layer %ld"), layerid);
        return false;
    }

    if (fieldsdef.GetCount() == 0) {
        wxLogError(_("No fields availlable for layer %ld"), layerid);
        return false;
    }

    wxASSERT(fieldsdef.GetCount() > 0);

    // copy fields to temp layer
    for (unsigned int i = 0; i < fieldsdef.GetCount(); i++) {
        ProjectDefMemoryFields* myField = new ProjectDefMemoryFields();
        *myField = *(fieldsdef.Item(i));
        myTempLayer.m_pLayerFieldArray.Add(myField);
    }

    // get values
    wxArrayLong mySelectedId;
    mySelectedId.Add(m_QueryObjectGeomID);
    tmAttributionData* myAttribObj = tmAttributionManager::CreateAttributionData(m_QueryLayerType);
    if (myAttribObj == nullptr) {
        wxLogError(_T("Unable to create attribution object"));
        return false;
    }

    myAttribObj->Create(&mySelectedId, database);
    bool bReturn = true;
    wxArrayString myFieldsCode;
    if (!myAttribObj->GetAdvancedAttribution(&myTempLayer, fieldsvalue, myFieldsCode, m_QueryObjectGeomID)) {
        wxLogError(_("Error getting attribution values"));
        bReturn = false;
    } else {
        wxASSERT(fieldsvalue.GetCount() == fieldsdef.GetCount());
    }

    // cleaning
    if (myAttribObj) {
        delete myAttribObj;
    }

    return bReturn;
}

bool QueriesData::DeleteFieldsValue(int index) {
    wxASSERT(m_QueryFields.GetCount() == m_QueryFieldsValues.GetCount());
    if (index >= (signed)m_QueryFields.GetCount()) {
        wxLogError(_T("Trying to delete out of bounds"));
        return false;
    }

    m_QueryFields.RemoveAt(index);
    m_QueryFieldsValues.RemoveAt(index);

    return true;
}

bool QueriesData::HasFieldsValues() {
    wxASSERT(m_QueryFields.GetCount() == m_QueryFieldsValues.GetCount());
    if (m_QueryFields.GetCount() == 0) {
        return false;
    }

    for (unsigned int i = 0; i < m_QueryFieldsValues.GetCount(); i++) {
        if (!m_QueryFieldsValues.Item(i).IsEmpty()) {
            return true;
        }
    }

    return false;
}
