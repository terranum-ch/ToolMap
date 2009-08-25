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
	CPPUNIT_TEST (  TESTActualVersion );
	CPPUNIT_TEST ( TESTGetNewVersionName );
	CPPUNIT_TEST ( TESTGetDownloadLinkAdress );
	CPPUNIT_TEST ( TESTSettingParameters );
	CPPUNIT_TEST ( TESTGettingParameters );
	CPPUNIT_TEST ( TESTGettingParameters2 );
	CPPUNIT_TEST_SUITE_END();
	
private:
	tmUpdate tm;
	tmUpdate * tm2;
	
public: 
	void setUp()
	{
		tm2 = new tmUpdate();
	}
	
	void tearDown() 
	{
		delete tm2;
	}
	
	void TESTConnection()
	{
			
		
		CPPUNIT_ASSERT(tm.GetErrorMessage() == 0);	
		CPPUNIT_ASSERT(tm.IsServerResponding()==true);
		wxLogDebug(_T("Connection status : %d"), tm.GetErrorMessage());
	}
	
	
	
	void TESTCheckNewVersion()
	{
		wxLogDebug(_T("Checking new version"));
		wxString myVersionLabel;
		CPPUNIT_ASSERT(tm.IsNewVersionAvaillable());
	}
	
	void TESTActualVersion()
	{
		tm.SetActualVersion(222);
		CPPUNIT_ASSERT(tm.IsNewVersionAvaillable() == true);
		tm.SetActualVersion(29000);
		CPPUNIT_ASSERT(tm.IsNewVersionAvaillable() == false);
	}
	
	void TESTGetNewVersionName()
	{
		CPPUNIT_ASSERT(tm.GetNewVersionName() == wxEmptyString);
		CPPUNIT_ASSERT(tm.IsNewVersionAvaillable()==true);
		CPPUNIT_ASSERT(tm.GetNewVersionName() == _T("2.0.279"));
		tm.SetActualVersion(290);
		CPPUNIT_ASSERT(tm.IsNewVersionAvaillable()==false);
		CPPUNIT_ASSERT(tm.GetNewVersionName() == _T("2.0.279"));
	
	}
	
	void TESTGetDownloadLinkAdress ()
	{
		CPPUNIT_ASSERT(tm.GetDownloadLink() == wxEmptyString);
		CPPUNIT_ASSERT(tm.IsNewVersionAvaillable()==true);
		CPPUNIT_ASSERT(tm.GetDownloadLink() == wxString::Format(_T("http://www.crealp.ch/down/toolmap/ToolMap2_d279.dmg")));
	}

	void TESTSettingParameters ()
	{
		tm.SetParameters(false, true, _T("vs.proxy.ch:8080"));
		CPPUNIT_ASSERT(tm.IsCheckOnStart()==false);
		tm.SetParameters();
		CPPUNIT_ASSERT(tm.IsCheckOnStart()==true);
	}
	
	void TESTGettingParameters ()
	{
		bool bCheck;
		bool bProxy;
		wxString sProxy;
		
		// par default
		tm2->GetParameters(bCheck, bProxy, sProxy);
		wxLogDebug(_T("%d, %d, %s"), bCheck, bProxy, sProxy.c_str());
		CPPUNIT_ASSERT(bCheck == true && bProxy == false && sProxy == wxEmptyString);
		
		tm2->SetParameters(false, true, _T("vs.proxy.ch:8080"));
		
		tm2->GetParameters(bCheck, bProxy, sProxy);
		CPPUNIT_ASSERT(bCheck == false && bProxy == true && sProxy == _T("vs.proxy.ch:8080"));
		//wxSleep(2);
	}
	
	void TESTGettingParameters2 ()
	{
		// are parameters kept ???
		bool bCheck;
		bool bProxy;
		wxString sProxy;
		
		tm2->GetParameters(bCheck, bProxy, sProxy);
		wxLogDebug(_T("%d, %d, %s"), bCheck, bProxy, sProxy.c_str());
		CPPUNIT_ASSERT(bCheck == false && bProxy == true && sProxy == _T("vs.proxy.ch:8080"));
		
		tm2->SetParameters();
	}
	
	
};

#endif


