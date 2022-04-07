/***************************************************************************
 test_pdf.h
 UNIT TESTING for pdf exporting
 -------------------
 copyright : (C) 2011 CREALP Lucien Schreiber
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
#include "../../src/core/pdfdocument.h"
#include "../../src/database/database_tm.h"


class TestPDF : public ::testing::Test {
 protected:
  DataBaseTM* m_db = nullptr;
  PrjDefMemManage *m_PrjDef = nullptr;

  virtual void SetUp() {
    m_db = new DataBaseTM();
    ASSERT_TRUE(m_db->OpenTMDatabase(g_TestPathPRJ + g_TestPrj_221));
    // load project Data
    m_PrjDef = m_db->GetProjectDataFromDB();
    ASSERT_TRUE(m_PrjDef != NULL);
    ASSERT_NE(m_PrjDef->GetCountLayers(), 0);
  }
  virtual void TearDown() {
    wxDELETE(m_db);
    wxDELETE(m_PrjDef);
  }
};

TEST_F(TestPDF, Generate1) {
  PdfDocument myPdf(m_PrjDef);
  EXPECT_TRUE(myPdf.Generate(wxFileName(g_TestPathEXPORT, "export-A4-portrait.pdf")));
}

TEST_F(TestPDF, Generate2) {
  PdfDocument myPdf(m_PrjDef);
  myPdf.SetPaperOrientation(wxLANDSCAPE);
  myPdf.SetTwoColsLayout(true);
  EXPECT_TRUE(myPdf.Generate(wxFileName(g_TestPathEXPORT, "export-A4-landscape.pdf")));
}

TEST_F(TestPDF, Generate3) {
  PdfDocument myPdf(m_PrjDef);
  myPdf.SetPaperOrientation(wxPORTRAIT);
  myPdf.SetTwoColsLayout(true);
  myPdf.SetPaperFormat(wxPAPER_A3);
  myPdf.SetPageBreak(false);
  EXPECT_TRUE(myPdf.Generate(wxFileName(g_TestPathEXPORT, "export-A3-portraittwo.pdf")));
}

TEST_F(TestPDF, Generate4) {
  PdfDocument myPdf(m_PrjDef);
  myPdf.SetTwoColsLayout(true);
  myPdf.SetOnePage(true);
  myPdf.SetPaperSize(296, 717);
  EXPECT_TRUE(myPdf.Generate(wxFileName(g_TestPathEXPORT, "export-onepage_two.pdf")));
}

TEST_F(TestPDF, Generate5) {
  PdfDocument myPdf(m_PrjDef);
  myPdf.SetTwoColsLayout(true);
  myPdf.SetPaperFormat(wxPAPER_A3);
  myPdf.SetPageBreak(false);
  myPdf.SetDecorate(false);
  EXPECT_TRUE(myPdf.Generate(wxFileName(g_TestPathEXPORT, "export-A3-portraittwo_nodecoration.pdf")));
}

TEST_F(TestPDF, Generate6) {
  PdfDocument myPdf(m_PrjDef);
  myPdf.SetTwoColsLayout(true);
  myPdf.SetPaperFormat(wxPAPER_A4);
  myPdf.SetPageBreak(false);
  EXPECT_TRUE(myPdf.Generate(wxFileName(g_TestPathEXPORT, "export-A4-portraittwo_nobreak.pdf")));
}

TEST_F(TestPDF, Generate7) {
  PdfDocument myPdf(m_PrjDef);
  myPdf.SetPageBreak(false);
  myPdf.SetOnePage(true);
  myPdf.SetPaperSize(148, 881);
  EXPECT_TRUE(myPdf.Generate(wxFileName(g_TestPathEXPORT, "export-onepage.pdf")));
}