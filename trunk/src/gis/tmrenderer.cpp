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
#include "../img/cursor_editing.cpp"
#include "../img/cursor_oriented1.cpp"
#include "tmmanagerevent.h"


DEFINE_EVENT_TYPE(tmEVT_LM_SIZE_CHANGED)
DEFINE_EVENT_TYPE(tmEVT_LM_MOUSE_MOVED)
DEFINE_EVENT_TYPE(tmEVT_LM_ZOOM_RECTANGLE_OUT)
DEFINE_EVENT_TYPE(tmEVT_LM_ZOOM_RECTANGLE_IN)
DEFINE_EVENT_TYPE(tmEVT_LM_PAN_ENDED)
DEFINE_EVENT_TYPE(tmEVT_LM_SELECTION)
DEFINE_EVENT_TYPE(tmEVT_EM_DRAW_ENTER)
DEFINE_EVENT_TYPE(tmEVT_EM_CUT_LINE)
DEFINE_EVENT_TYPE(tmEVT_AM_SHORTCUT_PRESSED)
DEFINE_EVENT_TYPE(tmEVT_EM_MODIFY_CLICK)
DEFINE_EVENT_TYPE(tmEVT_EM_MODIFY_MOVED)
DEFINE_EVENT_TYPE(tmEVT_EM_MODIFY_UP)
DEFINE_EVENT_TYPE(tmEVT_EM_DRAW_CLICK)
DEFINE_EVENT_TYPE(tmEVT_EM_DRAW_MOVE)
DEFINE_EVENT_TYPE(tmEVT_EM_DRAW_DOWN)
DEFINE_EVENT_TYPE(tmEVT_EM_DRAW_ESC)
DEFINE_EVENT_TYPE(tmEVT_EM_MODIFY_MENU)
DEFINE_EVENT_TYPE(tmEVT_TM_UPDATE_TOOL_VIEW)
DEFINE_EVENT_TYPE(tmEVT_EM_DRAW_ORIENT_DOWN)
DEFINE_EVENT_TYPE(tmEVT_EM_DRAW_ORIENT_MOVE)
DEFINE_EVENT_TYPE(tmEVT_EM_DRAW_ORIENT_UP)


BEGIN_EVENT_TABLE(tmRenderer, wxScrolledWindow)
	EVT_SIZE(tmRenderer::OnSizeChange)
	EVT_PAINT ( tmRenderer::OnPaint)
	EVT_ERASE_BACKGROUND(tmRenderer::OnAvoidFlickering)
	EVT_MOTION (tmRenderer::OnMouseMove)
	EVT_LEFT_DOWN (tmRenderer::OnMouseDown)
	EVT_RIGHT_DOWN (tmRenderer::OnMouseRightDown)
	EVT_LEFT_UP (tmRenderer::OnMouseUp)
	EVT_KEY_DOWN (tmRenderer::OnShiftDown)
	EVT_KEY_UP (tmRenderer::OnShiftUp)
	EVT_KEY_DOWN (tmRenderer::OnKey)
END_EVENT_TABLE()



/***************************************************************************//**
 @brief Constructor
 @details Do nothing for the moment
 @author Lucien Schreiber (c) CREALP 2008
 @date 21 July 2008
 *******************************************************************************/
tmRenderer::tmRenderer(wxWindow * parent, wxWindowID id) : 
wxScrolledWindow(parent,id, wxDefaultPosition,wxDefaultSize, 
				  wxWS_EX_PROCESS_UI_UPDATES | wxWANTS_CHARS )
{
	m_bmp = NULL;
	m_ModifyCalled = false;
	m_DrawCalled = false;
	m_SelectRect = new wxRubberBand(this);
	m_StartCoord = wxPoint(-1,-1);
	m_ActualTool = tmTOOL_SELECT;
	m_ActualNotStockCursor = tmCURSOR_ZOOM_IN;
	m_PanBmp = NULL;
	m_ShiftDown = false;
	m_SnappingRadius = 0;
	m_OldSize = wxSize(0,0);
	
	BitmapUpdateSize();
	bool bWhite = BitmapSetToWhite();
	wxASSERT(bWhite);
}


