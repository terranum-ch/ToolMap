/***************************************************************************
 test_export.h
 UNIT TESTING for exporting data
 -------------------
 copyright : (C) 2009 CREALP Lucien Schreiber
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef _TM_TEST_EXPORT_DATA_CONCAT_H_
#define _TM_TEST_EXPORT_DATA_CONCAT_H_

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <cxxtest/TestSuite.h>

#include "../../src/core/tmexportmanager.h"
#include "test_param.h"

class TEST_tmExportConcat : public CxxTest::TestSuite {
 public:
  DataBaseTM *m_pDB;
  PrjDefMemManage *m_PrjDef;
  wxString *m_RealExportPath;
  tmGISScale *m_ExportScale;

  TEST_tmExportConcat(bool bTest) {
    wxApp::SetInstance(new wxAppConsole());
    OGRRegisterAll();
    m_pDB = new DataBaseTM();
    TS_ASSERT(m_pDB->OpenTMDatabase(g_TestPathPRJ + g_TestExportConcat));
    // load project Data
    m_PrjDef = m_pDB->GetProjectDataFromDB();
    TS_ASSERT(m_PrjDef != nullptr);
    TS_ASSERT_DIFFERS(m_PrjDef->GetCountLayers(), 0);
    int iExportType = 0;
    wxString myTempString = wxEmptyString;
    TS_ASSERT_DIFFERS(m_pDB->GetProjectExportData(iExportType, myTempString), PATH_DATABASE_ERROR);
    m_RealExportPath = new wxString(myTempString);

    TS_ASSERT(m_pDB->SetProjectExportData(EXPORT_SHAPEFILE, g_TestPathEXPORT));

    // init scale for export. The project we export are in meters
    // thus we can create this "fake" scale.
    m_ExportScale = new tmGISScale();
    m_ExportScale->SetExtentWndReal(tmRealRect(0, 0, 1, 1));
    m_ExportScale->SetWidthDistanceInM(1);
  }

  virtual ~TEST_tmExportConcat() {
    // reset path to old value
    TS_ASSERT(m_pDB->SetProjectExportData(EXPORT_SHAPEFILE, *m_RealExportPath));

    wxDELETE(m_pDB);
    wxDELETE(m_PrjDef);
    wxDELETE(m_RealExportPath);
    wxDELETE(m_ExportScale);
  }

  static TEST_tmExportConcat *createSuite() {
    return new TEST_tmExportConcat(true);
  }
  static void destroySuite(TEST_tmExportConcat *suite) {
    delete suite;
  }

  void setUp() {}

  void tearDown() {}

  void testName() {
    wxLogMessage(_T("------------------------------------"));
    wxLogMessage(_T("-- TESTING EXPORT CONCAT DATA ------"));
    wxLogMessage(_T("------------------------------------"));
  }

  void testExportLine() {
    tmExportManager myManager(nullptr, m_pDB, m_ExportScale);
    TS_ASSERT(myManager.ExportConcatenated(m_PrjDef, LAYER_LINE, false));
  }

  void testExportPoint() {
    tmExportManager myManager(nullptr, m_pDB, m_ExportScale);
    TS_ASSERT(myManager.ExportConcatenated(m_PrjDef, LAYER_POINT, false));
  }

  void testExportLabels() {
    tmExportManager myManager(NULL, m_pDB, m_ExportScale);
    TS_ASSERT(myManager.ExportConcatenated(m_PrjDef, LAYER_POLYGON, false));
  }
};

#endif
