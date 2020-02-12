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

#ifndef _TM_TEST_GETTING_FIELDS_INFO_H_
#define _TM_TEST_GETTING_FIELDS_INFO_H_

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <cxxtest/TestSuite.h>

#include "../../src/database/database_tm.h"
#include "test_param.h"

class TEST_GettingFieldsInfo : public CxxTest::TestSuite {
 public:
  DataBaseTM *m_pDB;
  PrjDefMemManage *m_Proj;

  TEST_GettingFieldsInfo(bool bTest) {
    wxApp::SetInstance(new wxAppConsole());
    m_pDB = new DataBaseTM();
    TS_ASSERT(m_pDB->OpenTMDatabase(g_TestPathPRJ + g_TestPrj_AdvAttribution));
    tmGISData::InitGISDrivers(true, true);

    // open project for all tests
    m_Proj = m_pDB->GetProjectDataFromDB();
    TS_ASSERT(m_Proj);
  }

  virtual ~TEST_GettingFieldsInfo() {
    delete m_pDB;
    delete m_Proj;
  }

  static TEST_GettingFieldsInfo *createSuite() {
    return new TEST_GettingFieldsInfo(true);
  }
  static void destroySuite(TEST_GettingFieldsInfo *suite) {
    delete suite;
  }

  void testName() {
    wxLogMessage(_T("------------------------------------"));
    wxLogMessage(_T("--- TESTING TM GETTING FIELD INFO---"));
    wxLogMessage(_T("------------------------------------"));
  }

  void testGettingFieldNameForShapefiles() {
    wxFileName myVectName(g_TestPathSHP, g_TestFileSHP_PLG);
    tmGISDataVector *myVect = tmGISDataVector::CreateGISVectorBasedOnExt(myVectName.GetExt());
    TS_ASSERT(myVect != nullptr);
    TS_ASSERT(myVect->Open(myVectName.GetFullPath(), false));

    wxArrayString myFieldsValue;
    TS_ASSERT_EQUALS(myFieldsValue.GetCount(), 0);

    TS_ASSERT(myVect->GetFieldsName(myFieldsValue));
    TS_ASSERT_EQUALS(myFieldsValue.GetCount(), 4);

    TS_ASSERT(myVect->GetFieldsName(myFieldsValue));
    TS_ASSERT_EQUALS(myFieldsValue.GetCount(), 4);

    wxDELETE(myVect);
  }

  void testGettingFieldsValueForShapefiles() {
    wxFileName myVectName(g_TestPathSHP, g_TestFileSHP_PLG);
    tmGISDataVector *myVect = tmGISDataVector::CreateGISVectorBasedOnExt(myVectName.GetExt());
    TS_ASSERT(myVect != nullptr);
    TS_ASSERT(myVect->Open(myVectName.GetFullPath(), false));

    wxArrayString myValues;
    TS_ASSERT(myVect->GetFieldsValue(myValues, 1));
    TS_ASSERT_EQUALS(myValues.GetCount(), 4);
    // int iTotVal = myValues.GetCount();
    /*for (int i = 0; i< iTotVal; i++) {
            wxLogMessage(_T("Fields value %d = %s"), i, myValues.Item(i).c_str());
    }*/

    TS_ASSERT_EQUALS(myValues[1], _T("BE"));
    TS_ASSERT_EQUALS(myValues[2], _T("Bern"));
    wxDELETE(myVect);
  }

  void testGettingFieldsName2ForMySQL() {
    wxFileName mySQLFileName(g_TestPathPRJ + wxFileName::GetPathSeparator() + g_TestPrj_AdvAttribution,
                             _T("generic_lines.MYD"));
    TS_ASSERT(mySQLFileName.FileExists());
    // wxLogMessage(_T("Opening file : ") + mySQLFileName.GetFullPath());

    // opening layer
    TS_ASSERT(m_pDB);
    tmGISDataVector *myVect = tmGISDataVector::CreateGISVectorBasedOnExt(_T("mysql"));
    TS_ASSERT(myVect != nullptr);
    TS_ASSERT(myVect->GetDataType() == tmGIS_VECTOR_MYSQL);

    tmGISDataVectorMYSQL::SetDataBaseHandle(m_pDB);
    TS_ASSERT_EQUALS(myVect->Open(mySQLFileName.GetFullPath(), true), true);

    // getting fields failed because no project specified
    wxArrayString myFieldsName;
    TS_ASSERT(!myVect->GetFieldsName(myFieldsName, 9));

    // specifiy project object
    ((tmGISDataVectorMYSQL *)myVect)->SetProject(m_Proj);

    // getting fields ok because project specified
    TS_ASSERT(myVect->GetFieldsName(myFieldsName, 9));
    int iTotVal = myFieldsName.GetCount();
    /*for (int i = 0; i< iTotVal; i++) {
            wxLogMessage(_T("Fields name %d = %s"), i, myFieldsName.Item(i).c_str());
    }*/
    TS_ASSERT_EQUALS(iTotVal, 10);
    wxDELETE(myVect);
  }

