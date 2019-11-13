/***************************************************************************
								tmexportdatashp.h
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


#ifndef _TM_EXPORTDATA_SHP_H_
#define _TM_EXPORTDATA_SHP_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif


#include "tmexportdata.h"        // for parent class definition
#include "../gis/tmgisdatavectorshp.h"    // for shp


class tmExportDataSHP : public tmExportData
{
private:
    tmGISDataVectorSHP *m_Shp;
    OGRPolygon *m_Frame;

    // private function
    void InitMemberValues();


    int m_ExportPolyNbIteration;
    double m_ExportPolyRasterFactor;
    double m_ExportPolyPercentSkipped;
    bool m_ExportPolyFast;


    // for windows bug
    OGRGeometry *SafeIntersection(OGRGeometry *line, OGRGeometry *frame);

    OGRGeometry *SafeUnion(OGRGeometry *union1, OGRGeometry *line);

    OGRGeometry *SafeCreateFromGEOS(GEOSGeom geosGeom);

    OGRGeometry *SafeBuffer(OGRGeometry *ogrgeom, double size);

    // multiple fields
    bool SetMultipleFields(ProjectDefMemoryLayers *layer,
                           const wxArrayString &values);

    void _AppendValidToCollection(OGRGeometry *geometry, OGRGeometryCollection *collection);

    virtual bool SetPolyExportInfo(ProjectDefMemoryLayers *layer);


protected:
public:
    // ctor
    tmExportDataSHP();

    tmExportDataSHP(DataBaseTM *database);

    void Create(DataBaseTM *database);

    ~tmExportDataSHP();


    // create export file
    virtual bool CreateEmptyExportFile(ProjectDefMemoryLayers *myLayer, const wxString &path);

    virtual bool CreatePrjFile(ProjectDefMemoryLayers *myLayer, const wxString &path, PRJDEF_PROJ_TYPE proj);


    virtual bool AddOptFields(const PrjMemFieldArray &myfields);

    virtual bool AddGenericFields(int iSize);

    virtual bool AddFIDField();

    // frame function
    virtual void SetFrame(wxRealPoint *points, int nbvertex);


    // writing functions
    virtual bool WriteLines(ProjectDefMemoryLayers *myLayer);

    virtual bool WritePoints(ProjectDefMemoryLayers *myLayer);

    virtual bool WritePolygons(ProjectDefMemoryLayers *myLayer);

    virtual bool WriteLabels(ProjectDefMemoryLayers *myLayer);

    virtual long WriteConcatGeometries(ProjectDefMemoryLayers *layer, wxProgressDialog *progDlg, tmPercent *percent);

    virtual bool AddConcatAttributs(ProjectDefMemoryLayers *layer, PrjDefMemManage *projdef, long loop,
                                    wxProgressDialog *progDlg, tmPercent *percent);

    virtual bool SetAttributsBasic(DataBaseResult &results);

    virtual bool SetAttributsAdvanced(DataBaseResult &results, ProjectDefMemoryLayers *layer);

    virtual bool GetPolyExportInfo(ProjectDefMemoryLayers *layer, bool usefastexport);

    virtual bool CreateSpatialIndex(ProjectDefMemoryLayers *layer);

    virtual bool HasFeatures();



    virtual bool DeleteLayer(ProjectDefMemoryLayers *layer, const wxString &path);
};


#endif

