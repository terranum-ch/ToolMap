/***************************************************************************
 tmrenderer.cpp
 Deals with the main renderer windows
 -------------------
 copyright            : (C) 2007 CREALP Lucien Schreiber
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

#include "../core/tmstatsevent.h"
#include "../core/vrrubberband.h"
#include "../gui/tmtoolmanager.h"
#include "../gui/bitmaps.h"
#include "tmeditmanager.h"
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
DEFINE_EVENT_TYPE(tmEVT_EM_MODIFY_SHARED_DOWN)
DEFINE_EVENT_TYPE(tmEVT_EM_MODIFY_SHARED_UP)
DEFINE_EVENT_TYPE(tmEVT_EM_MODIFY_SHARED_MOVE)

BEGIN_EVENT_TABLE(tmRenderer, wxScrolledWindow)
EVT_SIZE(tmRenderer::OnSizeChange)
EVT_PAINT(tmRenderer::OnPaint)
EVT_ERASE_BACKGROUND(tmRenderer::OnAvoidFlickering)
EVT_MOTION(tmRenderer::OnMouseMove)
EVT_LEFT_DOWN(tmRenderer::OnMouseDown)
EVT_LEFT_DCLICK(tmRenderer::OnMouseDClick)
EVT_MOUSEWHEEL(tmRenderer::OnMouseWheel)
// EVT_RIGHT_DOWN (tmRenderer::OnMouseRightDown)
EVT_LEFT_UP(tmRenderer::OnMouseUp)
EVT_KEY_DOWN(tmRenderer::OnShiftDown)
EVT_KEY_UP(tmRenderer::OnShiftUp)
EVT_KEY_DOWN(tmRenderer::OnKey)
EVT_TIMER(wxID_ANY, tmRenderer::OnWheelTimer)
END_EVENT_TABLE()

tmRenderer::tmRenderer(wxWindow *parent, wxWindowID id)
    : wxScrolledWindow(parent, id, wxDefaultPosition, wxDefaultSize, wxWS_EX_PROCESS_UI_UPDATES | wxWANTS_CHARS,
                       TMRENDERER_WINDOW_NAME) {
  m_bmp = nullptr;
  m_EditManager = nullptr;
  m_ToolManager = nullptr;
  m_ModifyCalled = false;
  m_DrawCalled = false;
  m_StartCoord = wxPoint(-1, -1);
  m_ActualTool = tmTOOL_SELECT;
  m_ActualNotStockCursor = tmCURSOR_ZOOM_IN;
  m_PanBmp = nullptr;
  m_ShiftDown = false;
  m_OldSize = wxSize(0, 0);
  m_Rubber = nullptr;
  m_WheelRotation = 0;
  m_WheelTimer.SetOwner(this, wxID_ANY);
  m_WheelPosition = wxDefaultPosition;
  m_isPanning = false;

  SetBackgroundStyle(wxBG_STYLE_PAINT);

  InitSize();
}

bool tmRenderer::BitmapUpdateSize() {
  if (m_bmp != nullptr) {
    wxDELETE(m_bmp);
  }

  int myWidth = 0;
  int myHeight = 0;
  GetClientSize(&myWidth, &myHeight);
  if (myWidth == 0 || myHeight == 0) {
    return false;
  }
  m_bmp = new wxBitmap(myWidth, myHeight);
  return true;
}

bool tmRenderer::BitmapSetToWhite() {
  if (m_bmp == nullptr) {
    return false;
  }

  int myWidth = 0;
  int myHeight = 0;
  GetClientSize(&myWidth, &myHeight);

  wxMemoryDC mdc;
  mdc.SelectObject(*m_bmp);
  mdc.SetBrush(wxBrush(*wxWHITE_BRUSH));
  mdc.SetPen(wxPen(*wxWHITE_PEN));
  mdc.DrawRectangle(0, 0, myWidth, myHeight);
  mdc.SelectObject(wxNullBitmap);
  return true;
}

bool tmRenderer::BitmapCopyInto(wxBitmap *bmp) {
  wxASSERT(bmp);
  wxASSERT(m_bmp);
  wxASSERT(bmp->GetWidth() == m_bmp->GetWidth());
  wxASSERT(bmp->GetHeight() == m_bmp->GetHeight());

  wxMemoryDC mdc;
  mdc.SelectObject(*m_bmp);
  mdc.DrawBitmap(*bmp, 0, 0);
  mdc.SelectObject(wxNullBitmap);
  return true;
}

/***************************************************************************/ /**
  @brief Destructor
  @author Lucien Schreiber (c) CREALP 2009
  @date 26 January 2009
  *******************************************************************************/
