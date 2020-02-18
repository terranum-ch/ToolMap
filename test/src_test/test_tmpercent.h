/***************************************************************************
 test_tmpercent.h
 UNIT TESTING for tmPercent class
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

#ifndef _TM_PERCENT_TEST_H_
#define _TM_PERCENT_TEST_H_

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <cxxtest/TestSuite.h>

#include "../../src/core/tmpercent.h"

class TEST_tmPercent : public CxxTest::TestSuite {
 public:
  void testIncrement() {
    tmPercent tper(1000);
    tper.SetValue(150);
    TS_ASSERT(tper.IsNewStep());
    tper.SetValue(151);
    TS_ASSERT(!tper.IsNewStep());
    tper.SetValue(161);
    TS_ASSERT(tper.IsNewStep());
  }

  void testGetPercent() {
    tmPercent tper(1000);
    tper.SetValue(150);
    TS_ASSERT_EQUALS(tper.GetPercent(), 15);
    tper.SetValue(153);
    TS_ASSERT_EQUALS(tper.GetPercent(), 15);
    tper.SetValue(157);
    TS_ASSERT_EQUALS(tper.GetPercent(), 15);
    tper.SetValue(160);
    TS_ASSERT(tper.GetPercent() == 16);
  }

  void testSmallPercent() {
    tmPercent tper(23);
    tper.SetValue(0);
    TS_ASSERT(tper.GetPercent() == 0);

    tper.SetValue(12);
    TS_ASSERT(tper.GetPercent() == 52);

    tper.SetValue(23);
    TS_ASSERT(tper.GetPercent() == 100);

    tper.SetValue(150);
    TS_ASSERT(tper.GetPercent() == 100);
  }

  void testOutput() {
    int iTotal = 2543;
    tmPercent tper(iTotal);

    for (int i = 1; i <= iTotal; i++) {
      tper.SetValue(i);
    }
    TS_ASSERT_EQUALS(tper.GetPercent(), 100);
  }

  void testOutput2() {
    int iTotal = 10;
    tmPercent tper(iTotal);

    int myPercentVals[] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};

    for (int i = 1; i <= iTotal; i++) {
      tper.SetValue(i);
      if (tper.IsNewStep()) {
        TS_ASSERT_EQUALS(tper.GetPercent(), myPercentVals[i - 1]);
      }
    }
  }
};

#endif
