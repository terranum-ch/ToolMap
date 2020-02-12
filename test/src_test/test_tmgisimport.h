/***************************************************************************
 test_tmgisimport.h
 UNIT TESTING for importing gis data
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

#ifndef _TM_GIS_IMPORT_TEST_CPP_
#define _TM_GIS_IMPORT_TEST_CPP_

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <cxxtest/TestSuite.h>

#include "../../src/database/database_tm.h"
#include "../../src/gis/tmgisdatavector.h"
#include "../../src/gis/tmimportcsv.h"
#include "../../src/gis/tmimportgis.h"
#include "test_param.h"

class TEST_tmGISImport : public CxxTest::TestSuite {
 public:
  DataBaseTM *m_DB;

  TEST_tmGISImport(bool bTest) {
    wxApp::SetInstance(new wxAppConsole());
    tmGISData::InitGISDrivers(true, true);
    m_DB = new DataBaseTM();
    TS_ASSERT(m_DB->DataBaseOpen(g_TestPathPRJ, g_TestPrj_Fields));
  }

  virtual ~TEST_tmGISImport() {
    delete m_DB;
  }

  static TEST_tmGISImport *createSuite() {
    return new TEST_tmGISImport(true);
  }

  static void destroySuite(TEST_tmGISImport *suite) {
    delete suite;
  }

  void testName() {
    wxLogMessage(_T("------------------------------------"));
    wxLogMessage(_T("--------   TESTING IMPORT    -------"));
    wxLogMessage(_T("------------------------------------"));
  }

  void testOpen() {
    tmImportGIS myImport;
    TS_ASSERT(myImport.Open(wxFileName(g_TestPathSHP, g_TestFileSHP_L)));
    TS_ASSERT(!myImport.Open(wxFileName(g_TestPathSHP + _T("cantonsuisssssse.shp"))));
    TS_ASSERT(!myImport.Open(wxFileName(g_TestPathMISC, g_TestFileMisc)));
  }

  void testFileSpatialType() {
    tmImportGIS myImport;
    TS_ASSERT(myImport.Open(wxFileName(g_TestPathSHP + g_TestFileSHP_PLG)));
    TS_ASSERT_EQUALS(myImport.GetGeometryType(), LAYER_SPATIAL_POLYGON);
  }

  void testFeatureCount() {
    tmImportGIS myImport;
    TS_ASSERT(myImport.GetFeatureCount() == wxNOT_FOUND);
    TS_ASSERT(myImport.Open(wxFileName(g_TestPathSHP + g_TestFileSHP_PLG)));
    TS_ASSERT_EQUALS(myImport.GetFeatureCount(), 26);
  }

  void testImportInto() {
    tmImportGIS myImport;
    TS_ASSERT(myImport.Open(wxFileName(g_TestPathSHP + g_TestFileSHP_PLG)));
    TS_ASSERT(!myImport.Import(m_DB, NULL, NULL));
    myImport.SetTarget(TOC_NAME_LINES);
    TS_ASSERT(myImport.Import(m_DB, NULL, NULL));
  }

  // importing CSV
  void testOpenCSV() {
    tmImportCSV myImport;
    TS_ASSERT(myImport.Open(wxFileName(g_TestPathMISC, g_TestFileCSV)));
  }

  void testGettingColumns() {
    tmImportCSV myImport;
    TS_ASSERT(myImport.Open(wxFileName(g_TestPathMISC, g_TestFileCSV)));
    wxArrayString myCols = myImport.GetFieldsList();
    wxASSERT(myCols.GetCount() > 0);
    for (unsigned int i = 0; i < myCols.GetCount(); i++) {
      wxLogMessage(_T("Column %d - '%s'"), i, myCols.Item(i).c_str());
    }
  }

  void testImportCSV() {
    tmImportCSV myImport;
    TS_ASSERT(myImport.Open(wxFileName(g_TestPathMISC, g_TestFileCSV)));
    myImport.SetXYColumn(0, 2);
    myImport.SetTarget(TOC_NAME_POINTS);
    TS_ASSERT(myImport.Import(m_DB, NULL, NULL));
  }
};

#endif
