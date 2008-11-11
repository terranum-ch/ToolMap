/***************************************************************************
								tmgisdatavectormysql.cpp
                    For dealing with spatial data from a mysql database
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

#include "tmgisdatavectormysql.h"


// init static member
DataBaseTM * tmGISDataVectorMYSQL::m_DB = NULL;


tmGISDataVectorMYSQL::tmGISDataVectorMYSQL()
{

}




tmGISDataVectorMYSQL::~tmGISDataVectorMYSQL()
{

	
}



bool tmGISDataVectorMYSQL:: CheckGeometryFields(const wxString & tablename)
{
	wxASSERT(m_DB);
	wxString sSentence = _T("");
	sSentence = wxString::Format(_T("SELECT COLUMN_NAME from INFORMATION_SCHEMA.COLUMNS WHERE ")
								 _T("table_schema=\"%s\" AND table_name='%s' ")
								 _T("AND COLUMN_NAME IN ('%s','%s');"), m_DB->DataBaseGetName().c_str(),
								 tablename.c_str(), tmGISMYSQL_FIELD1.c_str(), tmGISMYSQL_FIELD2.c_str());
	
	if (!m_DB->DataBaseQuery(sSentence))
	{
		if (IsLoggingEnabled())
			wxLogDebug(_T("Error checking geometry fields : %s"), sSentence.c_str());
		return FALSE;
	}
	
	if (m_DB->DatabaseGetCountResults() != 2)
	{
		if (IsLoggingEnabled())
			wxLogDebug(_T("The table : %s dosen't contain needed geometry fields"), tablename.c_str());
		return FALSE;
	}
	
	return TRUE;
}




bool tmGISDataVectorMYSQL::Open (const wxString & filename, bool bReadWrite)
{
	tmGISData::Open(filename, bReadWrite);
	
	// ensure that we have set a valid pointer to the database
	wxASSERT(m_DB);
	if (!m_DB)
	{
		if (IsLoggingEnabled())
			wxLogDebug(_T("Pointer to database invalid, open database first"));
		return FALSE;
	}
	
	//TODO: Only checks those once
	// does the table exists ?
	/*if(!m_DB->DataBaseTableExist(filename))
	{
		if (IsLoggingEnabled())
			wxLogDebug(_T("Table '%s' dosen't exists in the database"), filename.c_str());
		return FALSE;
	}*/
	
	// does the fields exists ?
	//if (!CheckGeometryFields(filename))
	//	return FALSE;

	return TRUE;
}




tmRealRect tmGISDataVectorMYSQL::GetMinimalBoundingRectangle()
{
	OGREnvelope * psExtent = new OGREnvelope();
	OGREnvelope oEnv;
	MYSQL_ROW row;
	unsigned long *  row_length;
	
	// query for the geometry enveloppe for all lines
	wxString sSentence = wxString::Format(_T("SELECT Envelope(%s) FROM %s"),
										  tmGISMYSQL_FIELD2.c_str(), GetShortFileName().c_str());
	if (m_DB->DataBaseQuery(sSentence))
	{
		// init extend based on the first object
		// to avoid 0 values for Xmin.
		row_length = m_DB->DataBaseGetNextRowResult(row);
		
		if(!row)
		{
			// no comment for thread version --> SIGTRAP
			//wxLogDebug(_T("No spatial data in the layer : %s"),
			//		   GetShortFileName().c_str());
			
			return tmRealRect(0,0,0,0);
		}
		
		
		OGRGeometry *poGeometry = CreateDataBaseGeometry(row, row_length);
		
		wxASSERT(poGeometry);
		
		poGeometry->getEnvelope(&oEnv);
		psExtent->MinX = oEnv.MinX;
		psExtent->MinY = oEnv.MinY;
		delete poGeometry;
		
		// loop all lines
		while (1)
		{
			row_length = m_DB->DataBaseGetNextRowResult(row);
			if (row_length == NULL)
				break;
			
			// compute the geometry and get the xmin xmax, ymin, ymax
			OGRGeometry *poGeometry = CreateDataBaseGeometry(row, row_length);
			if ( poGeometry != NULL )
			{
				poGeometry->getEnvelope(&oEnv);
				if (oEnv.MinX < psExtent->MinX) 
					psExtent->MinX = oEnv.MinX;
				if (oEnv.MinY < psExtent->MinY) 
					psExtent->MinY = oEnv.MinY;
				if (oEnv.MaxX > psExtent->MaxX) 
					psExtent->MaxX = oEnv.MaxX;
				if (oEnv.MaxY > psExtent->MaxY) 
					psExtent->MaxY = oEnv.MaxY;
			}
			delete poGeometry;
		}
		return tmRealRect(psExtent->MinX,psExtent->MinY,
						  psExtent->MaxX,psExtent->MaxY);
		
	}
	
	//wxLogDebug(_T("Error computing extend : %s : Sentence is %s "),
	//		   m_DB->DataBaseGetLastError().c_str(),
	//		   sSentence.c_str());
	
	
	
	return tmRealRect(0,0,0,0);
}



