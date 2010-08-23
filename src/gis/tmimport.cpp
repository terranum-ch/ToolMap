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

tmImport::tmImport() {
	m_FileName = wxFileName();
	m_FileType = tmIMPORT_TYPE_SHP;
	m_GeometryType = LAYER_SPATIAL_UNKNOWN;
	m_FeatureCount = wxNOT_FOUND;
	m_FieldsCount = wxNOT_FOUND;
	m_ImportTarget = TOC_NAME_UNKNOWN;
}



tmImport::~tmImport() {
}



bool tmImport::Open(const wxFileName & filename) {
	// check if file exists
	wxFileName myFileName = filename;
	if(myFileName.FileExists() == false)
	{
		wxLogError(_("File '%s' doesn't exist"), myFileName.GetFullName().c_str());
		return false;
	}
	m_FileName = filename;
	return true;
}



bool tmImport::IsOk() {
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



void tmImport::SetTarget(TOC_GENERIC_NAME value) {
  m_ImportTarget = value;
}




