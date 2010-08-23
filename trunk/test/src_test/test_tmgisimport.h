/***************************************************************************
								test_tmgisimport.h
							UNIT TESTING for importing gis data
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

#ifndef _TM_GIS_IMPORT_TEST_CPP_
#define _TM_GIS_IMPORT_TEST_CPP_


#include "wx/wxprec.h"
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <cxxtest/TestSuite.h>

#include "test_param.h"
#include "../../src/database/database_tm.h"
#include "../../src/gis/tmimportgis.h"
#include "../../src/gis/tmimportcsv.h"
#include "../../src/gis/tmgisdatavector.h"


class TEST_tmGISImport : public CxxTest::TestSuite
{
public:
	DataBaseTM * m_DB;


	TEST_tmGISImport (bool bTest){
		tmGISData::InitGISDrivers(true,true);
		m_DB = new DataBaseTM();
		TS_ASSERT(m_DB->DataBaseOpen(g_TestPathPRJ, g_TestPrj_Fields));
	}
	
	virtual ~TEST_tmGISImport(){
		delete m_DB;
	}
	
	static TEST_tmGISImport *createSuite() { return new TEST_tmGISImport(true);}
    static void destroySuite( TEST_tmGISImport *suite ) { delete suite; }
	
		
	
	void testName(){
		wxLogMessage(_T("------------------------------------"));
		wxLogMessage(_T("--------   TESTING IMPORT    -------"));
		wxLogMessage(_T("------------------------------------"));
	}
	
	void testOpen()
	{
		tmImportGIS myImport;
		TS_ASSERT (myImport.Open(wxFileName(g_TestPathSHP, g_TestFileSHP_L)));
		TS_ASSERT (myImport.Open(wxFileName(g_TestPathSHP + _T("cantonsuissssse.shp")))==false);
		TS_ASSERT (myImport.Open(wxFileName(g_TestPathSHP, g_TestFileMisc))==false);
	}
		
	
	void testFileSpatialType ()
	{
		tmImportGIS myImport;
		TS_ASSERT (myImport.Open(wxFileName(g_TestPathSHP + g_TestFileSHP_PLG))==true);
		TS_ASSERT_EQUALS (myImport.GetGeometryType(), LAYER_SPATIAL_POLYGON);
		
	}
	
	void testFeatureCount ()
	{
		tmImportGIS myImport;
		TS_ASSERT (myImport.GetFeatureCount() == wxNOT_FOUND);
		TS_ASSERT (myImport.Open(wxFileName(g_TestPathSHP + g_TestFileSHP_PLG))==true);
		TS_ASSERT_EQUALS (myImport.GetFeatureCount(), 26);
	}

		
	void testImportInto()
	{
		tmImportGIS myImport;
		TS_ASSERT (myImport.Open(wxFileName(g_TestPathSHP + g_TestFileSHP_PLG))==true);
		TS_ASSERT (myImport.Import(m_DB, NULL)==false);
		myImport.SetTarget(TOC_NAME_LINES);
		TS_ASSERT (myImport.Import(m_DB, NULL)==true);
	}
	
	
	// importing CSV
	void testOpenCSV(){
		tmImportCSV myImport;
		TS_ASSERT(myImport.Open(wxFileName(g_TestPathMISC,  g_TestFileCSV)));
	}
	
	
	void testGettingColumns(){
		tmImportCSV myImport;
		TS_ASSERT(myImport.Open(wxFileName(g_TestPathMISC,  g_TestFileCSV)));
		wxArrayString myCols = myImport.ListColumns();
		wxASSERT(myCols.GetCount() > 0);
		for (unsigned int i = 0; i<myCols.GetCount(); i++) {
			wxLogMessage(_T("Column %d - '%s'"), i, myCols.Item(i).c_str());
		}
	}
	
	
	void testImportCSV(){
		tmImportCSV myImport;
		TS_ASSERT(myImport.Open(wxFileName(g_TestPathMISC,  g_TestFileCSV)));
		myImport.SetColumn(0, 1);
		myImport.SetTarget(TOC_NAME_LINES);
		TS_ASSERT(myImport.Import(m_DB, NULL));

	}
	
		
};




#endif
