/***************************************************************************
 test_export.h
 UNIT TESTING for exporting data
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

#include "gtest/gtest.h"

#include "test_param.h"
#include "../../src/core/tmexportmanager.h"
#include "database_environnement.h"


class TestExportConcat : public ::testing::Test {
 protected:
  DataBaseTM *m_pDB = DatabaseEnvironment::m_db;
  PrjDefMemManage *m_PrjDef = nullptr;
  wxString *m_RealExportPath = nullptr;
  tmGISScale *m_ExportScale = nullptr;

  virtual void SetUp() {
    OGRRegisterAll();
    ASSERT_TRUE(m_pDB->OpenTMDatabase(g_TestPathPRJ + g_TestExportConcat));
    // load project Data
    m_PrjDef = m_pDB->GetProjectDataFromDB();
    ASSERT_TRUE(m_PrjDef != nullptr);
    ASSERT_NE(m_PrjDef->GetCountLayers(), 0);
    int iExportType = 0;
    wxString myTempString = wxEmptyString;
    ASSERT_NE(m_pDB->GetProjectExportData(iExportType, myTempString), PATH_DATABASE_ERROR);
    m_RealExportPath = new wxString(myTempString);

    ASSERT_TRUE(m_pDB->SetProjectExportData(EXPORT_SHAPEFILE, g_TestPathEXPORT));

    // init scale for export. The project we export are in meters
    // thus we can create this "fake" scale.
    m_ExportScale = new tmGISScale();
    m_ExportScale->SetExtentWndReal(tmRealRect(0, 0, 1, 1));
    m_ExportScale->SetWidthDistanceInM(1);
  }
  virtual void TearDown() {
    // reset path to old value
    ASSERT_TRUE(m_pDB->SetProjectExportData(EXPORT_SHAPEFILE, *m_RealExportPath));

    wxDELETE(m_PrjDef);
    wxDELETE(m_RealExportPath);
    wxDELETE(m_ExportScale);
  }
};

TEST_F(TestExportConcat, ExportLine) {
  tmExportManager myManager(nullptr, m_pDB, m_ExportScale);
  EXPECT_TRUE(myManager.ExportConcatenated(m_PrjDef, LAYER_LINE, false));
}

TEST_F(TestExportConcat, ExportPoint) {
  tmExportManager myManager(nullptr, m_pDB, m_ExportScale);
  EXPECT_TRUE(myManager.ExportConcatenated(m_PrjDef, LAYER_POINT, false));
}

TEST_F(TestExportConcat, ExportLabels) {
  tmExportManager myManager(NULL, m_pDB, m_ExportScale);
  EXPECT_TRUE(myManager.ExportConcatenated(m_PrjDef, LAYER_POLYGON, false));
}