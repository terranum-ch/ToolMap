/***************************************************************************
								tmgisimport_test.cpp
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

#include "../../src/database/database.h"
#include "../../src/gis/tmgisimport.h"

#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>
//#include <iostream>

class tmGISImportTEST : public CppUnit::TestFixture 
{ 
	CPPUNIT_TEST_SUITE( tmGISImportTEST );
	CPPUNIT_TEST( TESTOpenDatabaseImport );
	CPPUNIT_TEST( TESTOpen );
	CPPUNIT_TEST (TESTFileSpatialType );
	CPPUNIT_TEST ( TESTFeatureCount );
	CPPUNIT_TEST ( TESTIsAllowed );
	CPPUNIT_TEST ( TESTImportInto );
	CPPUNIT_TEST ( TESTImport );
	CPPUNIT_TEST ( TESTElapsedTime );
	CPPUNIT_TEST ( TESTImportOneLine );
	CPPUNIT_TEST_SUITE_END();
	
private:
	DataBaseTM * m_DB;
	tmGISImport * m_Import;
	
public: 
	void setUp()
	{
		tmGISData::InitGISDrivers(true,true);
		m_DB = new DataBaseTM();
		CPPUNIT_ASSERT(m_DB->DataBaseOpen(_T("/Users/Lucien/Downloads/"),
										  _T("testimport")) == true);
		m_Import = new tmGISImport;
		
	}
	
	void tearDown() 
	{
		delete m_DB;
		delete m_Import;
	}
	
	void TESTOpenDatabaseImport()
	{
		CPPUNIT_ASSERT(m_DB->DataBaseOpen(_T("/Users/Lucien/Downloads/"),
										  _T("testfields")) == true);
	}
	
	void TESTOpen()
	{
		CPPUNIT_ASSERT (m_Import->IsOpen() == false);
		CPPUNIT_ASSERT (m_Import->Open(_T("/Users/Lucien/DATA/SIG/TOOLMAP/VECT/arcgis_line_test.shp"))==true);
		CPPUNIT_ASSERT (m_Import->IsOpen() == true);
		CPPUNIT_ASSERT (m_Import->Open(_T("/Users/Lucien/DATA/SIG/DATA/CH/cantonsuissesss.shp"))==false);
		CPPUNIT_ASSERT (m_Import->Open(_T("/Users/Lucien/DATA/SIG/SIRS_ERROR/index.html"))==false);
		CPPUNIT_ASSERT (m_Import->IsOpen() == false);

		
	}
	
	
	
	void TESTFileSpatialType ()
	{

		CPPUNIT_ASSERT (m_Import->Open(_T("/Users/Lucien/DATA/SIG/DATA/CH/cantonsuisse.shp"))==true);
		CPPUNIT_ASSERT (m_Import->GetSpatialType () == LAYER_SPATIAL_POLYGON);
		
	}
	
	void TESTFeatureCount ()
	{
		CPPUNIT_ASSERT (m_Import->GetFeatureCount() == -1);
		CPPUNIT_ASSERT (m_Import->Open(_T("/Users/Lucien/DATA/SIG/DATA/CH/cantonsuisse.shp"))==true);
		CPPUNIT_ASSERT (m_Import->GetFeatureCount() == 26);
	}
	
	void TESTIsAllowed ()
	{
		CPPUNIT_ASSERT (m_Import->IsImportAllowed()==false);
		CPPUNIT_ASSERT (m_Import->Open(_T("/Users/Lucien/DATA/SIG/DATA/CH/cantonsuisse.shp"))==true);
		CPPUNIT_ASSERT (m_Import->Open(_T("/Users/Lucien/DATA/SIG/TOOLMAP/VECT/arcgis_line_test.shp"))==true);
		CPPUNIT_ASSERT (m_Import->IsImportAllowed()==true);
	}
	
	void TESTImportOneLine()
	{
		CPPUNIT_ASSERT (m_Import->Open(_T("/Users/Lucien/DATA/SIG/DATA/GEOL/DIABLERETS/VECT/CADRE/DIABLERETS_FRONTIERE.shp"))==true);
		CPPUNIT_ASSERT (m_Import->IsImportAllowed()==true);
		CPPUNIT_ASSERT(m_Import->IsImportIntoAllowed (TOC_NAME_LINES)==true);
	}
	
	void TESTImportInto()
	{
		CPPUNIT_ASSERT(m_Import->IsImportIntoAllowed (TOC_NAME_LINES)==false);
		CPPUNIT_ASSERT (m_Import->Open(_T("/Users/Lucien/DATA/SIG/TOOLMAP/VECT/arcgis_line_test.shp"))==true);
		CPPUNIT_ASSERT(m_Import->IsImportIntoAllowed (TOC_NAME_LINES)==true);
		CPPUNIT_ASSERT(m_Import->IsImportIntoAllowed (TOC_NAME_FRAME)==true);
		CPPUNIT_ASSERT(m_Import->IsImportIntoAllowed (TOC_NAME_LABELS)==false);
	}
	
	void TESTImport ()
	{
		CPPUNIT_ASSERT (m_Import->Import(m_DB, TOC_NAME_LINES)==false);
		CPPUNIT_ASSERT (m_Import->Open(_T("/Users/Lucien/DATA/SIG/TOOLMAP/VECT/arcgis_line_test.shp"))==true);
		CPPUNIT_ASSERT (m_Import->Import(m_DB, TOC_NAME_LINES)==true);
	}
	
	void TESTElapsedTime()
	{
		CPPUNIT_ASSERT (m_Import->GetElapsedTime()== 0);
		CPPUNIT_ASSERT (m_Import->Open(_T("/Users/Lucien/DATA/SIG/TOOLMAP/VECT/arcgis_line_test.shp"))==true);
		CPPUNIT_ASSERT (m_Import->GetElapsedTime()== 0);
		CPPUNIT_ASSERT (m_Import->Import(m_DB, TOC_NAME_LINES)==true);
		long myTime = m_Import->GetElapsedTime();
		CPPUNIT_ASSERT (myTime > 0);
		wxLogDebug(_T("Import done in %u [ms]"), myTime);
		
	}
	
};
	


#endif