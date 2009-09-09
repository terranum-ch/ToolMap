/***************************************************************************
								tmgisdata.cpp
                    Main class for dealing with GIS data
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

#include "tmgisdata.h"
#include "tmgisdatavector.h"
#include "tmgisdataraster.h"

#include "../database/database_tm.h"		// for database acces (only for LoadLayer())

// logging start enabled
bool tmGISData::m_LogOn = true;

/***************************************************************************//**
 @brief Constructor
 @author Lucien Schreiber (c) CREALP 2008
 @date 14 July 2008
 *******************************************************************************/
tmGISData::tmGISData()
{
	InitMemberValue();
}



/***************************************************************************//**
 @brief Destructor
 @author Lucien Schreiber (c) CREALP 2008
 @date 14 July 2008
 *******************************************************************************/
tmGISData::~tmGISData()
{
	
}



/***************************************************************************//**
 @brief Init class members
 @author Lucien Schreiber (c) CREALP 2008
 @date 14 July 2008
 *******************************************************************************/
void tmGISData::InitMemberValue()
{
	m_ShortFileName = _T("");
	m_FullFileName = _T("");
}



/***************************************************************************//**
 @brief Init the GDAL / OGR driver
 @details Call this function during program startup to allow all GDAL / OGR
 drivers to be loaded.
 @param bRaster Should we load all GDAL rasters drivers
 @param bVector Should we load all OGR vector drivers
 @author Lucien Schreiber (c) CREALP 2008
 @date 14 July 2008
 *******************************************************************************/
void tmGISData::InitGISDrivers (bool bRaster, bool bVector)
{
	if (bRaster)
	{
		tmGISDataRaster::InitGISDriversRaster();
		wxLogDebug(_T("GIS Raster drivers loaded"));
	}
	
	if (bVector)
	{
		tmGISDataVector::InitGISDriversVector();
		initGEOS(NULL,NULL);
		wxLogDebug(_T("GIS Vector drivers loaded"));
	}
	
	
	wxLogMessage(_("loaded GDAL version : %s"), wxString::FromAscii(GDAL_RELEASE_NAME).c_str());
	wxLogMessage(_("loaded GEOS version : %s"), wxString::FromAscii(GEOS_VERSION).c_str());
}



/***************************************************************************//**
 @brief Return all supported formats
 @details Return the wildcards of all supported formats for loading.
 @return A String containing all supported formats as wildcards that one
 may use in a dialog box for file selection
 @author Lucien Schreiber (c) CREALP 2008
 @date 14 July 2008
 *******************************************************************************/
wxString tmGISData::GetAllSupportedGISFormatsWildcards()
{
	wxString myReturnedWildcard = _T("");
	
	myReturnedWildcard.Append(tmGISDataRaster::GetAllRasterGISFormatsWildcards());
	myReturnedWildcard.Append(_T("|"));
	myReturnedWildcard.Append(tmGISDataVector::GetAllVectorGISFormatsWildcards());
	
	return myReturnedWildcard;
}



/***************************************************************************//**
 @brief Returning all extensions in an array string
 @details All supported extensions in an array string but not the mysql one
 (because no extension is valid for that format).
 @note Raster format are returned first
 @return all supported extension, raster ones first
 @author Lucien Schreiber (c) CREALP 2008
 @date 06 August 2008
 *******************************************************************************/
wxArrayString tmGISData::GetAllSupportedGISFormatsExtensions()
{
	wxArrayString myExtensions;
	
	int iNbRaster = sizeof(tmGISDATA_RASTER_TYPE_EXTENSION) / sizeof (wxString);
	int iNbVector = sizeof(tmGISDATA_VECTOR_TYPE_EXTENSION) / sizeof (wxString);
	iNbVector = iNbVector - 1; // removing mysql format.
	
	
	// adding rasters
	for (int r = 0; r< iNbRaster; r++)
		myExtensions.Add(tmGISDATA_RASTER_TYPE_EXTENSION[r]);
	
	// adding vectors
	for (int v = 0; v< iNbVector; v++)
		myExtensions.Add(tmGISDATA_VECTOR_TYPE_EXTENSION[v]);
	
	return myExtensions;
}


/***************************************************************************//**
 @brief Create an object of the specified format
 @details Allowed values for gis_format_index are those from the
 #tmGISDATA_RASTER_TYPES and from the #tmGISDATA_VECTOR_TYPES
 @param gis_format_index zero based index of the item type to create
 @return  pointer to an object of the correct type or NULL if something's going
 wrong
 @author Lucien Schreiber (c) CREALP 2008
 @date 14 July 2008
 *******************************************************************************/
tmGISData * tmGISData::CreateGISBasedOnType (const int & gis_format_index)
{
	// create pointer depending on the selected format
	if (gis_format_index < tmGISVECTOR_OFFSET)
		return tmGISDataRaster::CreateGISRasterBasedOnType(gis_format_index);
	else
		return tmGISDataVector::CreateGISVectorBasedOnType(gis_format_index);
}



