/***************************************************************************
 tmgisdatavectorshp.cpp
 class for dealing with vector SHP data
 -------------------
 copyright : (C) 2007 CREALP Lucien Schreiber
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
#include "../core/projectdefmemory.h"
#include "../core/tmsnappingmemory.h"
#include "../database/database.h"

tmGISDataVectorSHP::tmGISDataVectorSHP() {
  m_Datasource = nullptr;
  m_Layer = nullptr;
  m_Feature = nullptr;
  m_polyTotalRings = 0;
  m_ClassType = tmGIS_VECTOR_SHAPEFILE;
  m_RasterizeDataset = nullptr;
  m_MultiFeaturesIterator = 0;
  m_MultiFeaturesOid = 0;
}

tmGISDataVectorSHP::~tmGISDataVectorSHP() {
  // safe destroy the datasource if needed
  if (m_Datasource) {
    GDALClose(m_Datasource);
  }

  if (m_RasterizeDataset) {
    GDALClose(m_RasterizeDataset);
  }
}

bool tmGISDataVectorSHP::Open(const wxString &filename, bool bReadWrite) {
  // init parent member values
  tmGISData::Open(filename, bReadWrite);

  // convert utf wxString into char *
  // const char* ascii_str =
  char *buffer = new char[filename.Length() * sizeof(wxString)];
  strcpy(buffer, (const char *)filename.mb_str(wxConvUTF8));

  // open the shapefile and return true if success
  unsigned int openFlags = GDAL_OF_VECTOR | GDAL_OF_READONLY;
  if (bReadWrite) {
    openFlags = GDAL_OF_VECTOR | GDAL_OF_UPDATE;
  }

  m_Datasource = (GDALDataset *)GDALOpenEx(buffer, openFlags, nullptr, nullptr, nullptr);
  wxDELETEA(buffer);

  if (m_Datasource == nullptr) {
    if (IsLoggingEnabled()) {
      wxLogDebug(_T("Unable to open shp : %s"), filename.c_str());
    }
    return false;
  }

  m_Layer = m_Datasource->GetLayer(0);
  wxASSERT(m_Layer);
  return true;
}

bool tmGISDataVectorSHP::Close() {
  if (m_Datasource) {
    GDALClose(m_Datasource);
  }
  m_Datasource = nullptr;
  m_Layer = nullptr;
  m_Feature = nullptr;
  m_polyTotalRings = 0;
  return true;
}

tmRealRect tmGISDataVectorSHP::GetMinimalBoundingRectangle() {
  wxASSERT(m_Layer);

  OGREnvelope myEnveloppe;
  OGRErr myError = m_Layer->GetExtent(&myEnveloppe, TRUE);
  if (myError == OGRERR_FAILURE) {
    if (IsLoggingEnabled()) {
      wxLogDebug(_T("Unable to compute extend for %s"), GetShortFileName().c_str());
    }
    return tmRealRect(0, 0, 0, 0);
  }

  return tmRealRect(myEnveloppe.MinX, myEnveloppe.MinY, myEnveloppe.MaxX, myEnveloppe.MaxY);
}

TM_GIS_SPATIAL_TYPES tmGISDataVectorSHP::GetSpatialType() {
  OGRGeometry *poGeometry;
  OGRFeature *poFeature;
  TM_GIS_SPATIAL_TYPES retvalue = LAYER_ERR;

  wxASSERT(m_Layer);

  // computing features count, not able to know the
  // spatial type if no features are present.
  if (m_Layer->GetFeatureCount() <= 0) {
    if (IsLoggingEnabled()) {
      wxLogError(_("Unable to add the %s layer, layer is empty"), GetShortFileName().c_str());
    }
    return LAYER_EMPTY;
  }

  // computing layer type (point, line, polygon or unknown)
  m_Layer->ResetReading();
  if ((poFeature = m_Layer->GetNextFeature()) == nullptr) {
    if (IsLoggingEnabled()) {
      wxLogError(_("Unable to read feature from : %s, layer may be corrupted"), GetShortFileName().c_str());
    }
    return LAYER_SPATIAL_UNKNOWN;
  }

  poGeometry = poFeature->GetGeometryRef();
  if (poGeometry != nullptr) {
    OGRwkbGeometryType type = wkbFlatten(poGeometry->getGeometryType());

    switch (type) {
      case wkbLineString:
      case wkbMultiLineString:
        retvalue = LAYER_SPATIAL_LINE;
        break;
      case wkbPoint:
      case wkbMultiPoint:
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

  OGRFeature::DestroyFeature(poFeature);

  if (retvalue == LAYER_ERR) {
    if (IsLoggingEnabled()) {
      wxLogDebug(_T("Error getting spatial layer type for : %s"), GetShortFileName().c_str());
    }
  }
  m_Layer->ResetReading();
  return retvalue;
}

bool tmGISDataVectorSHP::SetSpatialFilter(tmRealRect filter, int type) {
  wxASSERT(m_Layer);
  // clearing filter...
  if (filter == tmRealRect(0, 0, 0, 0)) {
    m_Layer->SetSpatialFilter(nullptr);
  } else {
    m_Layer->SetSpatialFilterRect(filter.x_min, filter.y_min, filter.x_max, filter.y_max);
  }
  m_Layer->ResetReading();
  return true;
}

bool tmGISDataVectorSHP::SetAttributeFilter(const wxString &query) {
  wxASSERT(m_Layer);
  if (query == wxEmptyString) {
    m_Layer->SetAttributeFilter(nullptr);
    return true;
  }

  // escape ' from query
  wxString myQuery(query);
  myQuery.Replace(_T("'"), _T("\\'"));
  myQuery.Replace(_T("\""), _T("'"));

  m_Layer->ResetReading();
  if (m_Layer->SetAttributeFilter((const char *)myQuery.mb_str(wxConvUTF8)) != OGRERR_NONE) {
    wxLogError(_("%s query fail on layer : '%s'"), myQuery, wxString(m_Layer->GetLayerDefn()->GetName()));
    return false;
  }
  return true;
}

wxRealPoint *tmGISDataVectorSHP::GetNextDataLine(int &nbvertex, long &oid, bool &isOver) {
  isOver = false;

  wxASSERT(m_Layer);
  OGRFeature *poFeature = nullptr;

  if (m_MultiFeaturesIterator > 0) {
    poFeature = m_Layer->GetFeature(m_MultiFeaturesOid);
  } else {
    poFeature = m_Layer->GetNextFeature();
  }

  // nothing more to read
  if (poFeature == nullptr) {
    nbvertex = 0;
    oid = -1;
    isOver = true;
    return nullptr;
  }

  oid = poFeature->GetFID();

  OGRGeometry *geom = poFeature->GetGeometryRef();
  OGRLineString *pline;

  if (wkbFlatten(geom->getGeometryType()) == wkbMultiLineString) {
    m_MultiFeaturesOid = oid;

    OGRMultiLineString *pmline = geom->toMultiLineString();
    OGRGeometry *geomLine = pmline->getGeometryRef(m_MultiFeaturesIterator);
    wxASSERT(wkbFlatten(geomLine->getGeometryType()) == wkbLineString);

    pline = geomLine->toLineString();
    if (!pline) {
      wxLogWarning(_("Line %d is corrupted in file: '%s'!"), (int)poFeature->GetFID(), wxString(m_Layer->GetName()));
      return nullptr;
    }

    // normal reading
    nbvertex = pline->getNumPoints();
    if (nbvertex <= 1) {
      if (IsLoggingEnabled()) {
        wxLogDebug(_T("Only one vertex or less in this line ???"));
      }
      OGRGeometryFactory::destroyGeometry(pmline);
      return nullptr;
    }

    wxRealPoint *pts = new wxRealPoint[nbvertex];
    for (int i = 0; i < nbvertex; i++) {
      pts[i].x = pline->getX(i);
      pts[i].y = pline->getY(i);
    }

    int nbLines = pmline->getNumGeometries();

    if (m_MultiFeaturesIterator >= pmline->getNumGeometries() - 1) {
      m_MultiFeaturesIterator = 0;
    } else {
      m_MultiFeaturesIterator++;
    }

    OGRFeature::DestroyFeature(poFeature);

    return pts;
  }

  if (wkbFlatten(geom->getGeometryType()) != wkbLineString) {
    wxLogWarning(_("Line %d is not a single line in file: '%s'!"), (int)poFeature->GetFID(),
                 wxString(m_Layer->GetName()));
    return nullptr;
  }

  pline = geom->toLineString();
  if (!pline) {
    wxLogWarning(_("Line %d is corrupted in file: '%s'!"), (int)poFeature->GetFID(), wxString(m_Layer->GetName()));
    return nullptr;
  }

  // normal reading
  nbvertex = pline->getNumPoints();
  if (nbvertex <= 1) {
    if (IsLoggingEnabled()) {
      wxLogDebug(_T("Only one vertex or less in this line ???"));
    }
    OGRGeometryFactory::destroyGeometry(pline);
    return nullptr;
  }

  wxRealPoint *pts = new wxRealPoint[nbvertex];
  for (int i = 0; i < nbvertex; i++) {
    pts[i].x = pline->getX(i);
    pts[i].y = pline->getY(i);
  }

  OGRFeature::DestroyFeature(poFeature);

  return pts;
}

bool tmGISDataVectorSHP::ResetReading() {
  m_Layer->ResetReading();

  return true;
}

OGRFeature *tmGISDataVectorSHP::GetNextFeature() {
  wxASSERT(m_Layer);
  OGRFeature *poFeature = m_Layer->GetNextFeature();

  return poFeature;
  // delete returned value after use
}

// OGRGeometry * tmGISDataVectorSHP::GetGeometryByOID (long oid){
OGRFeature *tmGISDataVectorSHP::GetFeatureByOID(long oid) {
  wxASSERT(m_Layer);
  wxASSERT(oid != wxNOT_FOUND);

  OGRFeature *myFeature = m_Layer->GetFeature(oid);
  if (myFeature == nullptr) {
    // wxLogError(_T("Error getting feature number : %ld"), oid);
    return nullptr;
  }
  return myFeature;
}

OGRGeometry *tmGISDataVectorSHP::GetNextGeometry(bool restart, long &oid) {
  wxASSERT(m_Layer);
  OGRFeature *poFeature = m_Layer->GetNextFeature();
  // nothing more to read
  if (poFeature == nullptr) {
    oid = wxNOT_FOUND;
    return nullptr;
  }

  OGRGeometry *myGeom = poFeature->GetGeometryRef()->clone();
  oid = poFeature->GetFID();
  OGRFeature::DestroyFeature(poFeature);
  return myGeom;
}

bool tmGISDataVectorSHP::SelectFeatureByOID(long oid) {
  wxASSERT(m_Feature == nullptr);
  wxASSERT(m_Layer);
  m_Feature = m_Layer->GetFeature(oid);
  if (m_Feature != nullptr) {
    return true;
  }
  return false;
}

wxRealPoint *tmGISDataVectorSHP::GetNextDataPoint(long &oid, bool &isOver) {
  isOver = false;

  wxASSERT(m_Layer);
  OGRFeature *poFeature = nullptr;

  if (m_MultiFeaturesIterator > 0) {
    poFeature = m_Layer->GetFeature(m_MultiFeaturesOid);
  } else {
    poFeature = m_Layer->GetNextFeature();
  }

  // nothing more to read
  if (poFeature == nullptr) {
    oid = -1;
    isOver = true;
    return nullptr;
  }

  oid = poFeature->GetFID();

  OGRGeometry *geom = poFeature->GetGeometryRef();
  OGRPoint *pPoint = nullptr;

  if (wkbFlatten(geom->getGeometryType()) == wkbMultiPoint) {
    m_MultiFeaturesOid = oid;

    OGRMultiPoint *pmPoint = geom->toMultiPoint();
    OGRGeometry *geomPoint = pmPoint->getGeometryRef(m_MultiFeaturesIterator);

    pPoint = geomPoint->toPoint();
    if (!pPoint) {
      wxLogWarning(_("Point %d is corrupted in file: '%s'!"), (int)poFeature->GetFID(), wxString(m_Layer->GetName()));
      OGRFeature::DestroyFeature(poFeature);
      return nullptr;
    }
    wxASSERT(wkbFlatten(pPoint->getGeometryType()) == wkbPoint);

    wxRealPoint *pts = new wxRealPoint();
    pts->x = pPoint->getX();
    pts->y = pPoint->getY();

    int nbPoints = pmPoint->getNumGeometries();

    if (m_MultiFeaturesIterator >= pmPoint->getNumGeometries() - 1) {
      m_MultiFeaturesIterator = 0;
    } else {
      m_MultiFeaturesIterator++;
    }

    OGRFeature::DestroyFeature(poFeature);

    return pts;
  }

  if (wkbFlatten(geom->getGeometryType()) != wkbPoint) {
    wxLogWarning(_("Point %d is not a single point in file: '%s'!"), (int)poFeature->GetFID(),
                 wxString(m_Layer->GetName()));
    return nullptr;
  }

  pPoint = geom->toPoint();
  if (!pPoint) {
    wxLogWarning(_("Point %d is corrupted in file: '%s'!"), (int)poFeature->GetFID(), wxString(m_Layer->GetName()));
    return nullptr;
  }

  wxRealPoint *pts = new wxRealPoint();
  pts->x = pPoint->getX();
  pts->y = pPoint->getY();

  OGRFeature::DestroyFeature(poFeature);

  return pts;
}

int tmGISDataVectorSHP::GetNextDataPolygonInfo(long &oid) {
  wxASSERT(m_Layer);
  m_Feature = m_Layer->GetNextFeature();

  // nothing more to read
  if (m_Feature == nullptr) {
    m_polyTotalRings = 0;
    oid = -1;
    return 0;
  }

  oid = m_Feature->GetFID();

  OGRGeometry *geom = m_Feature->GetGeometryRef();

  if (wkbFlatten(geom->getGeometryType()) == wkbMultiPolygon) {
    wxLogWarning(_("Unsupported multipolygons in file: '%s'!"), wxString(m_Layer->GetName()));
    return 0;
  }

  OGRPolygon *plgon = geom->toPolygon();
  wxASSERT(plgon);

  if (plgon == nullptr) {
    if (IsLoggingEnabled()) {
      wxLogDebug(_T("Conversion to polygon error"));
    }
    m_polyTotalRings = 0;
    OGRFeature::DestroyFeature(m_Feature);
    return 0;
  }

  // check polygons validity, long operations #Bug 275
  /*
  if(!plgon->IsValid())
  {
      if (IsLoggingEnabled()){
          wxLogDebug(_T("Polygon not valid @ FID : %ld"), m_Feature->GetFID());
      }
  }*/

  // count rings + 1 (exterior ring)
  m_polyTotalRings = plgon->getNumInteriorRings() + 1;
  return m_polyTotalRings;
}

