/***************************************************************************
								tmupdate_test.cpp
		Test the update process by cheking internet for new versions
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

// comment doxygen


#ifndef _TM_UPDATE_TEST_CPP_
#define _TM_UPDATE_TEST_CPP_


#include "../../src/components/tmupdate/tmupdate.h"

#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>
#include <wx/filefn.h> 

class tmUpdateTEST : public CppUnit::TestFixture 
{ 
	CPPUNIT_TEST_SUITE( tmUpdateTEST );
	CPPUNIT_TEST ( TESTConnection );
	CPPUNIT_TEST ( TESTCheckNewVersion );
	CPPUNIT_TEST_SUITE_END();
	
private:
	
	
public: 
	void setUp()
	{
	}
	
	void tearDown() 
	{
	}
	
	void TESTConnection()
	{
				
		tmUpdate tm;
		CPPUNIT_ASSERT(tm.IsServerResponding(NULL));
	}
	
	
	
	void TESTCheckNewVersion()
	{
		wxLogDebug(_T("Checking new version"));
		tmUpdate tm;
		wxString myVersionLabel;
		CPPUNIT_ASSERT(tm.IsNewVersionAvaillable(myVersionLabel));
	}
	

	
	
};

#endif


