/***************************************************************************
 test_tmstatsrecord.h
 UNIT TESTING for tmstatsrecord
 -------------------
 copyright : (C) 2011 CREALP Lucien Schreiber
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
#include "../../src/core/tmstats.h"
#include "../../src/core/tmstatsrecord.h"
#include "../../src/database/database_tm.h"


class TestStatsRecords : public ::testing::Test {
 protected:
  DataBaseTM *m_pDB = nullptr;

  virtual void SetUp() {
    m_pDB = new DataBaseTM();
    ASSERT_TRUE(m_pDB->OpenTMDatabase(g_TestPathPRJ + g_TestPrj_Stats));
  }
  virtual void TearDown() {
    wxDELETE(m_pDB);
  }
};

TEST_F(TestStatsRecords, Create) {
  tmStatsRecords myRecord(m_pDB);
  tmStatsData myData;
  long myRecordId = myRecord.Create(myData);
  EXPECT_EQ(myRecordId, wxNOT_FOUND);

  myData.m_TimeStart = wxDateTime::Now();
  myRecordId = myRecord.Create(myData);
  EXPECT_NE(myRecordId, wxNOT_FOUND);
  wxLogMessage("Adding stat record with id = %ld", myRecordId);
}

TEST_F(TestStatsRecords, AddRemove) {
  tmStatsRecords myRecord(m_pDB);
  tmStatsData myData;
  myData.m_TimeStart = wxDateTime::Now();
  myData.m_NbClick = 1;
  myData.m_NbAttribution = 2;
  myData.m_NbIntersection = 3;

  long myRecordId = myRecord.Create(myData);
  EXPECT_NE(myRecordId, wxNOT_FOUND);
  EXPECT_TRUE(myRecord.Add(myRecordId, myData));
  EXPECT_TRUE(myRecord.Delete(myRecordId));
  wxLogMessage("Adding and removing stat record with id = %ld", myRecordId);
}

TEST_F(TestStatsRecords, Load) {
  // id = 1 for first record
  tmStatsRecords myRecord(m_pDB);
  tmStatsData myData;

  EXPECT_TRUE(myRecord.Load(1, myData));
  wxLogMessage("time elapsed is: %s", myData.m_TimeElapsed.Format());

  EXPECT_EQ(myData.m_NbClick, 2);
  EXPECT_EQ(myData.m_NbIntersection, 0);
  EXPECT_EQ(myData.m_NbAttribution, 2);
}

TEST_F(TestStatsRecords, GetCount) {
  tmStatsRecords myRecord(m_pDB);
  long myNbRecords = myRecord.GetCount();
  EXPECT_LT(2, myNbRecords);
  wxLogMessage("%ld statistics records!", myNbRecords);
}

TEST_F(TestStatsRecords, LoadTotal) {
  tmStatsRecords myRecord(m_pDB);
  tmStatsData myData;
  EXPECT_TRUE(myRecord.LoadTotal(myData));

  EXPECT_LT(2, myData.m_NbClick);
  EXPECT_LT(1, myData.m_NbAttribution);

  wxLogMessage("Total (Click, attribution, intersection, time): %ld, %ld, %ld, %s", myData.m_NbClick,
               myData.m_NbAttribution, myData.m_NbIntersection, myData.m_TimeElapsed.Format());
}
