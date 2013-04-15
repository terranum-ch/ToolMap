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


bool tmIsSameDouble(double left, double right, double epsilon)
{
	return (fabs(left - right) < epsilon);
}


/***************************************************************************//**
 @brief Clip rectangle with another
 @details This function clip the rectangle with a src one and return the result.
 @param src Rectangle used for clipping (bigger)
 @param result Result of a clipping area, in case of no clipping, value of
 result isn't changed
 @return true if we can clip (image visible), false if we are outside the cliping area
 @author Lucien Schreiber (c) CREALP 2008
 @date 24 September 2008
 *******************************************************************************/
bool tmRealRect::Clip (const tmRealRect & src, tmRealRect & result)
{
	//double srcWidth = src.GetWidth();
	//double srcHeight = src.GetHeight();
	
	// Check to see if the image is even visible
	if(x_max <= src.x_min) // Too far off to the left
		return false;
	if(x_min >= src.x_max) // Too far off to the right
		return false;
	if(y_min >= src.y_max) // Too far off to the top
		return false;
	if(y_max <= src.y_min) // Too far off to the bottom
		return false;
	
	// copy into result if no clipping
	result = tmRealRect(x_min, y_min, x_max, y_max);
	
	// Ok image visible proceed with cliping
	if(x_min < src.x_min) // Image is hidden partially by the left edge of the screen
	{
		// Crop from lef
		result.x_min = src.x_min;
	}

	if(x_max > src.x_max) // Image is hidden partially by the right edge of the screen
	{
		// Crop from right
		result.x_max = src.x_max;
	}
	
	if(y_min < src.y_min) // Image is hidden partially by the top edge of the screen
	{
		// Crop from top
		result.y_min = src.y_min;
	}

	if(y_max > src.y_max) // Image is hidden parially by the bottom edge of the screen
	{
		// Crop from bottom
		result.y_max = src.y_max;
	}
	// Since the image is visible, return true
	return true;
}




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


double tmGISScale::GetLayersExtentWidth()
{
	return DifferenceCoord(m_ExtentMaxLayers.x_max, m_ExtentMaxLayers.x_min);
}


double tmGISScale::GetLayersExtentHeight()
{
	return DifferenceCoord(m_ExtentMaxLayers.y_max, m_ExtentMaxLayers.y_min);
}



double tmGISScale::GetWindowRealWidth()
{
	return DifferenceCoord(m_ExtentWndReal.x_max, m_ExtentWndReal.x_min);
}



