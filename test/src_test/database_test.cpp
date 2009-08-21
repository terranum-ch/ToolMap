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
#include <iostream>


class DataBaseTEST : public CppUnit::TestFixture 
{ 
	CPPUNIT_TEST_SUITE( DataBaseTEST );
	CPPUNIT_TEST( TESTOpenDatabase );
	CPPUNIT_TEST( TESTResults );
	CPPUNIT_TEST( TESTQueryNoResults );
	CPPUNIT_TEST( TESTResultString  );
	CPPUNIT_TEST( TESTResultArrayString );
	CPPUNIT_TEST( TESTCountResults );
	CPPUNIT_TEST( TESTResultLong );
	CPPUNIT_TEST( TESTResultArrayLong );
	CPPUNIT_TEST( TESTResultDouble );
	CPPUNIT_TEST( TESTResultArrayDouble );
	CPPUNIT_TEST( TESTColResultsString );
	CPPUNIT_TEST( TESTColResultsLong );
	CPPUNIT_TEST( TESTColResultsDouble );
	CPPUNIT_TEST( TESTPathName );
	CPPUNIT_TEST( TESTQueriesNumber );
	CPPUNIT_TEST( TESTVersion );
	CPPUNIT_TEST( TESTCreateNewDatabase );
	CPPUNIT_TEST( TESTGetDataBaseSize );
	CPPUNIT_TEST( TESTGetLastInsertID );
	CPPUNIT_TEST( TESTGetRawRow );
	CPPUNIT_TEST( TESTDeleteDB );
	CPPUNIT_TEST_SUITE_END();
	
private:
	DataBase * m_DB;	
	
public: 
	void setUp()
	{
		m_DB = new DataBase();		
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
		m_DB->DataBaseClearResults();
		CPPUNIT_ASSERT(m_DB->DataBaseQuery(_T("SELECT OBJECT_ID FROM generic_lines WHERE OBJECT_ID = 1000000")));
		CPPUNIT_ASSERT(m_DB->DataBaseHasResults() == false);
		
	}
	
	void TESTQueryNoResults ()
	{
		
		CPPUNIT_ASSERT(m_DB->DataBaseQueryNoResults(_T("SELECT OBJECT_ID FROM generic_lines"))==false);
		CPPUNIT_ASSERT(m_DB->DataBaseOpen(_T("/Users/Lucien/DATA/SIG/COMBIOULA/CORRIGE/TOOLMAP/"),
						   _T("combioula_correct"))==true);
		CPPUNIT_ASSERT(m_DB->DataBaseQueryNoResults(_T("SELECT OBJECT_ID FROM generic_lines"))==true);
		CPPUNIT_ASSERT(m_DB->DataBaseQueryNoResults(_T("SELECT COUNT(*) FROM generic_linesss"))==false);
	}
	
	void TESTResultString ()
	{
		CPPUNIT_ASSERT(m_DB->DataBaseOpen(_T("/Users/Lucien/DATA/SIG/COMBIOULA/CORRIGE/TOOLMAP/"),
										  _T("combioula_correct"))==true);
		wxString myReturnedString =  wxEmptyString;
		CPPUNIT_ASSERT(m_DB->DataBaseQuery(_T("SELECT OBJECT_DESC FROM dmn_layer_object WHERE OBJECT_ID = 17")));
		// second query should fail, must delete results first.
		m_DB->DataBaseClearResults();
		CPPUNIT_ASSERT(m_DB->DataBaseQuery(_T("SELECT OBJECT_DESC FROM dmn_layer_object WHERE OBJECT_ID = 17"))==true);
		CPPUNIT_ASSERT(m_DB->DataBaseGetNextResult(myReturnedString)==true);
		CPPUNIT_ASSERT(myReturnedString == _T("bord d'érosion")); // oid = 17
		CPPUNIT_ASSERT(m_DB->DataBaseGetNextResult(myReturnedString)==false);
		CPPUNIT_ASSERT(myReturnedString == wxEmptyString);
		m_DB->DataBaseClearResults();
		
		CPPUNIT_ASSERT(m_DB->DataBaseQuery(_T("SELECT OBJECT_ID FROM dmn_layer_object WHERE OBJECT_ID = 17777")));
		CPPUNIT_ASSERT(m_DB->DataBaseGetNextResult(myReturnedString)==false);
		CPPUNIT_ASSERT(myReturnedString == wxEmptyString);
		
	}
	
