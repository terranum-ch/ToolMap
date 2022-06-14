/***************************************************************************
 test_new_database_tm.h
 Testing Database TM class
 -------------------
 copyright : (C) 2012 CREALP Lucien Schreiber
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "gtest/gtest.h"

#include "test_param.h"
#include "../../src/database/database_tm.h"


class TestNewDatabaseTM : public ::testing::Test {
 protected:
  DataBaseTM* m_DB = nullptr;

  virtual void SetUp() {
    m_DB = new DataBaseTM();
  }
  virtual void TearDown() {
    wxDELETE(m_DB);
  }
};

TEST_F(TestNewDatabaseTM, CreateNewTMDatabase) {
  // remove if exists
  wxString myPrjName = g_TestPathPRJ + g_TestPrj_222;
  if (wxDirExists(myPrjName)) {
    wxLogMessage("Removing : " + myPrjName);
    wxFileName::Rmdir(myPrjName, wxPATH_RMDIR_RECURSIVE);
  }
  EXPECT_FALSE(wxDirExists(myPrjName));

  PrjDefMemManage myPrjdef;
  myPrjdef.m_PrjName = g_TestPrj_222;
  myPrjdef.m_PrjPath = g_TestPathPRJ;
  myPrjdef.m_PrjProjType = PROJ_SWISS_CH1903;
  myPrjdef.m_PrjUnitType = UNIT_METERS;

  EXPECT_TRUE(m_DB->CreateTMDatabase(&myPrjdef));
}

TEST_F(TestNewDatabaseTM, AddingLayerData) {
  EXPECT_TRUE(m_DB->OpenTMDatabase(g_TestPathPRJ + g_TestPrj_222));
  EXPECT_TRUE(m_DB->DataBaseGetName() == g_TestPrj_222);

  ProjectDefMemoryLayers myLayer;
  myLayer.m_LayerName = _T("Tectonic_Boundaries_L");
  myLayer.m_LayerType = LAYER_LINE;

  // adding layer
  EXPECT_TRUE(m_DB->AddLayer(&myLayer));

  // field1
  ProjectDefMemoryFields myField1;
  myField1.m_Fieldname = _T("Activity");
  myField1.m_FieldType = TM_FIELD_ENUMERATION;

  myField1.m_pCodedValueArray.Add(new ProjectDefMemoryFieldsCodedVal("1", "actif"));
  myField1.m_pCodedValueArray.Add(new ProjectDefMemoryFieldsCodedVal("2a", "inactif"));
  // adding attributs

  EXPECT_TRUE(m_DB->AddField(&myField1));
}

TEST_F(TestNewDatabaseTM, GetProjectInMemory) {
  EXPECT_TRUE(m_DB->OpenTMDatabase(g_TestPathPRJ + g_TestPrj_222));
  PrjDefMemManage* myPrjMem = m_DB->GetProjectDataFromDB();
  EXPECT_TRUE(myPrjMem != NULL);

  ProjectDefMemoryLayers* myLayer = myPrjMem->FindLayer(_T("Tectonic_Boundaries_L"));
  EXPECT_TRUE(myLayer != NULL);

  ProjectDefMemoryFieldsCodedVal* myVal = myLayer->m_pLayerFieldArray.Item(0)->m_pCodedValueArray.Item(1);
  EXPECT_TRUE(myVal != NULL);
  EXPECT_TRUE(myVal->m_ValueCode == _T("2a"));
  EXPECT_TRUE(myVal->m_ValueName == _T("inactif"));
  EXPECT_EQ(myLayer->m_pLayerFieldArray.Item(0)->m_FieldType, TM_FIELD_ENUMERATION);

  wxDELETE(myPrjMem);
}


TEST_F(TestNewDatabaseTM, DISABLED_UpdateProject) {
  // load project
  EXPECT_TRUE(m_DB->OpenTMDatabase(g_TestPathPRJ + g_TestPrj_222));
  PrjDefMemManage* myPrjMem = m_DB->GetProjectDataFromDB();
  EXPECT_TRUE(myPrjMem != NULL);

  ProjectDefMemoryLayers* myLayer = myPrjMem->FindLayer(_T("Tectonic_Boundaries_L"));
  EXPECT_TRUE(myLayer != NULL);

  ProjectDefMemoryFields* myField = myPrjMem->FindField(_T("Activity"));
  EXPECT_TRUE(myField != NULL);

  ProjectDefMemoryFieldsCodedVal* myVal = myLayer->m_pLayerFieldArray.Item(0)->m_pCodedValueArray.Item(1);
  EXPECT_TRUE(myVal != NULL);
  EXPECT_TRUE(myVal->m_ValueCode == _T("2a"));
  EXPECT_TRUE(myVal->m_ValueName == _T("inactif"));

  // change values for item 1
  myVal->m_ValueCode = _T("3b");
  myVal->m_ValueName = _T("inactiff");

  myPrjMem->RemoveCodedValue(0);

  // Add new item
  ProjectDefMemoryFieldsCodedVal* myNewCodedValue = myPrjMem->AddCodedValue();
  EXPECT_TRUE(myNewCodedValue);
  myNewCodedValue->m_ValueCode = _T("12");
  myNewCodedValue->m_ValueName = _T("Inconnu");

  // update project
  EXPECT_TRUE(m_DB->UpdateDataBaseProject(myPrjMem));

  wxDELETE(myPrjMem);
}