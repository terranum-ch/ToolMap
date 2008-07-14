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

/***************************************************************************//**
 @brief Constructor
 @author Lucien Schreiber (c) CREALP 2008
 @date 14 July 2008
 *******************************************************************************/
tmGISData::tmGISData()
{

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

}


void tmGISData::InitGISDriversRaster()
{
	 GDALAllRegister();
}


void tmGISData::InitGISDriversVector()
{
	OGRRegisterAll();
}


void tmGISData::InitGISDrivers (bool bRaster, bool bVector)
{
	if (bRaster)
	{
		InitGISDriversRaster();
		wxLogDebug(_T("GIS Raster drivers loaded"));
	}
	
	if (bVector)
	{
		InitGISDriversVector();
		wxLogDebug(_T("GIS Vector drivers loaded"));
	}
}



wxString tmGISData::GetAllSupportedGISFormatsWildcards()
{
	wxString myReturnedWildcard = _T("");
	
	// append vector wildcards
	myReturnedWildcard.Append(tmGISDataVector::GetAllVectorGISFormatsWildcards());
	myReturnedWildcard.Append(_T("|"));
	myReturnedWildcard.Append(tmGISDataRaster::GetAllRasterGISFormatsWildcards());
	return myReturnedWildcard;
}



tmGISData * tmGISData::CreateGISBasedOnType (const int & gis_format_index)
{
	// create pointer depending on the selected format
	if (gis_format_index < tmGISRASTER_OFFSET)
		return tmGISDataVector::CreateGISVectorBasedOnType(gis_format_index);
	else
		return tmGISDataRaster::CreateGISRasterBasedOnType(gis_format_index);
}
