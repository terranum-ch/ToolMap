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
#include "../../src/database/database.h"
#include "../../src/gis/tmgisimport.h"

class TEST_tmGISImport : public CxxTest::TestSuite
{
public:
	DataBaseTM * m_DB;
	tmGISImport * m_Import;


	void setUp()
	{
		tmGISData::InitGISDrivers(true,true);
		m_DB = new DataBaseTM();
		TS_ASSERT(m_DB->DataBaseOpen(g_TestPathPRJ,g_TestPrj_Import));
		m_Import = new tmGISImport;
		
	}
	
	void tearDown() 
	{
		delete m_DB;
		delete m_Import;
	}
	
	
	void testName(){
		wxLogMessage(_T("------------------------------------"));
		wxLogMessage(_T("-------- TESTING TMGISIMPORT -------"));
		wxLogMessage(_T("------------------------------------"));
	}
	
	void testOpenDatabaseImport()
	{
		TS_ASSERT(m_DB->DataBaseOpen(g_TestPathPRJ, g_TestPrj_Fields));
	}
	
	void testOpen()
	{
		TS_ASSERT (m_Import->IsOpen() == false);
		TS_ASSERT (m_Import->Open(g_TestPathSHP + g_TestFileSHP_L));
		TS_ASSERT (m_Import->IsOpen() == true);
		TS_ASSERT (m_Import->Open(g_TestPathSHP + _T("cantonsuissssse.shp"))==false);
		TS_ASSERT (m_Import->Open(g_TestPathSHP + g_TestFileMisc)==false);
		TS_ASSERT (m_Import->IsOpen() == false);
		
		
	}
	
	
	
	void testFileSpatialType ()
	{
		
		TS_ASSERT (m_Import->Open(g_TestPathSHP + g_TestFileSHP_PLG)==true);
		TS_ASSERT_EQUALS (m_Import->GetSpatialType (), LAYER_SPATIAL_POLYGON);
		
	}
	
	void testFeatureCount ()
	{
		TS_ASSERT (m_Import->GetFeatureCount() == -1);
		TS_ASSERT (m_Import->Open(g_TestPathSHP + g_TestFileSHP_PLG)==true);
		TS_ASSERT (m_Import->GetFeatureCount() == 26);
	}
	
	void testIsAllowed ()
	{
		TS_ASSERT (m_Import->IsImportAllowed()==false);
		TS_ASSERT (m_Import->Open(g_TestPathSHP + g_TestFileSHP_PLG)==true);
		TS_ASSERT (m_Import->Open(g_TestPathSHP + g_TestFileSHP_L)==true);
		TS_ASSERT (m_Import->IsImportAllowed()==true);
	}
	
	void testImportOneLine()
	{
		TS_ASSERT (m_Import->Open(g_TestPathSHP + g_TestFileSHP_LCadre)==true);
		TS_ASSERT (m_Import->IsImportAllowed()==true);
		TS_ASSERT(m_Import->IsImportIntoAllowed (TOC_NAME_LINES)==true);
	}
	
	void testImportInto()
	{
		TS_ASSERT(m_Import->IsImportIntoAllowed (TOC_NAME_LINES)==false);
		TS_ASSERT (m_Import->Open(g_TestPathSHP + g_TestFileSHP_L)==true);
		TS_ASSERT(m_Import->IsImportIntoAllowed (TOC_NAME_LINES)==true);
		TS_ASSERT(m_Import->IsImportIntoAllowed (TOC_NAME_FRAME)==true);
		TS_ASSERT(m_Import->IsImportIntoAllowed (TOC_NAME_LABELS)==false);
	}
	
	void testImport ()
	{
		TS_ASSERT (m_Import->Import(m_DB, TOC_NAME_LINES)==false);
		TS_ASSERT (m_Import->Open(g_TestPathSHP + g_TestFileSHP_L)==true);
		TS_ASSERT (m_Import->Import(m_DB, TOC_NAME_LINES)==true);
	}
	
	void testElapsedTime()
	{
		TS_ASSERT (m_Import->GetElapsedTime()== 0);
		TS_ASSERT (m_Import->Open(g_TestPathSHP + g_TestFileSHP_L)==true);
		TS_ASSERT (m_Import->GetElapsedTime()== 0);
		TS_ASSERT (m_Import->Import(m_DB, TOC_NAME_LINES)==true);
		long myTime = m_Import->GetElapsedTime();
		TS_ASSERT_LESS_THAN (0, myTime);
		wxLogMessage(_T("Import done in %u [ms]"), myTime);
		
	}
	
	
	
		
};




#endif
