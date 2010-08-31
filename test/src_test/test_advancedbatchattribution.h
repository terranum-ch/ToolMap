/***************************************************************************
								TEST_AdvAttribManager.h
							Testing Database class
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

#ifndef _TEST_ADVANCED_ATTRIBUTION_BATCH_H_
#define _TEST_ADVANCED_ATTRIBUTION_BATCH_H_


#include "wx/wxprec.h"
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <cxxtest/TestSuite.h>

#include "test_param.h"

#include "../../src/core/tmaattribbatchmanager.h"
#include "../../src/database/database_tm.h"



class TEST_AdvAttribManager : public CxxTest::TestSuite
{
public:
	DataBaseTM * m_pDB;
	PrjDefMemManage * m_Prj;
	
	TEST_AdvAttribManager (bool bTest){
		m_pDB = new DataBaseTM();
		TS_ASSERT(m_pDB->OpenTMDatabase(g_TestPathPRJ + g_TestPrj_AdvAttribution));
		m_Prj = m_pDB->GetProjectDataFromDB();
		TS_ASSERT(m_Prj);
	}
	
	virtual ~TEST_AdvAttribManager(){
		delete m_pDB;
		delete m_Prj;
	}
	
	static TEST_AdvAttribManager *createSuite() { return new TEST_AdvAttribManager(true);}
    static void destroySuite( TEST_AdvAttribManager *suite ) { delete suite; }
	
	
	
	void testName(){
		wxLogMessage(_T("------------------------------------------"));
		wxLogMessage(_T("--- TESTING ADVANCED BATCH ATTRIBUTION ---"));
		wxLogMessage(_T("------------------------------------------"));
	}
	
	
	void testCreatorDtor(){
		tmSelectedDataMemory mySelData;
		mySelData.SetLayerID(0);
		wxArrayLong * mySelected = new wxArrayLong;
		mySelected->Add(1);
		mySelected->Add(2);
		mySelected->Add(3);
		mySelected->Add(10);
		mySelData.AddSelected(mySelected);
		
		tmAAttribBatchManager myManager(m_Prj, m_pDB, &mySelData, LAYER_LINE);
		TS_ASSERT(myManager.IsOk()==true);
	}
	
	
	void testGettingTypes(){
		tmSelectedDataMemory mySelData;
		mySelData.SetLayerID(0);
		wxArrayLong * mySelected = new wxArrayLong;
		mySelected->Add(1);
		mySelected->Add(2);
		mySelected->Add(3);
		mySelected->Add(10);
		mySelData.AddSelected(mySelected);
		
		tmAAttribBatchManager myManager(m_Prj, m_pDB, &mySelData, LAYER_LINE);	
		PrjMemObjectsArray myObjects;
		wxArrayInt myNumber;
		wxArrayLong myLayerId;
		TS_ASSERT(myManager.GetTypes(myObjects, myNumber, myLayerId));
		TS_ASSERT(myObjects.GetCount() == myNumber.GetCount());
		TS_ASSERT(myNumber.GetCount() == myLayerId.GetCount());
		for (unsigned int i = 0; i< myObjects.GetCount(); i++) {
			wxLogMessage(_T("%d - %s (%d) - %d"),
						 myObjects.Item(i)->m_ObjectID,
						 myObjects.Item(i)->m_ObjectName.c_str(),
						 myNumber.Item(i),
						 myLayerId.Item(i));
		}
		
	}
	
	
	void testGettingFields(){
		tmSelectedDataMemory mySelData;
		mySelData.SetLayerID(0);
		wxArrayLong * mySelected = new wxArrayLong;
		mySelected->Add(1);
		mySelected->Add(2);
		mySelected->Add(3);
		mySelected->Add(10);
		mySelData.AddSelected(mySelected);
		
		tmAAttribBatchManager myManager(m_Prj, m_pDB, &mySelData, LAYER_LINE);	
		
		// Field 1 is TectoBound_L
		PrjMemFieldArray myFields;
		TS_ASSERT(myManager.GetFields(1, myFields));
		
		for (unsigned int i = 0; i< myFields.GetCount(); i++) {
			wxLogMessage(_T("%d - %s"),
						 myFields.Item(i)->m_FieldID,
						 myFields.Item(i)->m_Fieldname.c_str());
		}
		
		
		// layer 2 is Border of Rocks (no field should be returned
		// because of different spatial type
		TS_ASSERT(myManager.GetFields(2, myFields)==false);
		TS_ASSERT(myFields.GetCount() == 0);
		
		
		
	}
	
	
	void testAttribute(){
		wxLogMessage(_T("Testing attribute from Advanced batch attribution"));
		
		tmSelectedDataMemory mySelData;
		mySelData.SetLayerID(0);	// line ??
		wxArrayLong * mySelected = new wxArrayLong;
		mySelected->Add(8); // faille
		mySelected->Add(9); // faille, border of hazards_plg, campaign_10
		mySelected->Add(10); // border of hazards_plg
		mySelData.AddSelected(mySelected);
		
		tmAAttribBatchManager myManager(m_Prj, m_pDB, &mySelData, LAYER_LINE);
		
		PrjMemFieldArray myFields;
		TS_ASSERT(myManager.GetFields(1, myFields));
		ProjectDefMemoryFields myTestField = *(myFields.Item(0));
		TS_ASSERT(myTestField.m_Fieldname == _T("TYPE"));
		
		// try attribution
		int attributedObjects = myManager.Attribute(1, myTestField, _T("Secondary"));
		TS_ASSERT_EQUALS(attributedObjects, 2);
	}
		
};




#endif