tmRenderer::~tmRenderer() {
  wxDELETE(m_bmp);
}

/***************************************************************************/ /**
  @brief Respond to a Size message
  @details This function is called each time the size of the tmRenderer windows
  change. This function does nothing but send the informations to the
  tmLayerManager
  @author Lucien Schreiber (c) CREALP 2008
  @date 21 July 2008
  *******************************************************************************/
void tmRenderer::OnSizeChange(wxSizeEvent &event) {
  if (event.GetSize().GetWidth() < 2 && event.GetSize().GetHeight() < 2) {
    event.Skip();
    return;
  }

  wxSize myActualSize = GetClientSize();

  BitmapUpdateSize();
  BitmapSetToWhite();

  // size change direction : smaller, bigger
  bool bSmaller = true;
  if (myActualSize.GetWidth() > m_OldSize.GetWidth() || myActualSize.GetHeight() > m_OldSize.GetHeight()) {
    bSmaller = false;
  } else if (myActualSize == m_OldSize) {
    return;
  }

  // new size array object, will be deleted in the layer manager
  tmArraySize *mySizes = new tmArraySize();
  mySizes->Add(m_OldSize);
  mySizes->Add(myActualSize);

  m_OldSize = myActualSize;
  // send size to the layermanager
  wxCommandEvent evt(tmEVT_LM_SIZE_CHANGED, wxID_ANY);
  evt.SetInt(bSmaller);
  evt.SetClientData(mySizes);
  GetEventHandler()->ProcessEvent(evt);
}

void tmRenderer::InitSize() {
  wxSize myActualSize = GetClientSize();

  BitmapUpdateSize();
  BitmapSetToWhite();

  // new size array object, will be deleted in the layer manager
  tmArraySize *mySizes = new tmArraySize();
  mySizes->Add(m_OldSize);
  mySizes->Add(myActualSize);

  m_OldSize = myActualSize;
  // send size to the layermanager
  wxCommandEvent evt(tmEVT_LM_SIZE_CHANGED, wxID_ANY);
  evt.SetInt(false);
  evt.SetClientData(mySizes);
  GetEventHandler()->ProcessEvent(evt);
}

void tmRenderer::SetTool(tmGIS_TOOL selected_tool) {
  m_ActualTool = selected_tool;
  ChangeCursor(selected_tool);
}

wxCursor tmRenderer::LoadCursorFromBitmap(tmGIS_CURSOR cursor) {
  wxBitmap myCursorBmp;
  switch (cursor) {
    case tmCURSOR_ZOOM_IN:
      myCursorBmp = Bitmaps::GetCursorBitmap(Bitmaps::ID_CURSORS::ZOOM_IN);
      break;
    case tmCURSOR_ZOOM_OUT:
      myCursorBmp = Bitmaps::GetCursorBitmap(Bitmaps::ID_CURSORS::ZOOM_OUT);
      break;
    case tmCURSOR_HAND:
      myCursorBmp = Bitmaps::GetCursorBitmap(Bitmaps::ID_CURSORS::HAND);
      break;
    case tmCURSOR_EDIT:
      myCursorBmp = Bitmaps::GetCursorBitmap(Bitmaps::ID_CURSORS::VERTEX_EDIT);
      break;

    case tmCURSOR_VERTEX_ADD:
      myCursorBmp = Bitmaps::GetCursorBitmap(Bitmaps::ID_CURSORS::VERTEX_ADD);
      break;

    case tmCURSOR_VERTEX_REMOVE:
      myCursorBmp = Bitmaps::GetCursorBitmap(Bitmaps::ID_CURSORS::VERTEX_REMOVE);
      break;

    case tmCURSOR_ORIENTED:
      myCursorBmp = Bitmaps::GetCursorBitmap(Bitmaps::ID_CURSORS::ORIENTED);
      break;

    default:
      return wxCursor(wxCURSOR_ARROW);
      break;
  }
  m_ActualNotStockCursor = cursor;

  wxImage mycursor(myCursorBmp.ConvertToImage());
  mycursor.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_X, 16);
  mycursor.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_Y, 16);
  return wxCursor(mycursor);
}

