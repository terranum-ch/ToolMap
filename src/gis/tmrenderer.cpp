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

// cursor images
#include "../img/tmimgfunc.h"	// for image processing
#include "../img/cursor_zoom_in.cpp"
#include "../img/cursor_zoom_out.cpp"
#include "../img/cursor_hand.cpp"


DEFINE_EVENT_TYPE(tmEVT_LM_SIZE_CHANGED)
DEFINE_EVENT_TYPE(tmEVT_LM_MOUSE_MOVED)
DEFINE_EVENT_TYPE(tmEVT_LM_ZOOM_RECTANGLE_OUT)
DEFINE_EVENT_TYPE(tmEVT_LM_ZOOM_RECTANGLE_IN)
DEFINE_EVENT_TYPE(tmEVT_LM_PAN_ENDED)


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
	m_StartCoord = wxPoint(-1,-1);
	m_ActualTool = tmTOOL_SELECT;
	m_ActualNotStockCursor = tmCURSOR_ZOOM_IN;
	m_PanBmp = NULL;
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



void tmRenderer::SetTool (tmGIS_TOOL selected_tool)
{
	m_ActualTool = selected_tool;
	
	ChangeCursor(selected_tool);
}



wxCursor tmRenderer::LoadCursorFromBitmap (tmGIS_CURSOR cursor)
{
	wxBitmap myCursorBmp;
	
	switch (cursor)
	{
		case tmCURSOR_ZOOM_IN:
			myCursorBmp = wxGetBitmapFromMemory(cursor_zoom_in);
			break;
		case tmCURSOR_ZOOM_OUT:
			myCursorBmp = wxGetBitmapFromMemory(cursor_zoom_out);
			break;
		case tmCURSOR_HAND:
			myCursorBmp = wxGetBitmapFromMemory(cursor_hand);
			break;
		default:
			return wxCursor (wxCURSOR_ARROW);
			break;
	}
	m_ActualNotStockCursor = cursor;
	
	wxImage mycursor (myCursorBmp.ConvertToImage()); 
	mycursor.SetOption (wxIMAGE_OPTION_CUR_HOTSPOT_X,6);
	mycursor.SetOption (wxIMAGE_OPTION_CUR_HOTSPOT_Y,6);
	return wxCursor(mycursor);
	
}


void tmRenderer::ChangeCursor (const tmGIS_TOOL & selected_tool)
{
	switch (selected_tool)
	{
		case tmTOOL_ZOOM_RECTANGLE_IN:
			this->SetCursor(LoadCursorFromBitmap(tmCURSOR_ZOOM_IN));
			break;
			
		case tmTOOL_ZOOM_RECTANGLE_OUT:
			this->SetCursor(LoadCursorFromBitmap(tmCURSOR_ZOOM_OUT));
			break;
			
		case tmTOOL_PAN:
			this->SetCursor(LoadCursorFromBitmap(tmCURSOR_HAND));
			break;
		
		default:
			this->SetCursor(wxCursor(wxCURSOR_ARROW));
			break;
	}
	
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
	// rectangle zoom
	if (m_ActualTool == tmTOOL_ZOOM_RECTANGLE)
		RubberBandStart(event.GetPosition());
	
	// psn
	if (m_ActualTool == tmTOOL_PAN)
		PanStart(event.GetPosition());
}



void tmRenderer::OnMouseMove (wxMouseEvent & event)
{
	if (m_ActualTool == tmTOOL_ZOOM_RECTANGLE)
		RubberBandUpdate(event.GetPosition());
	
	if (m_ActualTool == tmTOOL_PAN)
		PanUpdate(event.GetPosition());
	
	// new point object, will be deleted in the layer
	// manager
	wxPoint * myPoint = new wxPoint(event.GetPosition());
	
	wxCommandEvent evt(tmEVT_LM_MOUSE_MOVED, wxID_ANY);
	evt.SetClientData(myPoint);
	GetEventHandler()->AddPendingEvent(evt);
}



