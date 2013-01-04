/***************************************************************************
 siattribut.cpp
 -------------------
 copyright            : (C) 2012 CREALP Lucien Schreiber
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

#include "siattribut.h"
#include "siparam.h"

siAttributValue::siAttributValue(siAttribut * parent) {
    m_ParentAttribut = parent;
    wxASSERT(m_ParentAttribut);
    Reset();
}



siAttributValue::~siAttributValue() {
}

bool siAttributValue::_GetCatalogDatabaseID (DataBase * database){
    // get real databaseID
    wxString myQuery = wxString::Format(_T("SELECT c.CATALOG_ID FROM dmn_catalog c LEFT JOIN dmn_attribut_value v ON (c.CATALOG_ID = v.CATALOG_ID) WHERE (c.DESCRIPTION_0 = '%s' AND v.ATTRIBUT_ID = %ld)"), m_ValueOut, m_ParentAttribut->GetAttributIDReal());
    if (database->DataBaseQuery(myQuery)==false) {
        return false;
    }
    
    if (database->DataBaseGetNextResult(m_ValueOutCode)==false) {
        database->DataBaseClearResults();
        return false;
    }
    database->DataBaseClearResults();
    //wxLogDebug(_("Attribut '%s' loaded (ID is %ld)"),m_ValueOut, m_ValueOutCode);
    return true;
}


bool siAttributValue::_LoadOperationValue(const wxString & text,  DataBase * database, long layerindex) {
    siParam myParam;
    bool bError = false;
    m_ValueOut = myParam.GetParamByCol(text, 0, bError);
    if (bError == true) {
        return false;
    }
    
    return _GetCatalogDatabaseID(database);
}



bool siAttributValue::_LoadOperationReplace(const wxString & text,  DataBase * database, long layerindex) {
    siParam myParam;
    bool bError = false;
    m_ValueIn = myParam.GetParamByCol(text, 0, bError);
    if (bError == true) {
        wxLogError(_("Unable to get VALUE IN"));
        return false;
    }
    m_ValueOut = myParam.GetParamByCol(text, 1, bError);
    if (bError == true) {
        wxLogError(_("Unable to get VALUE OUT"));
        return false;
    }
    
    return _GetCatalogDatabaseID(database);    
}



bool siAttributValue::LoadFromText(const wxString & text, DataBase * database, long layerindex) {
    Reset();
    wxASSERT(m_ParentAttribut);
    switch (m_ParentAttribut->GetAttributOperation()) {
        case SIATTRIBUT_OPERATION_VALUE:
            return _LoadOperationValue(text, database, layerindex);
            break;
            
        case SIATTRIBUT_OPERATION_REPLACE:
            return _LoadOperationReplace(text, database, layerindex);
            break;
            
        default:
            wxLogError(_("Loading attribut value not supported!"));
            break;
    }
    return false;
}


void siAttributValue::Reset(){
    m_ValueIn = wxEmptyString;
    m_ValueOut = wxEmptyString;
    m_ValueOutCode = wxNOT_FOUND;
}







/*************************************************************************************//**
@brief Attribut class
@author Lucien Schreiber copyright CREALP
@date 04 janvier 2013
*****************************************************************************************/
void siAttribut::_ClearAttributValueArray() {
    int iNbItem = m_Values.GetCount();
    for (int i = iNbItem-1; i >= 0; i--) {
        siAttributValue * myValue = m_Values.Item(i);
        wxASSERT(myValue);
        wxDELETE(myValue);
        m_Values.RemoveAt(i);
    }
    wxASSERT(m_Values.GetCount() == 0);
}



long siAttribut::_GetIDForValueIn(const wxString & valueintxt) {
    for (unsigned int i = 0; i< m_Values.GetCount(); i++) {
        siAttributValue * myValue = m_Values.Item(i);
        wxASSERT(myValue);
        if (myValue->GetValueIn() == valueintxt) {
            return myValue->GetValueOutCode();
        }
     }
    return wxNOT_FOUND;
}



siAttribut::siAttribut() {
    Reset();
}




siAttribut::~siAttribut() {
    _ClearAttributValueArray();
}



