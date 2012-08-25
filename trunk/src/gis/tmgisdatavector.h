/***************************************************************************
								tmgisdatavector.h
                   Super class for dealing with vector GIS data
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


#ifndef _TM_GISDATAVECTOR_H_
#define _TM_GISDATAVECTOR_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include "tmgisdata.h"

// Number of formats reserved for rasters
const int tmGISVECTOR_OFFSET = 3;

enum tmGISDATA_VECTOR_TYPES
{
	tmGIS_VECTOR_SHAPEFILE = tmGISVECTOR_OFFSET,
	tmGIS_VECTOR_MYSQL // should allways been put as last item
};

static wxString tmGISDATA_VECTOR_TYPE_WILDCARDS[] = 
{_("Esri's shapefiles (*.shp)|*.shp"),
_("MySQL geometric table (*.MYD)|*.MYD")};


static wxString tmGISDATA_VECTOR_TYPE_EXTENSION[] =
{
	_T("*.shp"),
	_T("*.mysql"),
};


class tmGISDataVector : public tmGISData
	{
	private:
		// inserting private function
		bool ComputeLineLineIntersection (OGRLineString * line, OGRLineString * intersection,
										 const wxArrayInt & vertexindex,
										  OGRMultiPoint & resultpos);
		bool SplitLinesAtVertex (OGRLineString * line,
								 const wxArrayInt & splitpos,
								 OGRMultiLineString & splitedline);
		OGRLineString * GetLineWithIntersection (OGRLineString * line, 
												 OGRLineString * intersection,
												 wxArrayInt & insertedvertex);
		OGRLineString * GetLineWithIntersection(OGRLineString * line, 
												OGRMultiLineString * multiline,
												wxArrayInt & intertedvertex);
		
		
	protected:
		virtual wxString GetTableName (TOC_GENERIC_NAME type) {return wxEmptyString;}

		// GEOS functions 
		bool CheckGEOSIntersection(GEOSGeom * rect, GEOSGeom * object);
		bool CheckGEOSCrosses (GEOSGeom * g1, GEOSGeom * g2);
		GEOSGeom  CreateGEOSGeometry (OGRGeometry * geom);
		GEOSGeom  CreateGEOSGeometry (const tmRealRect & rect);
	
		
		// Intersection for snapping
		bool GetVertexIntersection(OGRGeometry * geometry, OGRGeometry * buffer,
                                   wxArrayRealPoints & points);
		bool GetBeginEndInterseciton (OGRGeometry * geometry, OGRGeometry * buffer,
                                      wxArrayRealPoints & points);
		
		
		
		static OGRGeometry * SafeCreateFromGEOS (GEOSGeom geom);
		static OGRGeometry * SafeBuffer (OGRGeometry * ogrgeom,  int size);
		static OGRGeometry * SafeIntersection (OGRGeometry * geom1, OGRGeometry * geom2);
		static OGRGeometry * SafeUnion (OGRGeometry * union1, OGRGeometry * line);
		
		static OGRGeometry * CreateOGRGeometry(const wxRealPoint & pt);
		
		
	public:
		// ctor, dtor
		tmGISDataVector();
		~tmGISDataVector();
		
		// static functions
		static void InitGISDriversVector();
		static wxString GetAllVectorGISFormatsWildcards();
		static tmGISDataVector * CreateGISVectorBasedOnType (const int & gis_format_index);
		static tmGISDataVector * CreateGISVectorBasedOnExt (const wxString & extension);
		static OGRGeometry * CreateOGRGeometry (const tmRealRect & rect);
		OGRLineString * InsertVertex (OGRGeometry * pointbuffer,wxRealPoint ptclicked,
									  OGRLineString * line, int & inseredvertex);
		OGRLineString * InsertVertexMultiple (OGRLineString * line, 
											  OGRMultiPoint * vertex, 
											  const wxArrayInt & point_pos);
		
		
		// gis function
		virtual TM_GIS_SPATIAL_TYPES GetSpatialType (){ return LAYER_SPATIAL_UNKNOWN;}
		
		
		// virtual function for getting data & drawing
		virtual bool SetSpatialFilter (tmRealRect filter, int type){return FALSE;}
		virtual bool SetAttributFilter (const wxString & query){return FALSE;}
		virtual wxRealPoint * GetNextDataLine (int & nbvertex, long & oid){return NULL;}
		virtual OGRFeature * GetNextFeature (){return NULL;}
		virtual wxRealPoint * GetNextDataPoint (long & oid){return NULL;}
		virtual int GetNextDataPolygonInfo (long & oid){return -1;}
		virtual wxRealPoint * GetNextDataPolygon (int currentring, int & nbvertex){return NULL;}
		virtual OGRFeature * GetFeatureByOID (long oid){ return NULL;}
		virtual long AddGeometry (OGRGeometry * Geom, const long & oid, int layertype = wxNOT_FOUND){return -1;}
		virtual bool UpdateGeometry (OGRGeometry * geom, const long & oid){return false;}
		virtual bool SplitGeometry (OGRGeometryCollection * gCol, const long & oid, 
									wxArrayLong & AddedIds, int layertype);
		virtual OGRGeometryCollection * GetGeometryColByOID(wxArrayLong * OIDs){return NULL;}
		virtual void CloseGeometry(){;}
		
		// counting
		virtual int GetCount (){return -1;}
		
		// fields functions
		virtual int GetFieldsCount() {return -1;}
		virtual bool GetFieldsName (wxArrayString & Fields, long oid = wxNOT_FOUND){return false;}
		virtual bool GetFieldsValue (wxArrayString & values, long oid){return false;}
		
		// Metadata
		wxString GetFieldsMetadata ();
		
		// transformations
		bool CutLineAtVertex (long oid, const wxRealPoint & clickedpt, int searchRadius, int layertype);
		//OGRLineString * line1, OGRGeometry * pointbuffer,
		//					  wxRealPoint ptclicked, OGRLineString & lineresult1,
		//					  OGRLineString & lineresult2);
		bool CutLineGeometry(OGRLineString * line1, OGRLineString * line2,
							 OGRMultiLineString & res1, 
							 OGRMultiLineString & res2);
		bool CutLineMultiple (OGRLineString * linetocut, OGRMultiLineString * cutlines,
							  OGRMultiLineString & results);
		
		// merging
		bool LinesMerge (OGRMultiLineString * linetomerge,
						 OGRGeometry ** linemerged); 
		
		// Searching 
		virtual wxArrayLong * SearchIntersectingGeometry (OGRGeometry * intersectinggeom){return NULL;}
		
		
		
	};



#endif
