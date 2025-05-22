/***************************************************************************
 test_database.h
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

#include "gtest/gtest.h"

#include "test_param.h"
#include "../../src/database/database.h"
#include "database_environnement.h"


class TestDatabase : public ::testing::Test {
  protected:
    DataBase* m_db = DatabaseEnvironment::m_db;

    virtual void SetUp() {
        ;
    }
    virtual void TearDown() {
        ;
    }
};

TEST_F(TestDatabase, DatabaseOpen) {
    EXPECT_TRUE(m_db->DataBaseOpen(g_TestPathPRJ, g_TestPrj_CombioulaCorrect));
    wxString myFalsePath = g_TestPathPRJ + _T("JJJ");
    EXPECT_FALSE(m_db->DataBaseOpen(myFalsePath, g_TestPrj_CombioulaExercice));
    EXPECT_TRUE(m_db->DataBaseOpen(g_TestPathPRJ, g_TestPrj_CombioulaExercice));
}

TEST_F(TestDatabase, HasResults) {
    EXPECT_FALSE(m_db->DataBaseHasResults());
    ASSERT_TRUE(m_db->DataBaseOpen(g_TestPathPRJ, g_TestPrj_CombioulaCorrect));
    EXPECT_TRUE(m_db->DataBaseQuery(_T("SELECT OBJECT_ID FROM generic_lines")));
    EXPECT_TRUE(m_db->DataBaseHasResults());
    m_db->DataBaseClearResults();
    EXPECT_TRUE(m_db->DataBaseQuery(_T("SELECT OBJECT_ID FROM generic_lines WHERE OBJECT_ID = 1000000")));
    EXPECT_FALSE(m_db->DataBaseHasResults());
}

TEST_F(TestDatabase, QueryNoResults) {
    ASSERT_TRUE(m_db->DataBaseOpen(g_TestPathPRJ, g_TestPrj_CombioulaCorrect));
    EXPECT_TRUE(m_db->DataBaseQueryNoResults(_T("SELECT OBJECT_ID FROM generic_lines")));
    EXPECT_FALSE(m_db->DataBaseQueryNoResults(_T("SELECT COUNT(*) FROM generic_linesss")));
}

TEST_F(TestDatabase, ResultString) {
    ASSERT_TRUE(m_db->DataBaseOpen(g_TestPathPRJ, g_TestPrj_CombioulaCorrect));
    wxString myReturnedString = wxEmptyString;
    EXPECT_TRUE(m_db->DataBaseQuery(_T("SELECT OBJECT_DESC FROM dmn_layer_object WHERE OBJECT_ID = 16")));
    // second query should fail, must delete results first.
    m_db->DataBaseClearResults();
    EXPECT_TRUE(m_db->DataBaseQuery(_T("SELECT OBJECT_DESC FROM dmn_layer_object WHERE OBJECT_ID = 16")));
    EXPECT_TRUE(m_db->DataBaseGetNextResult(myReturnedString));
    EXPECT_EQ(myReturnedString, _T("delimitation tassement"));  // oid = 17
    EXPECT_FALSE(m_db->DataBaseGetNextResult(myReturnedString));
    EXPECT_EQ(myReturnedString, wxEmptyString);
    m_db->DataBaseClearResults();

    EXPECT_TRUE(m_db->DataBaseQuery(_T("SELECT OBJECT_ID FROM dmn_layer_object WHERE OBJECT_ID = 17777")));
    EXPECT_TRUE(!m_db->DataBaseGetNextResult(myReturnedString));
    EXPECT_EQ(myReturnedString, wxEmptyString);
    m_db->DataBaseClearResults();
}

TEST_F(TestDatabase, ResultArrayString) {
    ASSERT_TRUE(m_db->DataBaseOpen(g_TestPathPRJ, g_TestPrj_CombioulaCorrect));
    EXPECT_TRUE(m_db->DataBaseQuery(_T("SELECT * FROM dmn_layer_object WHERE OBJECT_ID = 16")));
    wxArrayString myResults;

    EXPECT_TRUE(m_db->DataBaseGetNextResult(myResults));
    EXPECT_EQ(myResults.GetCount(), 9);  // 9 cols in dmn_layer_object
    EXPECT_EQ(myResults.Item(4), _T("delimitation tassement"));
    m_db->DataBaseClearResults();

    EXPECT_TRUE(m_db->DataBaseQuery(_T("SELECT OBJECT_ID FROM dmn_layer_object WHERE OBJECT_ID = 17")));
    EXPECT_TRUE(m_db->DataBaseGetNextResult(myResults));
    EXPECT_EQ(myResults.GetCount(), 1);
    m_db->DataBaseClearResults();

    EXPECT_FALSE(m_db->DataBaseQuery(_T("SELECT OBJECT_ID FROM dmn_layer_object WHERE OBJCT_ID = 17777")));
    EXPECT_FALSE(m_db->DataBaseGetNextResult(myResults));
    EXPECT_EQ(myResults.GetCount(), 0);
}

TEST_F(TestDatabase, CountResults) {
    ASSERT_TRUE(m_db->DataBaseOpen(g_TestPathPRJ, g_TestPrj_CombioulaCorrect));
    unsigned int myCols = 0;
    long myRows = 0;
    EXPECT_FALSE(m_db->DataBaseGetResultSize(&myCols, &myRows));
    EXPECT_TRUE(m_db->DataBaseQuery(_T("SELECT * FROM dmn_layer_object WHERE OBJECT_ID = 17")));
    EXPECT_TRUE(m_db->DataBaseGetResultSize(&myCols, nullptr));
    EXPECT_EQ(myCols, 9);
    EXPECT_TRUE(m_db->DataBaseGetResultSize(nullptr, &myRows));
    EXPECT_EQ(myRows, 1);
    myRows = 0;
    myCols = 0;
    EXPECT_TRUE(m_db->DataBaseGetResultSize(&myCols, &myRows));
    EXPECT_EQ(myRows, 1);
    EXPECT_EQ(myCols, 9);
    m_db->DataBaseClearResults();
}

TEST_F(TestDatabase, ResultLong) {
    ASSERT_TRUE(m_db->DataBaseOpen(g_TestPathPRJ, g_TestPrj_CombioulaCorrect));
    long myResult = 0;
    EXPECT_TRUE(m_db->DataBaseQuery(_T("SELECT OBJECT_ID FROM dmn_layer_object WHERE OBJECT_ID = 17")));
    EXPECT_TRUE(m_db->DataBaseGetNextResult(myResult));
    EXPECT_EQ(myResult, 17);
    m_db->DataBaseClearResults();
}

TEST_F(TestDatabase, ResultArrayLong) {
    ASSERT_TRUE(m_db->DataBaseOpen(g_TestPathPRJ, g_TestPrj_CombioulaCorrect));
    EXPECT_TRUE(m_db->DataBaseQuery(_T("SELECT OBJECT_ID, OBJECT_CD FROM dmn_layer_object WHERE OBJECT_ID <= 17")));

    unsigned int myCols = 0;
    long myRows = 0;

    EXPECT_TRUE(m_db->DataBaseGetResultSize(&myCols, &myRows));
    EXPECT_EQ(myCols, 2);
    EXPECT_EQ(myRows, 17);

    wxArrayLong myResults;
    int i = 0;
    while (true) {
        bool bReturn = m_db->DataBaseGetNextResult(myResults);
        if (i < 17) {
            EXPECT_TRUE(bReturn);
        } else
            EXPECT_FALSE(bReturn);
        i++;
        if (!bReturn) break;
    }

    m_db->DataBaseClearResults();
    EXPECT_TRUE(
        m_db->DataBaseQuery(_T("SELECT OBJECT_ID FROM dmn_layer_object WHERE OBJECT_ID <= 17 ORDER BY OBJECT_ID")));
    EXPECT_TRUE(m_db->DataBaseGetNextResult(myResults));
    EXPECT_TRUE(m_db->DataBaseGetResultSize(&myCols, &myRows));
    EXPECT_EQ(myCols, 1);
    EXPECT_EQ(myRows, 17);
    m_db->DataBaseClearResults();
}

TEST_F(TestDatabase, ResultDouble) {
    ASSERT_TRUE(m_db->DataBaseOpen(g_TestPathPRJ, g_TestPrj_Fields));
    EXPECT_TRUE(m_db->DataBaseQuery(_T("SELECT TestFloat32 FROM layer_at1 WHERE OBJECT_ID = 1")));
    wxDouble myValue = 0;
    EXPECT_TRUE(m_db->DataBaseGetNextResult(myValue));
    EXPECT_EQ(myValue, 8.99);
    m_db->DataBaseClearResults();
}

TEST_F(TestDatabase, ResultArrayDouble) {
    ASSERT_TRUE(m_db->DataBaseOpen(g_TestPathPRJ, g_TestPrj_Fields));
    EXPECT_TRUE(m_db->DataBaseQuery(_T("SELECT TestFloat32 FROM layer_at1 ORDER BY OBJECT_ID")));
    wxArrayDouble values;
    EXPECT_TRUE(m_db->DataBaseGetNextResult(values));
    EXPECT_EQ(values.Item(0), 8.99);
    m_db->DataBaseClearResults();
}

TEST_F(TestDatabase, ColResultsString) {
    ASSERT_TRUE(m_db->DataBaseOpen(g_TestPathPRJ, g_TestPrj_Fields));
    EXPECT_TRUE(m_db->DataBaseQuery(_T("SELECT TestText FROM layer_at1 ORDER BY OBJECT_ID")));
    wxArrayString myResults;
    EXPECT_TRUE(m_db->DataBaseGetResults(myResults));
    EXPECT_EQ(myResults.GetCount(), 2);
    EXPECT_EQ(myResults.Item(1), _T("Ceci est un test pour un max de caracteres ke lonp"));
    EXPECT_FALSE(m_db->DataBaseGetResults(myResults));
    EXPECT_EQ(myResults.GetCount(), 0);
    EXPECT_TRUE(m_db->DataBaseQuery(_T("SELECT TestText FROM layer_at1 ORDER BY OBJECT_ID")));
    m_db->DataBaseClearResults();
}

TEST_F(TestDatabase, ColResultsLong) {
    ASSERT_TRUE(m_db->DataBaseOpen(g_TestPathPRJ, g_TestPrj_Fields));
    EXPECT_TRUE(m_db->DataBaseQuery(_T("SELECT OBJECT_ID FROM layer_at1 ORDER BY OBJECT_ID")));
    wxArrayLong myResults;
    EXPECT_TRUE(m_db->DataBaseGetResults(myResults));
    EXPECT_EQ(myResults.GetCount(), 2);
    EXPECT_EQ(myResults.Item(1), 4);
    m_db->DataBaseClearResults();
}

TEST_F(TestDatabase, ColResultsDouble) {
    ASSERT_TRUE(m_db->DataBaseOpen(g_TestPathPRJ, g_TestPrj_Fields));
    EXPECT_TRUE(m_db->DataBaseQuery(_T("SELECT TestFloat32 FROM layer_at1 ORDER BY OBJECT_ID")));
    wxArrayDouble myResults;
    EXPECT_TRUE(m_db->DataBaseGetResults(myResults));
    EXPECT_EQ(myResults.GetCount(), 2);
    EXPECT_EQ(myResults.Item(1), 6.00);
    m_db->DataBaseClearResults();
}

TEST_F(TestDatabase, PathName) {
    ASSERT_TRUE(m_db->DataBaseOpen(g_TestPathPRJ, g_TestPrj_Fields));

    EXPECT_EQ(m_db->DataBaseGetName(), g_TestPrj_Fields);
    EXPECT_EQ(m_db->DataBaseGetPath(), g_TestPathPRJ);

    wxString myFalseName = g_TestPrj_Fields + _T("ssss");
    ASSERT_TRUE(!m_db->DataBaseOpen(g_TestPathPRJ, myFalseName));
    EXPECT_EQ(m_db->DataBaseGetName(), wxEmptyString);
    EXPECT_EQ(m_db->DataBaseGetPath(), wxEmptyString);
}

TEST_F(TestDatabase, QueriesNumber) {
    wxString myQueries = _T("SELECT * FROM COUCOU; INSERT INTO ..; SELECT ALL");
    EXPECT_EQ(m_db->DataBaseQueriesNumber(myQueries), 3);
    wxString myQueries2 = _T("SELECT * FROM COUCOU; INSERT INTO; SELECT ALL;");
    EXPECT_EQ(m_db->DataBaseQueriesNumber(myQueries2), 3);
}

TEST_F(TestDatabase, Version) {
    EXPECT_TRUE(DataBase::DataBaseGetVersion() == _T("10.6.22-MariaDB"));
}

TEST_F(TestDatabase, CreateNewDatabase) {
    EXPECT_FALSE(m_db->DataBaseCreateNew(g_TestPathPRJ, g_TestPrj_MyTest));
    ASSERT_TRUE(m_db->DataBaseOpen(g_TestPathPRJ, g_TestPrj_MyTest));
    EXPECT_TRUE(m_db->DataBaseQuery(_T("SHOW TABLES FROM mytest1")));
    m_db->DataBaseClearResults();
}

TEST_F(TestDatabase, GetDataBaseSize) {
    wxString myFailMsg = _("Not available");
    ASSERT_TRUE(m_db->DataBaseOpen(g_TestPathPRJ, g_TestPrj_MyTest));
    wxString myDBSize = m_db->DataBaseGetSize(2, myFailMsg);
    wxLogMessage(myDBSize);
    EXPECT_FALSE(myDBSize == myFailMsg);

    ASSERT_TRUE(m_db->DataBaseOpen(g_TestPathPRJ, g_TestPrj_CombioulaCorrect));
    myDBSize = m_db->DataBaseGetSize(2, myFailMsg);
    wxLogDebug(myDBSize);
    EXPECT_FALSE(myDBSize == myFailMsg);
}

TEST_F(TestDatabase, GetLastInsertID) {
    ASSERT_TRUE(m_db->DataBaseOpen(g_TestPathPRJ, g_TestPrj_Fields));
    long myIID = m_db->DataBaseGetLastInsertedID();
    EXPECT_EQ(myIID, wxNOT_FOUND);
    EXPECT_TRUE(
        m_db->DataBaseQueryNoResults(_T("INSERT INTO dmn_layer_object (OBJECT_CD, OBJECT_TYPE_CD, ")
                                     _T("THEMATIC_LAYERS_LAYER_INDEX, OBJECT_DESC) VALUES (1, 0, 1, 'test')")));

    myIID = m_db->DataBaseGetLastInsertedID();
    EXPECT_TRUE(myIID != wxNOT_FOUND);
    wxLogDebug(_T("Last inserted ID = %ld"), myIID);

    myIID = m_db->DataBaseGetLastInsertedID();
    EXPECT_TRUE(myIID != wxNOT_FOUND);
}

TEST_F(TestDatabase, GetRawRow) {
    ASSERT_TRUE(m_db->DataBaseOpen(g_TestPathPRJ, g_TestPrj_Fields));
    EXPECT_TRUE(m_db->DataBaseQuery(_T("SELECT Envelope(OBJECT_GEOMETRY) FROM generic_lines WHERE OBJECT_ID = 1")));
    MYSQL_ROW myRow;
    tmArrayULong myLength;
    EXPECT_TRUE(m_db->DataBaseGetNextRowResult(myRow, myLength));
    EXPECT_TRUE(myRow != nullptr);
    EXPECT_TRUE(myLength.GetCount() != 0);
    EXPECT_FALSE(m_db->DataBaseGetNextRowResult(myRow, myLength));
    EXPECT_EQ(myRow, nullptr);
    EXPECT_EQ(myLength.GetCount(), 0);
    m_db->DataBaseClearResults();
}

TEST_F(TestDatabase, DeleteDB) {
    // ensure database testedit_12 didn't exists
    wxLogMessage(_T("Removing : " + g_TestPathPRJ + _T("testedit_12")));
    if (wxFileName::Rmdir(g_TestPathPRJ + _T("testedit_12"), wxPATH_RMDIR_RECURSIVE)) {
        wxLogMessage(_T("Removed testedit_12 allready existing"));
    }

    // EXPECT_FALSE(m_db->DataBaseDelete());
    EXPECT_TRUE(m_db->DataBaseCreateNew(g_TestPathPRJ, _T("testedit_12")));
    EXPECT_TRUE(m_db->DataBaseDelete());
}

TEST_F(TestDatabase, EscapeString) {
    wxLogDebug(_T("Escaping special character"));
    wxString myBefore = _T("SELECT coucou'toi");
    wxString myAfter = wxEmptyString;

    ASSERT_TRUE(m_db->DataBaseOpen(g_TestPathPRJ, g_TestPrj_Fields));
    EXPECT_TRUE(m_db->DataBaseStringEscapeQuery(myBefore, myAfter));
    wxLogDebug(_T("before : %s - after : %s"), myBefore.c_str(), myAfter.c_str());
    EXPECT_EQ(myAfter, _T("SELECT coucou\\'toi"));
}
