/***************************************************************************
 test_tmgettingfieldsinfo.h
 UNIT TESTING for Getting fields info
 -------------------
 copyright : (C) 2010 CREALP Lucien Schreiber
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


class TestGettingFieldsInfo : public ::testing::Test {
 protected:
  DataBaseTM *m_pDB = nullptr;
  PrjDefMemManage *m_Proj = nullptr;

  virtual void SetUp() {
    m_pDB = new DataBaseTM();
    ASSERT_TRUE(m_pDB->OpenTMDatabase(g_TestPathPRJ + g_TestPrj_AdvAttribution));
    tmGISData::InitGISDrivers(true, true);
    m_Proj = m_pDB->GetProjectDataFromDB();
    ASSERT_TRUE(m_Proj != nullptr);
  }
  virtual void TearDown() {
    wxDELETE(m_pDB);
    wxDELETE(m_Proj);
  }
};

TEST_F(TestGettingFieldsInfo, GettingFieldNameForShapefiles) {
  wxFileName myVectName(g_TestPathSHP, g_TestFileSHP_PLG);
  tmGISDataVector *myVect = tmGISDataVector::CreateGISVectorBasedOnExt(myVectName.GetExt());
  EXPECT_TRUE(myVect != nullptr);
  EXPECT_TRUE(myVect->Open(myVectName.GetFullPath(), false));

  wxArrayString myFieldsValue;
  EXPECT_EQ(myFieldsValue.GetCount(), 0);

  EXPECT_TRUE(myVect->GetFieldsName(myFieldsValue));
  EXPECT_EQ(myFieldsValue.GetCount(), 4);

  EXPECT_TRUE(myVect->GetFieldsName(myFieldsValue));
  EXPECT_EQ(myFieldsValue.GetCount(), 4);

  wxDELETE(myVect);
}

TEST_F(TestGettingFieldsInfo, GettingFieldsValueForShapefiles) {
  wxFileName myVectName(g_TestPathSHP, g_TestFileSHP_PLG);
  tmGISDataVector *myVect = tmGISDataVector::CreateGISVectorBasedOnExt(myVectName.GetExt());
  EXPECT_TRUE(myVect != nullptr);
  EXPECT_TRUE(myVect->Open(myVectName.GetFullPath(), false));

  wxArrayString myValues;
  EXPECT_TRUE(myVect->GetFieldsValue(myValues, 1));
  EXPECT_EQ(myValues.GetCount(), 4);

  EXPECT_EQ(myValues[1], _T("BE"));
  EXPECT_EQ(myValues[2], _T("Bern"));
  wxDELETE(myVect);
}

TEST_F(TestGettingFieldsInfo, GettingFieldsName2ForMySQL) {
  wxFileName mySQLFileName(g_TestPathPRJ + wxFileName::GetPathSeparator() + g_TestPrj_AdvAttribution,
                           _T("generic_lines.MYD"));
  EXPECT_TRUE(mySQLFileName.FileExists());
  // wxLogMessage(_T("Opening file : ") + mySQLFileName.GetFullPath());

  // opening layer
  EXPECT_TRUE(m_pDB != nullptr);
  tmGISDataVector *myVect = tmGISDataVector::CreateGISVectorBasedOnExt(_T("mysql"));
  EXPECT_TRUE(myVect != nullptr);
  EXPECT_TRUE(myVect->GetDataType() == tmGIS_VECTOR_MYSQL);

  tmGISDataVectorMYSQL::SetDataBaseHandle(m_pDB);
  EXPECT_EQ(myVect->Open(mySQLFileName.GetFullPath(), true), true);

  // getting fields failed because no project specified
  wxArrayString myFieldsName;
  EXPECT_TRUE(!myVect->GetFieldsName(myFieldsName, 9));

  // specifiy project object
  ((tmGISDataVectorMYSQL *)myVect)->SetProject(m_Proj);

  // getting fields ok because project specified
  EXPECT_TRUE(myVect->GetFieldsName(myFieldsName, 9));
  int iTotVal = myFieldsName.GetCount();
  EXPECT_EQ(iTotVal, 10);
  wxDELETE(myVect);
}

TEST_F(TestGettingFieldsInfo, GettingFieldsValueForMySQL) {
  // MySQL layer to open
  wxFileName mySQLFileName(g_TestPathPRJ + wxFileName::GetPathSeparator() + g_TestPrj_AdvAttribution,
                           _T("generic_lines.MYD"));
  EXPECT_TRUE(mySQLFileName.FileExists());
  // wxLogMessage(_T("Opening file : ") + mySQLFileName.GetFullPath());

  // opening layer
  EXPECT_TRUE(m_pDB != nullptr);
  tmGISDataVector *myVect = tmGISDataVector::CreateGISVectorBasedOnExt(_T("mysql"));
  EXPECT_TRUE(myVect != nullptr);
  tmGISDataVectorMYSQL::SetDataBaseHandle(m_pDB);
  EXPECT_TRUE(myVect->Open(mySQLFileName.GetFullPath(), true));
  EXPECT_TRUE(myVect->GetDataType() == tmGIS_VECTOR_MYSQL);
  ((tmGISDataVectorMYSQL *)myVect)->SetProject(m_Proj);

  // getting fields value
  wxArrayString myFieldsValue;
  EXPECT_EQ(myFieldsValue.GetCount(), 0);
  EXPECT_TRUE(myVect->GetFieldsValue(myFieldsValue, 9));

  EXPECT_EQ(myFieldsValue[0], _T("1"));
  EXPECT_EQ(myFieldsValue[1], _T("Fault"));
  EXPECT_EQ(myFieldsValue[2], _T("2 | Secondary"));

  EXPECT_EQ(myFieldsValue[7], _T("4"));
  EXPECT_EQ(myFieldsValue[8], _T("Campaign_10"));
  EXPECT_EQ(myFieldsValue[9], _T("1203"));
  EXPECT_EQ(myFieldsValue.GetCount(), 10);
  wxDELETE(myVect);
}

TEST_F(TestGettingFieldsInfo, GettingFieldsValueForMySQLLabels) {
  // MySQL layer to open
  wxFileName mySQLFileName(g_TestPathPRJ + wxFileName::GetPathSeparator() + g_TestPrj_AdvAttribution,
                           _T("generic_labels.MYD"));
  EXPECT_TRUE(mySQLFileName.FileExists());
  // wxLogMessage(_T("Opening file : ") + mySQLFileName.GetFullPath());

  // opening layer
  EXPECT_TRUE(m_pDB != nullptr);
  tmGISDataVector *myVect = tmGISDataVector::CreateGISVectorBasedOnExt(_T("mysql"));
  EXPECT_TRUE(myVect != nullptr);
  tmGISDataVectorMYSQL::SetDataBaseHandle(m_pDB);
  EXPECT_TRUE(myVect->Open(mySQLFileName.GetFullPath(), true));
  EXPECT_TRUE(myVect->GetDataType() == tmGIS_VECTOR_MYSQL);
  ((tmGISDataVectorMYSQL *)myVect)->SetProject(m_Proj);

  // getting fields value
  wxArrayString myFieldsValue;
  EXPECT_EQ(myFieldsValue.GetCount(), 0);
  EXPECT_TRUE(myVect->GetFieldsValue(myFieldsValue, 3));

  EXPECT_EQ(myFieldsValue.GetCount(), 4);
  EXPECT_TRUE(myFieldsValue.Item(myFieldsValue.GetCount() - 1) == _T("L. Schreiber"));
  wxDELETE(myVect);
}

TEST_F(TestGettingFieldsInfo, GettingFieldsName2ForMySQLLabels) {
  wxFileName mySQLFileName(g_TestPathPRJ + wxFileName::GetPathSeparator() + g_TestPrj_AdvAttribution,
                           _T("generic_labels.MYD"));
  EXPECT_TRUE(mySQLFileName.FileExists());
  // wxLogMessage(_T("Opening file : ") + mySQLFileName.GetFullPath());

  // opening layer
  EXPECT_TRUE(m_pDB != nullptr);
  tmGISDataVector *myVect = tmGISDataVector::CreateGISVectorBasedOnExt(_T("mysql"));
  EXPECT_TRUE(myVect != nullptr);
  EXPECT_TRUE(myVect->GetDataType() == tmGIS_VECTOR_MYSQL);

  tmGISDataVectorMYSQL::SetDataBaseHandle(m_pDB);
  EXPECT_TRUE(myVect->Open(mySQLFileName.GetFullPath(), true));

  // getting fields failed because no project specified
  wxArrayString myFieldsName;
  EXPECT_FALSE(myVect->GetFieldsName(myFieldsName, 3));

  // specifiy project object
  ((tmGISDataVectorMYSQL *)myVect)->SetProject(m_Proj);

  // getting fields ok because project specified
  EXPECT_TRUE(myVect->GetFieldsName(myFieldsName, 3));
  int iTotVal = myFieldsName.GetCount();
  EXPECT_EQ(iTotVal, 4);
  EXPECT_TRUE(myFieldsName.Item(3) == _T("update_author"));
  wxDELETE(myVect);
}
