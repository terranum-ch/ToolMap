/***************************************************************************
 test_tmcoordconvert
 -------------------
 copyright : (C) 2013 CREALP Lucien Schreiber
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
#include "../../src/core/tmcoordconvert.h"


class TestCoordConvert : public ::testing::Test {
    virtual void SetUp() {
        ;
    }
    virtual void TearDown() {
        ;
    }
};

TEST_F(TestCoordConvert, SwissToWGS84) {
    tmCoordConvert myConvert(PROJ_SWISS_CH1903);
    wxRealPoint myGenfPoint(500374, 117993);
    wxRealPoint myGenfPointWGS = myConvert.GetPointWGS(myGenfPoint);
    wxLogMessage("SWISS coord is: %f, %f", myGenfPoint.x, myGenfPoint.y);
    wxLogMessage("WGS84 coord is: %f, %f", myGenfPointWGS.x, myGenfPointWGS.y);
    EXPECT_NEAR(myGenfPointWGS.x, 6.147801, 0.000001);
    EXPECT_NEAR(myGenfPointWGS.y, 46.205999, 0.000001);
}

TEST_F(TestCoordConvert, SwissToGoogle) {
    tmCoordConvert myConvert(PROJ_SWISS_CH1903);
    wxRealPoint myGenfPoint(500374, 117993);
    wxRealPoint myGenfPointG = myConvert.GetPointGoogle(myGenfPoint);
    wxLogMessage("Google coord is: %f, %f", myGenfPointG.x, myGenfPointG.y);
    EXPECT_NEAR(myGenfPointG.x, 684370.0431519132, 0.000001);
    EXPECT_NEAR(myGenfPointG.y, 5813422.4727304634, 0.000001);
}

TEST_F(TestCoordConvert, DistanceWGS84) {
    tmCoordConvert myConvert(PROJ_WORLD_WGS84);
    wxRealPoint myP1(6.14712, 46.20593);  // GENEVA
    wxRealPoint myP2(7.45782, 46.94832);  // BERN
    double myDist = myConvert.GetDistance(myP1, myP2);
    wxLogMessage("Distance between Geneva and Bern : %f [m]", myDist);
    EXPECT_NEAR(myDist, 130015.3, 0.05);
}

TEST_F(TestCoordConvert, DistanceSwiss) {
    tmCoordConvert myConvert(PROJ_SWISS_CH1903);
    wxRealPoint myP1(500321.34231, 117986.13451);    // GENEVA
    wxRealPoint myP2(601460.745203, 199693.022818);  // BERN
    double myDist = myConvert.GetDistance(myP1, myP2);
    EXPECT_NEAR(myDist, 130015.3, 0.05);
}

TEST_F(TestCoordConvert, PointAtDistanceWGS84) {
    tmCoordConvert myConvert(PROJ_WORLD_WGS84);
    wxRealPoint myP1(6.14712, 46.20593);  // GENEVA
    // search point 20 km east of geneva
    wxRealPoint myP2 = myConvert.GetPointAtDistance(myP1, 20000, 90);
    EXPECT_NEAR(myP2.x, 6.40626894, 0.00001);
    EXPECT_NEAR(myP2.y, 46.20563628, 0.00001);
}

TEST_F(TestCoordConvert, PointAtDistanceSwiss) {
    tmCoordConvert myConvert(PROJ_SWISS_CH1903);
    wxRealPoint myGenfPoint(500374, 117993);
    // search point 100 m east of geneva
    wxRealPoint myP2 = myConvert.GetPointAtDistance(myGenfPoint, 100, 90);
    EXPECT_NEAR(myP2.x, 500474, 0.5);
    EXPECT_NEAR(myP2.y, myGenfPoint.y, 2);
}
