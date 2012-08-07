/***************************************************************************
								test_tmpercent.h
                    UNIT TESTING for tmPercent class
                             -------------------
    copyright            : (C) 2009 CREALP Lucien Schreiber 
    email                : lucien.schreiber at crealp dot vs dot ch
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



#include "wx/wxprec.h"
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <cxxtest/TestSuite.h>

#include "../../src/core/tmpercent.h"

class TEST_tmPercent : public CxxTest::TestSuite
{
public:
		
	void testName(){
		wxLogMessage(_T("------------------------------------"));
		wxLogMessage(_T("------- TESTING TMPERCENT ----------"));
		wxLogMessage(_T("------------------------------------"));
	}
	
	void testIncrement()
	{
		tmPercent tper(1000);
		tper.SetValue(150);
		TS_ASSERT(tper.IsNewStep()==true);
		tper.SetValue(151);
		TS_ASSERT(tper.IsNewStep()==false);
		tper.SetValue(161);
		TS_ASSERT(tper.IsNewStep()==true);
	}
	
	void testGetPercent()
	{
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
	
	void testSmallPercent()
	{
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
	
	void testOutput()
	{
		wxLogDebug(_T("Mon test de pourcent"));
		int iTotal = 2543;
		tmPercent tper(iTotal);
		
		for (int i=1; i<=iTotal;i++)
		{
			tper.SetValue(i);
			if (tper.IsNewStep()){
				wxLogDebug(_T("Percent : %d"), tper.GetPercent());
            }
		}
	}
	
	
	void testOutput2()
	{
		wxLogDebug(_T("Mon test de pourcent - 2"));
		int iTotal = 10;
		tmPercent tper(iTotal);
		
		for (int i=1; i<=iTotal;i++)
		{
			tper.SetValue(i);
			if (tper.IsNewStep()){
				wxLogDebug(_T("Percent : %d"), tper.GetPercent());
            }
		}
	}
	
	
	
		
};




#endif
