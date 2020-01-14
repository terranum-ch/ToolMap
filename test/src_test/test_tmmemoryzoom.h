/***************************************************************************
								test_tmmemoryzoom.h
						UNIT TESTING zoom in memory
                             -------------------
    copyright            : (C) 2009 CREALP Lucien Schreiber 
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef _TM_MEMORY_ZOOM_TEST_H_
#define _TM_MEMORY_ZOOM_TEST_H_


#include "wx/wxprec.h"
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <cxxtest/TestSuite.h>

#include "test_param.h"
#include "../../src/gis/tmmemoryzoom.h"

class TEST_tmMemoryZoom : public CxxTest::TestSuite
{
public:
	tmZoomExtent m_Z1;


	void setUp()
	{
		wxApp::SetInstance(new wxAppConsole());
		m_Z1.m_TopLeftPosition = wxPoint2DDouble(15.6,118.1);
		m_Z1.m_ZoomFactor = 1.77;
	}
	
	void tearDown() 
	{
	}
	
	
	void testName(){
		wxLogMessage(_T("------------------------------------"));
		wxLogMessage(_T("------- TESTING TMMEMORYZOOM -------"));
		wxLogMessage(_T("------------------------------------"));
	}
	
	void testAdd()
	{
		tmMemoryZoomManager myManager;
		TS_ASSERT(myManager.Add(25.0, 12.0, 1.6666)==true);
		TS_ASSERT(myManager.GetCount()==0);
		
		TS_ASSERT(myManager.Add(m_Z1)==true);
		TS_ASSERT(myManager.GetCount()==1);
	}
	
	void testAddMax()
	{
		tmMemoryZoomManager myManager;
		int i = 0;
		myManager.Add(25.0,12.0,1.6666);
		for (i = 0; i< 50; i++)
		{
			myManager.Add(m_Z1);
		}
		TS_ASSERT(myManager.GetCount() == 50);
		myManager.Add(m_Z1);
		TS_ASSERT(myManager.GetCount() == 50);
	}
	
	void testGetPrevious()
	{
		tmMemoryZoomManager myManager;
		myManager.Add(m_Z1);
		TS_ASSERT(myManager.GetCount() == 0);
		myManager.Add(25.0,12.0,1.6666);
		TS_ASSERT(myManager.GetCount() == 1);
		myManager.Add(m_Z1);
		TS_ASSERT(myManager.GetCount() == 2);
		
		
		
		tmZoomExtent myRetExtent;
		TS_ASSERT (myManager.GetPrevious(myRetExtent)==true);

		
		TS_ASSERT (myRetExtent == tmZoomExtent(25.0,12.0,1.6666));
		TS_ASSERT (myManager.GetPrevious(myRetExtent)==true);
		TS_ASSERT (myRetExtent == m_Z1);
		
		TS_ASSERT(myManager.GetCount() == 0);
		myManager.Add(25.0,12.0,1.6666);
		TS_ASSERT(myManager.GetCount() == 1);
		TS_ASSERT (myManager.GetPrevious(myRetExtent)==true);

		
		TS_ASSERT (myRetExtent == m_Z1);
		TS_ASSERT(myManager.GetCount() == 0);

		
		TS_ASSERT (myManager.GetPrevious(myRetExtent) == false);
		
	}
	
	void testIsOk()
	{
		tmZoomExtent myExtent;
		TS_ASSERT(myExtent.IsOk() == false);
		myExtent.m_ZoomFactor = 1.2;
		TS_ASSERT(myExtent.IsOk() == false);
		myExtent.m_TopLeftPosition = wxPoint2DDouble(12.3,1.7);
		TS_ASSERT(myExtent.IsOk() == true);
	}
	
	
	
		
};




#endif