  void testGettingFieldsValueForMySQL() {
    // MySQL layer to open
    wxFileName mySQLFileName(g_TestPathPRJ + wxFileName::GetPathSeparator() + g_TestPrj_AdvAttribution,
                             _T("generic_lines.MYD"));
    TS_ASSERT(mySQLFileName.FileExists());
    // wxLogMessage(_T("Opening file : ") + mySQLFileName.GetFullPath());

    // opening layer
    TS_ASSERT(m_pDB);
    tmGISDataVector *myVect = tmGISDataVector::CreateGISVectorBasedOnExt(_T("mysql"));
    TS_ASSERT(myVect != nullptr);
    tmGISDataVectorMYSQL::SetDataBaseHandle(m_pDB);
    TS_ASSERT_EQUALS(myVect->Open(mySQLFileName.GetFullPath(), true), true);
    TS_ASSERT(myVect->GetDataType() == tmGIS_VECTOR_MYSQL);
    ((tmGISDataVectorMYSQL *)myVect)->SetProject(m_Proj);

    // getting fields value
    wxArrayString myFieldsValue;
    TS_ASSERT_EQUALS(myFieldsValue.GetCount(), 0);
    TS_ASSERT(myVect->GetFieldsValue(myFieldsValue, 9));

    // int iTotVal = myFieldsValue.GetCount();
    /*for (int i = 0; i< iTotVal; i++) {
    //	wxLogMessage(_T("Fields value %d = %s"), i, myFieldsValue.Item(i).c_str());
    }*/

    TS_ASSERT_EQUALS(myFieldsValue[0], _T("1"));
    TS_ASSERT_EQUALS(myFieldsValue[1], _T("Fault"));
    TS_ASSERT_EQUALS(myFieldsValue[2], _T("2 | Secondary"));

    TS_ASSERT_EQUALS(myFieldsValue[7], _T("4"));
    TS_ASSERT_EQUALS(myFieldsValue[8], _T("Campaign_10"));
    TS_ASSERT_EQUALS(myFieldsValue[9], _T("1203"));
    TS_ASSERT_EQUALS(myFieldsValue.GetCount(), 10);
    wxDELETE(myVect);
  }

  void testGettingFieldsValueForMySQLLabels() {
    // MySQL layer to open
    wxFileName mySQLFileName(g_TestPathPRJ + wxFileName::GetPathSeparator() + g_TestPrj_AdvAttribution,
                             _T("generic_labels.MYD"));
    TS_ASSERT(mySQLFileName.FileExists());
    // wxLogMessage(_T("Opening file : ") + mySQLFileName.GetFullPath());

    // opening layer
    TS_ASSERT(m_pDB);
    tmGISDataVector *myVect = tmGISDataVector::CreateGISVectorBasedOnExt(_T("mysql"));
    TS_ASSERT(myVect != nullptr);
    tmGISDataVectorMYSQL::SetDataBaseHandle(m_pDB);
    TS_ASSERT_EQUALS(myVect->Open(mySQLFileName.GetFullPath(), true), true);
    TS_ASSERT(myVect->GetDataType() == tmGIS_VECTOR_MYSQL);
    ((tmGISDataVectorMYSQL *)myVect)->SetProject(m_Proj);

    // getting fields value
    wxArrayString myFieldsValue;
    TS_ASSERT_EQUALS(myFieldsValue.GetCount(), 0);
    TS_ASSERT(myVect->GetFieldsValue(myFieldsValue, 3));

    TS_ASSERT_EQUALS(myFieldsValue.GetCount(), 4);
    TS_ASSERT(myFieldsValue.Item(myFieldsValue.GetCount() - 1) == _T("L. Schreiber"));
    wxDELETE(myVect);
  }

  void testGettingFieldsName2ForMySQLLabels() {
    wxFileName mySQLFileName(g_TestPathPRJ + wxFileName::GetPathSeparator() + g_TestPrj_AdvAttribution,
                             _T("generic_labels.MYD"));
    TS_ASSERT(mySQLFileName.FileExists());
    // wxLogMessage(_T("Opening file : ") + mySQLFileName.GetFullPath());

    // opening layer
    TS_ASSERT(m_pDB);
    tmGISDataVector *myVect = tmGISDataVector::CreateGISVectorBasedOnExt(_T("mysql"));
    TS_ASSERT(myVect != nullptr);
    TS_ASSERT(myVect->GetDataType() == tmGIS_VECTOR_MYSQL);

    tmGISDataVectorMYSQL::SetDataBaseHandle(m_pDB);
    TS_ASSERT_EQUALS(myVect->Open(mySQLFileName.GetFullPath(), true), true);

    // getting fields failed because no project specified
    wxArrayString myFieldsName;
    TS_ASSERT(!myVect->GetFieldsName(myFieldsName, 3));

    // specifiy project object
    ((tmGISDataVectorMYSQL *)myVect)->SetProject(m_Proj);

    // getting fields ok because project specified
    TS_ASSERT(myVect->GetFieldsName(myFieldsName, 3));
    int iTotVal = myFieldsName.GetCount();
    TS_ASSERT_EQUALS(iTotVal, 4);
    TS_ASSERT(myFieldsName.Item(3) == _T("update_author"));
    wxDELETE(myVect);
  }
};

#endif
