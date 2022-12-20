#include <gtest/gtest.h>
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif
#include "database_environnement.h"

void DatabaseEnvironment::SetUp() {
    if (m_db == nullptr) {
        m_db = new DataBaseTM();
    }
}

void DatabaseEnvironment::TearDown() {
    wxDELETE(m_db);
}

DataBaseTM* DatabaseEnvironment::m_db = nullptr;

int main(int argc, char** argv) {
    wxInitialize();
    wxLog::SetActiveTarget(new wxLogStderr());
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::AddGlobalTestEnvironment(new DatabaseEnvironment);
    return RUN_ALL_TESTS();
}