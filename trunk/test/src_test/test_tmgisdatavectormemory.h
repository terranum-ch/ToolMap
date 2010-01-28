/***************************************************************************
							test_tmgisdatavectormemeory.h
						UNIT TESTING for gis data in memory
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

#ifndef _TM_TEST_GISDATA_VECTOR_MEMORY_H_
#define _TM_TEST_GISDATA_VECTOR_MEMORY_H_


#include "wx/wxprec.h"
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <cxxtest/TestSuite.h>

#include "test_param.h"
//#include "../../src/database/database.h"
//#include "../../src/gis/tmgisdatavectormemory.h"
#include "../../src/gis/tmgisdatavectormemory.h"

class TEST_tmGISDataVectorMemory : public CxxTest::TestSuite
{
public:
	DataBaseTM * m_pDB;
	tmGISDataVectorMemory * m_MemData;

	TEST_tmGISDataVectorMemory (bool bTest){
		m_pDB = new DataBaseTM();
		TS_ASSERT(m_pDB->OpenTMDatabase(g_TestPathPRJ + g_TestPrj_Boltigen));
	}
	
	virtual ~TEST_tmGISDataVectorMemory(){
		delete m_pDB;
	}
	
	static TEST_tmGISDataVectorMemory *createSuite() { return new TEST_tmGISDataVectorMemory(true);}
    static void destroySuite( TEST_tmGISDataVectorMemory *suite ) { delete suite; }

	
	void setUp()
	{
		m_MemData = new tmGISDataVectorMemory();
		TS_ASSERT_EQUALS(m_MemData->GetVertexCount(), 0);
		wxRealPoint myPt1 (1.1,2.2);
		wxRealPoint myPt2 (3.3,4.4);
		wxRealPoint myPt3 (5.5,6.6);
		wxRealPoint myPt4 (7.7,8.8);
		wxRealPoint myPt5 (9.9, 10.10);
		wxRealPoint myPt6 (11.11,12.12);
		
		TS_ASSERT(m_MemData->InsertVertex(myPt1)==false);
		TS_ASSERT(m_MemData->CreateFeature());
	
		TS_ASSERT(m_MemData->InsertVertex(myPt1)==true);
		TS_ASSERT(m_MemData->InsertVertex(myPt2)==true);
		TS_ASSERT(m_MemData->InsertVertex(myPt3)==true);
		TS_ASSERT(m_MemData->InsertVertex(myPt4)==true);
		TS_ASSERT(m_MemData->InsertVertex(myPt5)==true);
		TS_ASSERT(m_MemData->InsertVertex(myPt6)==true);		
	}
	
	void tearDown(){
		delete m_MemData;
	}
	

	void testName(){
		wxLogMessage(_T("------------------------------------"));
		wxLogMessage(_T("--- TESTING TMGISDATAVECTORMEMORY---"));
		wxLogMessage(_T("------------------------------------"));
	}
	
	void testCountVertex(){
		TS_ASSERT_EQUALS(m_MemData->GetVertexCount(), 6);
	}
	
	void testGetLastVertex(){
		wxRealPoint myPt (0.0,0.0);
		TS_ASSERT(m_MemData->GetVertex(myPt, -1));
		TS_ASSERT_EQUALS(myPt, wxRealPoint(11.11,12.12));
		TS_ASSERT(m_MemData->GetVertex(myPt, -1));
		TS_ASSERT_EQUALS(myPt, wxRealPoint(11.11,12.12));

		TS_ASSERT(m_MemData->GetVertex(myPt, 0));
		TS_ASSERT_EQUALS(myPt, wxRealPoint(1.1,2.2));

				
	}
	
	void testRemovingLastVertex(){
		wxLogMessage(_T("Getting vertex for gis data in memory"));
		
		wxRealPoint myPt (0.0,0.0);
		TS_ASSERT(m_MemData->GetVertex(myPt, -1));
		TS_ASSERT_EQUALS(myPt, wxRealPoint(11.11,12.12));
		TS_ASSERT(m_MemData->RemoveVertex());
		
		TS_ASSERT(m_MemData->GetVertex(myPt, -1));
		TS_ASSERT_EQUALS(myPt, wxRealPoint(9.9,10.10));
		TS_ASSERT(m_MemData->RemoveVertex());
		
		TS_ASSERT(m_MemData->GetVertex(myPt, -1));
		TS_ASSERT_EQUALS(myPt, wxRealPoint(7.7,8.8));
		TS_ASSERT(m_MemData->RemoveVertex());
		
		TS_ASSERT(m_MemData->GetVertex(myPt, -1));
		TS_ASSERT_EQUALS(myPt, wxRealPoint(5.5,6.6));
		TS_ASSERT(m_MemData->RemoveVertex());
		
		TS_ASSERT(m_MemData->GetVertex(myPt, -1));
		TS_ASSERT_EQUALS(myPt, wxRealPoint(3.3,4.4));
		TS_ASSERT(m_MemData->RemoveVertex());
		
		TS_ASSERT(m_MemData->GetVertex(myPt, -1));
		TS_ASSERT_EQUALS(myPt, wxRealPoint(1.1,2.2));
		wxLogMessage(_T("First vertex is %f - %f"), myPt.x, myPt.y);
		
		TS_ASSERT(m_MemData->RemoveVertex()==true);
		TS_ASSERT(m_MemData->RemoveVertex()==false);
	}
	
	
	
		
};




#endif
