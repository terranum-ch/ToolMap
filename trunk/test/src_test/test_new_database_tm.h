/***************************************************************************
 test_new_database_tm.h
 Testing Database TM class
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

#ifndef _TEST__NEW_DATABASE_TM_H_
#define _TEST__NEW_DATABASE_TM_H_


#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <cxxtest/TestSuite.h>
#include "test_param.h"
#include "../../src/database/database_tm.h"


class TEST_New_DatabaseTM : public CxxTest::TestSuite
{
public:
	DataBaseTM * m_DB;	
	void setUp(){
		m_DB = new DataBaseTM();
	}
	
	void tearDown() {
		wxDELETE(m_DB);
	}
	
	void testName(){
		wxLogMessage(_T("------------------------------------"));
		wxLogMessage(_T("----- TESTING NEW DATABASE_TM ------"));
		wxLogMessage(_T("------------------------------------"));
	}
		
	void testCreateNewTMDatabase(){
		// remove if exists
        wxString myPrjName = g_TestPathPRJ + g_TestPrj_222;
        if (wxDirExists(myPrjName) == true) {
            wxLogMessage("Removing : " + myPrjName);
            wxFileName::Rmdir(myPrjName, wxPATH_RMDIR_RECURSIVE);
        }
        TS_ASSERT(wxDirExists(myPrjName) == false);
		
		PrjDefMemManage myPrjdef;
		myPrjdef.m_PrjName = g_TestPrj_222;
		myPrjdef.m_PrjPath = g_TestPathPRJ;
		myPrjdef.m_PrjProjType = PROJ_SWISSPROJ;
		myPrjdef.m_PrjUnitType = UNIT_LATLONG;
		
		TS_ASSERT_EQUALS(m_DB->CreateTMDatabase(&myPrjdef),true);
	}
    
  
    void testAddingLayerData(){
        TS_ASSERT(m_DB->OpenTMDatabase(g_TestPathPRJ + g_TestPrj_222));
        TS_ASSERT(m_DB->DataBaseGetName() == g_TestPrj_222);
        
        ProjectDefMemoryLayers myLayer;
        myLayer.m_LayerName = _T("Tectonic_Boundaries_L");
        myLayer.m_LayerType = LAYER_LINE;
        
        // adding layer
        TS_ASSERT(m_DB->AddLayer(&myLayer));
        
        
        // field1 
        ProjectDefMemoryFields myField1;
        myField1.m_Fieldname = _T("Activity");
        myField1.m_FieldType = TM_FIELD_ENUMERATION;
        
        
        
        myField1.m_pCodedValueArray.Add(new ProjectDefMemoryFieldsCodedVal("1", "actif"));
        myField1.m_pCodedValueArray.Add(new ProjectDefMemoryFieldsCodedVal("2", "inactif"));        
        // adding attributs
        
    }
	
	
};




#endif
