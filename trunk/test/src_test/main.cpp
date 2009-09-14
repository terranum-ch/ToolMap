/***************************************************************************
								main.cpp
				test program for ToolMap 2. Unit testing using
							cppunit library
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
 **************************************************************************/
#include <stdio.h>
#include "wx/string.h"


#define UNIT_TESTING


#include <cppunit/ui/text/TestRunner.h>

// INCLUDE TESTS FILES
// DATABASE CLASS
#include "../../src/database/database.cpp"
#include "database_test.cpp"	// testing the database

//DATABASE TM CLASS
#include "../../src/gis/tmsymbol.cpp"
#include "../../src/gis/tmsymboldlg.cpp"
#include "../../src/gis/tmsymboldlgraster.cpp"
#include "../../src/gis/tmsymbolvector.cpp"
#include "../../src/gis/tmsymbolraster.cpp"
#include "../../src/gis/tmsymbolvectorline.cpp"
#include "../../src/gis/tmsymbolvectorpoint.cpp"
#include "../../src/gis/tmsymbolvectorpolygon.cpp"
#include "../../src/core/tmsliderwithtxt.cpp"
#include "../../src/core/tmcolourpickerctrl.cpp"
#include "../../src/gis/tmsymboldlgline.cpp"
#include "../../src/gis/tmsymboldlgpoint.cpp"
#include "../../src/gis/tmsymboldlgpolygon.cpp"
/*#include "../../
#include "../../*/

#include "../../src/core/tmsnappingmemory.cpp"
#include "../../src/components/wxserialize/tmserialize.cpp"
#include "../../src/gis/tmlayerproperties.cpp"
#include "../../src/gui/wxflatbutton.cpp"
#include "../../src/core/tmvalidator.cpp"
#include "../../src/gui/listgenreport_status.cpp"
#include "../../src/gui/objectdefinitionlistdlg.cpp"
#include "../../src/core/scale.cpp"
#include "../../src/core/wxdirpickerctrlbest.cpp"
#include "../../src/core/textparser.cpp"
#include "../../src/gui/projectproperties_dlg.cpp"
#include "../../src/gui/listgenreport.cpp"
#include "../../src/core/prjdefmemmanage.cpp"
#include "../../src/core/projectdefmemory.cpp"
#include "../../src/database/database_tm.cpp"
#include "database_tm_test.cpp"	// testing the toolmap database



// DRAWEREDIT
#include "../../src/gis/tmdraweredit.cpp"
#include "../../src/core/tmarraysize.cpp"
#include "tmdrawereditline_test.cpp"


//DANGLING NODES

#include "../../src/gis/tmgisdatavectormysql.cpp"
#include "../../src/gis/tmgisdatavector.cpp"
#include "../../src/gis/tmgisdatavectorshp.cpp"
#include "../../src/gis/tmgisdata.cpp"
#include "../../src/gis/tmgisdataraster.cpp"
#include "../../src/gis/tmgisdatarasteregrid.cpp"
#include "../../src/gis/tmgisdatarastertiff.cpp"
#include "../../src/gis/tmgisdatarasterjpeg.cpp"
#include "../../src/gis/tmgisscale.cpp"
#include "tooldanglingnodes_test.cpp"
#include "../../src/gis/tooldanglingnodes.cpp"

//UPDATE
#include "../../src/components/tmupdate/tmupdate.cpp"
#include "tmupdate_test.cpp"

// IMPORT GIS DATA
#include "tmgisimport_test.cpp"
#include "../../src/gis/tmgisimport.cpp"

int main (int argc, char * const argv[])
{

	CppUnit::TextUi::TestRunner runner;
	//runner.addTest( DataBaseTEST::suite() );
	//runner.addTest( DataBaseTMTEST::suite() );
	//runner.addTest( tmDrawerEditLineTEST::suite() );
	//runner.addTest( tmToolDanglingNodesTEST::suite() );
	//runner.addTest( tmUpdateTEST::suite());
	runner.addTest(tmGISImportTEST::suite());
	runner.run();
	return 0;
}


