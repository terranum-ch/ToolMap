/***************************************************************************
 test_tmpercent.h
 UNIT TESTING for tmPercent class
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

#include "../../src/core/tmpercent.h"


TEST(TestPercent, Increment) {
    tmPercent tper(1000);
    tper.SetValue(150);
    EXPECT_TRUE(tper.IsNewStep());
    tper.SetValue(151);
    EXPECT_TRUE(!tper.IsNewStep());
    tper.SetValue(161);
    EXPECT_TRUE(tper.IsNewStep());
}

TEST(TestPercent, GetPercent) {
    tmPercent tper(1000);
    tper.SetValue(150);
    EXPECT_EQ(tper.GetPercent(), 15);
    tper.SetValue(153);
    EXPECT_EQ(tper.GetPercent(), 15);
    tper.SetValue(157);
    EXPECT_EQ(tper.GetPercent(), 15);
    tper.SetValue(160);
    EXPECT_TRUE(tper.GetPercent() == 16);
}

TEST(TestPercent, SmallPercent) {
    tmPercent tper(23);
    tper.SetValue(0);
    EXPECT_TRUE(tper.GetPercent() == 0);

    tper.SetValue(12);
    EXPECT_TRUE(tper.GetPercent() == 52);

    tper.SetValue(23);
    EXPECT_TRUE(tper.GetPercent() == 100);

    tper.SetValue(150);
    EXPECT_TRUE(tper.GetPercent() == 100);
}

TEST(TestPercent, Output) {
    int iTotal = 2543;
    tmPercent tper(iTotal);

    for (int i = 1; i <= iTotal; i++) {
        tper.SetValue(i);
    }
    EXPECT_EQ(tper.GetPercent(), 100);
}

TEST(TestPercent, Output2) {
    int iTotal = 10;
    tmPercent tper(iTotal);

    int myPercentVals[] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};

    for (int i = 1; i <= iTotal; i++) {
        tper.SetValue(i);
        if (tper.IsNewStep()) {
            EXPECT_EQ(tper.GetPercent(), myPercentVals[i - 1]);
        }
    }
}