/***************************************************************************
								tmexportdatashp.cpp
                    Class for the process of exporting data as SHP
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

#include "tmexportdatashp.h"



/***************************************************************************//**
 @brief Init class member values
 @author Lucien Schreiber (c) CREALP 2008
 @date 14 November 2008
 *******************************************************************************/
void tmExportDataSHP::InitMemberValues()
{
	m_Extension = _T(".shp");
	m_Frame = NULL;
}



/***************************************************************************//**
 @brief Constructor
 @details Use tmExportDataSHP::Create() for two step initialization
 @author Lucien Schreiber (c) CREALP 2008
 @date 14 November 2008
 *******************************************************************************/
tmExportDataSHP::tmExportDataSHP()
{
	InitMemberValues();
}



/***************************************************************************//**
 @brief Constructor
 @param database A valid #DataBaseTM object (an assert is done in debug mode)
 @author Lucien Schreiber (c) CREALP 2008
 @date 14 November 2008
 *******************************************************************************/
tmExportDataSHP::tmExportDataSHP (DataBaseTM * database)
{
	InitMemberValues();
	Create(database);
}



/***************************************************************************//**
 @brief Creator
 @details For two step initialization. In case of use of the default constructor
 @param database A valid #DataBaseTM object (an assert is done in debug mode)
 @author Lucien Schreiber (c) CREALP 2008
 @date 14 November 2008
 *******************************************************************************/
void tmExportDataSHP::Create (DataBaseTM * database)
{
	tmExportData::Create(database);
}


/***************************************************************************//**
 @brief destructor
 @author Lucien Schreiber (c) CREALP 2008
 @date 14 November 2008
 *******************************************************************************/
tmExportDataSHP::~tmExportDataSHP()
{
	if (m_Frame)
		OGRGeometryFactory::destroyGeometry(m_Frame);
}



/***************************************************************************//**
 @brief Create empty file
 @details Create a file  ProjectDefMemoryLayers informations, path and extension
 specified in class
 @param myLayer contain the name of the file (without extension) to create
 @param path path where we want the file to be created
 @return  true if file created with sucess, false otherwise
 @author Lucien Schreiber (c) CREALP 2008
 @date 14 November 2008
 *******************************************************************************/
bool tmExportDataSHP::CreateEmptyExportFile (ProjectDefMemoryLayers * myLayer, 
											 const wxString & path)
{
	bool bReturn = true;
	wxASSERT(myLayer);
	wxFileName * myShpFileName = GetFileName(myLayer, path);
	if (!myShpFileName)
	{
		wxASSERT_MSG(0, _T("Unable to create the file name"));
		return false;
	}
	
	
	
	if(!m_Shp.CreateFile(myShpFileName->GetFullPath(), (int) myLayer->m_LayerType))
		bReturn =  false;
	
	// create the shp
	//TODO: Add error to report indicating that export file wasn't created
	
	delete myShpFileName;
	return bReturn;
	
}



/***************************************************************************//**
 @brief Add optional fields to the Shp
 @param myfields array of fields for this shapefile
 @return  true if fields were added successfully, false otherwise
 @author Lucien Schreiber (c) CREALP 2008
 @date 15 November 2008
 *******************************************************************************/
bool tmExportDataSHP::AddOptFields (PrjMemFieldArray * myfields)
{
	wxASSERT (myfields);
	
	bool bReturn = true; // if adding fields failed, stop
	for (unsigned int i = 0; i< myfields->GetCount(); i++)
	{
		ProjectDefMemoryFields field = myfields->Item(i);
		int iSize = 0;
		
		switch (field.m_FieldType)
		{
			case TM_FIELD_TEXT:
				bReturn = m_Shp.AddFieldText(field.m_Fieldname, field.m_FieldPrecision);
				break;
				
			case TM_FIELD_INTEGER:
				bReturn = m_Shp.AddFieldNumeric(field.m_Fieldname, false);
				break;
				
			case TM_FIELD_FLOAT:
				bReturn = m_Shp.AddFieldNumeric(field.m_Fieldname, true);
				break;
				
			case TM_FIELD_ENUMERATION:
				// compute max size for enum
				iSize = GetSizeOfEnum(field.m_pCodedValueArray);
				bReturn = m_Shp.AddFieldText(field.m_Fieldname, iSize);
				break;
				
			case TM_FIELD_DATE:
				bReturn = m_Shp.AddFieldDate(field.m_Fieldname);
				break;
				
			default:
				bReturn = false;
				break;
		}
		
		if (!bReturn)
			break;
		
	}
	
	return bReturn;
}



