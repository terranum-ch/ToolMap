/***************************************************************************
								tmgisdata.h
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


#ifndef _TM_GISDATA_H_
#define _TM_GISDATA_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif


#include "gdal_priv.h"		// GDAL ACCES C++
#include "ogrsf_frmts.h"	// OGR accessing
#include <wx/filename.h>	// for dealing with filename class


/*enum tmGISDATA_TYPES
{
	tmGIS_VECTOR_SHAPEFILE = 0,
	tmGIS_RASTER_TIFF
};

static wxString tmGISDATA_TYPE_WILDCARDS[] = 
{_("Esri's shapefiles (*.shp)|*.shp"),
_("Tiff raster file (*.tif *.tiff)|*.tif *.tiff")};*/




class tmRealRect 
{
public:
	double x_min;
	double y_min;
	double x_max;
	double y_max;
	
	tmRealRect() : x_min(0.0), y_min(0.0), x_max(0.0), y_max(0.0) { }
	tmRealRect(double xmin, double ymin, double xmax, double ymax) :
	x_min(xmin), y_min(ymin), x_max(xmax), y_max(ymax) { }
	
	bool operator==(const tmRealRect& pt) const
    {
        return wxIsSameDouble(x_min, pt.x_min) && wxIsSameDouble(y_min, pt.y_min)
		&& wxIsSameDouble(x_max, pt.x_max) && wxIsSameDouble(y_max, pt.y_max);
    }
};



/***************************************************************************//**
 @brief Main class for dealing with GIS data
 @details This class may be used for accessing GIS data using GDAL / OGR driver
 see also : http://www.gdal.org/
 @author Lucien Schreiber (c) CREALP 2008
 @date 14 July 2008
 *******************************************************************************/
class tmGISData : public wxObject
	{
	private:
		void InitMemberValue();
		
		static void InitGISDriversRaster();
		static void InitGISDriversVector();
		
	protected:
	public:
		// ctor and dtor
		tmGISData();
		~tmGISData();
		
		// static functions for init
		static void InitGISDrivers (bool bRaster = TRUE, bool bVector = TRUE);
		static wxString GetAllSupportedGISFormatsWildcards();
		static tmGISData * CreateGISBasedOnType (const int & gis_format_index);
		
		virtual bool Open (const wxString & filename){return TRUE;}
		virtual tmRealRect GetMinimalBoundingRectangle(){return tmRealRect(0,0,0,0);}
	};





#endif
