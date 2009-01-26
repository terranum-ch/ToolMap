/***************************************************************************
								tmrenderer.h
                    Deals with the main renderer windows
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


#ifndef _TM_GISRENDERER_H_
#define _TM_GISRENDERER_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <wx/scrolwin.h>	// for scrolled window base
#include "../gui/wxrubberband.h"	// for selection rubber band
#include "tmgisscale.h"			// for number of division;


// EVENT FOR GIS RENDERER CTRL
DECLARE_EVENT_TYPE(tmEVT_LM_SIZE_CHANGED, -1)
DECLARE_EVENT_TYPE(tmEVT_LM_MOUSE_MOVED , -1)
DECLARE_EVENT_TYPE(tmEVT_LM_ZOOM_RECTANGLE_OUT,-1)
DECLARE_EVENT_TYPE(tmEVT_LM_ZOOM_RECTANGLE_IN,-1)
DECLARE_EVENT_TYPE(tmEVT_LM_PAN_ENDED,-1)
DECLARE_EVENT_TYPE(tmEVT_LM_SELECTION, -1)


enum tmGIS_TOOL
{
	tmTOOL_SELECT = 0,
	tmTOOL_ZOOM_RECTANGLE_IN,
	tmTOOL_ZOOM_RECTANGLE_OUT,
	tmTOOL_PAN,
	tmTOOL_DRAW,
	tmTOOL_MODIFY,
	tmTOOL_ZOOM_RECTANGLE = tmTOOL_ZOOM_RECTANGLE_IN
};

enum tmGIS_CURSOR
{
	tmCURSOR_ZOOM_IN,
	tmCURSOR_ZOOM_OUT,
	tmCURSOR_HAND
};

// parameter : size of selection in pixels
// sould be able to divide it by two
const int tmSELECTION_DIAMETER = 4;


class tmRenderer : public wxScrolledWindow
	{
	private:
		wxBitmap * m_bmp;
		tmGIS_TOOL m_ActualTool;
		tmGIS_CURSOR m_ActualNotStockCursor;
				
		// rubber band
		wxRubberBand * m_SelectRect;
		wxPoint m_StartCoord;
		wxBitmap * m_PanBmp;
		
		// status of shift key
		bool m_ShiftDown;
		
		// snapping
		int m_SnappingRadius;
		
		// changing cursors
		wxCursor LoadCursorFromBitmap (tmGIS_CURSOR cursor);
		void ChangeCursor (const tmGIS_TOOL & selected_tool);
		
		// mouse event function
		void OnMouseDown	(wxMouseEvent & event);
		void OnMouseMove	(wxMouseEvent & event);
		void OnMouseUp		(wxMouseEvent & event);	
		//void OnMouseWheel	(wxMouseEvent & event);
		void OnShiftDown	(wxKeyEvent & event);
		void OnShiftUp		(wxKeyEvent & event);
		
			
		// scrollbar event (received)
		//void OnScroll (wxScrollWinEvent & event);
			
		DECLARE_EVENT_TABLE()
	protected:
		
		// rubber band functions
		void RubberBandStart (const wxPoint & mousepos);
		void RubberBandUpdate(const wxPoint & mousepos);
		void RubberBandStop();
		
		// selecting function
		void SelectStart (const wxPoint & mousepos);
		void SelectUpdate (const wxPoint & mousepos);
		void SelectStop (const wxPoint & mousepos);
		
		// panning function
		void PanStart (const wxPoint & mousepos);
		void PanUpdate (const wxPoint & mousepos);
		void PanStop (const wxPoint & mousepos);
		
		// drawing functions
		void DrawStart (const wxPoint & mousepos);
		
	
	
	public:
		tmRenderer(wxWindow * parent, wxWindowID id);
		~tmRenderer();
		
		void OnSizeChange(wxSizeEvent & event);
		
		void OnPaint(wxPaintEvent & event);
		
		void SetBitmapStatus(wxBitmap * bmp = NULL){m_bmp = bmp;}
		
		void SetTool (tmGIS_TOOL selected_tool);
		
		// edit snapping event
		void ToogleSnapping (int snapradius);

		
	};




#endif