void tmRenderer::ChangeCursor(const tmGIS_TOOL &selected_tool) {
  switch (selected_tool) {
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
    case tmTOOL_DRAW_BEZIER:

      // this->SetCursor(wxCursor(wxCURSOR_CROSS));
      this->SetCursor(LoadCursorFromBitmap(tmCURSOR_EDIT));
      break;

    case tmTOOL_MODIFY:
    case tmTOOL_MODIFY_BEZIER:
      this->SetCursor(wxCursor(wxCURSOR_SIZING));
      break;

    case tmTOOL_CUT_LINES:
      this->SetCursor(wxCursor(wxCURSOR_BULLSEYE));
      break;

    case tmTOOL_ORIENTED_POINTS:
      this->SetCursor(LoadCursorFromBitmap(tmCURSOR_ORIENTED));
      break;

    case tmTOOL_MODIFY_SHARED:
      this->SetCursor(wxCursor(wxCURSOR_RIGHT_ARROW));
      break;

    case tmTOOL_VERTEX_INSERT:
      this->SetCursor(LoadCursorFromBitmap(tmCURSOR_VERTEX_ADD));
      break;

    case tmTOOL_VERTEX_DELETE:
      this->SetCursor(LoadCursorFromBitmap(tmCURSOR_VERTEX_REMOVE));
      break;

    default:
      this->SetCursor(wxCursor(wxCURSOR_ARROW));
      break;
  }
}

void tmRenderer::OnPaint(wxPaintEvent &event) {
  wxAutoBufferedPaintDC dc(this);
  wxGCDC gcdc(dc);

  if (!gcdc.IsOk()) {
    return;
  }

  if (m_bmp && m_bmp->IsOk()) {
    dc.SetBackground(*wxWHITE);
    dc.Clear();
    dc.DrawBitmap(*m_bmp, wxPoint(0, 0), false);
  }

  if (!m_isPanning) {
    m_EditManager->BezierDraw(&gcdc);
    m_EditManager->DrawSnappingCircle(&gcdc);
  }
  m_EditManager->ArcDraw(&gcdc);
  m_EditManager->BezierModifyDraw(&gcdc);
  m_ToolManager->DrawDanglingNodes(&gcdc);
}

// do nothing but don't propagate event
void tmRenderer::OnAvoidFlickering(wxEraseEvent &event) {}

void tmRenderer::SetBitmapStatus(wxBitmap *bmp) {
  if (bmp == nullptr) {
    BitmapSetToWhite();
    return;
  }
  BitmapCopyInto(bmp);
}

void tmRenderer::OnMouseDown(wxMouseEvent &event) {
  SetFocus();
  CaptureMouse();

  // rectangle zoom
  if (m_ActualTool == tmTOOL_ZOOM_RECTANGLE) ZoomStart(event.GetPosition());

  // pan
  if (m_ActualTool == tmTOOL_PAN) PanStart(event.GetPosition());

  // select
  if (m_ActualTool == tmTOOL_SELECT) SelectStart(event.GetPosition());

  // draw
  if (m_ActualTool == tmTOOL_DRAW) {
    if (m_EditManager->IsLayerSpatialType(LAYER_SPATIAL_POLYGON) || m_EditManager->IsLayerSpatialType(LAYER_SPATIAL_POINT)){
      ReleaseMouse();
    }
    m_EditManager->ArcClick(event.GetPosition());
  }

  if (m_ActualTool == tmTOOL_MODIFY) {
    m_EditManager->ArcModifyClickDown(event.GetPosition());
  }

  if (m_ActualTool == tmTOOL_MODIFY_BEZIER) {
    m_EditManager->BezierModifyClickDown(event.GetPosition());
  }

  // oriented pts
  if (m_ActualTool == tmTOOL_ORIENTED_POINTS) OrientedPtsStart(event.GetPosition());

  if (m_ActualTool == tmTOOL_MODIFY_SHARED) {
    ModifySharedStart(event.GetPosition());
  }
}

