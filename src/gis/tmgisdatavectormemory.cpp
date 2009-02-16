/***************************************************************************
							tmgisdatavectormemory.h
                    For keeping line in memory during drawing
                             -------------------
    copyright            : (C) 2009 CREALP Lucien Schreiber 
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

#include "tmgisdatavectormemory.h"


/***************************************************************************//**
 @brief Constructor
 @author Lucien Schreiber (c) CREALP 2009
 @date 03 February 2009
 *******************************************************************************/
tmGISDataVectorMemory::tmGISDataVectorMemory()
{
	// init member
	m_Layer = NULL;
	m_Feature = NULL;
	
	 OGRRegisterAll();
	// create memory layer
	const char *pszDriverName = "Memory";
	OGRSFDriver *poDriver;
	poDriver = OGRSFDriverRegistrar::GetRegistrar()->
		GetDriverByName( pszDriverName );
    if( poDriver == NULL )
    {
		wxASSERT_MSG(0, _T(" driver not available."));
		return;
    }
	

	m_Datasource = poDriver->CreateDataSource(NULL,NULL);
    if( m_Datasource == NULL )
    {
		wxASSERT_MSG(0,_T("Creation of output file failed." ));
		return;
    }
	
    
    m_Layer = m_Datasource->CreateLayer( "memory_line", NULL, wkbLineString, NULL );
    if( m_Layer == NULL )
    {
        wxASSERT_MSG(0,_T( "Layer creation failed."));
        return;
    }
	
}


/***************************************************************************//**
 @brief Desctuctor
 @author Lucien Schreiber (c) CREALP 2009
 @date 03 February 2009
 *******************************************************************************/
tmGISDataVectorMemory::~tmGISDataVectorMemory()
{
	DestroyFeature();
}


/***************************************************************************//**
 @brief Create a line feature
 @details Create a line feature for storing editing line data in memory. This
 function destroy any existing feature if existing
 @return true if a feature was created and attached into the layer, false 
 otherwise.
 @author Lucien Schreiber (c) CREALP 2009
 @date 03 February 2009
 *******************************************************************************/
bool tmGISDataVectorMemory::CreateFeature()
{
	DestroyFeature();
	m_Feature = OGRFeature::CreateFeature(m_Layer->GetLayerDefn());
	if (m_Layer->CreateFeature(m_Feature) != OGRERR_NONE)
	{
		wxASSERT_MSG(0,_T("Error creating feature in memory"));
		return false;
	}
	
	return true;
}



/***************************************************************************//**
 @brief Destroy line feature
 @details If a feature exists, it is destroyed. One can call this as often as
 needed
 @return true if a feature exists and was deleted, false if no feature exists.
 @author Lucien Schreiber (c) CREALP 2009
 @date 03 February 2009
 *******************************************************************************/
bool tmGISDataVectorMemory::DestroyFeature()
{
	wxASSERT(m_Layer);
	if (m_Layer->GetFeatureCount() > 0)
	{
		m_Layer->DeleteFeature(m_Feature->GetFID());
		wxASSERT(m_Feature);
		OGRFeature::DestroyFeature(m_Feature);
		m_Feature = NULL;
		return true;
	}
	
	return false;
}



/***************************************************************************//**
 @brief Add or insert a vertex into the memory line
 @param pt the coordinate to add (real)
 @param index where we should insert the vertex (-1 means add the vertex at the
 end). If no vertex is present into the line, a new one is created regardless
 of the index value passed
 @return  true if the vertex was added correctly
 @author Lucien Schreiber (c) CREALP 2009
 @date 03 February 2009
 *******************************************************************************/
