/***************************************************************************
 tmimport.h
 
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

#ifndef _TMIMPORT_H
#define _TMIMPORT_H

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/filename.h>
#include <wx/progdlg.h>

#include "tmlayerpropertiesdef.h"


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

public:
    tmImport();

    virtual ~tmImport();

    virtual bool Open(const wxFileName &filename);

    virtual bool IsOk();

    virtual bool Import(DataBaseTM *database, wxProgressDialog *progress = NULL)
    { return false; }

    inline const wxFileName GetFileName() const;

    inline const tmImportFileType GetFileType() const;

    inline const TM_GIS_SPATIAL_TYPES GetGeometryType() const;

    inline const long GetFeatureCount() const;

    inline const int GetFieldCount() const;

    inline const TOC_GENERIC_NAME GetTarget() const;

    void SetTarget(TOC_GENERIC_NAME value);

    virtual wxArrayInt GetTargetSupported()
    { return wxArrayInt(); }

    virtual wxArrayString GetTargetSupportedName();

    void SetLayerName(const wxString &value);
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
