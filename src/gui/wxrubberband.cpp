/***************************************************************************
							wxrubberband.cpp
						Definition of a Rubber Band class 
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

/***************************************************************************//**
 @file wxrubberband.cpp
 @brief Declaration of a Rubber Band class
 @details This is mainly used for drawing a selection box arround
 something
 @version 1.1
 @author Lucien Schreiber (c) CREALP 2008
 @date 25 July 2008
 *******************************************************************************/


#include "wxrubberband.h"



// default constructor
wxRubberBand::wxRubberBand()
{
	RubberInitValues ();
}

// constructor
wxRubberBand::wxRubberBand(wxWindow *wnd)
{
	// we init all internal values
	RubberInitValues ();
	// we store the wnd 
	theWnd = wnd;	
}

// destructor
wxRubberBand::~wxRubberBand()
{
	// do nothing for the moment
}

void wxRubberBand::RubberInitValues()
{
	
	// init member data
	theWnd = NULL;
	theOldRubberRect = wxRect(0,0,0,0);
	m_OldPosStart = wxPoint(-1,-1);
	m_OldPosEnd = wxPoint (-1,-1);
	SetPen ();
	
}

void wxRubberBand::SetGeometry(int x, int y, int width, int height)
{
	
		wxClientDC MyDC(theWnd);
		MyDC.SetLogicalFunction (wxINVERT);
		// draw lines
		MyDC.SetPen (thePen);
		if (VerifyRubberLines(x,y,width,height)) // must be not null see documentation
			DrawRectangleLines (&MyDC,x,y,width,height);
}



void wxRubberBand::SetGeometry (const wxPoint & posStart, const  wxPoint & posEnd)
{
	wxClientDC MyDC(theWnd);
	// for errasing pens by drawing in inverse colors
	MyDC.SetLogicalFunction (wxINVERT);
	MyDC.SetPen (thePen);
	
	// clean old rubber band
	EraseLastRubber(&MyDC);	
	
	// draw new rubber band
	wxPoint pts[5] = 
	{
		posStart,
		wxPoint (posStart.x, posEnd.y),
		posEnd,
		wxPoint(posEnd.x, posStart.y),
		posStart
	};
	MyDC.DrawLines(5, pts, 0, 0);
	
	// modify old rubber band
	m_OldPosStart = posStart;
	m_OldPosEnd = posEnd;
}


void wxRubberBand::DrawRectangleLines(wxDC * MyDC,int x, int y, int width, int height)
{
	// dessine new lines
	MyDC->DrawLine (x,y, x+width,y);				// top segment
	MyDC->DrawLine (x+width,y,x+width,y+height);	// right segment
	MyDC->DrawLine (x+width,y+height,x,y+height);	// bottom segment
	MyDC->DrawLine (x,y+height,x,y);				// left segment

	// Erase old line if theOldRubberRect != 0
	if (!theOldRubberRect.IsEmpty())
	{
	MyDC->DrawLine (theOldRubberRect.x,theOldRubberRect.y, 
		theOldRubberRect.x+theOldRubberRect.width,theOldRubberRect.y);	// top segment
	MyDC->DrawLine (theOldRubberRect.x+theOldRubberRect.width,theOldRubberRect.y,
		theOldRubberRect.x+theOldRubberRect.width,theOldRubberRect.y+theOldRubberRect.height);	// right segment
	MyDC->DrawLine (theOldRubberRect.x+theOldRubberRect.width,
		theOldRubberRect.y+theOldRubberRect.height,theOldRubberRect.x,
		theOldRubberRect.y+theOldRubberRect.height);	// bottom segment
	MyDC->DrawLine (theOldRubberRect.x,theOldRubberRect.y+theOldRubberRect.height,
		theOldRubberRect.x,theOldRubberRect.y);				// left segment
	}
	theOldRubberRect.x = x;
	theOldRubberRect.y = y;
	theOldRubberRect.width = width;
	theOldRubberRect.height = height;

}


void wxRubberBand::SetPen()
{
	wxPen pen(*wxBLACK,1,wxDOT);
	thePen = pen;
	
}

bool wxRubberBand::VerifyRubberLines(int x, int y, int width, int height)
{
	if (x < 0 || y < 0)
		return FALSE;
	if (width <= 0 || height <= 0)
		return FALSE;
	return TRUE;

}

void wxRubberBand::SetPen(wxPen &pen)
{
	thePen = pen;
}

void wxRubberBand::ClearOldRubberRect()
{
	// if no dc is passed, create our own dc
	wxClientDC MyDC(theWnd);
	// for errasing pens by drawing in inverse colors
	MyDC.SetLogicalFunction (wxINVERT);
	MyDC.SetPen (thePen);
		
	
	EraseLastRubber(&MyDC);
		
	m_OldPosStart = wxPoint(-1,-1);
	m_OldPosEnd = wxPoint (-1,-1);
	theOldRubberRect = wxRect (0,0,0,0);
}


void wxRubberBand::EraseLastRubber (wxDC * dc)
{
	// erase old rubber band if needed
	if (m_OldPosStart != wxPoint (-1,-1) && m_OldPosEnd != wxPoint(-1,-1))
	{
		wxPoint oldPts [5] = 
		{
			m_OldPosStart,
			wxPoint (m_OldPosStart.x, m_OldPosEnd.y),
			m_OldPosEnd,
			wxPoint (m_OldPosEnd.x, m_OldPosStart.y),
			m_OldPosStart
		};
		dc->DrawLines(5, oldPts, 0, 0);
	}
}



