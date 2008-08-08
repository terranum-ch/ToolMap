/***************************************************************************
								tmgisdataraster.h
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


#ifndef _TM_GISDATARASTER_H_
#define _TM_GISDATARASTER_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "tmgisdata.h"


// if you add some raster format, don't forget
// to update the tmGISVECTOR_OFFSET
enum tmGISDATA_RASTER_TYPES
{
	tmGIS_RASTER_TIFF = 0, 
	tmGIS_RASTER_BINGRID
};

static wxString tmGISDATA_RASTER_TYPE_WILDCARDS[] = 
{_("Tiff raster file (*.tif,*.tiff)|*.tif;*.tiff"),
_("Esri's binary GRID (hdr.adf)|hdr.adf")};

static wxString tmGISDATA_RASTER_TYPE_EXTENSION[] =
{
	_T("tif,tiff"),
	_T("adf"),
};



class tmGISDataRaster : public tmGISData
	{
	private:
	protected:
		GDALDataset * m_DataSet;
		GDALRasterBand * m_RasterBand;
		
	public:
		tmGISDataRaster();
		~tmGISDataRaster();
		
		// static functions
		static void InitGISDriversRaster();
		static wxString GetAllRasterGISFormatsWildcards();
		static tmGISDataRaster * CreateGISRasterBasedOnType (const int & gis_format_index);
		static tmGISDataRaster * CreateGISRasterBasedOnExt (const wxString & extension);
		
		// gis functions
		virtual TM_GIS_SPATIAL_TYPES GetSpatialType (){ return LAYER_SPATIAL_RASTER;}
		
	};



#endif
