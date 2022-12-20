/***************************************************************************
 tmimportgis.cpp
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

#include "tmimportgis.h"

#include <ogr_feature.h>

#include "../core/tmpercent.h"
#include "../database/database_tm.h"

tmImportGIS::tmImportGIS() {
    m_Vector = nullptr;
    m_FileType = tmIMPORT_TYPE_SHP;
}

tmImportGIS::~tmImportGIS() {
    wxDELETE(m_Vector);
}

bool tmImportGIS::Open(const wxFileName& filename) {
    if (!tmImport::Open(filename)) {
        return false;
    }

    m_Vector = tmGISDataVector::CreateGISVectorBasedOnExt(m_FileName.GetExt());
    if (m_Vector == nullptr) {
        wxLogError(_("File format not supported"));
        return false;
    }

    if (!m_Vector->Open(m_FileName.GetFullPath(), true)) {
        wxDELETE(m_Vector);
        return false;
    }

    m_GeometryType = m_Vector->GetSpatialType();
    m_FeatureCount = m_Vector->GetCount();

    // TODO: Implement field count
    m_FieldsCount = m_Vector->GetFieldsCount();
    return true;
}

bool tmImportGIS::Import(DataBaseTM* database, PrjDefMemManage* prj, wxProgressDialog* progress) {
    wxASSERT(database);
    if (!IsOk()) {
        wxLogError(_("Importation failed"));
        return false;
    }

    tmGISDataVectorMYSQL* myGeomDB = new tmGISDataVectorMYSQL();
    tmGISDataVectorMYSQL::SetDataBaseHandle(database);

    long iCount = 0;
    wxStopWatch sv;
    tmPercent tpercent(GetFeatureCount());

    m_Vector->ResetReading();

    while (true) {
        OGRFeature* myFeature = m_Vector->GetNextFeature();
        if (myFeature == nullptr) {
            break;
        }

        wxArrayString fileValues;
        for (int i = 0; i < myFeature->GetFieldCount(); ++i) {
            const char* val = myFeature->GetFieldAsString(i);
            wxString sVal(val, wxConvUTF8);
            fileValues.Add(sVal);
        }

        if (ShouldSkipObjectKind(fileValues)) {
            continue;
        }

        OGRGeometry* myGeom = myFeature->GetGeometryRef();
        wxASSERT(myGeom);
        wxArrayLong oids;

        OGRwkbGeometryType myGeomType = wkbFlatten(myGeom->getGeometryType());
        if (myGeomType == wkbMultiPoint || myGeomType == wkbMultiLineString) {
            // import multi objects
            auto* myCollection = dynamic_cast<OGRGeometryCollection*>(myGeom);
            for (int i = 0; i < myCollection->getNumGeometries(); i++) {
                OGRGeometry* myCollGeom = myCollection->getGeometryRef(i);
                long oid = myGeomDB->AddGeometry(myCollGeom, -1, GetTarget());
                if (oid == wxNOT_FOUND) {
                    wxLogError(_("Error importing geometry (from multi-geometries) into project"));
                    continue;
                }
                oids.Add(oid);
                ++iCount;
            }
        } else {
            // import single object
            long oid = myGeomDB->AddGeometry(myGeom, -1, GetTarget());
            if (oid == wxNOT_FOUND) {
                OGRFeature::DestroyFeature(myFeature);
                wxLogError(_("Error importing geometry into the project"));
                break;
            }
            oids.Add(oid);
        }

        wxASSERT(!oids.IsEmpty());

        if (m_ImportTarget != TOC_NAME_FRAME && prj != nullptr) {
            if (!SetObjectKind(database, prj, fileValues, oids)) {
                break;
            }

            if (!SetAttributes(database, prj, fileValues, oids)) {
                break;
            }
        }

        ++iCount;
        OGRFeature::DestroyFeature(myFeature);

        bool bStop = false;
        tpercent.SetValue(iCount);
        if (tpercent.IsNewStep() && progress != nullptr) {
            if (!progress->Update(tpercent.GetPercent(), wxEmptyString)) {
                bStop = true;
            }
        }

        if (bStop) {
            wxLogMessage(_("Adding gis data into project stopped by user."));
            break;
        }
    }

    sv.Pause();
    wxLogMessage(_("%ld feature added in %ld [ms]"), iCount, sv.Time());
    wxDELETE(myGeomDB);
    return true;
}

bool tmImportGIS::GetExistingAttributeValues(const wxString& attName, wxArrayString& values) {
    m_Vector->ResetReading();

    while (true) {
        OGRFeature* myFeature = m_Vector->GetNextFeature();
        if (myFeature == nullptr) {
            break;
        }

        const char* val = myFeature->GetFieldAsString(attName.c_str());
        wxString fieldVal(val, wxConvUTF8);

        if (!fieldVal.IsEmpty()) {
            bool isNew = true;
            for (int i = 0; i < values.GetCount(); ++i) {
                if (fieldVal.IsSameAs(values.Item(i), false)) {
                    isNew = false;
                    break;
                }
            }

            if (isNew) {
                values.Add(fieldVal);
            }
        }

        OGRFeature::DestroyFeature(myFeature);
    }

    return true;
}

wxArrayInt tmImportGIS::GetTargetSupported() {
    wxArrayInt mySupportedTargets;
    switch (GetGeometryType()) {
        case LAYER_SPATIAL_LINE:
            mySupportedTargets.Add((int)TOC_NAME_LINES);
            mySupportedTargets.Add((int)TOC_NAME_FRAME);
            break;

        case LAYER_SPATIAL_POINT:
            mySupportedTargets.Add((int)TOC_NAME_POINTS);
            mySupportedTargets.Add((int)TOC_NAME_LABELS);
            break;
        default:
            break;
    }
    return mySupportedTargets;
}

bool tmImportGIS::GetFieldNames(wxArrayString& Fields) {
    return m_Vector->GetFieldsName(Fields);
}
