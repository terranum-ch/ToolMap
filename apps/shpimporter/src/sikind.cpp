/***************************************************************************
 sikind.cpp
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

#include "sikind.h"
#include "siparam.h"

siKind::siKind() {
    Reset();
}


siKind::~siKind() {
}


void siKind::Reset(){
    m_KindNameIn = wxEmptyString;
    m_CodesIn.Clear();
    m_CodesOut2.Clear();
    m_CodesRealOut.Clear();
    m_Database = NULL;
}


bool siKind::_LoadKindValue(const wxString & kindtxt) {
    siParam myParam;
    bool bError = false;
    
    long myCodeIn = wxNOT_FOUND;
    myParam.GetParamByCol(kindtxt, 0, bError).ToLong(&myCodeIn);
    if (bError == true) {
        return false;
    }
    
    wxString myCodeOut = myParam.GetParamByCol(kindtxt, 1, bError);
    if (bError == true) {
        return false;
    }
    
    // get real database ID;
    wxASSERT(m_Database);
    if (m_Database->DataBaseQuery(wxString::Format(_T("SELECT OBJECT_ID FROM dmn_Layer_object WHERE OBJECT_CD = \"%s\""), myCodeOut))==false) {
        return false;
    }
    DataBaseResult myResult;
    m_Database->DataBaseGetResults(&myResult);
    if (myResult.GetRowCount() != 1) {
        return false;
    }
    
    myResult.NextRow();
    long myCodeOutReal = wxNOT_FOUND;
    myResult.GetValue(0, myCodeOutReal);
    
    m_CodesIn.Add(myCodeIn);
    m_CodesOut2.Add(myCodeOut);
    m_CodesRealOut.Add(myCodeOutReal);
    wxLogDebug(_("Adding Kind code: %ld -> %s (Real ID: %ld)"), myCodeIn, myCodeOut, myCodeOutReal);
    return true;
}


bool siKind::LoadFromArray(const wxArrayString & kindtxt, DataBase * database) {
    // line 0 is fieldName and then next lines are codes!
    Reset();
    m_Database = database;
    if (kindtxt.GetCount() == 0) {
        return false;
    }
    
    siParam myParam;
    bool bError = false;
    m_KindNameIn = myParam.GetParam(kindtxt.Item(0), _T("KIND_IN"), bError);
    if (bError == true) {
        wxLogError(_("Unable to get KIND_IN"));
        return false;
    }
    wxLogMessage(_("KIND_IN field is %s"), m_KindNameIn);
    
    // get kind values line 1 to GetCount(), and search real kind id;
    for (unsigned int i = 1; i< kindtxt.GetCount(); i++) {
        if (_LoadKindValue(kindtxt.Item(i))==false) {
            wxLogError(_("Error loading kind value from: %s"), kindtxt.Item(i));
        }
    }
    return true;
}


long siKind::GetRealKind(long kindin) {
    for (unsigned int i = 0; i < m_CodesIn.GetCount(); i++) {
        if (m_CodesIn.Item(i) == kindin) {
            return m_CodesRealOut.Item(i);
        }
    }
    return wxNOT_FOUND;
}