wxRealPoint *tmGISDataVectorSHP::GetNextDataPolygon(int currentring, int &nbvertex) {
  if (m_Feature == nullptr) {
    if (IsLoggingEnabled()) {
      wxLogDebug(_T("Feature is null, call GetNextDataPolygonInfo first"));
    }
    m_polyTotalRings = 0;
    return nullptr;
  }

  OGRPolygon *plgon = (OGRPolygon *)m_Feature->GetGeometryRef();
  wxASSERT(plgon);

  // getting ring data (exterior then interior)
  OGRLineString *pLinePoly = nullptr;
  if (currentring == 0)
    pLinePoly = plgon->getExteriorRing();
  else
    pLinePoly = plgon->getInteriorRing(currentring - 1);

  if (pLinePoly == nullptr) {
    if (IsLoggingEnabled()) {
      wxLogDebug(_T("Error getting ring for polygon. Ring num. is : %d"), currentring);
    }
    m_polyTotalRings = 0;
    OGRFeature::DestroyFeature(m_Feature);
    return nullptr;
  }

  // Exporting vertex for rings
  nbvertex = pLinePoly->getNumPoints();
  if (nbvertex <= 1) {
    if (IsLoggingEnabled()) {
      wxLogDebug(_T("Only one vertex or less in this polygon ring ???"));
    }
    OGRGeometryFactory::destroyGeometry(pLinePoly);
    return nullptr;
  }

  wxRealPoint *pts = new wxRealPoint[nbvertex];
  for (int i = 0; i < nbvertex; i++) {
    pts[i].x = pLinePoly->getX(i);
    pts[i].y = pLinePoly->getY(i);
  }

  // destroying feature only if all rings read
  if (currentring == m_polyTotalRings) OGRFeature::DestroyFeature(m_Feature);

  return pts;
}

