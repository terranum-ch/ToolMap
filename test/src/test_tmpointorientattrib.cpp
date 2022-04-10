/***************************************************************************
 test_tmpointorientattrib.h
 TESTING point orientation & attribution class
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
#include "../../src/gis/tmpointorientattrib.h"


class TestPointOrientAttrib : public ::testing::Test {
 protected:
  wxPoint myPt1, myPt90, myPt180, myPt270, myPt45, myPt235, myPtd0;
  DataBaseTM* m_DB = nullptr;

  virtual void SetUp() {
    myPt1 = wxPoint(20, 20);
    myPt90 = wxPoint(30, 20);
    myPt180 = wxPoint(20, 30);
    myPt270 = wxPoint(10, 20);
    myPt45 = wxPoint(40, 0);
    myPt235 = wxPoint(0, 40);
    myPtd0 = wxPoint(20, 0);

    m_DB = new DataBaseTM();
    m_DB->OpenTMDatabase(g_TestPathPRJ + g_TestPrj_Export);
  }
  virtual void TearDown() {
    wxDELETE(m_DB);
  }
};

TEST_F(TestPointOrientAttrib, Valid) {
  tmPointOrientAttrib myPtA;
  EXPECT_FALSE(myPtA.IsValid());
  myPtA.SetStartPoint(myPt1);
  EXPECT_FALSE(myPtA.IsValid());
  myPtA.SetEndPoint(myPt90);
  EXPECT_TRUE(myPtA.IsValid());
  myPtA.Create(m_DB, 2);
  EXPECT_FALSE(myPtA.IsCorrectType());
  myPtA.Create(m_DB, 5);
  EXPECT_TRUE(myPtA.IsCorrectType());

  // no field pt
  myPtA.Create(m_DB, 6);
  EXPECT_FALSE(myPtA.IsCorrectType());

  // no orient field pt
  myPtA.Create(m_DB, 7);
  EXPECT_FALSE(myPtA.IsCorrectType());
}

TEST_F(TestPointOrientAttrib, UpdateOrient) {
  tmPointOrientAttrib myPtA;
  myPtA.Create(m_DB, 5);
  myPtA.SetStartPoint(myPt1);
  myPtA.SetEndPoint(myPt45);
  EXPECT_FALSE(myPtA.Update());
  EXPECT_TRUE(myPtA.IsCorrectType());
  EXPECT_TRUE(myPtA.IsValid());

  EXPECT_TRUE(myPtA.Update());
}

TEST_F(TestPointOrientAttrib, UpdateOrient2) {
  tmPointOrientAttrib myPtA;
  myPtA.Create(m_DB, 1);
  myPtA.SetStartPoint(myPt1);
  myPtA.SetEndPoint(myPt235);
  EXPECT_TRUE(myPtA.IsCorrectType());
  EXPECT_TRUE(myPtA.Update());
}
