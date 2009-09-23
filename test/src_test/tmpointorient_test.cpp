/***************************************************************************
						tmpointorient_test.cpp
						TESTING point orientation class
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

// comment doxygen

#ifndef _TM_POINT_ORIENT_TEST_CPP_
#define _TM_POINT_ORIENT_TEST_CPP_


#include "../../src/gis/tmpointorient.h"
#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>

class tmPointOrienTEST : public CppUnit::TestFixture 
{ 
	CPPUNIT_TEST_SUITE( tmPointOrienTEST );
	CPPUNIT_TEST ( TESTValid );
	CPPUNIT_TEST ( TESTGetOrientInt );
	CPPUNIT_TEST ( TESTGetOrientDouble );
	CPPUNIT_TEST_SUITE_END();
	
private:
	wxPoint myPt1, myPt90, myPt180, myPt270, myPt45, myPt235, myPtd0;
	tmPointOrient myPtO;

	
public: 
	void setUp()
	{
		myPt1 = wxPoint(20,20);
		myPt90 = wxPoint(30,20);
		myPt180 = wxPoint(20,30);
		myPt270 = wxPoint (10,20);
		myPt45 = wxPoint(40,0);
		myPt235 = wxPoint(0,40);
		myPtd0 = wxPoint(20,0);
	}
	
	void tearDown() 
	{
	}
	
	void TESTValid()
	{
		CPPUNIT_ASSERT(myPtO.IsValid()==false);
		myPtO.SetStartPoint(myPt1);
		CPPUNIT_ASSERT(myPtO.IsValid()==false);
		myPtO.SetEndPoint(myPt90);
		CPPUNIT_ASSERT(myPtO.IsValid()==true);
		myPtO.SetEndPoint(myPt1);
		CPPUNIT_ASSERT(myPtO.IsValid()==false);
	}
	
	void TESTGetOrientInt()
	{
		CPPUNIT_ASSERT(myPtO.GetOrientationInt() == wxNOT_FOUND);
		
		myPtO.SetStartPoint(myPt1);
		myPtO.SetEndPoint(myPt1);
		int myOrient = myPtO.GetOrientationInt();
		wxLogDebug(_T("orient error = %d"), myOrient);
		CPPUNIT_ASSERT(myOrient == wxNOT_FOUND);
		
		
		myPtO.SetEndPoint(myPtd0);
		myOrient = myPtO.GetOrientationInt();
		wxLogDebug(_T("orient 0 = %d"), myOrient);
		
		
		
		myPtO.SetEndPoint(myPt90);
		myOrient = myPtO.GetOrientationInt();
		wxLogDebug(_T("orient 90 = %d"), myOrient);
		CPPUNIT_ASSERT(myPtO.GetOrientationInt() == 90);
		//CPPUNIT_ASSERT(myOrient != wxNOT_FOUND);
		
		myPtO.SetEndPoint(myPt45);
		myOrient = myPtO.GetOrientationInt();
		wxLogDebug(_T("orient 45 = %d"), myOrient);
		CPPUNIT_ASSERT(myPtO.GetOrientationInt() == 45);
		
		myPtO.SetEndPoint(myPt235);
		myOrient = myPtO.GetOrientationInt();
		wxLogDebug(_T("orient 225 = %d"), myOrient);
		CPPUNIT_ASSERT(myPtO.GetOrientationInt() == 225);
		
		
		myPtO.SetEndPoint(myPt180);
		myOrient = myPtO.GetOrientationInt();
		wxLogDebug(_T("orient 180 = %d"), myOrient);
		CPPUNIT_ASSERT(myPtO.GetOrientationInt() == 180);
		//CPPUNIT_ASSERT(myOrient != wxNOT_FOUND);
		
		myPtO.SetEndPoint(myPt270);
		myOrient = myPtO.GetOrientationInt();
		wxLogDebug(_T("orient 270 = %d"), myOrient);
		
		CPPUNIT_ASSERT(myPtO.GetOrientationInt() == 270);
		
	}
	
	void TESTGetOrientDouble()
	{
		double dOrient = 0.0;
		dOrient = myPtO.GetOrientationDouble();
		CPPUNIT_ASSERT(dOrient == wxNOT_FOUND);
		myPtO.SetStartPoint(myPt1);
		myPtO.SetEndPoint(myPt1);
		dOrient = myPtO.GetOrientationDouble();
		CPPUNIT_ASSERT(dOrient == wxNOT_FOUND);
		
		myPtO.SetEndPoint(myPt90);
		dOrient = myPtO.GetOrientationDouble();
		CPPUNIT_ASSERT(dOrient == 90.0);
	}


};

#endif


