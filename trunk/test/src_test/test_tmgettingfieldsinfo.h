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
	PrjDefMemManage * m_Proj;
	
	TEST_GettingFieldsInfo (bool bTest){
		m_pDB = new DataBaseTM();
		TS_ASSERT(m_pDB->OpenTMDatabase(g_TestPathPRJ + g_TestPrj_AdvAttribution));
		tmGISData::InitGISDrivers(true,true);
		
		// open project for all tests
		m_Proj = m_pDB->GetProjectDataFromDB();
		TS_ASSERT(m_Proj);
		

	}
	
	virtual ~TEST_GettingFieldsInfo(){
		delete m_pDB;
		delete m_Proj;
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
	
	
	void testGettingFieldsName2ForMySQL(){
		wxFileName mySQLFileName (g_TestPathPRJ + wxFileName::GetPathSeparator() +
								  g_TestPrj_AdvAttribution, _T("generic_lines.MYD"));
		TS_ASSERT(mySQLFileName.FileExists()==true);
		wxLogMessage(_T("Opening file : ") + mySQLFileName.GetFullPath());
		
		// opening layer
		TS_ASSERT(m_pDB);
		tmGISDataVector * myVect = tmGISDataVector::CreateGISVectorBasedOnExt(_T("mysql"));
		TS_ASSERT(myVect != NULL);
		TS_ASSERT(myVect->GetDataType()==tmGIS_VECTOR_MYSQL);
		
		tmGISDataVectorMYSQL::SetDataBaseHandle(m_pDB);
		TS_ASSERT_EQUALS (myVect->Open(mySQLFileName.GetFullPath(), true),true);
		
		// getting fields failed because no project specified
		wxArrayString myFieldsName;
		TS_ASSERT(myVect->GetFieldsName(myFieldsName, 9)==false);
		
		// specifiy project object
		((tmGISDataVectorMYSQL *)myVect)->SetProject(m_Proj);
		
		// getting fields ok because project specified
		TS_ASSERT(myVect->GetFieldsName(myFieldsName, 9)==true);
		int iTotVal = myFieldsName.GetCount();
		for (int i = 0; i< iTotVal; i++) {
			wxLogMessage(_T("Fields name %d = %s"), i, myFieldsName.Item(i).c_str());
		}
		TS_ASSERT_EQUALS(iTotVal, 10);
		wxDELETE(myVect);
		
		
	}
	
	void testGettingFieldsNameForMySQL(){
		/*// MySQL layer to open
		wxFileName mySQLFileName (g_TestPathPRJ + wxFileName::GetPathSeparator() +
								  g_TestPrj_AdvAttribution, _T("generic_lines.MYD"));
		TS_ASSERT(mySQLFileName.FileExists()==true);
		wxLogMessage(_T("Opening file : ") + mySQLFileName.GetFullPath());
		
		// opening layer
		TS_ASSERT(m_pDB);
		tmGISDataVector * myVect = tmGISDataVector::CreateGISVectorBasedOnExt(_T("mysql"));
		TS_ASSERT(myVect != NULL);
		tmGISDataVectorMYSQL::SetDataBaseHandle(m_pDB);
		TS_ASSERT_EQUALS (myVect->Open(mySQLFileName.GetFullPath(), true),true);
		
		// getting fields name for line with 3 attributions
		wxArrayString myFieldsName;
		TS_ASSERT_EQUALS(myFieldsName.GetCount(),0);
		TS_ASSERT(myVect->GetFieldsName(myFieldsName, 9)==true);
		
		int iTotVal = myFieldsName.GetCount();
		for (int i = 0; i< iTotVal; i++) {
			wxLogMessage(_T("Fields name %d = %s"), i, myFieldsName.Item(i).c_str());
		}
		TS_ASSERT_EQUALS(myFieldsName.GetCount(), 10);
		
		
		// getting fields name for line with 0 attributions
		TS_ASSERT(myVect->GetFieldsName(myFieldsName, 12)==true);
		
		iTotVal = myFieldsName.GetCount();
		for (int i = 0; i< iTotVal; i++) {
			wxLogMessage(_T("Fields name %d = %s"), i, myFieldsName.Item(i).c_str());
		}
		TS_ASSERT_EQUALS(myFieldsName.GetCount(), 0);
		wxDELETE(myVect);*/
	}
	
	
	void testGettingFieldsValueForMySQL(){
		// MySQL layer to open
		wxFileName mySQLFileName (g_TestPathPRJ + wxFileName::GetPathSeparator() +
								  g_TestPrj_AdvAttribution, _T("generic_lines.MYD"));
		TS_ASSERT(mySQLFileName.FileExists()==true);
		wxLogMessage(_T("Opening file : ") + mySQLFileName.GetFullPath());
		
		// opening layer
		TS_ASSERT(m_pDB);
		tmGISDataVector * myVect = tmGISDataVector::CreateGISVectorBasedOnExt(_T("mysql"));
		TS_ASSERT(myVect != NULL);
		tmGISDataVectorMYSQL::SetDataBaseHandle(m_pDB);
		TS_ASSERT_EQUALS (myVect->Open(mySQLFileName.GetFullPath(), true),true);
		
		// getting fields value
		wxArrayString myFieldsValue;
		TS_ASSERT_EQUALS(myFieldsValue.GetCount(),0);
		TS_ASSERT(myVect->GetFieldsValue(myFieldsValue, 9)==true);
		
		int iTotVal = myFieldsValue.GetCount();
		for (int i = 0; i< iTotVal; i++) {
			wxLogMessage(_T("Fields value %d = %s"), i, myFieldsValue.Item(i).c_str());
		}
		TS_ASSERT_EQUALS(myFieldsValue.GetCount(), 10);
		
		
		wxDELETE(myVect);
	}
	
	
};




#endif
