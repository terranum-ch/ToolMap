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
	
	static TEST_tmCoordConvert *createSuite() { return new  TEST_tmCoordConvert(true);}
    static void destroySuite( TEST_tmCoordConvert *suite ) { delete suite; }
    
	void setUp()
	{
		wxApp::SetInstance(new wxAppConsole());
	}
	
	void tearDown(){
	}
	
    
	void testName(){
		wxLogMessage(_T("------------------------------------"));
		wxLogMessage(_T("------- TESTING COORD CONVERT  -----"));
		wxLogMessage(_T("------------------------------------"));
	}
    
    void testSwissToWGS84(){
        tmCoordConvert myConvert (PROJ_SWISS_CH1903);
        wxRealPoint myGenfPoint (500374,117993);
        wxRealPoint myGenfPointWGS = myConvert.GetPointWGS(myGenfPoint);
        wxLogMessage( "SWISS coord is: %f, %f", myGenfPoint.x, myGenfPoint.y);
        wxLogMessage( "WGS84 coord is: %f, %f", myGenfPointWGS.x, myGenfPointWGS.y);
        TS_ASSERT_DELTA (myGenfPointWGS.x, 6.147801, 0.000001);
        TS_ASSERT_DELTA (myGenfPointWGS.y, 46.205999, 0.000001);
    }
    
    void testSwissToGoogle(){
        tmCoordConvert myConvert (PROJ_SWISS_CH1903);
        wxRealPoint myGenfPoint (500374,117993);
        wxRealPoint myGenfPointG = myConvert.GetPointGoogle(myGenfPoint);
        wxLogMessage( "Google coord is: %f, %f", myGenfPointG.x, myGenfPointG.y);
        TS_ASSERT_DELTA (myGenfPointG.x, 684370.0431519132, 0.000001);
        TS_ASSERT_DELTA (myGenfPointG.y, 5813422.4727304634, 0.000001);
    }
    
    void testDistanceWGS84(){
        tmCoordConvert myConvert (PROJ_WORLD_WGS84);
        wxRealPoint myP1 (6.14712, 46.20593); // GENEVA
        wxRealPoint myP2 (7.45782, 46.94832); // BERN
        double myDist = myConvert.GetDistance(myP1, myP2);
        wxLogMessage ("Distance between Geneva and Bern : %f [m]", myDist);
        TS_ASSERT_DELTA (myDist, 130015.3, 0.05);
    }
    
    void testDistanceSwiss(){
        tmCoordConvert myConvert (PROJ_SWISS_CH1903);
        wxRealPoint myP1 (500321.34231, 117986.13451); // GENEVA
        wxRealPoint myP2 (601460.745203, 199693.022818); // BERN
        double myDist = myConvert.GetDistance(myP1, myP2);
        TS_ASSERT_DELTA (myDist, 130015.3, 0.05);
    }
    
    void testPointAtDistanceWGS84() {
        tmCoordConvert myConvert (PROJ_WORLD_WGS84);
        wxRealPoint myP1 (6.14712, 46.20593); // GENEVA
        // search point 20 km east of geneva
        wxRealPoint myP2 = myConvert.GetPointAtDistance(myP1, 20000, 90);
        TS_ASSERT_DELTA(myP2.x, 6.40626894 , 0.00001);
        TS_ASSERT_DELTA(myP2.y, 46.20563628, 0.00001);
    }
    
    void testPointAtDistanceSWISS() {
        tmCoordConvert myConvert (PROJ_SWISS_CH1903);
        wxRealPoint myGenfPoint (500374,117993);
        // search point 100 m east of geneva
        wxRealPoint myP2 = myConvert.GetPointAtDistance(myGenfPoint, 100, 90);
        TS_ASSERT_DELTA(myP2.x, 500474 , 0.5);
        TS_ASSERT_DELTA(myP2.y, myGenfPoint.y, 2);
    }

  
	
};

#endif
