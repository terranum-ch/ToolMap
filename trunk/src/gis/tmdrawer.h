/***************************************************************************
								tmdrawer.h
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


#ifndef _TM_DRAWER_H_
#define _TM_DRAWER_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "tmgisscale.h"
#include "tmlayerproperties.h"
#include "tmgisdata.h"

#include "tmsymbolvectorline.h"
#include "tmsymbolvectorpoint.h"
#include "tmsymbolvectorpolygon.h"
#include "tmgisdatavector.h"
#include "tmgisdataraster.h"
#include "tmselecteddatamemory.h"	// for stroing selected data into memory
#include "tmrenderer.h"				// for drawing directly (edition mode)

//#include "../database/database_tm.h"




/***************************************************************************//**
 @brief In charge of all the drawing into bitmaps
 @author Lucien Schreiber (c) CREALP 2008
 @date 24 July 2008
 *******************************************************************************/
class tmDrawer : public wxObject 
	{
	private:
		wxBitmap * m_bmp;
		tmGISScale m_scale;
		bool m_IsInitialised;
		tmRealRect m_spatFilter;
		static bool m_LogOn;
		long m_ActuallayerID;
		tmSelectedDataMemory * m_SelMem;
		
		// load image using GDAL
		// owned by image, do not destroy manually.
		unsigned char      *imgbuf;
		unsigned int        imglen;
		unsigned char      *maskbuf;
		unsigned int        masklen;

		
		// drawing functions
		bool DrawLines(tmLayerProperties * itemProp, tmGISData * pdata);
		bool DrawPoints (tmLayerProperties * itemProp, tmGISData * pdata);
		bool DrawPolygons (tmLayerProperties * itemProp, tmGISData * pdata);
		bool DrawRaster (tmLayerProperties * itemProp, tmGISData * pdata);
		
		// drawing vertex
		bool DrawVertexLine (wxGraphicsContext* pgdc, wxRealPoint * pts, int nb_pts,
						 tmLayerProperties * itemProp, wxPen * pen, int nb_pen = 1);
		bool DrawVertexPoly (tmLayerProperties * itemProp, tmGISData * pdata);
		
		wxPen * CreateVertexUniquePen (tmLayerProperties * itemProp, int size);
		wxPen * CreateEditUniqueVertexPen (int size);
		wxPen * CreateEditUniqueSegmentPen (int size);
		
		static void DrawPoint (const wxPoint & pt, wxMemoryDC * pMdc);
		static void DrawPoint (const wxPoint & pt, wxClientDC * pcdc);
		
	protected:
	public:
		tmDrawer();
		~tmDrawer();
		void SetSelectedData (tmSelectedDataMemory * selectedmemory){m_SelMem = selectedmemory;}
		
		void InitDrawer (wxBitmap * bitmap, tmGISScale & scale, 
						 const tmRealRect & filter);
		
		// Extent Drawing
		bool DrawExtentIntoBitmap (int width = 1, const wxColour & col = wxColour(*wxBLACK));
		bool Draw (tmLayerProperties * itemProp, tmGISData * pdata);
		
		static void EnableLogging (bool enable) {m_LogOn = enable;}
		static bool IsLoggingEnabled () {return m_LogOn;}
		
		void DrawEditVertex (const wxRealPoint & pt,int size, wxColour colour);
		void DrawEditSegment (const wxRealPoint & pt1,
							  const wxRealPoint & pt2,
							  int size);
		void DrawEditLine (const wxArrayRealPoints & pts, int size, wxColour colour = *wxRED);
		
		
	};






#endif
