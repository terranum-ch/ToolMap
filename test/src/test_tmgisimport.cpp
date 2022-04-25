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

#include "gtest/gtest.h"

#include "test_param.h"
#include "../../src/database/database_tm.h"
#include "../../src/gis/tmgisdatavector.h"
#include "../../src/gis/tmimportcsv.h"
#include "../../src/gis/tmimportgis.h"


class TestGISImport : public ::testing::Test {
 protected:
  DataBaseTM* m_db = nullptr;

  virtual void SetUp() {
    tmGISData::InitGISDrivers(true, true);
    m_db = new DataBaseTM();
    ASSERT_TRUE(m_db->DataBaseOpen(g_TestPathPRJ, g_TestPrj_Fields));
  }
  virtual void TearDown() {
    wxDELETE(m_db);
  }
};

TEST_F(TestGISImport, Open) {
  tmImportGIS myImport;
  EXPECT_TRUE(myImport.Open(wxFileName(g_TestPathSHP, g_TestFileSHP_L)));
  EXPECT_FALSE(myImport.Open(wxFileName(g_TestPathSHP + _T("cantonsuisssssse.shp"))));
  EXPECT_FALSE(myImport.Open(wxFileName(g_TestPathMISC, g_TestFileMisc)));
}

TEST_F(TestGISImport, FileSpatialType) {
  tmImportGIS myImport;
  EXPECT_TRUE(myImport.Open(wxFileName(g_TestPathSHP + g_TestFileSHP_PLG)));
  EXPECT_EQ(myImport.GetGeometryType(), LAYER_SPATIAL_POLYGON);
}

TEST_F(TestGISImport, FeatureCount) {
  tmImportGIS myImport;
  EXPECT_TRUE(myImport.GetFeatureCount() == wxNOT_FOUND);
  EXPECT_TRUE(myImport.Open(wxFileName(g_TestPathSHP + g_TestFileSHP_PLG)));
  EXPECT_EQ(myImport.GetFeatureCount(), 26);
}

TEST_F(TestGISImport, ImportInto) {
  tmImportGIS myImport;
  EXPECT_TRUE(myImport.Open(wxFileName(g_TestPathSHP + g_TestFileSHP_PLG)));
  EXPECT_FALSE(myImport.Import(m_db, nullptr, nullptr));
  myImport.SetTarget(TOC_NAME_LINES);
  EXPECT_TRUE(myImport.Import(m_db, nullptr, nullptr));
}

// importing CSV
TEST_F(TestGISImport, OpenCSV) {
  tmImportCSV myImport;
  EXPECT_TRUE(myImport.Open(wxFileName(g_TestPathMISC, g_TestFileCSV)));
}

TEST_F(TestGISImport, GettingColumns) {
  tmImportCSV myImport;
  EXPECT_TRUE(myImport.Open(wxFileName(g_TestPathMISC, g_TestFileCSV)));
  wxArrayString myCols = myImport.GetFieldsList();
  EXPECT_TRUE(myCols.GetCount() > 0);
  for (unsigned int i = 0; i < myCols.GetCount(); i++) {
    wxLogMessage(_T("Column %d - '%s'"), i, myCols.Item(i).c_str());
  }
}

TEST_F(TestGISImport, ImportCSV) {
  tmImportCSV myImport;
  EXPECT_TRUE(myImport.Open(wxFileName(g_TestPathMISC, g_TestFileCSV)));
  myImport.SetXYColumn(0, 2);
  myImport.SetTarget(TOC_NAME_POINTS);
  EXPECT_TRUE(myImport.Import(m_db, nullptr, nullptr));
}
