/***************************************************************************
 test_export.h
 UNIT TESTING for exporting data
 -------------------
 copyright            : (C) 2009 CREALP Lucien Schreiber 
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef _TM_TEST_EXPORT_DATA_H_
#define _TM_TEST_EXPORT_DATA_H_


#include "wx/wxprec.h"
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <cxxtest/TestSuite.h>

#include "test_param.h"
#include "../../src/core/tmexportmanager.h"

class TEST_tmExportData : public CxxTest::TestSuite
{
public:
	DataBaseTM * m_pDB;
	PrjDefMemManage * m_PrjDef;
	wxString * m_RealExportPath;
    tmGISScale * m_ExportScale;



	TEST_tmExportData (bool bTest){
		wxApp::SetInstance(new wxAppConsole());
		m_pDB = new DataBaseTM();
		TS_ASSERT(m_pDB->OpenTMDatabase(g_TestPathPRJ + g_TestPrj_Ambri));
		// load project Data
		m_PrjDef = m_pDB->GetProjectDataFromDB() ;
		TS_ASSERT(m_PrjDef != NULL);
		TS_ASSERT_DIFFERS(m_PrjDef->GetCountLayers(), 0);
		int iExportType = 0;
		wxString myTempString = wxEmptyString;
		TS_ASSERT_DIFFERS(m_pDB->GetProjectExportData(iExportType, myTempString), PATH_DATABASE_ERROR);
		m_RealExportPath = new wxString(myTempString);
		TS_ASSERT(m_pDB->SetProjectExportData(EXPORT_SHAPEFILE, g_TestPathEXPORT));

        // init scale for export. The project we export are in meters
        // thus we can create this "fake" scale.
        m_ExportScale = new tmGISScale();
		m_ExportScale->SetExtentWndReal(tmRealRect(0,0,1,1));
        m_ExportScale->SetWidthDistanceInM(1);
	}
	
	virtual ~TEST_tmExportData(){
		// reset path to old value
		TS_ASSERT(m_pDB->SetProjectExportData(EXPORT_SHAPEFILE, *m_RealExportPath));
		
		delete m_pDB;
		if (m_PrjDef != NULL) {
			delete m_PrjDef;
		}
		delete m_RealExportPath;
		delete m_ExportScale;
	
	}
	
	static TEST_tmExportData *createSuite() { return new TEST_tmExportData(true);}
    static void destroySuite( TEST_tmExportData *suite ) { delete suite; }

	
	void setUp()
	{
	}
	
	void tearDown(){
	}
	

	void testName(){
		wxLogMessage(_T("------------------------------------"));
		wxLogMessage(_T("------- TESTING TMEXPORT DATA ------"));
		wxLogMessage(_T("------------------------------------"));
	}
	
	void testCreateExportManager()
	{
		tmExportManager myManager(NULL, m_pDB, m_ExportScale);
	}
	
	void testExportNotEmptyLayers()
	{
		wxLogMessage(_T("Removing layers"));
		int iLayerCount = m_PrjDef->GetCountLayers();
		PrjDefMemManage myPrj;
		myPrj = *m_PrjDef;
		TS_ASSERT_EQUALS(myPrj.GetCountLayers(), iLayerCount);
		TS_ASSERT(myPrj.RemoveLayer(_T("WatOutflow_Empty_PT")));
		TS_ASSERT(myPrj.RemoveLayer(_T("GravitFeat_Empty_L")));
		TS_ASSERT(myPrj.RemoveLayer(_T("Exploitation_Empty_PLG")));
		TS_ASSERT_EQUALS(myPrj.GetCountLayers(), 3);
		
		wxLogMessage(_T("Initing all OGR drivers"));
		OGRRegisterAll();
		tmExportManager myManager( NULL, m_pDB, m_ExportScale);
	}
	
	
	
	void testExportEmptyLayers(){
		
		wxLogMessage(_T("Removing not empty layers"));
		int iLayerCount = m_PrjDef->GetCountLayers();
		PrjDefMemManage myPrj;
		myPrj = *m_PrjDef;
		TS_ASSERT_EQUALS(myPrj.GetCountLayers(), iLayerCount);
		TS_ASSERT(myPrj.RemoveLayer(_T("KarsticStruct_PT")));
		TS_ASSERT(myPrj.RemoveLayer(_T("TectoBound_L")));
		TS_ASSERT(myPrj.RemoveLayer(_T("Rocks_PLG")));
		TS_ASSERT_EQUALS(myPrj.GetCountLayers(), 3);
		
		
		tmExportManager myManager( NULL, m_pDB, m_ExportScale);
	}
    	
		
};




#endif