OGRGeometry *  tmGISDataVectorMYSQL::CreateDataBaseGeometry(MYSQL_ROW & row,
															unsigned long * length,
															int geometry_col)
{
	OGRGeometry * geometry = NULL;
	// Geometry columns will have the first 4 bytes contain the SRID.
	OGRGeometryFactory::createFromWkb(((unsigned char *)row[geometry_col]) + 4, 
									  NULL,
									  &geometry,
									  length[geometry_col] - 4 );
	
	return geometry;
}


/***************************************************************************//**
 @brief Get object ID from MYSQL row results
 @param row Adress of a row MySQL result
 @param col The zero based index of the column in which are stored the OID
 @return  the object OID or 0 if an error occured
 @author Lucien Schreiber (c) CREALP 2008
 @date 30 October 2008
 *******************************************************************************/
long tmGISDataVectorMYSQL::GetOid (MYSQL_ROW & row, const int & col)
{
	if (!row)
		return 0;
	long lreturnOID = 0;
	wxString (row[col], wxConvUTF8).ToLong( &lreturnOID);
	return lreturnOID;
}



/***************************************************************************//**
 @brief Set a spatial filter for geometric data
 @details This function sets a Geometric filter and ask the database for all
 data inside this filter
 @param filter the desired spatial filter
 @param type one of the #TOC_GENERIC_NAME, used to get the database table name
 @return  TRUE if the query works
 @author Lucien Schreiber (c) CREALP 2008
 @date 09 September 2008
 *******************************************************************************/
bool tmGISDataVectorMYSQL::SetSpatialFilter (tmRealRect filter, int type)
{
	m_DB->DataBaseDestroyResults();
	
	wxString table = GetTableName(type);	
	// check that a table is specified.
	if (table.IsEmpty())
	{
		if (IsLoggingEnabled())
			wxLogError(_T("No database table specified"));
		return false;
	}
	
	wxString sFilter = wxString::Format(_T("POLYGON ((%f %f,%f %f,%f %f,%f %f,%f %f))"),
										filter.x_min, filter.y_min,
										filter.x_max, filter.y_min,
										filter.x_max, filter.y_max,
										filter.x_min, filter.y_max,
										filter.x_min, filter.y_min);
	wxString sSentence = wxString::Format( _T("SELECT %s, %s FROM %s WHERE ")
										  _T("Intersects(GeomFromText('%s'),OBJECT_GEOMETRY)"),
										  tmGISMYSQL_FIELD1.c_str(),
										  tmGISMYSQL_FIELD2.c_str(),
										  table.c_str(), sFilter.c_str());
	
	if (m_DB->DataBaseQuery(sSentence))
	{
		bool bResult = m_DB->DataBaseHasResult();
		return TRUE;
	}
	
	if (IsLoggingEnabled())
		wxLogDebug(wxString::Format(_T("Error setting spatial filter : %s"),
									m_DB->DataBaseGetLastError().c_str()));
	
	return FALSE;
}



wxString tmGISDataVectorMYSQL::GetTableName (int type)
{
	if (type < TOC_GENERIC_NAME_NUMBER)
	{
		return wxString(TABLE_NAME_GIS_GENERIC[type]);
	}
	return _T("");
}



wxRealPoint * tmGISDataVectorMYSQL::GetNextDataLine (int & nbvertex, long & oid)
{
	MYSQL_ROW row;
	unsigned long *  row_length;
	
	// security check
	if(!m_DB->DataBaseHasResult())
	{
		if (IsLoggingEnabled())
			wxLogError(_T("Database should have results..."));
		nbvertex = 0;
		return NULL;
	}
	
	
	row_length = m_DB->DataBaseGetNextRowResult(row);
	if (row_length == NULL)
	{
		if (IsLoggingEnabled())
			wxLogDebug(_T("No more results"));
		nbvertex = 0;
		return NULL;
	}
		
		
	OGRLineString * pline = (OGRLineString*) CreateDataBaseGeometry(row, row_length, 1);
	oid = GetOid(row, 0);
	wxASSERT(pline);
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
	OGRGeometryFactory::destroyGeometry	(pline);
	return pts;
	
}



