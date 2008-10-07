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
	m_bmp = NULL;
	m_IsInitialised = FALSE;

	// load image using GDAL
	// owned by image, do not destroy manually.
	imgbuf = NULL;
	imglen = 0;
	maskbuf = NULL;
	masklen= 0;
}



tmDrawer::~tmDrawer()
{

}



void tmDrawer::InitDrawer(wxBitmap * bitmap, tmGISScale & scale, const tmRealRect & filter)
{
	m_bmp = bitmap;
	m_scale = scale;
	m_spatFilter = filter;
	
	if (m_bmp && m_bmp->IsOk())
		m_IsInitialised = TRUE;
	else
	{
		m_IsInitialised = FALSE;
		fprintf(stderr, "%s line %d : Error initing drawer \n ", __FUNCTION__, __LINE__);
	}
}


/***************************************************************************//**
 @brief Draw the max extent
 @details Draw a rectangle of specified colour and width corresponding to the 
 maximum extent of all visible layers
 @param width Width of the pen (in pixels)
 @param col Colour of the pen
 @return always true
 @author Lucien Schreiber (c) CREALP 2008
 @date 24 July 2008
 *******************************************************************************/
bool tmDrawer::DrawExtentIntoBitmap (int width, const wxColour & col)
{
	// check for initialisation
	if(!m_IsInitialised)
	{
		wxString sFunction = wxString::FromAscii(__FUNCTION__);
		wxString sFunctionLineError = wxString::Format( _T("%s line %d : "),
													   sFunction.c_str(), __LINE__); 
		wxString sErrMsg = wxString::Format(_T("%s Drawer not initialised"),
											sFunctionLineError.c_str());
		
		wxASSERT_MSG(0,sErrMsg);
	}
	
	
	wxMemoryDC temp_dc;
	temp_dc.SelectObject(*m_bmp);
	
	wxPen myPen (col, width);
	temp_dc.SetPen(myPen);
	temp_dc.SetBackground(*wxWHITE);
	
	
	tmRealRect myRealExt = m_scale.GetMaxLayersExtent();
	
	wxPoint pts[5];
	pts[0] = m_scale.RealToPixel(wxRealPoint(myRealExt.x_min, myRealExt.y_min));
	pts[1] = m_scale.RealToPixel(wxRealPoint(myRealExt.x_max, myRealExt.y_min));
	pts[2] = m_scale.RealToPixel(wxRealPoint(myRealExt.x_max, myRealExt.y_max));
	pts[3] = m_scale.RealToPixel(wxRealPoint(myRealExt.x_min, myRealExt.y_max));
	pts[4] = m_scale.RealToPixel(wxRealPoint(myRealExt.x_min, myRealExt.y_min));
	
	temp_dc.DrawLines(5, pts);
	
	
	wxBitmap nullbmp;
	temp_dc.SelectObject(nullbmp);
	
	return TRUE;
}



bool tmDrawer::Draw (tmLayerProperties * itemProp, tmGISData * pdata)
{
	switch (itemProp->m_LayerSpatialType)
	{
		case LAYER_SPATIAL_LINE:
			DrawLines(itemProp, pdata);
			break;
		case LAYER_SPATIAL_POINT:
			DrawPoints(itemProp, pdata);
			break;
		case LAYER_SPATIAL_POLYGON:
			DrawPolygons(itemProp, pdata);
			break;
		case LAYER_SPATIAL_RASTER:
			DrawRaster(itemProp, pdata);
			break;
		default:
			fprintf(stderr, "%s line %d : Error - no drawer found for one object\n ",
					__FUNCTION__, __LINE__);
			return FALSE;
			break;
	}
	
	return TRUE;	
}



/***************************************************************************//**
 @brief Draw all lines
 @details This  function uses the symbology (#tmSymbol) and the GIS data
 (#tmGISData) for drawing all lines into the bitmap.
 @param itemProp contain all informations about the layer properties.
 @param pdata A valid object of tmGISData type
 @return  false if it wasen't able to define a spatial filter see :
 tmGISDataVectorMYSQL::SetSpatialFilter
 @author Lucien Schreiber (c) CREALP 2008
 @date 10 September 2008
 *******************************************************************************/
