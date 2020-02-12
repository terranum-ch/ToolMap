/***************************************************************************
 TEST_DatabaseResults.h
 Testing Database class
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

#ifndef _TEST_DATABASE_RESULTS_H_
#define _TEST_DATABASE_RESULTS_H_

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <cxxtest/TestSuite.h>

#include "../../src/database/database_tm.h"
#include "../../src/database/databaseresult.h"
#include "test_param.h"

class TEST_DatabaseResults : public CxxTest::TestSuite {
 public:
  DataBaseTM *m_pDB;

  TEST_DatabaseResults(bool bTest) {
    wxApp::SetInstance(new wxAppConsole());
    m_pDB = new DataBaseTM();
    TS_ASSERT(m_pDB->OpenTMDatabase(g_TestPathPRJ + g_TestPrj_AdvAttribution));
  }

  virtual ~TEST_DatabaseResults() {
    delete m_pDB;
  }

  static TEST_DatabaseResults *createSuite() {
    return new TEST_DatabaseResults(true);
  }
  static void destroySuite(TEST_DatabaseResults *suite) {
    delete suite;
  }

  void testName() {
    wxLogMessage(_T("------------------------------------"));
    wxLogMessage(_T("--- TESTING DATABASE RESULTS   ---"));
    wxLogMessage(_T("------------------------------------"));
  }

  void testCreatorDtor() {
    TS_ASSERT(m_pDB->DataBaseQuery(_T("SELECT * FROM zoom_level")));
    DataBaseResult *myRes = new DataBaseResult();
    TS_ASSERT(m_pDB->DataBaseGetResults(myRes));
    delete myRes;
    TS_ASSERT(!m_pDB->DataBaseHasResults());
  }

  void testGetCount() {
    TS_ASSERT(m_pDB->DataBaseQuery(_T("SELECT * FROM zoom_level")));
    DataBaseResult myRes;
    TS_ASSERT(m_pDB->DataBaseGetResults(&myRes));
    TS_ASSERT_EQUALS(myRes.GetColCount(), 3);
    TS_ASSERT_EQUALS(myRes.GetRowCount(), 4);
  }

  void testHasResults() {
    TS_ASSERT(m_pDB->DataBaseQuery(_T("SELECT * FROM zoom_level WHERE ZOOM_ID=100")));
    DataBaseResult myRes;
    TS_ASSERT(m_pDB->DataBaseGetResults(&myRes));
    TS_ASSERT(!myRes.HasResults());

    // no result inited !
    DataBaseResult myRes2;
    TS_ASSERT(!myRes2.HasResults());
  }

  void testGetColName() {
    TS_ASSERT(m_pDB->DataBaseQuery(_T("SELECT * FROM zoom_level")));
    DataBaseResult myRes;
    TS_ASSERT(m_pDB->DataBaseGetResults(&myRes));
    wxArrayString myColName;
    TS_ASSERT(myRes.GetColName(myColName));
    TS_ASSERT_EQUALS(myColName.GetCount(), 3);
    for (unsigned int i = 0; i < myColName.GetCount(); i++) {
      wxLogMessage(_T("Col name : ") + myColName.Item(i));
    }
  }

  void testIsRowOk() {
    TS_ASSERT(m_pDB->DataBaseQuery(_T("SELECT * FROM zoom_level")));
    DataBaseResult myRes;
    TS_ASSERT(m_pDB->DataBaseGetResults(&myRes));
    TS_ASSERT(!myRes.IsRowOk());
    TS_ASSERT(myRes.NextRow());
    TS_ASSERT(myRes.IsRowOk());
  }

  void testGetValue() {
    TS_ASSERT(m_pDB->DataBaseQuery(_T("SELECT * FROM zoom_level")));
    DataBaseResult myRes;
    TS_ASSERT(m_pDB->DataBaseGetResults(&myRes));
    TS_ASSERT(myRes.NextRow());

    wxString myValue = wxEmptyString;
    TS_ASSERT(myRes.GetValue(1, myValue));
    TS_ASSERT(myValue == _T("5000"));
    wxLogMessage(_T("Value for col 1 is ") + myValue);
    TS_ASSERT(myRes.NextRow());
    TS_ASSERT(myRes.GetValue(1, myValue));
    TS_ASSERT(myValue == _T("10000"));

    TS_ASSERT(myRes.NextRow());
    TS_ASSERT(myRes.NextRow());
    TS_ASSERT(!myRes.NextRow());
  }

  void testGetValueLong() {
    TS_ASSERT(m_pDB->DataBaseQuery(_T("SELECT * FROM thematic_layers WHERE LAYER_INDEX = 1")));
    DataBaseResult myRes;
    TS_ASSERT(m_pDB->DataBaseGetResults(&myRes));
    TS_ASSERT(myRes.NextRow());

    long myValueLong;
    TS_ASSERT(myRes.GetValue(0, myValueLong));
    TS_ASSERT_EQUALS(myValueLong, 1);

    wxString myValueStr = wxEmptyString;
    TS_ASSERT(myRes.GetValue(2, myValueStr));
    TS_ASSERT(myValueStr == _T("TectoBound_L"));

    // trying to get string to long
    TS_ASSERT(!myRes.GetValue(2, myValueLong))
  }

  void testGetValueGeom() {
    TS_ASSERT(m_pDB->DataBaseQuery(
                  _T("SELECT OBJECT_ID, AsWKB(OBJECT_GEOMETRY) FROM generic_lines WHERE OBJECT_ID = 13")));
    DataBaseResult myRes;
    TS_ASSERT(m_pDB->DataBaseGetResults(&myRes));
    TS_ASSERT(myRes.NextRow());

    OGRGeometry *myGeom = NULL;
    TS_ASSERT(myRes.GetValue(1, &myGeom));
    TS_ASSERT(myGeom != NULL);
    TS_ASSERT(wkbFlatten(myGeom->getGeometryType()) == wkbLineString);
    wxLogMessage(_T("Returned line has %d vertex"), ((OGRLineString *)myGeom)->getNumPoints());
    OGRGeometryFactory::destroyGeometry(myGeom);
  }
};

#endif
