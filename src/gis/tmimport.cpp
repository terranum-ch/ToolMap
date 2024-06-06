/***************************************************************************
 tmimport.cpp
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

#include "tmimport.h"

#include "../gis/tmattributiondata.h"
#include "../gis/tmattributionmanager.h"

tmImport::tmImport() {
    m_FileName = wxFileName();
    m_FileType = tmIMPORT_TYPE_SHP;
    m_GeometryType = LAYER_SPATIAL_UNKNOWN;
    m_FeatureCount = wxNOT_FOUND;
    m_FieldsCount = wxNOT_FOUND;
    m_ImportTarget = TOC_NAME_UNKNOWN;
}

tmImport::~tmImport() {}

bool tmImport::Open(const wxFileName& filename) {
    // check if file exists
    wxFileName myFileName = filename;
    if (!myFileName.FileExists()) {
        wxLogError(_("File '%s' doesn't exist"), myFileName.GetFullName().c_str());
        return false;
    }
    m_FileName = filename;
    return true;
}

bool tmImport::IsOk() {
    if (!m_FileName.IsOk()) {
        return false;
    }

    if (m_GeometryType == LAYER_SPATIAL_UNKNOWN) {
        return false;
    }

    if (m_FeatureCount == wxNOT_FOUND) {
        return false;
    }

    if (m_ImportTarget == TOC_NAME_UNKNOWN) {
        return false;
    }

    return true;
}

void tmImport::SetTarget(TOC_GENERIC_NAME value) {
    m_ImportTarget = value;
}

wxArrayString tmImport::GetTargetSupportedName() {
    wxArrayInt mySupportedTargets = GetTargetSupported();
    wxArrayString mySupportedTargetsName;
    for (unsigned int i = 0; i < mySupportedTargets.GetCount(); i++) {
        mySupportedTargetsName.Add(TOC_GENERIC_NAME_STRING[mySupportedTargets.Item(i)]);
    }
    return mySupportedTargetsName;
}

void tmImport::SetLayerName(const wxString& value) {
    m_LayerName = value;
}

wxString tmImport::GetLayerName() {
    return m_LayerName;
}

void tmImport::SetFieldKind(const wxString& value) {
    m_FieldKind = value;
}

wxString tmImport::GetFieldKind() {
    return m_FieldKind;
}

void tmImport::SkipObjectKind(const wxString& fileKind) {
    m_SkipKinds.Add(fileKind);
}

void tmImport::AddObjectKindMatch(const wxString& fileKind, const wxString& dbKind) {
    m_FileKinds.Add(fileKind);
    m_DbKinds.Add(dbKind);
}

void tmImport::ClearObjectKindMatches() {
    m_FileKinds.Clear();
    m_DbKinds.Clear();
}

void tmImport::AddAttributeMatch(const wxString& fileAttribute, const wxString& dbAttribute, PRJDEF_FIELD_TYPE type) {
    m_FileAttributes.Add(fileAttribute);
    m_DbAttributes.Add(dbAttribute);
    m_AttributeTypes.Add(type);
}

void tmImport::ClearAttributeMatches() {
    m_FileAttributes.Clear();
    m_DbAttributes.Clear();
    m_AttributeTypes.Clear();
}

void tmImport::AddEnumerationMatch(const wxString& attributeName, const wxString& fileEnum, const wxString& dbEnum) {
    m_FileEnumsAttName.Add(attributeName);
    m_FileEnums.Add(fileEnum);
    m_DbEnums.Add(dbEnum);
}

void tmImport::ClearEnumerationMatches() {
    m_FileEnumsAttName.Clear();
    m_FileEnums.Clear();
    m_DbEnums.Clear();
}

int tmImport::GetAttributesMatchesCount() const {
    return (int)m_FileAttributes.GetCount();
}

bool tmImport::AttributeIsEnum(int index) const {
    wxASSERT(m_AttributeTypes.GetCount() > index);

    return (PRJDEF_FIELD_TYPE)m_AttributeTypes.Item(index) == TM_FIELD_ENUMERATION;
}

wxString tmImport::GetAttributeNameInDB(int index) const {
    wxASSERT(m_DbAttributes.GetCount() > index);

    return m_DbAttributes.Item(index);
}

wxString tmImport::GetAttributeNameInFile(int index) const {
    wxASSERT(m_FileAttributes.GetCount() > index);

    return m_FileAttributes.Item(index);
}

bool tmImport::HasEnumAttributes() const {
    for (int i = 0; i < m_AttributeTypes.GetCount(); ++i) {
        if (AttributeIsEnum(i)) {
            return true;
        }
    }
    return false;
}

bool tmImport::ShouldSkipObjectKind(const wxArrayString& fileValues) {
    // Get field names
    wxArrayString fields;
    GetFieldNames(fields);

    wxString kind;

    // Loop over the file header
    for (int i = 0; i < fields.GetCount(); ++i) {
        if (fields.Item(i).IsSameAs(m_FieldKind)) {
            wxASSERT(fileValues.GetCount() > i);
            for (int j = 0; j < m_SkipKinds.GetCount(); ++j) {
                if (fileValues.Item(i).IsSameAs(m_SkipKinds.Item(j))) {
                    return true;
                }
            }
        }
    }

    return false;
}

bool tmImport::SetObjectKind(DataBaseTM* database, PrjDefMemManage* prj, const wxArrayString& fileValues,
                             const wxArrayLong& oids) {
    // Get field names
    wxArrayString fields;
    GetFieldNames(fields);

    wxString kind;

    if (m_FileKinds.GetCount() == 1 && m_FileKinds.Item(0).IsSameAs("*")) {
        wxASSERT(m_DbKinds.GetCount() == 1);
        kind = m_DbKinds.Item(0);
    } else if (m_FileKinds.GetCount() > 0) {
        // Loop over the file header
        for (int i = 0; i < fields.GetCount(); ++i) {
            if (fields.Item(i).IsSameAs(m_FieldKind)) {
                wxASSERT(fileValues.GetCount() > i);
                for (int j = 0; j < m_FileKinds.GetCount(); ++j) {
                    if (fileValues.Item(i).IsSameAs(m_FileKinds.Item(j))) {
                        kind = m_DbKinds.Item(j);
                    }
                }
            }
        }
    }

    if (!kind.IsEmpty()) {
        // Get kind ID
        ProjectDefMemoryObjects* obj = prj->FindObject(kind);
        wxASSERT(obj);

        for (auto oid : oids) {
            wxString cmd = wxString::Format(_T("INSERT INTO %s VALUES (%ld, %ld); "),
                                            TABLE_NAME_GIS_ATTRIBUTION[m_ImportTarget], obj->m_ObjectID, oid);

            if (!database->DataBaseQueryNoResults(cmd)) {
                wxLogError(_("Adding kind(s) to selected features failed!"));
                return false;
            }
        }
    }

    return true;
}

bool tmImport::SetAttributes(DataBaseTM* database, PrjDefMemManage* prj, const wxArrayString& fileValues,
                             wxArrayLong& oids) {
    // Get layer
    ProjectDefMemoryLayers* layer = prj->FindLayer(m_LayerName);
    if (layer == nullptr) return false;

    // Get field names
    wxArrayString fields;
    GetFieldNames(fields);

    // Create attribution object
    tmAttributionData* myAttribObj = tmAttributionManager::CreateAttributionData(m_ImportTarget);
    if (myAttribObj == nullptr) return false;

    // Object ID
    myAttribObj->Create(&oids, database);

    // Set attributes
    wxArrayString fieldValues;

    // Loop over the fields as defined in the project in memory
    for (unsigned int i = 0; i < layer->m_pLayerFieldArray.GetCount(); i++) {
        ProjectDefMemoryFields* mypField = layer->m_pLayerFieldArray.Item(i);
        wxASSERT(mypField);
        wxString fieldName = mypField->m_Fieldname;
        wxString fieldValue = wxEmptyString;

        // Loop over the fields from the DB matching the ones in the file
        for (int j = 0; j < m_DbAttributes.GetCount(); ++j) {
            if (m_DbAttributes.Item(j).IsSameAs(fieldName)) {
                // Loop over the file header
                for (int k = 0; k < fields.GetCount(); ++k) {
                    if (fields.Item(k).IsSameAs(m_FileAttributes.Item(j))) {
                        wxASSERT(fileValues.GetCount() > k);

                        // Enumeration matching if required
                        if (mypField->m_FieldType == TM_FIELD_ENUMERATION) {
                            // Loop over the enumeration matching
                            for (int l = 0; l < m_FileEnums.GetCount(); ++l) {
                                if (m_FileEnumsAttName.Item(l).IsSameAs(fieldName) &&
                                    m_FileEnums.Item(l).IsSameAs(fileValues[k], false)) {
                                    fieldValue = m_DbEnums.Item(l);
                                }
                            }
                        } else {
                            fieldValue = fileValues[k];
                        }
                    }
                }
            }
        }
        fieldValues.Add(fieldValue);
    }

    PrjMemLayersArray myLayersInfoArray;
    myLayersInfoArray.Add(layer);
    for (auto oid : oids) {
        if (!myAttribObj->SetAttributesAdvanced(oid, &myLayersInfoArray, fieldValues)) return false;
    }

    return true;
}
