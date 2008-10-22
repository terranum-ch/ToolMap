/***************************************************************************
								tmgisdatavectorshp.cpp
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

#include "tmgisdatavectorshp.h"



tmGISDataVectorSHP::tmGISDataVectorSHP()
{
	m_Datasource = NULL;
	m_Layer = NULL;
	m_Feature = NULL;
	m_polyTotalRings = 0;
}



tmGISDataVectorSHP::~tmGISDataVectorSHP()
{
	// safe destroy the datasource if needed
	if (m_Datasource)
		OGRDataSource::DestroyDataSource(m_Datasource);
}



bool tmGISDataVectorSHP::Open (const wxString & filename, bool bReadWrite)
{
	// init parent member values
	tmGISData::Open(filename, bReadWrite);
	
	// convert utf wxString into char *
	//const char* ascii_str = 
	char * buffer = new char [filename.Length()+2];
	strcpy(buffer, (const char*)filename.mb_str(wxConvUTF8));
	
	// open the shapefile and return true if success
	m_Datasource = OGRSFDriverRegistrar::Open(buffer, FALSE );
	if( m_Datasource==NULL)
	{
		if (IsLoggingEnabled())
			wxLogDebug(_T("Unable to open shp : %s"), filename.c_str());
		return FALSE;
	}
	
	m_Layer = m_Datasource->GetLayer(0);
	wxASSERT (m_Layer);
	
	delete buffer;
	return TRUE;
}



tmRealRect tmGISDataVectorSHP::GetMinimalBoundingRectangle()
{
	wxASSERT(m_Layer);
	
	OGREnvelope myEnveloppe;
	OGRErr myError = m_Layer->GetExtent(&myEnveloppe, TRUE);
	if (myError == OGRERR_FAILURE)
	{
		if (IsLoggingEnabled())
			wxLogDebug(_T("Unable to compute extend for %s"), GetShortFileName().c_str());
		return tmRealRect(0,0,0,0);
	}
	
	return tmRealRect(myEnveloppe.MinX, myEnveloppe.MinY,
					  myEnveloppe.MaxX, myEnveloppe.MaxY);

}




TM_GIS_SPATIAL_TYPES tmGISDataVectorSHP::GetSpatialType ()
{
	OGRGeometry *poGeometry;
	OGRFeature *poFeature;
	TM_GIS_SPATIAL_TYPES retvalue = LAYER_ERR;
	
	wxASSERT(m_Layer);
	
	// computing features count, not able to know the
	// spatial type if no features are present.
	if (m_Layer->GetFeatureCount () <= 0)
	{
		if (IsLoggingEnabled())
			wxLogError(_("Unable to add the %s layer, layer is empty"),
					   GetShortFileName().c_str());
		return LAYER_SPATIAL_UNKNOWN;
	}
	
	// computing layer type (point, line, polygon or unknown)
	if ((poFeature = m_Layer->GetNextFeature()) == NULL)
	{
		if (IsLoggingEnabled())
			wxLogError(_("Unable to read feature from : %s, layer may be corrupted"),
					   GetShortFileName().c_str());
		return LAYER_SPATIAL_UNKNOWN;
	}
		
	
	poGeometry = poFeature->GetGeometryRef();
	if( poGeometry != NULL)
		{	
			switch (wkbFlatten(poGeometry->getGeometryType()))
			{
				case wkbLineString:
					retvalue = LAYER_SPATIAL_LINE;
					break;
				case wkbPoint:
					retvalue = LAYER_SPATIAL_POINT;
					break;
				case wkbPolygon:
					retvalue = LAYER_SPATIAL_POLYGON;
					break;
				default:
					retvalue = LAYER_SPATIAL_UNKNOWN;
					break;
			}
		}
	
	 OGRFeature::DestroyFeature( poFeature );
	
	if (retvalue == LAYER_ERR)
	{
		if (IsLoggingEnabled())
			wxLogDebug(_T("Error getting spatial layer type for : %s"), 
					   GetShortFileName().c_str());
	}
	
	return retvalue;
}



bool tmGISDataVectorSHP::SetSpatialFilter (tmRealRect filter, int type)
{
	wxASSERT(m_Layer);

	
	/* clearing filter...
	if (filter == tmRealRect(0,0,0,0))
	{
		m_Layer->SetSpatialFilter(NULL);
	}
	else 
	{
		m_Layer->SetSpatialFilterRect(filter.x_max, filter.y_min,
									  filter.x_max, filter.y_max);
	}
	*/
	m_Layer->ResetReading();
	return TRUE;
}



wxRealPoint * tmGISDataVectorSHP::GetNextDataLine (int & nbvertex)
{
	wxASSERT(m_Layer);
	OGRFeature * poFeature = m_Layer->GetNextFeature();
	
	// nothing more to read
	if (poFeature == NULL)
	{
		nbvertex = 0;
		return NULL;		
	}
	
	
	OGRLineString * pline = (OGRLineString*) poFeature->GetGeometryRef();
	wxASSERT(pline);	
	
	// normal reading
	nbvertex = pline->getNumPoints();
	if (nbvertex <= 1)
	{
		if (IsLoggingEnabled())
			wxLogDebug(_T("Only one vertex or less in this line ???"));
		OGRGeometryFactory::destroyGeometry	(pline);
		return NULL;
	}
	
	wxRealPoint * pts = new wxRealPoint[nbvertex];
	for (int i=0; i<nbvertex;i++)
	{
		pts[i].x = pline->getX(i);
		pts[i].y = pline->getY(i);
	}
	OGRFeature::DestroyFeature( poFeature );
	return pts;
}



