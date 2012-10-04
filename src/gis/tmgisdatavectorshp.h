/***************************************************************************
								tmgisdatavectorshp.h
                    class for dealing with vector SHP data
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


#ifndef _TM_GISDATAVECTOR_SHP_H_
#define _TM_GISDATAVECTOR_SHP_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "tmgisdatavector.h"
#include "ogrsf_frmts.h"
#include "ogr_api.h"
#include "gdal_alg.h"
#include "gdal_priv.h"




class tmGISDataVectorSHP : public tmGISDataVector
{    
protected:
    OGRDataSource       *m_Datasource;
    OGRLayer			*m_Layer;
    GDALDataset         *m_RasterizeDataset;
    OGRFeature			*m_Feature;
    int					m_polyTotalRings;

public:
    tmGISDataVectorSHP();
    ~tmGISDataVectorSHP();
    
    // implementing vi±rtual function
    virtual bool Open (const wxString & filename, bool bReadWrite = FALSE);
    virtual bool Close ();
    virtual tmRealRect GetMinimalBoundingRectangle();
    virtual TM_GIS_SPATIAL_TYPES GetSpatialType ();
    
    
    // virtual function for getting data & drawing
    virtual bool SetSpatialFilter (tmRealRect filter, int type);
    virtual bool SetAttributFilter (const wxString & query);
    virtual wxRealPoint * GetNextDataLine (int & nbvertex,long & oid);
    virtual wxRealPoint * GetNextDataPoint (long & oid);
    virtual int GetNextDataPolygonInfo (long & oid);
    virtual wxRealPoint * GetNextDataPolygon (int currentring, int & nbvertex);
    virtual OGRPolygon * GetNextDataOGRPolygon (long & oid);
    virtual OGRFeature * GetNextFeature ();
    virtual OGRFeature * GetFeatureByOID (long oid);
    bool SelectFeatureByOID (long oid);
    
    // virtual function for metadata
    virtual wxString GetMetaDataAsHtml ();
    virtual wxString GetDataSizeAsHtml (int iPrecision = 2);
    
    // count
    virtual int GetCount ();
    
    // fields functions
    virtual int GetFieldsCount();
    virtual bool GetFieldsName (wxArrayString & Fields, long oid = wxNOT_FOUND);
    virtual bool GetFieldsValue (wxArrayString & values, long oid);
    bool GetDistinctFieldsValue (const wxString & fieldname, wxArrayString & values);
    
    // searching data
    virtual wxArrayLong * SearchData (const tmRealRect & rect, int type);
    virtual wxArrayLong * GetAllData ();
    virtual bool GetSnapCoord (const wxRealPoint & clickpt, int iBuffer,
                               wxArrayRealPoints & snapppts, int snaptype);
    
    // creating file & fields
    virtual bool CreateFile (const wxFileName & filename, int type);
    virtual bool AddFieldText (const wxString & fieldname, int size);
    virtual bool AddFieldNumeric (const wxString & fieldname, bool isfloat = false);
    virtual bool AddFieldDate (const wxString & fieldname);
    
    virtual long AddGeometry (OGRGeometry * Geom, const long & oid, int layertype = wxNOT_FOUND);
    virtual bool SetNextFeature (bool resetreading = false);
    virtual bool SetFieldValue (const wxString & value,
                                int fieldtype, int iindex);
    virtual long GetActualOID ();
    virtual void SetActualOID (long oid);
    virtual bool UpdateFeature ();
    virtual void CloseGeometry();
    virtual bool GetFieldNumeric (const wxString & fieldname, int & fieldvalue);
    virtual bool SetFieldNumeric (const wxString & fieldname, int fieldvalue);
    
    bool CreateSpatialIndex(int indexdepth = wxNOT_FOUND);
    long GetFeatureIDIntersectedBy(OGRGeometry * geometry);
    long GetFeatureIDIntersectedOnRaster(OGRPoint * geometry);
    bool Rasterize(double rasterizefactor);
    void RemoveRasterizeFile();
    
    
    virtual bool CopyToFile(const wxFileName & filename, const wxString & drivername);
};






class tmGISDataVectorSHPMemory : public tmGISDataVectorSHP
{
private:
public:
    tmGISDataVectorSHPMemory();
    ~tmGISDataVectorSHPMemory();
    
    virtual bool CreateFile (const wxFileName & filename, int type);
};


#endif
