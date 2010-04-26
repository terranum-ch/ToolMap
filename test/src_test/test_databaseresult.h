/***************************************************************************
								TEST_DatabaseResults.h
							Testing Database class
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

#ifndef _TEST_DATABASE_RESULTS_H_
#define _TEST_DATABASE_RESULTS_H_


#include "wx/wxprec.h"
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <cxxtest/TestSuite.h>

#include "test_param.h"

#include "../../src/database/database_tm.h"
#include "../../src/database/databaseresult.h"



class TEST_DatabaseResults : public CxxTest::TestSuite
{
public:
	DataBaseTM * m_pDB;
	
	TEST_DatabaseResults (bool bTest){
		m_pDB = new DataBaseTM();
		TS_ASSERT(m_pDB->OpenTMDatabase(g_TestPathPRJ + g_TestPrj_AdvAttribution));
	}
	
	virtual ~TEST_DatabaseResults(){
		delete m_pDB;
	}
	
	static TEST_DatabaseResults *createSuite() { return new TEST_DatabaseResults(true);}
    static void destroySuite( TEST_DatabaseResults *suite ) { delete suite; }
	
	
	
	void testName(){
		wxLogMessage(_T("------------------------------------"));
		wxLogMessage(_T("--- TESTING DATABASE RESULTS   ---"));
		wxLogMessage(_T("------------------------------------"));
	}
	
	
	void testCreatorDtor(){
		TS_ASSERT(m_pDB->DataBaseQuery(_T("SELECT * FROM zoom_level"))==true);
		DataBaseResult * myRes = new DataBaseResult();
		TS_ASSERT(m_pDB->DataBaseGetResults(myRes)==true);
		delete myRes;
		TS_ASSERT(m_pDB->DataBaseHasResults() == false);
		
	}
	
	void testGetCount(){
		TS_ASSERT(m_pDB->DataBaseQuery(_T("SELECT * FROM zoom_level"))==true);
		DataBaseResult myRes;
		TS_ASSERT(m_pDB->DataBaseGetResults(&myRes)==true);
		TS_ASSERT_EQUALS(myRes.GetColCount(), 3);
		TS_ASSERT_EQUALS(myRes.GetRowCount(), 4);
	}
	
	void testHasResults(){
		TS_ASSERT(m_pDB->DataBaseQuery(_T("SELECT * FROM zoom_level WHERE ZOOM_ID=100"))==true);
		DataBaseResult myRes;
		TS_ASSERT(m_pDB->DataBaseGetResults(&myRes)==true);
		TS_ASSERT(myRes.HasResults() == false);
		
		// no result inited !
		DataBaseResult myRes2;
		TS_ASSERT(myRes2.HasResults()==false);
	}
	
	void testGetColName(){
		TS_ASSERT(m_pDB->DataBaseQuery(_T("SELECT * FROM zoom_level"))==true);
		DataBaseResult myRes;
		TS_ASSERT(m_pDB->DataBaseGetResults(&myRes)==true);
		wxArrayString myColName;
	 	TS_ASSERT(myRes.GetColName(myColName)==true);
		TS_ASSERT_EQUALS(myColName.GetCount(), 3);
		for (unsigned int i = 0; i< myColName.GetCount(); i++) {
			wxLogMessage(_T("Col name : ") + myColName.Item(i));
		}
		
	}
	
	void testIsRowOk(){
		TS_ASSERT(m_pDB->DataBaseQuery(_T("SELECT * FROM zoom_level"))==true);
		DataBaseResult myRes;
		TS_ASSERT(m_pDB->DataBaseGetResults(&myRes)==true);
		TS_ASSERT(myRes.IsRowOk() == false);
		TS_ASSERT(myRes.NextRow() == true);
		TS_ASSERT(myRes.IsRowOk() == true);
		
		
	}
	
	
	void testGetValue(){
		TS_ASSERT(m_pDB->DataBaseQuery(_T("SELECT * FROM zoom_level"))==true);
		DataBaseResult myRes;
		TS_ASSERT(m_pDB->DataBaseGetResults(&myRes)==true);
		TS_ASSERT(myRes.NextRow() == true);
		
		wxString myValue = wxEmptyString;
		TS_ASSERT(myRes.GetValue(1, myValue)==true);
		TS_ASSERT(myValue == _T("5000"));
		wxLogMessage(_T("Value for col 1 is ") + myValue);
		TS_ASSERT(myRes.NextRow()==true);
		TS_ASSERT(myRes.GetValue(1, myValue)==true);
		TS_ASSERT(myValue == _T("10000"));
		
		TS_ASSERT(myRes.NextRow()==true);	
		TS_ASSERT(myRes.NextRow()==true);
		TS_ASSERT(myRes.NextRow()==false);
		
	}
	
	void testGetValueLong(){
		TS_ASSERT(m_pDB->DataBaseQuery(_T("SELECT * FROM thematic_layers WHERE LAYER_INDEX = 1"))==true);
		DataBaseResult myRes;
		TS_ASSERT(m_pDB->DataBaseGetResults(&myRes)==true);
		TS_ASSERT(myRes.NextRow() == true);
		
		long myValueLong;
		TS_ASSERT(myRes.GetValue(0, myValueLong)==true);
		TS_ASSERT_EQUALS(myValueLong, 1);

		wxString myValueStr = wxEmptyString;
		TS_ASSERT(myRes.GetValue(2, myValueStr)==true);
		TS_ASSERT(myValueStr == _T("TectoBound_L"));
		
		// trying to get string to long
		TS_ASSERT(myRes.GetValue(2, myValueLong)==false)

	}
	
	
	void testGetValueGeom(){
		TS_ASSERT(m_pDB->DataBaseQuery(_T("SELECT * FROM generic_lines WHERE OBJECT_ID = 13"))==true);
		DataBaseResult myRes;
		TS_ASSERT(m_pDB->DataBaseGetResults(&myRes)==true);
		TS_ASSERT(myRes.NextRow() == true);
		
		OGRGeometry * myGeom = NULL;
		TS_ASSERT(myRes.GetValue(1, &myGeom)==true);
		TS_ASSERT(myGeom != NULL);
		TS_ASSERT(wkbFlatten(myGeom->getGeometryType()) == wkbLineString);
		wxLogMessage(_T("Returned line has %d vertex"), 
					 ((OGRLineString*) myGeom)->getNumPoints());
		OGRGeometryFactory::destroyGeometry (myGeom);
		
		
	}
	
	
	
};




#endif
