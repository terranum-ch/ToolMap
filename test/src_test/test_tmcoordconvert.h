/***************************************************************************
 test_tmcoordconvert
 -------------------
 copyright            : (C) 2013 CREALP Lucien Schreiber
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

#ifndef _TM_TEST_TM_COORD_CONVERT_H_
#define _TM_TEST_TM_COORD_CONVERT_H_


#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <cxxtest/TestSuite.h>
#include "test_param.h"
#include "../../src/core/tmcoordconvert.h"


class TEST_tmCoordConvert : public CxxTest::TestSuite
{
public:
	TEST_tmCoordConvert (bool bTest){
	}
	
	virtual ~TEST_tmCoordConvert(){
	}
	
	static TEST_tmCoordConvert *createSuite() { return new TEST_tmCoordConvert(true);}
    static void destroySuite( TEST_tmCoordConvert *suite ) { delete suite; }
    
	void setUp()
	{
	}
	
	void tearDown(){
	}
	
    
	void testName(){
		wxLogMessage(_T("------------------------------------"));
		wxLogMessage(_T("------- TESTING COORD CONVERT  -----"));
		wxLogMessage(_T("------------------------------------"));
	}
    
    void testSwissToWGS84(){
        tmCoordConvert myConvert (PROJ_SWISSPROJ);
        wxRealPoint myGenfPoint (500374,117993);
        wxRealPoint myGenfPointWGS = myConvert.GetPointWGS(myGenfPoint);
        wxLogMessage( "SWISS coord is: %f, %f", myGenfPoint.x, myGenfPoint.y);
        wxLogMessage( "WGS84 coord is: %f, %f", myGenfPointWGS.x, myGenfPointWGS.y);
        TS_ASSERT_DELTA (myGenfPointWGS.x, 6.147801, 0.000001);
        TS_ASSERT_DELTA (myGenfPointWGS.y, 46.205999, 0.000001);
    }
    
    void testSwissToGoogle(){
        tmCoordConvert myConvert (PROJ_SWISSPROJ);
        wxRealPoint myGenfPoint (500374,117993);
        wxRealPoint myGenfPointG = myConvert.GetPointGoogle(myGenfPoint);
        wxLogMessage( "Google coord is: %f, %f", myGenfPointG.x, myGenfPointG.y);
        TS_ASSERT_DELTA (myGenfPointG.x, 684370.102237, 0.000001);
        TS_ASSERT_DELTA (myGenfPointG.y, 5813422.394739, 0.000001);
    }
  
	
};

#endif
