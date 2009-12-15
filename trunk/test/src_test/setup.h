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


class Fixture1 : public CxxTest::GlobalFixture
{

public:
	bool setUpWorld(){
		wxApp::CheckBuildOptions(WX_BUILD_OPTIONS_SIGNATURE, "program");
		wxInitializer initializer;
		if ( !initializer )
		{
			fprintf(stderr, "Failed to initialize the wxWidgets library, aborting.");
			TS_FAIL(_T("Unable to init the wxWigets library"));
		}

#ifdef __LINUX__
		TS_ASSERT(gtk_init_check(NULL,NULL));
#endif

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
