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

#include "tmattributiondataline.h"
#include "tmattributiondatapoint.h"
#include "tmattributiondatalabel.h"


// init static member
DataBaseTM * tmGISDataVectorMYSQL::m_DB = NULL;
//PrjDefMemManage * tmGISDataVectorMYSQL::m_ProjDef = NULL;

tmGISDataVectorMYSQL::tmGISDataVectorMYSQL()
{
	m_PrjDef = NULL;
	m_ClassType = tmGIS_VECTOR_MYSQL;
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
	
	if (m_DB->DataBaseQuery(sSentence)==false)
	{
		if (IsLoggingEnabled())
			wxLogDebug(_T("Error checking geometry fields : %s"), sSentence.c_str());
		return false;
	}
	long myRows = 0;
	if(m_DB->DataBaseGetResultSize(NULL, &myRows)==false)
		return false;
	
	wxASSERT(myRows == 2);
	m_DB->DataBaseClearResults();
	
	
	/*if (m_DB->DatabaseGetCountResults() != 2)
	{
		if (IsLoggingEnabled())
			wxLogDebug(_T("The table : %s dosen't contain needed geometry fields"), tablename.c_str());
		return FALSE;
	}*/
	
	return true;
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
	tmArrayULong row_length;
	
	// query for the geometry enveloppe for all lines
	wxString sSentence = wxString::Format(_T("SELECT Envelope(%s) FROM %s"),
										  tmGISMYSQL_FIELD2.c_str(), GetShortFileName().c_str());
	
	if (m_DB->DataBaseQuery(sSentence)==false)
	{
		wxDELETE(psExtent);
		return tmRealRect(0,0,0,0);
	}
	
	if (m_DB->DataBaseGetNextRowResult(row, row_length)==false)
	{
		m_DB->DataBaseClearResults();
		wxDELETE(psExtent);
		return tmRealRect(0,0,0,0);
	}
	
	OGRGeometry *poGeometry = CreateDataBaseGeometry(row, row_length);
	wxASSERT(poGeometry);
	
	poGeometry->getEnvelope(&oEnv);
	psExtent->MinX = oEnv.MinX;
	psExtent->MinY = oEnv.MinY;
	psExtent->MaxX = oEnv.MaxX;
	psExtent->MaxY = oEnv.MaxY;
	
	OGRGeometryFactory::destroyGeometry(poGeometry);
	
	// loop all lines
	while (m_DB->DataBaseGetNextRowResult(row, row_length))
	{
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
		OGRGeometryFactory::destroyGeometry(poGeometry);
	}
	m_DB->DataBaseClearResults();
	tmRealRect myRect(psExtent->MinX,psExtent->MinY,
					  psExtent->MaxX,psExtent->MaxY);
	wxDELETE(psExtent);
	return myRect;
	
}