void tmRenderer::OnMouseUp(wxMouseEvent & event)
{
	if (m_ActualTool == tmTOOL_ZOOM_RECTANGLE)
		RubberBandStop();
	
	if (m_ActualTool == tmTOOL_PAN)
		PanStop(event.GetPosition());
	
}


void tmRenderer::RubberBandStart (const wxPoint & mousepos)
{
	m_StartCoord = mousepos;
}


void tmRenderer::RubberBandUpdate(const wxPoint & mousepos)
{
	if (m_StartCoord == wxPoint (-1,-1))
		return;
	
	m_SelectRect->SetGeometry(m_StartCoord, mousepos);
	
	// change cursor if needed
	if (m_SelectRect->IsSelectedRectanglePositive() && 
		m_ActualNotStockCursor != tmCURSOR_ZOOM_IN)
	{
		ChangeCursor(tmTOOL_ZOOM_RECTANGLE_IN);
	}
	
	if (!m_SelectRect->IsSelectedRectanglePositive() && 
		m_ActualNotStockCursor != tmCURSOR_ZOOM_OUT)
	{
		ChangeCursor(tmTOOL_ZOOM_RECTANGLE_OUT);
	}


}



void tmRenderer::RubberBandStop()
{
	wxRect * mySelectedRect = NULL;
	wxCommandEvent evt;
	evt.SetId(wxID_ANY);
	
	
	if (m_StartCoord == wxPoint(-1,-1))
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
	m_StartCoord = wxPoint(-1,-1);
}



void tmRenderer::PanStart (const wxPoint & mousepos)
{
	m_StartCoord = mousepos;
	
	// capture the dc in a bitmap
	wxClientDC dc (this);
	
	wxSize mybitmapsize=dc.GetSize(); 
		
	wxBitmap thebmp(mybitmapsize.GetWidth(),mybitmapsize.GetHeight(),-1); 
	wxMemoryDC mdc; 
	mdc.SelectObject(thebmp); 
	mdc.Blit(0,0,mybitmapsize.GetWidth(),mybitmapsize.GetHeight(),&dc,0,0); 
		
	m_PanBmp = new wxBitmap(thebmp);
}


void tmRenderer::PanUpdate (const wxPoint & mousepos)
{
	// compute the new raster origin
	wxPoint myNewPos(mousepos.x - m_StartCoord.x,
					 mousepos.y - m_StartCoord.y);
	
	if (m_PanBmp == NULL || !m_PanBmp->IsOk())
		return;
		
	// we move the raster if mouse mouve
	if (myNewPos.x != 0 && myNewPos.y != 0)
	{
		wxMemoryDC dc;
		wxSize wndsize = GetSize();
		wxBitmap tmpbmp (wndsize.GetWidth(), wndsize.GetHeight());
		dc.SelectObject (tmpbmp);
		
		// draw all white
		dc.SetBrush (wxBrush(*wxWHITE_BRUSH));
		dc.SetPen   (wxPen(*wxWHITE_PEN));
		dc.DrawRectangle (0,0,wndsize.GetWidth(),wndsize.GetHeight());
		dc.DrawBitmap (*m_PanBmp, myNewPos.x,myNewPos.y);
		dc.SelectObject(wxNullBitmap);
		
		*m_bmp = tmpbmp;
		Refresh();
		Update();
	}
}


void tmRenderer::PanStop (const wxPoint & mousepos)
{
	// compute the new raster origin
	// myNewPos will be deleted in the layermanager
	wxPoint * myNewPos = new wxPoint(mousepos.x - m_StartCoord.x,
									 mousepos.y - m_StartCoord.y);
	
	// send message to renderer with 
	// displacement done.
	wxCommandEvent evt(tmEVT_LM_PAN_ENDED, wxID_ANY);
	evt.SetClientData(myNewPos);
	GetEventHandler()->AddPendingEvent(evt);
	
	delete m_PanBmp;
	m_PanBmp = NULL;
	m_StartCoord = wxPoint(-1,-1);
}