	void TESTResultArrayString()
	{
		CPPUNIT_ASSERT(m_DB->DataBaseOpen(_T("/Users/Lucien/DATA/SIG/COMBIOULA/CORRIGE/TOOLMAP/"),
										  _T("combioula_correct"))==true);
		CPPUNIT_ASSERT(m_DB->DataBaseQuery(_T("SELECT * FROM dmn_layer_object WHERE OBJECT_ID = 17")));
		wxArrayString myResults;
		
		CPPUNIT_ASSERT(m_DB->DataBaseGetNextResult(myResults));
		CPPUNIT_ASSERT(myResults.GetCount() == 9); // 9 cols in dmn_layer_object
		CPPUNIT_ASSERT(myResults.Item(4) == _T("bord d'érosion"));
		m_DB->DataBaseClearResults();
		// limit tests
		
		CPPUNIT_ASSERT(m_DB->DataBaseQuery(_T("SELECT OBJECT_ID FROM dmn_layer_object WHERE OBJECT_ID = 17")));
		CPPUNIT_ASSERT(m_DB->DataBaseGetNextResult(myResults));
		CPPUNIT_ASSERT(myResults.GetCount() == 1);
		m_DB->DataBaseClearResults();
		
		CPPUNIT_ASSERT(m_DB->DataBaseQuery(_T("SELECT OBJECT_ID FROM dmn_layer_object WHERE OBJCT_ID = 17777"))==false);
		CPPUNIT_ASSERT(m_DB->DataBaseGetNextResult(myResults)==false);
		CPPUNIT_ASSERT(myResults.GetCount() == 0);
	}
	
	void TESTCountResults()
	{
		CPPUNIT_ASSERT(m_DB->DataBaseOpen(_T("/Users/Lucien/DATA/SIG/COMBIOULA/CORRIGE/TOOLMAP/"),
										  _T("combioula_correct"))==true);
		uint myCols = 0;
		long myRows = 0;
		CPPUNIT_ASSERT(m_DB->DataBaseGetResultSize(&myCols, &myRows)==false);
		CPPUNIT_ASSERT(m_DB->DataBaseQuery(_T("SELECT * FROM dmn_layer_object WHERE OBJECT_ID = 17")));
		CPPUNIT_ASSERT(m_DB->DataBaseGetResultSize(&myCols, NULL));
		CPPUNIT_ASSERT(myCols == 9);
		CPPUNIT_ASSERT(m_DB->DataBaseGetResultSize(NULL, &myRows));
		CPPUNIT_ASSERT(myRows == 1);
		myRows = 0;
		myCols = 0;
		CPPUNIT_ASSERT(m_DB->DataBaseGetResultSize(&myCols, &myRows));
		CPPUNIT_ASSERT(myRows == 1 && myCols == 9);
		
	}
	
	void TESTResultLong()
	{
		CPPUNIT_ASSERT(m_DB->DataBaseOpen(_T("/Users/Lucien/DATA/SIG/COMBIOULA/CORRIGE/TOOLMAP/"),
										  _T("combioula_correct"))==true);
		long myResult = 0;
		CPPUNIT_ASSERT(m_DB->DataBaseQuery(_T("SELECT OBJECT_ID FROM dmn_layer_object WHERE OBJECT_ID = 17")));
		CPPUNIT_ASSERT(m_DB->DataBaseGetNextResult(myResult));
		CPPUNIT_ASSERT(myResult == 17);
	}
	
	void TESTResultArrayLong()
	{
		CPPUNIT_ASSERT(m_DB->DataBaseOpen(_T("/Users/Lucien/DATA/SIG/COMBIOULA/CORRIGE/TOOLMAP/"),
										  _T("combioula_correct"))==true);
		CPPUNIT_ASSERT(m_DB->DataBaseQuery(_T("SELECT OBJECT_ID, OBJECT_CD FROM dmn_layer_object WHERE OBJECT_ID <= 17")));
		
		uint myCols = 0;
		long myRows = 0;
		
		CPPUNIT_ASSERT (m_DB->DataBaseGetResultSize(&myCols, &myRows));
		CPPUNIT_ASSERT (myCols == 2 && myRows == 17);
		
		
		wxArrayLong myResults;
		int i = 0;
		bool bReturn = false;
		while (1)
		{
			bReturn = m_DB->DataBaseGetNextResult(myResults);
			if (i < 17)
			{
				CPPUNIT_ASSERT(bReturn == true);
			}
			else
				CPPUNIT_ASSERT(bReturn == false);
			i++;
			if (bReturn == false)
				break;
		}
		
		m_DB->DataBaseClearResults();
		CPPUNIT_ASSERT(m_DB->DataBaseQuery(_T("SELECT OBJECT_ID FROM dmn_layer_object WHERE OBJECT_ID <= 17 ORDER BY OBJECT_ID")));
		CPPUNIT_ASSERT(m_DB->DataBaseGetNextResult(myResults));
		CPPUNIT_ASSERT (m_DB->DataBaseGetResultSize(&myCols, &myRows));
		CPPUNIT_ASSERT (myCols == 1 && myRows == 17);
	}
	