OGRGeometry *  tmGISDataVectorMYSQL::CreateDataBaseGeometry(MYSQL_ROW & row,
															const tmArrayULong & row_lengths,
															int geometry_col)
{
	OGRGeometry * geometry = NULL;
	// Geometry columns will have the first 4 bytes contain the SRID.
	OGRGeometryFactory::createFromWkb(((unsigned char *)row[geometry_col]) + 4, 
									  NULL,
									  &geometry,
									  row_lengths.Item(geometry_col) - 4 );
	
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
	//m_DB->DataBaseDestroyResults();
	
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
		bool bResult = m_DB->DataBaseHasResults();
		return bResult;
		//return TRUE;
	}
	
	if (IsLoggingEnabled())
		wxLogDebug(wxString::Format(_T("Error setting spatial filter")));
	
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
	tmArrayULong  row_length;
	
	// security check
	if(m_DB->DataBaseHasResults()==false)
	{
		if (IsLoggingEnabled())
			wxLogError(_T("Database should have results..."));
		nbvertex = 0;
		return NULL;
	}
	
	if (m_DB->DataBaseGetNextRowResult(row, row_length)==false)
	{
		m_DB->DataBaseClearResults();
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



OGRLineString * tmGISDataVectorMYSQL::GetNextDataLine (long & oid)
{
	MYSQL_ROW row;
	tmArrayULong row_length;
	
	// security check
	if(m_DB->DataBaseHasResults()==false)
	{
		if (IsLoggingEnabled())
			wxLogError(_T("Database should have results..."));
		return NULL;
	}
	
	if (m_DB->DataBaseGetNextRowResult(row, row_length)==false)
	{
		m_DB->DataBaseClearResults();
		return NULL;
	}
	
	OGRLineString * pline = (OGRLineString*) CreateDataBaseGeometry(row, row_length, 1);
	oid = GetOid(row, 0);
	wxASSERT (pline);
	return pline;
}



OGRPoint * tmGISDataVectorMYSQL::GetOGRNextDataPoint (long & oid)
{
	MYSQL_ROW row;
	tmArrayULong row_length;
	
	// security check
	if(m_DB->DataBaseHasResults()==false)
	{
		if (IsLoggingEnabled())
			wxLogError(_T("Database should have results..."));
		return NULL;
	}
	
	if (m_DB->DataBaseGetNextRowResult(row, row_length)==false)
	{
		m_DB->DataBaseClearResults();
		return NULL;
	}
	

	OGRPoint * ppoint = (OGRPoint*) CreateDataBaseGeometry(row, row_length, 1);
	oid = GetOid(row, 0);
	return ppoint;
}



OGRPoint * tmGISDataVectorMYSQL::GetNextDataPointWithAttrib (long & oid,
															 wxArrayString & values)
{
	MYSQL_ROW row;
	tmArrayULong row_length;
	values.Clear();
	
	// security check
	if(m_DB->DataBaseHasResults()==false)
	{
		if (IsLoggingEnabled())
			wxLogError(_T("Database should have results..."));
		return NULL;
	}
	
	if (m_DB->DataBaseGetNextRowResult(row, row_length)==false)
	{
		m_DB->DataBaseClearResults();
		return NULL;
	}
	
	unsigned int iRows = 0;
	bool bCount = m_DB->DataBaseGetResultSize(&iRows, NULL);
	wxASSERT(bCount);
	wxASSERT(iRows > 2);
	for (unsigned int i = 2; i<iRows;i++)
	{
		values.Add(wxString ( row[i], wxConvUTF8));
	}
	
	OGRPoint * ppoint = (OGRPoint*) CreateDataBaseGeometry(row, row_length, 1);
	oid = GetOid(row, 0);
	return ppoint;
}



wxRealPoint * tmGISDataVectorMYSQL::GetNextDataPoint (long & oid)
{
	MYSQL_ROW row;
	tmArrayULong row_length;
	
	// security check
	if(m_DB->DataBaseHasResults()==false)
	{
		if (IsLoggingEnabled())
			wxLogError(_T("Database should have results..."));
		return NULL;
	}
	
	if (m_DB->DataBaseGetNextRowResult(row, row_length)==false)
	{
		m_DB->DataBaseClearResults();
		return NULL;
	}
		
	OGRPoint * pPoint = (OGRPoint*) CreateDataBaseGeometry(row, row_length,1);
	oid = GetOid(row, 0);
	wxASSERT(pPoint);
	
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
	if (m_DB->DataBaseQuery(sSentence)==false)
	{
		wxLogDebug(_T("Error getting table type"));
		return myRetVal;
	}
	
	wxString myResult = _T("");
	if(m_DB->DataBaseGetNextResult(myResult)==false)
		return myRetVal;
	
	m_DB->DataBaseClearResults();
	
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
	if (m_DB->DataBaseQuery(sSentence)==false)
	{
		wxLogDebug(_T("Error getting number of features for %s"),
				   GetFullFileName().c_str());
		return 0;
	}
	
	long lNbFeatures = 0;
	if(m_DB->DataBaseGetNextResult(lNbFeatures)==false)
		return 0;
	
	m_DB->DataBaseClearResults();
	return (int) lNbFeatures;
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



int tmGISDataVectorMYSQL::GetFieldsCount() {
	// this function isn't working for MySQL layers because number
	// of fields is linked to the oid !!! Use GetFieldsName();
	return wxNOT_FOUND;
}

tmAttributionData * tmGISDataVectorMYSQL::_CreateAttributionObject(int & layertype){
	// getting layer type (line, point, poly)
	wxASSERT(m_DB);
	wxFileName myTable (GetShortFileName());
	if (myTable.IsOk() == false) {
		wxLogError(_T("Layer : %s wasn't open correctly"), GetShortFileName().c_str());
		return NULL;
	}
	
	layertype = wxNOT_FOUND;
	int iTableSize = sizeof(TABLE_NAME_GIS_GENERIC) / sizeof (wxString);
	for (int i = 0; i< iTableSize; i++){
		if (myTable.GetName() == TABLE_NAME_GIS_GENERIC[i]){
			layertype = i;
			break;
		}
	}
	
	if (layertype == wxNOT_FOUND){
		wxLogError(_T("Layer type : %d isn't found"), layertype);
		return NULL;
	}
	
	// creating tmAttributionData object
	tmAttributionData * myAttribData = NULL;
	switch (layertype) {
		case LAYER_SPATIAL_LINE: 
			myAttribData = new tmAttributionDataLine();
			break;
			
		case LAYER_SPATIAL_POINT:
			myAttribData = new tmAttributionDataPoint();
			break;
			
		case LAYER_SPATIAL_POLYGON:
			myAttribData = new tmAttributionDataLabel();
			break;
			
		default:
			break;
	}
	
	if (myAttribData == NULL) {
		wxLogError(_T("Layer type %d isn't supported actually"), layertype);
		return NULL;
	}
	return myAttribData;
}


bool tmGISDataVectorMYSQL::GetFieldsName (wxArrayString & Fields, long oid){
	// basic initialisation and checks
	Fields.Clear();
	if (oid == wxNOT_FOUND) {
		wxLogError(_T("OID specified is not valid (%d)"), oid);
		return false;
	}
	
	
	if (m_PrjDef == NULL) {
		wxLogError(_T("Project object not specified, use SetProject() first"));
		return false;
	}
	
	int iTableType = wxNOT_FOUND;
	tmAttributionData * myAttribData = _CreateAttributionObject(iTableType);
	if (myAttribData == NULL) {
		return false;
	}
	
	// passing info to attribution data
	wxArrayLong myOid;
	myOid.Add(oid);
	myAttribData->Create(&myOid, m_DB);
	
	tmLayerValueArray myLayerValues;
	if(myAttribData->GetAttributionLayersIDFull(oid, myLayerValues)==false){
		wxDELETE(myAttribData);
		//wxLogError(_T("Error getting attribution layers for oid %d"), oid);
		return false;
	}
	
	// iterate project for getting fields 
	PrjMemLayersArray myLayers;
	for (unsigned int i = 0; i< myLayerValues.GetCount(); i++) {
		ProjectDefMemoryLayers * myLayer = m_PrjDef->FindLayerByRealID(myLayerValues.Item(i).m_Oid);
		if (myLayer == NULL) {
			wxLogWarning(_T("Layer with ID : %d wasn't found in project"), myLayerValues.Item(i).m_Oid);
		}
		else {
			myLayers.Add(new ProjectDefMemoryLayers());
			myLayers.Item(myLayers.GetCount()-1) = *myLayer;
		}
		
	}
	
	// preparing results
	for (unsigned int j = 0; j < myLayers.GetCount(); j++) {
		Fields.Add(_T("OBJ_CD"));
		Fields.Add(_T("OBJ_DESC"));
		
		PrjMemFieldArray * myFields = &(myLayers.Item(j).m_pLayerFieldArray);
		// adding fields only for object of correct type (case of line / poly)
		if (myLayers.Item(j).m_LayerType == iTableType) {
			wxASSERT(myFields);
			for (unsigned int i = 0; i < myFields->GetCount(); i++) {
				Fields.Add(myFields->Item(i)->m_Fieldname);
			}
		}
		
		Fields.Add(_T("##<BREAK HERE>##"));
	
	}
	
	if (Fields.GetCount() > 0) {
		Fields.RemoveAt(Fields.GetCount()-1);
	}
	
	wxDELETE (myAttribData);
	return true;
}






bool tmGISDataVectorMYSQL::GetFieldsValue (wxArrayString & values, long oid){
	values.Clear();
	if (oid == wxNOT_FOUND) {
		wxLogError(_T("OID specified is not valid (%d)"), oid);
		return false;
	}
	
	if (m_PrjDef == NULL) {
		wxLogError(_T("Project object not specified, use SetProject() first"));
		return false;
	}
	
	int iTableType = wxNOT_FOUND;
	tmAttributionData * myAttribData = _CreateAttributionObject(iTableType);
	if (myAttribData == NULL) {
		return false;
	}
	
	// passing info to attribution data
	wxArrayLong myOid;
	myOid.Add(oid);
	myAttribData->Create(&myOid, m_DB);
	
	
	// isolating layers used for attribution
	tmLayerValueArray myLayerValues;
	if(myAttribData->GetAttributionLayersIDFull(oid, myLayerValues)==false){
		wxDELETE(myAttribData);
		//wxLogError(_T("Error getting attribution layers for oid %d"), oid);
		return false;
	}
	
	PrjMemLayersArray myLayers;
	for (unsigned int i = 0; i< myLayerValues.GetCount(); i++) {
		ProjectDefMemoryLayers * myLayer = m_PrjDef->FindLayerByRealID(myLayerValues.Item(i).m_Oid);
		if (myLayer == NULL) {
			wxLogWarning(_T("Layer with ID : %d wasn't found in project"), myLayerValues.Item(i).m_Oid);
		}
		else {
			myLayers.Add(new ProjectDefMemoryLayers());
			myLayers.Item(myLayers.GetCount()-1) = *myLayer;
		}
		
	}

	
	// gettting basic attribution
	tmAttributionBasicArray myValues;
	myAttribData->SetDataBaseTable(TABLE_NAME_GIS_ATTRIBUTION[iTableType]);
	
	wxArrayLong myObjID;
	wxArrayLong myObjCode;
	wxArrayString myObjVal;
	if (myAttribData->GetInfoBasic(oid, myObjID, myObjCode, myObjVal)==false) {
		wxLogError(_T("Error getting basic informations for object OID : %d"), oid);
		return false;
	}
	wxASSERT(myObjVal.GetCount() == myObjCode.GetCount());
	wxASSERT(myObjVal.GetCount() == myLayers.GetCount());
	
		
	for (unsigned int i = 0; i< myObjCode.GetCount(); i++) {
		values.Add(wxString::Format(_T("%d"), myObjCode.Item(i)));
		values.Add(myObjVal.Item(i));
		
		ProjectDefMemoryLayers * myLayer = & (myLayers.Item(i));
		wxASSERT(myLayer);
		wxASSERT(myLayer->m_LayerID != wxNOT_FOUND);
		// getting advanced attribution
		if (myLayer->m_LayerType == iTableType) {
			wxArrayString myAdvValues;
			if (myAttribData->GetAdvancedAttribution(myLayer,myAdvValues, oid)==true) {
				for (unsigned int j = 0; j< myAdvValues.GetCount(); j++) {
					values.Add(myAdvValues.Item(j));
				}
			}
		}
		
		
		values.Add(_T("##<BREAK HERE>##"));
	}
	
	// remove last BREAK
	if (values.GetCount() > 0) {
		values.RemoveAt(values.GetCount()-1);
	}
	
	wxDELETE(myAttribData);
	return true;
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
	//m_DB->DataBaseDestroyResults();

	
	wxString sRect = wxString::Format(_T("POLYGON ((%f %f,%f %f,%f %f,%f %f,%f %f))"),
										rect.x_min, rect.y_min,
										rect.x_max, rect.y_min,
										rect.x_max, rect.y_max,
										rect.x_min, rect.y_max,
										rect.x_min, rect.y_min);
	wxString sSentence = wxString::Format( _T("SELECT OBJECT_ID, OBJECT_GEOMETRY FROM %s WHERE ")
										  _T("Intersects(GeomFromText('%s'),OBJECT_GEOMETRY)"),
										  GetShortFileName().c_str(), sRect.c_str());
		
	
	MYSQL_ROW row;
	tmArrayULong row_size;
	
	if (m_DB->DataBaseQuery(sSentence)==false)
	{
		return NULL;
	}
	
	
	if (m_DB->DataBaseHasResults()==false)
	{
		return NULL;
	}
		
	GEOSGeom  grect = CreateGEOSGeometry(rect);
	wxASSERT(grect);		
		
	
	wxArrayLong * myArray = new wxArrayLong();
	while (m_DB->DataBaseGetNextRowResult(row, row_size))
	{
		
		OGRGeometry * ogrgeom = CreateDataBaseGeometry(row, row_size, 1);
		GEOSGeom  geom = CreateGEOSGeometry(ogrgeom);
		
		if (CheckGEOSIntersection(&grect,&geom))
			myArray->Add(GetOid(row, 0)); 
		
		// destroy geometry
		GEOSGeom_destroy(geom);
		OGRGeometryFactory::destroyGeometry(ogrgeom);	
	}
	GEOSGeom_destroy(grect);
	m_DB->DataBaseClearResults();
	return myArray;
}


/***************************************************************************//**
 @brief Get All OID back from the MySQL layer
 @author Lucien Schreiber (c) CREALP 2009
 @date 31 March 2009
 *******************************************************************************/
wxArrayLong * tmGISDataVectorMYSQL::GetAllData ()
{
	wxString sSentence = wxString::Format( _T("SELECT OBJECT_ID FROM %s ORDER BY OBJECT_ID"),
										  GetShortFileName().c_str());
	if (m_DB->DataBaseQuery(sSentence)==false)
		return NULL;
	
	
	wxArrayLong * mySel = new wxArrayLong();
	
	if (m_DB->DataBaseGetResults(*mySel)==false)
	{
		delete mySel;
		return NULL;
	}
	
	wxASSERT(mySel);
	return mySel;
}


/***************************************************************************//**
 @brief Search data for intersection
 @details Search in all dataset for objects intersecting the passed geometry.
 First we specify a rectangle (for spatial filter) and then we search with GEOS
 inside the results
 @param intersectinggeom The geometry we search for intersection
 @return  Adress of an array of OID, or NULL if nothing found
 @author Lucien Schreiber (c) CREALP 2009
 @date 09 February 2009
 *******************************************************************************/
wxArrayLong * tmGISDataVectorMYSQL::SearchIntersectingGeometry (OGRGeometry * intersectinggeom)
{
	//m_DB->DataBaseDestroyResults();
	
	// create bounding box
	wxASSERT (intersectinggeom);
	OGREnvelope myEnv;
	intersectinggeom->getEnvelope(&myEnv);
	
	
	wxString sRect = wxString::Format(_T("POLYGON ((%f %f,%f %f,%f %f,%f %f,%f %f))"),
									  myEnv.MinX, myEnv.MinY,
									  myEnv.MaxX, myEnv.MinY,
									  myEnv.MaxX, myEnv.MaxY,
									  myEnv.MinX, myEnv.MaxY,
									  myEnv.MinX, myEnv.MinY);
	wxString sSentence = wxString::Format( _T("SELECT OBJECT_ID, OBJECT_GEOMETRY FROM %s WHERE ")
										  _T("Intersects(GeomFromText('%s'),OBJECT_GEOMETRY)"),
										  GetShortFileName().c_str(), sRect.c_str());
	
	MYSQL_ROW row;
	tmArrayULong row_size;
	GEOSGeom grect = CreateGEOSGeometry(intersectinggeom);
	
	if (m_DB->DataBaseQuery(sSentence)==false)
	{
		GEOSGeom_destroy(grect);
		return NULL;
	}
	
	if (m_DB->DataBaseHasResults()==false)
	{
		GEOSGeom_destroy(grect);
		return NULL;
	}
	
	wxArrayLong * myArray = new wxArrayLong();
	while (m_DB->DataBaseGetNextRowResult(row, row_size))
	{
		OGRGeometry * ogrgeom = CreateDataBaseGeometry(row, row_size, 1);
		GEOSGeom  geom = CreateGEOSGeometry(ogrgeom);
		
		if (CheckGEOSCrosses(&grect,&geom))
			myArray->Add(GetOid(row, 0)); 
		
		// destroy geometry
		GEOSGeom_destroy(geom);
		OGRGeometryFactory::destroyGeometry(ogrgeom);
		
	}
	GEOSGeom_destroy(grect);
	m_DB->DataBaseClearResults();
	return myArray;
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
bool tmGISDataVectorMYSQL::GetSnapCoord (const wxRealPoint & clickpt, int iBuffer,
									   wxRealPoint & snappt, int snaptype)
{
	 //create OGRpoint and buffer
	OGRPoint myClickPoint;
	myClickPoint.setX(clickpt.x);
	myClickPoint.setY(clickpt.y);
	
	OGRGeometry * myBufferClick = tmGISDataVector::SafeBuffer(&myClickPoint, iBuffer);
	wxASSERT (myBufferClick);
		
	
	// convert buffer to text for sql query
	char * buffer = NULL;
	myBufferClick->exportToWkt(&buffer);
	wxASSERT(buffer);
	wxString mySBuffer = wxString::FromAscii(buffer);
	OGRFree(buffer);
	
	
	
	// search for intersecting features
	wxString sSentence = wxString::Format( _T("SELECT OBJECT_ID, OBJECT_GEOMETRY FROM %s WHERE ")
										  _T("Intersects(GeomFromText('%s'),OBJECT_GEOMETRY)"),
										  GetShortFileName().c_str(), mySBuffer.c_str());
		
	
	MYSQL_ROW row;
	tmArrayULong row_size;
	
	if (m_DB->DataBaseQuery(sSentence)==false)
	{
		wxLogError(_T("Error getting snapping info"));
		OGRGeometryFactory::destroyGeometry(myBufferClick);
		return false;
	}
	
	// no results found
	if (m_DB->DataBaseHasResults()==false)
	{
		OGRGeometryFactory::destroyGeometry(myBufferClick);
		return false;
	}
		
	
	// search into returned object for intersection
	bool bReturn = false;
	wxRealPoint * mySnapPoint = NULL;
	
	while(m_DB->DataBaseGetNextRowResult(row, row_size))
	{
		OGRGeometry * poGeometry = CreateDataBaseGeometry(row, row_size, 1);
		if (poGeometry->Intersects(myBufferClick))
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
		OGRGeometryFactory::destroyGeometry(poGeometry);
		
		if (mySnapPoint)
		{
			snappt = wxRealPoint(mySnapPoint->x,mySnapPoint->y);
			delete mySnapPoint;
			bReturn = true;
			break;
		}
	}
	m_DB->DataBaseClearResults();
	OGRGeometryFactory::destroyGeometry(myBufferClick);
	return bReturn;
}



/***************************************************************************//**
 @brief Get the geometry for specified oid
 @param oid The OID of the searched feature
 @return  A valid OGRGeometry (caller must delete the returned geometry) or NULL
 if an error occur
 @author Lucien Schreiber (c) CREALP 2009
 @date 06 February 2009
 *******************************************************************************/
OGRFeature * tmGISDataVectorMYSQL::GetFeatureByOID (long oid)
{
	wxString sSentence = wxString::Format(_T("SELECT OBJECT_ID, OBJECT_GEOMETRY FROM %s WHERE ")
										  _T("OBJECT_ID = %d;"),
										  GetShortFileName().c_str(),
										  oid);
	long myUnusedOid = 0;
	if (m_DB->DataBaseQuery(sSentence)==false)
	{
		wxLogError(_T("Error getting geometry for oid = %d"),oid);
		return false;
	}
	
	OGRGeometry * myGeom = GetNextDataLine(myUnusedOid);
	wxASSERT(myGeom);
	
	// create feature from geometry
	OGRFeatureDefn * myFeatDef = new OGRFeatureDefn(GetShortFileName().mb_str());
	OGRFieldDefn myFieldDef (wxString(_T("test")).mb_str(), OFTInteger);
	myFeatDef->AddFieldDefn(&myFieldDef);
#if (defined(__WXMSW__)  && defined(__WXDEBUG__))
	// this is needed for windows but generate a memory leak under other plateforms
	myFeatDef->Reference();
#endif
	if (TABLE_NAME_GIS_GENERIC[0] == GetShortFileName()) {
		myFeatDef->SetGeomType(wkbLineString);
	}
	else if (TABLE_NAME_GIS_GENERIC[1] == GetShortFileName()){
		myFeatDef->SetGeomType(wkbPoint);
	}
	else if (TABLE_NAME_GIS_GENERIC[2] == GetShortFileName()){
		myFeatDef->SetGeomType(wkbPoint);
	}
	else if (TABLE_NAME_GIS_GENERIC[3] == GetShortFileName()){
		myFeatDef->SetGeomType(wkbPoint);
	}
	else if (TABLE_NAME_GIS_GENERIC[4] == GetShortFileName()){
		myFeatDef->SetGeomType(wkbLineString);
	}

	
	OGRFeature * myFeature = OGRFeature::CreateFeature(myFeatDef);
	myFeature->SetGeometryDirectly(myGeom);
	myFeature->SetFID(oid);
	
	m_DB->DataBaseClearResults();
	return myFeature;
}



/***************************************************************************//**
 @brief Add geometry into database
 @param Geom The Geometry to Add
 @param oid No oid may be specified for MySQL, it's auto-incremented
 @param layertype One of the #TOC_GENERIC_NAME
 @return  OID of last inserted item or -1 if an errror occur
 @author Lucien Schreiber (c) CREALP 2009
 @date 06 February 2009
 *******************************************************************************/
long tmGISDataVectorMYSQL::AddGeometry (OGRGeometry * Geom, const long & oid, int layertype)
{
	// check that function was used correctly
	wxASSERT(layertype != wxNOT_FOUND);
	
	
	char * myCharGeom = NULL;
	Geom->setCoordinateDimension(2);
	Geom->exportToWkt(&myCharGeom);
	if (!myCharGeom)
		return wxNOT_FOUND; 
	
	wxString mySGeom = wxString::FromAscii(myCharGeom);
	OGRFree(myCharGeom);

	
	if (layertype == wxNOT_FOUND)
		return wxNOT_FOUND;
	
	
	wxString sSentence = wxString::Format(_T("INSERT INTO %s (OBJECT_GEOMETRY)")
										  _T(" VALUES (GeomFromText('%s'));"),
										  GetTableName(layertype).c_str(),
										  mySGeom.c_str());
	if (m_DB->DataBaseQueryNoResults(sSentence)==false)
	{
		wxLogDebug(_T("Error inserting geometry into database"));
		return -1;
	}
		
	return m_DB->DataBaseGetLastInsertedID();
}


/***************************************************************************//**
 @brief Update the geometry for specified OID
 @param geom The Geometry to Add
 @param oid The OID we want to update
 @return  true if the geometry was added successfull
 @author Lucien Schreiber (c) CREALP 2009
 @date 06 February 2009
 *******************************************************************************/
bool tmGISDataVectorMYSQL::UpdateGeometry (OGRGeometry * geom, const long & oid)
{
	char * myCharGeom = NULL;
	if (geom == NULL)
		return false;
	
	geom->setCoordinateDimension(2);
	geom->exportToWkt(&myCharGeom);
	if (!myCharGeom)
		return false;
	wxString mySGeom = wxString::FromAscii(myCharGeom);
	OGRFree(myCharGeom);
	
	
	wxString sSentence = wxString::Format(_T("UPDATE  %s SET OBJECT_GEOMETRY=")
										  _T("(GeomFromText('%s')) WHERE OBJECT_ID=%d;"),
										  GetShortFileName().c_str(),
										  mySGeom.c_str(), 
										  oid);
	if (m_DB->DataBaseQueryNoResults(sSentence)==false)
	{
		wxLogDebug(_T("Error updating geometry"));
		return false;
	}	
	return true;
}



/***************************************************************************//**
 @brief Get all geometries passed as array
 @param OIDs List of all OIDs for getting geometries
 @return  All geometries as a geometry collection or NULL if an error occur
 user should delete the returned geometries after use.
 @author Lucien Schreiber (c) CREALP 2009
 @date 27 February 2009
 *******************************************************************************/
OGRGeometryCollection * tmGISDataVectorMYSQL::
						GetGeometryColByOID(wxArrayLong * OIDs)
{
	// prepare query
	wxString sSentence = wxString::Format(_T("SELECT OBJECT_ID, OBJECT_GEOMETRY FROM %s WHERE ")
										  _T("OBJECT_ID IN ("),
										  GetShortFileName().c_str());
	unsigned int i = 0;
	for (i = 0; i< OIDs->GetCount();i++)
	{
		sSentence.Append(wxString::Format(_T("%d,"), OIDs->Item(i)));
	}
	sSentence.RemoveLast(1);
	sSentence.Append(_T(");"));
	
	// run query
	if (m_DB->DataBaseQuery(sSentence)==false)
	{
		if (IsLoggingEnabled())
			wxLogError(_T("Error getting geometry for multiple oid"));
		return NULL;
	}
	
	
	// create geometries
	OGRGeometryCollection * myGeomCol = (OGRGeometryCollection*) 
				OGRGeometryFactory::createGeometry(wkbGeometryCollection);
	MYSQL_ROW row;
	tmArrayULong row_length;

	for (i = 0; i< OIDs->GetCount();i++)
	{
		if (m_DB->DataBaseGetNextRowResult(row, row_length)==false)
			break;
		
		OGRGeometry * pGeom = CreateDataBaseGeometry(row, row_length, 1);
		myGeomCol->addGeometry(pGeom);
		OGRGeometryFactory::destroyGeometry(pGeom);
	}
	m_DB->DataBaseClearResults();
	return myGeomCol;
}




