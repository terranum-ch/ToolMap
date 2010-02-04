/***************************************************************************
 test_tmdataintegrity.h
 UNIT TESTING for data integrity
 -------------------
 copyright            : (C) 2010 CREALP Lucien Schreiber 
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

#ifndef _TM_TEST_DATA_INTEGRITY_H_
#define _TM_TEST_DATA_INTEGRITY_H_


#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <cxxtest/TestSuite.h>

#include "test_param.h"
#include "../../src/database/database_tm.h"
#include "../../src/core/tmdataintegrity.h"

class TEST_tmDataIntegrity : public CxxTest::TestSuite
{
public:
	DataBaseTM * m_pDB;
	
	TEST_tmDataIntegrity (bool bTest){
		m_pDB = new DataBaseTM();
		TS_ASSERT(m_pDB->OpenTMDatabase(g_TestPathPRJ + g_TestPrj_Ricken));
	}
	
	virtual ~TEST_tmDataIntegrity(){
		delete m_pDB;
	}
	
	static TEST_tmDataIntegrity *createSuite() { return new TEST_tmDataIntegrity(true);}
    static void destroySuite( TEST_tmDataIntegrity *suite ) { delete suite; }
	
	
	
	void testName(){
		wxLogMessage(_T("------------------------------------"));
		wxLogMessage(_T("--- TESTING TM DATA INTEGRITY   ---"));
		wxLogMessage(_T("------------------------------------"));
	}
	

	void testCreate(){
		tmDataIntegrity di(m_pDB);
		TS_ASSERT(di.GetLastError()==wxEmptyString);
	}
	
	void testHasErrorAAttrib(){
			
		tmDataIntegrity di(m_pDB);
		TS_ASSERT_EQUALS(di.HasErrorAAttrib(19, LAYER_POINT),true); // WatOutflow_PT with one error
		wxLogMessage(di.GetLastError());
		TS_ASSERT(di.GetLastError()!=wxEmptyString);
		
		
		TS_ASSERT_EQUALS(di.HasErrorAAttrib(12, LAYER_LINE),false); // Gravifeat_L with no error
		wxLogMessage(di.GetLastError());
		TS_ASSERT(di.GetLastError()==wxEmptyString);

	}
	
	
	void testHasErrorType(){
		tmDataIntegrity di(m_pDB);

		
		TS_ASSERT_EQUALS(di.HasErrorType(LAYER_LINE),true); // Error in lines 
		wxLogMessage(di.GetLastError());
		TS_ASSERT(di.GetLastError()!=wxEmptyString);
		
		TS_ASSERT_EQUALS(di.HasErrorType(LAYER_POLYGON),false); // no error in labels
		wxLogMessage(di.GetLastError());
		TS_ASSERT(di.GetLastError()==wxEmptyString);
		
		TS_ASSERT_EQUALS(di.HasErrorType(LAYER_POINT),false); // no error in points
		wxLogMessage(di.GetLastError());
		TS_ASSERT(di.GetLastError()==wxEmptyString);
	}
	
	
	void testCorrectType()
	{
		tmDataIntegrity di(m_pDB);
		TS_ASSERT(di.CorrectType(LAYER_LINE));
		TS_ASSERT_EQUALS(di.HasErrorType(LAYER_LINE), false);

	}
	
	void testCorrectAAttrib(){
		
		tmDataIntegrity di(m_pDB);
		TS_ASSERT(di.CorrectAAttrib(19, LAYER_POINT));
		TS_ASSERT_EQUALS(di.HasErrorAAttrib(19, LAYER_POINT), false);
		
		
	}
	
	
	
	void testAddErrorIntoDatabase(){
		
		// inserting one error into DB (will be corrected later)
		wxString myQuery = _T("INSERT INTO layer_at19 VALUES (92, 'gefasst')");
		TS_ASSERT(m_pDB->DataBaseQueryNoResults(myQuery));
		
		
		wxString myQuery2 = _T("INSERT INTO generic_aat VALUES (93, 2)");
		TS_ASSERT(m_pDB->DataBaseQueryNoResults(myQuery2));
	}
				  
				  
	
	
};




#endif
