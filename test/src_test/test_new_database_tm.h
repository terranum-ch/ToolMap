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
		wxApp::SetInstance(new wxAppConsole());
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
		myPrjdef.m_PrjProjType = PROJ_SWISS_CH1903;
		myPrjdef.m_PrjUnitType = UNIT_METERS;
		
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
        myField1.m_pCodedValueArray.Add(new ProjectDefMemoryFieldsCodedVal("2a", "inactif"));        
        // adding attributs
        
        TS_ASSERT(m_DB->AddField(&myField1));
    }
    
    
    
    void testGetProjectInMemory(){
        TS_ASSERT(m_DB->OpenTMDatabase(g_TestPathPRJ + g_TestPrj_222));
        PrjDefMemManage * myPrjMem = m_DB->GetProjectDataFromDB();
        TS_ASSERT(myPrjMem != NULL);
        
        ProjectDefMemoryLayers * myLayer = myPrjMem->FindLayer(_T("Tectonic_Boundaries_L"));
        TS_ASSERT(myLayer != NULL);
        
        ProjectDefMemoryFieldsCodedVal * myVal = myLayer->m_pLayerFieldArray.Item(0)->m_pCodedValueArray.Item(1);
        TS_ASSERT(myVal != NULL)
        TS_ASSERT(myVal->m_ValueCode == _T("2a") );
        TS_ASSERT(myVal->m_ValueName == _T("inactif"));
        TS_ASSERT_EQUALS(myLayer->m_pLayerFieldArray.Item(0)->m_FieldType, TM_FIELD_ENUMERATION);

        wxDELETE(myPrjMem);
    }
	
    
    void testUpdateProject(){
        // load project
        TS_ASSERT(m_DB->OpenTMDatabase(g_TestPathPRJ + g_TestPrj_222));
        PrjDefMemManage * myPrjMem = m_DB->GetProjectDataFromDB();
        TS_ASSERT(myPrjMem != NULL);

        ProjectDefMemoryLayers * myLayer = myPrjMem->FindLayer(_T("Tectonic_Boundaries_L"));
        TS_ASSERT(myLayer != NULL);
        
        ProjectDefMemoryFields * myField = myPrjMem->FindField(_T("Activity"));
        TS_ASSERT(myField != NULL);
        
        ProjectDefMemoryFieldsCodedVal * myVal = myLayer->m_pLayerFieldArray.Item(0)->m_pCodedValueArray.Item(1);
        TS_ASSERT(myVal != NULL)
        TS_ASSERT(myVal->m_ValueCode == _T("2a") );
        TS_ASSERT(myVal->m_ValueName == _T("inactif"));
        
        // change values for item 1
        myVal->m_ValueCode = _T("3b");
        myVal->m_ValueName = _T("inactiff");
        
        myPrjMem->RemoveCodedValue(0);
        
        // Add new item
        ProjectDefMemoryFieldsCodedVal * myNewCodedValue = myPrjMem->AddCodedValue();
        TS_ASSERT(myNewCodedValue);
        myNewCodedValue->m_ValueCode = _T("12");
        myNewCodedValue->m_ValueName = _T("Inconnu");

        // update project
        TS_ASSERT(m_DB->UpdateDataBaseProject(myPrjMem));

        wxDELETE(myPrjMem);
    }
	
};




#endif
