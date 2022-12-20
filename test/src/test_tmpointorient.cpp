/***************************************************************************
 test_tmpointorient.h
 TESTING point orientation class
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

#include "../../src/gis/tmpointorient.h"


class TestPointOrient : public ::testing::Test {
  protected:
    wxPoint myPt1, myPt90, myPt180, myPt270, myPt45, myPt235, myPtd0, myPt315;

    virtual void SetUp() {
        myPt1 = wxPoint(20, 20);
        myPt90 = wxPoint(30, 20);
        myPt180 = wxPoint(20, 30);
        myPt270 = wxPoint(10, 20);
        myPt45 = wxPoint(40, 0);
        myPt235 = wxPoint(0, 40);
        myPt315 = wxPoint(0, 0);
        myPtd0 = wxPoint(20, 0);
    }
    virtual void TearDown() {}
};

TEST_F(TestPointOrient, Valid) {
    tmPointOrient myPtO;
    EXPECT_FALSE(myPtO.IsValid());
    myPtO.SetStartPoint(myPt1);
    EXPECT_FALSE(myPtO.IsValid());
    myPtO.SetEndPoint(myPt90);
    EXPECT_TRUE(myPtO.IsValid());
    myPtO.SetEndPoint(myPt1);
    EXPECT_FALSE(myPtO.IsValid());
}

TEST_F(TestPointOrient, GetOrientInt) {
    tmPointOrient myPtO;
    EXPECT_TRUE(myPtO.GetOrientationInt() == wxNOT_FOUND);

    myPtO.SetStartPoint(myPt1);
    myPtO.SetEndPoint(myPt1);
    int myOrient = myPtO.GetOrientationInt();
    wxLogDebug(_T("orient error = %d"), myOrient);
    EXPECT_TRUE(myOrient == wxNOT_FOUND);

    myPtO.SetEndPoint(myPtd0);
    myOrient = myPtO.GetOrientationInt();
    wxLogDebug(_T("orient 0 = %d"), myOrient);

    myPtO.SetEndPoint(myPt90);
    myOrient = myPtO.GetOrientationInt();
    wxLogDebug(_T("orient 90 = %d"), myOrient);
    EXPECT_TRUE(myPtO.GetOrientationInt() == 90);

    myPtO.SetEndPoint(myPt45);
    myOrient = myPtO.GetOrientationInt();
    wxLogDebug(_T("orient 45 = %d"), myOrient);
    EXPECT_TRUE(myPtO.GetOrientationInt() == 45);

    myPtO.SetEndPoint(myPt235);
    myOrient = myPtO.GetOrientationInt();
    wxLogDebug(_T("orient 225 = %d"), myOrient);
    EXPECT_TRUE(myPtO.GetOrientationInt() == 225);

    myPtO.SetEndPoint(myPt180);
    myOrient = myPtO.GetOrientationInt();
    wxLogDebug(_T("orient 180 = %d"), myOrient);
    EXPECT_TRUE(myPtO.GetOrientationInt() == 180);

    myPtO.SetEndPoint(myPt270);
    myOrient = myPtO.GetOrientationInt();
    wxLogDebug(_T("orient 270 = %d"), myOrient);

    EXPECT_TRUE(myPtO.GetOrientationInt() == 270);

    myPtO.SetEndPoint(myPt315);
    myOrient = myPtO.GetOrientationInt();
    wxLogDebug(_T("orient 315 = %d"), myOrient);

    EXPECT_TRUE(myPtO.GetOrientationInt() == 315);
}

TEST_F(TestPointOrient, GetOrientDouble) {
    tmPointOrient myPtO;
    double dOrient = 0.0;
    dOrient = myPtO.GetOrientationDouble();
    EXPECT_EQ(dOrient, wxNOT_FOUND);
    myPtO.SetStartPoint(myPt1);
    myPtO.SetEndPoint(myPt1);
    dOrient = myPtO.GetOrientationDouble();
    EXPECT_EQ(dOrient, wxNOT_FOUND);

    myPtO.SetEndPoint(myPt90);
    dOrient = myPtO.GetOrientationDouble();
    EXPECT_TRUE(dOrient == 90.0);
}