bool tmGISDataVectorMemory::InsertVertex (const wxRealPoint & pt, int index)
{
	wxASSERT(m_Feature);
	if (m_Feature == NULL)
		return false;
	
	OGRLineString * myMemLine = NULL;
	if (GetVertexCount() == 0)
	{
		OGRLineString line;
		m_Feature->SetGeometry(&line);
	}
	myMemLine = (OGRLineString*) m_Feature->GetGeometryRef();
	
		
	// Adding vertex
	if (index == -1 || myMemLine->IsEmpty())
	{
		myMemLine->addPoint(pt.x, pt.y, 0);
	}
	else // Inserting vertex
	{
		// getting line points 
		int numPoints = myMemLine->getNumPoints();
		OGRRawPoint * myRawPoints = new OGRRawPoint[sizeof(OGRRawPoint*) * numPoints];
		myMemLine->getPoints(myRawPoints, NULL);
		
		// cleaning line
		myMemLine->empty();
		
		// inserting points
		for(int i = 0; i< numPoints;i++)
		{
		
			if (i == index)
			{
				myMemLine->addPoint(pt.x, pt.y, 0);
			}
			myMemLine->addPoint(myRawPoints[i].x, myRawPoints[i].y);
			
		}
		delete [] myRawPoints;
	}
	return true;
}



/***************************************************************************//**
 @brief Remove a vertex from the memory line
 @param index Where we should remove the vertex, -1 means we remove the last
 vertex
 @return  true if vertex was removed, false otherwise
 @author Lucien Schreiber (c) CREALP 2009
 @date 03 February 2009
 *******************************************************************************/
bool tmGISDataVectorMemory::RemoveVertex (int index)
{
	wxASSERT(m_Feature);
	if (m_Feature == NULL)
		return false;
	
	OGRLineString * myMemLine = (OGRLineString*) m_Feature->GetGeometryRef();
	if (myMemLine == NULL)
		return false;
	
	// getting line points 
	int numPoints = myMemLine->getNumPoints();
	wxASSERT(numPoints > 0);
	OGRRawPoint * myRawPoints = new OGRRawPoint[sizeof(OGRRawPoint*) * numPoints];
	myMemLine->getPoints(myRawPoints, NULL);
	
	// cleaning line
	myMemLine->empty();
	
	if (index == -1) 
		index = numPoints-1;
	
	// inserting points
	for(int i = 0; i< numPoints;i++)
	{
		if (i != index)
		{
			myMemLine->addPoint(myRawPoints[i].x, myRawPoints[i].y);
		}
	}
	delete [] myRawPoints;
	return true;
}



/***************************************************************************//**
 @brief Count the vertex
 @return  The number of vertex in the memory line
 @author Lucien Schreiber (c) CREALP 2009
 @date 03 February 2009
 *******************************************************************************/
int tmGISDataVectorMemory::GetVertexCount()
{
	int iReturn = 0;
	if (m_Feature)
	{
		OGRLineString * myMemLine = (OGRLineString*) m_Feature->GetGeometryRef();
		if (myMemLine)
			iReturn = myMemLine->getNumPoints();
	}
	
	return iReturn;
}



/***************************************************************************//**
 @brief Get a specified vertex
 @param pt if true is returned, will contain a valid vertex
 @param index the vertex we want back (-1, means the last vertex)
 @return true if a vertex exists at the specified index, false otherwise
 @author Lucien Schreiber (c) CREALP 2009
 @date 03 February 2009
 *******************************************************************************/
bool tmGISDataVectorMemory::GetVertex (wxRealPoint & pt, int index)
{
	if (m_Feature == NULL)
		return false;
	
	OGRLineString * myMemLine = (OGRLineString*) m_Feature->GetGeometryRef();
	if (myMemLine == NULL)
		return false;
		
	if (index == -1)
		index = myMemLine->getNumPoints() -1;
	
	
	pt.x = myMemLine->getX(index);
	pt.y = myMemLine->getY(index);
	
	return true;
}


/***************************************************************************//**
 @brief Get all vertex from the line
 @param myPts An array of wxRealPoints containing all vertex 
 @return true if points were returned, false otherwise
 @author Lucien Schreiber (c) CREALP 2009
 @date 05 February 2009
 *******************************************************************************/