void tmRenderer::OnMouseMove(wxMouseEvent &event) {
  if (m_ActualTool == tmTOOL_SELECT) SelectUpdate(event);

  if (m_ActualTool == tmTOOL_DRAW) {
    m_EditManager->ArcMove(event.GetPosition());
  }

  if (m_ActualTool == tmTOOL_DRAW_BEZIER) {
    m_EditManager->BezierMove(event.GetPosition());
  }

  if (event.Dragging()) {
    if (m_ActualTool == tmTOOL_ZOOM_RECTANGLE) ZoomUpdate(event);

    if (m_ActualTool == tmTOOL_PAN) PanUpdate(event.GetPosition());

    if (m_ActualTool == tmTOOL_MODIFY) {
      m_EditManager->ArcModifyClickMove(event.GetPosition());
    }

    if (m_ActualTool == tmTOOL_ORIENTED_POINTS) OrientedPtsMove(event.GetPosition());

    if (m_ActualTool == tmTOOL_MODIFY_SHARED) {
      ModifySharedUpdate(event.GetPosition());
    }

    if (m_ActualTool == tmTOOL_MODIFY_BEZIER) {
      m_EditManager->BezierModifyClickMove(event.GetPosition());
    }
  }

  // new point object, will be deleted in the layer
  // manager
  wxPoint *myPoint = new wxPoint(event.GetPosition());
  wxCommandEvent evt(tmEVT_LM_MOUSE_MOVED, wxID_ANY);
  evt.SetClientData(myPoint);
  GetEventHandler()->QueueEvent(evt.Clone());
}

void tmRenderer::OnMouseUp(wxMouseEvent &event) {
  if (HasCapture()) {
    ReleaseMouse();
  }

  if (m_ActualTool == tmTOOL_DRAW_BEZIER) {
    m_EditManager->BezierClick(event.GetPosition());
  }

  if (m_ActualTool == tmTOOL_ZOOM_RECTANGLE) ZoomStop(event.GetPosition());

  if (m_ActualTool == tmTOOL_PAN) PanStop(event.GetPosition());

  if (m_ActualTool == tmTOOL_SELECT) SelectStop(event.GetPosition());

  if (m_ActualTool == tmTOOL_CUT_LINES) CutLineClick(event.GetPosition());

  if (m_ActualTool == tmTOOL_MODIFY) {
    m_EditManager->ArcModifyClickUp(event.GetPosition());
  }

  if (m_ActualTool == tmTOOL_VERTEX_INSERT) {
    m_EditManager->ArcVertexInsertUp(event.GetPosition());
  }

  if (m_ActualTool == tmTOOL_VERTEX_DELETE) {
    m_EditManager->ArcVertexDeleteUp(event.GetPosition());
  }

  if (m_ActualTool == tmTOOL_ORIENTED_POINTS) OrientedPtsStop(event.GetPosition());

  if (m_ActualTool == tmTOOL_MODIFY_SHARED) {
    ModifySharedStop(event.GetPosition());
  }

  if (m_ActualTool == tmTOOL_MODIFY_BEZIER) {
    m_EditManager->BezierModifyClickUp(event.GetPosition());
  }

  // send statistics
  wxCommandEvent evt(tmEVT_STAT_CLICK, wxID_ANY);
  GetEventHandler()->QueueEvent(evt.Clone());
}

void tmRenderer::OnMouseCaptureLost(wxMouseEvent &event) {
  // Only used under Windows. Mouse capture is lost when a dialog is displayed.
  if (HasCapture()) {
    ReleaseMouse();
  }
}

void tmRenderer::OnMouseDClick(wxMouseEvent &event) {
  if (m_ActualTool == tmTOOL_PAN) {
    PanDClick(event);
  }
}

void tmRenderer::OnMouseWheel(wxMouseEvent &event) {
  if (event.GetWheelRotation() >= 0) {
    m_WheelRotation += 1;
  } else {
    m_WheelRotation -= 1;
  }
  m_WheelPosition = event.GetPosition();
  if (m_WheelTimer.IsRunning()) {
    return;
  }
  m_WheelTimer.Start(100, true);
}

