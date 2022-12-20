/***************************************************************************
 tmrenderer.h
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

#ifndef _TM_GISRENDERER_H_
#define _TM_GISRENDERER_H_

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/dcbuffer.h>
#include <wx/dcgraph.h>
#include <wx/overlay.h>
#include <wx/scrolwin.h>  // for scrolled window base

#include "../core/tmarraysize.h"  // for array of wxSize items
#include "tmgisscale.h"           // for number of division;

const wxString TMRENDERER_WINDOW_NAME = wxTRANSLATE("TMRENDERER_WINDOW");

class vrRubberBand;

// EVENT FOR GIS RENDERER CTRL
DECLARE_EVENT_TYPE(tmEVT_LM_SIZE_CHANGED, -1)
DECLARE_EVENT_TYPE(tmEVT_LM_MOUSE_MOVED, -1)
DECLARE_EVENT_TYPE(tmEVT_LM_ZOOM_RECTANGLE_OUT, -1)
DECLARE_EVENT_TYPE(tmEVT_LM_ZOOM_RECTANGLE_IN, -1)
DECLARE_EVENT_TYPE(tmEVT_LM_PAN_ENDED, -1)
DECLARE_EVENT_TYPE(tmEVT_LM_SELECTION, -1)
DECLARE_EVENT_TYPE(tmEVT_EM_MODIFY_CLICK, -1)
DECLARE_EVENT_TYPE(tmEVT_EM_DRAW_ENTER, -1)
DECLARE_EVENT_TYPE(tmEVT_EM_CUT_LINE, -1)
DECLARE_EVENT_TYPE(tmEVT_AM_SHORTCUT_PRESSED, -1)
DECLARE_EVENT_TYPE(tmEVT_EM_MODIFY_MOVED, -1)
DECLARE_EVENT_TYPE(tmEVT_EM_MODIFY_UP, -1)
DECLARE_EVENT_TYPE(tmEVT_EM_DRAW_CLICK, -1)
DECLARE_EVENT_TYPE(tmEVT_EM_DRAW_MOVE, -1)
DECLARE_EVENT_TYPE(tmEVT_EM_DRAW_DOWN, -1)
DECLARE_EVENT_TYPE(tmEVT_EM_DRAW_ESC, -1)
DECLARE_EVENT_TYPE(tmEVT_EM_MODIFY_MENU, -1)
DECLARE_EVENT_TYPE(tmEVT_TM_UPDATE_TOOL_VIEW, -1)
DECLARE_EVENT_TYPE(tmEVT_EM_DRAW_ORIENT_DOWN, -1)
DECLARE_EVENT_TYPE(tmEVT_EM_DRAW_ORIENT_MOVE, -1)
DECLARE_EVENT_TYPE(tmEVT_EM_DRAW_ORIENT_UP, -1)
DECLARE_EVENT_TYPE(tmEVT_EM_MODIFY_SHARED_DOWN, -1)
DECLARE_EVENT_TYPE(tmEVT_EM_MODIFY_SHARED_UP, -1)
DECLARE_EVENT_TYPE(tmEVT_EM_MODIFY_SHARED_MOVE, -1)

enum tmGIS_TOOL {
    tmTOOL_SELECT = 0,
    tmTOOL_ZOOM_RECTANGLE_IN,
    tmTOOL_ZOOM_RECTANGLE_OUT,
    tmTOOL_PAN,
    tmTOOL_DRAW,
    tmTOOL_DRAW_BEZIER,
    tmTOOL_MODIFY,
    tmTOOL_MODIFY_BEZIER,
    tmTOOL_CUT_LINES,
    tmTOOL_ORIENTED_POINTS,
    tmTOOL_MODIFY_SHARED,
    tmTOOL_VERTEX_DELETE,
    tmTOOL_VERTEX_INSERT,
    tmTOOL_ZOOM_RECTANGLE = tmTOOL_ZOOM_RECTANGLE_IN
};

enum tmGIS_CURSOR {
    tmCURSOR_ZOOM_IN,
    tmCURSOR_ZOOM_OUT,
    tmCURSOR_HAND,
    tmCURSOR_EDIT,
    tmCURSOR_ORIENTED,
    tmCURSOR_VERTEX_ADD,
    tmCURSOR_VERTEX_REMOVE
};

// parameter : size of selection in pixels
// sould be able to divide it by two
const int tmSELECTION_DIAMETER = 10;

class tmEditManager;

class tmToolManager;

class tmRenderer : public wxScrolledWindow {
  private:
    wxBitmap* m_bmp;
    tmGIS_TOOL m_ActualTool;
    tmGIS_CURSOR m_ActualNotStockCursor;

    tmEditManager* m_EditManager;
    tmToolManager* m_ToolManager;

    wxSize m_OldSize;

    // rubber band
    wxPoint m_StartCoord;
    wxBitmap* m_PanBmp;
    vrRubberBand* m_Rubber;
    wxOverlay m_Overlay;

    bool m_ShiftDown;

    bool m_ModifyCalled;
    bool m_DrawCalled;

    int m_WheelRotation;
    wxPoint m_WheelPosition;
    wxTimer m_WheelTimer;

    // changing cursors
    wxCursor LoadCursorFromBitmap(tmGIS_CURSOR cursor);

    void ChangeCursor(const tmGIS_TOOL& selected_tool);

    // mouse event function
    void OnMouseDown(wxMouseEvent& event);

    // void OnMouseRightDown (wxMouseEvent & event);
    void OnMouseMove(wxMouseEvent& event);

    void OnMouseUp(wxMouseEvent& event);

    void OnMouseCaptureLost(wxMouseEvent& event);

    void OnMouseDClick(wxMouseEvent& event);

    void OnMouseWheel(wxMouseEvent& event);

    void OnShiftDown(wxKeyEvent& event);

    void OnShiftUp(wxKeyEvent& event);

    void OnKey(wxKeyEvent& event);

    void OnWheelTimer(wxTimerEvent& event);

    void OnAvoidFlickering(wxEraseEvent& event);

    // bitmap functions
    bool BitmapUpdateSize();

    bool BitmapSetToWhite();

    bool BitmapCopyInto(wxBitmap* bmp);

    bool m_isPanning;
    DECLARE_EVENT_TABLE()

  protected:
    // rubber band functions
    void ZoomStart(const wxPoint& mousepos);

    void ZoomUpdate(wxMouseEvent& event);

    void ZoomStop(const wxPoint& mousepos);

    // selecting function
    void SelectStart(const wxPoint& mousepos);

    void SelectUpdate(wxMouseEvent& event);

    void SelectStop(const wxPoint& mousepos);

    // panning function
    void PanStart(const wxPoint& mousepos);

    void PanUpdate(const wxPoint& mousepos);

    void PanStop(const wxPoint& mousepos);

    void PanDClick(wxMouseEvent& event);

    // oriented pts functions
    void OrientedPtsStart(const wxPoint& mousepos);

    void OrientedPtsMove(const wxPoint& mousepos);

    void OrientedPtsStop(const wxPoint& mousepos);

    // modify shared function
    void ModifySharedStart(const wxPoint& mousepos);

    void ModifySharedStop(const wxPoint& mousepos);

    void ModifySharedUpdate(const wxPoint& mousepos);

    void CutLineClick(const wxPoint& mousepos);

  public:
    tmRenderer(wxWindow* parent, wxWindowID id);

    ~tmRenderer();

    void OnSizeChange(wxSizeEvent& event);

    void InitSize();

    void OnPaint(wxPaintEvent& event);

    void SetBitmapStatus(wxBitmap* bmp = nullptr);  //{m_bmp = bmp;}

    void SetTool(tmGIS_TOOL selected_tool);

    tmGIS_TOOL GetTool() {
        return m_ActualTool;
    }

    // get bitmap
    wxBitmap* GetBitmap() {
        return m_bmp;
    }

    // drawing
    void DrawCircleVideoInverse(wxPoint pt, int radius);

    void DrawCircleVideoInverseClean();

    // editing
    void StopModifyEvent() {
        m_ModifyCalled = false;
    }

    void SetEditManagerRef(tmEditManager* manager) {
        m_EditManager = manager;
    }

    void SetToolManagerRef(tmToolManager* manager) {
        m_ToolManager = manager;
    }
};

#endif