bool tmRenderer::BitmapUpdateSize()
{
	if (m_bmp != NULL)
	{
		delete m_bmp;
		m_bmp = NULL;
	}
	
	int myWidth = 0;
	int myHeight = 0;
	GetClientSize(&myWidth, &myHeight);
	
	m_bmp = new wxBitmap(myWidth, myHeight);
	return true;
}


bool tmRenderer::BitmapSetToWhite()
{
	if (m_bmp == NULL)
		return false;
	
	int myWidth = 0;
	int myHeight = 0;
	GetClientSize(&myWidth, &myHeight);
	
	wxMemoryDC mdc;
	mdc.SelectObject(*m_bmp);
	mdc.SetBrush (wxBrush(*wxWHITE_BRUSH));
	mdc.SetPen   (wxPen(*wxWHITE_PEN));
	mdc.DrawRectangle (0,0,myWidth,myHeight);
	mdc.SelectObject(wxNullBitmap);	
	return true;
}


bool tmRenderer::BitmapCopyInto(wxBitmap * bmp)
{
	wxASSERT(bmp);
	wxASSERT(bmp->GetWidth() == m_bmp->GetWidth());
	wxASSERT(bmp->GetHeight() == m_bmp->GetHeight());
	
	wxMemoryDC mdc;
	mdc.SelectObject(*m_bmp);
	mdc.DrawBitmap (*bmp, 0,0);
	mdc.SelectObject(wxNullBitmap);	
	return true;
}


/***************************************************************************//**
 @brief Destructor
 @author Lucien Schreiber (c) CREALP 2009
 @date 26 January 2009
 *******************************************************************************/
tmRenderer::~tmRenderer()
{
	if (m_bmp)
		delete m_bmp;
	
	delete m_SelectRect;
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
	wxSize myActualSize = GetClientSize();
	
	BitmapUpdateSize();
	BitmapSetToWhite();

	// size change direction : smaller, bigger
	bool bSmaller = true;
	if (myActualSize.GetWidth() > m_OldSize.GetWidth() ||
		myActualSize.GetHeight() > m_OldSize.GetHeight())
		bSmaller = false;
	else if (myActualSize == m_OldSize)
		return;
	
		
	// new size array object, will be deleted in the layer manager
	tmArraySize * mySizes = new tmArraySize();
	mySizes->Add(m_OldSize);
	mySizes->Add(myActualSize);
		
	
	m_OldSize = myActualSize;
	
	// send size to the layermanager
	wxCommandEvent evt(tmEVT_LM_SIZE_CHANGED, wxID_ANY);
	evt.SetInt(bSmaller);
	evt.SetClientData(mySizes);
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
		case tmCURSOR_EDIT:
			myCursorBmp = wxGetBitmapFromMemory(cursor_editing);
			break;
			
		case tmCURSOR_ORIENTED:
			myCursorBmp = wxGetBitmapFromMemory(cursor_oriented1);
			break;
			
		default:
			return wxCursor (wxCURSOR_ARROW);
			break;
	}
	m_ActualNotStockCursor = cursor;
	
	wxImage mycursor (myCursorBmp.ConvertToImage()); 
	mycursor.SetOption (wxIMAGE_OPTION_CUR_HOTSPOT_X,8);
	mycursor.SetOption (wxIMAGE_OPTION_CUR_HOTSPOT_Y,8);
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
			
		case tmTOOL_DRAW:
			
			//this->SetCursor(wxCursor(wxCURSOR_CROSS));
			this->SetCursor(LoadCursorFromBitmap(tmCURSOR_EDIT));
			break;
			
		case tmTOOL_MODIFY:
			this->SetCursor(wxCursor(wxCURSOR_SIZING));
			break;
			
		case tmTOOL_CUT_LINES:
			this->SetCursor(wxCursor(wxCURSOR_BULLSEYE));
			break;
			
		case tmTOOL_ORIENTED_POINTS:
			this->SetCursor(LoadCursorFromBitmap(tmCURSOR_ORIENTED));
			break;
		
		default:
			this->SetCursor(wxCursor(wxCURSOR_ARROW));
			break;
	}
	
}



