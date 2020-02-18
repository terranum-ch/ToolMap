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

#ifndef _TM_TEST_BACKUP_H_
#define _TM_TEST_BACKUP_H_

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <cxxtest/TestSuite.h>
#include <wx/datetime.h>

#include "../../src/core/backupmanager.h"
#include "../../src/database/database_tm.h"
#include "test_param.h"

class TEST_Backup : public CxxTest::TestSuite {
 public:
  DataBaseTM *m_Database;

  TEST_Backup(bool bTest) {
    wxApp::SetInstance(new wxAppConsole());
    m_Database = new DataBaseTM();
    TS_ASSERT(m_Database->OpenTMDatabase((g_TestPathPRJ + g_TestPrj_CombioulaCorrect)));
  }

  virtual ~TEST_Backup() {
    wxDELETE(m_Database);
  }

  static TEST_Backup *createSuite() {
    return new TEST_Backup(true);
  }
  static void destroySuite(TEST_Backup *suite) {
    delete suite;
  }

  void testName() {
    wxLogMessage(_T("------------------------------------"));
    wxLogMessage(_T("------- TESTING BACKUP DATA ------"));
    wxLogMessage(_T("------------------------------------"));
  }

  void testBackupFile() {
    BackupFile myFile;
    TS_ASSERT(!myFile.IsValid());
  }

  void testBackupFile2() {
    BackupFile myFile;
    myFile.SetOutputName(wxFileName(g_TestPathBackup, _T("testbackup.tmbk")));
    myFile.SetInputDirectory(wxFileName(g_TestPathPRJ, g_TestPrj_CombioulaCorrect));
    TS_ASSERT(!myFile.IsValid());
    myFile.SetDate(wxDateTime::Now());
    TS_ASSERT(myFile.IsValid());
  }

  void testBackup() {
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
    TS_ASSERT(myFile.IsValid());

    // testing backup here
    BackupManager myManager(m_Database);
    TS_ASSERT(myManager.Backup(myFile));
  }

  void testGetFileInfo() {
    BackupFile myInfoFile;

    BackupManager myManager(m_Database);
    TS_ASSERT(myManager.GetFileInfo(wxFileName(g_TestPathBackup, _T("testbackup-2000-01-01-120000.tmbk")), myInfoFile));

    TS_ASSERT(myInfoFile.GetComment() == _T("èà"));
    TS_ASSERT_EQUALS(myInfoFile.GetDate().GetDay(), 1);
    TS_ASSERT_EQUALS(myInfoFile.GetDate().GetMonth(), wxDateTime::Jan);
    TS_ASSERT_EQUALS(myInfoFile.GetDate().GetYear(), 2000);
    TS_ASSERT_EQUALS(myInfoFile.GetDate().GetHour(), 12);
  }

  void testRestore() {
    BackupFile myRestoreFile;

    BackupManager myManager(m_Database);

    myRestoreFile.SetOutputName(wxFileName(g_TestPathBackup, _T("testbackup-2000-01-01-120000.tmbk")));
    myRestoreFile.SetInputDirectory(wxFileName(g_TestPathBackup, _T("combioula_correct_restored")));
    myRestoreFile.SetDate(wxDateTime::Now());
    TS_ASSERT(myRestoreFile.IsValid());

    TS_ASSERT(myManager.Restore(myRestoreFile));
  }
};

#endif
