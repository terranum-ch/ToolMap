/***************************************************************************
 tmgisdatavectormysql.h
                    For dealing with spatial data from a mysql database
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


#ifndef _TM_GISDATAVECTOR_MYSQL_H_
#define _TM_GISDATAVECTOR_MYSQL_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

// Include wxWidgets' headers
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif


#include "tmgisdatavector.h"
#include "../database/database_tm.h"


class tmAttributionData;

const wxString tmGISMYSQL_FIELD1 = _T("OBJECT_ID");
const wxString tmGISMYSQL_FIELD2 = _T("OBJECT_GEOMETRY");

// same order as in TM_GIS_SPATIAL_TYPES
const wxString tmGISMYSQL_TEXT_TYPES[] = {_T("linestring"), _T("multipoint"), _T("polygon")};


class tmGISDataVectorMYSQL : public tmGISDataVector
{
private:
    static DataBaseTM *m_DB;
    //static PrjDefMemManage * m_ProjDef;

    tmAttributionData *_CreateAttributionObject(int &layertype);

    PrjDefMemManage *m_PrjDef;

protected:
    // checking db fields
    bool CheckGeometryFields(const wxString &tablename);

    // gis database function
    OGRGeometry *CreateDataBaseGeometry(MYSQL_ROW &row,
                                        const tmArrayULong &row_lengths,
                                        int geometry_col = 0);

    long GetOid(MYSQL_ROW &row, const int &col);

    virtual wxString GetTableName(TOC_GENERIC_NAME type);


public:
    tmGISDataVectorMYSQL();

    ~tmGISDataVectorMYSQL();

    // special function for DB, use it first.
    static void SetDataBaseHandle(DataBaseTM *db)
    { m_DB = db; }

    static DataBaseTM *GetDataBaseHandle()
    { return m_DB; }

    void SetProject(PrjDefMemManage *prj)
    { m_PrjDef = prj; }


    // implementing virtual function
    virtual bool Open(const wxString &filename, bool bReadWrite = FALSE);

    virtual tmRealRect GetMinimalBoundingRectangle();

    virtual TM_GIS_SPATIAL_TYPES GetSpatialType();

    // virtual function for getting data & drawing
    virtual bool SetSpatialFilter(tmRealRect filter, int type);

    virtual wxRealPoint *GetNextDataLine(int &nbvertex, long &oid, bool &isOver);

    virtual wxRealPoint *GetNextDataPoint(long &oid, bool &isOver);

    OGRPoint *GetNextDataPointWithAttrib(long &oid, wxArrayString &values);

    virtual OGRLineString *GetNextDataLine(long &oid);

    virtual OGRPoint *GetOGRNextDataPoint(long &oid);

    virtual OGRFeature *GetFeatureByOID(long oid);

    virtual long AddGeometry(OGRGeometry *Geom, const long &oid, int layertype = wxNOT_FOUND);

    virtual bool UpdateGeometry(OGRGeometry *geom, const long &oid);

    virtual OGRGeometryCollection *GetGeometryColByOID(wxArrayLong *OIDs);

    virtual OGRGeometry *GetNextGeometry(bool restart, long &oid);

    // metadata
    virtual wxString GetMetaDataAsHtml();

    virtual wxString GetDataSizeAsHtml(int iPrecision = 2);

    // fields functions
    //static void SetProjectDefinition(PrjDefMemManage * project){m_ProjDef = project;}
    virtual int GetFieldsCount();

    virtual bool GetFieldsName(wxArrayString &Fields, long oid = wxNOT_FOUND);

    virtual bool GetFieldsValue(wxArrayString &values, long oid);


    // count
    virtual int GetCount();

    // searching data
    virtual wxArrayLong *SearchData(const tmRealRect &rect, int type);

    virtual wxArrayLong *GetAllData();

    virtual wxArrayLong *SearchIntersectingGeometry(OGRGeometry *intersectinggeom);

    virtual bool GetSnapCoord(const wxRealPoint &clickpt, double buffersize,
                              wxArrayRealPoints &snapppts, int snaptype);

    virtual bool IsPointSnapped(const wxRealPoint &point, int snaptype, long excludeoid = wxNOT_FOUND);


};


#endif
