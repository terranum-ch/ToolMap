/***************************************************************************
 tmimportgis.cpp
 
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


#include "tmimportgis.h"
#include "../database/database_tm.h"
#include "../core/tmpercent.h"


tmImportGIS::tmImportGIS() {
	m_Vector = NULL;
	m_FileType = tmIMPORT_TYPE_SHP;
}

tmImportGIS::~tmImportGIS() {
	wxDELETE(m_Vector);
}

bool tmImportGIS::Open(const wxFileName & filename) {
	if(tmImport::Open(filename)==false){
		return false;
	}
	
	m_Vector = tmGISDataVector::CreateGISVectorBasedOnExt(m_FileName.GetExt());
	if (m_Vector == NULL)
	{
		wxLogError(_("File format not supported"));
		return false;
	}
	
	if (m_Vector->Open(m_FileName.GetFullPath(), true)==false)
	{
		wxDELETE(m_Vector);
		return false;
	}
	
	m_GeometryType = m_Vector->GetSpatialType();
	m_FeatureCount = m_Vector->GetCount();
	
	// TODO: Implement field count
	m_FieldsCount = m_Vector->GetFieldsCount(); 
	return true;
}



bool tmImportGIS::Import(DataBaseTM * database, wxProgressDialog * progress) {
	wxASSERT(database);
	if (IsOk() == false) {
		wxLogError(_("Importation failed"));
		return false;
	}
	
	tmGISDataVectorMYSQL * myGeomDB = new tmGISDataVectorMYSQL();
	tmGISDataVectorMYSQL::SetDataBaseHandle(database);

	long iCount = 0;
	wxStopWatch sv;
	tmPercent tpercent(GetFeatureCount());

	while (1){
		OGRFeature * myFeature = m_Vector->GetNextFeature();
		if (myFeature == NULL){
			break; 
		}
		OGRGeometry * myGeom = myFeature->GetGeometryRef();
		wxASSERT(myGeom);
        
        // import multi objects
        OGRwkbGeometryType myGeomType = wkbFlatten(myGeom->getGeometryType());        
        if (myGeomType == wkbMultiPoint || myGeomType == wkbMultiLineString){
            OGRGeometryCollection * myCollection = static_cast<OGRGeometryCollection*>(myGeom);
            for (unsigned int i = 0; i< myCollection->getNumGeometries(); i++) {
                OGRGeometry * myCollGeom = myCollection->getGeometryRef(i);
                if (myGeomDB->AddGeometry(myCollGeom, -1, GetTarget()) == wxNOT_FOUND){
                    wxLogError(_("Error importing geometry (from multi-geometries) into project"));
                    continue;
                }
                ++iCount;
            }
        }
        // import single object
        else {
            if (myGeomDB->AddGeometry(myGeom, -1, GetTarget()) == wxNOT_FOUND){
                OGRFeature::DestroyFeature(myFeature);
                wxLogError(_("Error importing geometry into the project"));
                break;
            }
        }
		++iCount;
		OGRFeature::DestroyFeature(myFeature);
		
		bool bStop = false;
		tpercent.SetValue(iCount);
		if (tpercent.IsNewStep() && progress != NULL){
			if(progress->Update(tpercent.GetPercent(), wxEmptyString)==false){
					bStop=true;
			}
		}
		
		if (bStop){
			wxLogMessage(_("Adding gis data into project stopped by user."));
			break;
		}
	}
	
	sv.Pause();
	wxLogMessage (_("%ld feature added in %ld [ms]"), iCount, sv.Time());
	wxDELETE(myGeomDB);
	return true;
}


wxArrayInt tmImportGIS::GetTargetSupported() {
	wxArrayInt mySupportedTargets;
	switch (GetGeometryType()) {
		case LAYER_SPATIAL_LINE:
			mySupportedTargets.Add((int) TOC_NAME_LINES);
			mySupportedTargets.Add((int) TOC_NAME_FRAME);
			break;
			
		case LAYER_SPATIAL_POINT:
			mySupportedTargets.Add((int) TOC_NAME_POINTS);
			mySupportedTargets.Add((int) TOC_NAME_LABELS);
			break;
		default:
			break;
	}
	return mySupportedTargets;
}




