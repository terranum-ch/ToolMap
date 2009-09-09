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
}

bool tmGISImport::IsOpen ()
{
	if (m_Vector == NULL)
		return false;
	return true;
}

bool tmGISImport::Open (const wxString & filename)
{
	// check if file exists
	m_Vector = NULL;
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
