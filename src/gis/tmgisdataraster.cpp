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
#include "tmgisdatarastertiff.h"


tmGISDataRaster::tmGISDataRaster()
{
	m_DataSet = NULL;
	m_RasterBand = NULL;
}


tmGISDataRaster::~tmGISDataRaster()
{
	// closing GDAL raster dataset
	if (m_DataSet)
	{
		GDALClose(m_DataSet);
		m_DataSet = NULL;
	}
	m_RasterBand = NULL;
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
			return new tmGISDataRasterTIFF();
			break;
		case tmGIS_RASTER_BINGRID:
			return NULL;
			break;
	}
	return NULL;
}



tmGISDataRaster * tmGISDataRaster::CreateGISRasterBasedOnExt (const wxString & extension)
{
	int iLoop = sizeof( tmGISDATA_RASTER_TYPE_EXTENSION) / sizeof(wxString);
	for (int i = 0; i< iLoop; i++)
	{
		if (tmGISDATA_RASTER_TYPE_EXTENSION[i].Contains(extension))
			return CreateGISRasterBasedOnType(i);
	}
	return NULL;
}



void tmGISDataRaster::InitGISDriversRaster()
{
	GDALAllRegister();
}


