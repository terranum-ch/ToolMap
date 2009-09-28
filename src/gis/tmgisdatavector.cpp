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
 @brief Check for crossing (not bounding box)
 @details This function may be used for ensuring that a geometry really
 crosses another one
 @note Needs GEOS library
 @param g1 GEOS geometry
 @param g2 GEOS geometry
 @return  true if g2 really crosses g1.
 @author Lucien Schreiber (c) CREALP 2008
 @date 10 February 2009
 *******************************************************************************/
bool tmGISDataVector::CheckGEOSCrosses (GEOSGeom * g1, GEOSGeom * g2)
{
	char result = GEOSCrosses(*g1, *g2);
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
	GEOSWKTReader * myReader = GEOSWKTReader_create();
	GEOSGeom grect = GEOSWKTReader_read(myReader, sRect.mb_str(wxConvUTF8));
	GEOSWKTReader_destroy(myReader);
	wxASSERT(grect);
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


/***************************************************************************//**
 @brief Search if a vertex intersect the passed geometry
 @details Check for all geometry (point, lines, polygon) if a vertex intersect
 the passed geometry. If true this vertex is returned
 @param geometry A valid geometry (point, line, polygon)
 @param buffer The buffer for intersection
 @return  the vertex intersecting the geometry (dont forget to delete) or null
 if nothing found
 @author Lucien Schreiber (c) CREALP 2009
 @date 29 January 2009
 *******************************************************************************/
wxRealPoint * tmGISDataVector::GetVertexIntersection(OGRGeometry * geometry, 
													 OGRGeometry * buffer)
{
	OGRPoint * myPointLine = (OGRPoint*) OGRGeometryFactory::createGeometry(wkbPoint);
	
	// should not be deleted, belong to OGR
	OGRLineString * myLineLine = NULL;
	OGRPolygon * myPoly = NULL;
	OGRPoint * myPoint = NULL;
	
	int i = 0;
	wxRealPoint * ptReturn = NULL;
	//bool bBreak = false;
	
	switch (wkbFlatten(geometry->getGeometryType()))
	{
		case wkbLineString:
			myLineLine = (OGRLineString*) geometry;
			for (i=0; i<myLineLine->getNumPoints();i++)
			{
				myLineLine->getPoint(i, myPointLine);
				if (myPointLine && myPointLine->Intersect(buffer))
				{
					ptReturn = new wxRealPoint(myPointLine->getX(),
											   myPointLine->getY());
					break;
				}
				
			}
			OGRGeometryFactory::destroyGeometry(myPointLine);
			break;
		
			
			
			
		case wkbPoint:
			myPoint = (OGRPoint*) geometry;
			if (myPoint && myPoint->Intersect(buffer))
			{
				ptReturn = new wxRealPoint(myPoint->getX(),
										   myPoint->getY());
			}
			break;

			
			
			
		case wkbPolygon:
			myPoly = (OGRPolygon*) geometry;
			// exterior ring
			myLineLine = (OGRLineString*) myPoly->getExteriorRing();
			ptReturn = GetVertexIntersection(myLineLine, buffer);
			
			// interior ring if needed
			if (!ptReturn)
			{
				for (i = 0; i<myPoly->getNumInteriorRings();i++)
				{
					myLineLine = (OGRLineString*) myPoly->getInteriorRing(i);
					if (!myLineLine)
						break;
					
					ptReturn = GetVertexIntersection(myLineLine, buffer);
					if (ptReturn)
						break;
				}
			}
			break;
		
		
		default:
			break;
	}
	
	return ptReturn;
}



/***************************************************************************//**
 @brief Search if a Begin/End vertex intersect the passed geometry
 @details This function is very close to the 
 tmGISDataVector::GetVertexIntersection but search only for begin / end vertex.
 So it works only for lines
 @param geometry A valid geometry (line ONLY)
 @param buffer The buffer for intersection
 @return  the vertex intersecting the geometry (dont forget to delete) or null 
 if nothing found
 @author Lucien Schreiber (c) CREALP 2009
 @date 29 January 2009
 *******************************************************************************/
wxRealPoint * tmGISDataVector::GetBeginEndInterseciton (OGRGeometry * geometry, 
														OGRGeometry * buffer)
{
	wxRealPoint * ptReturn = NULL;
	
	if (wkbFlatten(geometry->getGeometryType()) == wkbLineString)
	{
		OGRLineString *	myLine = (OGRLineString*) geometry;
		int iPointLine = myLine->getNumPoints();
		
		
		OGRPoint * myPoint = (OGRPoint*) OGRGeometryFactory::createGeometry(wkbPoint);
		OGRPoint * myPoint2 = (OGRPoint*) OGRGeometryFactory::createGeometry(wkbPoint);
		
		myLine->getPoint(0, myPoint);
		myLine->getPoint(iPointLine-1, myPoint2);
		
		if (myPoint->Intersect(buffer))
		{
			ptReturn = new wxRealPoint(myPoint->getX(),
									   myPoint->getY());
			
		}
		else if (myPoint2->Intersect(buffer))
		{
			ptReturn = new wxRealPoint(myPoint2->getX(),
									   myPoint2->getY());
		}
		
		OGRGeometryFactory::destroyGeometry(myPoint);
		OGRGeometryFactory::destroyGeometry(myPoint2);
		
	}
	
	return ptReturn;
}


/***************************************************************************//**
 @brief Safe conversion function
 @param geosGeom The GEOSGeom to convert
 @return  A valid OGRGeometry or NULL if an error occur
 @author Lucien Schreiber (c) CREALP 2008
 @date 15 January 2009
 *******************************************************************************/
OGRGeometry * tmGISDataVector::SafeCreateFromGEOS (GEOSGeom geosGeom)
{
	size_t nSize = 0;
    unsigned char *pabyBuf = NULL;
    OGRGeometry *poGeometry = NULL;
	
    pabyBuf = GEOSGeomToWKB_buf( geosGeom, &nSize );
    if( pabyBuf == NULL || nSize == 0 )
    {
        return NULL;
    }
	
    if( OGRGeometryFactory::createFromWkb( (unsigned char *) pabyBuf, 
										  NULL, &poGeometry, (int) nSize )
	   != OGRERR_NONE )
    {
        poGeometry = NULL;
    }
	
    if( pabyBuf != NULL )
    {
#ifndef  __WXMSW__    
		delete pabyBuf;
#endif
		//delete( pabyBuf );
    }
	
    return poGeometry;
	
}





OGRGeometry * tmGISDataVector::SafeBuffer (OGRGeometry * ogrgeom, int size)
{
	wxASSERT (ogrgeom);
	GEOSGeom geom = ogrgeom->exportToGEOS();
	GEOSGeom geombuffer;
	OGRGeometry * returnbuffer = NULL;
	
	if (geom != NULL)
	{
		geombuffer = GEOSBuffer(geom, size, 30);
		GEOSGeom_destroy(geom);
		
		if (geombuffer != NULL)
		{
			returnbuffer = tmGISDataVector::SafeCreateFromGEOS(geombuffer);
			GEOSGeom_destroy(geombuffer);
		}
	}
	
	return returnbuffer;
}

/***************************************************************************//**
 @brief Compute intersection
 @details This function try to bypass the Intersection() bug of GDAL by using
 GEOS directly
 @param geom1 The geometry to intersect
 @param geom2 the geometry intersecting
 @return  A valid OGRLineString or NULL
 @author Lucien Schreiber (c) CREALP 2008
 @date 18 November 2008
 *******************************************************************************/
OGRGeometry * tmGISDataVector::SafeIntersection(OGRGeometry * geom1, OGRGeometry * geom2)
{
	wxASSERT(geom1);
	wxASSERT(geom2);
	
	GEOSGeom  geosline = NULL;
	GEOSGeom  geosframe = NULL;
	GEOSGeom  geosintersect = NULL;
	OGRGeometry * returncrop = NULL;
	
	
	geosline = geom1->exportToGEOS();
	geosframe = geom2->exportToGEOS();
	
	wxASSERT(geosline);
	wxASSERT(geosframe);
	if (geosline != NULL && geosframe != NULL)
	{
		geosintersect = GEOSIntersection(geosline, geosframe);
		GEOSGeom_destroy(geosline);
		GEOSGeom_destroy(geosframe);
		
		if (geosintersect != NULL)
		{
			returncrop = SafeCreateFromGEOS(geosintersect);
			GEOSGeom_destroy(geosintersect);
		}
		
		
	}
	
	return returncrop;
}


/***************************************************************************//**
 @brief Compute Union
 @details This function try to bypass the Union() bug of GDAL by using
 GEOS directly
 @param union1 The multi-line string 
 @param line The line to intersect
 @return  A valid OGRLineString or NULL
 @author Lucien Schreiber (c) CREALP 2008
 @date 18 November 2008
 *******************************************************************************/
OGRGeometry * tmGISDataVector::SafeUnion (OGRGeometry * union1, OGRGeometry * line)
{
	wxASSERT(union1);
	wxASSERT(line);
	
	GEOSGeom  geosline = NULL;
	GEOSGeom  geosunion = NULL;
	GEOSGeom  geosresult = NULL;
	OGRGeometry * returnunion = NULL;
	
	
	
	geosline = line->exportToGEOS();
	geosunion = union1->exportToGEOS();
	if (geosline != NULL && union1 != NULL)
	{
		geosresult = GEOSUnion(geosunion, geosline);
		GEOSGeom_destroy(geosline);
		GEOSGeom_destroy(geosunion);
		
		if (geosresult != NULL)
		{
			returnunion = SafeCreateFromGEOS(geosresult);
			GEOSGeom_destroy(geosresult);
		}
		
	}
	
	return returnunion;
}



OGRGeometry * tmGISDataVector::CreateOGRGeometry(const wxRealPoint & pt)
{
	OGRGeometry * pPt = OGRGeometryFactory::createGeometry(wkbPoint);
	OGRPoint * mypPt = (OGRPoint*) pPt;
	wxASSERT (pPt);
	mypPt->setX(pt.x);
	mypPt->setY(pt.y);
	
	return pPt;
}


/***************************************************************************//**
 @brief Cut line in two at specified point
 @param line1 The original line to cut
 @param pointbuffer the point
 @param ptclicked the coordinate of the clicked point
 @param lineresult1 the resulting line 1
 @param lineresult2 the resulting line 2
 @return true if succeed, false otherwise
 @author Lucien Schreiber (c) CREALP 2009
 @date 06 February 2009
 *******************************************************************************/
bool tmGISDataVector::CutLineAtVertex (long oid, const wxRealPoint & clickedpt, int searchRadius, int layertype)
/*(OGRLineString * line1, OGRGeometry * pointbuffer,
									   wxRealPoint ptclicked,
									   OGRLineString & lineresult1,
									   OGRLineString & lineresult2)*/
{
	// get the line
	OGRLineString * myLine = (OGRLineString*) GetGeometryByOID(oid);
	if (myLine == NULL)
	{
		wxLogDebug(_T("%d line not found !"), oid);
		return false;
	}
	
	// create search buffer
	OGRPoint myClickPt;
	myClickPt.setX(clickedpt.x);
	myClickPt.setY(clickedpt.y);
	OGRGeometry * myClickBuffer = SafeBuffer(&myClickPt, searchRadius);
	wxASSERT(myClickBuffer);
	
	// iterate all vertex
	int iNumVertex = myLine->getNumPoints();
	OGRPoint myActualVertex;
	int iIntersectVertex = wxNOT_FOUND;
	for(int i = 0; i< iNumVertex;i++)
	{
		myActualVertex.setX(myLine->getX(i));
		myActualVertex.setY(myLine->getY(i));
		
		if (myActualVertex.Intersect(myClickBuffer))
		{
			iIntersectVertex = i;
			wxLogDebug(_T("Cutting @ vertex : %d"), iIntersectVertex);
			break;
		}
	}
	OGRGeometryFactory::destroyGeometry(myClickBuffer);
		
	
	// checks for boundary
	if (iIntersectVertex == 0 || iIntersectVertex == iNumVertex-1)
	{
		wxLogDebug(_T("Unable to cut at first or last vertex, try again"));
		OGRGeometryFactory::destroyGeometry(myLine);
		return false;
	}

	
	if (iIntersectVertex == wxNOT_FOUND)
	{
		OGRGeometryFactory::destroyGeometry(myLine);
		return false;	
	}
	
	
	OGRLineString myLine1;
	OGRLineString myLine2;
	// intersection @ vertex
	if (iIntersectVertex != -1)
	{
		// fill both geometry
		for (int j=0;j<iNumVertex;j++)
		{
			if (j <= iIntersectVertex)
			{
				myLine1.addPoint(myLine->getX(j), myLine->getY(j), myLine->getZ(j));
			}
			
			if (j >= iIntersectVertex)
			{
				myLine2.addPoint(myLine->getX(j), myLine->getY(j), myLine->getZ(j));
			}
		}
	}
	OGRGeometryFactory::destroyGeometry(myLine);
	
	
	// update geometry
	bool bupd = UpdateGeometry(&myLine1, oid);
	bool bAdd = AddGeometry(&myLine2, oid, layertype);
	wxASSERT(bupd);
	wxASSERT(bAdd);

	return true;
}



/***************************************************************************//**
 @brief Cut two lines where they intesects.
 @param line1 The first passed line for intersection
 @param line2 The second line passed for intersection
 @param res1 Resulting multi-lines for first passed lines
 @param res2 Resulting multi-lines for second passed line
 @return  true if the function works.
 @author Lucien Schreiber (c) CREALP 2009
 @date 10 February 2009
 *******************************************************************************/
bool tmGISDataVector::CutLineGeometry(OGRLineString * line1, OGRLineString * line2,
									   OGRMultiLineString & res1, 
									   OGRMultiLineString & res2)
{
	// get vertex position
	wxArrayInt myLine1VertexPos;
	wxArrayInt myLine2VertexPos;
	
	OGRLineString * myLine1WVertex;
	OGRLineString * myLine2WVertex;
	myLine1WVertex = GetLineWithIntersection(line1, line2, myLine1VertexPos);
	if (myLine1WVertex == NULL)
	{
		wxLogDebug(_T("Error inserting vertex"));
		return false;
	}
	myLine2WVertex = GetLineWithIntersection(line2, line1, myLine2VertexPos);
	if (myLine2WVertex == NULL)
	{
		wxLogDebug(_T("Error inserting vertex"));
		return false;
	}
	
	// LOG 
	for (unsigned int i = 0; i< myLine1VertexPos.GetCount(); i++)
		wxLogDebug(_T("Inserted Vertex @ pos : %d"), myLine1VertexPos.Item(i));
	// END LOG
	
	// cut lines @ specified vertex 
	SplitLinesAtVertex(myLine1WVertex, myLine1VertexPos , res1);
	SplitLinesAtVertex(myLine2WVertex, myLine2VertexPos, res2);
	
	OGRGeometryFactory::destroyGeometry(myLine1WVertex);
	OGRGeometryFactory::destroyGeometry(myLine2WVertex);

	return true;
}


/***************************************************************************//**
 @brief Cut one line with multiple line
 @param linetocut The line to cut
 @param cutlines Lines used for cutting
 @param results all resulting lines
 @return true if operation succeed, false otherwise
 @author Lucien Schreiber (c) CREALP 2009
 @date 12 February 2009
 *******************************************************************************/
bool tmGISDataVector::CutLineMultiple (OGRLineString * linetocut, 
									   OGRMultiLineString * cutlines,
									   OGRMultiLineString & results)
{
	// get vertex position
	wxArrayInt myLine1VertexPos;
	OGRLineString * myLine1WVertex;

	myLine1WVertex = GetLineWithIntersection(linetocut, cutlines, myLine1VertexPos);
	if (myLine1WVertex == NULL)
	{
		wxLogDebug(_T("Error inserting vertex"));
		return false;
	}
	
	// LOG 
	//for (unsigned int i = 0; i< myLine1VertexPos.GetCount(); i++)
	//	wxLogDebug(_T("Inserted Vertex @ pos : %d"), myLine1VertexPos.Item(i));
	// END LOG
	
	// cut lines @ specified vertex 
	//results.addGeometry(myLine1WVertex);
	SplitLinesAtVertex(myLine1WVertex, myLine1VertexPos , results);
	
	OGRGeometryFactory::destroyGeometry(myLine1WVertex);
	return true;
}


/***************************************************************************//**
 @brief Get a line with all intersection vertex
 @param line line to intersect
 @param intersection line used for intersection
 @param insertedvertex position of inserted vertex
 @return A line with inserted vertex. NULL if an error occur.
 (call should take care of deleting passed object)
 @author Lucien Schreiber (c) CREALP 2009
 @date 11 February 2009
 *******************************************************************************/
OGRLineString * tmGISDataVector::GetLineWithIntersection (OGRLineString * line, 
														  OGRLineString * intersection,
														  wxArrayInt & insertedvertex)
{	
	OGRPoint p1;
	OGRPoint p2;
	OGRLineString segment;
	OGRLineString * myResLine = (OGRLineString*) OGRGeometryFactory::createGeometry(wkbLineString);
	int iNumLineVertex = line->getNumPoints();

	
	for (int i = 0; i< iNumLineVertex; i++)
	{
		line->getPoint(i, &p1);
		myResLine->addPoint(&p1);
		
		if (i+1 < iNumLineVertex)
		{
			line->getPoint(i+1, &p2);
			segment.addPoint(&p1);
			segment.addPoint(&p2);
			
			// intersection found
			if (segment.Intersect(intersection))
			{
				OGRGeometry * myGeomIntersection = SafeIntersection(&segment, intersection); 
				wxASSERT (myGeomIntersection);
				// simple intersection
				if (wkbFlatten(myGeomIntersection->getGeometryType()) == wkbPoint)
				{
					myResLine->addPoint(((OGRPoint*) myGeomIntersection));
					insertedvertex.Add(myResLine->getNumPoints()-1);
				}
				// multiple intersections
				else if (wkbFlatten(myGeomIntersection->getGeometryType()) == wkbMultiPoint)
				{
					OGRMultiPoint * myPts = (OGRMultiPoint*) myGeomIntersection;
					for (int j = 0; j< myPts->getNumGeometries();j++)
					{
						myResLine->addPoint((OGRPoint*)myPts->getGeometryRef(j));
						insertedvertex.Add(myResLine->getNumPoints()-1);
					}
				}
				// error
				else
				{
					wxASSERT_MSG (0, _T("This case isn't taken into account"));
					return NULL;
				}
				
				
				
				OGRGeometryFactory::destroyGeometry(myGeomIntersection);
			}

			segment.empty();
		}
		
	}
	
	return myResLine;
	
}



/***************************************************************************//**
 @brief Get a line with intersection vertex insered.
 @param line the line to intersect
 @param multiline the multiline used for intersection
 @param intertedvertex the position (index) of inserted vertex
 @return  A line containing inserted vertex
 @author Lucien Schreiber (c) CREALP 2009
 @date 12 February 2009
 *******************************************************************************/
OGRLineString * tmGISDataVector::GetLineWithIntersection(OGRLineString * line, 
										OGRMultiLineString * multiline,
										wxArrayInt & intertedvertex)
{
	OGRPoint p1;
	OGRPoint p2;
	OGRLineString segment;
	OGRLineString * myResLine = (OGRLineString*) OGRGeometryFactory::createGeometry(wkbLineString);
	int iNumLineVertex = line->getNumPoints();
	
	
	for (int i = 0; i< iNumLineVertex; i++)
	{
		line->getPoint(i, &p1);
		myResLine->addPoint(&p1);
		wxLogDebug(_T("Adding base point %d"), i);
		
		if (i+1 < iNumLineVertex)
		{
			line->getPoint(i+1, &p2);
			segment.addPoint(&p1);
			segment.addPoint(&p2);
			
			// loop for all lines
			for (int l = 0; l < multiline->getNumGeometries(); l++)
			{
				OGRLineString * intersection = (OGRLineString*) multiline->getGeometryRef(l);
				wxASSERT (intersection);
				// intersection found
				if (segment.Intersect(intersection))
				{
					//TODO : Order vertex by distance
					OGRGeometry * myGeomIntersection = SafeIntersection(&segment, intersection); 
					wxASSERT (myGeomIntersection);
					// simple intersection
					if (wkbFlatten(myGeomIntersection->getGeometryType()) == wkbPoint)
					{
						myResLine->addPoint(((OGRPoint*) myGeomIntersection));
						intertedvertex.Add(myResLine->getNumPoints()-1);
						wxLogDebug(_T("Adding intersection point %d"), myResLine->getNumPoints()-1);
					}
					// multiple intersections
					else if (wkbFlatten(myGeomIntersection->getGeometryType()) == wkbMultiPoint)
					{
						OGRMultiPoint * myPts = (OGRMultiPoint*) myGeomIntersection;
						for (int j = 0; j< myPts->getNumGeometries();j++)
						{
							myResLine->addPoint((OGRPoint*)myPts->getGeometryRef(j));
							intertedvertex.Add(myResLine->getNumPoints()-1);
							wxLogDebug(_T("Adding intersection (multi) point %d"), myResLine->getNumPoints()-1);
						}
					}
					// error
					else
					{
						wxASSERT_MSG (0, _T("This case isn't taken into account"));
						return NULL;
					}
					
					
					
					OGRGeometryFactory::destroyGeometry(myGeomIntersection);
				}
				
			}
			segment.empty();
		}
		
	}
	
	return myResLine;
	
}


/***************************************************************************//**
 @brief Save splitted geometry to file
 @details The first geometry is updated, others are added into database.
 @param gCol the collection of geometries to update
 @param oid the oid
 @param AddedIds All IDs added.
 @param layertype one of the TOC_GENERIC_NAME values
 @return  true if all operations are successfull
 @author Lucien Schreiber (c) CREALP 2009
 @date 11 February 2009
 *******************************************************************************/
bool tmGISDataVector::SplitGeometry (OGRGeometryCollection * gCol, const long & oid,
									 wxArrayLong & AddedIds, int layertype)
{
	if (UpdateGeometry(gCol->getGeometryRef(0), oid) == false)
		return false;
	long lAdded = -1;
	for (int i = 1;i < gCol->getNumGeometries(); i++)
	{
		lAdded = AddGeometry(gCol->getGeometryRef(i), -1, layertype);
		if (lAdded == -1)
			return false;
		AddedIds.Add(lAdded);
	}
		
	return true;
}


/***************************************************************************//**
 @brief Split line at specified vertex
 @param line The line to split
 @param splitpos an array containing the position to split
 @param splitedline return all splitted lines
 @return  true if spliting line works, false otherwise
 @author Lucien Schreiber (c) CREALP 2009
 @date 11 February 2009
 *******************************************************************************/
bool tmGISDataVector::SplitLinesAtVertex (OGRLineString * line,
										  const wxArrayInt & splitpos,
										  OGRMultiLineString & splitedline)
{
	OGRLineString myLine;
	OGRPoint pt;
	//int lastpos = 0;
	int splititem = 0;
	
	// security
	if (splitpos.GetCount() == 0)
		return false;
		
	
	for (int i = 0 ; i< line->getNumPoints(); i++)
	{
		line->getPoint(i, &pt);
		myLine.addPoint(&pt);
		
		if (i == splitpos.Item(splititem))
		{
			wxLogDebug(_T("Cutting @ %d - Position : %.*f / %.*f"), splitpos.Item(splititem), 2,
					   pt.getX(), 2, pt.getY());
			splitedline.addGeometry(&myLine);
			myLine.empty();
			myLine.addPoint(&pt);
			splititem++;
			if (splititem >= (signed) splitpos.GetCount())
				splititem = 0;
		}
		
	}
	
	if (myLine.getNumPoints() >= 2)
		splitedline.addGeometry(&myLine);
	
	
	return true;
	
}




/***************************************************************************//**
 @brief Compute intersection
 @details between two line. The vertexindex is used to create a segment user for
 intersection
 @param line line used for creating segment for intersection
 @param intersection line used for intersecting
 @param vertexindex contain the vertex index, intersection should occur between
 vertexindex  and vertexindex + 1
 @param resultpos the position of the intersections
 @return true if point returned
 @author Lucien Schreiber (c) CREALP 2009
 @date 11 February 2009
 *******************************************************************************/
bool tmGISDataVector::ComputeLineLineIntersection (OGRLineString * line, 
												   OGRLineString * intersection,
												   const wxArrayInt & vertexindex,
												   OGRMultiPoint & resultpos)
{
	//TODO: Remove this function if no more used
	OGRLineString segment;
	OGRPoint p1;
	OGRPoint p2;
	bool bReturn = true;
	
	for (unsigned int i = 0; i< vertexindex.GetCount();i++)
	{
		line->getPoint(vertexindex.Item(i) -1,&p1);
		line->getPoint(vertexindex.Item(i), &p2);
		segment.addPoint(&p1);
		segment.addPoint(&p2);
		
		OGRGeometry * myIntersectionPoint = SafeIntersection(&segment, intersection);
		if (myIntersectionPoint)
		{
			if (wkbFlatten(myIntersectionPoint->getGeometryType()) == wkbPoint)
			{
				OGRPoint * myPt = (OGRPoint*) myIntersectionPoint;
				resultpos.addGeometryDirectly(myPt);
			}
			else
			{
				wxLogError(_T("Result of intersection isn't a point !"));
				bReturn = false;
			}
			
			//OGRGeometryFactory::destroyGeometry(myIntersectionPoint);
		}
		segment.empty();
	}
	return bReturn;
}


/***************************************************************************//**
 @brief Insert a vertex in a passed line
 @param pointbuffer the buffer (polygon) intersecting the line
 @param ptclicked the coordinate of the clicked point
 @param line the line
 @param inseredvertex return the number of the inserted vertex
 @return  The new line with the inserted vertex (caller should delete the 
 returned Geometry)
 @author Lucien Schreiber (c) CREALP 2009
 @date 06 February 2009
 *******************************************************************************/
OGRLineString * tmGISDataVector::InsertVertex (OGRGeometry * pointbuffer, wxRealPoint ptclicked,
								   OGRLineString * line, int & inseredvertex)
{
	
	// check segment by segment
	OGRPoint p1;
	OGRPoint p2;
	OGRLineString segment;
	OGRLineString * returnedLine = NULL;
	inseredvertex = -1;
	
	
	for (int i = 0;i < line->getNumPoints(); i++)
	{
		line->getPoint(i, &p1);
		
		if (i+1 < line->getNumPoints())
		{
			line->getPoint(i+1, &p2);
			segment.addPoint(&p1);
			segment.addPoint(&p2);
			
			// intersection found
			if (segment.Intersect(pointbuffer))
			{
				inseredvertex = i+1;
				wxLogDebug(_T("Intersection between vertex %d - %d"), i, inseredvertex);
			}
			
			segment.empty();
		}
		
		// end the loop when intersection found
		if (inseredvertex != -1)
			break;
		
	}
	
	
	// if intersection found, add vertex
	returnedLine = (OGRLineString*) OGRGeometryFactory::createGeometry(wkbLineString);
	for (int j = 0; j < line->getNumPoints(); j++)
	{
		line->getPoint(j, &p1);
		returnedLine->addPoint(&p1);
		
		if (j == inseredvertex-1)
		{
			p2.setX(ptclicked.x);
			p2.setY(ptclicked.y);
			returnedLine->addPoint(&p2);
		}
		
	}
	
	return returnedLine;
}



/***************************************************************************//**
 @brief Insert multiple vertex into line
 @details This function insert vertex at the right place into a line, based on
 where intersect the vertex to insert.
 @param line the passed geometry
 @param vertex an OGRMultiPoint structure containing all vertex to insert
 @param point_pos  index where the vertex are to be insered
 @return  The modified line with inserted vertex (must be destroyed by caller)
 @author Lucien Schreiber (c) CREALP 2009
 @date 10 February 2009
 *******************************************************************************/
OGRLineString * tmGISDataVector::InsertVertexMultiple (OGRLineString * line, 
													   OGRMultiPoint * vertex, 
													   const wxArrayInt & point_pos)
{
	// if intersection found, add multiple vertex
	OGRLineString * returnedLine;
	OGRPoint p1;
	OGRPoint p2;
	returnedLine = (OGRLineString*) OGRGeometryFactory::createGeometry(wkbLineString);
	int PointTruePos = 0;
	int RealIndex = 0;
	
	for (int k = 0; k < line->getNumPoints(); k++)
	{
		line->getPoint(k, &p1);
		returnedLine->addPoint(&p1);
		
		for (unsigned int l = 0; l<point_pos.GetCount();l++)
		{
			PointTruePos = point_pos.Item(l) -1 ; //+ (signed) l;
			if (k == PointTruePos)
			{
				OGRPoint * myPoint = (OGRPoint*) vertex->getGeometryRef(l);
				wxASSERT(myPoint);
				p2.setX(myPoint->getX());
				p2.setY(myPoint->getY());
				returnedLine->addPoint(&p2);
				RealIndex++;
			}
		}
		RealIndex++;
		
	}
	
	return returnedLine;
	
}



/***************************************************************************//**
 @brief Merge lines
 @param linetomerge lines to merge
 @param linemerged the resulting geometry if function return true. 
 @return true if merging was successfull, false otherwise
 @author Lucien Schreiber (c) CREALP 2009
 @date 27 February 2009
 *******************************************************************************/
bool tmGISDataVector::LinesMerge (OGRMultiLineString * linetomerge,
								  OGRGeometry ** linemerged)
{
	//bool bReturn = true;
	GEOSGeom myLinesToMerge = linetomerge->exportToGEOS();
	GEOSGeom myResult = GEOSLineMerge(myLinesToMerge);
	GEOSGeom_destroy(myLinesToMerge);
	if (!myResult)
		return false;
	
	*(linemerged) = SafeCreateFromGEOS(myResult);
	OGRGeometry * myMerge = *(linemerged);
	if (myMerge == NULL)
		return false;
	
	OGRwkbGeometryType myType = wkbFlatten(myMerge->getGeometryType());
	if (myType != wkbLineString)
	{
		wxLogDebug(_T("Unable to merge lines, they aren't adjacent."));
		return false;
	}
			
	return true;
}

