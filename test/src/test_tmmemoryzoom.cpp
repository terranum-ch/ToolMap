/***************************************************************************
 test_tmmemoryzoom.h
 UNIT TESTING zoom in memory
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
#include "../../src/gis/tmmemoryzoom.h"


class TestMemoryZoom : public ::testing::Test {
  protected:
    tmZoomExtent m_Z1;

    virtual void SetUp() {
        wxApp::SetInstance(new wxAppConsole());
        m_Z1.m_TopLeftPosition = wxPoint2DDouble(15.6, 118.1);
        m_Z1.m_ZoomFactor = 1.77;
    }
    virtual void TearDown() {
        ;
    }
};

TEST_F(TestMemoryZoom, Add) {
    tmMemoryZoomManager myManager;
    EXPECT_TRUE(myManager.Add(25.0, 12.0, 1.6666));
    EXPECT_TRUE(myManager.GetCount() == 0);

    EXPECT_TRUE(myManager.Add(m_Z1));
    EXPECT_TRUE(myManager.GetCount() == 1);
}

TEST_F(TestMemoryZoom, AddMax) {
    tmMemoryZoomManager myManager;
    int i = 0;
    myManager.Add(25.0, 12.0, 1.6666);
    for (i = 0; i < 50; i++) {
        myManager.Add(m_Z1);
    }
    EXPECT_TRUE(myManager.GetCount() == 50);
    myManager.Add(m_Z1);
    EXPECT_TRUE(myManager.GetCount() == 50);
}

TEST_F(TestMemoryZoom, GetPrevious) {
    tmMemoryZoomManager myManager;
    myManager.Add(m_Z1);
    EXPECT_TRUE(myManager.GetCount() == 0);
    myManager.Add(25.0, 12.0, 1.6666);
    EXPECT_TRUE(myManager.GetCount() == 1);
    myManager.Add(m_Z1);
    EXPECT_TRUE(myManager.GetCount() == 2);

    tmZoomExtent myRetExtent;
    EXPECT_TRUE(myManager.GetPrevious(myRetExtent));

    EXPECT_TRUE(myRetExtent == tmZoomExtent(25.0, 12.0, 1.6666));
    EXPECT_TRUE(myManager.GetPrevious(myRetExtent));
    EXPECT_TRUE(myRetExtent == m_Z1);

    EXPECT_TRUE(myManager.GetCount() == 0);
    myManager.Add(25.0, 12.0, 1.6666);
    EXPECT_TRUE(myManager.GetCount() == 1);
    EXPECT_TRUE(myManager.GetPrevious(myRetExtent));

    EXPECT_TRUE(myRetExtent == m_Z1);
    EXPECT_TRUE(myManager.GetCount() == 0);

    EXPECT_FALSE(myManager.GetPrevious(myRetExtent));
}

TEST_F(TestMemoryZoom, IsOk) {
    tmZoomExtent myExtent;
    EXPECT_FALSE(myExtent.IsOk());
    myExtent.m_ZoomFactor = 1.2;
    EXPECT_FALSE(myExtent.IsOk());
    myExtent.m_TopLeftPosition = wxPoint2DDouble(12.3, 1.7);
    EXPECT_TRUE(myExtent.IsOk());
}
