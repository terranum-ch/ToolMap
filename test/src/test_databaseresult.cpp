/***************************************************************************
 TEST_DatabaseResults.h
 Testing Database class
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
#include "../../src/database/databaseresult.h"


class TestDatabaseResults : public ::testing::Test {
 protected:
  DataBaseTM* m_db = nullptr;

  virtual void SetUp() {
    GTEST_SKIP();
    m_db = new DataBaseTM();
    ASSERT_TRUE(m_db->OpenTMDatabase(g_TestPathPRJ + g_TestPrj_AdvAttribution));
  }
  virtual void TearDown() {
    GTEST_SKIP();
    wxDELETE(m_db);
  }
};

TEST_F(TestDatabaseResults, CreatorDtor) {
  EXPECT_TRUE(m_db->DataBaseQuery(_T("SELECT * FROM zoom_level")));
  auto *myRes = new DataBaseResult();
  EXPECT_TRUE(m_db->DataBaseGetResults(myRes));
  delete myRes;
  EXPECT_FALSE(m_db->DataBaseHasResults());
}

TEST_F(TestDatabaseResults, GetCount) {
  EXPECT_TRUE(m_db->DataBaseQuery(_T("SELECT * FROM zoom_level")));
  DataBaseResult myRes;
  EXPECT_TRUE(m_db->DataBaseGetResults(&myRes));
  EXPECT_EQ(myRes.GetColCount(), 3);
  EXPECT_EQ(myRes.GetRowCount(), 4);
}

TEST_F(TestDatabaseResults, HasResults) {
  EXPECT_TRUE(m_db->DataBaseQuery(_T("SELECT * FROM zoom_level WHERE ZOOM_ID=100")));
  DataBaseResult myRes;
  EXPECT_TRUE(m_db->DataBaseGetResults(&myRes));
  EXPECT_FALSE(myRes.HasResults());

  // no result inited !
  DataBaseResult myRes2;
  EXPECT_FALSE(myRes2.HasResults());
}

TEST_F(TestDatabaseResults, GetColName) {
  EXPECT_TRUE(m_db->DataBaseQuery(_T("SELECT * FROM zoom_level")));
  DataBaseResult myRes;
  EXPECT_TRUE(m_db->DataBaseGetResults(&myRes));
  wxArrayString myColName;
  EXPECT_TRUE(myRes.GetColName(myColName));
  EXPECT_EQ(myColName.GetCount(), 3);
  for (unsigned int i = 0; i < myColName.GetCount(); i++) {
    wxLogMessage(_T("Col name : ") + myColName.Item(i));
  }
}

TEST_F(TestDatabaseResults, IsRowOk) {
  EXPECT_TRUE(m_db->DataBaseQuery(_T("SELECT * FROM zoom_level")));
  DataBaseResult myRes;
  EXPECT_TRUE(m_db->DataBaseGetResults(&myRes));
  EXPECT_FALSE(myRes.IsRowOk());
  EXPECT_TRUE(myRes.NextRow());
  EXPECT_TRUE(myRes.IsRowOk());
}

TEST_F(TestDatabaseResults, GetValue) {
  EXPECT_TRUE(m_db->DataBaseQuery(_T("SELECT * FROM zoom_level")));
  DataBaseResult myRes;
  EXPECT_TRUE(m_db->DataBaseGetResults(&myRes));
  EXPECT_TRUE(myRes.NextRow());

  wxString myValue = wxEmptyString;
  EXPECT_TRUE(myRes.GetValue(1, myValue));
  EXPECT_TRUE(myValue == _T("5000"));
  wxLogMessage(_T("Value for col 1 is ") + myValue);
  EXPECT_TRUE(myRes.NextRow());
  EXPECT_TRUE(myRes.GetValue(1, myValue));
  EXPECT_TRUE(myValue == _T("10000"));

  EXPECT_TRUE(myRes.NextRow());
  EXPECT_TRUE(myRes.NextRow());
  EXPECT_FALSE(myRes.NextRow());
}

TEST_F(TestDatabaseResults, GetValueLong) {
  EXPECT_TRUE(m_db->DataBaseQuery(_T("SELECT * FROM thematic_layers WHERE LAYER_INDEX = 1")));
  DataBaseResult myRes;
  EXPECT_TRUE(m_db->DataBaseGetResults(&myRes));
  EXPECT_TRUE(myRes.NextRow());

  long myValueLong;
  EXPECT_TRUE(myRes.GetValue(0, myValueLong));
  EXPECT_EQ(myValueLong, 1);

  wxString myValueStr = wxEmptyString;
  EXPECT_TRUE(myRes.GetValue(2, myValueStr));
  EXPECT_TRUE(myValueStr == _T("TectoBound_L"));

  // trying to get string to long
  EXPECT_FALSE(myRes.GetValue(2, myValueLong));
}

TEST_F(TestDatabaseResults, GetValueGeom) {
  EXPECT_TRUE(m_db->DataBaseQuery(_T("SELECT OBJECT_ID, AsWKB(OBJECT_GEOMETRY) FROM generic_lines WHERE OBJECT_ID = 13")));
  DataBaseResult myRes;
  EXPECT_TRUE(m_db->DataBaseGetResults(&myRes));
  EXPECT_TRUE(myRes.NextRow());

  OGRGeometry *myGeom = nullptr;
  EXPECT_TRUE(myRes.GetValue(1, &myGeom));
  EXPECT_TRUE(myGeom != nullptr);
  EXPECT_TRUE(wkbFlatten(myGeom->getGeometryType()) == wkbLineString);
  wxLogMessage(_T("Returned line has %d vertex"), ((OGRLineString *)myGeom)->getNumPoints());
  OGRGeometryFactory::destroyGeometry(myGeom);
}
