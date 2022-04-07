/***************************************************************************
 test_database_tm.h
 Testing Database TM class
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


class TestDatabaseTM : public ::testing::Test {
 protected:
  DataBaseTM *m_db = nullptr;
  
  virtual void SetUp() {
    m_db = new DataBaseTM();
  }
  virtual void TearDown() {
    wxDELETE(m_db);
  }
};

TEST_F(TestDatabaseTM, TableExist) {
  EXPECT_TRUE(m_db->DataBaseOpen(g_TestPathPRJ, g_TestPrj_Edit));
  EXPECT_TRUE(m_db->TableExist(_T("generic_lines")));
  EXPECT_FALSE(m_db->TableExist(_T("generic_linesss")));
  EXPECT_TRUE(m_db->TableExist(_T("dmn_layer_object")));
  EXPECT_TRUE(m_db->TableExist(_T("prj_settings")));
}

TEST_F(TestDatabaseTM, ToolMapVersion) {
  EXPECT_TRUE(m_db->DataBaseOpen(g_TestPathPRJ, g_TestPrj_Edit));
  EXPECT_EQ(m_db->GetDatabaseToolMapVersion(), TM_DATABASE_VERSION);
}

TEST_F(TestDatabaseTM, CreateNewTMDatabase) {
  EXPECT_TRUE(m_db->DataBaseOpen(g_TestPathPRJ, g_TestPrj_Edit));
  // deleting if existing
  if (m_db->DataBaseOpen(g_TestPathPRJ, g_TestPrj_Edit13)) {
    wxLogDebug(wxT("Deleting database : %s"), m_db->DataBaseGetName().c_str());
    EXPECT_TRUE(m_db->DataBaseDelete());
  }

  PrjDefMemManage myPrjdef;
  myPrjdef.m_PrjName = g_TestPrj_Edit;
  myPrjdef.m_PrjPath = g_TestPathPRJ;
  myPrjdef.m_PrjProjType = PROJ_SWISS_CH1903;
  myPrjdef.m_PrjUnitType = UNIT_METERS;

  EXPECT_FALSE(m_db->CreateTMDatabase(&myPrjdef));
  myPrjdef.m_PrjName = g_TestPrj_Edit13;
  EXPECT_TRUE(m_db->CreateTMDatabase(&myPrjdef));
  EXPECT_EQ(m_db->OpenTMDatabase(myPrjdef.m_PrjPath + wxFileName::GetPathSeparator() + myPrjdef.m_PrjName),
                   tmDB_OPEN_OK);
}

TEST_F(TestDatabaseTM, OpenTMDatabase) {
  EXPECT_EQ(m_db->OpenTMDatabase(g_TestPathPRJ + g_TestPrj_Edit), tmDB_OPEN_OK);
  EXPECT_TRUE(m_db->OpenTMDatabase(g_TestPathPRJ + g_TestPrj_MyTest) >= tmDB_OPEN_FAILED_NOT_TM_DB);
  int myStatus = m_db->OpenTMDatabase(g_TestPathPRJ + g_TestPrj_LuganoTM);
  EXPECT_LE(tmDB_OPEN_FAILED_WRONG_VERSION, myStatus);
  wxLogError(_("%s is wrong version %d"), g_TestPrj_LuganoTM.c_str(), myStatus);
  EXPECT_TRUE(m_db->GetDatabaseToolMapVersion() != TM_DATABASE_VERSION);
}

TEST_F(TestDatabaseTM, ConvertPath) {
  EXPECT_TRUE(m_db->DataBaseOpen(g_TestPathPRJ, g_TestPrj_Edit));
  wxString myPath = _T("C:\\Test\\");
  wxString myUnixPath = _T("User/share/test");
  wxString myConvertedPath = _T("C:\\\\Test\\\\");
#ifdef __WINDOWS__
  EXPECT_TRUE(DataBaseTM::ConvertPath(myPath));
  EXPECT_TRUE(DataBaseTM::ConvertPath(myUnixPath));
#else
  EXPECT_FALSE(DataBaseTM::ConvertPath(myPath));
  EXPECT_FALSE(DataBaseTM::ConvertPath(myUnixPath));
#endif
}
