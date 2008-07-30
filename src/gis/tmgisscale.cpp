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
	m_ExtentWndMM = wxSize(0,0);
	m_UnitScale = 0;
	m_PixelSize = 0;
	m_ExtentWndMM = wxSize(0,0);
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
	
	// scale has changed
	ComputeUnitScale();
	
	return TRUE;
}



double tmGISScale::GetBestDivFactor (const wxRect & selected_rect)
{
	double dx = (double) m_ExtentWnd.GetWidth() / (double)selected_rect.GetWidth();
	double dy = (double) m_ExtentWnd.GetHeight() / (double)selected_rect.GetHeight();
	
	if (dy < dx)
		return dy;
	
	return dx;
}



void tmGISScale::ComputeNewRealZoomExtent (const wxRect & calc_wnd_extent, const wxPoint & top_left)
{
	double drealx = m_ExtentWndReal.x_max - m_ExtentWndReal.x_min;
	double drealy = m_ExtentWndReal.y_max - m_ExtentWndReal.y_min;
	
	// new pixel size
	double dOldPxSize = m_PixelSize;
	//m_PixelSize = drealx / (m_PixelSize * ((double)calc_wnd_extent.GetWidth()));
	
	// set new real windows coordinates
	m_ExtentWndReal.x_min = m_ExtentWndReal.x_min + (((double)top_left.x) * dOldPxSize);
	m_ExtentWndReal.y_min = m_ExtentWndReal.y_min + (((double)top_left.y) * dOldPxSize);
	m_ExtentWndReal.x_max = m_ExtentWndReal.x_min + (((double)calc_wnd_extent.GetWidth()) * dOldPxSize);
	m_ExtentWndReal.y_max = m_ExtentWndReal.y_min + (((double)calc_wnd_extent.GetHeight()) * dOldPxSize);
	
	m_PixelSize = (m_ExtentWndReal.x_max - m_ExtentWndReal.x_min) / ((double)m_ExtentWnd.GetWidth());
	
	// scale has changed
	ComputeUnitScale();
}


void tmGISScale::ComputeNewRealPanExtent (const wxPoint & offsetxtop)
{
	double dmovex = ((double)offsetxtop.x) * m_PixelSize;
	double dmovey = ((double)offsetxtop.y) * m_PixelSize;
	
	m_ExtentWndReal.x_min = RemoveFromCoord(m_ExtentWndReal.x_min, dmovex);
	m_ExtentWndReal.x_max = RemoveFromCoord(m_ExtentWndReal.x_max, dmovex);
	
	m_ExtentWndReal.y_min = RemoveFromCoord(m_ExtentWndReal.y_min, dmovey);
	m_ExtentWndReal.y_max = RemoveFromCoord(m_ExtentWndReal.y_max, dmovey);
}


long tmGISScale::ComputeUnitScale ()
{
	int MMtoM = 1000;
	double ddistance = 0.0;
	if (m_ExtentWndReal.x_max != 0)
	{
		if (m_ExtentWndReal.x_max < 0)
		{
			ddistance = m_ExtentWndReal.x_max + m_ExtentWndReal.x_min;
		}
		else
		{
			ddistance = m_ExtentWndReal.x_max - m_ExtentWndReal.x_min;
		}
		
	// send message to scale control
		
	m_UnitScale =  ddistance / (((double)m_ExtentWndMM.GetWidth()) / MMtoM);	
		return m_UnitScale;
	}
	return 0;
}


bool tmGISScale::IsLayerExtentValid()
{
	if (m_ExtentMaxLayers == tmRealRect(0,0,0,0))
		return FALSE;
	return TRUE;
}


void tmGISScale::DistanceFromScale (const long & scale, double & xdist, double & ydist)
{
	xdist = ((double)m_ExtentWndMM.GetWidth()) / 1000 * scale;
	ydist = ((double)m_ExtentWndMM.GetHeight()) / 1000 * scale;
	m_UnitScale = scale;
}


void tmGISScale::ComputeNewScaleExtent (const long & scale)
{
	// getting new distance from scale and window with (mm)
	double dNewDistX = 0, dNewDistY = 0;
	DistanceFromScale(scale, dNewDistX, dNewDistY);
	
	// getting difference between new distance and actual distance
	// distance mean (windows extent real x max - x min)
	double dActDistX = DifferenceCoord(m_ExtentWndReal.x_max, m_ExtentWndReal.x_min);
	double dActDistY = DifferenceCoord(m_ExtentWndReal.y_max, m_ExtentWndReal.y_min);
	
	double dDiffX = (dNewDistX - dActDistX) / 2;
	double dDiffY = (dNewDistY - dActDistY) / 2;
	
	m_ExtentWndReal.x_min = RemoveFromCoord(m_ExtentWndReal.x_min, dDiffX);
	m_ExtentWndReal.y_min = RemoveFromCoord(m_ExtentWndReal.y_min, dDiffY);
	m_ExtentWndReal.x_max = AppendToCoord(m_ExtentWndReal.x_max, dDiffX);
	m_ExtentWndReal.y_max = AppendToCoord(m_ExtentWndReal.y_max, dDiffY);
	
	// change pixels size too :-)
	m_PixelSize = DifferenceCoord(m_ExtentWndReal.x_max, m_ExtentWndReal.x_min) / 
				((double)m_ExtentWnd.GetWidth());

}