void tmRenderer::OnPaint(wxPaintEvent & event)
{
	wxPaintDC dc (this);
	
	if (m_bmp && m_bmp->IsOk())
	{
		// draw the image
		dc.SetBackground(*wxWHITE);
		dc.Clear();

				
		//dc.SetPen(*wxBLACK_PEN);
		dc.DrawBitmap (*m_bmp,wxPoint(0,0),false);
		
	}
	
}


// do nothing but don't propagate event 
void tmRenderer::OnAvoidFlickering(wxEraseEvent & event){
	
}



void tmRenderer::SetBitmapStatus(wxBitmap * bmp)
{
	if (bmp == NULL)
	{
		BitmapSetToWhite();
		return;
	}
	
	
	BitmapCopyInto(bmp);
}


void tmRenderer::OnMouseDown(wxMouseEvent & event)
{
	// rectangle zoom
	if (m_ActualTool == tmTOOL_ZOOM_RECTANGLE)
		RubberBandStart(event.GetPosition());
	
	// pan
	if (m_ActualTool == tmTOOL_PAN)
		PanStart(event.GetPosition());
	
	// select
	if (m_ActualTool == tmTOOL_SELECT)
		SelectStart(event.GetPosition());
	
	// draw
	if (m_ActualTool == tmTOOL_DRAW)
		DrawStart(event.GetPosition());
	
	if (m_ActualTool == tmTOOL_MODIFY)
		ModifyStart(event.GetPosition());
	
	// oriented pts
	if (m_ActualTool == tmTOOL_ORIENTED_POINTS)
		OrientedPtsStart(event.GetPosition());
	
	
	event.Skip();
}



void tmRenderer::OnMouseRightDown (wxMouseEvent & event)
{
	if (m_ActualTool == tmTOOL_MODIFY)
		ModifyMenu(event.GetPosition());
	
	event.Skip();
}


void tmRenderer::OnMouseMove (wxMouseEvent & event)
{
	if (m_ActualTool == tmTOOL_ZOOM_RECTANGLE)
		RubberBandUpdate(event.GetPosition());
	
	if (m_ActualTool == tmTOOL_PAN)
		PanUpdate(event.GetPosition());
	
	if (m_ActualTool == tmTOOL_SELECT)
		SelectUpdate(event.GetPosition());
	
	if (m_ActualTool == tmTOOL_DRAW)
		DrawMove(event.GetPosition());
	
	if (m_ActualTool == tmTOOL_MODIFY)
		ModifyUpdate(event.GetPosition());
	
	if (m_ActualTool == tmTOOL_ORIENTED_POINTS)
		OrientedPtsMove(event.GetPosition());
	
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
	
	if (m_ActualTool == tmTOOL_SELECT)
		SelectStop(event.GetPosition());
	
	// draw
	if (m_ActualTool == tmTOOL_DRAW)
		DrawStop(event.GetPosition());
	
	if (m_ActualTool == tmTOOL_CUT_LINES)
		CutLineClick(event.GetPosition());
	
	if (m_ActualTool == tmTOOL_MODIFY)
		ModifyStop(event.GetPosition());
	
	if (m_ActualTool == tmTOOL_ORIENTED_POINTS)
		OrientedPtsStop(event.GetPosition());
	
}




/***************************************************************************//**
 @brief Called when a key is pressed
 @details This function is used for changing tmRenderer::m_ShiftDown
 status.
 Because when user selects features, we need to know if shift was down.
 @author Lucien Schreiber (c) CREALP 2008
 @date 29 October 2008
 *******************************************************************************/
