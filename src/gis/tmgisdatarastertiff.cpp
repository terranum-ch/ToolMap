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


