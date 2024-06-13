/***************************************************************************
 test_tmprojectupdate2.cpp
 UNIT TESTING for project update
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

#include "../../src/core/tmprojectupdater.h"
#include "database_environnement.h"
#include "test_param.h"

// =================================================================
bool CopyDir2(wxString from, wxString to) {
    wxString SLASH = wxFILE_SEP_PATH;

    // append a slash if there is not one (for easier parsing)
    // because who knows what people will pass to the function.
    if (to[to.length() - 1] != SLASH) {
        to += SLASH;
    }
    // for both dirs
    if (from[from.length() - 1] != SLASH) {
        from += SLASH;
    }

    // first make sure that the source dir exists
    if (!wxDir::Exists(from)) {
        wxLogError(from + " does not exist.  Can not copy directory.");
    } else {
        // check on the destination dir
        // if it doesn't exist...
        if (!wxDir::Exists(to)) {
            // if it doesn't get created
            if (!wxFileName::Mkdir(to, 0777, wxPATH_MKDIR_FULL)) {
                // Send an error
                wxLogError(to + " could not be created.");
                // And exit gracefully
                return false;
            }
        }

        // The directories to traverse
        wxArrayString myDirs;
        myDirs.Add("");

        // loop through each directory.. storing all sub directories
        // and copying over all files.. the final iteration of one loop
        // should begin an iteration for any subdirectories discovered
        // on the previous pass
        // (rather than pragma, unsigned int will shut the MS compiler up)
        for (unsigned int i = 0; i < myDirs.size(); i++) {
            // get the next directory
            wxDir nextDir(from + myDirs[i]);

            // check that it exists in destination form
            if (!wxDir::Exists(to + myDirs[i])) {
                // if it doesn't, then create it
                if (!wxFileName::Mkdir(to + myDirs[i], 0777, wxPATH_MKDIR_FULL)) {
                    // If it doesn't create, error
                    wxLogError(to + myDirs[i] + " could not be created.");
                    // And exit gracefully
                    return false;
                }
            }

            // get the first file in the next directory
            wxString nextFile;
            bool process = nextDir.GetFirst(&nextFile);

            // and while there are still files to process
            while (process) {
                // If this file is a directory
                if (wxDir::Exists(from + nextFile)) {
                    // then append it for creation/copying
                    myDirs.Add(nextFile + SLASH);  // only add the difference
                } else {
                    // otherwise just go ahead and copy the file over
                    if (!wxCopyFile(from + myDirs[i] + nextFile, to + myDirs[i] + nextFile)) {
                        // error if we couldn't
                        wxLogError("Could not copy " + from + myDirs[i] + nextFile + " to " + to + myDirs[i] +
                                   nextFile);
                    }
                }
                // and get the next file
                process = nextDir.GetNext(&nextFile);
            }
        }
        return true;
    }
    return false;
}
// =================================================================

class TestProjectUpdater2 : public ::testing::Test {
  protected:
    DataBaseTM* m_CopyDB = DatabaseEnvironment::m_db;

    virtual void SetUp() {
        // remove if exists
        wxFileName myDir2(g_TestPathPRJ + g_TestMigre222 + _T("_tmp"), _T(""));
        if (wxDir::Exists(myDir2.GetFullPath())) {
            wxLogMessage(_T("Removing temporary project '%s'"), myDir2.GetFullPath());
            myDir2.Rmdir(wxPATH_RMDIR_RECURSIVE);
        }
        ASSERT_TRUE(!wxDir::Exists(myDir2.GetFullPath()));

        // copy project
        CopyDir2(g_TestPathPRJ + g_TestMigre222, g_TestPathPRJ + g_TestMigre222 + _T("_tmp"));
        ASSERT_TRUE(m_CopyDB->OpenTMDatabase(g_TestPathPRJ + g_TestMigre222 + _T("_tmp")));
    }
    virtual void TearDown() {
        ;
    }
};

TEST_F(TestProjectUpdater2, UpdateMigreProject) {
    PrjDefMemManage* myPrjDef = m_CopyDB->GetProjectDataFromDB();

    // read project
    ASSERT_TRUE(myPrjDef != nullptr);
    ProjectDefMemoryLayers* myLayer = myPrjDef->GetNextLayer();
    ASSERT_TRUE(myLayer != nullptr);
    ASSERT_EQ(myLayer->m_LayerName, _T("Boreholes_PT"));

    ProjectDefMemoryFields* myField = myPrjDef->GetNextField();
    ASSERT_TRUE(myField != nullptr);
    ASSERT_EQ(myField->m_Fieldname, _T("D_C_UNDERG"));

    ProjectDefMemoryFieldsCodedVal* myVal = myField->m_pCodedValueArray.Item(1);
    ASSERT_TRUE(myVal != nullptr);
    ASSERT_EQ(myVal->m_ValueCode, _T("2"));
    ASSERT_EQ(myVal->m_ValueName, _T("non"));

    // update val
    myVal->m_ValueCode = _T("45a");

    // update project
    m_CopyDB->UpdateDataBaseProject(myPrjDef);
    wxDELETE(myPrjDef);
    ASSERT_TRUE(myPrjDef == nullptr);

    // reread project
    wxLogMessage(_T("Re-reading project"));
    myPrjDef = m_CopyDB->GetProjectDataFromDB();
    ASSERT_TRUE(myPrjDef != nullptr);

    ProjectDefMemoryLayers* myLayer2 = myPrjDef->GetNextLayer();
    ASSERT_TRUE(myLayer2 != nullptr);

    ProjectDefMemoryFields* myField2 = myPrjDef->GetNextField();
    ASSERT_TRUE(myField != nullptr);

    ProjectDefMemoryFieldsCodedVal* myVal2 = myField2->m_pCodedValueArray.Item(1);
    ASSERT_TRUE(myVal2 != nullptr);
    ASSERT_EQ(myVal2->m_ValueCode, _T("45a"));
    ASSERT_EQ(myVal2->m_ValueName, _T("non"));

    wxDELETE(myPrjDef);
}
