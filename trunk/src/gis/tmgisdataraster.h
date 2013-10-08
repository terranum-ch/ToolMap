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

DECLARE_EVENT_TYPE(tmEVT_LM_ROTATION_WARNING, -1)


// if you add some raster format, don't forget
// to update the tmGISVECTOR_OFFSET
enum tmGISDATA_RASTER_TYPES
{
	tmGIS_RASTER_TIFF = 0, 
	tmGIS_RASTER_BINGRID,
	tmGIS_RASTER_JPEG
};

static wxString tmGISDATA_RASTER_TYPE_WILDCARDS[] = {
    wxTRANSLATE("Tiff raster file (*.tif,*.tiff)|*.tif;*.tiff"),
wxTRANSLATE("Esri's binary GRID (hdr.adf)|hdr.adf"),
wxTRANSLATE("Jpeg raster file (*.jpg, *.jpeg)|*.jpg;*.jpeg")
};

static wxString tmGISDATA_RASTER_TYPE_EXTENSION[] =
{
	_T("*.tif;*.tiff"),
	_T("*.adf"),
	_T("*.jpg;*.jpeg")
};


const wxString tmRASTER_DATATYPES[] = {
    wxTRANSLATE("Unknown data type"),
   wxTRANSLATE("Eight bit unsigned integer"),           
   wxTRANSLATE("Sixteen bit unsigned integer"),         
    wxTRANSLATE("Sixteen bit signed integer"),        
    wxTRANSLATE("Thirty two bit unsigned integer"),
    wxTRANSLATE("Thirty two bit signed integer"),     
    wxTRANSLATE("Thirty two bit floating point"),    
    wxTRANSLATE("Sixty four bit floating point"),     
    wxTRANSLATE("Complex Int16"),                   
    wxTRANSLATE("Complex Int32"),                    
   wxTRANSLATE("Complex Float32"),                  
    wxTRANSLATE("Complex Float64"),            
    wxTRANSLATE("Data type out of limits")          
};



class tmGISDataRaster : public tmGISData
{
private:
protected:
	GDALDataset *	m_DataSet;
	GDALRasterBand *m_RasterBand;
	wxString		m_FileType;
	wxRect			m_PxImgFilter;
	tmRealRect		m_RasterExtent;
	tmRealRect		m_ClippedCoord;
	
	wxArrayString	m_RotationName;
	wxArrayShort	m_RotationStatus;
	
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
	
	virtual bool CreateSpatialIndex(GDALProgressFunc progress, void * pfProgressData);
    virtual int IsRaster(){return 1;}
	
};





class tmRotationWarning_DLG : public wxDialog {
private:
    bool m_Hide;
    double m_Rotation1;
    double m_Rotation2;
    wxString m_Layer;
	
	wxString m_TxtTemplate;
	
	wxStaticText* m_TextLayerCtrl;
	wxStaticText* m_TextRotationCtrl;
	wxCheckBox* m_HideCtrl;
	wxStdDialogButtonSizer* m_BtnSizerCtrl;
	wxButton* m_BtnSizerCtrlOK;
	wxButton* m_BtnSizerCtrlHelp;

    void _CreateControls();
    void OnHelp(wxCommandEvent & event);
	
public:
    tmRotationWarning_DLG(wxWindow * parent, wxWindowID id, const wxString & title);
    virtual ~tmRotationWarning_DLG();
	
    virtual bool TransferDataFromWindow();
    virtual bool TransferDataToWindow();
	
    inline const bool GetHide() const;
    void SetHide(bool value);
	
    inline const double GetRotation1() const;
    void SetRotation1(double value);
	
    inline const double GetRotation2() const;
    void SetRotation2(double value);
	
    inline const wxString GetLayerName() const;
    void SetLayerName(wxString value);
	
};



inline const bool tmRotationWarning_DLG::GetHide() const {
	return m_Hide;
}

inline const double tmRotationWarning_DLG::GetRotation1() const {
	return m_Rotation1;
}

inline const double tmRotationWarning_DLG::GetRotation2() const {
	return m_Rotation2;
}

inline const wxString tmRotationWarning_DLG::GetLayerName() const {
	return m_Layer;
}



#endif
