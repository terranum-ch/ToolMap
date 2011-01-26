/***************************************************************************
 vrrubberband.cpp
 
 -------------------
 copyright            : (C) 2010 CREALP Lucien Schreiber 
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

#include "vrrubberband.h"

vrRubberBand::vrRubberBand(wxWindow * window) {
	m_Window = window;
	m_PointStart = wxDefaultPosition;
	m_PointEnd = wxDefaultPosition;
	
}



vrRubberBand::~vrRubberBand() {
	// during unit testing, m_Window is allways null otherwise
	// m_Window should be valid
	if (m_Window == NULL) {
		wxLogError("Window object is NULL in rubberband (normal for unit testing)");
		return;
	}
	
	// brakets are needed to destroy wxClientDC before reseting
	// m_Overlay.
	{
		wxClientDC dc( m_Window );
		wxDCOverlay overlaydc( m_Overlay, &dc );
		overlaydc.Clear();
	}
	m_Overlay.Reset();
}



void vrRubberBand::SetPointFirst(const wxPoint & pt) {
	m_PointStart = pt;
}



void vrRubberBand::SetPointLast(const wxPoint & pt) {
	m_PointEnd = pt;
}



wxRect vrRubberBand::_CreateRect() {
	if (m_PointStart == wxDefaultPosition || 
		m_PointEnd == wxDefaultPosition) {
		wxLogError("Rubber-band not inited correctly");
		wxFAIL;
	}
	
	wxRect myRect;
	myRect.SetTopLeft(m_PointStart);
	myRect.SetBottomRight(m_PointEnd);
	return myRect;
}



wxRect vrRubberBand::GetRect() {
	wxRect myRect = _CreateRect();
	
	// transform the rect into a correct rect
	int myOldValue1 = 0;
	int myOldValue2 = 0;
	
	if (myRect.GetWidth() < 0) {
		myOldValue1 = myRect.GetRight();
		myOldValue2 = myRect.GetLeft();
		myRect.SetLeft(myOldValue1);
		myRect.SetRight(myOldValue2);
	}
	
	if (myRect.GetHeight() < 0) {
		myOldValue1 = myRect.GetBottom();
		myOldValue2 = myRect.GetTop();
		myRect.SetTop(myOldValue1);
		myRect.SetBottom(myOldValue2);
	}

	return myRect;
}


bool vrRubberBand::IsPositive() {
	
	wxRect myRect = _CreateRect();
	
	if (myRect.GetWidth() < 0 || myRect.GetHeight() < 0) {
		return false;
	}
	return true;
}


bool vrRubberBand::IsValid() {
	if (m_PointStart == wxDefaultPosition ||
		m_PointEnd == wxDefaultPosition) {
		wxLogError("First and / or last point not set, rubber band isn't valid");
		return false;
	}
	
	if (m_PointStart.x == m_PointEnd.x ||
		m_PointStart.y == m_PointEnd.y) {
		return false;
	}
	
	return true;
}


void vrRubberBand::Update() {
	
	wxClientDC dc( m_Window ) ;
	wxDCOverlay overlaydc( m_Overlay, &dc );
	overlaydc.Clear();
#ifdef __WXMAC__
	dc.SetPen( *wxGREY_PEN );
	dc.SetBrush( wxColour( 192,192,192,64 ) );
#else
	dc.SetPen( wxPen( *wxLIGHT_GREY, 2, wxSOLID ) );
	dc.SetBrush( *wxTRANSPARENT_BRUSH );
#endif
	dc.DrawRectangle( GetRect() );
	
}