wxRealPoint * tmGISDataVectorSHP::GetNextDataPoint ()
{
	wxASSERT(m_Layer);
	OGRFeature * poFeature = m_Layer->GetNextFeature();
	
	// nothing more to read
	if (poFeature == NULL)
	{
		return NULL;		
	}
	
	
	OGRPoint * pPoint = (OGRPoint*) poFeature->GetGeometryRef();
	wxASSERT(pPoint);	
	
	wxRealPoint * pts = new wxRealPoint();
	pts->x = pPoint->getX();
	pts->y = pPoint->getY();
	
	OGRFeature::DestroyFeature( poFeature );
	return pts;
}



int tmGISDataVectorSHP::GetNextDataPolygonInfo ()
{
	wxASSERT(m_Layer);
	m_Feature = m_Layer->GetNextFeature();
	
	// nothing more to read
	if (m_Feature == NULL)
	{
		m_polyTotalRings = 0;
		return 0;
	}
	
	OGRPolygon * plgon = (OGRPolygon*) m_Feature->GetGeometryRef();
	wxASSERT(plgon);	
	
	if (plgon == NULL)
	{
		if (IsLoggingEnabled())
			wxLogDebug(_T("Conversion to polygon error"));
		m_polyTotalRings = 0;
		OGRFeature::DestroyFeature( m_Feature );
		return 0;
	}
	
	// check polygons validity, long operations ??
	if(!plgon->IsValid())
	{
		if (IsLoggingEnabled())
			wxLogDebug(_T("Polygon not valid @ FID : %d"), m_Feature->GetFID());
	}
	
	// count rings + 1 (exterior ring)
	m_polyTotalRings = plgon->getNumInteriorRings() + 1;
	return m_polyTotalRings;
}



wxRealPoint * tmGISDataVectorSHP::GetNextDataPolygon (int currentring, int & nbvertex)
{
	if (m_Feature ==NULL)
	{
		if (IsLoggingEnabled())
			wxLogDebug(_T("Feature is null, call GetNextDataPolygonInfo first"));
		m_polyTotalRings = 0;
		return NULL;
	}
	
	OGRPolygon * plgon = (OGRPolygon*) m_Feature->GetGeometryRef();
	wxASSERT(plgon);	
	
	// getting ring data (exterior then interior)
	OGRLineString * pLinePoly = NULL;
	if (currentring == 0)
		pLinePoly = plgon->getExteriorRing();
	else
		pLinePoly = plgon->getInteriorRing(currentring-1);
	
	if (pLinePoly == NULL)
	{
		if (IsLoggingEnabled())
			wxLogDebug(_T("Error getting ring for polygon. Ring num. is : %d"), currentring);
		m_polyTotalRings = 0;
		OGRFeature::DestroyFeature( m_Feature );
		return NULL;
	}
	
	// Exporting vertex for rings
	nbvertex = pLinePoly->getNumPoints();
	if (nbvertex <= 1)
	{
		if (IsLoggingEnabled())
			wxLogDebug(_T("Only one vertex or less in this polygon ring ???"));
		OGRGeometryFactory::destroyGeometry	(pLinePoly);
		return NULL;
	}
	
	wxRealPoint * pts = new wxRealPoint[nbvertex];
	for (int i=0; i<nbvertex;i++)
	{
		pts[i].x = pLinePoly->getX(i);
		pts[i].y = pLinePoly->getY(i);
	}
	
	
	// destroying feature only if all rings read
	if (currentring == m_polyTotalRings)
		OGRFeature::DestroyFeature( m_Feature );
	
	return pts;
}


/***************************************************************************//**
 @brief Counting features in layer
 @details This function return the number of lines, points, or polygons stored
 in the layer.
 @todo Implement this function in #tmGISDataVectorMYSQL
 @return  Number of features found or -1 if an error is encountered
 @author Lucien Schreiber (c) CREALP 2008
 @date 17 September 2008
 *******************************************************************************/
int tmGISDataVectorSHP::GetCount ()
{
	if(!m_Layer)
	{
		if (IsLoggingEnabled())
			wxLogDebug(_T("m_layer not defined, error"));
		return -1;
	}
	
	return m_Layer->GetFeatureCount();
}



/***************************************************************************//**
 @brief Get Metadata information well formated
 @return  An html string to be displayed in the properties dialog
 @author Lucien Schreiber (c) CREALP 2008
 @date 22 October 2008
 *******************************************************************************/
wxString tmGISDataVectorSHP::GetMetaDataAsHtml ()
{
	wxString myType = TM_GIS_SPATIAL_TYPES_STRING[GetSpatialType()];
	wxString myResult = _T("");
	myResult.Append(_("<B><U>Name</B></U><BR>"));
	myResult.Append(GetFullFileName() + _T("<BR><BR>"));
	
	myResult.Append(_("<B><U>General informations</B></U><BR>"));
	myResult.Append(_("Vector type is : ") + myType + _T("<BR>"));
	return myResult;
}