/***************************************************************************/ /**
  @brief Called when a key is pressed
  @details This function is used for changing tmRenderer::m_ShiftDown
  status.
  Because when user selects features, we need to know if shift was down.
  @author Lucien Schreiber (c) CREALP 2008
  @date 29 October 2008
  *******************************************************************************/
void tmRenderer::OnShiftDown(wxKeyEvent &event) {
  if (event.GetKeyCode() == WXK_SHIFT) m_ShiftDown = true;
  event.Skip();
}

/***************************************************************************/ /**
  @brief Called when a key is released
  @details This function is used for changing tmRenderer::m_ShiftDown
  status.
  Because when user selects features, we need to know if shift was down.
  @author Lucien Schreiber (c) CREALP 2008
  @date 29 October 2008
  *******************************************************************************/
void tmRenderer::OnShiftUp(wxKeyEvent &event) {
  if (event.GetKeyCode() == WXK_SHIFT) m_ShiftDown = false;
  event.Skip();
}

/***************************************************************************/ /**
  @brief Enter key was pressed
  @details This function mainly send a message to the #tmEditManager for saving
  the edited line
  @author Lucien Schreiber (c) CREALP 2009
  @date 05 February 2009
  *******************************************************************************/
void tmRenderer::OnKey(wxKeyEvent &event) {
  if (event.GetKeyCode() == WXK_RETURN || event.GetKeyCode() == WXK_TAB) {
    wxCommandEvent evt(tmEVT_EM_DRAW_ENTER, wxID_ANY);
    GetEventHandler()->QueueEvent(evt.Clone());
  }

  if (event.GetKeyCode() == WXK_ESCAPE) {
    wxCommandEvent evt(tmEVT_EM_DRAW_ESC, wxID_ANY);
    GetEventHandler()->QueueEvent(evt.Clone());
  }

  if (event.GetKeyCode() >= WXK_F1 && event.GetKeyCode() <= WXK_F12) {
    wxCommandEvent evt2(tmEVT_AM_SHORTCUT_PRESSED, wxID_ANY);
    evt2.SetInt(event.GetKeyCode());
    GetEventHandler()->QueueEvent(evt2.Clone());
  }
  event.Skip();
}

void tmRenderer::OnWheelTimer(wxTimerEvent &event) {
  int myWheelRotation = m_WheelRotation;
  m_WheelRotation = 0;

  wxRect myBaseRect(wxPoint(0, 0), this->GetSize());
  wxRect myOriginRect(myBaseRect);
  int myWheelPercent = 10 - myWheelRotation;
  if (myWheelPercent < 1) {
    myWheelPercent = 1;
  }
  myBaseRect.SetWidth(wxRound(myBaseRect.GetWidth() * myWheelPercent / 10.0));
  myBaseRect.SetHeight(wxRound(myBaseRect.GetHeight() * myWheelPercent / 10.0));
  myBaseRect = myBaseRect.CenterIn(myOriginRect, wxBOTH);
  if (myBaseRect.GetSize() == this->GetSize()) {
    // no changes, return
    return;
  }

  // send event
  wxCommandEvent evt;
  evt.SetId(wxID_ANY);
  evt.SetClientData(new wxRect(myBaseRect));
  evt.SetEventType(tmEVT_LM_ZOOM_RECTANGLE_IN);
  GetEventHandler()->QueueEvent(evt.Clone());
}

void tmRenderer::ZoomStart(const wxPoint &mousepos) {
  wxASSERT(m_Rubber == nullptr);
  m_Rubber = new vrRubberBand(this);
  wxASSERT(m_Rubber);
  m_Rubber->SetPointFirst(mousepos);
}

void tmRenderer::ZoomUpdate(wxMouseEvent &event) {
  if (m_Rubber == nullptr) {
    return;
  }

  if (event.Dragging()) {
    m_Rubber->SetPointLast(event.GetPosition());
    m_Rubber->Update();
  }

  // change cursor if needed
  if (m_Rubber->IsPositive() && m_ActualNotStockCursor != tmCURSOR_ZOOM_IN) {
    ChangeCursor(tmTOOL_ZOOM_RECTANGLE_IN);
  }

  if (!m_Rubber->IsPositive() && m_ActualNotStockCursor != tmCURSOR_ZOOM_OUT) {
    ChangeCursor(tmTOOL_ZOOM_RECTANGLE_OUT);
  }
}

