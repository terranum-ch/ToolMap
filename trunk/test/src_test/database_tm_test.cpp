/***************************************************************************
								database_tm_test.cpp
							Test for database_tm class
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

#ifndef _DATABASE_TM_TEST_CPP_
#define _DATABASE_TM_TEST_CPP_


#include "../../src/database/database_tm.h"

#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>
#include <wx/filefn.h> 

class DataBaseTMTEST : public CppUnit::TestFixture 
{ 
	CPPUNIT_TEST_SUITE( DataBaseTMTEST );
	CPPUNIT_TEST ( TESTTableExist );
	CPPUNIT_TEST ( TESTToolMapVersion );
	CPPUNIT_TEST ( TESTOpenTMDatabase );
	CPPUNIT_TEST ( TESTCreateNewTMDatabase );
	CPPUNIT_TEST ( TESTConvertPath );
	CPPUNIT_TEST_SUITE_END();
	
private:
	DataBaseTM * m_DB;	
	
public: 
	void setUp()
	{
		m_DB = new DataBaseTM();
		CPPUNIT_ASSERT(m_DB->DataBaseOpen(_T("/Users/Lucien/Downloads/"), _T("testedit")));
	}
	
	void tearDown() 
	{
		delete m_DB;
	}
	
	void TESTTableExist()
	{
		CPPUNIT_ASSERT(m_DB->TableExist(_T("generic_lines")));
		CPPUNIT_ASSERT(m_DB->TableExist(_T("generic_linesss"))==false);
		CPPUNIT_ASSERT(m_DB->TableExist(_T("dmn_layer_object")));
		CPPUNIT_ASSERT(m_DB->TableExist(_T("prj_settings")));
	}
	
	void TESTToolMapVersion()
	{
		CPPUNIT_ASSERT(m_DB->GetDatabaseToolMapVersion()==220);
	}
	
	void TESTCreateNewTMDatabase()
	{
		// deleting if existing
		if(m_DB->DataBaseOpen(_T("/Users/Lucien/Downloads/"), _T("testedit_13")))
		{	
			wxLogDebug(_T("Deleting database : %s"), m_DB->DataBaseGetName().c_str());
			CPPUNIT_ASSERT(m_DB->DataBaseDelete()==true);
		}
			
		PrjDefMemManage myPrjdef;
		myPrjdef.m_PrjName = _T("testedit");
		myPrjdef.m_PrjPath = _T("/Users/Lucien/Downloads/");
		myPrjdef.m_PrjProjType = PROJ_SWISSPROJ;
		myPrjdef.m_PrjUnitType = UNIT_LATLONG;
		
		CPPUNIT_ASSERT(m_DB->CreateTMDatabase(&myPrjdef)==false);
		myPrjdef.m_PrjName = _T("testedit_13");
		CPPUNIT_ASSERT(m_DB->CreateTMDatabase(&myPrjdef)==true);
		CPPUNIT_ASSERT(m_DB->OpenTMDatabase(myPrjdef.m_PrjPath + 
											wxFileName::GetPathSeparator() + 
											myPrjdef.m_PrjName)==tmDB_OPEN_OK);
	}
	
	void TESTOpenTMDatabase()
	{
		CPPUNIT_ASSERT(m_DB->OpenTMDatabase(_T("/Users/Lucien/Downloads/testedit"))==tmDB_OPEN_OK);
		CPPUNIT_ASSERT(m_DB->OpenTMDatabase(_T("/Users/Lucien/Downloads/mytest1"))==tmDB_OPEN_FAILED_NOT_TM_DB);
		tmDB_OPEN_STATUS myStatus = m_DB->OpenTMDatabase(_T("/Users/Lucien/DATA/SIG/LUGANO/TM_Project/luganoTM"));
		CPPUNIT_ASSERT(myStatus == tmDB_OPEN_FAILED_WRONG_VERSION);
		CPPUNIT_ASSERT(m_DB->GetDatabaseToolMapVersion() != TM_DATABASE_VERSION);
	}
	
	void TESTConvertPath()
	{
		wxString myPath = _T("C:\\Test\\");
		wxString myUnixPath = _T("User/share/test");
		wxString myConvertedPath = _T("C:\\\\Test\\\\");
		
		CPPUNIT_ASSERT(DataBaseTM::ConvertPath(myPath)==false);
		CPPUNIT_ASSERT(DataBaseTM::ConvertPath(myUnixPath)==false);
	}
	
	
};

#endif

