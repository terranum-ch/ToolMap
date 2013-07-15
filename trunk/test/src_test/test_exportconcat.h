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

#ifndef _TM_TEST_EXPORT_DATA_CONCAT_H_
#define _TM_TEST_EXPORT_DATA_CONCAT_H_


#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <cxxtest/TestSuite.h>

#include "test_param.h"
#include "../../src/core/tmexportmanager.h"

class TEST_tmExportConcat : public CxxTest::TestSuite
{
public:
	DataBaseTM * m_pDB;
	PrjDefMemManage * m_PrjDef;
	wxString * m_RealExportPath;
    
	TEST_tmExportConcat (bool bTest){
        OGRRegisterAll();
		m_pDB = new DataBaseTM();
		TS_ASSERT(m_pDB->OpenTMDatabase(g_TestPathPRJ + g_TestExportConcat));
		// load project Data
		m_PrjDef = m_pDB->GetProjectDataFromDB() ;
		TS_ASSERT(m_PrjDef != NULL);
		TS_ASSERT_DIFFERS(m_PrjDef->GetCountLayers(), 0);
		int iExportType = 0;
		wxString myTempString = wxEmptyString;
		TS_ASSERT_DIFFERS(m_pDB->GetProjectExportData(iExportType, myTempString), PATH_DATABASE_ERROR);
		m_RealExportPath = new wxString(myTempString);
		
		TS_ASSERT(m_pDB->SetProjectExportData(EXPORT_SHAPEFILE,  g_TestPathEXPORT));
		
	}
	
	virtual ~TEST_tmExportConcat(){
		// reset path to old value
		TS_ASSERT(m_pDB->SetProjectExportData(EXPORT_SHAPEFILE, *m_RealExportPath));
		
		wxDELETE( m_pDB) ;
		wxDELETE( m_PrjDef );
        wxDELETE( m_RealExportPath );
	}
	
	static TEST_tmExportConcat *createSuite() { return new TEST_tmExportConcat(true);}
    static void destroySuite( TEST_tmExportConcat *suite ) { delete suite; }
    
	
	void setUp()
	{
	}
	
	void tearDown(){
	}
	
    
	void testName(){
		wxLogMessage(_T("------------------------------------"));
		wxLogMessage(_T("-- TESTING EXPORT CONCAT DATA ------"));
		wxLogMessage(_T("------------------------------------"));
	}
	
	void testExportLine () {
        tmExportManager myManager (NULL, m_pDB);
        TS_ASSERT(myManager.ExportLineConcatenated(m_PrjDef));
    }
    
};




#endif
