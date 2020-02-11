/***************************************************************************
 tmexportdata.h
 main class for the process of exporting data
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

#ifndef _TM_EXPORTDATA_H_
#define _TM_EXPORTDATA_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

// Include wxWidgets' headers
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/progdlg.h>

#include "../database/databaseresult.h"
#include "projectdefmemory.h"  // for ProjectDefMemoryLayers definition

class DataBaseTM;

class PrjDefMemManage;

class tmPercent;

/***************************************************************************/ /**
  @brief Parent class for exporting data
  @details This class is used by the #tmExportManager to abstract the access to
  the data and to allow us to implement new export format
  @author Lucien Schreiber (c) CREALP 2008
  @date 14 November 2008
  *******************************************************************************/
class tmExportData : public wxObject {
 private:
  bool m_ExportAttributEnumerationCode;

  void InitMemberValues();

  double m_CropBufferDistance;

 protected:
  DataBaseTM *m_pDB;
  wxString m_Extension;
  bool m_Overwrite;
  bool m_EmptyLayer;

  // protected functions
  wxFileName *GetFileName(ProjectDefMemoryLayers *myLayer, const wxString &path);

  wxFileName *GetFileNamePrj(ProjectDefMemoryLayers *myLayer, const wxString &path);

  int GetSizeOfEnum(const PrjMemFieldCodedValArray &mCodedVal);

  bool GetSimpleAttribData(PRJDEF_LAYERS_TYPE layertype, long layerindex);

  bool GetSimpleAttribDataWithSpatial(PRJDEF_LAYERS_TYPE layertype, long layerindex);

  bool GetAdvancedAttribution(ProjectDefMemoryLayers *layer);

  virtual bool SetPolyExportInfo(ProjectDefMemoryLayers *layer) {
    return false;
  }

 public:
  // ctor
  tmExportData();

  tmExportData(DataBaseTM *database);

  void Create(DataBaseTM *database);

  ~tmExportData();

  // informations
  int GetSizeOfObjDesc(int layerindex);

  wxString GetFileExtension();

  void SetExportAttributEnumeration(bool useAttributCode) {
    m_ExportAttributEnumerationCode = useAttributCode;
  }

  bool DoExportAttributEnumerationCode() {
    return m_ExportAttributEnumerationCode;
  }

  // create export file
  virtual bool CreateEmptyExportFile(ProjectDefMemoryLayers *myLayer, const wxString &path) {
    return false;
  }

  virtual bool CreatePrjFile(ProjectDefMemoryLayers *myLayer, const wxString &path, PRJDEF_PROJ_TYPE proj) {
    return false;
  }

  virtual bool AddOptFields(const PrjMemFieldArray &myfields) {
    return false;
  }

  virtual bool AddGenericFields(int iSize) {
    return false;
  }

  virtual bool AddFIDField() {
    return false;
  }

  virtual void SetFrame(wxRealPoint *points, int nbvertex) {
    ;
  }

  virtual bool WriteLines(ProjectDefMemoryLayers *myLayer) {
    return false;
  }

  virtual bool WritePoints(ProjectDefMemoryLayers *myLayer) {
    return false;
  }

  virtual bool WritePolygons(ProjectDefMemoryLayers *myLayer) {
    return false;
  }

  virtual bool WriteLabels(ProjectDefMemoryLayers *myLayer) {
    return false;
  }

  virtual long WriteConcatGeometries(ProjectDefMemoryLayers *layer, wxProgressDialog *progDlg, tmPercent *percent) {
    return false;
  }

  virtual bool AddConcatAttributs(ProjectDefMemoryLayers *layer, PrjDefMemManage *projdef, long loop,
                                  wxProgressDialog *progDlg, tmPercent *percent) {
    return false;
  }

  virtual bool SetAttributsBasic(DataBaseResult &results) {
    return false;
  }

  virtual bool SetAttributsAdvanced(DataBaseResult &results, ProjectDefMemoryLayers *layer) {
    return false;
  }

  virtual bool GetPolyExportInfo(ProjectDefMemoryLayers *layer, bool usefastexport) {
    return false;
  }

  virtual bool CreateSpatialIndex(ProjectDefMemoryLayers *layer) {
    return false;
  }

  virtual bool HasFeatures() {
    return false;
  }

  virtual void SetEmptyLayer();

  virtual bool DeleteLayer(ProjectDefMemoryLayers *layer, const wxString &path) {
    return false;
  }

  void SetCropBufferDistance(double value) {
    m_CropBufferDistance = value;
  }

  double GetCropBufferDistance() {
    return m_CropBufferDistance;
  }

  void SetOverwrite(bool value) {
    m_Overwrite = value;
  }

  bool DoOverwrite() {
    return m_Overwrite;
  }
};

#endif
