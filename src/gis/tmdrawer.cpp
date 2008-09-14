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
		default:
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
	
	// create pen based on symbology
	tmSymbolVectorLine * pSymbol = (tmSymbolVectorLine*) itemProp->m_LayerSymbol;
	wxPen myPen (pSymbol->GetColour(),pSymbol->GetWidth(), pSymbol->GetShape());
	temp_dc.SetPen(myPen);
	temp_dc.SetBackground(*wxWHITE);
	
	
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
		
		wxPoint *pIntpts = new wxPoint[iNbVertex];
		for (int i = 0; i<iNbVertex; i++)
		{
			pIntpts[i] = m_scale.RealToPixel(pptsReal[i]);
		}
		
		
		temp_dc.DrawLines(iNbVertex, pIntpts);
		
		delete [] pptsReal;
		delete [] pIntpts;
		iLoop++;
		
	}
	
	
	wxLogDebug(_T("%d Lines drawn"), iLoop);
	
	
	temp_dc.SelectObject(wxNullBitmap);
	
	return TRUE;
}



bool tmDrawer::DrawPoints (tmLayerProperties * itemProp, tmGISData * pdata)
{
	//creating empty bmp;
	wxBitmap bitmap (m_bmp->GetWidth(), m_bmp->GetHeight());
	wxBitmap mask (m_bmp->GetWidth(),m_bmp->GetHeight());
	wxMemoryDC dc;
	dc.SelectObject(bitmap);
	
	// coloring bitmap in white (works with unix)
	dc.SetBackground(wxBrush(*wxWHITE_BRUSH));
	dc.Clear();
	//TODO: Put above code in the creating empty bitmap function 

	// red half-transparent
	wxBrush myBrush (wxColour(255,0,0,100));
	

	wxGraphicsContext* pgdc = wxGraphicsContext::Create( dc); 
	wxGraphicsBrush myGBrush =	pgdc->CreateBrush(myBrush);
	pgdc->SetBrush(myGBrush);
	pgdc->DrawRectangle(0, 0, 200, 200);
	
	//wxGCDC gcdc(dc); 
				   //wxGraphicsContext * gdc = new wxGraphicsContext();
				   
				   
/*	dc.SetBrush (wxBrush(wxColour(255,0,0,100)));
	dc.SetPen (wxPen(*wxWHITE_PEN));
	dc.DrawRectangle (50,50,m_bmp->GetWidth(), m_bmp->GetHeight());*/
	
				   
	dc.SelectObject(wxNullBitmap);
	
	
	
	wxMemoryDC temp_dc;
	temp_dc.SelectObject(*m_bmp);
	
	
	temp_dc.DrawBitmap(bitmap, 0, 0, TRUE);
	/*
	// create pen based on symbology
	tmSymbolVectorPoint * pSymbol = (tmSymbolVectorPoint*) itemProp->m_LayerSymbol;
	wxPen myPen (pSymbol->GetColour(),pSymbol->GetRadius());
	temp_dc.SetPen(myPen);
	temp_dc.SetBackground(*wxWHITE);
	
	
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
	
		
		temp_dc.DrawLine(Intpts, Intpts);
		
		delete pptsReal;
		iLoop++;
	}
	
	
	wxLogDebug(_T("%d Points drawn"), iLoop);*/
	
	temp_dc.SelectObject(wxNullBitmap);
	
	return TRUE;
}