bool tmGISDataVectorMemory::GetVertexAll (wxArrayRealPoints & myPts)
{
	if (m_Feature == NULL)
		return false;
	
	OGRLineString * myMemLine = (OGRLineString*) m_Feature->GetGeometryRef();
	if (myMemLine == NULL)
		return false;
	
	
	
	int nb_pts = myMemLine->getNumPoints();
	//wxRealPoint * myPts = new wxRealPoint[nb_pts-1];
	wxRealPoint myPoint;
	
	for(int i = 0; i< nb_pts;i++)
	{
		myPoint.x = myMemLine->getX(i);
		myPoint.y = myMemLine->getY(i);
		myPts.Add(myPoint);
	}
	
	return true;
}


/***************************************************************************//**
 @brief Save into database the first vertex in the feature
 @param database Adress of a valid database
 @param layertype one of the #TOC_GENERIC_NAME values (must be <
 TOC_NAME_NOT_GENERIC)
 @return the ID of the insered point or -1 if an error occur
 @author Lucien Schreiber (c) CREALP 2009
 @date 04 February 2009
 *******************************************************************************/
long tmGISDataVectorMemory::SavePointToDatabase (DataBaseTM * database,
												 int layertype)
{
	wxRealPoint myPt; 
	if (GetVertex(myPt, 0) == false)
		return false;
	
	wxASSERT (database);
	wxASSERT (layertype < TOC_NAME_NOT_GENERIC);
	
	OGRPoint * myOGRPoint = (OGRPoint*) OGRGeometryFactory::createGeometry(wkbPoint);
	myOGRPoint->setX(myPt.x);
	myOGRPoint->setY(myPt.y);
	//myOGRPoint->flattenTo2D()
	
	long lReturn = SaveDatabaseGeometry((OGRGeometry*) myOGRPoint, layertype, database);
	OGRGeometryFactory::destroyGeometry(myOGRPoint);
	return lReturn;
}


/***************************************************************************//**
 @brief Save the line into database
 @param database a valid #DataBaseTM object
 @param layertype one of the #TOC_GENERIC_NAME values (must be <
 TOC_NAME_NOT_GENERIC) 
 @return  the OID of the inserted line or -1 if an error occur
 @author Lucien Schreiber (c) CREALP 2009
 @date 05 February 2009
 *******************************************************************************/
long tmGISDataVectorMemory::SaveLineToDatabase (DataBaseTM * database,
												int layertype)
{
	wxASSERT (database);
	wxASSERT (layertype < TOC_NAME_NOT_GENERIC);
	
	OGRLineString * myOGRLine = (OGRLineString*) m_Feature->GetGeometryRef();
	if (myOGRLine == NULL)
		return -1;
	
	// ensure only 2D
	myOGRLine->setCoordinateDimension(2);
	
	return SaveDatabaseGeometry((OGRGeometry*) myOGRLine, layertype, database);
}






/***************************************************************************//**
 @brief Save any geometry into database
 @param myGeom a valid geometry (the caller must delete the geometry)
 @param ilayertype one of the #TOC_GENERIC_NAME values (must be <
 TOC_NAME_NOT_GENERIC)
 @param database a valid #DataBaseTM object
 @return  the ID of stored item into database or -1 if an error occur
 @author Lucien Schreiber (c) CREALP 2009
 @date 04 February 2009
 *******************************************************************************/
long tmGISDataVectorMemory::SaveDatabaseGeometry (OGRGeometry * myGeom,
												  int ilayertype,
												  DataBaseTM * database)
{
	long lReturn = -1;
	char * myCharGeom = NULL;
	myGeom->exportToWkt(&myCharGeom);
	if (!myCharGeom)
		return -1;
	
	wxString mySGeom = wxString::FromAscii(myCharGeom);
#ifndef  __WXMSW__    
	delete [] myCharGeom;	
#endif
	
		
	wxString sSentence = wxString::Format(_T("INSERT INTO %s (OBJECT_GEOMETRY)")
										  _T(" VALUES (GeomFromText('%s'));"),
										  TABLE_NAME_GIS_GENERIC[ilayertype].c_str(),
										  mySGeom.c_str());
	if (!database->DataBaseQueryNoResult(sSentence))
	{
		wxLogDebug(_T("Error inserting geometry %s into database : %s"),
				   sSentence.c_str(),
				   database->DataBaseGetLastError().c_str());
		return -1;
	}
	
		
	lReturn = database->DataBaseGetLastInsertID();
	return lReturn;
}