void tmRenderer::ZoomStop(const wxPoint &mousepos) {
  if (m_Rubber == nullptr) {
    return;
  }

  m_Rubber->SetPointLast(mousepos);
  if (!m_Rubber->IsValid()) {
    wxDELETE(m_Rubber);
    return;
  }
  bool isPositive = m_Rubber->IsPositive();
  wxRect *mypRect = new wxRect(m_Rubber->GetRect());
  wxDELETE(m_Rubber);

  if (mypRect->IsEmpty()) {
    wxDELETE(mypRect);
    return;
  }

  // send event
  wxCommandEvent evt;
  evt.SetId(wxID_ANY);
  evt.SetClientData(mypRect);
  if (isPositive) {
    evt.SetEventType(tmEVT_LM_ZOOM_RECTANGLE_IN);
  } else {
    evt.SetEventType(tmEVT_LM_ZOOM_RECTANGLE_OUT);
  }
  GetEventHandler()->QueueEvent(evt.Clone());
}

/***************************************************************************/ /**
  @brief Called when selection start (mouse down)
  @param mousepos actual coordinate of the mouse in screen pixels
  @author Lucien Schreiber (c) CREALP 2008
  @date 29 October 2008
  *******************************************************************************/
void tmRenderer::SelectStart(const wxPoint &mousepos) {
  wxASSERT(m_Rubber == nullptr);
  m_Rubber = new vrRubberBand(this);
  wxASSERT(m_Rubber);
  m_Rubber->SetPointFirst(mousepos);
}

/***************************************************************************/ /**
  @brief Called when selection is updated (mouse move)
  @param mousepos actual coordinate of the mouse in screen pixels
  @author Lucien Schreiber (c) CREALP 2008
  @date 29 October 2008
  *******************************************************************************/
void tmRenderer::SelectUpdate(wxMouseEvent &event) {
  if (m_Rubber == nullptr) {
    return;
  }

  if (event.Dragging()) {
    m_Rubber->SetPointLast(event.GetPosition());
    m_Rubber->Update();
  }
}

/***************************************************************************/ /**
  @brief Called when selection finished (mouse up)
  @param mousepos actual coordinate of the mouse in screen pixels
  @author Lucien Schreiber (c) CREALP 2008
  @date 29 October 2008
  *******************************************************************************/
void tmRenderer::SelectStop(const wxPoint &mousepos) {
  if (m_Rubber == nullptr) {
    return;
  }

  bool myShiftDown = m_ShiftDown;
  m_Rubber->SetPointLast(mousepos);
  wxRect *mypRect = nullptr;
  if (!m_Rubber->IsValid()) {
    mypRect = new wxRect(mousepos.x - tmSELECTION_DIAMETER / 2.0, mousepos.y - tmSELECTION_DIAMETER / 2.0,
                         tmSELECTION_DIAMETER, tmSELECTION_DIAMETER);
  } else {
    mypRect = new wxRect(m_Rubber->GetRect());
  }
  wxDELETE(m_Rubber);

  wxCommandEvent evt(tmEVT_LM_SELECTION, wxID_ANY);
  evt.SetInt(static_cast<int>(myShiftDown));
  // do not delete mySelection Rect, will be deleted in the layer manager
  evt.SetClientData(mypRect);
  GetEventHandler()->QueueEvent(evt.Clone());
}

/***************************************************************************/ /**
  @brief Start the pan
  @details Call this function when the mouse is down for initiating the pan
  process.
  This function grab the screen into a bitmap used later (in
  #PanUpdate()) when user moves the mouse.
  @param mousepos wxPoint position of the mouse when user click
  @author Lucien Schreiber (c) CREALP 2008
  @date 31 July 2008
  *******************************************************************************/