bool tmDrawer::DrawLines(tmLayerProperties * itemProp, tmGISData * pdata)
{
	wxMemoryDC temp_dc;
	temp_dc.SelectObject(*m_bmp);
	wxGraphicsContext* pgdc = wxGraphicsContext::Create( temp_dc);
	
	// create pen based on symbology
	tmSymbolVectorLine * pSymbol = (tmSymbolVectorLine*) itemProp->m_LayerSymbol;
	wxPen myPen (pSymbol->GetColour(),pSymbol->GetWidth(), pSymbol->GetShape());
	pgdc->SetPen(myPen);
	
	// define spatial filter
	tmGISDataVector * pVectLine = (tmGISDataVector*) pdata;
	if(!pVectLine->SetSpatialFilter(m_spatFilter,itemProp->m_LayerType))
	{
		wxLogDebug(_T("Error setting spatial filter"));
		return false;
	}
	
	// iterate for all lines, will not work on a threaded version
	// because of all wxLogDebug commands
	int iNbVertex = 0;
	bool bReturn = true;
	int iLoop = 0;
	while (1)
	{
		iNbVertex = 0;
		wxRealPoint * pptsReal = pVectLine->GetNextDataLine(iNbVertex);
		
		// line must have more than one vertex
		if (iNbVertex <= 1) 
		{
			wxLogDebug(_T("No vertex returned @loop = %d"),iLoop);
			bReturn = false;
			break;
		}
		
		
		// creating path
		wxGraphicsPath myPath = pgdc->CreatePath();
		myPath.MoveToPoint(m_scale.RealToPixel(pptsReal[0]));
		for (int i = 1; i< iNbVertex; i++)
			myPath.AddLineToPoint(m_scale.RealToPixel(pptsReal[i]));
		
		pgdc->StrokePath(myPath);
				
		delete [] pptsReal;
		iLoop++;
		
	}
	
	
	wxLogDebug(_T("%d Lines drawn"), iLoop);
	
	
	temp_dc.SelectObject(wxNullBitmap);
	
	return bReturn;
}



/***************************************************************************//**
 @brief Draw all points
 @details This functions draw all points using the new wxGraphicsContext
 engine.
 @note library must be compiled with options according to that.
 @param itemProp contain all informations about the layer's properties such as
 the symbology
 @param pdata pointer to the GIS data (see wxGISData) and derived.
 @return  false if not able to draw all points
 @author Lucien Schreiber (c) CREALP 2008
 @date 16 September 2008
 *******************************************************************************/
bool tmDrawer::DrawPoints (tmLayerProperties * itemProp, tmGISData * pdata)
{
	wxMemoryDC dc;
	dc.SelectObject(*m_bmp);
	wxGraphicsContext* pgdc = wxGraphicsContext::Create( dc); 

	// create pen based on symbology
	tmSymbolVectorPoint * pSymbol = (tmSymbolVectorPoint*) itemProp->m_LayerSymbol;
	wxPen myPen (pSymbol->GetColour(),pSymbol->GetRadius());
	pgdc->SetPen(myPen);
	
	// define spatial filter
	tmGISDataVector * pVectPoint = (tmGISDataVector*) pdata;
	if(!pVectPoint->SetSpatialFilter(m_spatFilter,itemProp->m_LayerType))
	{
		wxLogDebug(_T("Error setting spatial filter"));
		return false;
	}
	
	// iterate for all points, will not work on a threaded version
	// because of all wxLogDebug commands
	bool bReturn = true;
	int iLoop = 0;
	wxPoint Intpts (0,0);
	
	while (1)
	{
		wxRealPoint * pptsReal = pVectPoint->GetNextDataPoint();
		
		if(pptsReal == NULL)
		{
			wxLogDebug(_T("No point returned @loop : %d"), iLoop);
			bReturn = FALSE;
			break;
		}
		
		// convert from real coordinates to screen coordinates
		Intpts = m_scale.RealToPixel(*pptsReal);
	
#ifdef __WXMSW__
		pgdc->StrokeLine (Intpts.x, Intpts.y, Intpts.x + 0.1, Intpts.y + 0.1);
#else
		pgdc->StrokeLine (Intpts.x, Intpts.y, Intpts.x, Intpts.y);
#endif
		
		delete pptsReal;
		iLoop++;
	}
	
	
	wxLogDebug(_T("%d Points drawn"), iLoop);
	
	dc.SelectObject(wxNullBitmap);
	
	return TRUE;
}