bool siAttribut::LoadFromArray(const wxArrayString & attribtxt, DataBase * database, long layerindex ) {
    Reset();
    siParam myParam;
    bool bError = false;
    
    m_AttributNameIn = myParam.GetParam(attribtxt.Item(0), _T("ATTRIBUT_IN"), bError);
    if (bError == true) {
        wxLogError(_("Error getting ATTRIBUT_IN"));
        return false;
    }
    m_AttributNameOut = myParam.GetParam(attribtxt.Item(1), _T("ATTRIBUT_OUT"), bError);
    if (bError == true) {
        wxLogError(_("Error getting ATTRIBUT_OUT"));
        return false;
    }
    wxString myAttributFilterKindTxt = myParam.GetParam(attribtxt.Item(2), _T("ATT_FILTER_KIND"), bError);
    if (bError == true) {
        wxLogError(_("Error getting ATT_FILTER_KIND"));
        return false;
    }
    wxString myAttributOperationTxt = myParam.GetParam(attribtxt.Item(3), _T("ATT_OPERATION"), bError);
    if (bError == true) {
        wxLogError(_("Error getting ATT_OPERATION"));
        return false;
    }
    
    if (myAttributFilterKindTxt != _T("All")) {
        // TODO: here process Kind IDS
    }
    
    if (myAttributOperationTxt == _T("VALUE")) {
        m_AttributOperation = SIATTRIBUT_OPERATION_VALUE;
    }else if (myAttributOperationTxt == _T("REPLACE")){
        m_AttributOperation = SIATTRIBUT_OPERATION_REPLACE;
    }else{
        wxLogError(_("'%s' value not supported!"), myAttributOperationTxt);
        return false;
    }
    
    // get attribut ID
    wxString myQuery = wxString::Format(_T("SELECT ATTRIBUT_ID FROM dmn_layer_attribut WHERE ATTRIBUT_NAME = '%s' AND LAYER_INDEX=%ld"), GetAttributNameOut(), layerindex);
    if (database->DataBaseQuery(myQuery)==false) {
        return false;
    }
    if (database->DataBaseGetNextResult(m_AttributIDReal)==false) {
        database->DataBaseClearResults();
        return false;
    }
    database->DataBaseClearResults();
    
    for (unsigned int i = 4; i< attribtxt.GetCount(); i++) {
        siAttributValue * myAtValue = new siAttributValue(this);
        if (myAtValue->LoadFromText(attribtxt.Item(i), database, layerindex)==false) {
            wxDELETE(myAtValue);
            wxLogError(_("Loading attribut value from '%s' failed!"), attribtxt.Item(i));
            continue;
        }   
        m_Values.Add(myAtValue);
    }
    return true;
}


bool siAttribut::Process(OGRFeature * feature, DataBase * database, long layerindex, long databaseid) {
    // TODO: Add Support for FILTER_KIND
    
    if (m_AttributOperation == SIATTRIBUT_OPERATION_VALUE) {
        siAttributValue * myValue = m_Values.Item(0);
        if (myValue == NULL) {
            return false;
        }

        // INSERT INTO layer_at55 (OBJECT_ID, Status) VALUES (2,264) ON DUPLICATE KEY UPDATE Status=264
        wxString myQuery = _T("INSERT INTO layer_at%ld (OBJECT_ID, %s) VALUES (%ld,%ld) ON DUPLICATE KEY UPDATE %s=%ld");
        if (database->DataBaseQueryNoResults(wxString::Format(myQuery, layerindex, m_AttributNameOut, databaseid, myValue->GetValueOutCode(), m_AttributNameOut, myValue->GetValueOutCode()))==false) {
            return false;
        }
        return true;
    }
    
    
    
    if (m_AttributOperation == SIATTRIBUT_OPERATION_REPLACE) {
        // get field value
        wxString myFieldLatin1Value (feature->GetFieldAsString((const char *) m_AttributNameIn.mb_str(wxConvUTF8)));
        wxString myFieldValue (myFieldLatin1Value.wc_str(wxConvISO8859_1), wxConvUTF8);
        long myOutCode = _GetIDForValueIn(myFieldValue);
        if (myOutCode == wxNOT_FOUND) {
            wxLogError(_("ID not found for '%s'"), myFieldValue);
            return false;
        }
        
        // insert
        wxString myQuery = _T("INSERT INTO layer_at%ld (OBJECT_ID, %s) VALUES (%ld,%ld) ON DUPLICATE KEY UPDATE %s=%ld");
        if (database->DataBaseQueryNoResults(wxString::Format(myQuery, layerindex, m_AttributNameOut, databaseid, myOutCode, m_AttributNameOut, myOutCode))==false) {
            return false;
        }
        return true;
    }
    
    
    return false;
}


void siAttribut::Reset(){
    m_AttributNameIn = wxEmptyString;
    m_AttributNameOut = wxEmptyString;
    m_AttributFilterIDs.Clear();
    m_AttributOperation = SIATTRIBUT_OPERATION_UNKNOWN;
    _ClearAttributValueArray();
    m_AttributIDReal = wxNOT_FOUND;
}


wxArrayLong * siAttribut::GetAttributFilterIDsRef() {
    return &m_AttributFilterIDs;
}



#include <wx/arrimpl.cpp> // This is a magic incantation which must be done!
WX_DEFINE_OBJARRAY(siAttributArray);

WX_DEFINE_OBJARRAY(siAttributValueArray);