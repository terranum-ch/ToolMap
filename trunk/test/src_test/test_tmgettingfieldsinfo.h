/***************************************************************************
 test_tmgettingfieldsinfo.h
 UNIT TESTING for Getting fields info
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

#ifndef _TM_TEST_GETTING_FIELDS_INFO_H_
#define _TM_TEST_GETTING_FIELDS_INFO_H_


#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <cxxtest/TestSuite.h>

#include "test_param.h"
#include "../../src/database/database_tm.h"


class TEST_GettingFieldsInfo : public CxxTest::TestSuite
{
public:
	DataBaseTM * m_pDB;
	
	TEST_GettingFieldsInfo (bool bTest){
		m_pDB = new DataBaseTM();
		TS_ASSERT(m_pDB->OpenTMDatabase(g_TestPathPRJ + g_TestPrj_Ricken));
		tmGISData::InitGISDrivers(true,true);

	}
	
	virtual ~TEST_GettingFieldsInfo(){
		delete m_pDB;
	}
	
	static TEST_GettingFieldsInfo *createSuite() { return new TEST_GettingFieldsInfo(true);}
    static void destroySuite( TEST_GettingFieldsInfo *suite ) { delete suite; }
	
	
	
	void testName(){
		wxLogMessage(_T("------------------------------------"));
		wxLogMessage(_T("--- TESTING TM GETTING FIELD INFO---"));
		wxLogMessage(_T("------------------------------------"));
	}
	

	void testGettingFieldNameForShapefiles(){
		
		wxFileName myVectName (g_TestPathSHP, g_TestFileSHP_PLG);
		tmGISDataVector * myVect = tmGISDataVector::CreateGISVectorBasedOnExt(myVectName.GetExt());
		TS_ASSERT(myVect != NULL);
		TS_ASSERT(myVect->Open(myVectName.GetFullPath(), false)==true);

		
		wxArrayString myFieldsValue;
		TS_ASSERT_EQUALS(myFieldsValue.GetCount(), 0);
		
		TS_ASSERT(myVect->GetFieldsName(myFieldsValue)==true);
		TS_ASSERT_EQUALS(myFieldsValue.GetCount(), 4);
		
		TS_ASSERT(myVect->GetFieldsName(myFieldsValue)==true);
		TS_ASSERT_EQUALS(myFieldsValue.GetCount(), 4);
		

		wxDELETE(myVect);
		
	}
		
	
	
	void testGettingFieldsValueForShapefiles(){
		wxFileName myVectName (g_TestPathSHP, g_TestFileSHP_PLG);
		tmGISDataVector * myVect = tmGISDataVector::CreateGISVectorBasedOnExt(myVectName.GetExt());
		TS_ASSERT(myVect != NULL);
		TS_ASSERT(myVect->Open(myVectName.GetFullPath(), false)==true);
		
		wxArrayString myValues;
		TS_ASSERT(myVect->GetFieldsValue(myValues, 1)==true);
		TS_ASSERT_EQUALS(myValues.GetCount(),4);
		int iTotVal = myValues.GetCount();
		for (int i = 0; i< iTotVal; i++) {
			wxLogMessage(_T("Fields value %d = %s"), i, myValues.Item(i).c_str());
		}
		
		wxDELETE(myVect);
	}
	
};




#endif
