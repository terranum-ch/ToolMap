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

class DataBaseTMTEST : public CppUnit::TestFixture 
{ 
	CPPUNIT_TEST_SUITE( DataBaseTMTEST );
	CPPUNIT_TEST ( TESTTableExist );
	CPPUNIT_TEST ( TESTToolMapVersion );
	CPPUNIT_TEST_SUITE_END();
	
private:
	DataBaseTM * m_DB;	
	
public: 
	void setUp()
	{
		m_DB = new DataBaseTM();
	}
	
	void tearDown() 
	{
		delete m_DB;
	}
	
	void TESTTableExist()
	{
		CPPUNIT_ASSERT(m_DB->TableExist(_T("generic_lines")));
	}
	
	void TESTToolMapVersion()
	{
		CPPUNIT_ASSERT(m_DB->GetDatabaseToolMapVersion()==220);
	}
	
	
	
};

#endif

