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


tmRenderer::tmRenderer(wxWindow * parent, wxWindowID id) : wxScrolledWindow(parent,id)
{

	
}




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
	// draw the image
	wxPaintDC dc (this);
	DoPrepareDC(dc);
	dc.Clear();
	
	
	dc.SetPen(*wxBLACK_PEN); 
	
	
	dc.DrawLine(0, 0, 100, 200); 
	
}
