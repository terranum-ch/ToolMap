/***************************************************************************
 tmimport.h
 
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

#ifndef _TMIMPORT_H
#define _TMIMPORT_H

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/filename.h>
#include <wx/progdlg.h>

#include "tmlayerpropertiesdef.h"
#include "../core/projectdefmemory.h"
#include "../core/prjdefmemmanage.h"


class DataBaseTM;


enum tmImportFileType
{
    tmIMPORT_TYPE_SHP = 0,
    tmIMPORT_TYPE_CSV

};


class tmImport
{
protected:
    wxFileName m_FileName;
    tmImportFileType m_FileType;
    TM_GIS_SPATIAL_TYPES m_GeometryType;
    long m_FeatureCount;
    int m_FieldsCount;
    TOC_GENERIC_NAME m_ImportTarget;
    wxString m_LayerName;
    wxString m_FieldKind;
    wxArrayString m_SkipKinds;
    wxArrayString m_FileKinds;
    wxArrayString m_DbKinds;
    wxArrayString m_FileAttributes;
    wxArrayString m_DbAttributes;
    wxArrayInt m_AttributeTypes;
    wxArrayString m_FileEnumsAttName;
    wxArrayString m_FileEnums;
    wxArrayString m_DbEnums;

    bool ShouldSkipObjectKind(const wxArrayString &fileValues);

    bool SetObjectKind(DataBaseTM *database, PrjDefMemManage *prj, const wxArrayString &fileValues, const wxArrayLong &oids);

    bool SetAttributes(DataBaseTM *database, PrjDefMemManage *prj, const wxArrayString &fileValues, wxArrayLong &oids);

public:
    tmImport();

    virtual ~tmImport();

    virtual bool Open(const wxFileName &filename);

    virtual bool IsOk();

    virtual bool Import(DataBaseTM *database, PrjDefMemManage *prj, wxProgressDialog *progress = NULL)
    { return false; }

    virtual bool GetExistingAttributeValues(const wxString &attName, wxArrayString &values)
    { return false; }

    inline const wxFileName GetFileName() const;

    inline const tmImportFileType GetFileType() const;

    inline const TM_GIS_SPATIAL_TYPES GetGeometryType() const;

    inline const long GetFeatureCount() const;

    inline const int GetFieldCount() const;

    virtual bool GetFieldNames(wxArrayString &Fields)
    { return false; }

    inline const TOC_GENERIC_NAME GetTarget() const;

    void SetTarget(TOC_GENERIC_NAME value);

    virtual wxArrayInt GetTargetSupported()
    { return wxArrayInt(); }

    virtual wxArrayString GetTargetSupportedName();

    void SetLayerName(const wxString &value);

    wxString GetLayerName();

    void SetFieldKind(const wxString &value);

    wxString GetFieldKind();

    void SkipObjectKind(const wxString &fileKind);

    void AddObjectKindMatch(const wxString &fileKind, const wxString &dbKind);

    void ClearObjectKindMatches();

    void AddAttributeMatch(const wxString &fileAttribute, const wxString &dbAttribute, PRJDEF_FIELD_TYPE type);

    void ClearAttributeMatches();

    void AddEnumerationMatch(const wxString &attributeName, const wxString &fileEnum, const wxString &dbEnum);

    void ClearEnumerationMatches();

    int GetAttributesMatchesCount() const;

    bool AttributeIsEnum(int index) const;

    wxString GetAttributeNameInDB(int index) const;

    wxString GetAttributeNameInFile(int index) const;

    bool HasEnumAttributes() const;

};


inline const wxFileName tmImport::GetFileName() const
{
    return m_FileName;
}


inline const tmImportFileType tmImport::GetFileType() const
{
    return m_FileType;
}


inline const TM_GIS_SPATIAL_TYPES tmImport::GetGeometryType() const
{
    return m_GeometryType;
}


inline const long tmImport::GetFeatureCount() const
{
    return m_FeatureCount;
}


inline const int tmImport::GetFieldCount() const
{
    return m_FieldsCount;
}

inline const TOC_GENERIC_NAME tmImport::GetTarget() const
{
    return m_ImportTarget;
}

#endif
