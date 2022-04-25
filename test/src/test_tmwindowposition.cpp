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

#include "gtest/gtest.h"

#include "test_param.h"
#include "../../src/core/tmwindowposition.h"

/*
TEST(TestWindowPosition, SavePosition) {
  tmWindowPosition wnd;
  EXPECT_TRUE(wnd.SavePosition(_T("MyWindow"), wxRect(10, 10, 300, 300)));
}

TEST(TestWindowPosition, LoadPosition) {
  tmWindowPosition wnd;
  wxRect myRect = wxRect(wxDefaultPosition, wxDefaultSize);
  EXPECT_FALSE(wnd.LoadPosition(_T("MyWindow2"), myRect));
  EXPECT_TRUE(myRect == wxRect(wxDefaultPosition, wxDefaultSize));
  EXPECT_TRUE(wnd.LoadPosition(_T("MyWindow"), myRect));
  EXPECT_TRUE(myRect == wxRect(10, 10, 300, 300));
  wnd.SaveScreenPosition();
}

TEST(TestWindowPosition, HasChanged) {
  tmWindowPosition wnd;
  EXPECT_FALSE(wnd.HasScreenChanged());
}

TEST(TestWindowPosition, Intersect) {
  tmWindowPosition wnd;
  wxRect myWndPos = wxRect(50, 50, 300, 300);
  EXPECT_TRUE(wnd.Intersects(myWndPos, wxSize(1024, 780)));
  EXPECT_FALSE(wnd.Intersects(wxRect(1200, 800, 300, 300), wxSize(1024, 780)));
}

TEST(TestWindowPosition, SavePosition2) {
  tmWindowPosition wnd;
  EXPECT_TRUE(wnd.SavePosition(_T("TESTWND"), _T("coucou123")));
}

TEST(TestWindowPosition, LoadPosition2) {
  tmWindowPosition wnd;
  wxString myPostext = wxEmptyString;
  EXPECT_FALSE(wnd.LoadPosition(_T("NO_EXISTING"), myPostext));
  EXPECT_TRUE(myPostext.IsEmpty());
  EXPECT_TRUE(wnd.LoadPosition(_T("TESTWND"), myPostext));
  EXPECT_TRUE(myPostext == _T("coucou123"));
}
*/