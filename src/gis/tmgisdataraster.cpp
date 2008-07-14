/***************************************************************************
								tmgisdataraster.cpp
                    Super class for dealing with raster GIS data
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

#include "tmgisdataraster.h"


tmGISDataRaster::tmGISDataRaster()
{
	
}


tmGISDataRaster::~tmGISDataRaster()
{
	
}



wxString tmGISDataRaster::GetAllRasterGISFormatsWildcards()
{
	wxString myWildCards = _T("");
	for (unsigned int i = 0; i < (sizeof(tmGISDATA_RASTER_TYPE_WILDCARDS) / sizeof(wxString)); i++)
	{
		myWildCards.Append(tmGISDATA_RASTER_TYPE_WILDCARDS[i]);
		
		if (i+1 < (sizeof(tmGISDATA_RASTER_TYPE_WILDCARDS) / sizeof (wxString)))
			myWildCards.Append(_T("|"));
	}
	
	return myWildCards;
}



tmGISDataRaster * tmGISDataRaster::CreateGISRasterBasedOnType (const int & gis_format_index)
{
	switch (gis_format_index)
	{
		case tmGIS_RASTER_TIFF:
			return NULL;
			break;
		case tmGIS_RASTER_BINGRID:
			return NULL;
			break;
	}
	return NULL;
}