	void TESTResultDouble()
	{
		CPPUNIT_ASSERT(m_DB->DataBaseOpen(_T("/Users/Lucien/Downloads/"),_T("testfields"))==true);
		CPPUNIT_ASSERT(m_DB->DataBaseQuery(_T("SELECT TestFloat32 FROM layer_at1 WHERE OBJECT_ID = 1")));
		double value = 0;
		CPPUNIT_ASSERT(m_DB->DataBaseGetNextResult(value));
		CPPUNIT_ASSERT_DOUBLES_EQUAL(8.99, value, 0.01);
		CPPUNIT_ASSERT(value == 8.99);
	}
	
	void TESTResultArrayDouble()
	{
		CPPUNIT_ASSERT(m_DB->DataBaseOpen(_T("/Users/Lucien/Downloads/"),_T("testfields"))==true);
		CPPUNIT_ASSERT(m_DB->DataBaseQuery(_T("SELECT TestFloat32 FROM layer_at1 ORDER BY OBJECT_ID")));
		wxArrayDouble values;
		CPPUNIT_ASSERT(m_DB->DataBaseGetNextResult(values));
		CPPUNIT_ASSERT(values.Item(0) == 8.99);
	}
	
	void TESTColResultsString ()
	{
		CPPUNIT_ASSERT(m_DB->DataBaseOpen(_T("/Users/Lucien/Downloads/"),_T("testfields"))==true);
		CPPUNIT_ASSERT(m_DB->DataBaseQuery(_T("SELECT TestText FROM layer_at1 ORDER BY OBJECT_ID")));
		wxArrayString myResults;
		CPPUNIT_ASSERT(m_DB->DataBaseGetResults(myResults));
		CPPUNIT_ASSERT(myResults.GetCount() == 2);
		CPPUNIT_ASSERT(myResults.Item(1) == _T("Ceci est un test pour un max de caracteres ke lonp"));
		CPPUNIT_ASSERT(m_DB->DataBaseGetResults(myResults)==false);
		CPPUNIT_ASSERT(myResults.GetCount() == 0);
		CPPUNIT_ASSERT(m_DB->DataBaseQuery(_T("SELECT TestText FROM layer_at1 ORDER BY OBJECT_ID")));
		m_DB->DataBaseClearResults();
	}
	
	void TESTColResultsLong ()
	{
		CPPUNIT_ASSERT(m_DB->DataBaseOpen(_T("/Users/Lucien/Downloads/"),_T("testfields"))==true);
		CPPUNIT_ASSERT(m_DB->DataBaseQuery(_T("SELECT OBJECT_ID FROM layer_at1 ORDER BY OBJECT_ID")));
		wxArrayLong myResults;
		CPPUNIT_ASSERT(m_DB->DataBaseGetResults(myResults));
		CPPUNIT_ASSERT(myResults.GetCount() == 2);
		CPPUNIT_ASSERT(myResults.Item(1) == 4);
	}
	
	void TESTColResultsDouble()
	{
		CPPUNIT_ASSERT(m_DB->DataBaseOpen(_T("/Users/Lucien/Downloads/"),_T("testfields"))==true);
		CPPUNIT_ASSERT(m_DB->DataBaseQuery(_T("SELECT TestFloat32 FROM layer_at1 ORDER BY OBJECT_ID")));
		wxArrayDouble myResults;
		CPPUNIT_ASSERT(m_DB->DataBaseGetResults(myResults));
		CPPUNIT_ASSERT(myResults.GetCount() == 2);
		CPPUNIT_ASSERT(myResults.Item(1) == 6.00);
	}
	
	void TESTPathName()
	{
		CPPUNIT_ASSERT(m_DB->DataBaseGetName() == wxEmptyString);
		CPPUNIT_ASSERT(m_DB->DataBaseGetPath() == wxEmptyString);
		CPPUNIT_ASSERT(m_DB->DataBaseOpen(_T("/Users/Lucien/Downloads/"),_T("testfields"))==true);
		CPPUNIT_ASSERT(m_DB->DataBaseGetName() == _T("testfields"));
		CPPUNIT_ASSERT(m_DB->DataBaseGetPath() == _T("/Users/Lucien/Downloads/"));
		CPPUNIT_ASSERT(m_DB->DataBaseOpen(_T("/Users/Lucien/Downloads"),_T("testfieldssss"))==false);
		CPPUNIT_ASSERT(m_DB->DataBaseGetName() == wxEmptyString);
		CPPUNIT_ASSERT(m_DB->DataBaseGetPath() == wxEmptyString);
	}
	
	
	void TESTQueriesNumber()
	{
		wxString myQueries = _T("SELECT * FROM COUCOU; INSERT INTO ..; SELECT ALL");
		CPPUNIT_ASSERT(m_DB->DataBaseQueriesNumber(myQueries)==3);
		wxString myQueries2 = _T("SELECT * FROM COUCOU; INSERT INTO; SELECT ALL;");
		CPPUNIT_ASSERT(m_DB->DataBaseQueriesNumber(myQueries)==3);		
	}
	
