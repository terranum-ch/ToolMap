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


BEGIN_EVENT_TABLE(tmRenderer, wxScrolledWindow)
	EVT_SIZE(tmRenderer::OnSizeChange)
	EVT_PAINT ( tmRenderer::OnPaint)
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
	
	if (m_bmp)
	{
		// draw the image
		wxPaintDC dc (this);
		dc.SetBackground(*wxWHITE);
		dc.Clear();
		
		//dc.SetPen(*wxBLACK_PEN);
		dc.DrawBitmap (*m_bmp,wxPoint(0,0),false);

	}
	
}
