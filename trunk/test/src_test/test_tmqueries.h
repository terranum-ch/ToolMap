/***************************************************************************
							test_tmqueries.h
						Test the new queries process
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

#ifndef _TM_TEST_QUERIES_H_
#define _TM_TEST_QUERIES_H_



#include "wx/wxprec.h"
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <cxxtest/TestSuite.h>

#include "../../src/core/queriesdata.h"
#include "../../src/core/queriesbuilder.h"
#include "test_param.h"


class TEST_tmQueries : public CxxTest::TestSuite
{
public:
	QueriesData * m_DataLayer;
	QueriesData * m_DataSelected;
	QueriesData * m_DataGeneric;
	QueriesData * m_DataSQL;
	
	DataBaseTM * m_pDB;
	
	TEST_tmQueries (bool bTest){
		m_pDB = new DataBaseTM();
		TS_ASSERT(m_pDB->OpenTMDatabase(g_TestPathPRJ + g_TestPrj_NewQueries));
	}
	
	virtual ~TEST_tmQueries(){
		delete m_pDB;
	}
	
	static TEST_tmQueries *createSuite() { return new TEST_tmQueries(true);}
    static void destroySuite( TEST_tmQueries *suite ) { delete suite; }

	
	
	void setUp()
	{
		// set up query for layers
		m_DataLayer = new QueriesData();
		m_DataLayer->m_QueryType = QUERY_LAYERS;
		m_DataLayer->m_QueryName = _T("Test Layers");
		m_DataLayer->m_QueryLayerID = 7; // Tectobound_L
		
		// set up query for selected
		m_DataSelected = new QueriesData();
		m_DataSelected->m_QueryType = QUERY_SELECTED;
		m_DataSelected->m_QueryName = _T("Test selected");
		m_DataSelected->m_QueryObjectID = 32; // Faille
		m_DataSelected->m_QueryObjectGeomID = 79; // ligne attribuée à une faille
		
		// set up query for generic
		m_DataGeneric = new QueriesData();
		m_DataGeneric->m_QueryType = QUERY_GENERIC;
		m_DataGeneric->m_QueryName = _T("Test generic");
		// query for selecting all lines wo attribution.
		m_DataGeneric->m_QuerySQL = _T("SELECT l.OBJECT_ID FROM generic_lines l WHERE l.OBJECT_ID NOT IN (SELECT generic_aat.OBJECT_GEOM_ID FROM generic_aat)"); // Faille
		
		
		// setup query for sql expert
		m_DataSQL = new QueriesData();
		m_DataSQL->m_QueryType = QUERY_SQL;
		m_DataSQL->m_QueryName = _T("Test SQL");
		// query for selecting all lines wo attribution.
		m_DataSQL->m_QueryLayerType = TOC_NAME_LINES;
		m_DataSQL->m_QuerySQL = _T("SELECT l.OBJECT_ID FROM generic_lines l WHERE l.OBJECT_ID NOT IN (SELECT generic_aat.OBJECT_GEOM_ID FROM generic_aat)"); // Faille
		
		
	}
	
	void tearDown() 
	{
		delete m_DataLayer;
		delete m_DataSelected;
		delete m_DataGeneric;
		delete m_DataSQL;
	}

		
	void testName(){
		wxLogMessage(_T("------------------------------------"));
		wxLogMessage(_T("------- TESTING NEW QUERIES --------"));
		wxLogMessage(_T("------------------------------------"));
	}
	
	void testQueryLayers()
	{
		QueriesBuilder myBuilder(m_DataLayer);
		TS_ASSERT(myBuilder.IsOk());
	}
	
	void testQueryNoType()
	{
		QueriesData myData;
		QueriesBuilder myBuilder(&myData);
		TS_ASSERT(myBuilder.IsOk() == false);
	}
	
	void testQuerySelected()
	{
		QueriesBuilder myBuilder(m_DataSelected);
		TS_ASSERT(myBuilder.IsOk());
	}
	
	void testQueryGeneric()
	{
		QueriesBuilder myBuilder(m_DataGeneric);
		TS_ASSERT(myBuilder.IsOk());
		
	}
	
	void testQuerySQL()
	{
		QueriesBuilder myBuilder(m_DataSQL);
		TS_ASSERT(myBuilder.IsOk());
		
	}
	
	void testIsCreated()
	{
		QueriesBuilder myBuilder(m_DataSQL);
		TS_ASSERT(myBuilder.Save(m_pDB)==false) // query not created
	}
	
	
	void testCreateGeneric()
	{
		QueriesBuilder myBuilder(m_DataGeneric);
		TS_ASSERT(myBuilder.Create(m_pDB)==true);
		
	}
	
		
};




#endif
