/***************************************************************************
 test_tmwindowposition.h
 Test the window position saving / loading process
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

#ifndef _TM_WINDOWS_POSITION_TEST_H_
#define _TM_WINDOWS_POSITION_TEST_H_

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <cxxtest/TestSuite.h>

#include "../../src/core/tmwindowposition.h"

class TEST_tmWindowPosition : public CxxTest::TestSuite {
 public:
  TEST_tmWindowPosition(bool bTest) {
    wxApp::SetInstance(new wxAppConsole());
  }

  virtual ~TEST_tmWindowPosition() {}

  static TEST_tmWindowPosition *createSuite() {
    return new TEST_tmWindowPosition(true);
  }
  static void destroySuite(TEST_tmWindowPosition *suite) {
    delete suite;
  }

  void testName() {
    wxLogMessage(_T("------------------------------------"));
    wxLogMessage(_T("--- TESTING WINDOW POSITION --------"));
    wxLogMessage(_T("------------------------------------"));
  }

  void testSavePosition() {
    tmWindowPosition wnd;
    TS_ASSERT(wnd.SavePosition(_T("MyWindow"), wxRect(10, 10, 300, 300)));
  }

  void testLoadPosition() {
    tmWindowPosition wnd;
    wxRect myRect = wxRect(wxDefaultPosition, wxDefaultSize);
    TS_ASSERT(!wnd.LoadPosition(_T("MyWindow2"), myRect));
    TS_ASSERT(myRect == wxRect(wxDefaultPosition, wxDefaultSize));
    TS_ASSERT(wnd.LoadPosition(_T("MyWindow"), myRect));
    TS_ASSERT(myRect == wxRect(10, 10, 300, 300));
    wnd.SaveScreenPosition();
  }

  void testHasChanged() {
    tmWindowPosition wnd;
    TS_ASSERT(!wnd.HasScreenChanged());
  }

  void testIntersect() {
    tmWindowPosition wnd;
    wxRect myWndPos = wxRect(50, 50, 300, 300);
    TS_ASSERT(wnd.Intersects(myWndPos, wxSize(1024, 780)));
    TS_ASSERT(!wnd.Intersects(wxRect(1200, 800, 300, 300), wxSize(1024, 780)));
  }

  void testSavePosition2() {
    tmWindowPosition wnd;
    TS_ASSERT(wnd.SavePosition(_T("TESTWND"), _T("coucou123")));
  }

  void testLoadPosition2() {
    tmWindowPosition wnd;
    wxString myPostext = wxEmptyString;
    TS_ASSERT(!wnd.LoadPosition(_T("NO_EXISTING"), myPostext));
    TS_ASSERT(myPostext.IsEmpty());
    TS_ASSERT(wnd.LoadPosition(_T("TESTWND"), myPostext));
    TS_ASSERT(myPostext == _T("coucou123"));
  }
};

#endif
