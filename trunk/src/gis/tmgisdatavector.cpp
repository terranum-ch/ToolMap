/***************************************************************************
								tmgisdatavector.cpp
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

#include "tmgisdatavector.h"
#include "tmgisdatavectorshp.h"
#include "tmgisdatavectormysql.h"


tmGISDataVector::tmGISDataVector()
{

}



tmGISDataVector::~tmGISDataVector()
{

}


wxString tmGISDataVector::GetAllVectorGISFormatsWildcards()
{
	// Don't return mysql wildcard, used only internally
	wxString myWildcards = _T("");
	
	myWildcards.Append(tmGISDATA_VECTOR_TYPE_WILDCARDS[tmGIS_VECTOR_SHAPEFILE - tmGISVECTOR_OFFSET]);
	return myWildcards;
}


tmGISDataVector * tmGISDataVector::CreateGISVectorBasedOnType(const int & gis_format_index)
{
	switch (gis_format_index)
	{
		case tmGIS_VECTOR_SHAPEFILE:
			return new tmGISDataVectorSHP();
			break;
		case tmGIS_VECTOR_MYSQL:
			return new tmGISDataVectorMYSQL();
			break;
	}
	return NULL;
}



tmGISDataVector * tmGISDataVector::CreateGISVectorBasedOnExt (const wxString & extension)
{
	int iLoop = sizeof(tmGISDATA_VECTOR_TYPE_EXTENSION) / sizeof(wxString);
	for (int i = 0; i< iLoop; i++)
	{
		if (tmGISDATA_VECTOR_TYPE_EXTENSION[i].Contains(extension))
			return CreateGISVectorBasedOnType(i+tmGISVECTOR_OFFSET);
	}
	return NULL;
}




void tmGISDataVector::InitGISDriversVector()
{
	OGRRegisterAll();
}



/***************************************************************************//**
 @brief Formating fields info into html string
 @return  The html compliant string
 @author Lucien Schreiber (c) CREALP 2008
 @date 23 October 2008
 *******************************************************************************/
wxString tmGISDataVector::GetFieldsMetadata ()
{
	wxString myResult = _("<B><U>Fields info</U></B><BR>");
	int iNbFields = GetFieldsCount();
	if (iNbFields < 0)
	{
		myResult.Append(_("Error getting fields info<BR>"));
		return myResult;
	}
	
	myResult.Append(wxString::Format(_("Number of field(s) : %d<BR>"), iNbFields));
	
	wxArrayString myFieldName;
	if (GetFieldsName(myFieldName)==false)
	{
		myResult.Append(_("Error getting fields name<BR>"));
		return myResult;
	}
	
	myResult.Append(_T("<OL>"));
	
	for (unsigned int i =0; i<myFieldName.GetCount();i++)
		myResult.Append(_T("<LI>") + myFieldName.Item(i) + _T("</LI>"));
	
	myResult.Append(_T("</OL><BR>"));

	
	return myResult;
}



/***************************************************************************//**
 @brief Compute real GEOS intersection (not bounding box)
 @details This function may be used for ensuring that a geometry really
 intersects another one
 @note Needs GEOS library
 @param rect GEOS geometry of intersecting rectangle
 @param object GEOS geometry of object
 @return  true if object really intersects rect.
 @author Lucien Schreiber (c) CREALP 2008
 @date 31 October 2008
 *******************************************************************************/
bool tmGISDataVector::CheckGEOSIntersection(GEOSGeom * rect, GEOSGeom * object)
{
	char result = GEOSIntersects(*object, *rect);
	if (result == 1)
		return true;
	return false;
}


/***************************************************************************//**
 @brief Create a GEOS geometry
 @details This function may be used for creating a GEOS compatible geometry from
 an OGR object
 @note Needs GEOS library
 @warning Don't forget to destroy Object
 returned with : GEOSGeom_destroy(GEOSGeometry* g)
 @param geom A valid OGR geometry
 @return Valid GEOS object or NULL if an error occur. Don't forget
 to destroy the returned object with GEOSGeom_destroy(GEOSGeometry* g);
 @author Lucien Schreiber (c) CREALP 2008
 @date 31 October 2008
 *******************************************************************************/
GEOSGeom tmGISDataVector::CreateGEOSGeometry (OGRGeometry * geom)
{
	GEOSGeom geosgeom =  geom->exportToGEOS();

	return geosgeom;
}



/***************************************************************************//**
 @brief Create a GEOS geometry
 @details This function may be used for creating a GEOS compatible geometry from
 an real rectangle
 @note Needs GEOS library
 @warning Don't forget to destroy Object returned with :
 GEOSGeom_destroy(GEOSGeometry* g)
 @param rect A #tmRealRect object
 @return Valid GEOS object or NULL if an error occur. Don't forget
 to destroy the returned object with GEOSGeom_destroy(GEOSGeometry* g);
 @author Lucien Schreiber (c) CREALP 2008
 @date 31 October 2008
 *******************************************************************************/
GEOSGeom  tmGISDataVector::CreateGEOSGeometry (const tmRealRect & rect)
{
	// creating using wkt
	wxString sRect = wxString::Format(_T("POLYGON ((%f %f,%f %f,%f %f,%f %f,%f %f))"),
									  rect.x_min, rect.y_min,
									  rect.x_max, rect.y_min,
									  rect.x_max, rect.y_max,
									  rect.x_min, rect.y_max,
									  rect.x_min, rect.y_min);
	// conversion Unicode wxString -> const char *
	char * buffer = new char [sRect.Length()+2];
	strcpy(buffer, (const char*)sRect.mb_str(wxConvUTF8));
	GEOSGeom  grect = GEOSGeomFromWKT(buffer);
		
	return grect;
}



/***************************************************************************//**
 @brief Create a OGR geometry
 @param rect object of type : #tmRealRect
 @return  return an OGRgeometry or null
 @author Lucien Schreiber (c) CREALP 2008
 @date 31 October 2008
 *******************************************************************************/
OGRGeometry * tmGISDataVector::CreateOGRGeometry (const tmRealRect & rect)
{
	
	wxString sRect = wxString::Format(_T("POLYGON ((%f %f,%f %f,%f %f,%f %f,%f %f))"),
									  rect.x_min, rect.y_min,
									  rect.x_max, rect.y_min,
									  rect.x_max, rect.y_max,
									  rect.x_min, rect.y_max,
									  rect.x_min, rect.y_min);
	// conversion Unicode wxString -> const char *
	char * buffer = new char [sRect.Length()+2];
	strcpy(buffer, (const char*)sRect.mb_str(wxConvUTF8));
	
	
	OGRGeometry * geom = NULL;
	OGRGeometryFactory::createFromWkt(&buffer, NULL, &geom);
	
	return geom;
}

