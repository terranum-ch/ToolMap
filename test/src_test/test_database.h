/***************************************************************************
								test_database.h
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

#ifndef _TEST_VR_DRIVER_H_
#define _TEST_VR_DRIVER_H_


#include "wx/wxprec.h"
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "test_param.h"	//for test parameters
#include "../../src/database/database.h"


class TEST_Database : public CxxTest::TestSuite
{
private:
	DataBase * m_DB;	
	
public:
	void setUp()
	{
		m_DB = new DataBase();		
	}
	
	void tearDown() 
	{
		delete m_DB;
	}
	
	// setting output to the std err (otherwise log into windows)
	void testOutputToWindows()
	{
		wxLog::SetActiveTarget(new wxLogStderr());
	}

	void TESTOpenDatabase()
	{
		TS_ASSERT(m_DB->DataBaseOpen(g_TestPathPRJ, g_TestPrj_CombioulaCorrect));
		wxString myFalsePath = g_TestPathPRJ + _T("JJJ");
		TS_ASSERT( m_DB->DataBaseOpen(myFalsePath, g_TestPrj_CombioulaExercice)==false);
		TS_ASSERT( m_DB->DataBaseOpen(g_TestPathPRJ, g_TestPrj_CombioulaExercice));
	}
	
};







#endif
