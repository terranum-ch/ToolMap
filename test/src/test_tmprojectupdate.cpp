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
#include "../../src/core/tmprojectupdater.h"
#include "database_environnement.h"


// =================================================================
bool CopyDir(wxString from, wxString to) {
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
            wxLogError("Could not copy " + from + myDirs[i] + nextFile + " to " + to + myDirs[i] + nextFile);
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

class TestProjectUpdater : public ::testing::Test {
 protected:
  DataBaseTM *m_CopyDB = DatabaseEnvironment::m_db;

  virtual void SetUp() {
    // remove if exists
    wxFileName myDir(g_TestPathPRJ + _T("tmp_testprjupdate"), _T(""));
    if (wxDir::Exists(myDir.GetFullPath())) {
      wxLogMessage(_T("Removing tempory project: '%s'"), myDir.GetFullPath());
      myDir.Rmdir(wxPATH_RMDIR_RECURSIVE);
    }
    ASSERT_FALSE(wxDir::Exists(myDir.GetFullPath()));

    // copy project
    CopyDir(g_TestPathPRJ + g_TestPrj_PrjUpdate, g_TestPathPRJ + _T("tmp_testprjupdate"));
    ASSERT_TRUE(m_CopyDB->OpenTMDatabase(g_TestPathPRJ + _T("tmp_testprjupdate")));
  }
  virtual void TearDown() {;
  }
};

TEST_F(TestProjectUpdater, IsUpdateNeeded) {
  tmProjectUpdater myPrjUpd(m_CopyDB);
  ASSERT_FALSE(myPrjUpd.IsCorrectVersion());
}

TEST_F(TestProjectUpdater, DISABLED_UpdateOK) {
  tmProjectUpdater myPrjUpd(m_CopyDB);
  ASSERT_FALSE(myPrjUpd.IsCorrectVersion());
  ASSERT_EQ(myPrjUpd.DoUpdate(), tmPRJ_UPD_ERROR_OK);
  ASSERT_TRUE(myPrjUpd.IsCorrectVersion());
}
