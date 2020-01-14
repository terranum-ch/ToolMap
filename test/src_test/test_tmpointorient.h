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

#ifndef _TM_POINT_ORIENT_TEST_H_
#define _TM_POINT_ORIENT_TEST_H_

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <cxxtest/TestSuite.h>

#include "../../src/gis/tmpointorient.h"

class TEST_tmPointOrient : public CxxTest::TestSuite {
 public:
  wxPoint myPt1, myPt90, myPt180, myPt270, myPt45, myPt235, myPtd0, myPt315;

  void setUp() {
    wxApp::SetInstance(new wxAppConsole());
    myPt1 = wxPoint(20, 20);
    myPt90 = wxPoint(30, 20);
    myPt180 = wxPoint(20, 30);
    myPt270 = wxPoint(10, 20);
    myPt45 = wxPoint(40, 0);
    myPt235 = wxPoint(0, 40);
    myPt315 = wxPoint(0, 0);
    myPtd0 = wxPoint(20, 0);
  }

  void tearDown() {}

  void testName() {
    wxLogMessage(_T("------------------------------------"));
    wxLogMessage(_T("------- TESTING TMPOINTORIENT ------"));
    wxLogMessage(_T("------------------------------------"));
  }

  void testValid() {
    tmPointOrient myPtO;
    TS_ASSERT(myPtO.IsValid() == false);
    myPtO.SetStartPoint(myPt1);
    TS_ASSERT(myPtO.IsValid() == false);
    myPtO.SetEndPoint(myPt90);
    TS_ASSERT(myPtO.IsValid() == true);
    myPtO.SetEndPoint(myPt1);
    TS_ASSERT(myPtO.IsValid() == false);
  }

  void testGetOrientInt() {
    tmPointOrient myPtO;
    TS_ASSERT(myPtO.GetOrientationInt() == wxNOT_FOUND);

    myPtO.SetStartPoint(myPt1);
    myPtO.SetEndPoint(myPt1);
    int myOrient = myPtO.GetOrientationInt();
    wxLogDebug(_T("orient error = %d"), myOrient);
    TS_ASSERT(myOrient == wxNOT_FOUND);

    myPtO.SetEndPoint(myPtd0);
    myOrient = myPtO.GetOrientationInt();
    wxLogDebug(_T("orient 0 = %d"), myOrient);

    myPtO.SetEndPoint(myPt90);
    myOrient = myPtO.GetOrientationInt();
    wxLogDebug(_T("orient 90 = %d"), myOrient);
    TS_ASSERT(myPtO.GetOrientationInt() == 90);
    // TS_ASSERT(myOrient != wxNOT_FOUND);

    myPtO.SetEndPoint(myPt45);
    myOrient = myPtO.GetOrientationInt();
    wxLogDebug(_T("orient 45 = %d"), myOrient);
    TS_ASSERT(myPtO.GetOrientationInt() == 45);

    myPtO.SetEndPoint(myPt235);
    myOrient = myPtO.GetOrientationInt();
    wxLogDebug(_T("orient 225 = %d"), myOrient);
    TS_ASSERT(myPtO.GetOrientationInt() == 225);

    myPtO.SetEndPoint(myPt180);
    myOrient = myPtO.GetOrientationInt();
    wxLogDebug(_T("orient 180 = %d"), myOrient);
    TS_ASSERT(myPtO.GetOrientationInt() == 180);
    // TS_ASSERT(myOrient != wxNOT_FOUND);

    myPtO.SetEndPoint(myPt270);
    myOrient = myPtO.GetOrientationInt();
    wxLogDebug(_T("orient 270 = %d"), myOrient);

    TS_ASSERT(myPtO.GetOrientationInt() == 270);

    myPtO.SetEndPoint(myPt315);
    myOrient = myPtO.GetOrientationInt();
    wxLogDebug(_T("orient 315 = %d"), myOrient);

    TS_ASSERT(myPtO.GetOrientationInt() == 315);
  }

  void testGetOrientDouble() {
    tmPointOrient myPtO;
    double dOrient = 0.0;
    dOrient = myPtO.GetOrientationDouble();
    TS_ASSERT_EQUALS(dOrient, wxNOT_FOUND);
    myPtO.SetStartPoint(myPt1);
    myPtO.SetEndPoint(myPt1);
    dOrient = myPtO.GetOrientationDouble();
    TS_ASSERT_EQUALS(dOrient, wxNOT_FOUND);

    myPtO.SetEndPoint(myPt90);
    dOrient = myPtO.GetOrientationDouble();
    TS_ASSERT(dOrient == 90.0);
  }
};

#endif