bool tmDrawer::DrawPolygons (tmLayerProperties * itemProp, tmGISData * pdata)
{
	// variables
	wxMemoryDC dc;
	bool bReturn = true;
	int iNbVertex = 0;
	int iLoop = 0;
	int i=0;
	
	// device context for drawing
	dc.SelectObject(*m_bmp);
	wxGraphicsContext* pgdc = wxGraphicsContext::Create( dc); 
	
	
	// create pen and brush based on symbology
	tmSymbolVectorPolygon * pSymbol = (tmSymbolVectorPolygon*) itemProp->m_LayerSymbol;
	wxPen myPen (pSymbol->GetBorderColour(), pSymbol->GetBorderWidth());
	wxBrush myBrush (pSymbol->GetFillColour(),pSymbol->GetFillStyle());
	pgdc->SetPen(myPen);
	pgdc->SetBrush(myBrush);
	
		
	// define spatial filter
	tmGISDataVector * pVectPoly = (tmGISDataVector*) pdata;
	if(!pVectPoly->SetSpatialFilter(m_spatFilter,itemProp->m_LayerType))
	{
		wxLogDebug(_T("Error setting spatial filter"));
		return false;
	}
	
	 //loop all features 
	while (1)
	{
		// get polygons info
		int iPolyRings = pVectPoly->GetNextDataPolygonInfo();
		if (iPolyRings <= 0)
		{
			wxLogDebug(_T("Error getting info about polygons, return value is : %d"), iPolyRings);
			break;
		}
		
		//TODO: Temp code, for debuging remove after
		if (iPolyRings > 1)
		{
			wxLogDebug(_T("Polygon : %d contain : %d rings"),iLoop, iPolyRings);
		}
			
		wxGraphicsPath myPolygonPath = pgdc->CreatePath();
		// get polygons data, loop all rings into polygons
		for (i = 0; i<iPolyRings; i++)
		{
			wxRealPoint * pptsReal = pVectPoly->GetNextDataPolygon(i, iNbVertex);
			
			if(pptsReal == NULL)
			{
				wxLogDebug(_T("No point returned @polygon: %d @loop : %d"), iLoop, i);
				bReturn = FALSE;
				break;
			}
			
			// creating path based on ring data and putting this path
			// into main path
			wxGraphicsPath myPath = pgdc->CreatePath();
			myPath.MoveToPoint(m_scale.RealToPixel(pptsReal[0]));
			for (int i = 1; i< iNbVertex; i++)
				myPath.AddLineToPoint(m_scale.RealToPixel(pptsReal[i]));
			
			myPath.CloseSubpath();
			myPolygonPath.AddPath(myPath);
			delete [] pptsReal;
		}
		
		
		pgdc->DrawPath(myPolygonPath);
	
		
		iLoop ++;
		
	}
	
	/*const double base = 80.0;  // sizes used in shapes drawn below 
    const double base2 = base/2.0; 
    const double base4 = base/4.0; 
	
	wxGraphicsPath path = pgdc->CreatePath();
	path.AddCircle(0, 0, base2); 
    path.MoveToPoint(0, -base2); 
    path.AddLineToPoint(0, base2); 
    path.MoveToPoint(-base2, 0); 
    path.AddLineToPoint(base2, 0); 
    path.CloseSubpath(); 
    path.AddRectangle(-base4, -base4/2, base2, base4); 
	pgdc->Translate(2*base, 2*base);
	
	//pgdc->StrokePath(path); 
	//
	//pgdc->StrokePath(path);
	//pgdc->FillPath(path);
	pgdc->DrawPath(path);*/
	
	wxLogDebug(_T("%d Polygons drawn"), iLoop);
	
	dc.SelectObject(wxNullBitmap);
	
	
	return bReturn;
}



bool tmDrawer::DrawRaster (tmLayerProperties * itemProp, tmGISData * pdata)
{
	
	tmGISDataRaster * pRaster = (tmGISDataRaster*) pdata;
	if(!pRaster->SetSpatialFilter(m_spatFilter, itemProp->m_LayerType))
	{
		return false;
	}
	
	// check if we need to draw raster (inside visible area)
	if (!pRaster->IsImageInsideVisibleArea())
	{
		return false;
	}
	
	/*// load image using GDAL
	// owned by image, do not destroy manually.
	unsigned char      *imgbuf = NULL;
	unsigned int        imglen = 0;
	unsigned char      *maskbuf = NULL;
	unsigned int        masklen= 0;*/
	bool bReturn =		true;
	
	// converting image coordinate & clipping 
	tmRealRect myClippedCoordReal = pRaster->GetImageClipedCoordinates();
	wxPoint topleftpx = m_scale.RealToPixel(wxRealPoint(myClippedCoordReal.x_min,
														myClippedCoordReal.y_min));
	wxPoint bottomright = m_scale.RealToPixel(wxRealPoint(myClippedCoordReal.x_max,
														  myClippedCoordReal.y_max));
	wxRect myClippedCoordPx (topleftpx, bottomright);
	wxImage * myRaster = new wxImage (myClippedCoordPx.GetWidth(), myClippedCoordPx.GetHeight(), true);

		
	if (pRaster->GetImageData(&imgbuf, &imglen, &maskbuf, &masklen, 
							  wxSize(myClippedCoordPx.GetWidth(),myClippedCoordPx.GetHeight())) != CE_None)
	{
		fprintf(stderr, "%s line %d : Error loading image data \n ", __FUNCTION__, __LINE__);
		// need cleanup
		// in all case, clean data
		if (imgbuf)
			delete imgbuf;
		if (maskbuf)
			delete maskbuf;
		return false;
	}
	
	// data loaded successfully, draw image on display now
	// device context for drawing
	wxMemoryDC dc;
	dc.SelectObject(*m_bmp);
	//wxGraphicsContext* pgdc = wxGraphicsContext::Create( dc); 
	
	
	
	myRaster->SetData(imgbuf);

	dc.DrawBitmap(*myRaster, wxPoint(myClippedCoordPx.GetX(), myClippedCoordPx.GetY()),true);
	dc.SelectObject(wxNullBitmap);

	// destroying bitmap lead to a crash under windows
	//myRaster->Destroy();
	
	return TRUE;	
}

