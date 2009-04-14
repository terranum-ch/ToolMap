/***************************************************************************
								database_test.cpp
							UNIT TEST for database
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

#ifndef _DATABASE_TEST_CPP_
#define _DATABASE_TEST_CPP_

#include "../../src/database/database.h"

#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>


class DataBaseTEST : public CppUnit::TestFixture 
{ 
	CPPUNIT_TEST_SUITE( DataBaseTEST );
	CPPUNIT_TEST( TESTOpenDatabase );
	CPPUNIT_TEST( TESTResults );
	CPPUNIT_TEST( TESTQueryNoResults );
	CPPUNIT_TEST_SUITE_END();
	
private:
	DataBase * m_DB;	
	
public: 
	void setUp()
	{
		m_DB = new DataBase();
		
		//m_DB1 = new tmoDatabase();
		//CPPUNIT_ASSERT( m_DB1->DatabaseOpen("--datadir=/Users/Lucien/DATA/SIG/COMBIOULA/EXERCICE/", 
		//	"combioula_exercice")== true);
		
		//m_DB2 = new tmoDatabase();
	}
	
	void tearDown() 
	{
		delete m_DB;
	}
	
	void TESTOpenDatabase()
	{
		CPPUNIT_ASSERT(m_DB->DataBaseOpen(_T("/Users/Lucien/DATA/SIG/COMBIOULA/CORRIGE/TOOLMAP/"),
										  _T("combioula_correct")) == true);
		CPPUNIT_ASSERT( m_DB->DataBaseOpen(_T("/Users/Lucien/DATA/SIG/COMBIOULA/EXERCICESSSSS"),
						   _T("combioula_exercice"))==false);
		CPPUNIT_ASSERT( m_DB->DataBaseOpen(_T("/Users/Lucien/DATA/SIG/COMBIOULA/EXERCICE"),
										   _T("combioula_exercice"))==true);
	}
	
	void TESTResults ()
	{
		CPPUNIT_ASSERT( m_DB->DataBaseHasResults() == false);
		CPPUNIT_ASSERT(m_DB->DataBaseOpen(_T("/Users/Lucien/DATA/SIG/COMBIOULA/CORRIGE/TOOLMAP/"),
										  _T("combioula_correct"))==true);
		CPPUNIT_ASSERT(m_DB->DataBaseQuery(_T("SELECT OBJECT_ID FROM generic_lines"))==true);
		CPPUNIT_ASSERT( m_DB->DataBaseHasResults() == true);
		
	}
	
	void TESTQueryNoResults ()
	{
		
		CPPUNIT_ASSERT(m_DB->DataBaseQueryNoResults(_T("SELECT OBJECT_ID FROM generic_lines"))==false);
		CPPUNIT_ASSERT(m_DB->DataBaseOpen(_T("/Users/Lucien/DATA/SIG/COMBIOULA/CORRIGE/TOOLMAP/"),
						   _T("combioula_correct"))==true);
		CPPUNIT_ASSERT(m_DB->DataBaseQueryNoResults(_T("SELECT OBJECT_ID FROM generic_lines"))==true);
		CPPUNIT_ASSERT(m_DB->DataBaseQueryNoResults(_T("SELECT COUNT(*) FROM generic_linesss"))==false);
	}
	
	/*
	 void testDatabaseQueryNoResults()
	 {
	 CPPUNIT_ASSERT (m_DB1->DatabaseQueryNoResults("SELECT COUNT(*) FROM generic_lines") == true);
	 }
	 
	 void testDatabaseResults()
	 {
	 CPPUNIT_ASSERT(m_DB1->DatabaseHasResults() == false);
	 CPPUNIT_ASSERT(m_DB1->DatabaseQuery("SELECT COUNT(*) FROM generic_lines")==true);
	 CPPUNIT_ASSERT(m_DB1->DatabaseHasResults() == true);
	 m_DB1->DatabaseClearResults();
	 CPPUNIT_ASSERT(m_DB1->DatabaseQuery("SELECT COUNT(*) FROM generic_aat")==true);
	 }*/
	
	
	
};

#endif



