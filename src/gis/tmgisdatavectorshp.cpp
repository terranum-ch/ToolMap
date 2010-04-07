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

// for snapping constants
#include "../core/tmsnappingmemory.h"


tmGISDataVectorSHP::tmGISDataVectorSHP()
{
	m_Datasource = NULL;
	m_Layer = NULL;
	m_Feature = NULL;
	m_polyTotalRings = 0;
	m_ClassType = tmGIS_VECTOR_SHAPEFILE;
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
	m_Layer->ResetReading();
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
	m_Layer->ResetReading();
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



wxRealPoint * tmGISDataVectorSHP::GetNextDataLine (int & nbvertex, long & oid)
{
	wxASSERT(m_Layer);
	OGRFeature * poFeature = m_Layer->GetNextFeature();
	
	// nothing more to read
	if (poFeature == NULL)
	{
		nbvertex = 0;
		oid = -1;
		return NULL;		
	}
	
	
	OGRLineString * pline = (OGRLineString*) poFeature->GetGeometryRef();
	wxASSERT(pline);
	oid = poFeature->GetFID();
	
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



OGRGeometry *  tmGISDataVectorSHP::GetNextGeometry (long & oid)
{
	wxASSERT (m_Layer);
	OGRFeature * poFeature = m_Layer->GetNextFeature();
	
	// nothing more to read
	if (poFeature == NULL)
	{
		oid = wxNOT_FOUND;
		return NULL;		
	}
	
	OGRGeometry * myGeom = poFeature->GetGeometryRef();
	wxASSERT(myGeom);
	oid = poFeature->GetFID();
	
	return myGeom;
	//TODO: delete returned value after use
}



wxRealPoint * tmGISDataVectorSHP::GetNextDataPoint (long & oid)
{
	wxASSERT(m_Layer);
	OGRFeature * poFeature = m_Layer->GetNextFeature();
	
	// nothing more to read
	if (poFeature == NULL)
	{
		oid = -1;
		return NULL;		
	}
	
	
	OGRPoint * pPoint = (OGRPoint*) poFeature->GetGeometryRef();
	wxASSERT(pPoint);	
	oid = poFeature->GetFID();
	
	wxRealPoint * pts = new wxRealPoint();
	pts->x = pPoint->getX();
	pts->y = pPoint->getY();
	
	OGRFeature::DestroyFeature( poFeature );
	return pts;
}



int tmGISDataVectorSHP::GetNextDataPolygonInfo (long & oid)
{
	wxASSERT(m_Layer);
	m_Feature = m_Layer->GetNextFeature();
	
	// nothing more to read
	if (m_Feature == NULL)
	{
		m_polyTotalRings = 0;
		oid = -1;
		return 0;
	}
	
	oid = m_Feature->GetFID();
	
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
 @brief Get the actual feature as polygon
 @param oid the oid of the returned polygon
 @return  a valid OGRPolygon (must be destroyed by caller) or NULL
 @author Lucien Schreiber (c) CREALP 2008
 @date 17 November 2008
 *******************************************************************************/
OGRPolygon * tmGISDataVectorSHP::GetNextDataOGRPolygon (long & oid)
{
	wxASSERT(m_Feature);
	OGRPolygon * myPoly = (OGRPolygon*) m_Feature->GetGeometryRef();
	oid = m_Feature->GetFID();
	
	return myPoly;
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
	myResult.Append(wxString::Format(_("Number of feature(s) : %d<BR><BR>"), GetCount()));
	
	myResult.Append(GetMinimalBoundingRectangleAsHtml(2) + _T("<BR>"));
	
	myResult.Append(GetFieldsMetadata() + _T("<BR>"));
	
	myResult.Append(GetDataSizeAsHtml());
	
	return myResult;
}




/***************************************************************************//**
 @brief Getting Data size
 @details Return the size of the dataset
 @return  Human readable and html compliant string containing data size
 @author Lucien Schreiber (c) CREALP 2008
 @date 23 October 2008
 *******************************************************************************/
wxString tmGISDataVectorSHP::GetDataSizeAsHtml (int iPrecision)
{
	// compute size for all part of the shp file
	wxULongLong myFilesSize = 0;
	wxFileName myFileName (GetFullFileName()); 
	wxString myFileWoutExt = myFileName.GetPath(wxPATH_GET_SEPARATOR) + 
							myFileName.GetName();
	wxString myShpExt[] = {_T(".shp"), _T(".sbn"), _T(".shx"), _T(".dbf")};
	
	for (unsigned int i=0;i< (sizeof(myShpExt) / sizeof(wxString));i++)
	{
		wxULongLong myTempSize = wxFileName::GetSize(myFileWoutExt + myShpExt[i]);
		if (myTempSize != wxInvalidSize)
			myFilesSize += myTempSize;
		else
			wxLogDebug(_T("Unable to compute %s%s file size, maybe dosen't exists"),
					   myFileWoutExt.c_str(), myShpExt[i].c_str());
	}
		
	// modifiy the size to be MB
	double dMegaBytes =  (myFilesSize.ToDouble() / 1024) / 1024;
	
	wxString myResult = _("<U><B>File Size</B></U><BR>");
	myResult.Append(wxString::Format(_("Shapefile size is : %.*f [Mb]<BR>"), 
									 iPrecision, dMegaBytes));
	return myResult;
	
}



/***************************************************************************//**
 @brief Get the number of fields
 @return  Number of fields. If -1 is returned, an error occur
 @author Lucien Schreiber (c) CREALP 2008
 @date 23 October 2008
 *******************************************************************************/
int tmGISDataVectorSHP::GetFieldsCount()
{
	if (!m_Layer)
		return -1;
	
	OGRFeatureDefn * poFDefn = m_Layer->GetLayerDefn();
	return  poFDefn->GetFieldCount ();	
}



/***************************************************************************//**
 @brief Getting fields name
 @param Fields adress of an array string to be filled with fields name
 @return  true if fields name returned succesfully, false otherwise
 @author Lucien Schreiber (c) CREALP 2008
 @date 23 October 2008
 *******************************************************************************/
bool tmGISDataVectorSHP::GetFieldsName (wxArrayString & Fields, long oid)
{
	wxASSERT(oid == wxNOT_FOUND);
	Fields.Clear();
	if (!m_Layer)
		return false;
	
	OGRFeatureDefn * poFDefn = m_Layer->GetLayerDefn();
	OGRFieldDefn * poFieldDf = NULL;
	
	for (int i=0;i<GetFieldsCount();i++)
	{
		poFieldDf = poFDefn->GetFieldDefn(i);
		Fields.Add(wxString::FromAscii(poFieldDf->GetNameRef()));
	}
	
	return true;
}


bool tmGISDataVectorSHP::GetFieldsValue (wxArrayString & values, long oid){
	values.Clear();
	
	if (m_Layer == NULL) {
		wxLogError(_T("No layer defined, unable to get fields value"));
		return false;
	}
	
	OGRFeature * myFeature = m_Layer->GetFeature(oid);
	if (myFeature == NULL) {
		wxLogError(_T("No feature with id n\u00B0%d in layer. Reading fields value not possible"),oid);
		return false;
	}
	
	int iTotFields = GetFieldsCount();
	for (int i = 0; i < iTotFields; i++) {
		wxString myVal = wxString::FromAscii(myFeature->GetFieldAsString(i));
		values.Add(myVal);
	}
	
	OGRFeature::DestroyFeature(myFeature);
	return true;
}

		
		



/***************************************************************************//**
 @brief Search spatial data
 @param rect Real rectangle for searching data
 @param type Layertype see #tmLayerProperties
 @return  An array containing OID of data found or NULL if nothing found
 @author Lucien Schreiber (c) CREALP 2008
 @date 31 October 2008
 *******************************************************************************/
wxArrayLong * tmGISDataVectorSHP::SearchData (const tmRealRect & rect, int type)
{
	// test one going directly
	
	m_Layer->ResetReading();
	wxBusyCursor wait;
	
	OGRGeometry * poGeometry;
	OGRFeature * poFeature;
	OGRGeometry * poRectGeom = CreateOGRGeometry(rect);
	if (!poRectGeom)
	{
		if(IsLoggingEnabled())
			wxLogDebug(_T("Unable to create geometry for rectangle"));
		return NULL;	
	}
	
	// searching all features
	wxArrayLong * myArray = new wxArrayLong();
	while( (poFeature = m_Layer->GetNextFeature()) != NULL )
	{
		poGeometry = poFeature->GetGeometryRef();
		if (poGeometry)
		{
			if (poGeometry->Intersect(poRectGeom))
				myArray->Add(poFeature->GetFID());
		
			OGRGeometryFactory::destroyGeometry(poGeometry);
		}
	}
	OGRGeometryFactory::destroyGeometry(poRectGeom);
	
	return myArray;
	
}



/***************************************************************************//**
 @brief Get OID of all features back from the shapefile
 @author Lucien Schreiber (c) CREALP 2009
 @date 31 March 2009
 *******************************************************************************/
wxArrayLong * tmGISDataVectorSHP::GetAllData ()
{
	wxASSERT (m_Layer);
	
	OGRFeature * poFeature = NULL;
	m_Layer->ResetReading();
	// searching all features
	wxArrayLong * myArray = new wxArrayLong();
	while( (poFeature = m_Layer->GetNextFeature()) != NULL )
	{
			myArray->Add(poFeature->GetFID());
	}
	
	return myArray;
}


/***************************************************************************//**
 @brief Create the shp on the disk
 @param filename the filename we want to create
 @param type one of the following values : 
  - 0 for lines
  - 1 for points
  - 2 for polygons
 @return  true if file creation succeed, false otherwise
 @author Lucien Schreiber (c) CREALP 2008
 @date 15 November 2008
 *******************************************************************************/
bool tmGISDataVectorSHP::CreateFile (const wxFileName & filename, int type)
{
	const char *pszDriverName = "ESRI Shapefile";
    OGRSFDriver *poDriver;
	
  	
    poDriver = OGRSFDriverRegistrar::GetRegistrar()->GetDriverByName(
																	 pszDriverName );
    if( poDriver == NULL )
    {
		wxASSERT_MSG(0, _T(" driver not available."));
		return false;
    }
	
   
	
	// creating the file
	wxString mysFileName = filename.GetFullPath();
	char * buffer = new char [mysFileName.Length()+2];
	strcpy(buffer, (const char*)mysFileName.mb_str(wxConvUTF8));
	
	m_Datasource = poDriver->CreateDataSource( buffer, NULL );
    
#ifndef  __WXMSW__ 
	delete [] buffer;
#endif
	
	if( m_Datasource == NULL )
    {
        wxASSERT_MSG(0, _T("Creation of output file failed."));
        return false;
    }
	
	
	OGRwkbGeometryType myGeomType = wkbUnknown;
	switch (type)
	{
		case 0: // LINE
			myGeomType = wkbLineString;
			break;
			
		case 1: // POINT
			myGeomType = wkbPoint;
			break;
			
		case 2: // POLYGON
			myGeomType = wkbPolygon;
			break;
			
		default:
			myGeomType = wkbUnknown;
			break;
	}
	
	if (myGeomType == wkbUnknown)
		return false;
	
	
	// creating the layer
	wxString myFileNameWOExt = filename.GetName();
	char * buffer2 = new char [myFileNameWOExt.Length()+2];
	strcpy(buffer2, (const char*)myFileNameWOExt.mb_str(wxConvUTF8));
	
	m_Layer = m_Datasource->CreateLayer( buffer2, NULL, myGeomType, NULL );
	
#ifndef  __WXMSW__ 
	delete [] buffer2;
#endif
	
    if( m_Layer == NULL )
    {
		wxASSERT_MSG(0 , _T("Layer creation failed."));
        return false;
    }
	
	
	return true;	
	
}



/***************************************************************************//**
 @brief Add text fields to the Shp
 @param fieldname name of the field
 @param size length of the field
 @return true if field added, false otherwise
 @author Lucien Schreiber (c) CREALP 2008
 @date 15 November 2008
 *******************************************************************************/
bool tmGISDataVectorSHP::AddFieldText (const wxString & fieldname, int size)
{
	wxASSERT (m_Layer);
	bool bReturn = true;
	
	char * buffer = new char [fieldname.Length()+2];
	strcpy(buffer, (const char*)fieldname.mb_str(wxConvUTF8));
	OGRFieldDefn oField( buffer, OFTString );
	
    oField.SetWidth(size);
	
    if( m_Layer->CreateField( &oField ) != OGRERR_NONE )
    {
        wxASSERT_MSG(0,_T("Creating text field failed."));
        bReturn = false;
    }
	
#ifndef  __WXMSW__ 
	delete [] buffer;
#endif
	
	return bReturn;
	
}



/***************************************************************************//**
 @brief Add int field to the Shp
 @param fieldname name of the field
 @param isfloat Set this value to true if you want a float field
 @return true if field added, false otherwise
 @author Lucien Schreiber (c) CREALP 2008
 @date 15 November 2008
 *******************************************************************************/
bool tmGISDataVectorSHP::AddFieldNumeric (const wxString & fieldname, bool isfloat)
{
	wxASSERT (m_Layer);
	bool bReturn = true;
	
	char * buffer = new char [fieldname.Length()+2];
	strcpy(buffer, (const char*)fieldname.mb_str(wxConvUTF8));
	
	OGRFieldType myFieldType = OFTInteger;
	if (isfloat)
		myFieldType = OFTReal;
		
	OGRFieldDefn oField( buffer, myFieldType);	
	
    if( m_Layer->CreateField( &oField ) != OGRERR_NONE )
    {
        wxASSERT_MSG(0,_T("Creating int field failed."));
		bReturn = false;
    }
	
#ifndef  __WXMSW__ 
	delete [] buffer;
#endif
	
	return bReturn;	
}



/***************************************************************************//**
 @brief Add date field to the Shp
 @param fieldname name of the field
 @author Lucien Schreiber (c) CREALP 2008
 @date 15 November 2008
 *******************************************************************************/
bool tmGISDataVectorSHP::AddFieldDate (const wxString & fieldname)
{
	wxASSERT (m_Layer);
	bool bReturn = true;
	
	char * buffer = new char [fieldname.Length()+2];
	strcpy(buffer, (const char*)fieldname.mb_str(wxConvUTF8));
	
		
	OGRFieldDefn oField( buffer, OFTDateTime);	
	
    if( m_Layer->CreateField( &oField ) != OGRERR_NONE )
    {
        wxASSERT_MSG(0,_T("Creating int field failed."));
        bReturn = false;
    }
	
#ifndef  __WXMSW__ 
	delete [] buffer;
#endif
	
	return bReturn;	
}



/***************************************************************************//**
 @brief Add a geometry in the shapefile
 @param Geom a OGRGeometry
 @param oid the oid of the new geometry added
 @param layertype Not used for shapefiles
 @return  true if geometry added, false otherwise
 @author Lucien Schreiber (c) CREALP 2008
 @date 16 November 2008
 *******************************************************************************/
long tmGISDataVectorSHP::AddGeometry (OGRGeometry * Geom, const long & oid, int layertype)
{
	wxASSERT(Geom);
	wxASSERT (m_Layer);
	
	
	OGRFeature * poFeature;
	poFeature = OGRFeature::CreateFeature( m_Layer->GetLayerDefn() );
	
	poFeature->SetGeometry(Geom);
	if (oid != -1)
	{
		poFeature->SetFID(oid);	
		
		// add FID into special column
		poFeature->SetField(0, (int) oid);
	}
	
	OGRErr myErr = m_Layer->CreateFeature(poFeature);
	if (myErr != OGRERR_NONE)
		return -1;
	
	
	long lRetVal = poFeature->GetFID();
	OGRFeature::DestroyFeature(poFeature);
	return lRetVal;
	
}



/***************************************************************************//**
 @brief Move to next feature
 @param resetreading should we restart iterating features ?
 @return true if next feature is a valid feature, false otherwise (end of file
 encountred).
 @author Lucien Schreiber (c) CREALP 2008
 @date 17 November 2008
 *******************************************************************************/
bool tmGISDataVectorSHP::SetNextFeature (bool resetreading)
{
	wxASSERT (m_Layer);
	if (m_Feature)
	{
		OGRFeature::DestroyFeature(m_Feature);
		m_Feature = NULL;
	}

	if (resetreading)
		m_Layer->ResetReading();
	
	m_Feature = m_Layer->GetNextFeature();
		
	if (m_Feature)
		return true;
	
	return false;
}



/***************************************************************************//**
 @brief Return the actual OID
 @return  the actual OID or wxNOT_FOUND if not found
 @author Lucien Schreiber (c) CREALP 2009
 @date 26 March 2009
 *******************************************************************************/
long tmGISDataVectorSHP::GetActualOID ()
{
	wxASSERT (m_Layer);
	wxASSERT (m_Feature->GetFieldCount() >= 1);
	
	if (m_Feature)
		return m_Feature->GetFieldAsInteger(0);
	else
		return wxNOT_FOUND;
}


/***************************************************************************//**
 @brief Set the OID for the current Feature
 @details See also SetNextFeature() for more informations about changing actual
 feature
 @param oid OID to set. (converted into int for shapefiles)
 @author Lucien Schreiber (c) CREALP 2009
 @date 27 March 2009
 *******************************************************************************/
void tmGISDataVectorSHP::SetActualOID (long oid)
{
	wxASSERT (m_Layer);
	wxASSERT (m_Feature->GetFieldCount() >= 1);
	
	if (m_Feature)
		m_Feature->SetField(0, (int) oid);
		
}


/***************************************************************************//**
 @brief Add value into a field
 @param value a wxString containing the value (will be converted in the function)
 @param fieldtype the type of the field based on : #PRJDEF_FIELD_TYPE
 @param iindex 0 based field index
 @return true if value was added to the field
 @author Lucien Schreiber (c) CREALP 2008
 @date 17 November 2008
 *******************************************************************************/
bool tmGISDataVectorSHP::SetFieldValue (const wxString & value, 
							int fieldtype, int iindex)
{
	wxASSERT(m_Feature);
	
	// check that the field exists
	if (iindex > m_Feature->GetFieldCount() -1)
	{
		wxASSERT_MSG(0, _T("Trying to set value to a field that doesn't exists"));
		return false;
	}
	
	bool bReturn = true;
	char * buffer = NULL;

	wxStringTokenizer myTok;
	int myYear = 0, myMonth = 0, myDay = 0;
	
	switch (fieldtype)
	{
		case 4: // enumeration
		case 0: // TEXT
			buffer = new char [value.Length()+2];
			strcpy(buffer, (const char*)value.mb_str(wxConvUTF8));
			m_Feature->SetField(iindex, buffer);
			
			// crash under windows ??? why
#ifndef  __WXMSW__ 
			delete [] buffer;
#endif			
			break;
			
		case 1: // INTEGER
			m_Feature->SetField(iindex, wxAtoi(value));
			break;
			
		case 2: // FLOAT
			m_Feature->SetField(iindex, wxAtof(value));
			break;
		
		case 3: //DATE
			myTok.SetString(value, _T("-"), wxTOKEN_DEFAULT);
			wxASSERT(myTok.CountTokens() == 3);
			myYear = wxAtoi(myTok.GetNextToken());
			myMonth = wxAtoi(myTok.GetNextToken());
			wxASSERT (myMonth >= 1 && myMonth <=12);
			myDay = wxAtoi (myTok.GetNextToken());
			wxASSERT (myDay >= 1 && myDay <= 31);
			wxLogDebug(_T("Date computed is : %d / %d / %d"),
						   myYear,
						   myMonth,
						   myDay);
			m_Feature->SetField(iindex, myYear, myMonth, myDay, 0, 0, 0, 1);
			break;

			
		default:
			wxLogDebug(_T("Not implemented now, sorrry...."));
			bReturn = false;
			break;
	}

	return bReturn;
}



/***************************************************************************//**
 @brief Update the actual feature
 @details This function should be used after applying modification to the
 feature for saving the modification in the file. For exemple after setting
 fields value with tmGISDataVectorSHP::SetFieldValue() use this function for
 apply changes
 @return  true if saving features works
 @author Lucien Schreiber (c) CREALP 2008
 @date 17 November 2008
 *******************************************************************************/
bool tmGISDataVectorSHP::UpdateFeature ()
{
	wxASSERT(m_Feature);
	wxASSERT (m_Layer);
	
	if (m_Layer->SetFeature(m_Feature) != OGRERR_NONE)
		return false;
	
	return true;
}



/***************************************************************************//**
 @brief Get snapping coordinate
 @details Return, if found, the closest existing coordinate for snapping
 @param clickpt The real coordinate of the clicked point
 @param iBuffer the size of the buffer in map unit
 @param snappt the returned snapping coordinate if function return true
 @param snaptype the type of snapping : #tmSNAPPING_BEGIN_END or tmSNAPPING_VERTEX
 @return  true if a snapping coordinate was found, false otherwise
 @author Lucien Schreiber (c) CREALP 2009
 @date 29 January 2009
 *******************************************************************************/
bool tmGISDataVectorSHP::GetSnapCoord (const wxRealPoint & clickpt, int iBuffer,
						   wxRealPoint & snappt, int snaptype)
{
	// create OGRpoint and buffer
	OGRPoint myClickPoint;
	myClickPoint.setX(clickpt.x);
	myClickPoint.setY(clickpt.y);
	
	OGRGeometry * myBufferClick = tmGISDataVector::SafeBuffer(&myClickPoint, iBuffer);
	wxASSERT (myBufferClick);
	wxASSERT (m_Layer);
	//OGRGeometryFactory::destroyGeometry(myClickPoint);
	
	// searching all features
	m_Layer->ResetReading();
	OGRGeometry * poGeometry;
	OGRFeature * poFeature;
	wxRealPoint * mySnapPoint = NULL;
	bool bReturn = false;
	while( (poFeature = m_Layer->GetNextFeature()) != NULL )
	{
		poGeometry = poFeature->GetGeometryRef();
		if (poGeometry)
		{
			if (poGeometry->Intersect(myBufferClick))
			{
				
				if (snaptype & tmSNAPPING_VERTEX == tmSNAPPING_VERTEX)
				{
					mySnapPoint = GetVertexIntersection(poGeometry, myBufferClick);
				}
				else if (snaptype == tmSNAPPING_BEGIN_END)
				{
					mySnapPoint = GetBeginEndInterseciton(poGeometry, myBufferClick);
				}
				
			}
			
			OGRFeature::DestroyFeature(poFeature);
			
			if (mySnapPoint)
			{
				snappt = wxRealPoint(mySnapPoint->x,mySnapPoint->y);
				delete mySnapPoint;
				bReturn = true;
				break;
			}
			
		}
	}
	OGRGeometryFactory::destroyGeometry(myBufferClick);

	return bReturn;
}
