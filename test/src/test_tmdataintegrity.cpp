/***************************************************************************
 test_tmdataintegrity.h
 UNIT TESTING for data integrity
 -------------------
 copyright : (C) 2010 CREALP Lucien Schreiber
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
#include "../../src/core/tmdataintegrity.h"
#include "database_environnement.h"

class TestDataIntegrity : public ::testing::Test {
  protected:
    DataBaseTM* m_pDB = DatabaseEnvironment::m_db;

    virtual void SetUp() {
        ASSERT_TRUE(m_pDB->OpenTMDatabase(g_TestPathPRJ + g_TestPrj_Ricken));
    }
    virtual void TearDown() {
        ;
    }
};

TEST_F(TestDataIntegrity, Create) {
    tmDataIntegrity di(m_pDB);
    ASSERT_TRUE(di.GetLastError() == wxEmptyString);
}

TEST_F(TestDataIntegrity, HasErrorAAttrib) {
    tmDataIntegrity di(m_pDB);
    ASSERT_TRUE(di.HasErrorAAttrib(19, LAYER_POINT));  // WatOutflow_PT with one error
    wxLogMessage(di.GetLastError());
    ASSERT_TRUE(di.GetLastError() != wxEmptyString);

    ASSERT_FALSE(di.HasErrorAAttrib(12, LAYER_LINE));  // Gravifeat_L with no error
    wxLogMessage(di.GetLastError());
    ASSERT_TRUE(di.GetLastError() == wxEmptyString);
}

TEST_F(TestDataIntegrity, HasErrorType) {
    tmDataIntegrity di(m_pDB);

    ASSERT_TRUE(di.HasErrorType(LAYER_LINE));  // Error in lines
    wxLogMessage(di.GetLastError());
    ASSERT_TRUE(di.GetLastError() != wxEmptyString);

    ASSERT_FALSE(di.HasErrorType(LAYER_POLYGON));  // no error in labels
    wxLogMessage(di.GetLastError());
    ASSERT_TRUE(di.GetLastError() == wxEmptyString);

    ASSERT_FALSE(di.HasErrorType(LAYER_POINT));  // no error in points
    wxLogMessage(di.GetLastError());
    ASSERT_TRUE(di.GetLastError() == wxEmptyString);
}

TEST_F(TestDataIntegrity, CorrectType) {
    tmDataIntegrity di(m_pDB);
    ASSERT_TRUE(di.CorrectType(LAYER_LINE));
    ASSERT_FALSE(di.HasErrorType(LAYER_LINE));
}

TEST_F(TestDataIntegrity, CorrectAAttrib) {
    tmDataIntegrity di(m_pDB);
    ASSERT_TRUE(di.CorrectAAttrib(19, LAYER_POINT));
    ASSERT_FALSE(di.HasErrorAAttrib(19, LAYER_POINT));
}

TEST_F(TestDataIntegrity, AddErrorIntoDatabase) {
    // inserting one error into DB (will be corrected later)
    wxString myQuery = _T("INSERT INTO layer_at19 VALUES (92, 'gefasst')");
    ASSERT_TRUE(m_pDB->DataBaseQueryNoResults(myQuery));

    wxString myQuery2 = _T("INSERT INTO generic_aat VALUES (93, 2)");
    ASSERT_TRUE(m_pDB->DataBaseQueryNoResults(myQuery2));
}