	void TESTVersion()
	{
		CPPUNIT_ASSERT(DataBase::DataBaseGetVersion() == _T("5.1.37"));
	}
	
	
	void TESTCreateNewDatabase()
	{
		CPPUNIT_ASSERT(m_DB->DataBaseCreateNew(_T("/Users/Lucien/Downloads/"), _T("mytest1"))==false);
		CPPUNIT_ASSERT(m_DB->DataBaseOpen(_T("/Users/Lucien/Downloads/"), _T("mytest1")));
		CPPUNIT_ASSERT(m_DB->DataBaseQuery(_T("SHOW TABLES FROM mytest1")));
	}
	
	void TESTGetDataBaseSize()
	{
		wxString myFailMsg = _("Not available");
		wxString myDBSize = m_DB->DataBaseGetSize(2, myFailMsg);
		CPPUNIT_ASSERT(myDBSize == myFailMsg);
		CPPUNIT_ASSERT(m_DB->DataBaseOpen(_T("/Users/Lucien/Downloads/"), _T("mytest1")));
		myDBSize = m_DB->DataBaseGetSize(2, myFailMsg);
		wxLogDebug(myDBSize);
		CPPUNIT_ASSERT(myDBSize != myFailMsg);
		
		CPPUNIT_ASSERT(m_DB->DataBaseOpen(_T("/Users/Lucien/DATA/SIG/COMBIOULA/CORRIGE/TOOLMAP/"),
										  _T("combioula_correct"))==true);
		myDBSize = m_DB->DataBaseGetSize(2, myFailMsg);
		wxLogDebug(myDBSize);
		CPPUNIT_ASSERT(myDBSize != myFailMsg);
	}
	
	void TESTGetLastInsertID()
	{
		CPPUNIT_ASSERT(m_DB->DataBaseOpen(_T("/Users/Lucien/Downloads/"),_T("testfields"))==true);
		long myIID = m_DB->DataBaseGetLastInsertedID();
		CPPUNIT_ASSERT(myIID == wxNOT_FOUND);
		CPPUNIT_ASSERT(m_DB->DataBaseQueryNoResults(_T("INSERT INTO dmn_layer_object (OBJECT_CD) VALUES (1)")));
		
		myIID = m_DB->DataBaseGetLastInsertedID();
		CPPUNIT_ASSERT(myIID != wxNOT_FOUND);
		wxLogDebug(_T("Last inserted ID = %d"), myIID);
		
		myIID = m_DB->DataBaseGetLastInsertedID();
		CPPUNIT_ASSERT(myIID != wxNOT_FOUND);
		
	}
	
	
	void TESTGetRawRow ()
	{
		CPPUNIT_ASSERT(m_DB->DataBaseOpen(_T("/Users/Lucien/Downloads/"),_T("testfields"))==true);
		CPPUNIT_ASSERT(m_DB->DataBaseQuery(_T("SELECT Envelope(OBJECT_GEOMETRY) FROM generic_lines WHERE OBJECT_ID = 1"))); //WHERE OBJECT_ID = 2")));
		MYSQL_ROW myRow;
		tmArrayULong myLength;
		CPPUNIT_ASSERT(m_DB->DataBaseGetNextRowResult(myRow, myLength));
		CPPUNIT_ASSERT(myRow != NULL);
		CPPUNIT_ASSERT(myLength.GetCount() != 0);
		CPPUNIT_ASSERT(m_DB->DataBaseGetNextRowResult(myRow, myLength)==false);
		CPPUNIT_ASSERT(myRow == NULL);
		CPPUNIT_ASSERT(myLength.GetCount() == 0);
		
	}
	
	void TESTPause()
	{
		// this create a pause test
		int i = 0;
		std::cout << "waiting for input...";
		std::cin >> i;
	}
	
	void TESTDeleteDB()
	{
		CPPUNIT_ASSERT(m_DB->DataBaseDelete()==false);
		CPPUNIT_ASSERT(m_DB->DataBaseCreateNew(_T("/Users/Lucien/Downloads/"),_T("testedit_12"))==true);
		CPPUNIT_ASSERT(m_DB->DataBaseDelete()==true);
	}
	
};

#endif