wxRealPoint * tmGISDataVectorMYSQL::GetNextDataPoint (long & oid)
{
	MYSQL_ROW row;
	unsigned long *  row_length;
	
	// security check
	if(!m_DB->DataBaseHasResult())
	{
		if (IsLoggingEnabled())
			wxLogError(_T("Database should have results..."));
		return NULL;
	}
	
	
	row_length = m_DB->DataBaseGetNextRowResult(row);
	if (row_length == NULL)
	{
		if (IsLoggingEnabled())
			wxLogDebug(_T("No more results"));
		return NULL;
	}
	
	
	OGRPoint * pPoint = (OGRPoint*) CreateDataBaseGeometry(row, row_length,1);
	oid = GetOid(row, 0);
	wxASSERT(pPoint);
	
	if (!pPoint)
	{
		if (IsLoggingEnabled())
			wxLogError(_T("Not able to create geometry"));
		return NULL;
	}
	
	
	wxRealPoint * pts = new wxRealPoint;
	
	pts->x = pPoint->getX();
	pts->y = pPoint->getY();
	
	OGRGeometryFactory::destroyGeometry	(pPoint);
	return pts;
}



/***************************************************************************//**
 @brief Getting Data Spatial type
 @return  One of the #TM_GIS_SPATIAL_TYPES values : 
 - LAYER_SPATIAL_LINE
 - LAYER_SPATIAL_POINT
 - LAYER_SPATIAL_POLYGON
 - LAYER_ERR
 @author Lucien Schreiber (c) CREALP 2008
 @date 23 October 2008
 *******************************************************************************/
TM_GIS_SPATIAL_TYPES tmGISDataVectorMYSQL::GetSpatialType ()
{
	TM_GIS_SPATIAL_TYPES myRetVal = LAYER_ERR;
	
	wxString sSentence = _T("");
	sSentence = wxString::Format(_T("SELECT COLUMN_TYPE from INFORMATION_SCHEMA.COLUMNS WHERE ")
								 _T("table_schema=\"%s\" AND ")
								 _T("table_name='%s' AND COLUMN_NAME IN ('%s')"),
								 m_DB->DataBaseGetName().c_str(),
								 GetFullFileName().c_str(),
								 tmGISMYSQL_FIELD2.c_str());
	if (!m_DB->DataBaseQuery(sSentence))
	{
		wxLogDebug(_T("Error getting table type : %s"), m_DB->DataBaseGetLastError().c_str());
		return myRetVal;
	}
	
	wxString myResult = _T("");
	m_DB->DataBaseGetNextResult(myResult);
	
	if (myResult == tmGISMYSQL_TEXT_TYPES[0])
		myRetVal = LAYER_SPATIAL_LINE; // lines
	
	else if (myResult == tmGISMYSQL_TEXT_TYPES[1])
		myRetVal = LAYER_SPATIAL_POINT; // points
	
	else if (myResult == tmGISMYSQL_TEXT_TYPES[2])
		myRetVal = LAYER_SPATIAL_POLYGON; // polygons

	return myRetVal;
}



/***************************************************************************//**
 @brief Get Metadata information well formated
 @return  An html string to be displayed in the properties dialog
 @author Lucien Schreiber (c) CREALP 2008
 @date 22 October 2008
 *******************************************************************************/
wxString tmGISDataVectorMYSQL::GetMetaDataAsHtml ()
{
	wxString myType = TM_GIS_SPATIAL_TYPES_STRING[GetSpatialType()];
	wxString myResult = _T("");
	myResult.Append(_("<B><U>Embedded table Name</B></U><BR>"));
	myResult.Append(GetFullFileName() + _T("<BR><BR>"));
	
	myResult.Append(_("<B><U>General informations</B></U><BR>"));
	myResult.Append(_("Vector type is : ") + myType + _T("<BR>"));
	myResult.Append(wxString::Format(_("Number of feature(s) : %d<BR><BR>"), GetCount()));
	
	myResult.Append(GetMinimalBoundingRectangleAsHtml(2) + _T("<BR>"));
	
	// no sense to display fields
	/*myResult.Append(GetFieldsMetadata() + _T("<BR>"));*/
	
	myResult.Append(GetDataSizeAsHtml());
	
	return myResult;
}