/***************************************************************************//**
 @brief Adding generic fields
 @details Adding field :
 - OBJECT_CD
 - OBJECT_DESC
 @return  true if fields added successfully
 @author Lucien Schreiber (c) CREALP 2008
 @date 16 November 2008
 *******************************************************************************/
bool tmExportDataSHP::AddGenericFields (int iObjeDescSize)
{
	wxASSERT (iObjeDescSize);
	
	if (m_Shp.AddFieldNumeric(_T("OBJ_CD")) &&
		m_Shp.AddFieldText(_T("OBJ_DESC"), iObjeDescSize))
		return true;
	
	return false;
	
}


/***************************************************************************//**
 @brief Adding Field for storing ToolMap FID
 @details Add a first field containing ToolMap's FID. This is needed for adding
 advanced attribution because setting FID doesn't work with Shapefile (GDAL Bug
 ???). Added field name is TM_OID
 @return  true if field added successfully
 @author Lucien Schreiber (c) CREALP 2009
 @date 26 March 2009
 *******************************************************************************/
bool tmExportDataSHP::AddFIDField ()
{
	if(m_Shp.AddFieldNumeric(_T("TM_OID"), false))
		return true;
	
	return false;
}



/***************************************************************************//**
 @brief Write all geometrics lines to the shp
 @param myLayer object containing info on the actual layer, such as layer_ID or
 layer_Name
 @return  false if write failled, true otherwise
 @author Lucien Schreiber (c) CREALP 2008
 @date 16 November 2008
 *******************************************************************************/
bool tmExportDataSHP::WriteLines (ProjectDefMemoryLayers * myLayer)
{
	wxASSERT (m_Frame);
	tmGISDataVectorMYSQL myDBData;
	tmGISDataVectorMYSQL::SetDataBaseHandle(m_pDB);
	OGRLineString * myLine = NULL;
	OGRGeometry * myCropLine = NULL;
	long myOid = 0;
	
	while (1)
	{
		myLine = myDBData.GetNextDataLine(myOid);
		if (!myLine)
			break;
		
		myCropLine = SafeIntersection(myLine, m_Frame);
		if (myCropLine)
		{
			if (!myCropLine->IsEmpty())
			{
				//long myAddedOID = 
				m_Shp.AddGeometry(myCropLine, myOid);
			}
		
			OGRGeometryFactory::destroyGeometry(myCropLine);
		}
		
		
		OGRGeometryFactory::destroyGeometry(myLine);
		
	}
	
	
	return true;
}


/***************************************************************************//**
 @brief Compute intersection
 @details This function try to bypass the Intersection() bug of GDAL by using
 GEOS directly
 @param line The line to intersect
 @param frame the frame for intersecting
 @return  A valid OGRLineString or NULL
 @author Lucien Schreiber (c) CREALP 2008
 @date 18 November 2008
 *******************************************************************************/
OGRGeometry * tmExportDataSHP::SafeIntersection(OGRGeometry * line, OGRGeometry * frame)
{
	wxASSERT(line);
	wxASSERT(frame);
	
	GEOSGeom  geosline = NULL;
	GEOSGeom  geosframe = NULL;
	GEOSGeom  geosintersect = NULL;
	OGRGeometry * returncrop = NULL;
	
	
	/* convert to GEOS
#ifdef __WXOSX__
	return (OGRLineString*) line->Intersection(frame);
	
#else*/
	
	geosline = line->exportToGEOS();
	geosframe = frame->exportToGEOS();
	
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
OGRGeometry * tmExportDataSHP::SafeUnion (OGRGeometry * union1, OGRGeometry * line)
{
	wxASSERT(union1);
	wxASSERT(line);
	
	GEOSGeom  geosline = NULL;
	GEOSGeom  geosunion = NULL;
	GEOSGeom  geosresult = NULL;
	OGRGeometry * returnunion = NULL;
	
	
	// convert to GEOS
//#ifdef __WXOSX__
//	return union1->Union(line);
	
//#else
	
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

	
//#endif
}


/***************************************************************************//**
 @brief Safe conversion function
 @param geosGeom The GEOSGeom to convert
 @return  A valid OGRGeometry or NULL if an error occur
 @author Lucien Schreiber (c) CREALP 2008
 @date 15 January 2009
 *******************************************************************************/
OGRGeometry * tmExportDataSHP::SafeCreateFromGEOS (GEOSGeom geosGeom)
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
	
	OGRFree(pabyBuf);
	
/*    if( pabyBuf != NULL )
    {
#ifndef  __WXMSW__    
		delete [] pabyBuf;
#endif
		//delete( pabyBuf );
    }*/
	
    return poGeometry;

}


