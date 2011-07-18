/***************************************************************************
 test_tmstatsrecord.h
 UNIT TESTING for tmstatsrecord
 -------------------
 copyright            : (C) 2011 CREALP Lucien Schreiber 
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

#ifndef _TM_TEST_TM_STATS_RECORD_H_
#define _TM_TEST_TM_STATS_RECORD_H_


#include "wx/wxprec.h"
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <cxxtest/TestSuite.h>

#include "test_param.h"
#include "../../src/core/tmstatsrecord.h"
#include "../../src/database/database_tm.h"
#include "../../src/core/tmstats.h"

class TEST_tmStatsRecords : public CxxTest::TestSuite
{
public:
	DataBaseTM * m_pDB;

	TEST_tmStatsRecords (bool bTest){
		m_pDB = new DataBaseTM();
		TS_ASSERT(m_pDB->OpenTMDatabase(g_TestPathPRJ + g_TestPrj_Stats));
	}
	
	
	virtual ~TEST_tmStatsRecords(){
		wxDELETE(m_pDB);
	}
	
	static TEST_tmStatsRecords *createSuite() { return new TEST_tmStatsRecords(true);}
    static void destroySuite( TEST_tmStatsRecords *suite ) { delete suite; }

	
	void setUp()
	{
	}
	
	void tearDown(){
	}
	

	void testName(){
		wxLogMessage(_T("------------------------------------"));
		wxLogMessage(_T("------- TESTING TMSTATSRECORDS -----"));
		wxLogMessage(_T("------------------------------------"));
	}
	
	void testCreate(){
		tmStatsRecords myRecord(m_pDB);
		tmStatsData myData;
		long myRecordId = myRecord.Create(myData);
		TS_ASSERT_EQUALS(myRecordId, wxNOT_FOUND);
		
		myData.m_TimeStart = wxDateTime::Now();
		myRecordId = myRecord.Create(myData);
		TS_ASSERT_DIFFERS(myRecordId, wxNOT_FOUND);
		wxLogMessage("Adding stat record with id = %ld", myRecordId);
	}
	
	void testAddRemove(){
		tmStatsRecords myRecord(m_pDB);
		tmStatsData myData;
		myData.m_TimeStart = wxDateTime::Now();
		myData.m_NbClick = 1;
		myData.m_NbAttribution = 2;
		myData.m_NbIntersection = 3;
		
		long myRecordId = myRecord.Create(myData);
		TS_ASSERT_DIFFERS(myRecordId, wxNOT_FOUND);
		TS_ASSERT_EQUALS(myRecord.Add(myRecordId, myData), true);
		TS_ASSERT_EQUALS(myRecord.Delete(myRecordId), true);
		wxLogMessage("Adding and removing stat record with id = %ld", myRecordId);
	}
	
	void testLoad(){
		// id = 1 for first record
		tmStatsRecords myRecord(m_pDB);
		tmStatsData myData;
		
		TS_ASSERT_EQUALS(myRecord.Load(1, myData),true);
		wxLogMessage("time elapsed is: %s", myData.m_TimeElapsed.Format());
		
	}
	
	
		
};




#endif
