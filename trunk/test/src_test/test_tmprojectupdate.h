/***************************************************************************
 test_export.h
 UNIT TESTING for exporting data
 -------------------
 copyright            : (C) 2009 CREALP Lucien Schreiber 
 email                : lucien.schreiber at crealp dot vs dot ch
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef _TM_TEST_PROJECT_UPDATE_H_
#define _TM_TEST_PROJECT_UPDATE_H_


#include "wx/wxprec.h"
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include <cxxtest/TestSuite.h>

#include "test_param.h"
#include "../../src/core/tmprojectupdater.h"

// =================================================================
bool CopyDir(wxString from, wxString to) {
	wxString SLASH = wxFILE_SEP_PATH;

	// append a slash if there is not one (for easier parsing)
	// because who knows what people will pass to the function.
	if (to[to.length()-1] != SLASH) {
		to += SLASH;   
	}
	// for both dirs
	if (from[from.length()-1] != SLASH) {
		from += SLASH;   
	}
	
	// first make sure that the source dir exists
	if(!wxDir::Exists(from)) {
		wxLogError(from + " does not exist.  Can not copy directory.");
	}
	else {
		// check on the destination dir
		// if it doesn't exist...
		if(!wxDir::Exists(to)) {
			// if it doesn't get created
			if(!wxFileName::Mkdir(to, 0777, wxPATH_MKDIR_FULL)) {
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
			if(!wxDir::Exists(to + myDirs[i])) {
				// if it doesn't, then create it
				if(!wxFileName::Mkdir(to + myDirs[i], 0777, wxPATH_MKDIR_FULL)) {
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
				if(wxDir::Exists(from+nextFile)) {
					// then append it for creation/copying
					myDirs.Add(nextFile + SLASH);   // only add the difference
				}
				else {
					
					// otherwise just go ahead and copy the file over
					if(!wxCopyFile(from + myDirs[i] + nextFile, 
								   to   + myDirs[i] + nextFile)) {
						// error if we couldn't
						wxLogError("Could not copy " +
								   from + myDirs[i] + nextFile + " to "
								   + to + myDirs[i] + nextFile);
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




class TEST_tmProjectUpdater : public CxxTest::TestSuite
{
public:
	//DataBaseTM * m_OriginalDB;
	DataBaseTM * m_CopyDB;

	TEST_tmProjectUpdater (bool bTest){
		// remove if exists
        wxFileName myDir (g_TestPathPRJ + _T("tmp_testprjupdate"), _T(""));
        if (wxDir::Exists(myDir.GetFullPath())==true) {
            wxLogMessage(_T("Removing tempory project: '%s'"), myDir.GetFullPath());
            myDir.Rmdir(wxPATH_RMDIR_RECURSIVE);
        }
        TS_ASSERT(wxDir::Exists(myDir.GetFullPath()) == false);
        
        // copy project
		CopyDir(g_TestPathPRJ + g_TestPrj_PrjUpdate, g_TestPathPRJ + _T("tmp_testprjupdate"));
		m_CopyDB = new DataBaseTM();
		TS_ASSERT(m_CopyDB->OpenTMDatabase(g_TestPathPRJ +  _T("tmp_testprjupdate")));
	}

	
	virtual ~TEST_tmProjectUpdater(){
		wxDELETE(m_CopyDB);
		//wxFileName myDir (g_TestPathPRJ + _T("tmp_testprjupdate"), _T(""));
		// TODO: uncomment bellow to remove directory
        //myDir.Rmdir(wxPATH_RMDIR_RECURSIVE);
	}
	
	static TEST_tmProjectUpdater *createSuite() { return new TEST_tmProjectUpdater(true);}
    static void destroySuite( TEST_tmProjectUpdater *suite ) { delete suite; }

	
	void setUp()
	{
	}
	
	void tearDown(){
	}
	

	void testName(){
		wxLogMessage(_T("------------------------------------"));
		wxLogMessage(_T("------- TESTING TMPROJECTUPDATER----"));
		wxLogMessage(_T("------------------------------------"));
	}
	
	void testIsUpdateNeeded1(){
		tmProjectUpdater myPrjUpd(m_CopyDB);
		TS_ASSERT(myPrjUpd.IsCorrectVersion() == false);
	}
	
	void testUpdateOK(){
		tmProjectUpdater myPrjUpd (m_CopyDB);
		TS_ASSERT(myPrjUpd.IsCorrectVersion() == false);
		TS_ASSERT_EQUALS(myPrjUpd.DoUpdate(), tmPRJ_UPD_ERROR_OK);
		TS_ASSERT(myPrjUpd.IsCorrectVersion() == true);
	}
};




#endif
