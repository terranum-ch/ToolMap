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
 @details Draw a red rectangle corresponding to the maximum extent of all
 visible layers
 @param bitmap pointer to a valid bitmap
 @param scale actual values of scale and zoom
 @return  TRUE if bitmap drawn, FALSE if bitmap not valid
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
	bool bSkip = false;
	int iLoop = 0;
	while (1)
	{
		iNbVertex = 0;
		wxRealPoint * pptsReal = pVectLine->GetNextDataLine(iNbVertex);
		
		// line must have more than one vertex... we
		// could try to skip invalid line once
		if (iNbVertex <= 1 && bSkip)
		{
			wxLogDebug(_T("No vertex returned @loop = %d"),iLoop);
			bReturn = false;
			break;
		}
		
		if(iNbVertex <= 1)
		{
			wxLogDebug(_T("Trying to skip line : %d"),iLoop);
			bSkip = true;
		}
		else
		{
			bSkip = false;
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
		
	}
	
	
	wxLogDebug(_T("%d Lines drawn"), iLoop);
	
	wxBitmap nullbmp;
	temp_dc.SelectObject(nullbmp);
	
	return TRUE;
}
