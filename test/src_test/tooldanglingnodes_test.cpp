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

#ifndef _TM_TOOL_DANGLING_NODES_TEST_CPP_
#define _TM_TOOL_DANGLING_NODES_TEST_CPP_


#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>
#include "../../src/gis/tooldanglingnodes.h"

class tmToolDanglingNodesTEST : public CppUnit::TestFixture 
{ 
	CPPUNIT_TEST_SUITE( tmToolDanglingNodesTEST );
	CPPUNIT_TEST( TESTGetDanglingNodes1 );
	CPPUNIT_TEST( TESTCreate1 );
	CPPUNIT_TEST( TESTCreate2 );
	CPPUNIT_TEST( TESTSearch1 );
	CPPUNIT_TEST( TESTSearch2 );
	CPPUNIT_TEST( TESTSearchRun1 );
	CPPUNIT_TEST( TESTSearchRun2 );
	CPPUNIT_TEST( TESTSearchRun3 );
	CPPUNIT_TEST_SUITE_END();
	
private:
	ToolDanglingNodes * m_DN;
	wxString m_DBPathName;
	DataBaseTM * m_DB;
		
public: 
	void setUp()
	{
		
		m_DBPathName = _T("/Users/Lucien/Downloads/testdangling");
		m_DB = new DataBaseTM();
		m_DB->OpenTMDatabase(m_DBPathName);
		m_DN = new ToolDanglingNodes(m_DB);
	}
	
	void tearDown() 
	{
		delete m_DN;
		delete m_DB;
	}
	
	void TESTGetDanglingNodes1()
	{
		
		wxArrayRealPoints myPts;
		CPPUNIT_ASSERT(m_DN->GetDanglingNodes(myPts)==false);		
	}
	
	void TESTCreate1 ()
	{
		ToolDanglingNodes myTool;
		CPPUNIT_ASSERT(myTool.IsOk() == false);
		myTool.Create(m_DB);
		CPPUNIT_ASSERT(myTool.IsOk() == true);
	}

	void TESTCreate2 ()
	{
		
		ToolDanglingNodes myTool2(m_DB);
		CPPUNIT_ASSERT(myTool2.IsOk() == true);	
	}
	
	void TESTSearch1()
	{
		CPPUNIT_ASSERT(m_DN->SearchInit(2)==false);
		CPPUNIT_ASSERT(m_DN->SearchInit(1)==true);
		CPPUNIT_ASSERT(m_DN->SearchInit(100)==false);
	}
	
	void TESTSearch2 ()
	{
		int iFeatureSearched = 0;
		CPPUNIT_ASSERT(m_DN->SearchInfo(iFeatureSearched)==false);
		CPPUNIT_ASSERT(m_DN->SearchInit(1)==true);
		
		CPPUNIT_ASSERT(m_DN->SearchInfo(iFeatureSearched));
		CPPUNIT_ASSERT(iFeatureSearched > 0);
		wxLogDebug(_T("Number of feature to process : %d"), iFeatureSearched);
	}
	
	void TESTSearchRun1()
	{
		CPPUNIT_ASSERT(m_DN->SearchRun()==false);
	}
	
	void TESTSearchRun2()
	{
		int iNum = 0;
		CPPUNIT_ASSERT(m_DN->SearchInit(1)==true);
		CPPUNIT_ASSERT(m_DN->SearchRun()==true);
		CPPUNIT_ASSERT(m_DN->SearchInfo(iNum)==false);
	}
	
	void TESTSearchRun3()
	{
		//int iNum = 0;
		CPPUNIT_ASSERT(m_DN->SearchInit(1)==true);
		CPPUNIT_ASSERT(m_DN->SearchRun()==true);
		wxArrayRealPoints myPts;
		CPPUNIT_ASSERT(m_DN->GetDanglingNodes(myPts)==true);
		wxLogDebug(_T("%d dangling nodes "), myPts.GetCount());
	}
	
};

#endif


