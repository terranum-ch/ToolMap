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
	
	void testCheckAAttrib(){
		tmDataIntegrity di(m_pDB);
		TS_ASSERT_EQUALS(di.CheckAAttrib(19, TOC_NAME_POINTS),false); // WatOutflow_PT with one error
		wxLogMessage(di.GetLastError());
		TS_ASSERT(di.GetLastError()!=wxEmptyString);
		
		
		TS_ASSERT_EQUALS(di.CheckAAttrib(12, TOC_NAME_LINES),true); // Gravifeat_L with no error
		wxLogMessage(di.GetLastError());
		TS_ASSERT(di.GetLastError()==wxEmptyString);

	}
	
	
	void testCheckType(){
		tmDataIntegrity di(m_pDB);

		
		TS_ASSERT_EQUALS(di.CheckType(TOC_NAME_LINES),false); // Error in lines 
		wxLogMessage(di.GetLastError());
		TS_ASSERT(di.GetLastError()!=wxEmptyString);
	}
	
	
	
};




#endif
