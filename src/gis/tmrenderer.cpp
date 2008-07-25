/***************************************************************************
								tmrenderer.cpp
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

#include "tmrenderer.h"


DEFINE_EVENT_TYPE(tmEVT_LM_SIZE_CHANGED)
DEFINE_EVENT_TYPE(tmEVT_LM_MOUSE_MOVED)
DEFINE_EVENT_TYPE(tmEVT_LM_ZOOM_RECTANGLE_OUT)
DEFINE_EVENT_TYPE(tmEVT_LM_ZOOM_RECTANGLE_IN)


BEGIN_EVENT_TABLE(tmRenderer, wxScrolledWindow)
	EVT_SIZE(tmRenderer::OnSizeChange)
	EVT_PAINT ( tmRenderer::OnPaint)
	EVT_MOTION (tmRenderer::OnMouseMove)
	EVT_LEFT_DOWN (tmRenderer::OnMouseDown)
	EVT_LEFT_UP (tmRenderer::OnMouseUp)
END_EVENT_TABLE()



/***************************************************************************//**
 @brief Constructor
 @details Do nothing for the moment
 @author Lucien Schreiber (c) CREALP 2008
 @date 21 July 2008
 *******************************************************************************/
tmRenderer::tmRenderer(wxWindow * parent, wxWindowID id) : wxScrolledWindow(parent,id)
{
	m_bmp = NULL;
	m_SelectRect = new wxRubberBand(this);
	m_RubberStartCoord = wxPoint(-1,-1);
}



/***************************************************************************//**
 @brief Respond to a Size message
 @details This function is called each time the size of the tmRenderer windows
 change. This function does nothing but send the informations to the
 tmLayerManager
 @author Lucien Schreiber (c) CREALP 2008
 @date 21 July 2008
 *******************************************************************************/
void tmRenderer::OnSizeChange(wxSizeEvent & event)
{
	// new size object, will be deleted in the 
	// layer manager
	wxSize * mySize = new wxSize(event.GetSize());
	
	// send size to the layermanager
	wxCommandEvent evt(tmEVT_LM_SIZE_CHANGED, wxID_ANY);
	evt.SetClientData(mySize);
	GetEventHandler()->AddPendingEvent(evt);
}




void tmRenderer::OnPaint(wxPaintEvent & event)
{
	wxPaintDC dc (this);
	
	if (m_bmp)
	{
		// draw the image
		dc.SetBackground(*wxWHITE);
		dc.Clear();

				
		//dc.SetPen(*wxBLACK_PEN);
		dc.DrawBitmap (*m_bmp,wxPoint(0,0),false);

	}
	
}



void tmRenderer::OnMouseDown(wxMouseEvent & event)
{
	RubberBandStart(event.GetPosition());
}



void tmRenderer::OnMouseMove (wxMouseEvent & event)
{
	// rubber band
	RubberBandUpdate(event.GetPosition());
	
	// new point object, will be deleted in the layer
	// manager
	wxPoint * myPoint = new wxPoint(event.GetPosition());
	
	wxCommandEvent evt(tmEVT_LM_MOUSE_MOVED, wxID_ANY);
	evt.SetClientData(myPoint);
	GetEventHandler()->AddPendingEvent(evt);
}



void tmRenderer::OnMouseUp(wxMouseEvent & event)
{
	RubberBandStop();
	
}


void tmRenderer::RubberBandStart (const wxPoint & mousepos)
{
	m_RubberStartCoord = mousepos;
}


void tmRenderer::RubberBandUpdate(const wxPoint & mousepos)
{
	if (m_RubberStartCoord == wxPoint (-1,-1))
		return;
	
	m_SelectRect->SetGeometry(m_RubberStartCoord, mousepos);

}



void tmRenderer::RubberBandStop()
{
	wxRect * mySelectedRect = NULL;
	wxCommandEvent evt;
	evt.SetId(wxID_ANY);
	
	
	
	
	
	if (m_RubberStartCoord == wxPoint(-1,-1))
		return;
	
	// get selected values if selection is valid
	// and send it to the layermanager
	if(m_SelectRect->IsSelectedRectangleValid())
	{
		if(m_SelectRect->IsSelectedRectanglePositive())
		{
			evt.SetEventType(tmEVT_LM_ZOOM_RECTANGLE_IN);
			mySelectedRect = new wxRect(m_SelectRect->GetSelectedRectangle());
			evt.SetClientData(mySelectedRect);
			
		}
		else
		{
			evt.SetEventType(tmEVT_LM_ZOOM_RECTANGLE_OUT);
			mySelectedRect = new wxRect(m_SelectRect->GetSelectedRectangle());
			evt.SetClientData(mySelectedRect);
		}
		
		GetEventHandler()->AddPendingEvent(evt);
	}
	
	
	m_SelectRect->ClearOldRubberRect();
	m_RubberStartCoord = wxPoint(-1,-1);
}
