/***************************************************************************
								tmgisdatavector.cpp
                   Super class for dealing with vector GIS data
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

#include "tmgisdatavector.h"
#include "tmgisdatavectorshp.h"
#include "tmgisdatavectormysql.h"


tmGISDataVector::tmGISDataVector()
{

}



tmGISDataVector::~tmGISDataVector()
{

}


wxString tmGISDataVector::GetAllVectorGISFormatsWildcards()
{
	// Don't return mysql wildcard, used only internally
	wxString myWildcards = _T("");
	
	myWildcards.Append(tmGISDATA_VECTOR_TYPE_WILDCARDS[tmGIS_VECTOR_SHAPEFILE - tmGISVECTOR_OFFSET]);
	return myWildcards;
}


tmGISDataVector * tmGISDataVector::CreateGISVectorBasedOnType(const int & gis_format_index)
{
	switch (gis_format_index)
	{
		case tmGIS_VECTOR_SHAPEFILE:
			return new tmGISDataVectorSHP();
			break;
		case tmGIS_VECTOR_MYSQL:
			return new tmGISDataVectorMYSQL();
			break;
	}
	return NULL;
}



tmGISDataVector * tmGISDataVector::CreateGISVectorBasedOnExt (const wxString & extension)
{
	int iLoop = sizeof(tmGISDATA_VECTOR_TYPE_EXTENSION) / sizeof(wxString);
	for (int i = 0; i< iLoop; i++)
	{
		if (tmGISDATA_VECTOR_TYPE_EXTENSION[i].Contains(extension))
			return CreateGISVectorBasedOnType(i+tmGISVECTOR_OFFSET);
	}
	return NULL;
}




void tmGISDataVector::InitGISDriversVector()
{
	OGRRegisterAll();
}


