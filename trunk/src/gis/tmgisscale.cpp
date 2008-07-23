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



tmRealRect tmGISScale::ComputeMaxCoord (const tmRealRect & r1, const tmRealRect & r2)
{
	// if one of the extent is null
	if (r1 == tmRealRect(0,0,0,0))
		return r2;
	if (r2 == tmRealRect(0,0,0,0))
		return r1;
	
	if (r1 == r2)
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
	m_ExtentMaxLayers = ComputeMaxCoord(r, m_ExtentMaxLayers);
}


double tmGISScale::ComputeDivFactor (wxSize wnd_extent)
{
	if (wnd_extent == wxDefaultSize)
		wnd_extent = wxSize(m_ExtentWnd.GetWidth(), m_ExtentWnd.GetHeight());
	
	double dx = 0, dy=0;
	
	// check data
	if (wnd_extent == wxSize(0,0) || m_ExtentMaxLayers == tmRealRect(0,0,0,0))
		return 0.0;
	
	dx = (m_ExtentMaxLayers.x_max -  m_ExtentMaxLayers.x_min) / (wnd_extent.GetWidth());
	dy = (m_ExtentMaxLayers.y_max - m_ExtentMaxLayers.y_min) / (wnd_extent.GetHeight());
	if (dx > dy)
		return dx;
	else 
		return dy;
}



bool tmGISScale::ComputeMaxExtent ()
{
	// get smaller drawing area than real for margin
	wxSize myMarginSize (m_ExtentWnd.GetWidth() - tmSCALE_MARGIN,
						 m_ExtentWnd.GetHeight() - tmSCALE_MARGIN);
	
	// compute best div factor
	m_PixelSize = ComputeDivFactor(myMarginSize);
	
	// compute centering
	wxSize myOffset = ComputeCenterPxWnd(m_PixelSize);
	
	// compute real extent
	ComputeMaxExtentReal(myOffset);
	
	return TRUE;
	
}


wxSize tmGISScale::ComputeCenterPxWnd(double divratio, wxSize wnd_extent)
{
	if (wnd_extent == wxDefaultSize)
		wnd_extent =  wxSize(m_ExtentWnd.GetWidth(), m_ExtentWnd.GetHeight());
	
	double dXwidth = (m_ExtentMaxLayers.x_max -  m_ExtentMaxLayers.x_min) / divratio;
	double dYwidth = (m_ExtentMaxLayers.y_max - m_ExtentMaxLayers.y_min) / divratio;
	
	double dXRest = wnd_extent.GetWidth() - dXwidth;
	double dYRest = wnd_extent.GetHeight() - dYwidth;
	
	dXRest = dXRest / 2;
	dYRest = dYRest / 2;
	
	return wxSize (dXRest, dYRest);
}



bool tmGISScale::ComputeMaxExtentReal (wxSize wnd_offset)
{
	int xoffset = 0;
	int yoffset = 0;
	
	// add the offset to the computation
	if (wnd_offset != wxDefaultSize)
	{
		xoffset = wnd_offset.GetWidth();
		yoffset = wnd_offset.GetHeight();
	}
	
	m_ExtentWndReal.x_min = m_ExtentMaxLayers.x_min - m_PixelSize * xoffset;
	m_ExtentWndReal.x_max = m_ExtentWndReal.x_min + m_PixelSize * m_ExtentWnd.GetWidth();
	
	m_ExtentWndReal.y_min = m_ExtentMaxLayers.y_min - m_PixelSize * yoffset;
	m_ExtentWndReal.y_max = m_ExtentWndReal.y_min + m_PixelSize * m_ExtentWnd.GetHeight();
	
	return TRUE;
}




