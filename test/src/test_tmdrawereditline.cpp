/***************************************************************************
 test_tmdrawereditline.h
 Testing editing for lines
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
#include "../../src/core/tmarraysize.h"
#include "../../src/gis/tmdraweredit.h"


class TestDrawerEditLine : public ::testing::Test {
 protected:
  tmDrawerEditLine* m_DL = nullptr;
  wxArrayPoints m_Pts;

  virtual void SetUp() {
    GTEST_SKIP();
    m_DL = new tmDrawerEditLine();
    m_Pts.Add(wxPoint(598, 110));
    m_Pts.Add(wxPoint(520, 111));
    m_Pts.Add(wxPoint(600, 112));
  }
  virtual void TearDown() {
    wxDELETE(m_DL);
    m_Pts.Clear();
  }
};

TEST_F(TestDrawerEditLine, CreateVertex) {
  EXPECT_TRUE(m_DL->CreateVertex(m_Pts, 2));
  EXPECT_TRUE(m_DL->IsEndVertex());
  m_Pts.RemoveAt(2);
  EXPECT_FALSE(m_DL->CreateVertex(m_Pts, 2));
  EXPECT_TRUE(m_DL->CreateVertex(m_Pts, 1));
  EXPECT_TRUE(m_DL->IsEndVertex());
}

TEST_F(TestDrawerEditLine, CreateVertex2) {
  EXPECT_FALSE(m_DL->CreateVertex(m_Pts.Item(0), nullptr, nullptr, 0));
  EXPECT_TRUE(m_DL->CreateVertex(m_Pts.Item(0), &m_Pts.Item(1), &m_Pts.Item(2), 0));
  EXPECT_FALSE(m_DL->IsEndVertex());

  EXPECT_TRUE(m_DL->CreateVertex(m_Pts.Item(0), &m_Pts.Item(1), nullptr, 12));
  EXPECT_TRUE(m_DL->IsEndVertex());
}

TEST_F(TestDrawerEditLine, IsOk) {
  EXPECT_FALSE(m_DL->IsOK());
  EXPECT_TRUE(m_DL->CreateVertex(m_Pts, 0));
  EXPECT_TRUE(m_DL->IsOK());
  EXPECT_TRUE(m_DL->IsEndVertex());

  EXPECT_TRUE(m_DL->CreateVertex(m_Pts, 2));
  EXPECT_TRUE(m_DL->IsOK());
  EXPECT_TRUE(m_DL->IsEndVertex());

  EXPECT_TRUE(m_DL->CreateVertex(m_Pts, 1));
  EXPECT_TRUE(m_DL->IsOK());
  EXPECT_TRUE(!m_DL->IsEndVertex());
}

TEST_F(TestDrawerEditLine, SetVertex) {
  EXPECT_FALSE(m_DL->SetVertex(wxPoint(500, 114)));
  EXPECT_TRUE(m_DL->CreateVertex(m_Pts, 1));
  EXPECT_TRUE(m_DL->SetVertex(wxPoint(500, 114)));
}

TEST_F(TestDrawerEditLine, DrawEditLine) {
  EXPECT_FALSE(m_DL->DrawEditPart(nullptr));
}

TEST_F(TestDrawerEditLine, GetVertexIndex) {
  EXPECT_TRUE(m_DL->GetVertexIndex() == wxNOT_FOUND);
  EXPECT_TRUE(m_DL->CreateVertex(m_Pts, 1));
  EXPECT_TRUE(m_DL->GetVertexIndex() == 1);
  EXPECT_TRUE(m_DL->CreateVertex(m_Pts.Item(0), &m_Pts.Item(1), nullptr, 12));
  EXPECT_TRUE(m_DL->GetVertexIndex() == 12);
}

TEST_F(TestDrawerEditLine, CreateVertex3) {
  EXPECT_TRUE(m_DL->CreateVertex(m_Pts.Item(0)));
  EXPECT_TRUE(m_DL->SetVertex(m_Pts.Item(1)));
  EXPECT_TRUE(m_DL->GetVertexIndex() == wxNOT_FOUND);
}

TEST_F(TestDrawerEditLine, Clear) {
  EXPECT_FALSE(m_DL->ClearVertex());
  EXPECT_TRUE(m_DL->CreateVertex(m_Pts.Item(0)));
  EXPECT_TRUE(m_DL->ClearVertex());
  EXPECT_FALSE(m_DL->IsOK());
}
