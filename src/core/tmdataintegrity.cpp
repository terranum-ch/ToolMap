/***************************************************************************
 tmdataintegrity.cpp
 -------------------
 copyright            : (C) 2010 CREALP Lucien Schreiber
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "tmdataintegrity.h"

#include "../database/database_tm.h"

tmDataIntegrity::tmDataIntegrity(DataBaseTM* database) {
    m_pDB = nullptr;
    wxASSERT(database);
    m_pDB = database;
    m_ErrorType = wxNOT_FOUND;
    m_ErrorAAttrib = wxNOT_FOUND;

    m_QueryTemplateType = _T(" FROM %s WHERE OBJECT_GEOM_ID NOT IN (SELECT OBJECT_ID FROM %s)");
    m_QueryTemplateAAttrib = _T(" FROM ") + TABLE_NAME_LAYER_AT +
                             _T("%ld WHERE OBJECT_ID NOT IN (SELECT OBJECT_GEOM_ID FROM %s)");
}

tmDataIntegrity::~tmDataIntegrity() {}

bool tmDataIntegrity::HasErrorType(const PRJDEF_LAYERS_TYPE& layertype) {
    wxASSERT(m_pDB);
    m_ErrorType = wxNOT_FOUND;
    m_ErrorAAttrib = wxNOT_FOUND;

    // preparing queries
    wxString myQueryType = _T("SELECT * ");
    myQueryType.Append(wxString::Format(m_QueryTemplateType, TABLE_NAME_GIS_ATTRIBUTION[layertype].c_str(),
                                        TABLE_NAME_GIS_GENERIC[layertype].c_str()));

    // running query (type)
    if (!m_pDB->DataBaseQuery(myQueryType)) {
        return true;
    }

    if (m_pDB->DataBaseHasResults()) {
        m_pDB->DataBaseGetResultSize(nullptr, &m_ErrorType);
        m_pDB->DataBaseClearResults();
    }

    if (m_ErrorType != wxNOT_FOUND) {
        return true;
    }

    return false;
}

bool tmDataIntegrity::HasErrorAAttrib(long layerindex, const PRJDEF_LAYERS_TYPE& layertype) {
    wxASSERT(m_pDB);
    m_ErrorType = wxNOT_FOUND;
    m_ErrorAAttrib = wxNOT_FOUND;

    wxString myQueryAAttrib = _T("SELECT * ");
    myQueryAAttrib.Append(
        wxString::Format(m_QueryTemplateAAttrib, layerindex, TABLE_NAME_GIS_ATTRIBUTION[layertype].c_str()));

    // running query (aattrib)
    if (!m_pDB->DataBaseQuery(myQueryAAttrib)) {
        // advanced table may not exist
        return false;
    }

    if (m_pDB->DataBaseHasResults()) {
        m_pDB->DataBaseGetResultSize(nullptr, &m_ErrorAAttrib);
        m_pDB->DataBaseClearResults();
    }

    if (m_ErrorAAttrib != wxNOT_FOUND) {
        return true;
    }

    return false;
}

bool tmDataIntegrity::CorrectType(const PRJDEF_LAYERS_TYPE& layertype) {
    wxASSERT(m_pDB);

    // preparing queries
    wxString myQueryType = _T("DELETE ");
    myQueryType.Append(wxString::Format(m_QueryTemplateType, TABLE_NAME_GIS_ATTRIBUTION[layertype].c_str(),
                                        TABLE_NAME_GIS_GENERIC[layertype].c_str()));
    // running query (type)
    if (!m_pDB->DataBaseQueryNoResults(myQueryType)) {
        return false;
    }
    m_ErrorType = wxNOT_FOUND;
    return true;
}

bool tmDataIntegrity::CorrectAAttrib(long layerindex, const PRJDEF_LAYERS_TYPE& layertype) {
    wxASSERT(m_pDB);

    wxString myQueryAAttrib = _T("DELETE ");
    myQueryAAttrib.Append(
        wxString::Format(m_QueryTemplateAAttrib, layerindex, TABLE_NAME_GIS_ATTRIBUTION[layertype].c_str()));

    // running query (aattrib)
    if (!m_pDB->DataBaseQueryNoResults(myQueryAAttrib)) {
        // advanced table should exist here
        return false;
    }
    m_ErrorAAttrib = wxNOT_FOUND;
    return true;
}

wxString tmDataIntegrity::GetLastError() {
    if (m_ErrorType != wxNOT_FOUND && m_ErrorAAttrib != wxNOT_FOUND) {
        wxFAIL;
    }

    wxString myReturn = wxEmptyString;

    if (m_ErrorType != wxNOT_FOUND || m_ErrorAAttrib != wxNOT_FOUND) {
        myReturn = _("Integrity problem : ");
    }

    if (m_ErrorType != wxNOT_FOUND) {
        myReturn.Append(wxString::Format(_("%ld Type problem "), m_ErrorType));
    }

    if (m_ErrorAAttrib != wxNOT_FOUND) {
        myReturn.Append(wxString::Format(_("%ld Advanced attribution problem"), m_ErrorAAttrib));
    }

    return myReturn;
}