void tmRenderer::OnShiftDown	(wxKeyEvent & event)
{
	if(event.GetKeyCode() == WXK_SHIFT)
		m_ShiftDown = true;
	event.Skip();
		
}

/***************************************************************************//**
 @brief Called when a key is released
 @details This function is used for changing tmRenderer::m_ShiftDown
 status.
 Because when user selects features, we need to know if shift was down.
 @author Lucien Schreiber (c) CREALP 2008
 @date 29 October 2008
 *******************************************************************************/
void tmRenderer::OnShiftUp		(wxKeyEvent & event)
{
	if (event.GetKeyCode() == WXK_SHIFT)
		m_ShiftDown = false;
	event.Skip();
}



/***************************************************************************//**
 @brief Enter key was pressed
 @details This function mainly send a message to the #tmEditManager for saving
 the edited line
 @author Lucien Schreiber (c) CREALP 2009
 @date 05 February 2009
 *******************************************************************************/
void tmRenderer::OnKey	(wxKeyEvent & event)
{
	if (event.GetKeyCode() == WXK_RETURN || event.GetKeyCode() == WXK_TAB)
	{
		wxCommandEvent evt(tmEVT_EM_DRAW_ENTER, wxID_ANY);
		GetEventHandler()->AddPendingEvent(evt);
	}
	
	if (event.GetKeyCode() == WXK_ESCAPE)
	{
		wxCommandEvent evt(tmEVT_EM_DRAW_ESC, wxID_ANY);
		GetEventHandler()->AddPendingEvent(evt);
	}

	if (event.GetKeyCode() >= WXK_F1 && event.GetKeyCode() <= WXK_F12)
	{
		wxCommandEvent evt2(tmEVT_AM_SHORTCUT_PRESSED, wxID_ANY);
		evt2.SetInt( event.GetKeyCode());
		GetEventHandler()->AddPendingEvent(evt2);
	}

	
	event.Skip();
}



