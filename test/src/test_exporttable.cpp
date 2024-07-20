#include "gtest/gtest.h"

#include "test_param.h"
#include "../../src/database/tableexport.h"
#include "database_environnement.h"

// open the combioula correct database and try to
// export the zoom_table to a csv file

class TableExportTest : public ::testing::Test {
  protected:
    DataBaseTM* m_pDB = DatabaseEnvironment::m_db;
    wxFileName m_export_path = wxFileName(g_TestPathEXPORT + "test_export_table.csv");

    void SetUp() override {
        ASSERT_TRUE(m_pDB->OpenTMDatabase(g_TestPathPRJ + g_TestPrj_CombioulaCorrect));
        // remove the export file if it exists
        if (m_export_path.FileExists()) {
            wxRemoveFile(m_export_path.GetFullPath());
        }
        ASSERT_FALSE(m_export_path.Exists());
    }

    void TearDown() override {

    }
};

TEST_F(TableExportTest, EmptyExportName) {
    TableExport tableExport(m_pDB);

    bool result = tableExport.ExportCSV(wxEmptyString, m_export_path);
    EXPECT_FALSE(result);
    ASSERT_FALSE(m_export_path.Exists());
}

TEST_F(TableExportTest, ExportZoomTable) {
    TableExport tableExport(m_pDB);

    bool result = tableExport.ExportCSV(TABLE_NAME_SCALE, m_export_path);
    EXPECT_TRUE(result);

    // check file content
    wxTextFile file(m_export_path.GetFullPath());
    ASSERT_TRUE(file.Exists());
    ASSERT_TRUE(file.Open());
    ASSERT_EQ(file.GetLineCount(), 4);
    ASSERT_EQ(file.GetFirstLine(), "ZOOM_ID;SCALE_VALUE;RANK;");
    ASSERT_EQ(file.GetNextLine(), "1;5000;2;");
    ASSERT_EQ(file.GetNextLine(), "2;10000;1;");
    ASSERT_EQ(file.GetNextLine(), "4;50000;0;");
}

TEST_F(TableExportTest, ExportZoomTableWithAddTableName) {
    TableExport tableExport(m_pDB);

    bool result = tableExport.ExportCSV(TABLE_NAME_SCALE, m_export_path, 2, true);
    EXPECT_TRUE(result);

    // check file content
    wxTextFile file(m_export_path.GetFullPath());
    ASSERT_TRUE(file.Exists());
    ASSERT_TRUE(file.Open());
    ASSERT_EQ(file.GetLineCount(), 4);
    ASSERT_EQ(file.GetFirstLine(), "zoom_level");
    ASSERT_EQ(file.GetNextLine(), "ZOOM_ID;SCALE_VALUE;RANK;");
    ASSERT_EQ(file.GetNextLine(), "1;5000;2;");
    ASSERT_EQ(file.GetNextLine(), "2;10000;1;");
}
