/***************************************************************************
 TEST_AdvAttribManager.h
 Testing Database class
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

#include "../../src/core/tmaattribbatchmanager.h"
#include "gtest/gtest.h"
#include "test_database_handle.h"
#include "test_param.h"

class TestAdvAttribManager : public DatabaseHandle {
 protected:
  PrjDefMemManage *m_prj = nullptr;

  virtual void SetUp() {
    ASSERT_TRUE(m_db->OpenTMDatabase(g_TestPathPRJ + g_TestPrj_AdvAttribution));
    m_prj = m_db->GetProjectDataFromDB();
    wxASSERT(m_prj);
  }
  virtual void TearDown() {
    wxDELETE(m_prj);
  }
};

TEST_F(TestAdvAttribManager, CreatorDtor) {
  tmSelectedDataMemory mySelData;
  mySelData.SetLayerID(0);
  wxArrayLong *mySelected = new wxArrayLong;
  mySelected->Add(1);
  mySelected->Add(2);
  mySelected->Add(3);
  mySelected->Add(10);
  mySelData.AddSelected(mySelected);

  tmAAttribBatchManager myManager(m_prj, m_db, &mySelData, LAYER_LINE);
  EXPECT_TRUE(myManager.IsOk());
}

TEST_F(TestAdvAttribManager, GettingTypes) {
  tmSelectedDataMemory mySelData;
  mySelData.SetLayerID(0);
  wxArrayLong *mySelected = new wxArrayLong;
  mySelected->Add(1);
  mySelected->Add(2);
  mySelected->Add(3);
  mySelected->Add(10);
  mySelData.AddSelected(mySelected);

  tmAAttribBatchManager myManager(m_prj, m_db, &mySelData, LAYER_LINE);
  PrjMemObjectsArray myObjects;
  wxArrayInt myNumber;
  wxArrayLong myLayerId;
  EXPECT_TRUE(myManager.GetTypes(myObjects, myNumber, myLayerId));
  EXPECT_EQ(myObjects.GetCount(), myNumber.GetCount());
  EXPECT_EQ(myNumber.GetCount(), myLayerId.GetCount());

  EXPECT_EQ(myObjects[0]->m_ObjectID, 1);
  EXPECT_EQ(myObjects[0]->m_ObjectName, _T("Fault"));
  EXPECT_EQ(myNumber[0], 2);
  EXPECT_EQ(myLayerId[0], 1);

  EXPECT_EQ(myObjects[1]->m_ObjectID, 3);
  EXPECT_EQ(myObjects[1]->m_ObjectName, _T("Border of Hazards_PLG"));
  EXPECT_EQ(myNumber[1], 3);
  EXPECT_EQ(myLayerId[1], 2);
}

TEST_F(TestAdvAttribManager, GettingFields) {
  tmSelectedDataMemory mySelData;
  mySelData.SetLayerID(0);
  wxArrayLong *mySelected = new wxArrayLong;
  mySelected->Add(1);
  mySelected->Add(2);
  mySelected->Add(3);
  mySelected->Add(10);
  mySelData.AddSelected(mySelected);
  tmAAttribBatchManager myManager(m_prj, m_db, &mySelData, LAYER_LINE);

  // Field 1 is TectoBound_L
  PrjMemFieldArray myFields;
  EXPECT_TRUE(myManager.GetFields(1, myFields));
  EXPECT_EQ(myFields[0]->m_FieldID, 1);
  EXPECT_EQ(myFields[0]->m_Fieldname, _T("TYPE"));
  EXPECT_EQ(myFields.GetCount(), 1);

  // layer 2 is Border of Rocks (no field should be returned
  // because of different spatial type
  EXPECT_FALSE(myManager.GetFields(2, myFields));
  EXPECT_EQ(myFields.GetCount(), 0);
}

TEST_F(TestAdvAttribManager, Attribute) {
  tmSelectedDataMemory mySelData;
  mySelData.SetLayerID(0);  // line ??
  wxArrayLong *mySelected = new wxArrayLong;
  mySelected->Add(8);   // faille
  mySelected->Add(9);   // faille, border of hazards_plg, campaign_10
  mySelected->Add(10);  // border of hazards_plg
  mySelData.AddSelected(mySelected);

  tmAAttribBatchManager myManager(m_prj, m_db, &mySelData, LAYER_LINE);

  PrjMemFieldArray myFields;
  EXPECT_TRUE(myManager.GetFields(1, myFields));
  ProjectDefMemoryFields myTestField = *(myFields.Item(0));
  EXPECT_TRUE(myTestField.m_Fieldname == _T("TYPE"));

  // try attribution
  int attributedObjects = myManager.Attribute(1, myTestField, _T("Secondary"));
  EXPECT_EQ(attributedObjects, 2);
}
