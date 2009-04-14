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

#include <cppunit/ui/text/TestRunner.h>

// INCLUDE TESTS FILES
#include "../../src/database/database.cpp"
#include "database_test.cpp"	// testing the database





int main (int argc, char * const argv[])
{

	CppUnit::TextUi::TestRunner runner;
	runner.addTest( DataBaseTEST::suite() );
	runner.run();
	return 0;
}


