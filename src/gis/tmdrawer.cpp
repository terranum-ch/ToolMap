/***************************************************************************
								tmdrawer.cpp
						Draws GIS data into bitmap
                             -------------------
    copyright            : (C) 2007 CREALP Lucien Schreiber 
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

// comment doxygen

#include "tmdrawer.h"


tmDrawer::tmDrawer()
{

}



tmDrawer::~tmDrawer()
{

}



/***************************************************************************//**
 @brief Draw the max extent
 @details Draw a red rectangle corresponding to the maximum extent of all
 visible layers
 @param bitmap pointer to a valid bitmap
 @param scale actual values of scale and zoom
 @return  TRUE if bitmap drawn, FALSE if bitmap not valid
 @author Lucien Schreiber (c) CREALP 2008
 @date 24 July 2008
 *******************************************************************************/
bool tmDrawer::DrawExtentIntoBitmap (wxBitmap * bitmap, tmGISScale & scale)
{

	if (!bitmap)
	{
		wxLogDebug(_T("No bitmap present, unable to draw into"));
		return FALSE;
	}
	
	
	wxMemoryDC temp_dc;
	temp_dc.SelectObject(*bitmap);
	
	temp_dc.SetPen(*wxRED_PEN);
	temp_dc.SetBackground(*wxWHITE);
	
	tmRealRect myRealExt = scale.GetMaxLayersExtent();
	
	wxPoint pts[5];
	pts[0] = scale.RealToPixel(wxRealPoint(myRealExt.x_min, myRealExt.y_min));
	pts[1] = scale.RealToPixel(wxRealPoint(myRealExt.x_max, myRealExt.y_min));
	pts[2] = scale.RealToPixel(wxRealPoint(myRealExt.x_max, myRealExt.y_max));
	pts[3] = scale.RealToPixel(wxRealPoint(myRealExt.x_min, myRealExt.y_max));
	pts[4] = scale.RealToPixel(wxRealPoint(myRealExt.x_min, myRealExt.y_min));
	
	temp_dc.DrawLines(5, pts);
	
	
	wxBitmap nullbmp;
	temp_dc.SelectObject(nullbmp);
	
	return TRUE;
}

