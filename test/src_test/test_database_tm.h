/***************************************************************************
								test_database_tm.h
							Testing Database TM class
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

#ifndef _TEST_DATABASE_TM_H_
#define _TEST_DATABASE_TM_H_


#include "wx/wxprec.h"
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <cxxtest/TestSuite.h>

#include "test_param.h"
#include "../../src/database/database_tm.h"



class TEST_DatabaseTM : public CxxTest::TestSuite
{
public:
	DataBaseTM * m_DB;	

	void setUp()
	{
		wxApp::SetInstance(new wxAppConsole());
		m_DB = new DataBaseTM();

	}
	
	void tearDown() 
	{
		delete m_DB;
	}
	
	void testName(){
		wxLogMessage(_T("------------------------------------"));
		wxLogMessage(_T("-------- TESTING DATABASE_TM -------"));
		wxLogMessage(_T("------------------------------------"));
	}
	
	void testTableExist()
	{
		TS_ASSERT(m_DB->DataBaseOpen(g_TestPathPRJ, g_TestPrj_Edit));
		TS_ASSERT(m_DB->TableExist(_T("generic_lines")));
		TS_ASSERT_EQUALS(m_DB->TableExist(_T("generic_linesss")),false);
		TS_ASSERT(m_DB->TableExist(_T("dmn_layer_object")));
		TS_ASSERT(m_DB->TableExist(_T("prj_settings")));
	}
	
	void testToolMapVersion()
	{
		TS_ASSERT(m_DB->DataBaseOpen(g_TestPathPRJ, g_TestPrj_Edit));
		TS_ASSERT_EQUALS(m_DB->GetDatabaseToolMapVersion(),TM_DATABASE_VERSION);
	}
	
	void testCreateNewTMDatabase()
	{
		TS_ASSERT(m_DB->DataBaseOpen(g_TestPathPRJ, g_TestPrj_Edit));
		// deleting if existing
		if(m_DB->DataBaseOpen(g_TestPathPRJ, g_TestPrj_Edit13))
		{	
			wxLogDebug(wxT("Deleting database : %s"), m_DB->DataBaseGetName().c_str());
			TS_ASSERT(m_DB->DataBaseDelete()==true);
		}
		
		PrjDefMemManage myPrjdef;
		myPrjdef.m_PrjName = g_TestPrj_Edit;
		myPrjdef.m_PrjPath = g_TestPathPRJ;
		myPrjdef.m_PrjProjType = PROJ_SWISSPROJ;
		myPrjdef.m_PrjUnitType = UNIT_METERS;
		
		TS_ASSERT_EQUALS(m_DB->CreateTMDatabase(&myPrjdef),false);
		myPrjdef.m_PrjName = g_TestPrj_Edit13;
		TS_ASSERT(m_DB->CreateTMDatabase(&myPrjdef)==true);
		TS_ASSERT_EQUALS(m_DB->OpenTMDatabase(myPrjdef.m_PrjPath + 
											wxFileName::GetPathSeparator() + 
											myPrjdef.m_PrjName), tmDB_OPEN_OK);
	}
	
	void testOpenTMDatabase()
	{
		TS_ASSERT_EQUALS(m_DB->OpenTMDatabase(g_TestPathPRJ + g_TestPrj_Edit), tmDB_OPEN_OK);
		TS_ASSERT(m_DB->OpenTMDatabase(g_TestPathPRJ + g_TestPrj_MyTest)>=tmDB_OPEN_FAILED_NOT_TM_DB);
		int myStatus = m_DB->OpenTMDatabase(g_TestPathPRJ + g_TestPrj_LuganoTM);
		TS_ASSERT_LESS_THAN_EQUALS(tmDB_OPEN_FAILED_WRONG_VERSION, myStatus);
		wxLogError(_("%s is wrong version %d"),g_TestPrj_LuganoTM.c_str(), myStatus);
		TS_ASSERT(m_DB->GetDatabaseToolMapVersion() != TM_DATABASE_VERSION);
	}
	
	void testConvertPath()
	{
		TS_ASSERT(m_DB->DataBaseOpen(g_TestPathPRJ, g_TestPrj_Edit));
		wxString myPath = _T("C:\\Test\\");
		wxString myUnixPath = _T("User/share/test");
		wxString myConvertedPath = _T("C:\\\\Test\\\\");
#ifdef __WINDOWS__
		TS_ASSERT_EQUALS(DataBaseTM::ConvertPath(myPath),true);
		TS_ASSERT_EQUALS(DataBaseTM::ConvertPath(myUnixPath),true);
#else
		TS_ASSERT_EQUALS(DataBaseTM::ConvertPath(myPath),false);
		TS_ASSERT_EQUALS(DataBaseTM::ConvertPath(myUnixPath),false);
#endif
	}
	
	
	
	
};




#endif