/***************************************************************************//**
 @brief Write all geometrics points to the shp
 @param myLayer object containing info on the actual layer, such as layer_ID or
 layer_Name
 @return  false if write failled, true otherwise
 @author Lucien Schreiber (c) CREALP 2008
 @date 16 November 2008
 *******************************************************************************/
bool tmExportDataSHP::WritePoints (ProjectDefMemoryLayers * myLayer)
{
	wxASSERT (m_Frame);
	tmGISDataVectorMYSQL myDBData;
	tmGISDataVectorMYSQL::SetDataBaseHandle(m_pDB);
	OGRPoint * myPoint = NULL;
	long myOid = 0;
	
	while (1)
	{
		myPoint = myDBData.GetOGRNextDataPoint(myOid);
		if (!myPoint)
			break;
		
		// intersects with the frame
		if( myPoint->Intersect((OGRGeometry*)m_Frame))
		{
			m_Shp.AddGeometry((OGRGeometry*)myPoint, myOid);
		}
		
		OGRGeometryFactory::destroyGeometry(myPoint);
	}
	
	
	return true;
	
}



/***************************************************************************//**
 @brief Compute polygons from lines
 @details This function uses GEOS for computing polygon from lines
 @param myLayer Informations about the current layer
 @return  true if polygons were created successfully
 @author Lucien Schreiber (c) CREALP 2008
 @date 17 November 2008
 *******************************************************************************/
bool tmExportDataSHP::WritePolygons (ProjectDefMemoryLayers * myLayer)
{
	wxASSERT (m_Frame);
	tmGISDataVectorMYSQL myDBData;
	tmGISDataVectorMYSQL::SetDataBaseHandle(m_pDB);
	OGRLineString * myLine = NULL;
	OGRGeometry * myCropLine = NULL;
	long myOid = 0;
	//int iValidLines = 0;
	
	long myNbResuts = 0;
	if (m_pDB->DataBaseGetResultSize(NULL, &myNbResuts)==false)
		return false;
	
		
	// transform non standard OGRLinearRing -> OGRLineString
	OGRLinearRing * myFrame = m_Frame->getExteriorRing();
	OGRLineString * myLineString = (OGRLineString*) OGRGeometryFactory::createGeometry(wkbLineString);
	for (int p= 0; p<myFrame->getNumPoints();p++)
	{
		OGRPoint * myPoint = (OGRPoint*) OGRGeometryFactory::createGeometry(wkbPoint);
		myFrame->getPoint(p,myPoint);
		myLineString->addPoint(myPoint);
	}
	
	
	// loop for all lines 
	OGRGeometry * myTempNodedLines = OGRGeometryFactory::createGeometry(wkbMultiLineString);
	for (int i = 0; i < myNbResuts ; i++)
	{
		myLine = myDBData.GetNextDataLine(myOid);
		if (!myLine)
			break;
		
		
		// intersects with the frame
		myCropLine = SafeIntersection(myLine, m_Frame);
		OGRGeometryFactory::destroyGeometry(myLine);
		if (myCropLine)
		{
			if (!myCropLine->IsEmpty())
			{
				// union all lines into one multiline
				myTempNodedLines = SafeUnion(myTempNodedLines, myCropLine);
				//int myGeomCount = ((OGRGeometryCollection *)myTempNodedLines)->getNumGeometries();
				//wxLogDebug(_T("Nb of geom : %d"), myGeomCount);
			}
			OGRGeometryFactory::destroyGeometry(myCropLine);
		}
		
		
	}
	
	
	// union with frame
	OGRGeometry * myAllLinesNoded = SafeUnion(myTempNodedLines, myLineString);
	OGRGeometryFactory::destroyGeometry(myTempNodedLines);
	OGRGeometryFactory::destroyGeometry(myLineString);
	
	// create polygons
	GEOSGeom myGEOSAllLines = myAllLinesNoded->exportToGEOS();
	OGRGeometryFactory::destroyGeometry(myAllLinesNoded);
	GEOSGeom  myGEOSPoly = GEOSPolygonize(&myGEOSAllLines, 1);
	GEOSGeom_destroy(myGEOSAllLines);
	int myNbPoly = GEOSGetNumGeometries(myGEOSPoly);

	
	// save polygon to shp
	for (int j = 0; j < myNbPoly;j++)
	{
		const GEOSGeometry * myActualPoly = GEOSGetGeometryN(myGEOSPoly, j);
		GEOSGeom myActualPolyClone = GEOSGeom_clone(myActualPoly);
		OGRGeometry * myPoly = SafeCreateFromGEOS(myActualPolyClone);
		GEOSGeom_destroy(myActualPolyClone);
		m_Shp.AddGeometry(myPoly, -1);
		OGRGeometryFactory::destroyGeometry(myPoly);
	}
	GEOSGeom_destroy(myGEOSPoly);
	return true;
}


