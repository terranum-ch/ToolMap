/***************************************************************************
 tmpointorientattrib.cpp
 Point orientation with attribution
 -------------------
 copyright : (C) 2007 CREALP Lucien Schreiber
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

// comment doxygen

#include "tmpointorientattrib.h"

tmPointOrientAttrib::tmPointOrientAttrib() {
    InitMemberValues();
}

void tmPointOrientAttrib::InitMemberValues() {
    m_Oid = wxNOT_FOUND;
    m_DB = nullptr;
    m_LayerId = wxNOT_FOUND;
}

void tmPointOrientAttrib::Create(DataBaseTM* database, long oid) {
    m_Oid = oid;
    m_DB = database;
}

bool tmPointOrientAttrib::POA_IsOIDInited() {
    if (m_Oid == wxNOT_FOUND) {
        wxLogError(_T("No point selected for orientation"));
        return false;
    }

    if (m_DB == nullptr) {
        wxLogError(_("Database not inited"));
        return false;
    }

    return true;
}

bool tmPointOrientAttrib::POA_IsAttributed(long& attributedvalue) {
    wxASSERT(m_DB);

    // get attribution
    wxString sSentence = _T("SELECT OBJECT_VAL_ID FROM ") + TABLE_NAME_GIS_ATTRIBUTION[TOC_NAME_POINTS] +
                         _T(" WHERE OBJECT_GEOM_ID=") + wxString::Format(_T("%ld"), m_Oid);

    if (!m_DB->DataBaseQuery(sSentence)) return false;

    wxArrayLong myAttributedValues;
    m_DB->DataBaseGetResults(myAttributedValues);

    if (myAttributedValues.GetCount() != 1) {
        wxLogError(_T("No attributs / too much attributs for selected object"));
        return false;
    }

    attributedvalue = myAttributedValues.Item(0);
    return true;
}

tmPointOrientAttrib::~tmPointOrientAttrib() {}

bool tmPointOrientAttrib::POA_HasOrientField() {
    wxASSERT(m_DB);

    // get layer id;
    wxString sSentence = wxString::Format(
        _T("SELECT o.THEMATIC_LAYERS_LAYER_INDEX FROM ")
        _T("%s o LEFT JOIN %s p ON (o.OBJECT_ID = p.OBJECT_VAL_ID) ")
        _T("WHERE p.OBJECT_GEOM_ID=%ld"),
        TABLE_NAME_OBJECTS.c_str(), TABLE_NAME_GIS_ATTRIBUTION[TOC_NAME_POINTS].c_str(), m_Oid);
    if (!m_DB->DataBaseQuery(sSentence)) return false;

    m_LayerId = wxNOT_FOUND;
    if (!m_DB->DataBaseGetNextResult(m_LayerId)) {
        wxLogError(_("No layer found, object not attributed ?"));
        return false;
    }
    wxLogDebug(_T("Layer found is : %ld"), m_LayerId);
    m_DB->DataBaseClearResults();
    wxASSERT(m_LayerId != wxNOT_FOUND);

    // get field info
    PrjMemFieldArray myFields;
    ProjectDefMemoryLayers myActualLayer;
    myActualLayer.m_LayerID = m_LayerId;

    if (!m_DB->GetFields(myFields, &myActualLayer)) {
        wxLogError(_("No orientation field found"));
        return false;
    }
    m_DB->DataBaseClearResults();

    bool bFound = false;
    for (unsigned int i = 0; i < myFields.GetCount(); i++) {
        if (myFields.Item(i)->m_FieldOrientation) {
            bFound = true;
            m_OrientField = *(myFields.Item(i));
        }
    }

    if (!bFound) {
        wxLogError(_("No orientation field found"));
        return false;
    }

    wxLogDebug(_T("Orientation Fields found %s"), m_OrientField.m_Fieldname.c_str());
    return true;
}

bool tmPointOrientAttrib::IsCorrectType() {
    if (!POA_IsOIDInited()) return false;

    long myAttribValue = wxNOT_FOUND;
    if (!POA_IsAttributed(myAttribValue)) return false;

    if (!POA_HasOrientField()) return false;

    // checks for
    return true;
}

bool tmPointOrientAttrib::Update() {
    if (!POA_IsOIDInited()) return false;

    if (m_LayerId == wxNOT_FOUND) {
        wxLogDebug(_T("Use IsCorrectType() first"));
        return false;
    }

    int myOrientI = GetOrientationInt();
    double myOrientD = GetOrientationDouble();
    if (myOrientD == wxNOT_FOUND) {
        wxLogError(_T("Unable to update orientation, no orientation computed"));
        return false;
    }

    wxString sSentence = wxString::Format(_T("INSERT INTO %s%ld (OBJECT_ID, %s) VALUES (%ld, "),
                                          TABLE_NAME_LAYER_AT.c_str(), m_LayerId, m_OrientField.m_Fieldname.c_str(),
                                          m_Oid);

    switch (m_OrientField.m_FieldType) {
        case TM_FIELD_INTEGER:
            sSentence.Append(wxString::Format(_T("%d) ON DUPLICATE KEY UPDATE %s=%d;"), myOrientI,
                                              m_OrientField.m_Fieldname.c_str(), myOrientI));
            break;
        case TM_FIELD_FLOAT:
            sSentence.Append(wxString::Format(_T("%g) ON DUPLICATE KEY UPDATE %s=%g;"), myOrientD,
                                              m_OrientField.m_Fieldname.c_str(), myOrientD));
            break;

        default:
            wxASSERT_MSG(1, _T("Incorrect field stored, something wrong"));
            break;
    };

    if (!m_DB->DataBaseQueryNoResults(sSentence)) return false;

    return true;
}
