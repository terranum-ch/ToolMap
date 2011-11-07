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


#include "gdal_priv.h"			// GDAL ACCES C++
#include "ogrsf_frmts.h"		// OGR accessing
#include "geos_c.h"				// GEOS accessing
#include <wx/filename.h>		// for dealing with filename class
#include "tmgisscale.h"			// for dealing with scale and real rectangle.
#include "tmlayerproperties.h"	// for GIS spatial types and tmLayerProperties
#include "../core/tmarraysize.h" // for wxArrayRealPoints





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
		wxString		m_ShortFileName;
		wxString		m_FullFileName;
		static bool	m_LogOn;	
		
		void InitMemberValue();
		
	
	protected:
		wxString GetMinimalBoundingRectangleAsHtml (int iprecision = 2);
		int m_ClassType;
		
	public:
		
		// ctor and dtor
		tmGISData();
		~tmGISData();
		
		// get type of class
		int GetDataType(){return m_ClassType;}
		
		// static functions for init
		static void InitGISDrivers (bool bRaster = TRUE, bool bVector = TRUE);
		static wxString GetAllSupportedGISFormatsWildcards();
		static wxArrayString GetAllSupportedGISFormatsExtensions();
		static tmGISData * CreateGISBasedOnType (const int & gis_format_index);
		static tmGISData * CreateGISBasedOnExt (const wxString & extension);
		// load layer
		static tmGISData * LoadLayer (tmLayerProperties * layerprop);
		
		static void EnableLogging (bool enable = true) {m_LogOn = enable;}
		static bool IsLoggingEnabled () {return m_LogOn;}
		
		
		// gis function
		virtual bool Open (const wxString & filename, bool bReadWrite = TRUE);
		virtual tmRealRect GetMinimalBoundingRectangle(){return tmRealRect(0,0,0,0);}
		virtual TM_GIS_SPATIAL_TYPES GetSpatialType (){ return LAYER_ERR;}
		
		// misc function
		wxString GetShortFileName (){return m_ShortFileName;}
		wxString GetFullFileName () {return m_FullFileName;}
		
		// metadata functions
		virtual wxString GetMetaDataAsHtml (){return wxEmptyString;}
		virtual wxString GetDataSizeAsHtml (int iPrecision = 2){return wxEmptyString;}
		
		// search function
		virtual wxArrayLong * SearchData (const tmRealRect & rect, int type){return NULL;}
		virtual wxArrayLong * GetAllData (){return NULL;}
		virtual bool GetSnapCoord (const wxRealPoint & clickpt, int iBuffer,
								   wxArrayRealPoints & snapppts, int snaptype) {return false;}
		
		
		
		
	};





#endif
