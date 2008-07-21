/***************************************************************************
								tmgisscale.cpp
                    Deals with GIS scale and conversion
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

#include "tmgisscale.h"



tmGISScale::tmGISScale()
{
	InitMemberValues();
}



tmGISScale::~tmGISScale()
{

}



void tmGISScale::InitMemberValues()
{
	m_ExtentMaxLayers = tmRealRect(0,0,0,0);
	m_ExtentWnd = wxRect(0,0,0,0);
}



tmRealRect tmGISScale::ComputeMaxExtent (const tmRealRect & r1, const tmRealRect & r2)
{
	// if one of the extent is null
	if (r1 == tmRealRect(0,0,0,0))
		return r2;
	if (r2 == tmRealRect(0,0,0,0))
		return r1;
	
	tmRealRect retRect (r2.x_min,r2.y_min,r2.x_max,r2.y_max);
	
	if (r1.x_min < r2.x_min) 
		retRect.x_min = r1.x_min;
	if (r1.y_min < r2.y_min) 
		retRect.y_min = r1.y_min;
	if (r1.x_max > r2.x_max) 
		retRect.x_max = r1.x_max;
	if (r1.y_max > r2.y_max) 
		retRect.y_max = r1.y_max;
	
	return retRect;
}



void tmGISScale::SetMaxLayersExtentAsExisting (const tmRealRect & r)
{
	m_ExtentMaxLayers = ComputeMaxExtent(r, m_ExtentMaxLayers);
}


double tmGISScale::ComputeDivFactor ()
{
	double dx = 0, dy=0;
	
	// check data
	if (m_ExtentWnd == wxRect(0,0,0,0) || m_ExtentMaxLayers == tmRealRect(0,0,0,0))
		return 0.0;
	
	dx = (m_ExtentMaxLayers.x_max -  m_ExtentMaxLayers.x_min) / m_ExtentWnd.GetWidth();
	dy = (m_ExtentMaxLayers.y_max - m_ExtentMaxLayers.y_min) / m_ExtentWnd.GetHeight();
	if (dx > dy)
		return dx;
	else 
		return dy;
}