/***************************************************************************//**
 @brief Getting number of features
 @return  Number of feature in the layer
 @author Lucien Schreiber (c) CREALP 2008
 @date 23 October 2008
 *******************************************************************************/
int tmGISDataVectorMYSQL::GetCount ()
{
	wxString sSentence = _T("");
	sSentence = wxString::Format(_T("SELECT COUNT(*) FROM %s"),
								 GetFullFileName().c_str());
	if (!m_DB->DataBaseQuery(sSentence))
	{
		wxLogDebug(_T("Error getting number of features for %s, error was : %s"),
				   GetFullFileName().c_str(),
				   m_DB->DataBaseGetLastError().c_str());
		return 0;
	}
	
	return m_DB->DataBaseGetResultAsInt(true);
}



/***************************************************************************//**
 @brief Getting Database size
 @details Return the size of the database
 @return  Human readable and html compliant string containing data size
 @author Lucien Schreiber (c) CREALP 2008
 @date 23 October 2008
 *******************************************************************************/
wxString tmGISDataVectorMYSQL::GetDataSizeAsHtml (int iPrecision)
{
	wxString myResultVal = _("<U><B>Database Size</B></U><BR>");
	
	wxULongLong myDBSize = wxDir::GetTotalSize(m_DB->DataBaseGetPath() + 
											   wxFileName::GetPathSeparator() + 
											   m_DB->DataBaseGetName());
	if (myDBSize == wxInvalidSize)
	{
		myResultVal.Append(_("Error computing database size<BR>"));
		return myResultVal;
	}
	
	// modifiy the size to be MB
	double dMegaBytes =  (myDBSize.ToDouble() / 1024) / 1024;
	myResultVal.Append(wxString::Format(_("Total project size is : %.*f [Mb]<BR>"), 
									 iPrecision, dMegaBytes));
	return myResultVal;
	
}



/***************************************************************************//**
 @brief Search spatial data
 @param rect Real rectangle for searching data
 @param type Layertype see #tmLayerProperties
 @return  An array containing OID of data found or NULL if nothing found
 @note This function needs GEOS to ensure that real intersection is computed
 @author Lucien Schreiber (c) CREALP 2008
 @date 29 October 2008
 *******************************************************************************/
wxArrayLong * tmGISDataVectorMYSQL::SearchData (const tmRealRect & rect, int type)
{
	wxBusyCursor wait;
	m_DB->DataBaseDestroyResults();
	
	wxString table = GetTableName(type);	
	// check that a table is specified.
	if (table.IsEmpty())
	{
		if (IsLoggingEnabled())
			wxLogError(_T("No database table specified"));
		return NULL;
	}
	
	
	wxString sRect = wxString::Format(_T("POLYGON ((%f %f,%f %f,%f %f,%f %f,%f %f))"),
										rect.x_min, rect.y_min,
										rect.x_max, rect.y_min,
										rect.x_max, rect.y_max,
										rect.x_min, rect.y_max,
										rect.x_min, rect.y_min);
	wxString sSentence = wxString::Format( _T("SELECT OBJECT_ID, OBJECT_GEOMETRY FROM %s WHERE ")
										  _T("Intersects(GeomFromText('%s'),OBJECT_GEOMETRY)"),
										  table.c_str(), sRect.c_str());
	
	
	
	MYSQL_ROW row;
	unsigned long * row_size = 0;
	
	if (m_DB->DataBaseQuery(sSentence))
	{
		if (m_DB->DataBaseHasResult())
		{
			GEOSGeom  grect = CreateGEOSGeometry(rect);
			if (!grect)
				return NULL;
			
			// get all oid
			wxArrayLong * myArray = new wxArrayLong();
			for (int i = 0; i< m_DB->DatabaseGetCountResults();i++)
			{
				// construct geos geom
				row_size = m_DB->DataBaseGetNextRowResult(row);
				OGRGeometry * ogrgeom = CreateDataBaseGeometry(row, row_size, 1);
				GEOSGeom  geom = CreateGEOSGeometry(ogrgeom);
				
				if (CheckGEOSIntersection(&grect,&geom))
					myArray->Add(GetOid(row, 0)); 
				
				// destroy geometry
				GEOSGeom_destroy(geom);
			}
			GEOSGeom_destroy(grect);
			return myArray;
		}
		
	}
	
	
	
	
	if (IsLoggingEnabled())
		wxLogDebug(wxString::Format(_T("Error searching MySQL data : %s"),
									m_DB->DataBaseGetLastError().c_str()));
	
	return NULL;

}