tmGISData * tmGISData::CreateGISBasedOnExt (const wxString & extension)
{
	tmGISData * myDataObj = NULL;
	
	// try opening vector extensions
	myDataObj = tmGISDataVector::CreateGISVectorBasedOnExt(extension);
	if (myDataObj)
		return myDataObj;
	
	// try opening raster extensions
	myDataObj = tmGISDataRaster::CreateGISRasterBasedOnExt(extension);
	if (myDataObj)
		return myDataObj;
	
	// if all failed then return null.
	if (IsLoggingEnabled())
		wxLogDebug(_T("No format handler found for extension : %s"), extension.c_str());
	return NULL;
	
}




/***************************************************************************//**
 @brief Generic open code
 @details Mainly init member values used by all GIS class
 @param filename path and name of the GIS data to open
 @param bReadWrite Should we open the dataset for r/w operations ?
 @return  Allways TRUE
 @author Lucien Schreiber (c) CREALP 2008
 @date 14 July 2008
 *******************************************************************************/
bool tmGISData::Open (const wxString & filename, bool bReadWrite)
{
	wxFileName myFilename (filename);
	m_ShortFileName = myFilename.GetFullName();
	m_FullFileName = myFilename.GetFullPath();
	
	return TRUE;
}



/***************************************************************************//**
 @brief Get Minimal bounding rectangle as html
 @details This function is intented for getting minimal bounding rectangle for
 metadata
 @return  An html compliant string
 @author Lucien Schreiber (c) CREALP 2008
 @date 23 October 2008
 *******************************************************************************/
wxString tmGISData::GetMinimalBoundingRectangleAsHtml (int iprecision)
{
	tmRealRect MinBounding = GetMinimalBoundingRectangle();
	wxString myReturnedVal = _("<U><B>Extent of the layers</B></U><BR>");
	
	if (MinBounding == tmRealRect(0,0,0,0))
	{
		myReturnedVal.Append(_("Unable to compute the layers extent<BR>"));
		myReturnedVal.Append(_("<I>Maybe no data are present in the layer</I><BR>"));
		return myReturnedVal;
	}
	
	myReturnedVal.Append(wxString::Format(_("x min : %.*f<br>x max : %.*f<BR>"),
										  iprecision, MinBounding.x_min,
										  iprecision, MinBounding.x_max));
	myReturnedVal.Append(wxString::Format(_("y min : %.*f<BR>y max : %.*f<BR>"),
										  iprecision, MinBounding.y_min,
										  iprecision, MinBounding.y_max));
	return myReturnedVal;
	
}



/***************************************************************************//**
 @brief Call this function for loading a layer
 @details This function create an object of type #tmGISData based on layer
 properties
 @param layerProp the layer properties
 @return  a valid tmGISData object or NULL if an error occur
 @author Lucien Schreiber (c) CREALP 2009
 @date 30 January 2009
 *******************************************************************************/
tmGISData * tmGISData::LoadLayer (tmLayerProperties * layerProp)
{
	wxASSERT(layerProp);
	tmGISData * m_Data = NULL;
	wxString myFileName = _T("");
	wxString myErrMsg = _T("");
	// only used if not generic layers
	wxFileName layerfullname (layerProp->m_LayerPathOnly, layerProp->m_LayerNameExt);
	
	switch (layerProp->m_LayerType)
	{
		case TOC_NAME_LINES:
		case TOC_NAME_POINTS:
		case TOC_NAME_ANNOTATIONS:
		case TOC_NAME_LABELS:
		case TOC_NAME_FRAME:
			m_Data = tmGISData::CreateGISBasedOnType(tmGIS_VECTOR_MYSQL);
			myFileName = TABLE_NAME_GIS_GENERIC[layerProp->m_LayerType];
			myErrMsg = layerProp->m_LayerNameExt;
			break;
			
		case TOC_NAME_TIFF:
		case TOC_NAME_EGRID:
		case TOC_NAME_JPEG:
		case TOC_NAME_SHP:
			m_Data = tmGISData::CreateGISBasedOnExt(layerProp->GetFileExtension());
			myFileName = layerfullname.GetFullPath();
			myErrMsg = myFileName;
			break;
			
		default:
			if (IsLoggingEnabled())
				wxLogDebug(_T("%s file format not supported yet \n "),
						   layerProp->m_LayerNameExt.c_str());
			return NULL;
			break;
	}
	
	// here load data
	if (!m_Data)
	{
		if (IsLoggingEnabled())
			wxLogError(_("Error loading : %s"), myErrMsg.c_str());
		return NULL;
	}
	
	if (!m_Data->Open(myFileName, TRUE))
	{
		if (IsLoggingEnabled())
			wxLogError(_("Error opening : %s"), myErrMsg.c_str());
		return NULL;
	}
	
	return m_Data;
}



