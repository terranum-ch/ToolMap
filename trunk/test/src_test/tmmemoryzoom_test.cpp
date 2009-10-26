/***************************************************************************
								tmupdate_test.cpp
		Test the update process by cheking internet for new versions
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


#ifndef _TM_MEMORY_ZOOM_TEST_H_
#define _TM_MEMORY_ZOOM_TEST_H_


#include "../../src/gis/tmmemoryzoom.h"

#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>
//#include <wx/filefn.h> 

class tmMemoryZoomTEST : public CppUnit::TestFixture 
{ 
	CPPUNIT_TEST_SUITE( tmMemoryZoomTEST );
	CPPUNIT_TEST ( TESTAdd );
	CPPUNIT_TEST ( TESTAddMax );
	CPPUNIT_TEST ( TESTGetPrevious );
	CPPUNIT_TEST ( TESTIsOk );
	CPPUNIT_TEST_SUITE_END();
	
private:
	tmZoomExtent m_Z1;
	
public: 
	void setUp()
	{
		m_Z1.m_TopLeftPosition = wxPoint2DDouble(15.6,118.1);
		m_Z1.m_ZoomFactor = 1.77;
	}
	
	void tearDown() 
	{
		
	}
	
	void TESTAdd()
	{
		tmMemoryZoomManager myManager;
		CPPUNIT_ASSERT(myManager.Add(25.0, 12.0, 1.6666)==true);
		CPPUNIT_ASSERT(myManager.GetCount()==0);
		
		CPPUNIT_ASSERT(myManager.Add(m_Z1)==true);
		CPPUNIT_ASSERT(myManager.GetCount()==1);
	}
	
	void TESTAddMax()
	{
		tmMemoryZoomManager myManager;
		int i = 0;
		myManager.Add(25.0,12.0,1.6666);
		for (i = 0; i< 50; i++)
		{
			myManager.Add(m_Z1);
		}
		CPPUNIT_ASSERT(myManager.GetCount() == 50);
		myManager.Add(m_Z1);
		CPPUNIT_ASSERT(myManager.GetCount() == 50);
	}
	
	void TESTGetPrevious()
	{
		tmMemoryZoomManager myManager;
		myManager.Add(m_Z1);
		CPPUNIT_ASSERT(myManager.GetCount() == 0);
		myManager.Add(25.0,12.0,1.6666);
		CPPUNIT_ASSERT(myManager.GetCount() == 1);
		myManager.Add(m_Z1);
		CPPUNIT_ASSERT(myManager.GetCount() == 2);
		

		
		tmZoomExtent myRetExtent;
		CPPUNIT_ASSERT (myManager.GetPrevious(myRetExtent)==true);
		//CPPUNIT_ASSERT (myRetExtent == m_Z1);
		CPPUNIT_ASSERT (myRetExtent == tmZoomExtent(25.0,12.0,1.6666));
		CPPUNIT_ASSERT (myManager.GetPrevious(myRetExtent)==true);
		CPPUNIT_ASSERT (myRetExtent == m_Z1);
		
		CPPUNIT_ASSERT(myManager.GetCount() == 0);
		myManager.Add(25.0,12.0,1.6666);
		CPPUNIT_ASSERT(myManager.GetCount() == 1);
		CPPUNIT_ASSERT (myManager.GetPrevious(myRetExtent)==true);
		//CPPUNIT_ASSERT (myRetExtent == m_Z1);
		CPPUNIT_ASSERT (myRetExtent == m_Z1);
		CPPUNIT_ASSERT(myManager.GetCount() == 0);
		
		/*myManager.Add(m_Z1);
		CPPUNIT_ASSERT(myManager.GetCount() == 2);
		CPPUNIT_ASSERT (myManager.GetPrevious(myRetExtent)==true);
		CPPUNIT_ASSERT (myRetExtent == m_Z1);*/

		
		//CPPUNIT_ASSERT (myManager.GetPrevious(myRetExtent)==true);
		CPPUNIT_ASSERT (myManager.GetPrevious(myRetExtent) == false);
		
	}
	
	void TESTIsOk()
	{
		tmZoomExtent myExtent;
		CPPUNIT_ASSERT(myExtent.IsOk() == false);
		myExtent.m_ZoomFactor = 1.2;
		CPPUNIT_ASSERT(myExtent.IsOk() == false);
		myExtent.m_TopLeftPosition = wxPoint2DDouble(12.3,1.7);
		CPPUNIT_ASSERT(myExtent.IsOk() == true);
	}
	
		
	
};

#endif


