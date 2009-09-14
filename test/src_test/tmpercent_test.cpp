/***************************************************************************
								tmpercent_test.cpp
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


#ifndef _TM_PERCENT_TEST_CPP_
#define _TM_PERCENT_TEST_CPP_


#include "../../src/core/tmpercent.h"

#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>
//#include <iostream>

class tmPercentTEST : public CppUnit::TestFixture 
{ 
	CPPUNIT_TEST_SUITE( tmPercentTEST );
		CPPUNIT_TEST ( TESTIncrement );
		CPPUNIT_TEST ( TESTGetPercent );
		CPPUNIT_TEST ( TESTSmallPercent );
		CPPUNIT_TEST ( TESTOutput );
		CPPUNIT_TEST ( TESTOutput2 );
	CPPUNIT_TEST_SUITE_END();
	
private:
	
	
public: 
	void setUp()
	{
	}
	
	void tearDown() 
	{
	}
	
	void TESTIncrement()
	{
		tmPercent tper(1000);
		tper.SetValue(150);
		CPPUNIT_ASSERT(tper.IsNewStep()==true);
		tper.SetValue(151);
		CPPUNIT_ASSERT(tper.IsNewStep()==false);
		tper.SetValue(161);
		CPPUNIT_ASSERT(tper.IsNewStep()==true);
	}
	
	void TESTGetPercent()
	{
		tmPercent tper(1000);
		tper.SetValue(150);
		CPPUNIT_ASSERT(tper.GetPercent() == 15);
		tper.SetValue(153);
		CPPUNIT_ASSERT(tper.GetPercent() == 15);
		tper.SetValue(157);
		CPPUNIT_ASSERT(tper.GetPercent() == 15);
		tper.SetValue(160);
		CPPUNIT_ASSERT(tper.GetPercent() == 16);
	}
	
	void TESTSmallPercent()
	{
		tmPercent tper(23);
		tper.SetValue(0);
		CPPUNIT_ASSERT(tper.GetPercent() == 0);
		
		tper.SetValue(12);
		CPPUNIT_ASSERT(tper.GetPercent() == 52);
		
		tper.SetValue(23);
		CPPUNIT_ASSERT(tper.GetPercent() == 100);
		
		tper.SetValue(150);
		CPPUNIT_ASSERT(tper.GetPercent() == 100);
		
	}
	
	void TESTOutput()
	{
		wxLogDebug(_T("Mon test de pourcent"));
		int iTotal = 2543;
		tmPercent tper(iTotal);
		
		for (int i=1; i<=iTotal;i++)
		{
			tper.SetValue(i);
			if (tper.IsNewStep())
				wxLogDebug(_T("Percent : %d"), tper.GetPercent());
		}
	}
	
	
	void TESTOutput2()
	{
		wxLogDebug(_T("Mon test de pourcent - 2"));
		int iTotal = 10;
		tmPercent tper(iTotal);
		
		for (int i=1; i<=iTotal;i++)
		{
			tper.SetValue(i);
			if (tper.IsNewStep())
				wxLogDebug(_T("Percent : %d"), tper.GetPercent());
		}
	}
	
	
};
	


#endif