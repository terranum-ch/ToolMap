/***************************************************************************
								tmwindowposition.cpp
				Test the window position saving / loading process
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


#ifndef _TM_WINDOWS_POSITION_TEST_H_
#define _TM_WINDOWS_POSITION_TEST_H_


#include "../../src/core/tmwindowposition.h"

#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>
#include <wx/filefn.h> 

class tmWindowPositionTEST : public CppUnit::TestFixture 
{ 
	CPPUNIT_TEST_SUITE( tmWindowPositionTEST );
		CPPUNIT_TEST ( TESTSavePosition );
		CPPUNIT_TEST ( TESTLoadPosition );
		CPPUNIT_TEST ( TESTHasChanged );
		CPPUNIT_TEST ( TESTIntersect );
	CPPUNIT_TEST_SUITE_END();
	
private:
	
public: 
	void setUp()
	{
	}
	
	void tearDown() 
	{
	}
	
	void TESTSavePosition()
	{
		tmWindowPosition wnd; // (m_Frame, m_manager);
		CPPUNIT_ASSERT(wnd.SavePosition(_T("MyWindow"), wxRect(10,10,300,300))==true);
		
	}
	
	
	void TESTLoadPosition()
	{
		tmWindowPosition wnd; // (m_Frame, m_manager);
		wxRect myRect = wxRect(wxDefaultPosition, wxDefaultSize);
		CPPUNIT_ASSERT(wnd.LoadPosition(_T("MyWindow2"), myRect)==false);
		CPPUNIT_ASSERT(myRect == wxRect(wxDefaultPosition, wxDefaultSize));
		CPPUNIT_ASSERT(wnd.LoadPosition(_T("MyWindow"), myRect)==true);
		CPPUNIT_ASSERT(myRect == wxRect(10,10,300,300));
	}
	
	void TESTHasChanged ()
	{
		tmWindowPosition wnd;
		CPPUNIT_ASSERT(wnd.HasScreenChanged()==false);
	}
	
	void TESTIntersect()
	{
		tmWindowPosition wnd;
		wxRect myWndPos = wxRect(50,50,300,300);
		CPPUNIT_ASSERT(wnd.Intersects(myWndPos, wxSize(1024,780))==true);
		CPPUNIT_ASSERT(wnd.Intersects(wxRect(1200,800, 300,300), wxSize(1024,780))==false);
	
		
	}
	
	
};

#endif


