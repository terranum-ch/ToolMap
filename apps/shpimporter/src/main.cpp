#include "wx/wxprec.h"
#ifdef __BORLANDC__
#pragma hdrstop
#endif
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include <wx/app.h>
#include <wx/cmdline.h>
#include <wx/dir.h>
#include <wx/filename.h>

#include "database.h"
#include "silayer.h"
#include "siprogress.h"

static const wxCmdLineEntryDesc cmdLineDesc[] = {
    {wxCMD_LINE_SWITCH, "h", "help", "show this help message", wxCMD_LINE_VAL_NONE, wxCMD_LINE_OPTION_HELP},
    {wxCMD_LINE_SWITCH, "v", "verbose", "Be more verbose"},
    {wxCMD_LINE_SWITCH, "o", "overwrite", "overwrite output"},
    {wxCMD_LINE_PARAM, NULL, NULL, "[ToolMap project file]", wxCMD_LINE_VAL_STRING, wxCMD_LINE_OPTION_MANDATORY},
    {wxCMD_LINE_PARAM, NULL, NULL, "[SHP directory]", wxCMD_LINE_VAL_STRING, wxCMD_LINE_OPTION_MANDATORY},
    {wxCMD_LINE_PARAM, NULL, NULL, "[rule files directory]", wxCMD_LINE_VAL_STRING, wxCMD_LINE_OPTION_MANDATORY},
    {wxCMD_LINE_NONE}};

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

void PrintArray(const wxArrayString &array, const wxString &msg) {
  wxPrintf(msg + _T("\n"));
  for (unsigned int i = 0; i < array.GetCount(); i++) {
    wxPrintf(array[i] + _T("\n"));
  }
}

int main(int argc, char **argv) {
  // debugging string for OSX
  // this is needed for viewing string content with Xcode !!
  wxString myTest = _T("Test debugging");
  myTest.ToUTF8().data();
  myTest.Len();

  wxApp::CheckBuildOptions(WX_BUILD_OPTIONS_SIGNATURE, "program");
  wxInitializer initializer;
  if (!initializer) {
    fprintf(stderr, "Failed to initialize the wxWidgets library, aborting.");
    return -1;
  }

  wxString myLogoTxt =
      _T("*\n* ShpImporter \n* Import SHP into ToolMap projects \n* (c) Copyright 2012 Lucien Schreiber - CREALP . ")
      _T("All Rights Reserved. \n*\n");
  wxCmdLineParser parser(cmdLineDesc, argc, argv);
  parser.SetLogo(myLogoTxt);
  if (parser.Parse() != 0) {
    return 0;
  }
  bool beVerbose = parser.Found("verbose");

  // cmd line is correct !!
  wxPrintf(myLogoTxt);

  // make ToolMap project backup and open it.
  DataBase myDB(_T("./"));
  wxFileName myToolMapProjectName(parser.GetParam(0));
  wxFileName myToolMapProjectNameBkp(parser.GetParam(0) + _T("_bkp"));
  wxFileName myDirToRemove(myToolMapProjectNameBkp.GetFullPath(), _T(""), _T(""));

  if (parser.Found("overwrite")) {
    if (myDirToRemove.Exists()) {
      myDirToRemove.Rmdir(wxPATH_RMDIR_RECURSIVE);
    }
  }
  if (!myDirToRemove.Exists()) {
    if (!CopyDir(myToolMapProjectName.GetFullPath(), myToolMapProjectNameBkp.GetFullPath())) {
      wxLogError(_("Unable to create project Backup!"));
    }
  }

  if (!myDB.DataBaseOpen(myToolMapProjectNameBkp.GetPath(), myToolMapProjectNameBkp.GetName())) {
    return 0;
  }
  if (beVerbose) {
    wxLogMessage(_("Loading data into: ") + myToolMapProjectNameBkp.GetName());
  }
  OGRRegisterAll();

  wxString mySHPDirectory = parser.GetParam(1);
  wxString myRuleDirectory = parser.GetParam(2);
  if (!wxDir::Exists(myRuleDirectory)) {
    wxLogError(_("Rule Directory: %s didn't exists"), myRuleDirectory);
    return 0;
  }
  if (beVerbose) {
    wxPrintf(_("Rule file directory is: %s\n"), myRuleDirectory);
  }

  wxArrayString myRuleFiles;
  wxDir::GetAllFiles(myRuleDirectory, &myRuleFiles, _T("*.txt"), wxDIR_FILES);
  if (beVerbose) {
    wxLogMessage(_("%ld rule files to process!\n"), myRuleFiles.GetCount());
  }

  // loading and processing
  long myTotalFeatureCount = 0;
  wxStopWatch sw;
  for (unsigned int i = 0; i < myRuleFiles.GetCount(); i++) {
    int myFeatureCount = 0;
    wxFileName myActualRuleFile(myRuleFiles.Item(i));
    wxPrintf(_("********** Processing: '%s' **********\n"), myActualRuleFile.GetName());
    siLayer myLayer(mySHPDirectory, &myDB);
    siProgressIndicator myProgress;
    myLayer.SetProgressIndicator(&myProgress);
    if (!myLayer.LoadFromFile(myRuleFiles.Item(i))) {
      wxLogError(_("Loading '%s' failed!"), myRuleFiles.Item(i));
      continue;
    }

    myFeatureCount = myLayer.Process();
    if (myFeatureCount == wxNOT_FOUND) {
      wxLogError(_("Processing '%s' failed!"), myRuleFiles.Item(i));
    } else {
      myTotalFeatureCount += myFeatureCount;
    }
    wxPrintf(_T("\n"));
  }

  wxTimeSpan mySpanTime = wxTimeSpan::Milliseconds(sw.Time());
  wxPrintf(_("%ld Features processed in: %s\n"), myTotalFeatureCount, mySpanTime.Format(_T("%H:%M:%S.%l")));
}
