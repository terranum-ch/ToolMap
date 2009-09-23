/***************************************************************************
						tmpointorientattrib_test.cpp
						TESTING point orientation & attribution class
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

#ifndef _TM_POINT_ORIENT_ATTRIB_TEST_CPP_
#define _TM_POINT_ORIENT_ATTRIB_TEST_CPP_


#include "../../src/gis/tmpointorientattrib.h"
#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>

class tmPointOrientAttribTEST : public CppUnit::TestFixture 
{ 
	CPPUNIT_TEST_SUITE( tmPointOrientAttribTEST );
	CPPUNIT_TEST ( TESTValid );
	CPPUNIT_TEST_SUITE_END();
	
private:
	wxPoint myPt1, myPt90, myPt180, myPt270, myPt45, myPt235, myPtd0;
	tmPointOrientAttrib myPtA;
	
	DataBaseTM * m_DB;

	
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
		
		
		m_DB = new DataBaseTM();
		m_DB->OpenTMDatabase(_T("/Users/Lucien/Downloads/testexport"));
		
	}
	
	void tearDown() 
	{
							 delete m_DB;
	}
	
	void TESTValid()
	{
		CPPUNIT_ASSERT(myPtA.IsValid()==false);
		myPtA.SetStartPoint(myPt1);
		CPPUNIT_ASSERT(myPtA.IsValid()==false);
		myPtA.SetEndPoint(myPt90);
		CPPUNIT_ASSERT(myPtA.IsValid()==false);
		myPtA.Create(m_DB, 2);
		CPPUNIT_ASSERT(myPtA.IsValid()==false);
		myPtA.Create(m_DB, 5);
		CPPUNIT_ASSERT(myPtA.IsValid()==true);
		
		
		
	}
	
	
	

};

#endif


