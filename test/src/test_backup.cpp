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
#include "../../src/core/backupmanager.h"
#include "../../src/database/database_tm.h"


class TestBackup : public ::testing::Test {
 protected:
  DataBaseTM *m_db = nullptr;

  virtual void SetUp() {
    m_db = new DataBaseTM();
    ASSERT_TRUE(m_db->OpenTMDatabase(g_TestPathPRJ + g_TestPrj_CombioulaCorrect));
  }
  virtual void TearDown() {
    wxDELETE(m_db);
  }
};

TEST_F(TestBackup, BackupFile) {
  BackupFile myFile;
  EXPECT_FALSE(myFile.IsValid());
}

TEST_F(TestBackup, BackupFile2) {
  BackupFile myFile;
  myFile.SetOutputName(wxFileName(g_TestPathBackup, _T("testbackup.tmbk")));
  myFile.SetInputDirectory(wxFileName(g_TestPathPRJ, g_TestPrj_CombioulaCorrect));
  EXPECT_FALSE(myFile.IsValid());
  myFile.SetDate(wxDateTime::Now());
  EXPECT_TRUE(myFile.IsValid());
}

TEST_F(TestBackup, BackupShouldWork) {
  // remove backup if exists
  if (wxRemoveFile(g_TestPathBackup + _T("testbackup-2000-01-01-120000.tmbk"))) {
    wxLogMessage("removed old backup file!");
  }

  // create file object
  BackupFile myFile;
  myFile.SetOutputName(wxFileName(g_TestPathBackup, _T("testbackup.tmbk")));
  myFile.SetInputDirectory(wxFileName(g_TestPathPRJ, g_TestPrj_CombioulaCorrect));
  myFile.SetDate(wxDateTime(1, wxDateTime::Jan, 2000, 12, 0, 0));

  // adding comment and author, not mandatory
  myFile.SetComment(_T("èà"));
  myFile.SetAuthor(_T("Lucien"));
  EXPECT_TRUE(myFile.IsValid());

  // testing backup here
  BackupManager myManager(m_db);
  EXPECT_TRUE(myManager.Backup(myFile));
}

TEST_F(TestBackup, GetFileInfo) {
  BackupFile myInfoFile;

  BackupManager myManager(m_db);
  EXPECT_TRUE(myManager.GetFileInfo(wxFileName(g_TestPathBackup, _T("testbackup-2000-01-01-120000.tmbk")), myInfoFile));

  EXPECT_EQ(myInfoFile.GetComment(), _T("èà"));
  EXPECT_EQ(myInfoFile.GetDate().GetDay(), 1);
  EXPECT_EQ(myInfoFile.GetDate().GetMonth(), wxDateTime::Jan);
  EXPECT_EQ(myInfoFile.GetDate().GetYear(), 2000);
  EXPECT_EQ(myInfoFile.GetDate().GetHour(), 12);
}

TEST_F(TestBackup, Restore) {
  BackupFile myRestoreFile;

  BackupManager myManager(m_db);

  myRestoreFile.SetOutputName(wxFileName(g_TestPathBackup, _T("testbackup-2000-01-01-120000.tmbk")));
  myRestoreFile.SetInputDirectory(wxFileName(g_TestPathBackup, _T("combioula_correct_restored")));
  myRestoreFile.SetDate(wxDateTime::Now());
  EXPECT_TRUE(myRestoreFile.IsValid());

  EXPECT_TRUE(myManager.Restore(myRestoreFile));
}