void tmRenderer::RubberBandStart (const wxPoint & mousepos)
{
	m_StartCoord = mousepos;
	m_SelectRect->SetPen(); // default pen
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


/***************************************************************************//**
 @brief Called when selection start (mouse down)
 @param mousepos actual coordinate of the mouse in screen pixels
 @author Lucien Schreiber (c) CREALP 2008
 @date 29 October 2008
 *******************************************************************************/
void tmRenderer::SelectStart (const wxPoint & mousepos)
{
	m_StartCoord = mousepos;
	m_SelectRect->SetPen(*wxBLACK_PEN);
	
}



/***************************************************************************//**
 @brief Called when selection is updated (mouse move)
 @param mousepos actual coordinate of the mouse in screen pixels
 @author Lucien Schreiber (c) CREALP 2008
 @date 29 October 2008
 *******************************************************************************/
void tmRenderer::SelectUpdate (const wxPoint & mousepos)
{
	// avoid drawing all the times when mouse wasen't down first.
	if (m_StartCoord == wxPoint(-1,-1))
		return;
	
	
	m_SelectRect->SetGeometry(m_StartCoord, mousepos);
}



/***************************************************************************//**
 @brief Called when selection finished (mouse up)
 @param mousepos actual coordinate of the mouse in screen pixels
 @author Lucien Schreiber (c) CREALP 2008
 @date 29 October 2008
 *******************************************************************************/
void tmRenderer::SelectStop (const wxPoint & mousepos)
{
	bool myShiftDown = m_ShiftDown;
	
	// if no rectangle is selected, select data by point
	// we make a small rectangle around the curent point
	wxRect * mySelectionRect = new wxRect(0,0,0,0);
	if(m_SelectRect->IsSelectedRectangleValid() == false)
	{
		int myRadius = tmSELECTION_DIAMETER / 2;
		*mySelectionRect = wxRect (mousepos.x - myRadius,
								  mousepos.y - myRadius,
								  tmSELECTION_DIAMETER,
								  tmSELECTION_DIAMETER);
	}
	else // if selection was done by rectangle
	{
		*mySelectionRect = m_SelectRect->GetSelectedRectangle();
	}
	
	// send event to the layermanager
	wxCommandEvent evt(tmEVT_LM_SELECTION, wxID_ANY);
	evt.SetInt(static_cast<int> (myShiftDown));
	// do not delete mySelection Rect, will be deleted in the layer
	// manager
	evt.SetClientData(mySelectionRect);
	GetEventHandler()->AddPendingEvent(evt);
	
	
	// cleaning stuff
	m_SelectRect->ClearOldRubberRect();
	m_StartCoord = wxPoint(-1,-1);
}




/***************************************************************************//**
 @brief Start the pan
 @details Call this function when the mouse is down for initiating the pan
 process. 
 This function grab the screen into a bitmap used later (in
 #PanUpdate()) when user moves the mouse.
 @param mousepos wxPoint position of the mouse when user click
 @author Lucien Schreiber (c) CREALP 2008
 @date 31 July 2008
 *******************************************************************************/
void tmRenderer::PanStart (const wxPoint & mousepos)
{
	m_StartCoord = mousepos;
	
	// capture the dc in a bitmap
	wxClientDC dc (this);
	
	wxSize mybitmapsize= GetClientSize(); 
	wxSize myFullSize = GetSize();
	int iHeight = mybitmapsize.GetHeight();
		
	if (m_PanBmp)
	{
		delete m_PanBmp;
		m_PanBmp = NULL;
	}
	
	//FIXME: BUG IN OSX
#ifdef __WXMAC__
	iHeight -= myFullSize.GetHeight() - iHeight;
#endif
	//
	
	m_PanBmp = new wxBitmap(mybitmapsize.GetWidth(),iHeight, -1);
	wxMemoryDC mdc;
	mdc.SelectObject(*m_PanBmp);
	mdc.Blit(0,0,mybitmapsize.GetWidth(),mybitmapsize.GetHeight(),&dc,0,0);
	mdc.SelectObject(wxNullBitmap);
	
	// empty real bmp
	SetBitmapStatus();
}



/***************************************************************************//**
 @brief Update the pan image during mouse move
 @details Call this function when the mouse move for moving the pan image.
 @param mousepos actual mouse position
 @author Lucien Schreiber (c) CREALP 2008
 @date 31 July 2008
 *******************************************************************************/
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
		wxMemoryDC mdc;
		wxSize wndsize = GetClientSize();
		wxBitmap tmpbmp (wndsize.GetWidth(), wndsize.GetHeight());
		mdc.SelectObject (tmpbmp);
		
		// draw all white
		mdc.SetBrush (wxBrush(*wxWHITE_BRUSH));
		mdc.SetPen   (wxPen(*wxWHITE_PEN));
		mdc.DrawRectangle (0,0,wndsize.GetWidth(),wndsize.GetHeight());
		mdc.DrawBitmap (*m_PanBmp, myNewPos.x,myNewPos.y);
		mdc.SelectObject(wxNullBitmap);
		
		SetBitmapStatus(&tmpbmp);
		Refresh();
		Update();
	}
}


/***************************************************************************//**
 @brief Refresh screen after pan
 @details Call this function when the pan is over to make cleanup and refresh
 screen.
 @param mousepos actual mouse position
 @author Lucien Schreiber (c) CREALP 2008
 @date 31 July 2008
 *******************************************************************************/
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
	
	SetBitmapStatus();

	if (m_PanBmp)
	{
		delete m_PanBmp;
		m_PanBmp = NULL;
	}
	m_StartCoord = wxPoint(-1,-1);
}



/***************************************************************************//**
 @brief Activate or not the snapping informations
 @details When activated, during mouse click, a radius indicating the snapping
 radius
 @param snapradius Size of the snapping tolerence, if 0 snapping is OFF
 @author Lucien Schreiber (c) CREALP 2009
 @date 26 January 2009
 *******************************************************************************/
