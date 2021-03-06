/***************************************************************************
 test_tooldanglingnodes.h
 Test the dangling nodes tool
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

#ifndef _TM_TOOL_DANGLING_NODES_TEST_H_
#define _TM_TOOL_DANGLING_NODES_TEST_H_

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <cxxtest/TestSuite.h>

#include "../../src/gis/tooldanglingnodes.h"
#include "test_param.h"

class TEST_toolDanglingNodes : public CxxTest::TestSuite {
 public:
  ToolDanglingNodes* m_DN;
  wxString m_DBPathName;
  DataBaseTM* m_DB;

  void setUp() {
    wxApp::SetInstance(new wxAppConsole());
    m_DBPathName = g_TestPathPRJ + g_TestPrj_Dangling;
    m_DB = new DataBaseTM();
    m_DB->OpenTMDatabase(m_DBPathName);
    m_DN = new ToolDanglingNodes(m_DB);
  }

  void tearDown() {
    delete m_DN;
    delete m_DB;
  }

  void testName() {
    wxLogMessage(_T("------------------------------------"));
    wxLogMessage(_T("--- TESTING DANGLING NODES ---------"));
    wxLogMessage(_T("------------------------------------"));
  }

  void testGetDanglingNodes1() {
    wxArrayRealPoints myPts;
    TS_ASSERT(!m_DN->GetDanglingNodes(myPts));
  }

  void testCreate1() {
    ToolDanglingNodes myTool;
    TS_ASSERT(!myTool.IsOk());
    myTool.Create(m_DB);
    TS_ASSERT(myTool.IsOk());
  }

  void testCreate2() {
    ToolDanglingNodes myTool2(m_DB);
    TS_ASSERT(myTool2.IsOk());
  }

  void testSearch1() {
    TS_ASSERT(!m_DN->SearchInit(2, "no layer name"));
    TS_ASSERT(m_DN->SearchInit(1, "no layer name"));
    TS_ASSERT(!m_DN->SearchInit(100, "no layer name"));
  }

  void testSearch2() {
    int iFeatureSearched = 0;
    TS_ASSERT(!m_DN->SearchInfo(iFeatureSearched));
    TS_ASSERT(m_DN->SearchInit(1, "no layer name"));

    TS_ASSERT(m_DN->SearchInfo(iFeatureSearched));
    TS_ASSERT(iFeatureSearched > 0);
    wxLogDebug(_T("Number of feature to process : %d"), iFeatureSearched);
  }

  void testSearchRun1() {
    TS_ASSERT(!m_DN->SearchRun());
  }

  void testSearchRun2() {
    int iNum = 0;
    TS_ASSERT(m_DN->SearchInit(1, "no layer name"));
    TS_ASSERT(m_DN->SearchRun());
    TS_ASSERT(!m_DN->SearchRun());
    TS_ASSERT(!m_DN->SearchInfo(iNum));
  }

  void testSearchRun3() {
    // int iNum = 0;
    TS_ASSERT(m_DN->SearchInit(1, "no layer name"));
    TS_ASSERT(m_DN->SearchRun());
    wxArrayRealPoints myPts;
    bool bGet = m_DN->GetDanglingNodes(myPts);
    TS_ASSERT(bGet);
    if (bGet) {
      wxLogDebug(_T("%zu dangling nodes "), myPts.GetCount());
    }

    for (unsigned int i = 0; i < myPts.GetCount(); i++)
      wxLogDebug(_T("--- dangling node : %.*f, %.*f"), 2, myPts.Item(i).x, 2, myPts.Item(i).y);
  }
};

#endif
