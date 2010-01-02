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
		m_DataSelected->m_QueryObjectGeomID = 231; // ligne attribuée à une faille + Border of rocks
		m_DataSelected->m_QueryLayerType = TOC_NAME_LINES;
		
		// set up query for generic
		m_DataGeneric = new QueriesData();
		m_DataGeneric->m_QueryType = QUERY_GENERIC;
		m_DataGeneric->m_QueryName = _T("Test generic");
		m_DataGeneric->m_QueryLayerType = TOC_NAME_LINES;
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
		TS_ASSERT(myBuilder.Save(m_pDB));
		wxLogMessage(_T("Saving queries into database"));
		
		// delete last added query
		long myLastId = m_pDB->DataBaseGetLastInsertedID();
		TS_ASSERT_DIFFERS(myLastId, wxNOT_FOUND);
		TS_ASSERT (m_pDB->DeleteQuery(myLastId));
		wxLogMessage(_T("Deleting Generic queries n.%d from database"), myLastId);
	}
	
	void testCreateSQL()
	{
		QueriesBuilder myBuilder(m_DataSQL);
		TS_ASSERT(myBuilder.Create(m_pDB));
		TS_ASSERT(myBuilder.Save(m_pDB));
		
		// delete last added query
		long myLastId = m_pDB->DataBaseGetLastInsertedID();
		TS_ASSERT_DIFFERS(myLastId, wxNOT_FOUND);
		TS_ASSERT (m_pDB->DeleteQuery(myLastId));
		wxLogMessage(_T("Deleting Expert queries n.%d from database"), myLastId);
	}
	
	void testCreateLayers()
	{
		QueriesBuilder myBuilder(m_DataLayer);
		TS_ASSERT(myBuilder.Create(m_pDB));
		TS_ASSERT(myBuilder.Save(m_pDB));
		
		// delete last added query
		long myLastId = m_pDB->DataBaseGetLastInsertedID();
		TS_ASSERT_DIFFERS(myLastId, wxNOT_FOUND);
		TS_ASSERT (m_pDB->DeleteQuery(myLastId));
		wxLogMessage(_T("Deleting layers queries n.%d from database"), myLastId);
		
	}
	
	void testListLayers()
	{
		PrjMemLayersArray myLayers;
		
		TS_ASSERT(m_DataLayer->GetLayers(m_pDB, myLayers));
		TS_ASSERT_EQUALS(myLayers.GetCount(),9);
		
		for (unsigned int i = 0; i<myLayers.GetCount(); i++) {
			wxLogMessage(_T("%d Layers name : %s"),i,myLayers.Item(i).m_LayerName.c_str());
		}
	}
	
	void testListObjectValue()
	{
		PrjMemObjectsArray myTypes;
		
		// lignes n.231 (Faille et Border of Rocks_PLG)
		TS_ASSERT(m_DataSelected->GetTypes(m_pDB, myTypes));
		TS_ASSERT_EQUALS(myTypes.GetCount(),2);
		TS_ASSERT_EQUALS(myTypes.Item(1).m_ObjectName, _T("faille"));
		TS_ASSERT_EQUALS(myTypes.Item(1).m_ObjectID, 32);
		
	}
	
	void testIsGenericLayer()
	{
		TS_ASSERT(m_DataSelected->IsGenericLayer(m_pDB, 0)==false);
		TS_ASSERT_EQUALS(m_DataSelected->m_QueryLayerType, TOC_NAME_LINES);
		TS_ASSERT(m_DataSelected->IsGenericLayer(m_pDB, 1)==true);
		TS_ASSERT_EQUALS(m_DataSelected->m_QueryLayerType, TOC_NAME_LINES);
		TS_ASSERT(m_DataSelected->IsGenericLayer(m_pDB, 2)==true);
		TS_ASSERT_EQUALS(m_DataSelected->m_QueryLayerType, TOC_NAME_POINTS);
		TS_ASSERT(m_DataSelected->IsGenericLayer(m_pDB, 3)==true);
		TS_ASSERT_EQUALS(m_DataSelected->m_QueryLayerType, TOC_NAME_LABELS);
		TS_ASSERT(m_DataSelected->IsGenericLayer(m_pDB, 4)==true);
		TS_ASSERT_EQUALS(m_DataSelected->m_QueryLayerType, TOC_NAME_ANNOTATIONS);
		TS_ASSERT(m_DataSelected->IsGenericLayer(m_pDB, 5)==false);
		TS_ASSERT_EQUALS(m_DataSelected->m_QueryLayerType, TOC_NAME_LINES);
		TS_ASSERT(m_DataSelected->IsGenericLayer(m_pDB, 31)==false);
		TS_ASSERT_EQUALS(m_DataSelected->m_QueryLayerType, TOC_NAME_LINES);
		TS_ASSERT(m_DataSelected->IsGenericLayer(m_pDB, 161)==false);
		TS_ASSERT_EQUALS(m_DataSelected->m_QueryLayerType, TOC_NAME_LINES);
	}
	
	void testCreateSelectedNoAttribs(){
		QueriesBuilder myBuilder(m_DataSelected);
		TS_ASSERT(myBuilder.Create(m_pDB)==true);
		TS_ASSERT(myBuilder.Save(m_pDB));
		wxLogMessage(_T("Saving queries into database"));
		
		// delete last added query
		long myLastId = m_pDB->DataBaseGetLastInsertedID();
		TS_ASSERT_DIFFERS(myLastId, wxNOT_FOUND);
		TS_ASSERT (m_pDB->DeleteQuery(myLastId));
		wxLogMessage(_T("Deleting Selected queries n.%d from database"), myLastId);
	}
	
	
	void testGetValues(){
		wxArrayString myValues;
		long myLayerID = 7; // Tectobound_L pour faille.
		long myLayerID2 = 4; // GlacStruct, pas de champs
		PrjMemFieldArray myFields;
		
		// no fields availlable
		TS_ASSERT(m_DataSelected->GetFieldsValues(m_pDB,myLayerID2, 
												  myFields, myValues)==false);
		TS_ASSERT_EQUALS(myFields.GetCount(), 0);
		TS_ASSERT_EQUALS(myValues.GetCount(), 0);
		
		
		
		// getting fields and values
		TS_ASSERT(m_DataSelected->GetFieldsValues(m_pDB,myLayerID,myFields, myValues)==true);
		TS_ASSERT_EQUALS(myFields.GetCount(), 4);
		TS_ASSERT_EQUALS(myValues.GetCount(), 4);
		TS_ASSERT_EQUALS(myValues.Item(3), wxEmptyString);
		
	}
	
	
	void testGetAttribValues(){
		
		
		
		m_DataSelected->m_QueryObjectGeomID = 140; // faille avec attrib avancés
		
		wxArrayString myValues;
		PrjMemFieldArray myFields;
		
		// no fields availlable
		TS_ASSERT(m_DataSelected->GetFieldsValues(m_pDB,7, 
												  myFields, myValues)==true);
		TS_ASSERT_EQUALS(myFields.GetCount(), 4);
		TS_ASSERT_EQUALS(myValues.GetCount(), 4);
		TS_ASSERT_DIFFERS(myValues.Item(0), wxEmptyString);
		TS_ASSERT_EQUALS(myValues.Item(0), _T("probable"));
		
		for (unsigned int i = 0; i< myValues.GetCount(); i++) {
			wxLogDebug(myValues.Item(i));
		}
		
	}
	
	
	void testGetParentLayer(){
		wxLogMessage(_T("Testing Getting Parent layer values"));
		
		long layerid = 0;
		TS_ASSERT(m_DataSelected->GetParentLayer(m_pDB, layerid));
		TS_ASSERT_EQUALS(layerid, 7);
		m_DataSelected->m_QueryObjectID = 50; // n'existe pas
		TS_ASSERT(m_DataSelected->GetParentLayer(m_pDB, layerid)==false);
		TS_ASSERT_EQUALS(layerid, wxNOT_FOUND);
		
		m_DataSelected->m_QueryObjectID = 41; // moraine -> layer 8
		TS_ASSERT(m_DataSelected->GetParentLayer(m_pDB, layerid));
		TS_ASSERT_EQUALS(layerid, 8);
	}
		
};




#endif
