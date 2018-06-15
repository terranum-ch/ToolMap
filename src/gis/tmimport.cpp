/***************************************************************************
 tmimport.cpp
 
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


#include "tmimport.h"

tmImport::tmImport()
{
    m_FileName = wxFileName();
    m_FileType = tmIMPORT_TYPE_SHP;
    m_GeometryType = LAYER_SPATIAL_UNKNOWN;
    m_FeatureCount = wxNOT_FOUND;
    m_FieldsCount = wxNOT_FOUND;
    m_ImportTarget = TOC_NAME_UNKNOWN;
}


tmImport::~tmImport()
{
}


bool tmImport::Open(const wxFileName &filename)
{
    // check if file exists
    wxFileName myFileName = filename;
    if (myFileName.FileExists() == false) {
        wxLogError(_("File '%s' doesn't exist"), myFileName.GetFullName().c_str());
        return false;
    }
    m_FileName = filename;
    return true;
}


bool tmImport::IsOk()
{
    if (m_FileName.IsOk() == false) {
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


void tmImport::SetTarget(TOC_GENERIC_NAME value)
{
    m_ImportTarget = value;
}


wxArrayString tmImport::GetTargetSupportedName()
{
    wxArrayInt mySupportedTargets = GetTargetSupported();
    wxArrayString mySupportedTargetsName;
    for (unsigned int i = 0; i < mySupportedTargets.GetCount(); i++) {
        mySupportedTargetsName.Add(TOC_GENERIC_NAME_STRING[mySupportedTargets.Item(i)]);
    }
    return mySupportedTargetsName;
}


void tmImport::SetLayerName(const wxString &value)
{
    m_LayerName = value;
}

wxString tmImport::GetLayerName()
{
    return m_LayerName;
}

void tmImport::AddAttribute(const wxString &fileAttribute, const wxString &dbAttribute, PRJDEF_FIELD_TYPE type)
{
    m_FileAttributes.Add(fileAttribute);
    m_DbAttributes.Add(dbAttribute);
    m_AttributeTypes.Add(type);
}

void tmImport::ClearAttributes()
{
    m_FileAttributes.Clear();
    m_DbAttributes.Clear();
    m_AttributeTypes.Clear();
}

int tmImport::GetAttributesCount() const
{
    return (int) m_FileAttributes.GetCount();
}

bool tmImport::AttributeIsEnum(int index) const
{
    wxASSERT(m_AttributeTypes.GetCount() > index);

    return (PRJDEF_FIELD_TYPE) m_AttributeTypes.Item(index) == TM_FIELD_ENUMERATION;
}

wxString tmImport::GetAttributeNameInDB(int index) const
{
    wxASSERT(m_DbAttributes.GetCount() > index);

    return m_DbAttributes.Item(index);
}

wxString tmImport::GetAttributeNameInFile(int index) const
{
    wxASSERT(m_FileAttributes.GetCount() > index);

    return m_FileAttributes.Item(index);
}

bool tmImport::HasEnumAttributes() const
{
    for (int i = 0; i < m_AttributeTypes.GetCount(); ++i) {
        if (AttributeIsEnum(i)) {
            return true;
        }
    }
    return false;
}