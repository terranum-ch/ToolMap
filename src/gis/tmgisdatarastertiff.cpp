/***************************************************************************
								tmgisdatarastertiff.cpp
				For dealing with spatial raster data (*.tiff, *.tif)
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

#include "tmgisdatarastertiff.h"


tmGISDataRasterTIFF::tmGISDataRasterTIFF()
{
	m_FileType = _T("GDAL TIFF Raster");
}

tmGISDataRasterTIFF::~tmGISDataRasterTIFF()
{
	
}


/***************************************************************************//**
 @brief Get Size of The Tiff raster
 @param iPrecision The number of digits required (default is 2)
 @return  An html compliant string containing the Tiff size in an human
 readable format
 @author Lucien Schreiber (c) CREALP 2008
 @date 24 October 2008
 *******************************************************************************/
wxString tmGISDataRasterTIFF::GetDataSizeAsHtml (int iPrecision)
{
	wxString myResult = _("<U><B>Tiff Raster Size</B></U><BR>");
	
	wxFileName myTiffName (GetFullFileName());
	myResult.Append(wxString::Format(_("Raster size : %s"),
					myTiffName.GetHumanReadableSize(_T("Error computing raster size"),
													iPrecision).c_str()));
	
	return myResult;
}
