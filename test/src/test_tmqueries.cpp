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

#include "../../src/core/queriesbuilder.h"
#include "../../src/core/queriesdata.h"
#include "gtest/gtest.h"
#include "test_param.h"

class TestQueries : public ::testing::Test {
 protected:
  DataBaseTM *m_pDB;
  QueriesData *m_DataLayer;
  QueriesData *m_DataSelected;
  QueriesData *m_DataGeneric;
  QueriesData *m_DataSQL;
  QueriesData *m_DataObjects;
  QueriesData *m_DataGeomLine;
  QueriesData *m_DataGeomNode;

  virtual void SetUp() {
    m_pDB = new DataBaseTM();
    ASSERT_TRUE(m_pDB->OpenTMDatabase(g_TestPathPRJ + g_TestPrj_Ricken));

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
    m_DataSelected->m_QueryObjectGeomID = 231;  // ligne attribuée à une faille + Border of rocks, attribution avancée vide
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

  virtual void TearDown() {
    wxDELETE(m_pDB);
    wxDELETE(m_DataLayer);
    wxDELETE(m_DataSelected);
    wxDELETE(m_DataGeneric);
    wxDELETE(m_DataSQL);
    wxDELETE(m_DataObjects);
    wxDELETE(m_DataGeomLine);
    wxDELETE(m_DataGeomNode);
  }
};

TEST_F(TestQueries, QueryLayers) {
  QueriesBuilder myBuilder(m_DataLayer);
  EXPECT_TRUE(myBuilder.IsOk());
}

TEST_F(TestQueries, QueryNoType) {
  QueriesData myData;
  QueriesBuilder myBuilder(&myData);
  EXPECT_FALSE(myBuilder.IsOk());
}

TEST_F(TestQueries, QuerySelected) {
  QueriesBuilder myBuilder(m_DataSelected);
  EXPECT_TRUE(myBuilder.IsOk());
}

TEST_F(TestQueries, QueryGeneric) {
  QueriesBuilder myBuilder(m_DataGeneric);
  EXPECT_TRUE(myBuilder.IsOk());
}

TEST_F(TestQueries, QuerySQL) {
  QueriesBuilder myBuilder(m_DataSQL);
  EXPECT_TRUE(myBuilder.IsOk());
}

TEST_F(TestQueries, IsCreated) {
  QueriesBuilder myBuilder(m_DataSQL);
  EXPECT_FALSE(myBuilder.Save(m_pDB));  // query not created
}

TEST_F(TestQueries, CreateGeneric) {
  QueriesBuilder myBuilder(m_DataGeneric);
  EXPECT_TRUE(myBuilder.Create(m_pDB));
  EXPECT_TRUE(myBuilder.Save(m_pDB));
  wxLogMessage(_T("Saving queries into database"));

  // delete last added query
  long myLastId = m_pDB->DataBaseGetLastInsertedID();
  EXPECT_NE(myLastId, wxNOT_FOUND);
  EXPECT_TRUE(m_pDB->DeleteQuery(myLastId));
  wxLogMessage(_T("Deleting Generic queries n.%ld from database"), myLastId);
}

TEST_F(TestQueries, CreateSQL) {
  QueriesBuilder myBuilder(m_DataSQL);
  EXPECT_TRUE(myBuilder.Create(m_pDB));
  EXPECT_TRUE(myBuilder.Save(m_pDB));

  // delete last added query
  long myLastId = m_pDB->DataBaseGetLastInsertedID();
  EXPECT_NE(myLastId, wxNOT_FOUND);
  EXPECT_TRUE(m_pDB->DeleteQuery(myLastId));
  wxLogMessage(_T("Deleting Expert queries n.%ld from database"), myLastId);
}

TEST_F(TestQueries, CreateLayers) {
  QueriesBuilder myBuilder(m_DataLayer);
  EXPECT_TRUE(myBuilder.Create(m_pDB));
  EXPECT_TRUE(myBuilder.Save(m_pDB));

  // delete last added query
  long myLastId = m_pDB->DataBaseGetLastInsertedID();
  EXPECT_NE(myLastId, wxNOT_FOUND);
  EXPECT_TRUE(m_pDB->DeleteQuery(myLastId));
  wxLogMessage(_T("Deleting layers queries n.%ld from database"), myLastId);
}

TEST_F(TestQueries, ListLayers) {
  PrjMemLayersArray myLayers;

  EXPECT_TRUE(m_DataLayer->GetLayers(m_pDB, myLayers));
  EXPECT_EQ(myLayers.GetCount(), 9);

  EXPECT_EQ(myLayers[0]->m_LayerName, _T("Boreholes_PT"));
  EXPECT_EQ(myLayers[1]->m_LayerName, _T("WatOutflow_PT"));
  EXPECT_EQ(myLayers[2]->m_LayerName, _T("GravitFeat_PLG"));
  EXPECT_EQ(myLayers[3]->m_LayerName, _T("GravitFeat_L"));
  EXPECT_EQ(myLayers[4]->m_LayerName, _T("GlacStruct_L"));
  EXPECT_EQ(myLayers[5]->m_LayerName, _T("Schistosity_PT"));
  EXPECT_EQ(myLayers[6]->m_LayerName, _T("TectoBound_L"));
  EXPECT_EQ(myLayers[7]->m_LayerName, _T("Drifts_PLG"));
  EXPECT_EQ(myLayers[8]->m_LayerName, _T("Rocks_PLG"));
}

TEST_F(TestQueries, ListObjectValue) {
  PrjMemObjectsArray myTypes;

  // lignes n.231 (Faille et Border of Rocks_PLG)
  EXPECT_TRUE(m_DataSelected->GetObjectsForSelection(m_pDB, myTypes));
  EXPECT_EQ(myTypes.GetCount(), 2);
  EXPECT_EQ(myTypes.Item(1)->m_ObjectName, _T("faille"));
  EXPECT_EQ(myTypes.Item(1)->m_ObjectID, 32);
}

TEST_F(TestQueries, IsGenericLayer) {
  EXPECT_FALSE(m_DataSelected->IsGenericLayer(m_pDB, 0));
  EXPECT_EQ(m_DataSelected->m_QueryLayerType, TOC_NAME_LINES);
  EXPECT_TRUE(m_DataSelected->IsGenericLayer(m_pDB, 1));
  EXPECT_EQ(m_DataSelected->m_QueryLayerType, TOC_NAME_LINES);
  EXPECT_TRUE(m_DataSelected->IsGenericLayer(m_pDB, 2));
  EXPECT_EQ(m_DataSelected->m_QueryLayerType, TOC_NAME_POINTS);
  EXPECT_TRUE(m_DataSelected->IsGenericLayer(m_pDB, 3));
  EXPECT_EQ(m_DataSelected->m_QueryLayerType, TOC_NAME_LABELS);
  EXPECT_TRUE(m_DataSelected->IsGenericLayer(m_pDB, 4));
  EXPECT_EQ(m_DataSelected->m_QueryLayerType, TOC_NAME_ANNOTATIONS);
  EXPECT_FALSE(m_DataSelected->IsGenericLayer(m_pDB, 5));
  EXPECT_EQ(m_DataSelected->m_QueryLayerType, TOC_NAME_LINES);
  EXPECT_FALSE(m_DataSelected->IsGenericLayer(m_pDB, 31));
  EXPECT_EQ(m_DataSelected->m_QueryLayerType, TOC_NAME_LINES);
  EXPECT_FALSE(m_DataSelected->IsGenericLayer(m_pDB, 161));
  EXPECT_EQ(m_DataSelected->m_QueryLayerType, TOC_NAME_LINES);
}

TEST_F(TestQueries, CreateSelectedNoAttribs) {
  QueriesBuilder myBuilder(m_DataSelected);
  EXPECT_TRUE(myBuilder.Create(m_pDB));
  EXPECT_TRUE(myBuilder.Save(m_pDB));
  wxLogMessage(_T("Saving queries into database"));

  // delete last added query
  long myLastId = m_pDB->DataBaseGetLastInsertedID();
  EXPECT_NE(myLastId, wxNOT_FOUND);
  EXPECT_TRUE(m_pDB->DeleteQuery(myLastId));
  wxLogMessage(_T("Deleting Selected queries n.%ld from database"), myLastId);
}

TEST_F(TestQueries, CreateSelectionWithoutAttribution) {
  QueriesBuilder myBuilder(m_DataSelected);
  m_DataSelected->m_QueryFieldsStatus = AATTRIBUTION_EMPTY;

  EXPECT_FALSE(myBuilder.IsOk());
  long myLayerID = wxNOT_FOUND;
  EXPECT_TRUE(m_DataSelected->GetParentLayer(m_pDB, myLayerID));
  EXPECT_NE(myLayerID, -1);
  EXPECT_TRUE(m_DataSelected->GetFieldsValues(m_pDB, myLayerID, m_DataSelected->m_QueryFields,
                                              m_DataSelected->m_QueryFieldsValues));
  EXPECT_TRUE(myBuilder.IsOk());
  EXPECT_TRUE(myBuilder.Create(m_pDB));
  EXPECT_TRUE(myBuilder.Save(m_pDB));
  // wxLogMessage(_T("Saving  selected queries into database"));

  // delete last added query
  long myLastId = m_pDB->DataBaseGetLastInsertedID();
  EXPECT_NE(myLastId, wxNOT_FOUND);
  EXPECT_TRUE(m_pDB->DeleteQuery(myLastId));
  wxLogMessage(_T("Deleting Selected queries n.%ld from database"), myLastId);
}

TEST_F(TestQueries, CreateSelectionQueryAttribut) {
  QueriesBuilder myBuilder(m_DataSelected);
  m_DataSelected->m_QueryFieldsStatus = AATTRIBUTION_YES;
  m_DataSelected->m_QueryObjectGeomID = 140;

  EXPECT_FALSE(myBuilder.IsOk());

  long myLayerID = wxNOT_FOUND;
  EXPECT_TRUE(m_DataSelected->GetParentLayer(m_pDB, myLayerID));
  EXPECT_NE(myLayerID, -1);
  EXPECT_TRUE(m_DataSelected->GetFieldsValues(m_pDB, myLayerID, m_DataSelected->m_QueryFields,
                                            m_DataSelected->m_QueryFieldsValues));
  EXPECT_TRUE(myBuilder.IsOk());
  EXPECT_TRUE(myBuilder.Create(m_pDB));
  EXPECT_TRUE(myBuilder.Save(m_pDB));

  // delete last added query
  long myLastId = m_pDB->DataBaseGetLastInsertedID();
  EXPECT_NE(myLastId, wxNOT_FOUND);
  EXPECT_TRUE(m_pDB->DeleteQuery(myLastId));
  wxLogMessage(_T("Deleting Selected queries n.%ld from database"), myLastId);
}

TEST_F(TestQueries, GetValues) {
  wxArrayString myValues;
  long myLayerID = 7;   // Tectobound_L pour faille.
  long myLayerID2 = 4;  // GlacStruct, pas de champs
  PrjMemFieldArray myFields;

  EXPECT_FALSE(m_DataSelected->GetFieldsValues(m_pDB, myLayerID2, myFields, myValues));
  EXPECT_EQ(myFields.GetCount(), 0);
  EXPECT_EQ(myValues.GetCount(), 0);

  // getting fields and values
  EXPECT_TRUE(m_DataSelected->GetFieldsValues(m_pDB, myLayerID, myFields, myValues));
  EXPECT_EQ(myFields.GetCount(), 4);
  EXPECT_EQ(myValues.GetCount(), 4);
  EXPECT_EQ(myValues.Item(3), wxEmptyString);
}

TEST_F(TestQueries, GetAttribValues) {
  m_DataSelected->m_QueryObjectGeomID = 140;  // faille avec attrib avancés

  wxArrayString myValues;
  PrjMemFieldArray myFields;

  // fields availlable
  EXPECT_TRUE(m_DataSelected->GetFieldsValues(m_pDB, 7, myFields, myValues));
  EXPECT_EQ(myFields.GetCount(), 4);
  EXPECT_EQ(myValues.GetCount(), 4);
  EXPECT_EQ(myValues.Item(0), _T("probable"));  // probable
  EXPECT_EQ(myValues.Item(1), _T("majeure"));   // majeure
  EXPECT_EQ(myValues.Item(2), _T("normale"));   // normale
  EXPECT_EQ(myValues.Item(3), _T("1"));         // 1

  // deuxième fois
  EXPECT_TRUE(m_DataSelected->GetFieldsValues(m_pDB, 7, myFields, myValues));
  EXPECT_EQ(myFields.GetCount(), 4);
  EXPECT_EQ(myValues.GetCount(), 4);
  EXPECT_EQ(myValues.Item(0), _T("probable"));  // probable
  EXPECT_EQ(myValues.Item(1), _T("majeure"));   // majeure
  EXPECT_EQ(myValues.Item(2), _T("normale"));   // normale
  EXPECT_EQ(myValues.Item(3), _T("1"));         // 1
}

TEST_F(TestQueries, GetParentLayer) {
  long layerid = 0;
  EXPECT_TRUE(m_DataSelected->GetParentLayer(m_pDB, layerid));
  EXPECT_EQ(layerid, 7);
  m_DataSelected->m_QueryObjectID = 50;  // n'existe pas
  EXPECT_FALSE(m_DataSelected->GetParentLayer(m_pDB, layerid));
  EXPECT_EQ(layerid, wxNOT_FOUND);

  m_DataSelected->m_QueryObjectID = 41;  // moraine -> layer 8
  EXPECT_TRUE(m_DataSelected->GetParentLayer(m_pDB, layerid));
  EXPECT_EQ(layerid, 8);
}

TEST_F(TestQueries, DeleteFields) {
  // no fields
  EXPECT_FALSE(m_DataSelected->DeleteFieldsValue(2));

  // load fields
  m_DataSelected->m_QueryObjectGeomID = 140;  // faille avec attrib avancés
  EXPECT_TRUE(m_DataSelected->GetFieldsValues(m_pDB, 7, m_DataSelected->m_QueryFields,
                                              m_DataSelected->m_QueryFieldsValues));

  EXPECT_TRUE(m_DataSelected->DeleteFieldsValue(2));
  EXPECT_EQ(m_DataSelected->m_QueryFieldsValues.GetCount(), 3);
  EXPECT_EQ(m_DataSelected->m_QueryFields.GetCount(), 3);
  EXPECT_EQ(m_DataSelected->m_QueryFieldsValues.Item(2), _T("1"));
  EXPECT_EQ(m_DataSelected->m_QueryFields.Item(2)->m_Fieldname, _T("Thrust_class"));
}

TEST_F(TestQueries, AllAttribsEmpty) {
  EXPECT_EQ(m_DataSelected->HasFieldsValues(), false);

  // all fields empty
  // m_DataSelected->m_QueryUseFields = true;
  EXPECT_TRUE(m_DataSelected->GetFieldsValues(m_pDB, 7, m_DataSelected->m_QueryFields,
                                              m_DataSelected->m_QueryFieldsValues));
  EXPECT_EQ(m_DataSelected->HasFieldsValues(), false);

  m_DataSelected->m_QueryObjectGeomID = 140;  // faille avec attrib avancés
  EXPECT_TRUE(m_DataSelected->GetFieldsValues(m_pDB, 7, m_DataSelected->m_QueryFields,
                                              m_DataSelected->m_QueryFieldsValues));
  EXPECT_EQ(m_DataSelected->HasFieldsValues(), true);
}

TEST_F(TestQueries, GetObjForTypes) {
  PrjMemObjectsArray myObjects;
  EXPECT_TRUE(m_DataObjects->GetObjectsForTypes(m_pDB, myObjects));
  EXPECT_NE(myObjects.GetCount(), 0);
  wxLogMessage(_T("%zu objects returned for types : %s"), myObjects.GetCount(),
               PRJDEF_LAYERS_TYPE_STRING[m_DataSelected->m_QueryLayerType].c_str());

  m_DataObjects->m_QueryLayerType = TOC_NAME_POINTS;
  EXPECT_TRUE(m_DataObjects->GetObjectsForTypes(m_pDB, myObjects));
  EXPECT_NE(myObjects.GetCount(), 0);
  wxLogMessage(_T("%zu objects returned for types : %s"), myObjects.GetCount(),
               PRJDEF_LAYERS_TYPE_STRING[m_DataObjects->m_QueryLayerType].c_str());

  m_DataObjects->m_QueryLayerType = TOC_NAME_LABELS;
  EXPECT_TRUE(m_DataObjects->GetObjectsForTypes(m_pDB, myObjects));
  EXPECT_NE(myObjects.GetCount(), 0);
  wxLogMessage(_T("%zu objects returned for types : %s"), myObjects.GetCount(),
               PRJDEF_LAYERS_TYPE_STRING[m_DataObjects->m_QueryLayerType].c_str());
}

TEST_F(TestQueries, QueryObjectOk) {
  EXPECT_EQ(m_DataObjects->IsOk(), false);
  m_DataObjects->m_QueryObjectID = 32;
  EXPECT_EQ(m_DataObjects->IsOk(), true);
}

TEST_F(TestQueries, QueryObjectSave) {
  m_DataObjects->m_QueryObjectID = 32;
  QueriesBuilder myBuilder(m_DataObjects);

  EXPECT_TRUE(myBuilder.IsOk());
  EXPECT_TRUE(myBuilder.Create(m_pDB));
  EXPECT_TRUE(myBuilder.Save(m_pDB));
  wxLogMessage(_T("Saving  Object queries into database"));

  // delete last added query
  long myLastId = m_pDB->DataBaseGetLastInsertedID();
  EXPECT_NE(myLastId, wxNOT_FOUND);
  EXPECT_TRUE(m_pDB->DeleteQuery(myLastId));
  wxLogMessage(_T("Deleting Selected queries n.%ld from database"), myLastId);
}

TEST_F(TestQueries, QueryGeomOk) {
  EXPECT_TRUE(m_DataGeomLine->IsOk());
  EXPECT_TRUE(m_DataGeomNode->IsOk());
}

TEST_F(TestQueries, QueryLineBuild) {
  wxLogMessage(_T("Testing Geometry Query OK"));

  QueriesBuilder myBuilder(m_DataGeomLine);
  EXPECT_TRUE(myBuilder.IsOk());
  EXPECT_TRUE(myBuilder.Create(m_pDB));
}

TEST_F(TestQueries, QueryLineSizeSave) {
  QueriesBuilder myBuilder(m_DataGeomLine);

  EXPECT_TRUE(myBuilder.IsOk());
  EXPECT_TRUE(myBuilder.Create(m_pDB));
  EXPECT_TRUE(myBuilder.Save(m_pDB));
  wxLogMessage(_T("Saving  Line Size queries into database"));

  // delete last added query
  long myLastId = m_pDB->DataBaseGetLastInsertedID();
  EXPECT_NE(myLastId, wxNOT_FOUND);
  EXPECT_TRUE(m_pDB->DeleteQuery(myLastId));
  wxLogMessage(_T("Deleting queries n.%ld from database"), myLastId);
}

TEST_F(TestQueries, QueryNodeNumberSave) {
  QueriesBuilder myBuilder(m_DataGeomNode);

  EXPECT_TRUE(myBuilder.IsOk());
  EXPECT_TRUE(myBuilder.Create(m_pDB));
  EXPECT_TRUE(myBuilder.Save(m_pDB));
  wxLogMessage(_T("Saving  Node number queries into database"));

  // delete last added query
  long myLastId = m_pDB->DataBaseGetLastInsertedID();
  EXPECT_NE(myLastId, wxNOT_FOUND);
  EXPECT_TRUE(m_pDB->DeleteQuery(myLastId));
  wxLogMessage(_T("Deleting queries n.%ld from database"), myLastId);
}
