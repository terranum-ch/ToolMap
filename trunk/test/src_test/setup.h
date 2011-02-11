/***************************************************************************
								test_setup.h
							Setup tests 
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

#ifndef _TEST_SETUP_H_
#define _TEST_SETUP_H_


#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif



#include <cxxtest/TestSuite.h>
#include <cxxtest/GlobalFixture.h>

#ifdef __LINUX__
#include "gtk/gtk.h"	
#endif

static void myAssertHandler(const wxString &file, int line, const wxString &func, const wxString &cond, const wxString &msg) {
	wxTrap();
}

class Fixture1 : public CxxTest::GlobalFixture
{

public:
	bool setUpWorld(){
		wxApp::SetInstance(new wxAppConsole());
		int argc=0;
		char **argv=NULL;
		wxEntryStart(argc, argv);
		//wxInitialize();
		wxTheColourDatabase = new wxColourDatabase();
		wxInitAllImageHandlers();
        setlocale(LC_NUMERIC, "C");
		wxLogMessage(_T("Initing test application"));
        wxSetAssertHandler(myAssertHandler);

#ifdef __LINUX__
		TS_ASSERT(gtk_init_check(NULL,NULL));
#endif

		return true;
	}	
    
    bool tearDownWorld() {
		wxLogMessage(_T("Cleaning test application"));
		//wxUninitialize();
		wxEntryCleanup(); 
		return true; 
	}

};


static Fixture1 fixture1;

 
class Suite : public CxxTest::TestSuite
{
public:
	void testOne() {
		//setting output to the std err (otherwise log into windows)
		wxLog::SetActiveTarget(new wxLogStderr());
	}
	
	void testName(){
		wxLogMessage(_T("------------------------------------"));
		wxLogMessage(_T("-------- SETUP TESTS PASSED --------"));
		wxLogMessage(_T("------------------------------------"));
	}
};



#endif
