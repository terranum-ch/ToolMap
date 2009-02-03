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
 @param bool true if a vertex exists at the specified index, false otherwise
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
	
	//wxASSERT(index > myMemLine->getNumPoints());
	
	pt.x = myMemLine->getX(index);
	pt.y = myMemLine->getY(index);
	
	return true;
}