/***************************************************************************//**
 @brief Set the frame
 @details If the frame allready exists, it is destroyed and this new frame is
 used
 @param points array of points creating a polygon
 @param nbvertex number of vertex
 @author Lucien Schreiber (c) CREALP 2008
 @date 16 November 2008
 *******************************************************************************/
void tmExportDataSHP::SetFrame (wxRealPoint * points, int nbvertex)
{
	wxASSERT (points);
	if (m_Frame)
		OGRGeometryFactory::destroyGeometry(m_Frame);
	
	m_Frame =(OGRPolygon*) OGRGeometryFactory::createGeometry(wkbPolygon);
	OGRLineString * myLine;
	myLine = (OGRLineString*) OGRGeometryFactory::createGeometry(wkbLineString);
	for (int i = 0; i<nbvertex;i++)
		myLine->addPoint(points[i].x, points[i].y);
	
	m_Frame->addRing((OGRLinearRing*) myLine);
	
	OGRGeometryFactory::destroyGeometry(myLine);
	
}



/***************************************************************************//**
 @brief Setting simple attributs into SHP for lines
 @param myLayer informations about the current layer
 @return  true if data passed successfully to the SHP
 @author Lucien Schreiber (c) CREALP 2008
 @date 17 November 2008
 *******************************************************************************/
bool tmExportDataSHP::AddSimpleDataToLine (ProjectDefMemoryLayers * myLayer)
{
	wxASSERT(m_pDB);
	
	if (!GetSimpleAttribData(myLayer->m_LayerType, myLayer->m_LayerID))
	{	
		wxLogDebug(_T("Unable to get layer attribution information"));
		return false;
	}
	
	//bool bFirstLoop = true;
	wxArrayString myResults;
	bool bSetFieldValue = true;
	
	if(m_Shp.SetNextFeature(true)==false)
		return false;
		
	while (m_pDB->DataBaseGetNextResult(myResults)) 
	{

		wxASSERT(myResults.GetCount() == 2);
		
		if(!m_Shp.SetFieldValue(myResults.Item(0), TM_FIELD_INTEGER, 1))
		{
			bSetFieldValue = false;
			break;
		}
		
		if(!m_Shp.SetFieldValue(myResults.Item(1), TM_FIELD_TEXT, 2))
		{
			bSetFieldValue = false;
			break;
		}
		
		m_Shp.UpdateFeature();
		
		
		if(m_Shp.SetNextFeature(false)==false)
			break;

	}
	
	
	m_pDB->DataBaseClearResults();
	return bSetFieldValue;
	
}


/***************************************************************************//**
 @brief Setting simple attributs into SHP for points
 @details Actually this is exactly the same as calling 
 tmExportDataSHP::AddSimpleDataToLine() but is kept for clarity.
 @param myLayer informations about the current layer
 @return  true if data passed successfully to the SHP
 @author Lucien Schreiber (c) CREALP 2008
 @date 17 November 2008
 *******************************************************************************/
bool tmExportDataSHP::AddSimpleDataToPoint (ProjectDefMemoryLayers * myLayer)
{
	return AddSimpleDataToLine(myLayer);	
}


/***************************************************************************//**
 @brief Setting simple attributs into SHP for polygons
 @param myLayer informations about the current layer
 @return  true if data passed successfully to the SHP
 @author Lucien Schreiber (c) CREALP 2008
 @date 17 November 2008
 *******************************************************************************/
