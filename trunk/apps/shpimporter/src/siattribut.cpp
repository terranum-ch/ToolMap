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

siAttributValue::siAttributValue() {
}



siAttributValue::~siAttributValue() {
}










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




siAttribut::siAttribut() {
    Reset();
}




siAttribut::~siAttribut() {
    _ClearAttributValueArray();
}



bool siAttribut::LoadFromArray(const wxArrayString & attribtxt) {
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
    
    

    return true;
}



void siAttribut::Reset(){
    m_AttributNameIn = wxEmptyString;
    m_AttributNameOut = wxEmptyString;
    m_AttributFilterIDs.Clear();
    m_AttributOperation = SIATTRIBUT_OPERATION_UNKNOWN;
    _ClearAttributValueArray();
}



#include <wx/arrimpl.cpp> // This is a magic incantation which must be done!
WX_DEFINE_OBJARRAY(siAttributArray);

WX_DEFINE_OBJARRAY(siAttributValueArray);