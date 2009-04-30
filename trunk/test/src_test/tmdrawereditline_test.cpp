/***************************************************************************
						tmdrawereditline_test.cpp
						TESTING tmdrawereditline class
                             -------------------
    copyright            : (C) 2007 CREALP Lucien Schreiber 
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

#ifndef _TMDRAWER_EDIT_LINE_TEST_CPP_
#define _TMDRAWER_EDIT_LINE_TEST_CPP_


#include "../../src/gis/tmdraweredit.h"
#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>
#include "../../src/core/tmarraysize.h"

class tmDrawerEditLineTEST : public CppUnit::TestFixture 
{ 
	CPPUNIT_TEST_SUITE( tmDrawerEditLineTEST );
	CPPUNIT_TEST ( TESTCreateDraw );
	CPPUNIT_TEST_SUITE_END();
	
private:
	tmDrawerEditLine * m_DL;
	wxArrayRealPoints m_Pts;
	
	
public: 
	void setUp()
	{
		m_DL = new tmDrawerEditLine();	
		m_Pts.Add(wxRealPoint(598100.10, 110000.100));
		m_Pts.Add(wxRealPoint(520500.20, 111000.200));
		m_Pts.Add(wxRealPoint(600100.30, 112000.300));
	}
	
	void tearDown() 
	{
		delete m_DL;
		m_Pts.Clear();
	}
	
	void TESTCreateDraw()
	{
		CPPUNIT_ASSERT(m_DL->CreateVertex(m_Pts, 2)==true);
		CPPUNIT_ASSERT(m_DL->IsEndVertex()==true);
		m_Pts.RemoveAt(2);
		CPPUNIT_ASSERT(m_DL->CreateVertex(m_Pts, 2)==false);
		CPPUNIT_ASSERT(m_DL->CreateVertex(m_Pts, 1)==true);
		CPPUNIT_ASSERT(m_DL->IsEndVertex()==true);

		
	}
	
};

#endif