bool tmExportDataSHP::AddSimpleDataToPolygon (ProjectDefMemoryLayers * myLayer)
{
	wxASSERT(m_pDB);
	
	// process request 
	if (!GetSimpleAttribDataWithSpatial (myLayer->m_LayerType, myLayer->m_LayerID))
	{	
		wxLogDebug(_T("Unable to get layer attribution information"));
		return false;
	}
	
	tmGISDataVectorMYSQL mySQLData;
	tmGISDataVectorMYSQL::SetDataBaseHandle(m_pDB);
	
	long myOidPT = 0;
	wxArrayString myAttribVal;
	bool bFirstLoop = true;
	//OGRPolygon * myPolygon;
	long myOidPLG = 0;
	
	// loop for all labels and search polygons
	while (1)
	{

		OGRPoint * myPoint = mySQLData.GetNextDataPointWithAttrib(myOidPT, myAttribVal);
		if (myPoint)
		{
			
			// loop for all polygons
			while (1)
			{
				if(!m_Shp.SetNextFeature(bFirstLoop))
					break;
				
				OGRPolygon * myPolygon = m_Shp.GetNextDataOGRPolygon(myOidPLG);
				if (myPolygon)
				{
					if (myPoint->Intersect(myPolygon))
					{
						m_Shp.SetActualOID(myOidPT);
						m_Shp.SetFieldValue(myAttribVal.Item(0), TM_FIELD_INTEGER, 1);
						m_Shp.SetFieldValue(myAttribVal.Item(1), TM_FIELD_TEXT, 2);
						m_Shp.UpdateFeature();
					}				
					// do not destroy geometry, will be destroyed 
					// when feature is destroyed.
					//OGRGeometryFactory::destroyGeometry(myPolygon);	
				}
				bFirstLoop = false;
			
			}
			
			
			bFirstLoop = true;
			OGRGeometryFactory::destroyGeometry(myPoint);
		}
		else
			break;
	}	
	

	return true;
}


/***************************************************************************//**
 @brief Set Advanced values into SHP columns
 @param layer information about the current layer
 @return  true if data passed succesfully to the shapefile
 @author Lucien Schreiber (c) CREALP 2009
 @date 26 March 2009
 *******************************************************************************/
bool tmExportDataSHP::AddAdvancedDataToLine (ProjectDefMemoryLayers * layer)
{
	// process request
	if (!GetAdvancedAttribution(layer))
	{
		wxLogDebug(_T("No advanced attribution informations for layer %s"),
				   layer->m_LayerName.c_str());
		return false;
	}

	
	bool bFirstLoop = true;
	bool bReturn = true;
	bool bGetNextResult = true;
	wxArrayString myResults;
	//bool bSetFieldValue = true;
	
	while (1) 
	{
		// loop shapefile
		if(!m_Shp.SetNextFeature(bFirstLoop))
			break;
		bFirstLoop = false;
		
		
		// get advanced attribution
		if (bGetNextResult)
		{
			if (m_pDB->DataBaseGetNextResult(myResults)==false)
			{
				wxLogDebug(_T("No more advanced attribution found"));
				break;
			}
			
			
			if (myResults.GetCount() == 1)
			{
				wxLogDebug(_T("No advanced attribution for layer : %d"), 
						   layer->m_LayerName.c_str());
				bReturn = false;
				break;
			}
		}
		
		// compare to OID
		long lAAOid = wxNOT_FOUND;
		myResults.Item(0).ToLong(&lAAOid);
		if (m_Shp.GetActualOID() == lAAOid)
		{
			if (!SetMultipleFields(layer, myResults))
			{
				wxLogError(_("Error exporting advanced attribution for layer : %s, OID : %d"),
						   layer->m_LayerName.c_str(), lAAOid);
			}
			bGetNextResult = true;
		}
		else
		{
			bGetNextResult = false;
		}
		
	}
	m_pDB->DataBaseClearResults();
	return bReturn;
}



bool tmExportDataSHP::AddAdvancedDataToPoint (ProjectDefMemoryLayers * layer)
{
	return AddAdvancedDataToLine(layer);
}


bool tmExportDataSHP::AddAdvancedDataToPolygon (ProjectDefMemoryLayers * layer)
{
	return AddAdvancedDataToLine(layer);
}


/***************************************************************************//**
 @brief Set value for multiple fields
 @details Loop in all layer's field and set value for all of them
 @param layer valid object containing layer (and layer's fields) definition
 @param values values to set to fields (a check is done that the numbers of
 values is egal to the number of fields)
 @return  true if all fields were set correctly
 @author Lucien Schreiber (c) CREALP 2009
 @date 27 March 2009
 *******************************************************************************/
bool tmExportDataSHP::SetMultipleFields (ProjectDefMemoryLayers * layer,
										 const wxArrayString & values)
{
	PrjMemFieldArray * myFields = layer->m_pLayerFieldArray;
	//unsigned int nbField = myFields->GetCount();
	wxASSERT (myFields);
	wxASSERT (values.GetCount()-1 == myFields->GetCount());
	
	for (unsigned int i = 0; i<myFields->GetCount();i++)
	{
		if(!m_Shp.SetFieldValue(values.Item(i+1), myFields->Item(i).m_FieldType, i+3))
			return false;
		else
			m_Shp.UpdateFeature();
	}
	
	return true;
}



