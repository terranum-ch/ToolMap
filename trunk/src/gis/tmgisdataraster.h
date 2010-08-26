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
#include <wx/dir.h>	// for computing dir size


// if you add some raster format, don't forget
// to update the tmGISVECTOR_OFFSET
enum tmGISDATA_RASTER_TYPES
{
	tmGIS_RASTER_TIFF = 0, 
	tmGIS_RASTER_BINGRID,
	tmGIS_RASTER_JPEG
};

static wxString tmGISDATA_RASTER_TYPE_WILDCARDS[] = 
{_("Tiff raster file (*.tif,*.tiff)|*.tif;*.tiff"),
_("Esri's binary GRID (hdr.adf)|hdr.adf"),
_("Jpeg raster file (*.jpg, *.jpeg)|*.jpg;*.jpeg")};

static wxString tmGISDATA_RASTER_TYPE_EXTENSION[] =
{
	_T("*.tif;*.tiff"),
	_T("*.adf"),
	_T("*.jpg;*.jpeg")
};


const wxString tmRASTER_DATATYPES[] = {
    _("Unknown data type"),
   _("Eight bit unsigned integer"),           
   _("Sixteen bit unsigned integer"),         
    _("Sixteen bit signed integer"),        
    _("Thirty two bit unsigned integer"),  
    _("Thirty two bit signed integer"),     
    _("Thirty two bit floating point"),    
    _("Sixty four bit floating point"),     
    _("Complex Int16"),                   
    _("Complex Int32"),                    
   _("Complex Float32"),                  
    _("Complex Float64"),            
    _("Data type out of limits")          
};



class tmGISDataRaster : public tmGISData
	{
	private:
	protected:
		GDALDataset *	m_DataSet;
		GDALRasterBand *m_RasterBand;
		wxString		m_FileType;
		wxRect			m_PxImgFilter;
		tmRealRect		m_ClippedCoord;
		
		wxSize GetImagePxDim ();
		bool GetImagePxSize (double & pxsizeX, double & pxsizeY, 
							 const tmRealRect & imgrealcoord = tmRealRect(0,0,0,0));
		wxRect ConvertClipedImage (const tmRealRect & origin, const tmRealRect & clipped);
		
		// reading image (DTM)
		void * ReadImageData ( GDALRasterBand *gdalBand, const wxRect & imgfilter,
							  const wxSize & imgSize, int & buffsize);
		double ReadGDALValueToDouble ( void *data, GDALDataType type, int index );
		
		// statistics function
		bool GetStatMinMaxNoDataValue (double & dmin, double & dmax, double & dnodata);
		
	public:
		tmGISDataRaster();
		~tmGISDataRaster();
		
		// standard GDAL functions
		virtual bool Open (const wxString & filename, bool bReadWrite = TRUE);
		virtual tmRealRect GetMinimalBoundingRectangle();
		
		// static functions
		static void InitGISDriversRaster();
		static wxString GetAllRasterGISFormatsWildcards();
		static tmGISDataRaster * CreateGISRasterBasedOnType (const int & gis_format_index);
		static tmGISDataRaster * CreateGISRasterBasedOnExt (const wxString & extension);
		
		// gis functions
		virtual TM_GIS_SPATIAL_TYPES GetSpatialType (){ return LAYER_SPATIAL_RASTER;}
		
		// reading functions
		virtual bool SetSpatialFilter (tmRealRect filter, int type);
		bool IsImageInsideVisibleArea ();
		CPLErr GetImageData(unsigned char **imgbuf, unsigned int   *imglen,
							unsigned char **maskbuf, unsigned int   *masklen,
							wxSize imgSize);
		bool GetImageTranslucency (wxSize imgSize, int translucencypercent,
								   unsigned char **alphachn); 
		tmRealRect GetImageClipedCoordinates (){return m_ClippedCoord;}
		
		
		virtual int GetBandCount ();
		virtual int GetPyramidsInfo (wxArrayString * pyramids = NULL);
		
		// virtual function for metadata
		virtual wxString GetMetaDataAsHtml ();
		wxString GetBandMetaData ();
		wxString GetUnitMetaData ();
		wxString GetPyramidsMetadata();
		wxString GetImagePxSizeMetadata ();
		
		
		
	};



#endif