/***************************************************************************
								tmgisimport.cpp
						Import data into project
                             -------------------
    copyright            : (C) 2007 CREALP Lucien Schreiber 
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

// comment doxygen

#include "tmgisimport.h"

tmGISImport::tmGISImport()
{
	InitMember();
}



tmGISImport::~tmGISImport()
{
	
}

void tmGISImport::InitMember()
{
	m_Vector = NULL;
	m_Time = 0;
}

bool tmGISImport::IsOpen ()
{
	if (m_Vector == NULL)
		return false;
	return true;
}

bool tmGISImport::Open (const wxString & filename)
{
	m_Vector = NULL;
	m_Time = 0;
	
	// check if file exists
	wxFileName myFileName = filename;
	if(myFileName.FileExists() == false)
	{
		wxLogDebug(_T("File %s doesn't exist"), myFileName.GetFullName().c_str());
		return false;
	}
	
	
	// check for correct type (spatial file)
	m_Vector = tmGISDataVector::CreateGISVectorBasedOnExt(myFileName.GetExt());
	if (m_Vector == NULL)
	{
		wxLogDebug(_T("File format not supported"));
		return false;
	}
	
	if (m_Vector->Open(filename, true)==false)
	{
		delete m_Vector;
		m_Vector = NULL;
		return false;
	}
	
	return true;
}




TM_GIS_SPATIAL_TYPES tmGISImport::GetSpatialType ()
{
	if (IsOpen()==false)
		return LAYER_SPATIAL_UNKNOWN;
	return  m_Vector->GetSpatialType();
}



long tmGISImport::GetFeatureCount()
{
	if (IsOpen() == false)
		return wxNOT_FOUND;
	
	return m_Vector->GetCount();
}



bool tmGISImport::IsImportAllowed()
{
	if (GetSpatialType() == LAYER_SPATIAL_LINE || 
		GetSpatialType() == LAYER_SPATIAL_POINT)
		return true;
	
	return false;
}


bool tmGISImport::IsImportIntoAllowed (const TOC_GENERIC_NAME & importintotype)
{
	if (IsOpen() == false)
	{
		wxLogDebug(_T("Open a file first. Otherwise import isn't allowed"));
		return false;
	}
	
	if (IsImportAllowed() == false)
		return false;
	
	bool bReturn = false;
	TM_GIS_SPATIAL_TYPES myType = GetSpatialType();
	if (myType == LAYER_SPATIAL_LINE)
	{
		if (importintotype == TOC_NAME_LINES || 
			importintotype == TOC_NAME_FRAME)
			bReturn = true;
	}
	
	if (myType == LAYER_SPATIAL_POINT)
	{
		if (importintotype == TOC_NAME_POINTS || 
			importintotype == TOC_NAME_LABELS ||
			importintotype == TOC_NAME_ANNOTATIONS)
			bReturn = true;
	}
	
	return bReturn;
}


bool tmGISImport::Import(DataBaseTM * projectdb, const TOC_GENERIC_NAME & importintotype, wxProgressDialog * progress)
{
	m_Time = 0;
	
	if (IsImportIntoAllowed(importintotype)==false)
	{
		wxLogDebug(_T("Importing into %s not possible"), 
				   TOC_GENERIC_NAME_STRING[importintotype].c_str());
		return false;
	}
	
	if (projectdb == NULL)
		return false;
	
	
	tmGISDataVectorMYSQL * myGeomDB = new tmGISDataVectorMYSQL();
	tmGISDataVectorMYSQL::SetDataBaseHandle(projectdb);
	
	long iCount = 0;
	wxStopWatch sv;
	tmPercent tpercent(GetFeatureCount());

	while (1)
	{
		OGRFeature * myFeature = m_Vector->GetNextFeature();
		if (myFeature == NULL){
			break; 
		}
		OGRGeometry * myGeom = myFeature->GetGeometryRef();
		wxASSERT(myGeom);
		
		if (myGeomDB->AddGeometry(myGeom, -1, importintotype) == wxNOT_FOUND)
		{
			OGRFeature::DestroyFeature(myFeature);
			wxLogError(_("Error importing geometry into the project"));
			break;
		}
		iCount ++;
		OGRFeature::DestroyFeature(myFeature);
		
		bool bStop = false;
		tpercent.SetValue(iCount);
		if (tpercent.IsNewStep())
			if (progress)
				if(progress->Update(tpercent.GetPercent(), wxEmptyString)==false)
					bStop=true;
		
		if (bStop)
		{
			wxLogMessage(_("Adding gis data into project stopped by user."));
			break;
		}
	}
	
	sv.Pause();
	m_Time = sv.Time();
	
	wxLogMessage (_("%d feature added"), iCount);
	return true;
	
}


long tmGISImport::GetElapsedTime()
{
	return m_Time;
}