/***************************************************************************/ /**
  @brief Get the actual feature as polygon
  @param oid the oid of the returned polygon
  @return  a valid OGRPolygon (must be destroyed by caller) or nullptr
  @author Lucien Schreiber (c) CREALP 2008
  @date 17 November 2008
  *******************************************************************************/
OGRPolygon *tmGISDataVectorSHP::GetNextDataOGRPolygon(long &oid) {
  wxASSERT(m_Feature);
  OGRPolygon *myPoly = (OGRPolygon *)m_Feature->GetGeometryRef();
  oid = m_Feature->GetFID();

  return myPoly;
}

/***************************************************************************/ /**
  @brief Counting features in layer
  @details This function return the number of lines, points, or polygons stored
  in the layer.
  @todo Implement this function in #tmGISDataVectorMYSQL
  @return  Number of features found or -1 if an error is encountered
  @author Lucien Schreiber (c) CREALP 2008
  @date 17 September 2008
  *******************************************************************************/
int tmGISDataVectorSHP::GetCount() {
  if (!m_Layer) {
    if (IsLoggingEnabled()) {
      wxLogDebug(_T("m_layer not defined, error"));
    }
    return -1;
  }

  return m_Layer->GetFeatureCount();
}

/***************************************************************************/ /**
  @brief Get Metadata information well formated
  @return  An html string to be displayed in the properties dialog
  @author Lucien Schreiber (c) CREALP 2008
  @date 22 October 2008
  *******************************************************************************/
