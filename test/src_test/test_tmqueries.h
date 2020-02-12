/***************************************************************************
 test_tmqueries.h
 Test the new queries process
 -------------------
 copyright : (C) 2009 CREALP Lucien Schreiber
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

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <cxxtest/TestSuite.h>

#include "../../src/core/queriesbuilder.h"
#include "../../src/core/queriesdata.h"
#include "test_param.h"

class TEST_tmQueries : public CxxTest::TestSuite {
 public:
  QueriesData *m_DataLayer;
  QueriesData *m_DataSelected;
  QueriesData *m_DataGeneric;
  QueriesData *m_DataSQL;
  QueriesData *m_DataObjects;
  QueriesData *m_DataGeomLine;
  QueriesData *m_DataGeomNode;

  DataBaseTM *m_pDB;

  TEST_tmQueries(bool bTest) {
    wxApp::SetInstance(new wxAppConsole());
    m_pDB = new DataBaseTM();
    TS_ASSERT(m_pDB->OpenTMDatabase(g_TestPathPRJ + g_TestPrj_NewQueries));
  }

  virtual ~TEST_tmQueries() {
    delete m_pDB;
  }

  static TEST_tmQueries *createSuite() {
    return new TEST_tmQueries(true);
  }
  static void destroySuite(TEST_tmQueries *suite) {
    delete suite;
  }

  void setUp() {
    wxApp::SetInstance(new wxAppConsole());
    // set up query for layers
    m_DataLayer = new QueriesData();
    m_DataLayer->m_QueryType = QUERY_LAYERS;
    m_DataLayer->m_QueryName = _T("Test Layers");
    m_DataLayer->m_QueryLayerID = 7;  // Tectobound_L

    // set up query for selected
    m_DataSelected = new QueriesData();
    m_DataSelected->m_QueryType = QUERY_SELECTED;
    m_DataSelected->m_QueryName = _T("Test selected");
    m_DataSelected->m_QueryObjectID = 32;  // Faille
    m_DataSelected->m_QueryObjectGeomID =
        231;  // ligne attribuée à une faille + Border of rocks, attribution avancée vide
    m_DataSelected->m_QueryLayerType = TOC_NAME_LINES;

    // set up query for generic
    m_DataGeneric = new QueriesData();
    m_DataGeneric->m_QueryType = QUERY_GENERIC;
    m_DataGeneric->m_QueryName = _T("Test generic");
    m_DataGeneric->m_QueryLayerType = TOC_NAME_LINES;
    // query for selecting all lines wo attribution.
    m_DataGeneric->m_QuerySQL =
        _T("SELECT l.OBJECT_ID FROM generic_lines l WHERE l.OBJECT_ID NOT IN (SELECT generic_aat.OBJECT_GEOM_ID FROM ")
        _T("generic_aat)");  // Faille

    // setup query for sql expert
    m_DataSQL = new QueriesData();
    m_DataSQL->m_QueryType = QUERY_SQL;
    m_DataSQL->m_QueryName = _T("Test SQL");
    // query for selecting all lines wo attribution.
    m_DataSQL->m_QueryLayerType = TOC_NAME_LINES;
    m_DataSQL->m_QuerySQL =
        _T("SELECT l.OBJECT_ID FROM generic_lines l WHERE l.OBJECT_ID NOT IN (SELECT generic_aat.OBJECT_GEOM_ID FROM ")
        _T("generic_aat)");  // Faille

    // setup for object data
    m_DataObjects = new QueriesData();
    m_DataObjects->m_QueryType = QUERY_OBJECTS;
    m_DataObjects->m_QueryName = _T("Test Object Query");
    m_DataObjects->m_QueryLayerType = TOC_NAME_LINES;

    // setup for geom queries
    m_DataGeomLine = new QueriesData();
    m_DataGeomLine->m_QueryType = QUERY_LINES;
    m_DataGeomLine->m_QueryName = _T("Test Line Query");
    // m_DataGeomLine->m_QueryLineSize = 10; // all lines smaller than this value

    m_DataGeomNode = new QueriesData();
    m_DataGeomNode->m_QueryType = QUERY_NODES;
    m_DataGeomNode->m_QueryName = _T("Test node query");
  }

  void tearDown() {
    delete m_DataLayer;
    delete m_DataSelected;
    delete m_DataGeneric;
    delete m_DataSQL;
    delete m_DataObjects;
    delete m_DataGeomLine;
    delete m_DataGeomNode;
  }

  void testName() {
    wxLogMessage(_T("------------------------------------"));
    wxLogMessage(_T("------- TESTING NEW QUERIES --------"));
    wxLogMessage(_T("------------------------------------"));
  }

  void testQueryLayers() {
    QueriesBuilder myBuilder(m_DataLayer);
    TS_ASSERT(myBuilder.IsOk());
  }

  void testQueryNoType() {
    QueriesData myData;
    QueriesBuilder myBuilder(&myData);
    TS_ASSERT(!myBuilder.IsOk());
  }

  void testQuerySelected() {
    QueriesBuilder myBuilder(m_DataSelected);
    TS_ASSERT(myBuilder.IsOk());
  }

  void testQueryGeneric() {
    QueriesBuilder myBuilder(m_DataGeneric);
    TS_ASSERT(myBuilder.IsOk());
  }

  void testQuerySQL() {
    QueriesBuilder myBuilder(m_DataSQL);
    TS_ASSERT(myBuilder.IsOk());
  }

  void testIsCreated() {
    QueriesBuilder myBuilder(m_DataSQL);
    TS_ASSERT(!myBuilder.Save(m_pDB))  // query not created
  }

  void testCreateGeneric() {
    QueriesBuilder myBuilder(m_DataGeneric);
    TS_ASSERT(myBuilder.Create(m_pDB));
    TS_ASSERT(myBuilder.Save(m_pDB));
    wxLogMessage(_T("Saving queries into database"));

    // delete last added query
    long myLastId = m_pDB->DataBaseGetLastInsertedID();
    TS_ASSERT_DIFFERS(myLastId, wxNOT_FOUND);
    TS_ASSERT(m_pDB->DeleteQuery(myLastId));
    wxLogMessage(_T("Deleting Generic queries n.%ld from database"), myLastId);
  }

  void testCreateSQL() {
    QueriesBuilder myBuilder(m_DataSQL);
    TS_ASSERT(myBuilder.Create(m_pDB));
    TS_ASSERT(myBuilder.Save(m_pDB));

    // delete last added query
    long myLastId = m_pDB->DataBaseGetLastInsertedID();
    TS_ASSERT_DIFFERS(myLastId, wxNOT_FOUND);
    TS_ASSERT(m_pDB->DeleteQuery(myLastId));
    wxLogMessage(_T("Deleting Expert queries n.%ld from database"), myLastId);
  }

  void testCreateLayers() {
    QueriesBuilder myBuilder(m_DataLayer);
    TS_ASSERT(myBuilder.Create(m_pDB));
    TS_ASSERT(myBuilder.Save(m_pDB));

    // delete last added query
    long myLastId = m_pDB->DataBaseGetLastInsertedID();
    TS_ASSERT_DIFFERS(myLastId, wxNOT_FOUND);
    TS_ASSERT(m_pDB->DeleteQuery(myLastId));
    wxLogMessage(_T("Deleting layers queries n.%ld from database"), myLastId);
  }

  void testListLayers() {
    PrjMemLayersArray myLayers;

    TS_ASSERT(m_DataLayer->GetLayers(m_pDB, myLayers));
    TS_ASSERT_EQUALS(myLayers.GetCount(), 9);

    TS_ASSERT_EQUALS(myLayers[0]->m_LayerName, _T("Boreholes_PT"));
    TS_ASSERT_EQUALS(myLayers[1]->m_LayerName, _T("WatOutflow_PT"));
    TS_ASSERT_EQUALS(myLayers[2]->m_LayerName, _T("GravitFeat_PLG"));
    TS_ASSERT_EQUALS(myLayers[3]->m_LayerName, _T("GravitFeat_L"));
    TS_ASSERT_EQUALS(myLayers[4]->m_LayerName, _T("GlacStruct_L"));
    TS_ASSERT_EQUALS(myLayers[5]->m_LayerName, _T("Schistosity_PT"));
    TS_ASSERT_EQUALS(myLayers[6]->m_LayerName, _T("TectoBound_L"));
    TS_ASSERT_EQUALS(myLayers[7]->m_LayerName, _T("Drifts_PLG"));
    TS_ASSERT_EQUALS(myLayers[8]->m_LayerName, _T("Rocks_PLG"));

    /*for (unsigned int i = 0; i<myLayers.GetCount(); i++) {
            wxLogMessage(_T("%d Layers name : %s"),i,myLayers.Item(i)->m_LayerName.c_str());
    }*/
  }

  void testListObjectValue() {
    PrjMemObjectsArray myTypes;

    // lignes n.231 (Faille et Border of Rocks_PLG)
    TS_ASSERT(m_DataSelected->GetObjectsForSelection(m_pDB, myTypes));
    TS_ASSERT_EQUALS(myTypes.GetCount(), 2);
    TS_ASSERT_EQUALS(myTypes.Item(1)->m_ObjectName, _T("faille"));
    TS_ASSERT_EQUALS(myTypes.Item(1)->m_ObjectID, 32);
  }

  void testIsGenericLayer() {
    TS_ASSERT(!m_DataSelected->IsGenericLayer(m_pDB, 0));
    TS_ASSERT_EQUALS(m_DataSelected->m_QueryLayerType, TOC_NAME_LINES);
    TS_ASSERT(m_DataSelected->IsGenericLayer(m_pDB, 1));
    TS_ASSERT_EQUALS(m_DataSelected->m_QueryLayerType, TOC_NAME_LINES);
    TS_ASSERT(m_DataSelected->IsGenericLayer(m_pDB, 2));
    TS_ASSERT_EQUALS(m_DataSelected->m_QueryLayerType, TOC_NAME_POINTS);
    TS_ASSERT(m_DataSelected->IsGenericLayer(m_pDB, 3));
    TS_ASSERT_EQUALS(m_DataSelected->m_QueryLayerType, TOC_NAME_LABELS);
    TS_ASSERT(m_DataSelected->IsGenericLayer(m_pDB, 4));
    TS_ASSERT_EQUALS(m_DataSelected->m_QueryLayerType, TOC_NAME_ANNOTATIONS);
    TS_ASSERT(!m_DataSelected->IsGenericLayer(m_pDB, 5));
    TS_ASSERT_EQUALS(m_DataSelected->m_QueryLayerType, TOC_NAME_LINES);
    TS_ASSERT(!m_DataSelected->IsGenericLayer(m_pDB, 31));
    TS_ASSERT_EQUALS(m_DataSelected->m_QueryLayerType, TOC_NAME_LINES);
    TS_ASSERT(!m_DataSelected->IsGenericLayer(m_pDB, 161));
    TS_ASSERT_EQUALS(m_DataSelected->m_QueryLayerType, TOC_NAME_LINES);
  }

  void testCreateSelectedNoAttribs() {
    QueriesBuilder myBuilder(m_DataSelected);
    TS_ASSERT(myBuilder.Create(m_pDB));
    TS_ASSERT(myBuilder.Save(m_pDB));
    wxLogMessage(_T("Saving queries into database"));

    // delete last added query
    long myLastId = m_pDB->DataBaseGetLastInsertedID();
    TS_ASSERT_DIFFERS(myLastId, wxNOT_FOUND);
    TS_ASSERT(m_pDB->DeleteQuery(myLastId));
    wxLogMessage(_T("Deleting Selected queries n.%ld from database"), myLastId);
  }

  void testCreateSelectionWithoutAttribution() {
    QueriesBuilder myBuilder(m_DataSelected);
    m_DataSelected->m_QueryFieldsStatus = AATTRIBUTION_EMPTY;

    TS_ASSERT(!myBuilder.IsOk());
    long myLayerID = wxNOT_FOUND;
    TS_ASSERT(m_DataSelected->GetParentLayer(m_pDB, myLayerID));
    TS_ASSERT_DIFFERS(myLayerID, -1);
    TS_ASSERT(m_DataSelected->GetFieldsValues(m_pDB, myLayerID, m_DataSelected->m_QueryFields,
                                              m_DataSelected->m_QueryFieldsValues));
    TS_ASSERT(myBuilder.IsOk());
    TS_ASSERT(myBuilder.Create(m_pDB));
    TS_ASSERT(myBuilder.Save(m_pDB));
    // wxLogMessage(_T("Saving  selected queries into database"));

    // delete last added query
    long myLastId = m_pDB->DataBaseGetLastInsertedID();
    TS_ASSERT_DIFFERS(myLastId, wxNOT_FOUND);
    TS_ASSERT(m_pDB->DeleteQuery(myLastId));
    wxLogMessage(_T("Deleting Selected queries n.%ld from database"), myLastId);
  }

  void testCreateSelectionQueryAttribut() {
    QueriesBuilder myBuilder(m_DataSelected);
    m_DataSelected->m_QueryFieldsStatus = AATTRIBUTION_YES;
    m_DataSelected->m_QueryObjectGeomID = 140;

    TS_ASSERT(!myBuilder.IsOk());

    long myLayerID = wxNOT_FOUND;
    TS_ASSERT(m_DataSelected->GetParentLayer(m_pDB, myLayerID));
    TS_ASSERT_DIFFERS(myLayerID, -1);
    TS_ASSERT(m_DataSelected->GetFieldsValues(m_pDB, myLayerID, m_DataSelected->m_QueryFields,
                                              m_DataSelected->m_QueryFieldsValues));
    TS_ASSERT(myBuilder.IsOk());
    TS_ASSERT(myBuilder.Create(m_pDB));
    TS_ASSERT(myBuilder.Save(m_pDB));
    // wxLogMessage(_T("Saving  selected queries into database"));

    // delete last added query
    long myLastId = m_pDB->DataBaseGetLastInsertedID();
    TS_ASSERT_DIFFERS(myLastId, wxNOT_FOUND);
    TS_ASSERT(m_pDB->DeleteQuery(myLastId));
    wxLogMessage(_T("Deleting Selected queries n.%ld from database"), myLastId);
  }

  void testGetValues() {
    wxArrayString myValues;
    long myLayerID = 7;   // Tectobound_L pour faille.
    long myLayerID2 = 4;  // GlacStruct, pas de champs
    PrjMemFieldArray myFields;

    TS_ASSERT(!m_DataSelected->GetFieldsValues(m_pDB, myLayerID2, myFields, myValues));
    TS_ASSERT_EQUALS(myFields.GetCount(), 0);
    TS_ASSERT_EQUALS(myValues.GetCount(), 0);

    // getting fields and values
    TS_ASSERT(m_DataSelected->GetFieldsValues(m_pDB, myLayerID, myFields, myValues));
    TS_ASSERT_EQUALS(myFields.GetCount(), 4);
    TS_ASSERT_EQUALS(myValues.GetCount(), 4);
    TS_ASSERT_EQUALS(myValues.Item(3), wxEmptyString);
  }

  void testGetAttribValues() {
    m_DataSelected->m_QueryObjectGeomID = 140;  // faille avec attrib avancés

    wxArrayString myValues;
    PrjMemFieldArray myFields;

    // fields availlable
    TS_ASSERT(m_DataSelected->GetFieldsValues(m_pDB, 7, myFields, myValues));
    TS_ASSERT_EQUALS(myFields.GetCount(), 4);
    TS_ASSERT_EQUALS(myValues.GetCount(), 4);
    TS_ASSERT_EQUALS(myValues.Item(0), _T("probable"));  // probable
    TS_ASSERT_EQUALS(myValues.Item(1), _T("majeure"));   // majeure
    TS_ASSERT_EQUALS(myValues.Item(2), _T("normale"));   // normale
    TS_ASSERT_EQUALS(myValues.Item(3), _T("1"));         // 1

    /*for (unsigned int i = 0; i< myValues.GetCount(); i++) {
            wxLogDebug(myValues.Item(i));
    }*/

    // deuxième fois
    TS_ASSERT(m_DataSelected->GetFieldsValues(m_pDB, 7, myFields, myValues));
    TS_ASSERT_EQUALS(myFields.GetCount(), 4);
    TS_ASSERT_EQUALS(myValues.GetCount(), 4);
    TS_ASSERT_EQUALS(myValues.Item(0), _T("probable"));  // probable
    TS_ASSERT_EQUALS(myValues.Item(1), _T("majeure"));   // majeure
    TS_ASSERT_EQUALS(myValues.Item(2), _T("normale"));   // normale
    TS_ASSERT_EQUALS(myValues.Item(3), _T("1"));         // 1
  }

  void testGetParentLayer() {
    long layerid = 0;
    TS_ASSERT(m_DataSelected->GetParentLayer(m_pDB, layerid));
    TS_ASSERT_EQUALS(layerid, 7);
    m_DataSelected->m_QueryObjectID = 50;  // n'existe pas
    TS_ASSERT(!m_DataSelected->GetParentLayer(m_pDB, layerid));
    TS_ASSERT_EQUALS(layerid, wxNOT_FOUND);

    m_DataSelected->m_QueryObjectID = 41;  // moraine -> layer 8
    TS_ASSERT(m_DataSelected->GetParentLayer(m_pDB, layerid));
    TS_ASSERT_EQUALS(layerid, 8);
  }

  void testDeleteFields() {
    // no fields
    TS_ASSERT(!m_DataSelected->DeleteFieldsValue(2));

    // load fields
    m_DataSelected->m_QueryObjectGeomID = 140;  // faille avec attrib avancés
    TS_ASSERT(m_DataSelected->GetFieldsValues(m_pDB, 7, m_DataSelected->m_QueryFields,
                                              m_DataSelected->m_QueryFieldsValues));

    TS_ASSERT(m_DataSelected->DeleteFieldsValue(2));
    TS_ASSERT_EQUALS(m_DataSelected->m_QueryFieldsValues.GetCount(), 3);
    TS_ASSERT_EQUALS(m_DataSelected->m_QueryFields.GetCount(), 3);
    TS_ASSERT_EQUALS(m_DataSelected->m_QueryFieldsValues.Item(2), _T("1"));
    TS_ASSERT_EQUALS(m_DataSelected->m_QueryFields.Item(2)->m_Fieldname, _T("Thrust_class"));

    /*for (unsigned int i = 0; i< m_DataSelected->m_QueryFieldsValues.GetCount(); i++) {
            wxLogDebug(m_DataSelected->m_QueryFieldsValues.Item(i));
    }*/
  }

  void testAllAttribsEmpty() {
    TS_ASSERT_EQUALS(m_DataSelected->HasFieldsValues(), false);

    // all fields empty
    // m_DataSelected->m_QueryUseFields = true;
    TS_ASSERT(m_DataSelected->GetFieldsValues(m_pDB, 7, m_DataSelected->m_QueryFields,
                                              m_DataSelected->m_QueryFieldsValues));
    TS_ASSERT_EQUALS(m_DataSelected->HasFieldsValues(), false);
    /*
for (unsigned int i = 0; i< m_DataSelected->m_QueryFieldsValues.GetCount(); i++) {
            wxLogDebug(m_DataSelected->m_QueryFieldsValues.Item(i));
    }
    */

    m_DataSelected->m_QueryObjectGeomID = 140;  // faille avec attrib avancés
    TS_ASSERT(m_DataSelected->GetFieldsValues(m_pDB, 7, m_DataSelected->m_QueryFields,
                                              m_DataSelected->m_QueryFieldsValues));
    TS_ASSERT_EQUALS(m_DataSelected->HasFieldsValues(), true);
  }

  void testGetObjForTypes() {
    PrjMemObjectsArray myObjects;
    TS_ASSERT(m_DataObjects->GetObjectsForTypes(m_pDB, myObjects));
    TS_ASSERT_DIFFERS(myObjects.GetCount(), 0);
    wxLogMessage(_T("%zu objects returned for types : %s"), myObjects.GetCount(),
                 PRJDEF_LAYERS_TYPE_STRING[m_DataSelected->m_QueryLayerType].c_str());

    m_DataObjects->m_QueryLayerType = TOC_NAME_POINTS;
    TS_ASSERT(m_DataObjects->GetObjectsForTypes(m_pDB, myObjects));
    TS_ASSERT_DIFFERS(myObjects.GetCount(), 0);
    wxLogMessage(_T("%zu objects returned for types : %s"), myObjects.GetCount(),
                 PRJDEF_LAYERS_TYPE_STRING[m_DataObjects->m_QueryLayerType].c_str());

    m_DataObjects->m_QueryLayerType = TOC_NAME_LABELS;
    TS_ASSERT(m_DataObjects->GetObjectsForTypes(m_pDB, myObjects));
    TS_ASSERT_DIFFERS(myObjects.GetCount(), 0);
    wxLogMessage(_T("%zu objects returned for types : %s"), myObjects.GetCount(),
                 PRJDEF_LAYERS_TYPE_STRING[m_DataObjects->m_QueryLayerType].c_str());
  }

  void testQueryObjectOk() {
    TS_ASSERT_EQUALS(m_DataObjects->IsOk(), false);
    m_DataObjects->m_QueryObjectID = 32;
    TS_ASSERT_EQUALS(m_DataObjects->IsOk(), true);
  }

  void testQueryObjectSave() {
    m_DataObjects->m_QueryObjectID = 32;
    QueriesBuilder myBuilder(m_DataObjects);

    TS_ASSERT(myBuilder.IsOk());
    TS_ASSERT(myBuilder.Create(m_pDB));
    TS_ASSERT(myBuilder.Save(m_pDB));
    wxLogMessage(_T("Saving  Object queries into database"));

    // delete last added query
    long myLastId = m_pDB->DataBaseGetLastInsertedID();
    TS_ASSERT_DIFFERS(myLastId, wxNOT_FOUND);
    TS_ASSERT(m_pDB->DeleteQuery(myLastId));
    wxLogMessage(_T("Deleting Selected queries n.%ld from database"), myLastId);
  }

  void testQueryGeomOk() {
    TS_ASSERT(m_DataGeomLine->IsOk());
    TS_ASSERT(m_DataGeomNode->IsOk());
  }

  void testQueryLineBuild() {
    wxLogMessage(_T("Testing Geometry Query OK"));

    QueriesBuilder myBuilder(m_DataGeomLine);
    TS_ASSERT(myBuilder.IsOk());
    TS_ASSERT(myBuilder.Create(m_pDB));
  }

  void testQueryLineSizeSave() {
    QueriesBuilder myBuilder(m_DataGeomLine);

    TS_ASSERT(myBuilder.IsOk());
    TS_ASSERT(myBuilder.Create(m_pDB));
    TS_ASSERT(myBuilder.Save(m_pDB));
    wxLogMessage(_T("Saving  Line Size queries into database"));

    // delete last added query
    long myLastId = m_pDB->DataBaseGetLastInsertedID();
    TS_ASSERT_DIFFERS(myLastId, wxNOT_FOUND);
    TS_ASSERT(m_pDB->DeleteQuery(myLastId));
    wxLogMessage(_T("Deleting queries n.%ld from database"), myLastId);
  }

  void testQueryNodeNumberSave() {
    QueriesBuilder myBuilder(m_DataGeomNode);

    TS_ASSERT(myBuilder.IsOk());
    TS_ASSERT(myBuilder.Create(m_pDB));
    TS_ASSERT(myBuilder.Save(m_pDB));
    wxLogMessage(_T("Saving  Node number queries into database"));

    // delete last added query
    long myLastId = m_pDB->DataBaseGetLastInsertedID();
    TS_ASSERT_DIFFERS(myLastId, wxNOT_FOUND);
    TS_ASSERT(m_pDB->DeleteQuery(myLastId));
    wxLogMessage(_T("Deleting queries n.%ld from database"), myLastId);
  }
};

#endif
