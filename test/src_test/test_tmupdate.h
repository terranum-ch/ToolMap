/***************************************************************************
							test_tmupdate.h
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

#ifndef _TM_UPDATE_TEST_H_
#define _TM_UPDATE_TEST_H_



#include "wx/wxprec.h"
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <cxxtest/TestSuite.h>

#include "test_param.h"
#include "../../src/components/tmupdate/tmupdate.h"

class TEST_tmUpdate : public CxxTest::TestSuite
{
public:
	tmUpdate * tm2;
	
	void setUp()
	{
		tm2 = new tmUpdate();
	}
	
	void tearDown() 
	{
		delete tm2;
	}

		
	void testName(){
		wxLogMessage(_T("------------------------------------"));
		wxLogMessage(_T("--------- TESTING TMUPDATE ---------"));
		wxLogMessage(_T("------------------------------------"));
	}
	
	void testConnection()
	{
		tmUpdate tm;
		if(g_UseProxy == true)
			tm.SetParameters(true, true, g_ProxyName);


		TS_ASSERT(tm.GetErrorMessage() == 0);	
		TS_ASSERT(tm.IsServerResponding()==true);
		wxLogDebug(_T("Connection status : %d"), tm.GetErrorMessage());
	}
	
	void testErrorProxy()
	{
		tmUpdate tm;
		// wrong proxy
		tm.SetParameters(true, true, _T("wrongproxy.vs.ch:8080"));
		TS_ASSERT_EQUALS( tm.GetErrorMessage(),0);
		TS_ASSERT(tm.IsServerResponding()==false);
		// Error connecting proxy : CURLE_COULDNT_RESOLVE_PROXY (5)
		// see error list here : http://curl.haxx.se/libcurl/c/libcurl-errors.html
		TS_ASSERT_EQUALS( tm.GetErrorMessage(), 5);

	}

	
	
	void testCheckNewVersion()
	{
		tmUpdate tm;
		tm.SetParameters();
		if(g_UseProxy == true)
			tm.SetParameters(true, true, g_ProxyName);

		wxLogDebug(_T("Checking new version"));
		wxString myVersionLabel;
		TS_ASSERT(tm.IsNewVersionAvaillable());
	}
	
	void testActualVersion()
	{
		tmUpdate tm;
		tm.SetActualVersion(222);
		if(g_UseProxy == true)
			tm.SetParameters(true, true, g_ProxyName);

		TS_ASSERT(tm.IsNewVersionAvaillable() == true);
		tm.SetActualVersion(29000);
		TS_ASSERT(tm.IsNewVersionAvaillable() == false);
	}
	
	void testGetNewVersionName()
	{
		tmUpdate tm;
		if(g_UseProxy == true)
			tm.SetParameters(true, true, g_ProxyName);

		TS_ASSERT(tm.GetNewVersionName() == wxEmptyString);
		TS_ASSERT(tm.IsNewVersionAvaillable()==true);
		TS_ASSERT(tm.GetNewVersionName() == wxString::Format(_T("2.0.%d"), g_ToolMapInternetVersion));
		tm.SetActualVersion(g_ToolMapInternetVersion);
		TS_ASSERT(tm.IsNewVersionAvaillable()==false);
		TS_ASSERT(tm.GetNewVersionName() == wxString::Format(_T("2.0.%d"), g_ToolMapInternetVersion));
		
	}
	
	void testGetDownloadLinkAdress ()
	{
		tmUpdate tm;
		if(g_UseProxy == true)
			tm.SetParameters(true, true, g_ProxyName);

		TS_ASSERT(tm.GetDownloadLink() == wxEmptyString);
		TS_ASSERT(tm.IsNewVersionAvaillable()==true);
		wxString myDownloadName = tm.GetDownloadLink();
		TS_ASSERT(myDownloadName == g_InternetVName);
	}
	
	void testSettingParameters ()
	{
		tmUpdate tm;
		tm.SetParameters(false, true, g_ProxyName);
		TS_ASSERT(tm.IsCheckOnStart()==false);
		tm.SetParameters();
		TS_ASSERT(tm.IsCheckOnStart()==true);
	}
	
	void testGettingParameters ()
	{
		bool bCheck;
		bool bProxy;
		wxString sProxy;
		
		// par default
		tm2->GetParameters(bCheck, bProxy, sProxy);
		wxLogDebug(_T("%d, %d, %s"), bCheck, bProxy, sProxy.c_str());
		TS_ASSERT(bCheck == true && bProxy == false && sProxy == wxEmptyString);
		
		tm2->SetParameters(false, true, g_ProxyName);
		
		tm2->GetParameters(bCheck, bProxy, sProxy);
		TS_ASSERT(bCheck == false && bProxy == true && sProxy == g_ProxyName);
		//wxSleep(2);
	}
	
	void testGettingParameters2 ()
	{
		// are parameters kept ???
		bool bCheck;
		bool bProxy;
		wxString sProxy;
		
		tm2->GetParameters(bCheck, bProxy, sProxy);
		wxLogDebug(_T("%d, %d, %s"), bCheck, bProxy, sProxy.c_str());
		TS_ASSERT(bCheck == false && bProxy == true && sProxy == g_ProxyName);
		tm2->SetParameters();
	}
	
	
	
		
};




#endif
