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

#ifndef _TEST_TMDRAWER_EDIT_LINE_H_
#define _TEST_TMDRAWER_EDIT_LINE_H_

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <cxxtest/TestSuite.h>

#include "../../src/core/tmarraysize.h"
#include "../../src/gis/tmdraweredit.h"
#include "test_param.h"

class TEST_tmDrawerEditLine : public CxxTest::TestSuite {
 public:
  tmDrawerEditLine* m_DL;
  wxArrayPoints m_Pts;

  void setUp() {
    wxApp::SetInstance(new wxAppConsole());
    m_DL = new tmDrawerEditLine();
    m_Pts.Add(wxPoint(598, 110));
    m_Pts.Add(wxPoint(520, 111));
    m_Pts.Add(wxPoint(600, 112));
  }

  void tearDown() {
    delete m_DL;
    m_Pts.Clear();
  }

  void testName() {
    wxLogMessage(_T("------------------------------------"));
    wxLogMessage(_T("----- TESTING TMDRAWEREDITLINE -----"));
    wxLogMessage(_T("------------------------------------"));
  }

  void testCreateVertex() {
    TS_ASSERT(m_DL->CreateVertex(m_Pts, 2));
    TS_ASSERT(m_DL->IsEndVertex());
    m_Pts.RemoveAt(2);
    TS_ASSERT(!m_DL->CreateVertex(m_Pts, 2));
    TS_ASSERT(m_DL->CreateVertex(m_Pts, 1));
    TS_ASSERT(m_DL->IsEndVertex());
  }

  void testCreateVertex2() {
    TS_ASSERT(!m_DL->CreateVertex(m_Pts.Item(0), NULL, NULL, 0));
    TS_ASSERT(m_DL->CreateVertex(m_Pts.Item(0), &m_Pts.Item(1), &m_Pts.Item(2), 0));
    TS_ASSERT(!m_DL->IsEndVertex());

    TS_ASSERT(m_DL->CreateVertex(m_Pts.Item(0), &m_Pts.Item(1), NULL, 12));
    TS_ASSERT(m_DL->IsEndVertex());
  }

  void testIsOk() {
    TS_ASSERT(!m_DL->IsOK());
    TS_ASSERT(m_DL->CreateVertex(m_Pts, 0));
    TS_ASSERT(m_DL->IsOK());
    TS_ASSERT(m_DL->IsEndVertex());

    TS_ASSERT(m_DL->CreateVertex(m_Pts, 2));
    TS_ASSERT(m_DL->IsOK());
    TS_ASSERT(m_DL->IsEndVertex());

    TS_ASSERT(m_DL->CreateVertex(m_Pts, 1));
    TS_ASSERT(m_DL->IsOK());
    TS_ASSERT(!m_DL->IsEndVertex());
  }

  void testSetVertex() {
    TS_ASSERT(!m_DL->SetVertex(wxPoint(500, 114)));
    TS_ASSERT(m_DL->CreateVertex(m_Pts, 1));
    TS_ASSERT(m_DL->SetVertex(wxPoint(500, 114)));
  }

  void testDrawEditLine() {
    TS_ASSERT(!m_DL->DrawEditPart(NULL));
  }

  void testGetVertexIndex() {
    TS_ASSERT(m_DL->GetVertexIndex() == wxNOT_FOUND);
    TS_ASSERT(m_DL->CreateVertex(m_Pts, 1));
    TS_ASSERT(m_DL->GetVertexIndex() == 1);
    TS_ASSERT(m_DL->CreateVertex(m_Pts.Item(0), &m_Pts.Item(1), NULL, 12));
    TS_ASSERT(m_DL->GetVertexIndex() == 12);
  }

  void testCreateVertex3() {
    TS_ASSERT(m_DL->CreateVertex(m_Pts.Item(0)));
    TS_ASSERT(m_DL->SetVertex(m_Pts.Item(1)));
    TS_ASSERT(m_DL->GetVertexIndex() == wxNOT_FOUND);
  }

  void testClear() {
    TS_ASSERT(!m_DL->ClearVertex());
    TS_ASSERT(m_DL->CreateVertex(m_Pts.Item(0)));
    TS_ASSERT(m_DL->ClearVertex());
    TS_ASSERT(!m_DL->IsOK());
  }
};

#endif