double tmGISScale::GetwindowRealHeight()
{
	return DifferenceCoord(m_ExtentWndReal.y_max, m_ExtentWndReal.y_min);	
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




/***************************************************************************//**
 @brief Compute new real extend when windows size change
 @author Lucien Schreiber (c) CREALP 2009
 @date 24 April 2009
 *******************************************************************************/
bool tmGISScale::ComptuteNewWindowSize (const wxSize & oldsize, const wxSize & newsize)
{
	m_ExtentWndReal.x_max = AppendToCoord(m_ExtentWndReal.x_min, m_PixelSize * m_ExtentWnd.GetWidth());
	m_ExtentWndReal.y_min = RemoveFromCoord(m_ExtentWndReal.y_max, m_PixelSize * m_ExtentWnd.GetHeight());
	return true;
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
	
	// keep old pixel size
	double dOldPxSize = m_PixelSize;
	
	// compute new x / y
	m_ExtentWndReal.x_min = AppendToCoord(m_ExtentWndReal.x_min, ((double)top_left.x) * dOldPxSize);
	m_ExtentWndReal.x_max = AppendToCoord(m_ExtentWndReal.x_min, (((double)calc_wnd_extent.GetWidth()) * dOldPxSize));

	m_ExtentWndReal.y_max = RemoveFromCoord(m_ExtentWndReal.y_max, (((double)top_left.y) * dOldPxSize));
	m_ExtentWndReal.y_min = RemoveFromCoord(m_ExtentWndReal.y_max, (((double)calc_wnd_extent.GetHeight()) * dOldPxSize));
		
	m_PixelSize = (m_ExtentWndReal.x_max - m_ExtentWndReal.x_min) / ((double)m_ExtentWnd.GetWidth());
	
	// scale has changed
	ComputeUnitScale();
}


void tmGISScale::ComputePrevZoomExtent (double pixelsize,const wxPoint2DDouble & topleft)
{
	
	m_PixelSize = pixelsize;
	m_ExtentWndReal.x_min = topleft.m_x;
	m_ExtentWndReal.y_max = topleft.m_y;
	
	m_ExtentWndReal.x_max = AppendToCoord(m_ExtentWndReal.x_min, m_PixelSize * m_ExtentWnd.GetWidth());
	m_ExtentWndReal.y_min = RemoveFromCoord(m_ExtentWndReal.y_max, m_PixelSize * m_ExtentWnd.GetHeight());
	
	ComputeUnitScale();	
}



void tmGISScale::ComputeNewRealPanExtent (const wxPoint & offsetxtop)
{
	double dmovex = ((double)offsetxtop.x) * m_PixelSize;
	double dmovey = ((double)offsetxtop.y) * m_PixelSize;
	
	m_ExtentWndReal.x_min = RemoveFromCoord(m_ExtentWndReal.x_min, dmovex);
	m_ExtentWndReal.x_max = RemoveFromCoord(m_ExtentWndReal.x_max, dmovex);
	
	m_ExtentWndReal.y_min = AppendToCoord(m_ExtentWndReal.y_min, dmovey);
	m_ExtentWndReal.y_max = AppendToCoord(m_ExtentWndReal.y_max, dmovey);
}



// moving, zooming display
bool tmGISScale::MoveViewTo (const vrRealRect & rect){
	vrRealRect myWndExtent;
	myWndExtent.SetLeftTop(wxPoint2DDouble(m_ExtentWndReal.x_min, m_ExtentWndReal.y_max));
	myWndExtent.SetRightBottom(wxPoint2DDouble(m_ExtentWndReal.x_max, m_ExtentWndReal.y_min));
	wxASSERT(myWndExtent.IsOk());
	
	wxPoint2DDouble myCenter = rect.GetCentre();
	myWndExtent.SetCentre(myCenter);
	
	if (myWndExtent.IsOk() == false){
		wxLogError(_T("Error moving display to %.2f, %.2f"), myCenter.m_x, myCenter.m_y);
		return false;
	}
	
	m_ExtentWndReal.x_min = myWndExtent.GetLeft();
	m_ExtentWndReal.y_max = myWndExtent.GetTop();
	m_ExtentWndReal.x_max = myWndExtent.GetRight();
	m_ExtentWndReal.y_min = myWndExtent.GetBottom();
	return true;	
}


bool tmGISScale::ZoomViewTo (const vrRealRect & rect){

	vrRealRect myWndExtent;
	myWndExtent.SetLeftTop(wxPoint2DDouble(m_ExtentWndReal.x_min, m_ExtentWndReal.y_max));
	myWndExtent.SetRightBottom(wxPoint2DDouble(m_ExtentWndReal.x_max, m_ExtentWndReal.y_min));
	wxASSERT(myWndExtent.IsOk());
	
	// check for size too small
	if (fabs(rect.m_width) <= 1 && fabs(rect.m_height) <= 1) {
		wxLogError(_("Object size is too small for zooming\n width = %.f, height = %.f"),
				   fabs(rect.m_width), fabs(rect.m_height));
		return false;
	}
	
	double dWndwidth = m_ExtentWnd.GetWidth() - tmSCALE_MARGIN;
	double dWndheight = m_ExtentWnd.GetHeight()  - tmSCALE_MARGIN;

	// compute shape 
	double dpixelx = rect.m_width / dWndwidth;
	double dpixely = rect.m_height / dWndheight;
	
	// computedivfactor
	double myDivFactor = 0.0;
	if (fabs(dpixelx) >= fabs(dpixely)) {
		myDivFactor = dpixelx;
	}
	else {
		myDivFactor = dpixely;
	}
	
	double myDivFactorX = fabs(myDivFactor);
	double myDivFactorY = fabs(myDivFactor);
	
	if (dpixelx <= 0) {
		myDivFactorX = myDivFactorX * -1;
	}
	
	if (dpixely <= 0) {
		myDivFactorY = myDivFactorY * -1;
	}
	
	vrRealRect myNewWndExtent;
	myNewWndExtent.m_width = m_ExtentWnd.GetWidth();
	myNewWndExtent.m_height = m_ExtentWnd.GetHeight();
	
	// scale
	myNewWndExtent.m_width *= myDivFactorX;
	myNewWndExtent.m_height *= myDivFactorY;
	
	
	// center rect
	wxPoint2DDouble myCenter = rect.GetCentre();
	myNewWndExtent.SetCentre(myCenter);
	
	
	m_ExtentWndReal.x_min = myNewWndExtent.GetLeft();
	m_ExtentWndReal.y_max = myNewWndExtent.GetTop();
	m_ExtentWndReal.x_max = myNewWndExtent.GetRight();
	m_ExtentWndReal.y_min = myNewWndExtent.GetBottom();
	m_PixelSize = fabs(myDivFactor);
	
	ComputeUnitScale();
	
	return true;
}




/***************************************************************************//**
 @brief Convert pixel rectange to real units
 @param rectpx pixels values rectangle to convert
 @return  The real coordinates
 @author Lucien Schreiber (c) CREALP 2008
 @date 29 October 2008
 *******************************************************************************/
tmRealRect tmGISScale::PixelsToReal (const wxRect & rectpx)
{
	tmRealRect rectreal (0,0,0,0);
	
	wxRealPoint bottomleft = PixelToReal(wxPoint(rectpx.x, rectpx.y));
	
	rectreal.x_min = bottomleft.x;
	rectreal.y_max = bottomleft.y;
	rectreal.x_max = AppendToCoord(rectreal.x_min, rectpx.width * m_PixelSize);
	rectreal.y_min = RemoveFromCoord(rectreal.y_max, rectpx.height * m_PixelSize);
	return rectreal;
}


wxPoint tmGISScale::RealToPixel (wxRealPoint realpt){
    wxPoint myPt (wxRound((realpt.x - m_ExtentWndReal.x_min) / m_PixelSize),
                  wxRound(((double) m_ExtentWnd.height) - ((realpt.y - m_ExtentWndReal.y_min) / m_PixelSize)));
    return myPt;
}


long tmGISScale::ComputeUnitScale ()
{
	//int MMtoM = 1000;
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
		double dInchPx = 1.0 / 0.0254 * ((double)m_PPI.x);
		double dSizeMH = ((double) m_ExtentWnd.width) / dInchPx; 
		
		m_UnitScale = wxRound(ddistance / dSizeMH);
		//m_UnitScale = wxRound( ddistance / (((double)m_ExtentWndMM.GetWidth()) / 1000.0));	
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
	wxASSERT(m_PPI.x = m_PPI.y);
	double dInchPx = 1.0 / 0.0254 * ((double)m_PPI.x);
	double dSizeMH = ((double) m_ExtentWnd.width) / dInchPx; 
	double dSizeMV = ((double) m_ExtentWnd.height) / dInchPx; 

	xdist = dSizeMH * ((double) scale);
	ydist = dSizeMV * ((double) scale);
	//xdist = ((double)m_ExtentWndMM.GetWidth()) / 1000.0 * (double) scale;
	//ydist = ((double)m_ExtentWndMM.GetHeight()) / 1000.0 * (double) scale;
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
	
	double dDiffX = (dNewDistX - dActDistX) / 2.0;
	double dDiffY = (dNewDistY - dActDistY) / 2.0;
	
	m_ExtentWndReal.x_min = RemoveFromCoord(m_ExtentWndReal.x_min, dDiffX);
	m_ExtentWndReal.y_min = RemoveFromCoord(m_ExtentWndReal.y_min, dDiffY);
	m_ExtentWndReal.x_max = AppendToCoord(m_ExtentWndReal.x_max, dDiffX);
	m_ExtentWndReal.y_max = AppendToCoord(m_ExtentWndReal.y_max, dDiffY);
	
	// change pixels size too :-)
	m_PixelSize = DifferenceCoord(m_ExtentWndReal.x_max, m_ExtentWndReal.x_min) / 
				((double)m_ExtentWnd.GetWidth());
}