void tmRenderer::ToogleSnapping (int snapradius)
{
	m_SnappingRadius = snapradius;
}



/***************************************************************************//**
 @brief User click with Draw tool activated
 @param mousepos Actual mouse position in screen coordinate
 @author Lucien Schreiber (c) CREALP 2009
 @date 26 January 2009
 *******************************************************************************/
void tmRenderer::DrawStart (const wxPoint & mousepos)
{
	// ensure only called once.
	if (m_DrawCalled == true)
		return;
	
	m_DrawCalled = true;
	
	
	m_StartCoord = mousepos;
	
	
	wxCommandEvent evt(tmEVT_EM_DRAW_DOWN, wxID_ANY);
	wxPoint * myClickedPos = new wxPoint(mousepos.x,
										 mousepos.y);
	evt.SetClientData(myClickedPos);
	GetEventHandler()->AddPendingEvent(evt);
	
	/*wxClientDC myDC (this);
	myDC.SetLogicalFunction (wxINVERT);
	
	if (m_SnappingRadius > 0)
		myDC.DrawCircle(mousepos.x, mousepos.y, m_SnappingRadius);*/
	
}

/***************************************************************************//**
 @brief User move with Draw tool activated
 @param mousepos Actual mouse position in screen coordinate
 @author Lucien Schreiber (c) CREALP 2009
 @date 26 January 2009
 *******************************************************************************/
void tmRenderer::DrawMove (const wxPoint & mousepos)
{
	// sent message to edit manager
	wxCommandEvent evt(tmEVT_EM_DRAW_MOVE, wxID_ANY);
	wxPoint * myClickedPos = new wxPoint(mousepos.x,
										 mousepos.y);
	evt.SetClientData(myClickedPos);
	GetEventHandler()->AddPendingEvent(evt);
}


/***************************************************************************//**
 @brief Called when mouse up and drawing tool selected
 @param mousepos Actual mouse position
 @author Lucien Schreiber (c) CREALP 2009
 @date 26 January 2009
 *******************************************************************************/
void tmRenderer::DrawStop  (const wxPoint & mousepos)
{
	if (m_DrawCalled == false)
		return;
	
	/*wxClientDC myDC (this);
	myDC.SetLogicalFunction(wxINVERT);
	if (m_SnappingRadius > 0 && m_StartCoord != wxPoint(-1,-1))
	{
		myDC.DrawCircle(m_StartCoord.x, m_StartCoord.y, m_SnappingRadius);
	}*/
	
	// sent message to edit manager
	wxCommandEvent evt(tmEVT_EM_DRAW_CLICK, wxID_ANY);
	wxPoint * myClickedPos = new wxPoint(m_StartCoord.x,
										 m_StartCoord.y);
	evt.SetClientData(myClickedPos);
	GetEventHandler()->AddPendingEvent(evt);
	
	m_StartCoord = wxPoint(-1,-1);
	
	wxASSERT(m_DrawCalled);
	m_DrawCalled = false;
}


void tmRenderer::OrientedPtsStart(const wxPoint & mousepos)
{
	wxCommandEvent evt(tmEVT_EM_DRAW_ORIENT_DOWN, wxID_ANY);
	wxPoint * myClickedPos = new wxPoint(mousepos.x,
										 mousepos.y);
	evt.SetClientData(myClickedPos);
	GetEventHandler()->AddPendingEvent(evt);
	
}



void tmRenderer::OrientedPtsMove (const wxPoint & mousepos)
{
	wxCommandEvent evt(tmEVT_EM_DRAW_ORIENT_MOVE, wxID_ANY);
	wxPoint * myClickedPos = new wxPoint(mousepos.x,
										 mousepos.y);
	evt.SetClientData(myClickedPos);
	GetEventHandler()->AddPendingEvent(evt);
	
}