void tmRenderer::PanStart(const wxPoint &mousepos) {
  m_StartCoord = mousepos;
  // capture the dc in a bitmap
  wxClientDC dc(this);

  wxSize mybitmapsize = GetClientSize();
  wxSize myFullSize = GetSize();
  int iHeight = mybitmapsize.GetHeight();

  if (m_PanBmp) {
    delete m_PanBmp;
    m_PanBmp = nullptr;
  }

  // FIXME: BUG IN OSX
#ifdef __WXMAC__
  iHeight -= myFullSize.GetHeight() - iHeight;
#endif
  //

  m_PanBmp = new wxBitmap(mybitmapsize.GetWidth(), iHeight, -1);
  wxMemoryDC mdc;
  mdc.SelectObject(*m_PanBmp);
  mdc.Blit(0, 0, mybitmapsize.GetWidth(), mybitmapsize.GetHeight(), &dc, 0, 0);
  mdc.SelectObject(wxNullBitmap);

  // empty real bmp
  SetBitmapStatus();
  m_isPanning = true;
}

/***************************************************************************/ /**
  @brief Update the pan image during mouse move
  @details Call this function when the mouse move for moving the pan image.
  @param mousepos actual mouse position
  @author Lucien Schreiber (c) CREALP 2008
  @date 31 July 2008
  *******************************************************************************/
void tmRenderer::PanUpdate(const wxPoint &mousepos) {
  // compute the new raster origin
  wxPoint myNewPos(mousepos.x - m_StartCoord.x, mousepos.y - m_StartCoord.y);

  if (m_PanBmp == nullptr || !m_PanBmp->IsOk()) return;

  // we move the raster if mouse mouve
  if (myNewPos.x != 0 && myNewPos.y != 0) {
    wxMemoryDC mdc;
    wxSize wndsize = GetClientSize();
    wxBitmap tmpbmp(wndsize.GetWidth(), wndsize.GetHeight());
    mdc.SelectObject(tmpbmp);

    // draw all white
    mdc.SetBrush(wxBrush(*wxWHITE_BRUSH));
    mdc.SetPen(wxPen(*wxWHITE_PEN));
    mdc.DrawRectangle(0, 0, wndsize.GetWidth(), wndsize.GetHeight());
    mdc.DrawBitmap(*m_PanBmp, myNewPos.x, myNewPos.y);
    mdc.SelectObject(wxNullBitmap);

    SetBitmapStatus(&tmpbmp);
    Refresh();
    Update();
  }
}

/***************************************************************************/ /**
  @brief Refresh screen after pan
  @details Call this function when the pan is over to make cleanup and refresh
  screen.
  @param mousepos actual mouse position
  @author Lucien Schreiber (c) CREALP 2008
  @date 31 July 2008
  *******************************************************************************/
void tmRenderer::PanStop(const wxPoint &mousepos) {
  m_isPanning = false;
  if (m_StartCoord == wxPoint(-1, -1)) {
    wxDELETE(m_PanBmp);
    return;
  }

  // compute the new raster origin
  // myNewPos will be deleted in the layermanager
  wxPoint *myNewPos = new wxPoint(mousepos.x - m_StartCoord.x, mousepos.y - m_StartCoord.y);
  if (myNewPos->x == 0 && myNewPos->y == 0) {
    wxDELETE(myNewPos);
    wxDELETE(m_PanBmp);
    m_StartCoord = wxPoint(-1, -1);
    wxLogMessage("Deleted");
    return;
  }

  // send message to renderer with
  // displacement done.
  wxCommandEvent evt(tmEVT_LM_PAN_ENDED, wxID_ANY);
  evt.SetClientData(myNewPos);
  GetEventHandler()->QueueEvent(evt.Clone());

  SetBitmapStatus();

  wxDELETE(m_PanBmp);
  m_StartCoord = wxPoint(-1, -1);
}

void tmRenderer::PanDClick(wxMouseEvent &event) {
  // compute middle pixel
  wxSize myDisplaySize = GetSize();
  if (!myDisplaySize.IsFullySpecified() || myDisplaySize == wxSize(0, 0)) {
    wxLogError("Error getting display size!");
    return;
  }

  wxPoint myMiddlePoint(wxRound(myDisplaySize.GetWidth() / 2.0), wxRound(myDisplaySize.GetHeight() / 2.0));
  // computing moved point
  wxPoint myMovedPoint(myMiddlePoint - event.GetPosition());

  wxCommandEvent evt(tmEVT_LM_PAN_ENDED, wxID_ANY);
  evt.SetClientData(new wxPoint(myMovedPoint));
  GetEventHandler()->QueueEvent(evt.Clone());
}

