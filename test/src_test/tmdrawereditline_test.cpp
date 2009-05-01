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
	CPPUNIT_TEST ( TESTCreateVertex );
	CPPUNIT_TEST (TESTCreateVertex2 );
	CPPUNIT_TEST ( TESTIsOk );
	CPPUNIT_TEST ( TESTSetVertex );
	CPPUNIT_TEST ( TestDrawEditLine );
	CPPUNIT_TEST ( TESTGetVertexIndex );
	CPPUNIT_TEST_SUITE_END();
	
private:
	tmDrawerEditLine * m_DL;
	wxArrayPoints m_Pts;
	
	
public: 
	void setUp()
	{
		m_DL = new tmDrawerEditLine();	
		m_Pts.Add(wxPoint(598, 110));
		m_Pts.Add(wxPoint(520, 111));
		m_Pts.Add(wxPoint(600, 112));
	}
	
	void tearDown() 
	{
		delete m_DL;
		m_Pts.Clear();
	}
	
	void TESTCreateVertex()
	{
		CPPUNIT_ASSERT(m_DL->CreateVertex(m_Pts, 2)==true);
		CPPUNIT_ASSERT(m_DL->IsEndVertex()==true);
		m_Pts.RemoveAt(2);
		CPPUNIT_ASSERT(m_DL->CreateVertex(m_Pts, 2)==false);
		CPPUNIT_ASSERT(m_DL->CreateVertex(m_Pts, 1)==true);
		CPPUNIT_ASSERT(m_DL->IsEndVertex()==true);
	}

	void TESTCreateVertex2()
	{
		CPPUNIT_ASSERT(m_DL->CreateVertex(m_Pts.Item(0),NULL, NULL, 0)==false);
		CPPUNIT_ASSERT(m_DL->CreateVertex(m_Pts.Item(0),&m_Pts.Item(1),&m_Pts.Item(2), 0)==true);
		CPPUNIT_ASSERT(m_DL->IsEndVertex()==false);
		
		CPPUNIT_ASSERT(m_DL->CreateVertex(m_Pts.Item(0),&m_Pts.Item(1),NULL,12)==true);
		CPPUNIT_ASSERT(m_DL->IsEndVertex()==true);
	}
	
	
	
	void TESTIsOk()
	{
		
		CPPUNIT_ASSERT(m_DL->IsOK()==false);
		CPPUNIT_ASSERT(m_DL->CreateVertex(m_Pts, 0)==true);
		CPPUNIT_ASSERT(m_DL->IsOK()==true);
		CPPUNIT_ASSERT(m_DL->IsEndVertex()==true);

		CPPUNIT_ASSERT(m_DL->CreateVertex(m_Pts, 2)==true);
		CPPUNIT_ASSERT(m_DL->IsOK()==true);
		CPPUNIT_ASSERT(m_DL->IsEndVertex()==true);
		
		CPPUNIT_ASSERT(m_DL->CreateVertex(m_Pts, 1)==true);
		CPPUNIT_ASSERT(m_DL->IsOK()==true);
		CPPUNIT_ASSERT(m_DL->IsEndVertex()==false);
	}
	
	void TESTSetVertex()
	{
		CPPUNIT_ASSERT(m_DL->SetVertex(wxPoint(500,114))==false);
		CPPUNIT_ASSERT(m_DL->CreateVertex(m_Pts, 1)==true);
		CPPUNIT_ASSERT(m_DL->SetVertex(wxPoint(500,114))==true);
	}
	
	void TestDrawEditLine()
	{
		
		CPPUNIT_ASSERT(m_DL->DrawEditLine(NULL)==false);
		
	}
	
	void TESTGetVertexIndex()
	{
		CPPUNIT_ASSERT(m_DL->GetVertexIndex()==wxNOT_FOUND);
		CPPUNIT_ASSERT(m_DL->CreateVertex(m_Pts, 1));
		CPPUNIT_ASSERT(m_DL->GetVertexIndex()==1);
		CPPUNIT_ASSERT(m_DL->CreateVertex(m_Pts.Item(0),
										  &m_Pts.Item(1),
										  NULL,
										  12));
		CPPUNIT_ASSERT(m_DL->GetVertexIndex() == 12);
	}
	
	
};

#endif