void tmRenderer:: OrientedPtsStop (const wxPoint & mousepos)
{
	wxCommandEvent evt(tmEVT_EM_DRAW_ORIENT_UP, wxID_ANY);
	wxPoint * myClickedPos = new wxPoint(mousepos.x,
										 mousepos.y);
	evt.SetClientData(myClickedPos);
	GetEventHandler()->AddPendingEvent(evt);
	
}


/***************************************************************************//**
 @brief Called when mouse down and modify tool selected
 @param mousepos Actual mouse position
 @author Lucien Schreiber (c) CREALP 2009
 @date 28 April 2009
 *******************************************************************************/
void tmRenderer::ModifyStart (const wxPoint & mousepos)
{
	// ensure only called once.
	if (m_ModifyCalled == true)
		return;
	
	m_ModifyCalled = true;
	
	// sent message to edit manager
	wxCommandEvent evt(tmEVT_EM_MODIFY_CLICK, wxID_ANY);
	wxPoint * myClickedPos = new wxPoint(mousepos.x,
										 mousepos.y);
	evt.SetClientData(myClickedPos);
	GetEventHandler()->AddPendingEvent(evt);
}


/***************************************************************************//**
 @brief Called when mouse move and modify tool selected
 @param mousepos Actual mouse position
 @author Lucien Schreiber (c) CREALP 2009
 @date 28 April 2009
 *******************************************************************************/
void tmRenderer::ModifyUpdate (const wxPoint & mousepos)
{
	if (m_ModifyCalled == false)
		return;
	
	// sent message to edit manager
	wxCommandEvent evt(tmEVT_EM_MODIFY_MOVED, wxID_ANY);
	wxPoint * myClickedPos = new wxPoint(mousepos.x,
										 mousepos.y);
	evt.SetClientData(myClickedPos);
	GetEventHandler()->AddPendingEvent(evt);
	
}


/***************************************************************************//**
 @brief Called when mouse up and modify tool selected
 @param mousepos Actual mouse position
 @author Lucien Schreiber (c) CREALP 2009
 @date 28 April 2009
 *******************************************************************************/
void tmRenderer::ModifyStop (const wxPoint & mousepos)
{
	if (m_ModifyCalled == false)
		return;
	
	
	// sent message to edit manager
	wxCommandEvent evt(tmEVT_EM_MODIFY_UP, wxID_ANY);
	wxPoint * myClickedPos = new wxPoint(mousepos.x,
										 mousepos.y);
	evt.SetClientData(myClickedPos);
	GetEventHandler()->AddPendingEvent(evt);
	
		
	wxASSERT(m_ModifyCalled == true);
	m_ModifyCalled = false;
}



void tmRenderer::ModifyMenu (const wxPoint & mousepos)
{
	// sent message to edit manager
	wxCommandEvent evt(tmEVT_EM_MODIFY_MENU, wxID_ANY);
	wxPoint * myClickedPos = new wxPoint(mousepos.x,
										 mousepos.y);
	evt.SetClientData(myClickedPos);
	GetEventHandler()->AddPendingEvent(evt);	
}



/***************************************************************************//**
 @brief Click up with cut line tool
 @author Lucien Schreiber (c) CREALP 2009
 @date 05 February 2009
 *******************************************************************************/
void tmRenderer::CutLineClick (const wxPoint & mousepos)
{
	wxCommandEvent evt(tmEVT_EM_CUT_LINE, wxID_ANY);
	wxPoint * myClickedPos = new wxPoint(mousepos.x,
										 mousepos.y);
	evt.SetClientData(myClickedPos);
	GetEventHandler()->AddPendingEvent(evt);
}



/***************************************************************************//**
 @brief Draw a circle using inverse video
 @param pt Coordinate of center
 @param radius Circle radius in px.
 @author Lucien Schreiber (c) CREALP 2009
 @date 24 February 2009
 *******************************************************************************/
void tmRenderer::DrawCircleVideoInverse (wxPoint pt, int radius)
{
	wxClientDC dc(this);
	dc.SetLogicalFunction(wxINVERT);
	
	dc.DrawCircle(pt, radius);
}