void tmRenderer::OrientedPtsStart(const wxPoint &mousepos) {
  wxCommandEvent evt(tmEVT_EM_DRAW_ORIENT_DOWN, wxID_ANY);
  wxPoint *myClickedPos = new wxPoint(mousepos.x, mousepos.y);
  evt.SetClientData(myClickedPos);
  GetEventHandler()->QueueEvent(evt.Clone());
}

void tmRenderer::OrientedPtsMove(const wxPoint &mousepos) {
  wxCommandEvent evt(tmEVT_EM_DRAW_ORIENT_MOVE, wxID_ANY);
  wxPoint *myClickedPos = new wxPoint(mousepos.x, mousepos.y);
  evt.SetClientData(myClickedPos);
  GetEventHandler()->QueueEvent(evt.Clone());
}

void tmRenderer::OrientedPtsStop(const wxPoint &mousepos) {
  wxCommandEvent evt(tmEVT_EM_DRAW_ORIENT_UP, wxID_ANY);
  wxPoint *myClickedPos = new wxPoint(mousepos.x, mousepos.y);
  evt.SetClientData(myClickedPos);
  GetEventHandler()->QueueEvent(evt.Clone());
}

void tmRenderer::ModifySharedStart(const wxPoint &mousepos) {
  // sent message to edit manager
  wxCommandEvent evt(tmEVT_EM_MODIFY_SHARED_DOWN, wxID_ANY);
  wxPoint *myClickedPos = new wxPoint(mousepos.x, mousepos.y);
  evt.SetClientData(myClickedPos);
  GetEventHandler()->QueueEvent(evt.Clone());
}

void tmRenderer::ModifySharedStop(const wxPoint &mousepos) {
  // sent message to edit manager
  wxCommandEvent evt(tmEVT_EM_MODIFY_SHARED_UP, wxID_ANY);
  wxPoint *myClickedPos = new wxPoint(mousepos.x, mousepos.y);
  evt.SetClientData(myClickedPos);
  GetEventHandler()->QueueEvent(evt.Clone());
}

void tmRenderer::ModifySharedUpdate(const wxPoint &mousepos) {
  // sent message to edit manager
  wxCommandEvent evt(tmEVT_EM_MODIFY_SHARED_MOVE, wxID_ANY);
  wxPoint *myClickedPos = new wxPoint(mousepos.x, mousepos.y);
  evt.SetClientData(myClickedPos);
  GetEventHandler()->QueueEvent(evt.Clone());
}

/***************************************************************************/ /**
  @brief Click up with cut line tool
  @author Lucien Schreiber (c) CREALP 2009
  @date 05 February 2009
  *******************************************************************************/
void tmRenderer::CutLineClick(const wxPoint &mousepos) {
  wxCommandEvent evt(tmEVT_EM_CUT_LINE, wxID_ANY);
  wxPoint *myClickedPos = new wxPoint(mousepos.x, mousepos.y);
  evt.SetClientData(myClickedPos);
  GetEventHandler()->QueueEvent(evt.Clone());
}

/***************************************************************************/ /**
  @brief Draw a circle using inverse video
  @param pt Coordinate of center
  @param radius Circle radius in px.
  @author Lucien Schreiber (c) CREALP 2009
  @date 24 February 2009
  *******************************************************************************/
void tmRenderer::DrawCircleVideoInverse(wxPoint pt, int radius) {
  wxClientDC dc(this);
  wxDCOverlay myOverlayDC(m_Overlay, &dc);
  myOverlayDC.Clear();

#ifdef __WXMAC__
  dc.SetPen(*wxGREY_PEN);
  dc.SetBrush(wxColour(192, 192, 192, 64));
#else
  dc.SetPen(wxPen(*wxLIGHT_GREY, 2, wxPENSTYLE_SOLID));
  dc.SetBrush(*wxTRANSPARENT_BRUSH);
#endif
  dc.DrawCircle(pt, radius);
}

void tmRenderer::DrawCircleVideoInverseClean() {
  {
    wxClientDC dc(this);
    wxDCOverlay overlaydc(m_Overlay, &dc);
    overlaydc.Clear();
  }
  m_Overlay.Reset();
}
