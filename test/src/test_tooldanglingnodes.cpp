/***************************************************************************
 test_tooldanglingnodes.h
 Test the dangling nodes tool
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
#include "../../src/gis/tooldanglingnodes.h"


class TestToolDanglingNodes : public ::testing::Test {
 protected:
  ToolDanglingNodes* m_DN = nullptr;
  wxString m_DBPathName;
  DataBaseTM* m_DB = nullptr;

  virtual void SetUp() {
    m_DBPathName = g_TestPathPRJ + g_TestPrj_Dangling;
    m_DB = new DataBaseTM();
    m_DB->OpenTMDatabase(m_DBPathName);
    m_DN = new ToolDanglingNodes(m_DB);
  }
  virtual void TearDown() {
    wxDELETE(m_DN);
    wxDELETE(m_DB);
  }
};

TEST_F(TestToolDanglingNodes, GetDanglingNodes1) {
  wxArrayRealPoints myPts;
  EXPECT_FALSE(m_DN->GetDanglingNodes(myPts));
}

TEST_F(TestToolDanglingNodes, Create1) {
  ToolDanglingNodes myTool;
  EXPECT_FALSE(myTool.IsOk());
  myTool.Create(m_DB);
  EXPECT_TRUE(myTool.IsOk());
}

TEST_F(TestToolDanglingNodes, Create2) {
  ToolDanglingNodes myTool2(m_DB);
  EXPECT_TRUE(myTool2.IsOk());
}

TEST_F(TestToolDanglingNodes, Search1) {
  EXPECT_FALSE(m_DN->SearchInit(2, "no layer name"));
  EXPECT_TRUE(m_DN->SearchInit(1, "no layer name"));
  EXPECT_FALSE(m_DN->SearchInit(100, "no layer name"));
}

TEST_F(TestToolDanglingNodes, Search2) {
  int iFeatureSearched = 0;
  EXPECT_FALSE(m_DN->SearchInfo(iFeatureSearched));
  EXPECT_TRUE(m_DN->SearchInit(1, "no layer name"));

  EXPECT_TRUE(m_DN->SearchInfo(iFeatureSearched));
  EXPECT_TRUE(iFeatureSearched > 0);
  wxLogDebug(_T("Number of feature to process : %d"), iFeatureSearched);
}

TEST_F(TestToolDanglingNodes, SearchRun1) {
  EXPECT_FALSE(m_DN->SearchRun());
}

TEST_F(TestToolDanglingNodes, SearchRun2) {
  int iNum = 0;
  EXPECT_TRUE(m_DN->SearchInit(1, "no layer name"));
  EXPECT_TRUE(m_DN->SearchRun());
  EXPECT_FALSE(m_DN->SearchRun());
  EXPECT_FALSE(m_DN->SearchInfo(iNum));
}

TEST_F(TestToolDanglingNodes, SearchRun3) {
  // int iNum = 0;
  EXPECT_TRUE(m_DN->SearchInit(1, "no layer name"));
  EXPECT_TRUE(m_DN->SearchRun());
  wxArrayRealPoints myPts;
  bool bGet = m_DN->GetDanglingNodes(myPts);
  EXPECT_TRUE(bGet);
  if (bGet) {
    wxLogDebug(_T("%zu dangling nodes "), myPts.GetCount());
  }

  for (unsigned int i = 0; i < myPts.GetCount(); i++)
    wxLogDebug(_T("--- dangling node : %.*f, %.*f"), 2, myPts.Item(i).x, 2, myPts.Item(i).y);
}