wxString tmGISDataVectorSHP::GetMetaDataAsHtml() {
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

/***************************************************************************/ /**
  @brief Getting Data size
  @details Return the size of the dataset
  @return  Human readable and html compliant string containing data size
  @author Lucien Schreiber (c) CREALP 2008
  @date 23 October 2008
  *******************************************************************************/
wxString tmGISDataVectorSHP::GetDataSizeAsHtml(int iPrecision) {
  // compute size for all part of the shp file
  wxULongLong myFilesSize = 0;
  wxFileName myFileName(GetFullFileName());
  wxString myFileWoutExt = myFileName.GetPath(wxPATH_GET_SEPARATOR) + myFileName.GetName();
  wxString myShpExt[] = {_T(".shp"), _T(".sbn"), _T(".shx"), _T(".dbf")};

  for (unsigned int i = 0; i < (sizeof(myShpExt) / sizeof(wxString)); i++) {
    if (!wxFileName::Exists(myFileWoutExt + myShpExt[i])) {
      continue;
    }
    wxULongLong myTempSize = wxFileName::GetSize(myFileWoutExt + myShpExt[i]);
    if (myTempSize != wxInvalidSize)
      myFilesSize += myTempSize;
    else
      wxLogWarning(_T("Unable to compute %s%s file size, maybe dosen't exists"), myFileWoutExt.c_str(),
                   myShpExt[i].c_str());
  }

  // modifiy the size to be MB
  double dMegaBytes = (myFilesSize.ToDouble() / 1024) / 1024;

  wxString myResult = _("<U><B>File Size</B></U><BR>");
  myResult.Append(wxString::Format(_("Shapefile size is : %.*f [Mb]<BR>"), iPrecision, dMegaBytes));
  return myResult;
}

/***************************************************************************/ /**
  @brief Get the number of fields
  @return  Number of fields. If -1 is returned, an error occur
  @author Lucien Schreiber (c) CREALP 2008
  @date 23 October 2008
  *******************************************************************************/
int tmGISDataVectorSHP::GetFieldsCount() {
  if (!m_Layer) return -1;

  OGRFeatureDefn *poFDefn = m_Layer->GetLayerDefn();
  return poFDefn->GetFieldCount();
}

/***************************************************************************/ /**
  @brief Getting fields name
  @param Fields adress of an array string to be filled with fields name
  @return  true if fields name returned succesfully, false otherwise
  @author Lucien Schreiber (c) CREALP 2008
  @date 23 October 2008
  *******************************************************************************/
bool tmGISDataVectorSHP::GetFieldsName(wxArrayString &Fields, long oid) {
  Fields.Clear();
  if (!m_Layer) return false;

  OGRFeatureDefn *poFDefn = m_Layer->GetLayerDefn();
  OGRFieldDefn *poFieldDf = nullptr;

  for (int i = 0; i < GetFieldsCount(); i++) {
    poFieldDf = poFDefn->GetFieldDefn(i);
    Fields.Add(wxString::FromAscii(poFieldDf->GetNameRef()));
  }

  return true;
}

bool tmGISDataVectorSHP::GetFieldsValue(wxArrayString &values, long oid) {
  values.Clear();

  if (m_Layer == nullptr) {
    wxLogError(_T("No layer defined, unable to get fields value"));
    return false;
  }

  OGRFeature *myFeature = m_Layer->GetFeature(oid);
  if (myFeature == nullptr) {
    wxLogError(_T("No feature with id n\u00B0%ld in layer. Reading fields value not possible"), oid);
    return false;
  }

  int iTotFields = GetFieldsCount();
  for (int i = 0; i < iTotFields; i++) {
#ifdef __WXMSW__
    const char *myValue = myFeature->GetFieldAsString(i);
    wxString myVal(myValue, wxConvUTF8);
    values.Add(myVal);
#else
    wxString myVal(myFeature->GetFieldAsString(i));
    values.Add(myVal);
#endif
  }

  OGRFeature::DestroyFeature(myFeature);
  return true;
}

bool tmGISDataVectorSHP::GetDistinctFieldsValue(const wxString &fieldname, wxArrayString &values) {
  wxASSERT(m_Datasource);
  values.Clear();
  wxString myLayerName(m_Layer->GetLayerDefn()->GetName());
  wxString myQuery = wxString::Format(_T("SELECT DISTINCT \"%s\" FROM \"%s\""), fieldname, myLayerName);
  OGRLayer *myResultLayer = m_Datasource->ExecuteSQL(myQuery, nullptr, nullptr);
  if (myResultLayer == nullptr) {
    return false;
  }

  // iterate results
  myResultLayer->ResetReading();
  OGRFeature *myFeature = nullptr;
  while ((myFeature = myResultLayer->GetNextFeature()) != nullptr) {
#ifdef __WXMSW__
    const char *myValue = myFeature->GetFieldAsString(0);
    wxString myVal(myValue, wxConvUTF8);
    values.Add(myVal);
#else
    wxString myValue(myFeature->GetFieldAsString(0));
    values.Add(myValue);
#endif

    OGRFeature::DestroyFeature(myFeature);
  }
  m_Datasource->ReleaseResultSet(myResultLayer);
  return true;
}

/***************************************************************************/ /**
  @brief Search spatial data
  @param rect Real rectangle for searching data
  @param type Layertype see #tmLayerProperties
  @return  An array containing OID of data found or nullptr if nothing found
  @author Lucien Schreiber (c) CREALP 2008
  @date 31 October 2008
  *******************************************************************************/
wxArrayLong *tmGISDataVectorSHP::SearchData(const tmRealRect &rect, int type) {
  // test one going directly

  m_Layer->ResetReading();
  wxBusyCursor wait;

  OGRGeometry *poGeometry;
  OGRFeature *poFeature;
  OGRGeometry *poRectGeom = CreateOGRGeometry(rect);
  if (!poRectGeom) {
    if (IsLoggingEnabled()) {
      wxLogDebug(_T("Unable to create geometry for rectangle"));
    }
    return nullptr;
  }

  // searching all features
  wxArrayLong *myArray = new wxArrayLong();
  while ((poFeature = m_Layer->GetNextFeature()) != nullptr) {
    poGeometry = poFeature->GetGeometryRef();
    if (poGeometry) {
      if (poGeometry->Intersects(poRectGeom)) myArray->Add(poFeature->GetFID());

      OGRGeometryFactory::destroyGeometry(poGeometry);
    }
  }
  OGRGeometryFactory::destroyGeometry(poRectGeom);

  return myArray;
}

/***************************************************************************/ /**
  @brief Get OID of all features back from the shapefile
  @author Lucien Schreiber (c) CREALP 2009
  @date 31 March 2009
  *******************************************************************************/
wxArrayLong *tmGISDataVectorSHP::GetAllData() {
  wxASSERT(m_Layer);

  OGRFeature *poFeature = nullptr;
  m_Layer->ResetReading();
  // searching all features
  wxArrayLong *myArray = new wxArrayLong();
  while ((poFeature = m_Layer->GetNextFeature()) != nullptr) {
    myArray->Add(poFeature->GetFID());
  }

  return myArray;
}

/***************************************************************************/ /**
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
bool tmGISDataVectorSHP::CreateFile(const wxFileName &filename, int type) {
  const char *pszDriverName = "ESRI Shapefile";
  GDALDriver *poDriver;

  poDriver = GetGDALDriverManager()->GetDriverByName(pszDriverName);
  if (poDriver == nullptr) {
    wxASSERT_MSG(0, _T(" driver not available."));
    return false;
  }

  // creating the file
  wxString mysFileName = filename.GetFullPath();
  char *buffer = new char[mysFileName.Length() * sizeof(wxString)];
  strcpy(buffer, (const char *)mysFileName.mb_str(wxConvUTF8));
  m_Datasource = poDriver->Create(buffer,
                                  0,  // x size, not used for vector
                                  0,  // y size, not used for vector
                                  0,  // nb of bands
                                  GDT_Unknown, NULL);
  wxDELETEA(buffer);

  if (m_Datasource == nullptr) {
    wxLogError(_("Creation of file '%s' failed. Is it open in another software?"), filename.GetFullName().c_str());
    return false;
  }

  OGRwkbGeometryType myGeomType = wkbUnknown;
  switch (type) {
    case 0:  // LINE
      myGeomType = wkbLineString;
      break;

    case 1:  // POINT
      myGeomType = wkbPoint;
      break;

    case 2:  // POLYGON
      myGeomType = wkbPolygon;
      break;

    default:
      myGeomType = wkbUnknown;
      break;
  }

  if (myGeomType == wkbUnknown) return false;

  // creating the layer
  wxString myFileNameWOExt = filename.GetName();
  char *buffer2 = new char[myFileNameWOExt.Length() * sizeof(wxString)];
  strcpy(buffer2, (const char *)myFileNameWOExt.mb_str(wxConvUTF8));
  m_Layer = m_Datasource->CreateLayer(buffer2, nullptr, myGeomType, nullptr);
  wxDELETEA(buffer2);

  if (m_Layer == nullptr) {
    wxASSERT_MSG(0, _T("Layer creation failed."));
    return false;
  }

  tmGISData::Open(filename.GetFullPath(), true);
  return true;
}

/***************************************************************************/ /**
  @brief Add text fields to the Shp
  @param fieldname name of the field
  @param size length of the field
  @return true if field added, false otherwise
  @author Lucien Schreiber (c) CREALP 2008
  @date 15 November 2008
  *******************************************************************************/
bool tmGISDataVectorSHP::AddFieldText(const wxString &fieldname, int size) {
  wxASSERT(m_Layer);
  bool bReturn = true;

  char *buffer = new char[fieldname.Length() * sizeof(wxString)];
  strcpy(buffer, (const char *)fieldname.mb_str(wxConvUTF8));
  OGRFieldDefn oField(buffer, OFTString);
  wxDELETEA(buffer);

  oField.SetWidth(size);

  if (m_Layer->CreateField(&oField) != OGRERR_NONE) {
    wxASSERT_MSG(0, _T("Creating text field failed."));
    bReturn = false;
  }
  return bReturn;
}

/***************************************************************************/ /**
  @brief Add int field to the Shp
  @param fieldname name of the field
  @param isfloat Set this value to true if you want a float field
  @return true if field added, false otherwise
  @author Lucien Schreiber (c) CREALP 2008
  @date 15 November 2008
  *******************************************************************************/
bool tmGISDataVectorSHP::AddFieldNumeric(const wxString &fieldname, bool isfloat) {
  wxASSERT(m_Layer);
  bool bReturn = true;

  char *buffer = new char[fieldname.Length() * sizeof(wxString)];
  strcpy(buffer, (const char *)fieldname.mb_str(wxConvUTF8));
  OGRFieldType myFieldType = OFTInteger;
  if (isfloat) myFieldType = OFTReal;

  OGRFieldDefn oField(buffer, myFieldType);
  wxDELETEA(buffer);

  if (m_Layer->CreateField(&oField) != OGRERR_NONE) {
    wxASSERT_MSG(0, _T("Creating int field failed."));
    bReturn = false;
  }
  return bReturn;
}

/***************************************************************************/ /**
  @brief Add date field to the Shp
  @param fieldname name of the field
  @author Lucien Schreiber (c) CREALP 2008
  @date 15 November 2008
  *******************************************************************************/
bool tmGISDataVectorSHP::AddFieldDate(const wxString &fieldname) {
  wxASSERT(m_Layer);
  bool bReturn = true;

  char *buffer = new char[fieldname.Length() * sizeof(wxString)];
  strcpy(buffer, (const char *)fieldname.mb_str(wxConvUTF8));
  OGRFieldDefn oField(buffer, OFTDateTime);
  wxDELETEA(buffer);

  if (m_Layer->CreateField(&oField) != OGRERR_NONE) {
    wxASSERT_MSG(0, _T("Creating int field failed."));
    bReturn = false;
  }
  return bReturn;
}

/***************************************************************************/ /**
  @brief Add a geometry in the shapefile
  @param Geom a OGRGeometry
  @param oid the oid of the new geometry added
  @param layertype Not used for shapefiles
  @return  true if geometry added, false otherwise
  @author Lucien Schreiber (c) CREALP 2008
  @date 16 November 2008
  *******************************************************************************/
long tmGISDataVectorSHP::AddGeometry(OGRGeometry *Geom, const long &oid, int layertype) {
  wxASSERT(Geom);
  wxASSERT(m_Layer);
  wxASSERT(m_Feature == nullptr);

  // OGRFeature * poFeature;
  m_Feature = OGRFeature::CreateFeature(m_Layer->GetLayerDefn());
  wxASSERT(m_Feature);

  m_Feature->SetGeometry(Geom);
  if (oid != -1) {
    m_Feature->SetFID(oid);

    // add FID into special column
    m_Feature->SetField(0, (int)oid);
  }

  OGRErr myErr = m_Layer->CreateFeature(m_Feature);
  if (myErr != OGRERR_NONE) return -1;

  long lRetVal = m_Feature->GetFID();
  // OGRFeature::DestroyFeature(m_Feature);
  return lRetVal;
}

/***************************************************************************/ /**
  @brief Move to next feature
  @param resetreading should we restart iterating features ?
  @return true if next feature is a valid feature, false otherwise (end of file
  encountred).
  @author Lucien Schreiber (c) CREALP 2008
  @date 17 November 2008
  *******************************************************************************/
bool tmGISDataVectorSHP::SetNextFeature(bool resetreading) {
  wxASSERT(m_Layer);
  if (m_Feature) {
    OGRFeature::DestroyFeature(m_Feature);
    m_Feature = nullptr;
  }

  if (resetreading) m_Layer->ResetReading();

  m_Feature = m_Layer->GetNextFeature();

  if (m_Feature) return true;

  return false;
}

/***************************************************************************/ /**
  @brief Return the actual OID
  @return  the actual OID or wxNOT_FOUND if not found
  @author Lucien Schreiber (c) CREALP 2009
  @date 26 March 2009
  *******************************************************************************/
long tmGISDataVectorSHP::GetActualOID() {
  wxASSERT(m_Layer);
  wxASSERT(m_Feature->GetFieldCount() >= 1);

  if (m_Feature)
    return m_Feature->GetFieldAsInteger(0);
  else
    return wxNOT_FOUND;
}

/***************************************************************************/ /**
  @brief Set the OID for the current Feature
  @details See also SetNextFeature() for more informations about changing actual
  feature
  @param oid OID to set. (converted into int for shapefiles)
  @author Lucien Schreiber (c) CREALP 2009
  @date 27 March 2009
  *******************************************************************************/
void tmGISDataVectorSHP::SetActualOID(long oid) {
  wxASSERT(m_Layer);
  wxASSERT(m_Feature->GetFieldCount() >= 1);

  if (m_Feature) m_Feature->SetField(0, (int)oid);
}

/***************************************************************************/ /**
  @brief Add value into a field
  @param value a wxString containing the value (will be converted in the function)
  @param fieldtype the type of the field based on : #PRJDEF_FIELD_TYPE
  @param iindex 0 based field index
  @return true if value was added to the field
  @author Lucien Schreiber (c) CREALP 2008
  @date 17 November 2008
  *******************************************************************************/
bool tmGISDataVectorSHP::SetFieldValue(const wxString &value, int fieldtype, int iindex) {
  wxASSERT(m_Feature);

  // check that the field exists
  if (iindex > m_Feature->GetFieldCount() - 1) {
    wxASSERT_MSG(0, _T("Trying to set value to a field that doesn't exists"));
    return false;
  }

  // Value is empty, don't try to pass it to the fields
  // let the default null value.
  if (value.Len() == 0) {
    return true;
  }
  wxASSERT(value.Len() > 0);

  wxStringTokenizer myTok;
  int myYear = 0, myMonth = 0, myDay = 0;

  switch (fieldtype) {
    case TM_FIELD_ENUMERATION:  // enumeration
    case TM_FIELD_TEXT:         // TEXT
    {
      wxString myValue = value;
      m_Feature->SetField(iindex, (const char *)myValue.mb_str(wxCSConv(wxFONTENCODING_ISO8859_1)));
    } break;

    case TM_FIELD_INTEGER:  // INTEGER
      m_Feature->SetField(iindex, wxAtoi(value));
      break;

    case TM_FIELD_FLOAT:  // FLOAT
      m_Feature->SetField(iindex, wxAtof(value));
      break;

    case TM_FIELD_DATE:  // DATE
      myTok.SetString(value, _T("-"), wxTOKEN_DEFAULT);
      wxASSERT(myTok.CountTokens() == 3);

      myYear = wxAtoi(myTok.GetNextToken());
      myMonth = wxAtoi(myTok.GetNextToken());
      wxASSERT(myMonth >= 1 && myMonth <= 12);
      myDay = wxAtoi(myTok.GetNextToken());
      wxASSERT(myDay >= 1 && myDay <= 31);
      wxLogDebug(_T("Date computed is : %d / %d / %d"), myYear, myMonth, myDay);
      m_Feature->SetField(iindex, myYear, myMonth, myDay, 0, 0, 0, 1);
      break;

    default:
      wxLogError(_("Field type not supported now, sorrry...."));
      return false;
      break;
  }

  return true;
}

/***************************************************************************/ /**
  @brief Update the actual feature
  @details This function should be used after applying modification to the
  feature for saving the modification in the file. For exemple after setting
  fields value with tmGISDataVectorSHP::SetFieldValue() use this function for
  apply changes
  @return  true if saving features works
  @author Lucien Schreiber (c) CREALP 2008
  @date 17 November 2008
  *******************************************************************************/
bool tmGISDataVectorSHP::UpdateFeature() {
  wxASSERT(m_Feature);
  wxASSERT(m_Layer);

  if (m_Layer->SetFeature(m_Feature) != OGRERR_NONE) return false;

  return true;
}

void tmGISDataVectorSHP::CloseGeometry() {
  wxASSERT(m_Feature);
  wxASSERT(m_Layer);

  UpdateFeature();

  OGRFeature::DestroyFeature(m_Feature);
  m_Feature = nullptr;
}

bool tmGISDataVectorSHP::GetFieldNumeric(const wxString &fieldname, int &fieldvalue) {
  wxASSERT(m_Feature);
  wxASSERT(m_Layer);
  int myFieldIndex = m_Layer->GetLayerDefn()->GetFieldIndex((const char *)fieldname.mb_str(wxConvUTF8));
  wxASSERT(myFieldIndex != -1);

  if (m_Feature == nullptr || !m_Feature->IsFieldSet(myFieldIndex)) {
    return false;
  }
  fieldvalue = m_Feature->GetFieldAsInteger(myFieldIndex);
  return true;
}

bool tmGISDataVectorSHP::SetFieldNumeric(const wxString &fieldname, int fieldvalue) {
  wxASSERT(m_Feature);
  wxASSERT(m_Layer);

  int myFieldIndex = m_Layer->GetLayerDefn()->GetFieldIndex((const char *)fieldname.mb_str(wxConvUTF8));
  wxASSERT(myFieldIndex != -1);
  if (myFieldIndex == -1) {
    return false;
  }

  m_Feature->SetField(myFieldIndex, fieldvalue);
  return true;
}

/***************************************************************************/ /**
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
bool tmGISDataVectorSHP::GetSnapCoord(const wxRealPoint &clickpt, double buffersize, wxArrayRealPoints &snapppts,
                                      int snaptype) {
  // create OGRpoint and buffer
  OGRPoint myClickPoint;
  myClickPoint.setX(clickpt.x);
  myClickPoint.setY(clickpt.y);

  OGRGeometry *myBufferClick = myClickPoint.Buffer(buffersize);  // tmGISDataVector::SafeBuffer(&myClickPoint, iBuffer);
  wxASSERT(myBufferClick);
  wxASSERT(m_Layer);

  // searching all features
  m_Layer->ResetReading();
  OGRGeometry *poGeometry;
  OGRFeature *poFeature;
  unsigned int myPtsCount = snapppts.GetCount();
  while ((poFeature = m_Layer->GetNextFeature()) != nullptr) {
    poGeometry = poFeature->GetGeometryRef();
    if (poGeometry) {
      if (poGeometry->Intersects(myBufferClick)) {
        if ((snaptype & tmSNAPPING_VERTEX) == tmSNAPPING_VERTEX) {
          GetVertexIntersection(poGeometry, myBufferClick, snapppts);
        } else if (snaptype == tmSNAPPING_BEGIN_END) {
          GetBeginEndInterseciton(poGeometry, myBufferClick, snapppts);
        }
      }
      OGRFeature::DestroyFeature(poFeature);
    }
  }
  OGRGeometryFactory::destroyGeometry(myBufferClick);

  if (myPtsCount < snapppts.GetCount()) {
    return true;
  }
  return false;
}

/*
bool tmGISDataVectorSHP::CreateSpatialIndex(int indexdepth){
    wxASSERT(m_Datasource);
    wxFileName myLayerName(m_Datasource->GetName());
    wxString myStmt = wxString::Format(_T("CREATE SPATIAL INDEX ON %s "), myLayerName.GetName());
    if (indexdepth != wxNOT_FOUND) {
        myStmt << _T("DEPTH ");
        myStmt << indexdepth;
    }
    OGRLayer *poResultSet = nullptr;
    poResultSet = m_Datasource->ExecuteSQL((const char*) myStmt.mb_str(wxConvUTF8), NULL, "generic");
    if (poResultSet != nullptr) {
        //wxLogMessage(_("Releasing result set"));
        m_Datasource->ReleaseResultSet( poResultSet );
    }
    //wxString myError (CPLGetLastErrorMsg());
    //wxLogMessage(_("Error: %s"), myError);
    //wxLogMessage(myStmt);
    return true;
}*/

bool tmGISDataVectorSHP::ExecuteSQLQuery(const wxString &query) {
  OGRLayer *poResultSet = nullptr;
  const char *dialect = "";
  poResultSet = m_Datasource->ExecuteSQL((const char *)query.mb_str(wxConvUTF8), NULL, dialect);
  if (poResultSet != nullptr) {
    m_Datasource->ReleaseResultSet(poResultSet);
    return false;
  }
  return true;
}

long tmGISDataVectorSHP::GetFeatureIDIntersectedBy(OGRGeometry *geometry) {
  wxASSERT(m_Layer);
  m_Layer->ResetReading();
  m_Layer->SetSpatialFilter(geometry);
  OGRFeature *myFeature = m_Layer->GetNextFeature();
  m_Layer->SetSpatialFilter(nullptr);
  if (myFeature == nullptr) {
    return wxNOT_FOUND;
  }

  long myID = myFeature->GetFID();
  OGRFeature::DestroyFeature(myFeature);
  return myID;
}

long tmGISDataVectorSHP::GetFeatureIDIntersectedOnRaster(OGRPoint *geometry) {
  if (m_RasterizeDataset == nullptr) {
    wxLogError(_("Unable to get feature on raster, raster not open!"));
    return wxNOT_FOUND;
  }

  // convert real coord to pixel coord
  double myGeoTransform[6];
  m_RasterizeDataset->GetGeoTransform(&myGeoTransform[0]);
  double myLeft = myGeoTransform[0];
  double myTop = myGeoTransform[3];
  double myPxWidth = myGeoTransform[1];
  double myPxHeight = myGeoTransform[5];
  int myWidth = m_RasterizeDataset->GetRasterXSize();
  int myHeight = m_RasterizeDataset->GetRasterYSize();

  double myCoordx = ((geometry->getX() - myLeft) / myPxWidth);  // - (0.5 * myPxWidth);
  double myCoordy = ((geometry->getY() - myTop) / myPxHeight);  // + (0.5 * myPxHeight);
  int pxcoordx = wxRound(myCoordx);
  int pxcoordy = wxRound(fabs(myCoordy));

  // create a rectangle centred on pxcoordx, pxcoordy
  wxRect myPtRect(pxcoordx - 1, pxcoordy - 1, 3, 3);
  wxRect myImgRect(0, 0, myWidth, myHeight);
  wxRect myIntersectRect = myImgRect.Intersect(myPtRect);

  void *pData =
      CPLMalloc((GDALGetDataTypeSize(GDT_Float32) / 8) * myIntersectRect.GetWidth() * myIntersectRect.GetHeight());
  if (m_RasterizeDataset->RasterIO(GF_Read, myIntersectRect.GetLeft(), myIntersectRect.GetTop(),
                                   myIntersectRect.GetWidth(), myIntersectRect.GetHeight(), pData,
                                   myIntersectRect.GetWidth(), myIntersectRect.GetHeight(), GDT_Float32, 1, NULL, 0, 0,
                                   0) != CE_None) {
    wxLogError("Error reading value at pixel (%d, %d) from rasterized file", pxcoordx, pxcoordy);
    if (pData != nullptr) {
      CPLFree(pData);
      pData = nullptr;
    }
    return wxNOT_FOUND;
  }

  long myValue = (long)((float *)pData)[5];
  for (int i = 0; i < myIntersectRect.GetWidth() * myIntersectRect.GetHeight(); i++) {
    long myRectValue = (long)((float *)pData)[i];
    if (myRectValue != myValue) {
      CPLFree(pData);
      return wxNOT_FOUND;
    }
  }
  CPLFree(pData);
  return myValue;

  // temp code to export label into image
  /*
  float * pData = new float[myIntersectRect.GetWidth() * myIntersectRect.GetHeight()];
  for (int i = 0; i< myIntersectRect.GetWidth() * myIntersectRect.GetHeight(); i++) {
      *(pData+i) = 255;
  }
  if (m_RasterizeDataset->RasterIO(GF_Write, myIntersectRect.GetLeft(), myIntersectRect.GetTop(),
myIntersectRect.GetWidth(), myIntersectRect.GetHeight(),pData, myIntersectRect.GetWidth(),
myIntersectRect.GetHeight(),GDT_Float32,1, NULL, 0,0,0) != CE_None){ wxLogError("Error reading value at pixel (%d, %d)
from rasterized file",pxcoordx, pxcoordy); if (pData != NULL) { wxDELETEA(pData);
}
return wxNOT_FOUND;
}
  wxDELETEA(pData);
  return wxNOT_FOUND;
  */
}

bool tmGISDataVectorSHP::Rasterize(double rasterizefactor) {
  if (rasterizefactor == 0) {
    return true;
  }

  // get filename
  wxFileName myRasterName(GetFullFileName());
  myRasterName.SetExt(_T("tif"));
  wxString myRasterNameTxt = myRasterName.GetFullPath();

  // get driver
  GDALDriverH hDriver = GDALGetDriverByName("GTIFF");
  if (hDriver == nullptr) {
    wxLogError(_("Driver 'GTIFF' not found"));
    return false;
  }

  // get shp dimensions
  OGREnvelope myExtent;
  OGRErr myErr = m_Layer->GetExtent(0, &myExtent);
  wxSize mySize(wxRound(fabs(myExtent.MaxX - myExtent.MinX)), wxRound(fabs(myExtent.MaxY - myExtent.MinY)));
  mySize.SetWidth(wxRound(mySize.GetWidth() / rasterizefactor));
  mySize.SetHeight(wxRound(mySize.GetHeight() / rasterizefactor));

  // create raster
  GDALDatasetH hDstDS = nullptr;
  hDstDS = GDALCreate(hDriver, (const char *)myRasterNameTxt.mb_str(wxConvUTF8), mySize.GetWidth(), mySize.GetHeight(),
                      1, GDT_Float32, NULL);
  if (hDstDS == nullptr) {
    return false;
  }

  // set projection
  double adfProjection[6];
  adfProjection[0] = myExtent.MinX;
  adfProjection[1] = rasterizefactor;
  adfProjection[2] = 0;
  adfProjection[3] = myExtent.MaxY;
  adfProjection[4] = 0;
  adfProjection[5] = -1.0 * rasterizefactor;
  GDALSetGeoTransform(hDstDS, adfProjection);

  // read shp features
  std::vector<OGRGeometryH> ahGeometries;
  std::vector<double> adfFullBurnValues;
  m_Layer->ResetReading();
  while ((m_Feature = m_Layer->GetNextFeature()) != nullptr) {
    OGRGeometryH myGeom = m_Feature->GetGeometryRef()->clone();
    ahGeometries.push_back(myGeom);
    adfFullBurnValues.push_back(m_Feature->GetFID());
    OGRFeature::DestroyFeature(m_Feature);
  }

  std::vector<int> anBandList;
  anBandList.push_back(1);
  GDALRasterizeGeometries(hDstDS, anBandList.size(), &(anBandList[0]), ahGeometries.size(), &(ahGeometries[0]), NULL,
                          NULL, &(adfFullBurnValues[0]), NULL, NULL, NULL);

  // clean
  for (int iGeom = ahGeometries.size() - 1; iGeom >= 0; iGeom--) {
    OGR_G_DestroyGeometry(ahGeometries[iGeom]);
  }
  GDALClose(hDstDS);

  m_RasterizeDataset = (GDALDataset *)GDALOpen((const char *)myRasterNameTxt.mb_str(wxConvUTF8), GA_Update);
  if (m_RasterizeDataset == nullptr) {
    return false;
  }
  return true;
}

void tmGISDataVectorSHP::RemoveRasterizeFile() {
  if (m_RasterizeDataset == nullptr) {
    return;
  }
  GDALClose(m_RasterizeDataset);

  GDALDriver *poDriver = GetGDALDriverManager()->GetDriverByName("GTiff");
  wxASSERT(poDriver);
  if (poDriver == nullptr) {
    return;
  }

  wxFileName myRasterName(GetFullFileName());
  myRasterName.SetExt(_T("tif"));
  wxString myRasterNameTxt = myRasterName.GetFullPath();
  poDriver->Delete((const char *)myRasterNameTxt.mb_str(wxConvUTF8));
  m_RasterizeDataset = nullptr;
}

bool tmGISDataVectorSHP::CopyToFile(const wxFileName &filename, const wxString &drivername, bool overwrite) {
  if (!overwrite && filename.Exists()) {
    wxLogError(_("File '%s' exists copy failed!"), filename.GetFullPath());
    return false;
  }

  wxASSERT(m_Datasource);
  wxASSERT(m_Layer);

  // create file
  GDALDriver *poDriver = GetGDALDriverManager()->GetDriverByName((const char *)drivername.mb_str(wxConvUTF8));
  if (poDriver == nullptr) {
    wxASSERT_MSG(0, _T(" driver not available."));
    return false;
  }

  // creating the file
  wxString mysFileName = filename.GetFullPath();
  GDALDataset *myNewDS = poDriver->Create((const char *)mysFileName.mb_str(wxConvUTF8),
                                          0,  // x size, not used for vector
                                          0,  // y size, not used for vector
                                          0,  // nb of bands
                                          GDT_Unknown, nullptr);
  if (myNewDS == nullptr) {
    wxLogError(_("Creation of file '%s' failed. Is it open in another software?"), filename.GetFullName().c_str());
    return false;
  }

  // copy fields definition and features
  wxString myFileNameWOExt = filename.GetName();
  myNewDS->CopyLayer(m_Layer, (const char *)myFileNameWOExt.mb_str(wxConvUTF8));

  // close new file
  GDALClose(myNewDS);
  return true;
}

bool tmGISDataVectorSHP::CreateSpatialIndex(GDALProgressFunc progress, void *pfProgressData) {
  wxString myLayerName(m_Layer->GetLayerDefn()->GetName());
  wxString myQuery = wxString::Format(_T("CREATE SPATIAL INDEX ON %s"), myLayerName);
  return ExecuteSQLQuery(myQuery);
}

bool tmGISDataVectorSHP::DeleteFile(const wxString &layername) {
  const char *pszDriverName = "ESRI Shapefile";
  GDALDriver *poDriver;

  poDriver = GetGDALDriverManager()->GetDriverByName(pszDriverName);
  if (poDriver == nullptr) {
    wxASSERT_MSG(0, _T(" driver not available."));
    return false;
  }

  // deleting the file
  CPLErr myErr = poDriver->Delete((const char *)layername.mb_str(wxConvUTF8));
  if (myErr != CE_None) {
    wxLogError(_("Unable to delete :%s"), layername);
    return false;
  }
  return true;
}

tmGISDataVectorSHPMemory::tmGISDataVectorSHPMemory() {}

tmGISDataVectorSHPMemory::~tmGISDataVectorSHPMemory() {}

bool tmGISDataVectorSHPMemory::CreateFile(const wxFileName &filename, int type) {
  GDALDriver *poDriver = GetGDALDriverManager()->GetDriverByName("Memory");
  if (poDriver == nullptr) {
    wxASSERT_MSG(0, _T(" driver not available."));
    return false;
  }

  // creating the file
  wxString mysFileName = filename.GetFullPath();
  m_Datasource = poDriver->Create((const char *)mysFileName.mb_str(wxConvUTF8), 0, 0, 0, GDT_Unknown, NULL);
  if (m_Datasource == nullptr) {
    wxLogError(_("Creation of in memory file '%s' failed."), filename.GetFullName().c_str());
    return false;
  }

  OGRwkbGeometryType myGeomType = wkbUnknown;
  switch (type) {
    case 0:  // LINE
      myGeomType = wkbLineString;
      break;

    case 1:  // POINT
      myGeomType = wkbPoint;
      break;

    case 2:  // POLYGON
      myGeomType = wkbPolygon;
      break;

    default:
      myGeomType = wkbUnknown;
      break;
  }

  if (myGeomType == wkbUnknown) return false;

  // creating the layer
  wxString myFileNameWOExt = filename.GetName();
  m_Layer = m_Datasource->CreateLayer((const char *)myFileNameWOExt.mb_str(wxConvUTF8), NULL, myGeomType, NULL);
  if (m_Layer == nullptr) {
    wxASSERT_MSG(0, _T("Layer creation failed."));
    return false;
  }

  tmGISData::Open(filename.GetFullPath(), true);
  return true;
}
