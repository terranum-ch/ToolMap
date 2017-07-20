/***************************************************************************
 test_tmprojectupdate2.cpp
 UNIT TESTING for project update 
 -------------------
 copyright            : (C) 2012 CREALP Lucien Schreiber 
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

#ifndef _TM_TEST_PROJECT_UPDATE2_H_
#define _TM_TEST_PROJECT_UPDATE2_H_


#include "wx/wxprec.h"
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include <cxxtest/TestSuite.h>

#include "test_param.h"
#include "../../src/core/tmprojectupdater.h"


// =================================================================
bool CopyDir2(wxString from, wxString to) {
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




class TEST_tmProjectUpdater2 : public CxxTest::TestSuite
{
public:
	//DataBaseTM * m_OriginalDB;
	DataBaseTM * m_CopyDB;

	TEST_tmProjectUpdater2 (bool bTest){
		wxApp::SetInstance(new wxAppConsole());
		// remove if exists
        wxFileName myDir2 (g_TestPathPRJ + g_TestMigre222 + _T("_tmp"), _T(""));
        if (wxDir::Exists(myDir2.GetFullPath())==true) {
            wxLogMessage(_T("Removing temporary project '%s'"), myDir2.GetFullPath());
            myDir2.Rmdir(wxPATH_RMDIR_RECURSIVE);
        }
        TS_ASSERT(wxDir::Exists(myDir2.GetFullPath()) == false);
        
        // copy project
        CopyDir2(g_TestPathPRJ + g_TestMigre222, g_TestPathPRJ + g_TestMigre222 + _T("_tmp"));
		m_CopyDB = new DataBaseTM();
		TS_ASSERT(m_CopyDB->OpenTMDatabase(g_TestPathPRJ + g_TestMigre222 + _T("_tmp")));
	}

	
	virtual ~TEST_tmProjectUpdater2(){
		wxDELETE(m_CopyDB);
	}
	
	static TEST_tmProjectUpdater2 *createSuite() { return new TEST_tmProjectUpdater2(true);}
    static void destroySuite( TEST_tmProjectUpdater2 *suite ) { delete suite; }

	
	void setUp()
	{
	}
	
	void tearDown(){
	}
	

	void testName(){
		wxLogMessage(_T("------------------------------------"));
		wxLogMessage(_T("------- TESTING TMPROJECTUPDATER2----"));
		wxLogMessage(_T("------------------------------------"));
	}
    
    
    ProjectDefMemoryFieldsCodedVal * ReadProjectData(PrjDefMemManage * prjdef, const wxString & valuetoread){
        TS_ASSERT(prjdef);
        ProjectDefMemoryLayers * myLayer = prjdef->GetNextLayer();
        TS_ASSERT (myLayer);
        TS_ASSERT_EQUALS(myLayer->m_LayerName, _T("Boreholes_PT"));
        
        ProjectDefMemoryFields * myField = prjdef->GetNextField();
        TS_ASSERT(myField);
        TS_ASSERT_EQUALS(myField->m_Fieldname, _T("D_C_UNDERG"));
        
        ProjectDefMemoryFieldsCodedVal * myVal = myField->m_pCodedValueArray.Item(1);
        TS_ASSERT(myVal);
        TS_ASSERT_EQUALS(myVal->m_ValueCode, valuetoread);
        TS_ASSERT_EQUALS(myVal->m_ValueName, _T("non"));
        return myVal;
    }
    
    void testUpdateMigreProject(){
        PrjDefMemManage * myPrjDef = m_CopyDB->GetProjectDataFromDB();
        
        // read project
        ProjectDefMemoryFieldsCodedVal * myVal = ReadProjectData(myPrjDef, _T("2"));
        TS_ASSERT(myVal);
        
        // update val
        myVal->m_ValueCode = _T("45a");
        
        // update project
        m_CopyDB->UpdateDataBaseProject(myPrjDef);
        wxDELETE(myPrjDef);
        TS_ASSERT(myPrjDef==NULL);
        
        // reread project
        wxLogMessage(_T("Re-reading project"));
        myPrjDef = m_CopyDB->GetProjectDataFromDB();
        TS_ASSERT(myPrjDef);
        
        ReadProjectData(myPrjDef, _T("45a"));

		wxDELETE(myPrjDef);
   }
    
    
    
    
};




#endif