/***************************************************************************//**
 @brief Load any geometry from database
 @param oid the OID of the searched Geometry
 @param ilayertype one of the #TOC_GENERIC_NAME values (must be <
 TOC_NAME_NOT_GENERIC)
 @param database a valid #DataBaseTM object
 @return  A valid OGRGeometry if ok (caller must destroy the geometry) or NULL
 if an error occur
 @author Lucien Schreiber (c) CREALP 2009
 @date 04 February 2009
 *******************************************************************************/
OGRGeometry * tmGISDataVectorMemory::LoadDatabaseGeometry (long oid,
									int ilayertype,
									DataBaseTM * database)
{
	wxString sSentence = wxString::Format(_T("SELECT (OBJECT_GEOMETRY)")
										  _T(" FROM %s WHERE OBJECT_ID=%d;"),
										  TABLE_NAME_GIS_GENERIC[ilayertype].c_str(),
										  oid);
	if (!database->DataBaseQuery(sSentence))
	{
		wxLogDebug(_T("Error getting geometry for id %d - %s"),
				   oid, database->DataBaseGetLastError().c_str());
		return NULL;
	}
	
	
	unsigned long *  row_length;
	MYSQL_ROW row;
	
	row_length = database->DataBaseGetNextRowResult(row);
	if (row_length == NULL)
	{
		wxLogDebug(_T("No geometry found for id : %d"), oid);
		return NULL;
	}
	
	OGRGeometry * geometry = NULL;
	// Geometry columns will have the first 4 bytes contain the SRID.
	OGRGeometryFactory::createFromWkb(((unsigned char *)row[0]) + 4, 
									  NULL,
									  &geometry,
									  row_length[0] - 4 );
	
	return geometry;
}



/***************************************************************************//**
 @brief Get a specified point from database
 @param database a valid #DataBaseTM object
 @param oid the OID of the searched point
 @param layertype one of the #TOC_GENERIC_NAME values (must be <
 TOC_NAME_NOT_GENERIC)
 @return  A wxRealPoint or -1,-1 if an error occur
 @author Lucien Schreiber (c) CREALP 2009
 @date 04 February 2009
 *******************************************************************************/
wxRealPoint tmGISDataVectorMemory::GetPointFromDatabase (DataBaseTM * database,
														 long oid,
														 int layertype)
{
	OGRPoint * myOGRPoint = (OGRPoint*) LoadDatabaseGeometry(oid, 
													layertype, database);
	if (myOGRPoint == NULL)
		return wxRealPoint(-1,-1);
	
	wxRealPoint myPoint (myOGRPoint->getX(), myOGRPoint->getY());
	OGRGeometryFactory::destroyGeometry(myOGRPoint);
	
	return myPoint;
}


/***************************************************************************//**
 @brief Get a specified line from database
 @param database a valid #DataBaseTM object
 @param oid the OID of the searched line
 @param layertype one of the #TOC_GENERIC_NAME values (must be <
 TOC_NAME_NOT_GENERIC)
 @return  true if the line was loaded or false if an error occur
 @author Lucien Schreiber (c) CREALP 2009
 @date 05 February 2009
 *******************************************************************************/
bool tmGISDataVectorMemory::GetLineFromDatabase (DataBaseTM * database, long oid, 
												 int layertype)
{
	
		
	OGRLineString * myOGRLine = (OGRLineString*) LoadDatabaseGeometry(oid,
																	  layertype,
																	  database);
	if (myOGRLine == NULL)
		return false;
	
	m_Feature->SetGeometry(myOGRLine);
	OGRGeometryFactory::destroyGeometry(myOGRLine);
	return true;
}

