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


bool tmDrawer::m_LogOn = true;



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
	m_ActuallayerID = 0;
	m_SelMem = NULL;
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
		if (IsLoggingEnabled())
			wxLogDebug(_T("%s line %d : Error initing drawer \n"));
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
	m_ActuallayerID = itemProp->m_LayerID;
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
			DrawVertexPoly(itemProp, pdata);
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
	
	// pen for selection
	wxPen mySPen (m_SelMem->GetSelectionColour(), pSymbol->GetWidth());
	
	// pen for vertex
	wxPen * myVPen = CreateVertexUniquePen(itemProp, pSymbol->GetWidth());
	
	// define spatial filter
	tmGISDataVector * pVectLine = (tmGISDataVector*) pdata;
	if(!pVectLine->SetSpatialFilter(m_spatFilter,itemProp->m_LayerType))
	{
		if (IsLoggingEnabled())
			wxLogDebug(_T("Error setting spatial filter"));
		return false;
	}
	
	// iterate for all lines, will not work on a threaded version
	// because of all wxLogDebug commands
	int iNbVertex = 0;
	bool bReturn = true;
	bool bSelected = false;
	int iLoop = 0;
	while (1)
	{
		pgdc->SetPen(myPen);
		
		iNbVertex = 0;
		long myOid = 0;
		wxRealPoint * pptsReal = pVectLine->GetNextDataLine(iNbVertex, myOid);
		
		// line must have more than one vertex
		if (iNbVertex <= 1) 
		{
			if (IsLoggingEnabled())
				wxLogDebug(_T("No vertex returned @loop = %d"),iLoop);
			bReturn = false;
			break;
		}
		
		if (m_ActuallayerID == m_SelMem->GetSelectedLayer())
			if (m_SelMem->IsSelected(myOid))
				pgdc->SetPen(mySPen);
		
		// creating path
		wxGraphicsPath myPath = pgdc->CreatePath();
		myPath.MoveToPoint(m_scale.RealToPixel(pptsReal[0]));
		for (int i = 1; i< iNbVertex; i++)
			myPath.AddLineToPoint(m_scale.RealToPixel(pptsReal[i]));
		
		pgdc->StrokePath(myPath);
		
		// drawing vertex
		DrawVertexLine(pgdc, pptsReal, iNbVertex, itemProp, myVPen);
				
		delete [] pptsReal;
		iLoop++;
		
	}
	
	if (IsLoggingEnabled())
		wxLogDebug(_T("%d Lines drawn"), iLoop);
	
	
	temp_dc.SelectObject(wxNullBitmap);
	delete myVPen;
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
	wxPen mySPen (m_SelMem->GetSelectionColour(), pSymbol->GetRadius());
	pgdc->SetPen(myPen);
	
	// define spatial filter
	tmGISDataVector * pVectPoint = (tmGISDataVector*) pdata;
	if(!pVectPoint->SetSpatialFilter(m_spatFilter,itemProp->m_LayerType))
	{
		if (IsLoggingEnabled())
			wxLogDebug(_T("Error setting spatial filter"));
		return false;
	}
	
	// iterate for all points, will not work on a threaded version
	// because of all wxLogDebug commands
	bool bReturn = true;
	int iLoop = 0;
	long myOid = 0;
	wxPoint Intpts (0,0);
	bool bAsSelection = false;
	
	while (1)
	{
		wxRealPoint * pptsReal = pVectPoint->GetNextDataPoint(myOid);
		
		if(pptsReal == NULL)
		{
			if (IsLoggingEnabled())
				wxLogDebug(_T("No point returned @loop : %d"), iLoop);
			bReturn = FALSE;
			break;
		}
		
		// changing pen for selected data
		if (m_ActuallayerID == m_SelMem->GetSelectedLayer())
			if (m_SelMem->IsSelected(myOid))
			{
				pgdc->SetPen(mySPen);
				bAsSelection = true;
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
		
		// returning to basic pen
		if (bAsSelection)
		{
			bAsSelection = false;
			pgdc->SetPen(myPen);
		}
		
	}
	
	if (IsLoggingEnabled())
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
	
	wxPen mySPen (m_SelMem->GetSelectionColour(), pSymbol->GetBorderWidth());
	wxBrush mySBrush (m_SelMem->GetSelectionColour2(), pSymbol->GetFillStyle());
	
	pgdc->SetBrush(myBrush);
	pgdc->SetPen(myPen);
		
	// define spatial filter
	tmGISDataVector * pVectPoly = (tmGISDataVector*) pdata;
	if(!pVectPoly->SetSpatialFilter(m_spatFilter,itemProp->m_LayerType))
	{
		if (IsLoggingEnabled())
			wxLogDebug(_T("Error setting spatial filter"));
		return false;
	}
	
	long myOid = -1;
	bool bAsSelection = false;
	 //loop all features 
	while (1)
	{
		// get polygons info
		int iPolyRings = pVectPoly->GetNextDataPolygonInfo(myOid);
		if (iPolyRings <= 0)
		{
			if (IsLoggingEnabled())
				wxLogDebug(_T("Error getting info about polygons, return value is : %d"), iPolyRings);
			break;
		}
		
		//TODO: Temp code, for debuging remove after
		if (iPolyRings > 1)
		{
			if (IsLoggingEnabled())
				wxLogDebug(_T("Polygon : %d contain : %d rings"),iLoop, iPolyRings);
		}
			
		wxGraphicsPath myPolygonPath = pgdc->CreatePath();
		// get polygons data, loop all rings into polygons
		for (i = 0; i<iPolyRings; i++)
		{
			wxRealPoint * pptsReal = pVectPoly->GetNextDataPolygon(i, iNbVertex);
			
			if(pptsReal == NULL)
			{
				if (IsLoggingEnabled())
					wxLogDebug(_T("No point returned @polygon: %d @loop : %d"), iLoop, i);
				bReturn = FALSE;
				break;
			}
			
			// changing pen for selected data
			if (m_ActuallayerID == m_SelMem->GetSelectedLayer())
				if (m_SelMem->IsSelected(myOid))
				{
					pgdc->SetPen(mySPen);
					pgdc->SetBrush(mySBrush);
					bAsSelection = true;
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
		
		// returning to basic pen
		if (bAsSelection)
		{
			pgdc->SetPen(myPen);
			pgdc->SetBrush(myBrush);
			bAsSelection = false;
		}
		
	}

	if (IsLoggingEnabled())
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



/***************************************************************************//**
 @brief Draw the vertex for lines and polygons
 @details If required by options, this function draw a dot for each vertex of
 the line or polygons
 @param pgdc pointer to a valid wxGraphicsContext (no check done)
 @param pts pointer to a list of nb_pts points. This function dosen't take owner
 of the list. Il must be destroyed elsewhere
 @param nb_pts number of points stored in pts
 @param pen Adress of a pen or pen array see also nb_pen
 @param nb_pen Number of pen stored into pen. May be used for drawing vertex in
 with different pen (not implemented now)
 @return true if no exception encoutered
 @param itemProp Properties of item.
 @author Lucien Schreiber (c) CREALP 2008
 @date 20 October 2008
 *******************************************************************************/
bool tmDrawer::DrawVertexLine (wxGraphicsContext* pgdc, wxRealPoint * pts, int nb_pts,
						   tmLayerProperties * itemProp, wxPen * pen, int nb_pen)
{
	wxPoint Intpts (0,0);
	int i = 0;
	
	if (nb_pen == 1) // only one pen
		pgdc->SetPen(*pen);
	
	
	switch (itemProp->m_DrawFlags)
	{
		case tmDRAW_VERTEX_ALL: // ALL VERTEX
			for (i = 0;i<nb_pts; i++)
			{
				// convert from real coordinates to screen coordinates
				Intpts = m_scale.RealToPixel(pts[i]);
				
			#ifdef __WXMSW__
				pgdc->StrokeLine (Intpts.x, Intpts.y, Intpts.x + 0.1, Intpts.y + 0.1);
			#else
				pgdc->StrokeLine (Intpts.x, Intpts.y, Intpts.x, Intpts.y);
			#endif
			}
			break;
			
		case tmDRAW_VERTEX_BEGIN_END: // ONLY BEGIN / END
			for (i = 0; i< nb_pts; i = i+nb_pts-1)
			{
				Intpts = m_scale.RealToPixel(pts[i]);
				#ifdef __WXMSW__
				pgdc->StrokeLine (Intpts.x, Intpts.y, Intpts.x + 0.1, Intpts.y + 0.1);
				#else
				pgdc->StrokeLine (Intpts.x, Intpts.y, Intpts.x, Intpts.y);
				#endif
			}
			break;
			
		default:
			break;
	}
	return true;
}



/***************************************************************************//**
 @brief Draw vertex for polygons
 @details Because polygons may hide points if we draw them in the same loop, we
 must draw vertex in a second loop to ensure they are displayed properly
 @param itemProp Item's properties
 @param pdata Item's data
 @return  true if drawing was successfull, false otherwise
 @author Lucien Schreiber (c) CREALP 2008
 @date 21 October 2008
 *******************************************************************************/
bool tmDrawer::DrawVertexPoly (tmLayerProperties * itemProp, tmGISData * pdata)
{
	// variables
	wxMemoryDC dc;
	bool bReturn = true;
	int iNbVertex = 0;
	int iLoop = 0;
	int i=0;
	
	// check if this function must run
	if (itemProp->m_DrawFlags == tmDRAW_VERTEX_NONE)
		return false;
	
	// device context for drawing
	dc.SelectObject(*m_bmp);
	wxGraphicsContext* pgdc = wxGraphicsContext::Create( dc); 
	
	
	// create pen for vertex
	tmSymbolVectorPolygon * pSymbol = (tmSymbolVectorPolygon*) itemProp->m_LayerSymbol;
	wxPen * myVPen = CreateVertexUniquePen(itemProp, pSymbol->GetBorderWidth());
	
	// define spatial filter
	tmGISDataVector * pVectPoly = (tmGISDataVector*) pdata;
	if(!pVectPoly->SetSpatialFilter(m_spatFilter,itemProp->m_LayerType))
	{
		if (IsLoggingEnabled())
			wxLogDebug(_T("Error setting spatial filter"));
		return false;
	}
	
	long myOid = -1;
	//loop all features 
	while (1)
	{
		// get polygons info
		int iPolyRings = pVectPoly->GetNextDataPolygonInfo(myOid);
		if (iPolyRings <= 0)
		{
			if (IsLoggingEnabled())
				wxLogDebug(_T("Error getting info about polygons, return value is : %d"), iPolyRings);
			break;
		}
		
		if (iPolyRings > 1)
		{
			if (IsLoggingEnabled())
				wxLogDebug(_T("Polygon : %d contain : %d rings"),iLoop, iPolyRings);
		}
		
		// get polygons data, loop all rings into polygons
		for (i = 0; i<iPolyRings; i++)
		{
			wxRealPoint * pptsReal = pVectPoly->GetNextDataPolygon(i, iNbVertex);
			
			if(pptsReal == NULL)
			{
				if (IsLoggingEnabled())
					wxLogDebug(_T("No point returned @polygon: %d @loop : %d"), iLoop, i);
				bReturn = FALSE;
				break;
			}
			
			// drawing vertex
			DrawVertexLine(pgdc, pptsReal, iNbVertex, itemProp, myVPen);
			
			
			delete [] pptsReal;
		}
		iLoop ++;
		
	}
	delete myVPen;
	dc.SelectObject(wxNullBitmap);
	return bReturn;
	
}



/***************************************************************************//**
 @brief Create a pen for drawin vertex
 @details This create an unique pen of color wxBLACK and where size is two time
 the line's size.
 @param itemProp Properties of item
 @param size This is half the size of the vertex (vertex radius)
 @return  Pointer to a wxPen. User must take care of destroying the pen after
 usage
 @author Lucien Schreiber (c) CREALP 2008
 @date 21 October 2008
 *******************************************************************************/
wxPen * tmDrawer::CreateVertexUniquePen (tmLayerProperties * itemProp, int size)
{
	wxPen * myPen = new wxPen (*wxBLACK, size*2);
	return myPen;
}



/***************************************************************************//**
 @brief Create a pen for drawing editing vertex
 @author Lucien Schreiber (c) CREALP 2009
 @date 03 February 2009
 *******************************************************************************/
wxPen * tmDrawer::CreateEditUniqueVertexPen (int size)
{
	wxPen * myPen = new wxPen (*wxBLACK, size);
	return myPen;
}



/***************************************************************************//**
 @brief Create a pen for drawing editing segment
 @author Lucien Schreiber (c) CREALP 2009
 @date 03 February 2009
 *******************************************************************************/
wxPen * tmDrawer::CreateEditUniqueSegmentPen (int size)
{
	wxPen * myPen = new wxPen (*wxRED, size);
	return myPen;
}


/***************************************************************************//**
 @brief Draw a vertex (used during editing)
 @param pt a point (real coordinates)
 @param size the size of the vertex to draw
 @param colour the wxColour to use for drawing vertex 
 @author Lucien Schreiber (c) CREALP 2009
 @date 03 February 2009
 *******************************************************************************/
void tmDrawer::DrawEditVertex (const wxRealPoint & pt,int size, wxColour colour)
{
	wxPoint myScreenPt = m_scale.RealToPixel(pt);
	wxMemoryDC mdc;
	mdc.SelectObject(*m_bmp);
	wxPen * myVPen = new wxPen(colour, size);
	mdc.SetPen(*myVPen);
	DrawPoint(myScreenPt, &mdc);	
	delete myVPen;
	mdc.SelectObject(wxNullBitmap);
}




/***************************************************************************//**
 @brief Draw the segment between two points
 @param pt1 first point (Real)
 @param pt2 second point (Real)
 @param size size of the segment
 @author Lucien Schreiber (c) CREALP 2009
 @date 03 February 2009
 *******************************************************************************/
void tmDrawer::DrawEditSegment (const wxRealPoint & pt1,
								const wxRealPoint & pt2,
								int size)
{
	wxPoint myPt1(0,0), myPt2(0,0);
	wxPen * mySegmentPen = CreateEditUniqueSegmentPen(size);
	wxPen * myVertexPen = CreateEditUniqueVertexPen(size);
	
	wxMemoryDC mdc;
	mdc.SelectObject(*m_bmp);

	myPt1 = m_scale.RealToPixel(pt1);
	
	// draw segment first, then vertex
	if (pt2 != wxRealPoint(0,0))
	{
		myPt2 = m_scale.RealToPixel(pt2);
		
		mdc.SetPen(*mySegmentPen);
		mdc.DrawLine(myPt1, myPt2);
		
		mdc.SetPen(*myVertexPen);
		DrawPoint(myPt2, &mdc);
	}
	
	
	if (pt1 != wxRealPoint(0,0))
	{
		DrawPoint(myPt1, &mdc);
	}
	
	
	// cleaning
	mdc.SelectObject(wxNullBitmap);
	delete myVertexPen;
	delete mySegmentPen;
}


/***************************************************************************//**
 @brief Draw a point
 @param pt a point value in screen coordinates
 @param pMdc a valid wxMemory object
 @author Lucien Schreiber (c) CREALP 2009
 @date 03 February 2009
 *******************************************************************************/
void tmDrawer::DrawPoint (const wxPoint & pt, wxMemoryDC * pMdc)
{
#ifdef __WXMSW__
	pMdc->DrawLine(pt.x, pt.y, pt.x + 0.1, pt.y + 0.1);
#else
	pMdc->DrawLine(pt.x, pt.y, pt.x, pt.y);
#endif
}


/***************************************************************************//**
 @brief Draw a point
 @param pt a point value in screen coordinates
 @param pcdc a valid wxClientDC object
 @author Lucien Schreiber (c) CREALP 2009
 @date 04 February 2009
 *******************************************************************************/
void tmDrawer::DrawPoint (const wxPoint & pt, wxClientDC * pcdc)
{
#ifdef __WXMSW__
	pcdc->DrawLine(pt.x, pt.y, pt.x + 0.1, pt.y + 0.1);
#else
	pcdc->DrawLine(pt.x, pt.y, pt.x, pt.y);
#endif
}



/***************************************************************************//**
 @brief Draw all edited lines
 @param pts Adress of a wxArrayRealPoints
 @param size Size of the line
 @param colour the Colour of the line to draw, default is RED
 @author Lucien Schreiber (c) CREALP 2009
 @date 04 February 2009
 *******************************************************************************/
void tmDrawer::DrawEditLine (const wxArrayRealPoints & pts, int size, wxColour colour)
{
	wxMemoryDC mdc;
	mdc.SelectObject(*m_bmp);
	
	wxPen * mySegPen = new wxPen(colour, size);
	wxPen * myVertexPen = CreateEditUniqueVertexPen(size);
	
	// convert real to screen
	int nb_pts = pts.GetCount();
	wxPoint * myPts = new wxPoint[nb_pts];
	for (int i = 0; i<nb_pts; i++)
	{
		myPts[i] = m_scale.RealToPixel(pts.Item(i));
	}
	
	// draw segments
	mdc.SetPen(*mySegPen);
	mdc.DrawLines(nb_pts, myPts);
	
	// Draw Vertex
	mdc.SetPen(*myVertexPen);
	for (int j = 0; j< nb_pts; j++)
	{
		DrawPoint(myPts[j], &mdc);		
	}
	
	delete [] myPts;
	delete mySegPen;
	delete myVertexPen;
	
}




