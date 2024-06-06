/***************************************************************************
 tmeditmanager.cpp
 Deals with editing data in construction layers
 -------------------
 copyright            : (C) 2007 CREALP Lucien Schreiber
 email                : lucien.schreiber a"t crealp dot vs dot ch
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "tmeditmanager.h"

#include "../core/agg_curves.h"
#include "../core/tmstatsevent.h"
#include "../core/toolmap.h"
#include "../database/database_tm.h"  // for database
#include "tmrenderer.h"               // for GIS rendering
#include "tmsymbolvectorline.h"
#include "tmsymbolvectorlinemultiple.h"
#include "tmsymbolvectorpoint.h"
#include "tmsymbolvectorpolygon.h"

DEFINE_EVENT_TYPE(tmEVT_FOCUS_RENDERER);

BEGIN_EVENT_TABLE(tmEditManager, wxEvtHandler)
EVT_COMMAND(wxID_ANY, tmEVT_EM_EDIT_START, tmEditManager::OnEditStart)
EVT_COMMAND(wxID_ANY, tmEVT_EM_EDIT_STOP, tmEditManager::OnEditStop)
EVT_COMMAND(wxID_ANY, tmEVT_EM_DRAW_ENTER, tmEditManager::OnDrawFeatureValidate)
EVT_COMMAND(wxID_ANY, tmEVT_EM_CUT_LINE, tmEditManager::OnCutLines)
EVT_COMMAND(wxID_ANY, tmEVT_EV_DISPLAY_VERTEX_COORD, tmEditManager::OnShowVertexPosition)
EVT_COMMAND(wxID_ANY, tmEVT_EM_DRAW_ESC, tmEditManager::OnDrawFeatureEscape)
EVT_COMMAND(wxID_ANY, tmEVT_FOCUS_RENDERER, tmEditManager::OnSetRenderFocus)

EVT_COMMAND(wxID_ANY, tmEVT_EM_DRAW_ORIENT_DOWN, tmEditManager::OnOrientedPtsDown)
EVT_COMMAND(wxID_ANY, tmEVT_EM_DRAW_ORIENT_MOVE, tmEditManager::OnOrientedPtsMove)
EVT_COMMAND(wxID_ANY, tmEVT_EM_DRAW_ORIENT_UP, tmEditManager::OnOrientedPtsUp)

EVT_COMMAND(wxID_ANY, tmEVT_EM_MODIFY_SHARED_DOWN, tmEditManager::OnEditSharedDown)
EVT_COMMAND(wxID_ANY, tmEVT_EM_MODIFY_SHARED_UP, tmEditManager::OnEditSharedUp)
EVT_COMMAND(wxID_ANY, tmEVT_EM_MODIFY_SHARED_MOVE, tmEditManager::OnEditSharedMove)

EVT_MENU(tmEM_CONTEXTMENU_LINE_CANCEL, tmEditManager::OnDrawFeatureEscape)
EVT_MENU(tmEM_CONTEXTMENU_LINE_SAVE, tmEditManager::OnDrawFeatureValidate)
EVT_MENU(tmEM_CONTEXTMENU_VERTEX_INSERT, tmEditManager::OnMenuInsertVertex)
EVT_MENU(tmEM_CONTEXTMENU_VERTEX_DELETE, tmEditManager::OnMenuDeleteVertex)
END_EVENT_TABLE()

/***************************************************************************/ /**
  @brief Constructor
  @author Lucien Schreiber (c) CREALP 2009
  @date 26 January 2009
  *******************************************************************************/
tmEditManager::tmEditManager(ToolMapFrame* parent, TocCtrl* toc, tmSelectedDataMemory* seldata, tmRenderer* renderer,
                             tmGISScale* scale) {
    InitMemberValues();
    m_ParentEvt = parent;
    m_TocCtrl = toc;
    m_SelectedData = seldata;
    m_Renderer = renderer;
    m_Scale = scale;
    m_EditStarted = false;
    m_DrawLine.SetSymbology(*wxBLACK, 1);
    m_SelectionColour = *wxRED;

    m_ParentEvt->PushEventHandler(this);

    // m_GISMemory = new tmGISDataVectorMemory();

    m_BezierActualP1 = wxPoint(0, 0);
    m_BezierActualP2 = wxPoint(0, 0);
    m_BezierActualC1 = wxPoint(0, 0);
    m_BezierActualC2 = wxPoint(0, 0);
    m_BezierDrawControlPoints = false;
    m_BezierRefreshRect = wxRect(wxDefaultPosition, wxDefaultSize);
    m_BezierModifyIndexPoint = wxNOT_FOUND;
    m_BezierModifyIndexControl = wxNOT_FOUND;
    m_BezierSnappedPointsIndexes.Clear();
    m_BezierModifyControlInverted = false;
    // m_BezierApproximationScale = 0.5;

    m_ArcActualPt = wxDefaultPosition;
    m_ArcRefreshRect = wxRect(wxDefaultPosition, wxDefaultSize);
    m_ArcModifyIndexPoint = wxNOT_FOUND;
    m_ArcOID = wxNOT_FOUND;
}

/***************************************************************************/ /**
  @brief Destructor
  @author Lucien Schreiber (c) CREALP 2009
  @date 26 January 2009
  *******************************************************************************/
tmEditManager::~tmEditManager() {
    m_ParentEvt->PopEventHandler(false);
    // m_ParentEvt->SetEventHandler(m_ParentEvt);
}

/***************************************************************************/ /**
  @brief Init pointer to default values
  @author Lucien Schreiber (c) CREALP 2009
  @date 26 January 2009
  *******************************************************************************/
void tmEditManager::InitMemberValues() {
    m_ParentEvt = nullptr;
    m_TocCtrl = nullptr;
    m_SelectedData = nullptr;
    m_Renderer = nullptr;
    m_pDB = nullptr;
    m_SnapMem = nullptr;
    m_INSDELVertex = wxNOT_FOUND;
    m_INSVertexPos = wxRealPoint(-1, -1);
    m_SnappingShowOnMap = false;
}

/***************************************************************************/ /**
  @brief User select edit tool
  @author Lucien Schreiber (c) CREALP 2009
  @date 26 January 2009
  *******************************************************************************/
void tmEditManager::OnToolEdit() {
    wxASSERT(m_Renderer);
    BezierClear();
    m_Renderer->SetTool(tmTOOL_DRAW);
    m_ArcActualPt = wxDefaultPosition;
    m_Renderer->Refresh();
    m_Renderer->Update();
}

void tmEditManager::OnToolBezier() {
    ArcClear();
    m_Renderer->SetTool(tmTOOL_DRAW_BEZIER);
    m_BezierActualP2 = wxPoint(0, 0);
    m_Renderer->Refresh();
    m_Renderer->Update();
}

void tmEditManager::OnToolBezierModify() {
    m_Renderer->SetTool(tmTOOL_MODIFY_BEZIER);
    m_BezierActualP2 = wxPoint(0, 0);
    m_Renderer->Refresh();
    m_Renderer->Update();
}

void tmEditManager::BezierClick(const wxPoint& mousepos) {
    // add point to Bezier
    if (m_BezierPoints.GetCount() == m_BezierPointsControl.GetCount()) {
        wxRealPoint myPt = m_Scale->PixelToReal(mousepos);
        if (EMGetSnappingCoord(myPt)) {
            m_BezierSnappedPointsIndexes.Add(m_BezierPoints.GetCount());
        }

        m_BezierPoints.push_back(new wxRealPoint(myPt));
        m_BezierActualP2 = mousepos;
        m_BezierDrawControlPoints = true;
    }

    // add control point to Bezier
    else {
        wxPoint myP2 = m_Scale->RealToPixel(*m_BezierPoints.back());
        m_BezierActualC2 = myP2 - (mousepos - myP2);
        if (m_BezierPointsControl.GetCount() == 0) {
            m_BezierPointsControl.push_back(new wxRealPoint(m_Scale->PixelToReal(mousepos)));
        } else {
            m_BezierPointsControl.push_back(new wxRealPoint(m_Scale->PixelToReal(m_BezierActualC2)));
        }
        m_BezierDrawControlPoints = false;
    }
    m_Renderer->Refresh();
    m_Renderer->Update();
}

void tmEditManager::BezierMove(const wxPoint& mousepos) {
    if (m_BezierPoints.GetCount() == 0) {
        return;
    }

    // move control point
    if (m_BezierPoints.GetCount() > m_BezierPointsControl.GetCount()) {
        m_BezierActualP2 = m_Scale->RealToPixel(*m_BezierPoints.back());
        m_BezierActualC2 = m_BezierActualP2 - (mousepos - m_BezierActualP2);
    }

    // move point
    else {
        m_BezierActualP1 = m_Scale->RealToPixel(*m_BezierPoints.back());
        if (m_BezierPointsControl.GetCount() > 0) {
            if (m_BezierPointsControl.GetCount() == 1) {
                m_BezierActualC1 = m_Scale->RealToPixel(*m_BezierPointsControl.front());
            } else {
                wxPoint myInvertedC1 = m_Scale->RealToPixel(*m_BezierPointsControl.back());
                m_BezierActualC1 = m_BezierActualP1 - (myInvertedC1 - m_BezierActualP1);
            }
        }
        m_BezierActualP2 = mousepos;
        m_BezierActualC2 = mousepos;
    }

    m_Renderer->RefreshRect(m_BezierRefreshRect);
    m_Renderer->Update();
}

void tmEditManager::BezierDraw(wxGCDC* dc) {
    if (m_BezierPoints.GetCount() == 0) {
        return;
    }
    if (m_Renderer->GetTool() == tmTOOL_MODIFY_BEZIER) {
        return;
    }

    tmLayerProperties* myLayerProperties = m_TocCtrl->GetEditLayer();
    wxASSERT(myLayerProperties);
    auto* mySymbol = dynamic_cast<tmSymbolVectorLine*>(myLayerProperties->GetSymbolRef());
    wxASSERT(mySymbol);

    // draw existing bezier
    dc->SetPen(wxPen(m_SelectionColour, mySymbol->GetWidth()));
    wxGraphicsPath path = dc->GetGraphicsContext()->CreatePath();
    wxPoint myFirstPt(m_Scale->RealToPixel(*m_BezierPoints[0]));
    path.MoveToPoint(myFirstPt);
    for (unsigned int i = 1; i < m_BezierPointsControl.GetCount(); i++) {
        wxPoint myLastCPt(
            m_Scale->RealToPixel(*m_BezierPoints[i - 1]) -
            (m_Scale->RealToPixel(*m_BezierPointsControl[i - 1]) - m_Scale->RealToPixel(*m_BezierPoints[i - 1])));
        if (i == 1) {
            myLastCPt = m_Scale->RealToPixel(*m_BezierPointsControl[i - 1]);
        }
        wxPoint myPt(m_Scale->RealToPixel(*m_BezierPoints[i]));
        wxPoint myCPt1(m_Scale->RealToPixel(*m_BezierPointsControl[i]));
        path.AddCurveToPoint(myLastCPt, myCPt1, myPt);
    }
    dc->GetGraphicsContext()->StrokePath(path);

    // draw nodes with different color based on snapping status
    int myNodeWidth = 2.0 * mySymbol->GetWidth();
    if (myNodeWidth < 4) {
        myNodeWidth = 4;
    }

    wxPen myNodeBlackPen = wxPen(*wxBLACK, myNodeWidth);
    wxPen myNodeGreenPen = wxPen(*wxGREEN, myNodeWidth);
    for (unsigned int i = 0; i < m_BezierPointsControl.GetCount(); i++) {
        if (m_BezierSnappedPointsIndexes.Index(i) == wxNOT_FOUND) {
            dc->SetPen(myNodeBlackPen);
        } else {
            dc->SetPen(myNodeGreenPen);
        }

        wxPoint myPt(m_Scale->RealToPixel(*m_BezierPoints[i]));
#ifdef __WXMSW__
        dc->DrawLine(myPt.x, myPt.y, myPt.x + 1, myPt.y + 1);
#else
        dc->DrawLine(myPt.x, myPt.y, myPt.x, myPt.y);
#endif
    }

#ifdef __WXMAC__
    dc->SetPen(*wxGREY_PEN);
#else
    dc->SetPen(wxPen(*wxLIGHT_GREY, 2, wxPENSTYLE_SOLID));
#endif
    // draw actual bezier when needed
    if (m_BezierActualP1 != wxPoint(0, 0) && m_BezierActualC1 != wxPoint(0, 0) && m_BezierActualP2 != wxPoint(0, 0)) {
        wxGraphicsPath path = dc->GetGraphicsContext()->CreatePath();
        path.MoveToPoint(m_BezierActualP1);
        path.AddCurveToPoint(m_BezierActualC1, m_BezierActualC2, m_BezierActualP2);
        dc->GetGraphicsContext()->StrokePath(path);
    }

    // draw bezier control when needed
    if (m_BezierDrawControlPoints && m_BezierActualC2 != wxPoint(0, 0)) {
        dc->DrawLine(m_BezierActualP2, m_BezierActualC2);
        wxPoint myInvertedC2 = m_BezierActualP2 - (m_BezierActualC2 - m_BezierActualP2);
        dc->DrawLine(m_BezierActualP2, myInvertedC2);

        dc->SetPen(wxPen(*wxBLACK, 4));
#ifdef __WXMSW__
        dc->DrawLine(m_BezierActualC2.x, m_BezierActualC2.y, m_BezierActualC2.x + 1, m_BezierActualC2.y + 1);
        dc->DrawLine(m_BezierActualP2.x, m_BezierActualP2.y, m_BezierActualP2.x + 1, m_BezierActualP2.y + 1);
        dc->DrawLine(myInvertedC2.x, myInvertedC2.y, myInvertedC2.x + 1, myInvertedC2.y + 1);
#else
        dc->DrawLine(m_BezierActualC2.x, m_BezierActualC2.y, m_BezierActualC2.x, m_BezierActualC2.y);
        dc->DrawLine(m_BezierActualP2.x, m_BezierActualP2.y, m_BezierActualP2.x, m_BezierActualP2.y);
        dc->DrawLine(myInvertedC2.x, myInvertedC2.y, myInvertedC2.x, myInvertedC2.y);
#endif
    }

    // compute bounding box for refreshing. This is mainly to avoid flickering
    dc->ResetBoundingBox();
    if (m_BezierActualP1 != wxPoint(0, 0) && m_BezierActualC1 != wxPoint(0, 0)) {
        dc->CalcBoundingBox(m_BezierActualP1.x, m_BezierActualP1.y);
        dc->CalcBoundingBox(m_BezierActualC1.x, m_BezierActualC1.y);
        // inverted C1 is never needed for bounding box
    }
    dc->CalcBoundingBox(m_BezierActualP2.x, m_BezierActualP2.y);
    dc->CalcBoundingBox(m_BezierActualC2.x, m_BezierActualC2.y);
    dc->CalcBoundingBox(m_BezierActualP2.x - (m_BezierActualC2.x - m_BezierActualP2.x),
                        m_BezierActualP2.y - (m_BezierActualC2.y - m_BezierActualP2.y));
    m_BezierRefreshRect = wxRect(wxPoint(dc->MinX(), dc->MaxY()), wxPoint(dc->MaxX(), dc->MinY()));
    m_BezierRefreshRect.Inflate(wxSize(3, 3));
}

void tmEditManager::BezierClear() {
    m_BezierActualP1 = wxPoint(0, 0);
    m_BezierActualP2 = wxPoint(0, 0);
    m_BezierActualC1 = wxPoint(0, 0);
    m_BezierActualC2 = wxPoint(0, 0);
    m_BezierDrawControlPoints = false;

    m_BezierPoints.DeleteContents(true);
    m_BezierPointsControl.DeleteContents(true);
    m_BezierPoints.Clear();
    m_BezierPointsControl.Clear();

    m_BezierRefreshRect = wxRect(wxDefaultPosition, wxDefaultSize);
    m_BezierSnappedPointsIndexes.Clear();
}

void tmEditManager::ArcClear() {
    m_ArcActualPt = wxDefaultPosition;
    m_ArcPoints.DeleteContents(true);
    m_ArcPoints.Clear();

    m_ArcRefreshRect = wxRect(wxDefaultPosition, wxDefaultSize);
    m_ArcSnappedPointsIndexes.Clear();
    m_ArcModifyIndexPoint = wxNOT_FOUND;
    m_ArcOID = wxNOT_FOUND;
}

void tmEditManager::ArcModifyClickDown(const wxPoint& mousepos) {
    m_ArcModifyIndexPoint = wxNOT_FOUND;

    if (m_ArcPoints.GetCount() == 0) {
        if (_LoadSnappingStatus()) {
            m_Renderer->Refresh();
            m_Renderer->Update();
        }
    }

    m_ArcActualPt = mousepos;
    wxRect myRect(0, 0, 7, 7);
    myRect = myRect.CentreIn(wxRect(m_ArcActualPt, wxSize(0, 0)));

    for (unsigned int i = 0; i < m_ArcPoints.GetCount(); i++) {
        wxPoint myPt = m_Scale->RealToPixel(*m_ArcPoints[i]);
        if (myRect.Contains(myPt)) {
            m_ArcModifyIndexPoint = i;
            return;
        }
    }
}

void tmEditManager::ArcModifyClickMove(const wxPoint& mousepos) {
    if (m_ArcModifyIndexPoint == wxNOT_FOUND) {
        return;
    }

    *m_ArcPoints[m_ArcModifyIndexPoint] = m_Scale->PixelToReal(mousepos);
    m_Renderer->RefreshRect(m_ArcRefreshRect);
    m_Renderer->Update();
}

void tmEditManager::ArcModifyClickUp(const wxPoint& mousepos) {
    if (m_ArcModifyIndexPoint != wxNOT_FOUND) {
        wxRealPoint myPt = *m_ArcPoints[m_ArcModifyIndexPoint];
        int myPtSnappingIndex = m_ArcSnappedPointsIndexes.Index(m_ArcModifyIndexPoint);

        if (EMGetSnappingCoord(myPt)) {
            if (myPtSnappingIndex == wxNOT_FOUND) {
                m_ArcSnappedPointsIndexes.Add(m_ArcModifyIndexPoint);
            }
            *m_ArcPoints[m_ArcModifyIndexPoint] = myPt;
        } else {
            if (myPtSnappingIndex != wxNOT_FOUND) {
                m_ArcSnappedPointsIndexes.RemoveAt(myPtSnappingIndex);
            }
        }
    }

    m_ArcActualPt = wxDefaultPosition;
    m_ArcModifyIndexPoint = wxNOT_FOUND;

    if (IsLayerSpatialType(LAYER_SPATIAL_POINT)) {
        wxCommandEvent evt;
        OnDrawFeatureValidate(evt);
    }

    m_Renderer->Refresh();
    m_Renderer->Update();
}

void tmEditManager::ArcVertexInsertUp(const wxPoint& mousepos) {
    ArcClear();
    m_ArcOID = m_SelectedData->GetSelectedUnique();
    if (m_ArcOID == wxNOT_FOUND) {
        return;
    }

    if (m_TocCtrl->GetEditLayer()->GetSpatialType() != LAYER_SPATIAL_LINE) {
        return;
    }

    wxRect myRect(0, 0, 5, 5);
    myRect = myRect.CentreIn(wxRect(mousepos, wxSize(0, 0)));
    OGRLineString myRectDiagonal;
    wxRealPoint myTopLeft = m_Scale->PixelToReal(myRect.GetLeftTop());
    wxRealPoint myBottomRight = m_Scale->PixelToReal(myRect.GetBottomRight());
    myRectDiagonal.addPoint(myTopLeft.x, myTopLeft.y);
    myRectDiagonal.addPoint(myBottomRight.x, myBottomRight.y);

    tmLayerProperties* myLayerProperties = m_TocCtrl->GetEditLayer();
    wxASSERT(m_pDB);
    OGRGeometry* myGeometry = m_pDB->GeometryLoad(m_ArcOID, myLayerProperties->GetType());
    if (myGeometry == nullptr) {
        return;
    }
    OGRLineString* myLine = static_cast<OGRLineString*>(myGeometry);
    wxASSERT(myLine);

    // Check that there are no unfinished other operation (avoid duplicating the nodes).
    if (m_ArcPoints.GetCount() > 0) {
        return;
    }

    OGRPoint p1;
    OGRPoint p2;
    OGRLineString segment;
    OGRLineString myLineModified;
    bool bFoundIntersection = false;
    for (int i = 1; i < myLine->getNumPoints(); i++) {
        myLine->getPoint(i - 1, &p1);
        myLine->getPoint(i, &p2);
        segment.addPoint(&p1);
        segment.addPoint(&p2);

        m_ArcPoints.push_back(new wxRealPoint(p1.getX(), p1.getY()));
        // ensure a point is inserted only once.
        if (segment.Intersects(&myRectDiagonal) && !bFoundIntersection) {
            OGRPoint* myPtToInsert = static_cast<OGRPoint*>(segment.Intersection(&myRectDiagonal));
            wxASSERT(myPtToInsert);
            m_ArcPoints.push_back(new wxRealPoint(myPtToInsert->getX(), myPtToInsert->getY()));
            bFoundIntersection = true;
        }
        segment.empty();
    }
    m_ArcPoints.push_back(
        new wxRealPoint(myLine->getX(myLine->getNumPoints() - 1), myLine->getY(myLine->getNumPoints() - 1)));

    // update the line in the database and refresh screen
    if (bFoundIntersection) {
        _SaveToDatabase();
    }

    OGRGeometryFactory::destroyGeometry(myGeometry);
    ArcClear();

    wxCommandEvent evt2(tmEVT_LM_UPDATE, wxID_ANY);
    m_ParentEvt->GetEventHandler()->QueueEvent(evt2.Clone());
}

void tmEditManager::ArcVertexDeleteUp(const wxPoint& mousePos) {
    m_ArcOID = m_SelectedData->GetSelectedUnique();
    if (m_ArcOID == wxNOT_FOUND) {
        return;
    }

    if (m_TocCtrl->GetEditLayer()->GetSpatialType() != LAYER_SPATIAL_LINE) {
        return;
    }

    wxRect myRect(0, 0, 5, 5);
    myRect = myRect.CentreIn(wxRect(mousePos, wxSize(0, 0)));

    tmLayerProperties* myLayerProperties = m_TocCtrl->GetEditLayer();
    wxASSERT(m_pDB);
    OGRGeometry* myGeometry = m_pDB->GeometryLoad(m_ArcOID, myLayerProperties->GetType());
    if (myGeometry == nullptr) {
        return;
    }

    // Check that there are no unfinished other operation (avoid duplicating the nodes).
    if (m_ArcPoints.GetCount() > 0) {
        return;
    }

    bool bVertexDeleted = false;
    OGRLineString* myLine = static_cast<OGRLineString*>(myGeometry);
    for (int i = 0; i < myLine->getNumPoints(); i++) {
        wxPoint myPt = m_Scale->RealToPixel(wxRealPoint(myLine->getX(i), myLine->getY(i)));
        if (myRect.Contains(myPt) && !bVertexDeleted) {
            bVertexDeleted = true;
            continue;
        }
        m_ArcPoints.push_back(new wxRealPoint(myLine->getX(i), myLine->getY(i)));
    }

    if (m_ArcPoints.GetCount() >= 2) {
        _SaveToDatabase();
    }

    OGRGeometryFactory::destroyGeometry(myGeometry);
    ArcClear();

    wxCommandEvent evt2(tmEVT_LM_UPDATE, wxID_ANY);
    m_ParentEvt->GetEventHandler()->QueueEvent(evt2.Clone());
}

void tmEditManager::BezierModifyDraw(wxGCDC* dc) {
    if (m_Renderer == nullptr) {
        return;
    }
    if (m_Renderer->GetTool() != tmTOOL_MODIFY_BEZIER) {
        return;
    }
    if (m_BezierPoints.GetCount() < 2) {
        return;
    }
    wxASSERT(dc);

    tmLayerProperties* myLayerProperties = m_TocCtrl->GetEditLayer();
    if (!myLayerProperties) return;
    tmSymbolVectorLine* mySymbol = static_cast<tmSymbolVectorLine*>(myLayerProperties->GetSymbolRef());

    // draw existing bezier
    dc->SetPen(wxPen(m_SelectionColour, mySymbol->GetWidth()));
    wxGraphicsPath path = dc->GetGraphicsContext()->CreatePath();
    wxPoint myFirstPt(m_Scale->RealToPixel(*m_BezierPoints[0]));
    path.MoveToPoint(myFirstPt);
    for (unsigned int i = 1; i < m_BezierPointsControl.GetCount(); i++) {
        wxPoint myLastCPt(
            m_Scale->RealToPixel(*m_BezierPoints[i - 1]) -
            (m_Scale->RealToPixel(*m_BezierPointsControl[i - 1]) - m_Scale->RealToPixel(*m_BezierPoints[i - 1])));
        if (i == 1) {
            myLastCPt = m_Scale->RealToPixel(*m_BezierPointsControl[i - 1]);
        }
        wxPoint myPt(m_Scale->RealToPixel(*m_BezierPoints[i]));
        wxPoint myCPt1(m_Scale->RealToPixel(*m_BezierPointsControl[i]));
        path.AddCurveToPoint(myLastCPt, myCPt1, myPt);
    }
    dc->GetGraphicsContext()->StrokePath(path);

    // draw nodes
    int myNodeWidth = 2.0 * mySymbol->GetWidth();
    if (myNodeWidth < 4) {
        myNodeWidth = 4;
    }
    wxPen myNodeBlackPen = wxPen(*wxBLACK, myNodeWidth);
    wxPen myNodeGreenPen = wxPen(*wxGREEN, myNodeWidth);

    for (unsigned int i = 0; i < m_BezierPointsControl.GetCount(); i++) {
        if (m_BezierSnappedPointsIndexes.Index(i) == wxNOT_FOUND) {
            dc->SetPen(myNodeBlackPen);
        } else {
            dc->SetPen(myNodeGreenPen);
        }

        wxPoint myPt(m_Scale->RealToPixel(*m_BezierPoints[i]));
#ifdef __WXMSW__
        dc->DrawLine(myPt.x, myPt.y, myPt.x + 1, myPt.y + 1);
#else
        dc->DrawLine(myPt.x, myPt.y, myPt.x, myPt.y);
#endif
    }

    // draw controls
    wxPen myGreyPen(wxPen(*wxLIGHT_GREY, 2, wxPENSTYLE_SOLID));
#ifdef __WXMAC__
    myGreyPen = wxPen(*wxGREY_PEN);
#endif

    wxASSERT(m_BezierPointsControl.GetCount() == m_BezierPoints.GetCount());
    for (unsigned int i = 0; i < m_BezierPoints.GetCount(); i++) {
        wxPoint myPoint = m_Scale->RealToPixel(*m_BezierPoints[i]);
        wxPoint myControl = m_Scale->RealToPixel(*m_BezierPointsControl[i]);
        wxPoint myControlInverted = myPoint - (myControl - myPoint);

        dc->SetPen(myGreyPen);
        dc->DrawLine(myPoint, myControl);

        dc->SetPen(myNodeBlackPen);
#ifdef __WXMSW__
        dc->DrawLine(myControl.x, myControl.y, myControl.x + 1, myControl.y + 1);
#else
        dc->DrawLine(myControl.x, myControl.y, myControl.x, myControl.y);
#endif

        // special case for first and last loop. We draw only one
        // control instead of two.
        if (i == 0 || i == m_BezierPoints.GetCount() - 1) {
            continue;
        }
        dc->SetPen(myGreyPen);
        dc->DrawLine(myPoint, myControlInverted);

        dc->SetPen(myNodeBlackPen);
#ifdef __WXMSW__
        dc->DrawLine(myControlInverted.x, myControlInverted.y, myControlInverted.x + 1, myControlInverted.y + 1);
#else
        dc->DrawLine(myControlInverted.x, myControlInverted.y, myControlInverted.x, myControlInverted.y);
#endif
    }

    if (m_BezierModifyIndexControl == wxNOT_FOUND && m_BezierModifyIndexPoint == wxNOT_FOUND) {
        return;
    }

    // Compute Refreshing Rect
    dc->ResetBoundingBox();
    wxPointList myPoints;

    int myIndex = m_BezierModifyIndexControl;
    if (m_BezierModifyIndexControl == wxNOT_FOUND) {
        wxASSERT(m_BezierModifyIndexPoint != wxNOT_FOUND);
        myIndex = m_BezierModifyIndexPoint;
    }

    wxPoint myPt = m_Scale->RealToPixel(*m_BezierPoints[myIndex]);
    wxPoint myControl = m_Scale->RealToPixel(*m_BezierPointsControl[myIndex]);
    wxPoint myControlInverted = myPt - (myControl - myPt);
    myPoints.push_back(new wxPoint(myPt));

    if (myIndex != 0 && myIndex != m_BezierPoints.GetCount() - 1) {
        myPoints.push_back(new wxPoint(myControl));
        myPoints.push_back(new wxPoint(myControlInverted));
    }

    if (myIndex == 0 || myIndex == m_BezierPoints.GetCount() - 1) {
        myPoints.push_back(new wxPoint(myControl));
    }

    if (myIndex != 0) {
        wxPoint myPt1 = m_Scale->RealToPixel(*m_BezierPoints[myIndex - 1]);
        wxPoint myControl1 = m_Scale->RealToPixel(*m_BezierPointsControl[myIndex - 1]);
        wxPoint myControlInverted1 = myPt1 - (myControl1 - myPt1);
        myPoints.push_back(new wxPoint(myPt1));
        myPoints.push_back(new wxPoint(myControlInverted1));
    }

    if (myIndex != m_BezierPoints.GetCount() - 1) {
        wxPoint myPt2 = m_Scale->RealToPixel(*m_BezierPoints[myIndex + 1]);
        wxPoint myControl2 = m_Scale->RealToPixel(*m_BezierPointsControl[myIndex + 1]);
        myPoints.push_back(new wxPoint(myPt2));
        myPoints.push_back(new wxPoint(myControl2));
    }

    for (unsigned int i = 0; i < myPoints.GetCount(); i++) {
        dc->CalcBoundingBox(myPoints[i]->x, myPoints[i]->y);
    }
    myPoints.DeleteContents(true);
    myPoints.Clear();

    m_BezierRefreshRect = wxRect(wxPoint(dc->MinX(), dc->MaxY()), wxPoint(dc->MaxX(), dc->MinY()));
    m_BezierRefreshRect.Inflate(wxSize(3, 3));
    /*dc->SetPen(myGreyPen);
    dc->SetBrush(*wxTRANSPARENT_BRUSH);
    dc->DrawRectangle(m_BezierRefreshRect);*/
}

// search point or control clicked
void tmEditManager::BezierModifyClickDown(const wxPoint& mousepos) {
    if (m_BezierActualP2 != wxDefaultPosition) {
        return;
    }
    m_BezierActualP2 = mousepos;
    m_BezierActualC2 = wxDefaultPosition;
    m_BezierModifyIndexPoint = wxNOT_FOUND;
    m_BezierModifyIndexControl = wxNOT_FOUND;
    m_BezierModifyControlInverted = false;

    wxRect myRect(0, 0, 7, 7);
    myRect = myRect.CentreIn(wxRect(m_BezierActualP2, wxSize(0, 0)));

    for (unsigned int i = 0; i < m_BezierPointsControl.GetCount(); i++) {
        wxPoint myPt = m_Scale->RealToPixel(*m_BezierPointsControl[i]);
        if (myRect.Contains(myPt)) {
            m_BezierModifyIndexControl = i;
            return;
        }
    }

    for (unsigned int i = 0; i < m_BezierPointsControl.GetCount(); i++) {
        wxPoint myControlPt = m_Scale->RealToPixel(*m_BezierPointsControl[i]);
        wxPoint myPt = m_Scale->RealToPixel(*m_BezierPoints[i]);
        wxPoint myControlPtInverted = myPt - (myControlPt - myPt);
        if (myRect.Contains(myControlPtInverted)) {
            m_BezierModifyIndexControl = i;
            m_BezierModifyControlInverted = true;
            return;
        }
    }

    for (unsigned int i = 0; i < m_BezierPoints.GetCount(); i++) {
        wxPoint myPt = m_Scale->RealToPixel(*m_BezierPoints[i]);
        if (myRect.Contains(myPt)) {
            m_BezierModifyIndexPoint = i;
            m_BezierActualC2 = m_Scale->RealToPixel(*m_BezierPointsControl[i]);
            return;
        }
    }
}

void tmEditManager::BezierModifyClickMove(const wxPoint& mousepos) {
    if (m_BezierModifyIndexControl != wxNOT_FOUND) {
        if (m_BezierModifyControlInverted) {
            wxPoint myPt = m_Scale->RealToPixel(*m_BezierPoints[m_BezierModifyIndexControl]);
            *m_BezierPointsControl[m_BezierModifyIndexControl] = m_Scale->PixelToReal(myPt - (mousepos - myPt));
        } else {
            *m_BezierPointsControl[m_BezierModifyIndexControl] = m_Scale->PixelToReal(mousepos);
        }

        m_Renderer->RefreshRect(m_BezierRefreshRect);
        m_Renderer->Update();
        return;
    }

    if (m_BezierModifyIndexPoint != wxNOT_FOUND) {
        *m_BezierPoints[m_BezierModifyIndexPoint] = m_Scale->PixelToReal(mousepos);

        // move control points too when moving point
        if (m_BezierActualC2 != wxDefaultPosition) {
            wxPoint myMovingPoint = mousepos - m_BezierActualP2;
            wxPoint myControlPx = m_BezierActualC2 + myMovingPoint;
            *m_BezierPointsControl[m_BezierModifyIndexPoint] = m_Scale->PixelToReal(myControlPx);
        }

        m_Renderer->RefreshRect(m_BezierRefreshRect);
        m_Renderer->Update();
        return;
    }
}

void tmEditManager::BezierModifyClickUp(const wxPoint& mousepos) {
    // search for snapping only when a point was moved !
    // and update the vertex snapping status
    if (m_BezierModifyIndexPoint != wxNOT_FOUND) {
        wxRealPoint myPt = *m_BezierPoints[m_BezierModifyIndexPoint];
        int myPtSnappingIndex = m_BezierSnappedPointsIndexes.Index(m_BezierModifyIndexPoint);

        if (EMGetSnappingCoord(myPt)) {
            if (myPtSnappingIndex == wxNOT_FOUND) {
                m_BezierSnappedPointsIndexes.Add(m_BezierModifyIndexPoint);
            }
            *m_BezierPoints[m_BezierModifyIndexPoint] = myPt;
        } else {
            if (myPtSnappingIndex != wxNOT_FOUND) {
                m_BezierSnappedPointsIndexes.RemoveAt(myPtSnappingIndex);
            }
        }
    }

    m_BezierActualP2 = wxDefaultPosition;
    m_BezierActualC2 = wxDefaultPosition;
    m_BezierModifyIndexPoint = wxNOT_FOUND;
    m_BezierModifyIndexControl = wxNOT_FOUND;

    m_Renderer->Refresh();
    m_Renderer->Update();
}

bool tmEditManager::BezierToLine(BezierSettingsData settings) {
    if (!IsBezierToLinePreviewAllowed()) {
        return false;
    }

    for (unsigned int i = 1; i < m_BezierPoints.GetCount(); i++) {
        wxRealPoint p1 = *m_BezierPoints[i - 1];
        wxRealPoint p2 = *m_BezierPoints[i];
        wxRealPoint c1 = *m_BezierPoints[i - 1] - (*m_BezierPointsControl[i - 1] - *m_BezierPoints[i - 1]);
        if (i == 1) {
            c1 = *m_BezierPointsControl[i - 1];
        }
        wxRealPoint c2 = *m_BezierPointsControl[i];

        if (settings.method == BezierSettingsData::AGG) {
            agg::curve4 myCurv;
            myCurv.approximation_scale(settings.agg_approximation);
            myCurv.init(p1.x, p1.y, c1.x, c1.y, c2.x, c2.y, p2.x, p2.y);

            double x = 0.0;
            double y = 0.0;
            while (myCurv.vertex(&x, &y) != agg::path_cmd_stop) {
                m_ArcPoints.push_back(new wxRealPoint(x, y));
            }

        } else if (settings.method == BezierSettingsData::ETHZ) {
            BezierEthz myEth(settings.ethz_width, settings.ethz_max_points);
            myEth.Init(p1, c1, c2, p2);
            wxRealPointList* myList = myEth.GetPointListRef();
            wxASSERT(myList);
            wxRealPointList::iterator iter;
            m_ArcPoints.push_back(new wxRealPoint(p1));
            for (iter = myList->begin(); iter != myList->end(); ++iter) {
                m_ArcPoints.push_back(new wxRealPoint(**iter));
            }
        }

        // Avoid duplicating points. We remove last
        // point because it will be the same as next bezier's first point.
        if (i != m_BezierPoints.GetCount() - 1 && m_ArcPoints.GetCount() > 0) {
            m_ArcPoints.pop_back();
        }
    }
    return true;
}

bool tmEditManager::IsBezierToLinePreviewAllowed() {
    if (m_BezierPoints.GetCount() <= 1) {
        return false;
    }

    if (m_BezierPoints.GetCount() != m_BezierPointsControl.GetCount()) {
        return false;
    }
    return true;
}

void tmEditManager::SetBezierSettings(BezierSettingsData data, bool savetodb) {
    m_BezierSettings = data;
    if (savetodb) {
        wxASSERT(m_pDB);
        // TODO: Add database support for saving bezier settings
        // m_pDB->SaveBezierApproximationScale(m_BezierApproximationScale);
    }
}

void tmEditManager::ArcClick(const wxPoint& mousepos) {
    wxRealPoint myPt = m_Scale->PixelToReal(mousepos);
    if (EMGetSnappingCoord(myPt)) {
        m_ArcSnappedPointsIndexes.Add(m_ArcPoints.GetCount());
    }

    m_ArcPoints.push_back(new wxRealPoint(myPt));
    m_ArcActualPt = mousepos;

    if (IsLayerSpatialType(LAYER_SPATIAL_POINT)) {
        wxCommandEvent evt;
        OnDrawFeatureValidate(evt);
    }

    m_Renderer->Refresh();
    m_Renderer->Update();
}

void tmEditManager::ArcMove(const wxPoint& mousepos) {
    m_ArcActualPt = mousepos;

    m_Renderer->RefreshRect(m_ArcRefreshRect);
    m_Renderer->Update();
}

void tmEditManager::ArcDraw(wxGCDC* dc) {
    if (m_ArcPoints.GetCount() == 0) {
        return;
    }

    tmLayerProperties* myLayerProperties = m_TocCtrl->GetEditLayer();
    wxASSERT(myLayerProperties);
    tmSymbolVectorLine* mySymbol = static_cast<tmSymbolVectorLine*>(myLayerProperties->GetSymbolRef());

    // draw existing lines
    dc->SetPen(wxPen(m_SelectionColour, mySymbol->GetWidth()));
    wxPointList myPts;
    for (unsigned int i = 0; i < m_ArcPoints.GetCount(); i++) {
        myPts.push_back(new wxPoint(m_Scale->RealToPixel(*m_ArcPoints[i])));
    }

    if (myPts.GetCount() > 1) {
        dc->DrawLines(&myPts);
    }

    // draw nodes with different color based on snapping status
    int myNodeWidth = 2.0 * mySymbol->GetWidth();
    if (myNodeWidth < 4) {
        myNodeWidth = 4;
    }

    wxPen myNodeBlackPen = wxPen(*wxBLACK, myNodeWidth);
    wxPen myNodeGreenPen = wxPen(*wxGREEN, myNodeWidth);
    for (unsigned int i = 0; i < myPts.GetCount(); i++) {
        if (m_ArcSnappedPointsIndexes.Index(i) == wxNOT_FOUND) {
            dc->SetPen(myNodeBlackPen);
        } else {
            dc->SetPen(myNodeGreenPen);
        }

        wxPoint myPt(*myPts[i]);
#ifdef __WXMSW__
        dc->DrawLine(myPt.x, myPt.y, myPt.x + 1, myPt.y + 1);
#else
        dc->DrawLine(myPt.x, myPt.y, myPt.x, myPt.y);
#endif
    }

    dc->ResetBoundingBox();

    // draw actual arc when needed
    if (m_Renderer->GetTool() == tmTOOL_DRAW) {
#ifdef __WXMAC__
        dc->SetPen(*wxGREY_PEN);
#else
        dc->SetPen(wxPen(*wxLIGHT_GREY, 2, wxPENSTYLE_SOLID));
#endif
        if (m_ArcActualPt != wxDefaultPosition && myPts.GetCount() > 0 && m_Renderer->GetTool() == tmTOOL_DRAW) {
            dc->DrawLine(*myPts[myPts.GetCount() - 1], m_ArcActualPt);
        }
    }

    // compute bounding box for modifications
    if (m_Renderer->GetTool() == tmTOOL_MODIFY && m_ArcModifyIndexPoint != wxNOT_FOUND) {
        if (IsLayerSpatialType(LAYER_SPATIAL_LINE)) {
            dc->CalcBoundingBox(myPts[m_ArcModifyIndexPoint]->x, myPts[m_ArcModifyIndexPoint]->y);
            if (m_ArcModifyIndexPoint != 0) {
                dc->CalcBoundingBox(myPts[m_ArcModifyIndexPoint - 1]->x, myPts[m_ArcModifyIndexPoint - 1]->y);
            }
            if (m_ArcModifyIndexPoint < m_ArcPoints.GetCount() - 1) {
                dc->CalcBoundingBox(myPts[m_ArcModifyIndexPoint + 1]->x, myPts[m_ArcModifyIndexPoint + 1]->y);
            }
        } else if (IsLayerSpatialType(LAYER_SPATIAL_POINT)) {
            dc->CalcBoundingBox(myPts[0]->x - myNodeWidth, myPts[0]->y - myNodeWidth);
            dc->CalcBoundingBox(myPts[0]->x + myNodeWidth, myPts[0]->y + myNodeWidth);
        }
    }

    myPts.DeleteContents(true);
    myPts.Clear();

    m_ArcRefreshRect = wxRect(wxPoint(dc->MinX(), dc->MaxY()), wxPoint(dc->MaxX(), dc->MinY()));
    m_ArcRefreshRect.Inflate(wxSize(3, 3));
}

void tmEditManager::DrawSnappingCircle(wxGCDC* dc) {
    if (m_Renderer == nullptr || m_Scale == nullptr) {
        return;
    }

    if (!m_SnappingShowOnMap) {
        return;
    }

    wxASSERT(dc);
    wxASSERT(m_SnapMem);

    int myRadius = wxRound(m_Scale->MetersToPixels(m_SnapMem->GetTolerence()));
    wxPoint myCenter(5, 5);
    myCenter = myCenter + wxPoint(myRadius, myRadius);
    dc->SetPen(*wxGREY_PEN);
    dc->SetBrush(wxBrush(wxColour(100, 100, 100, 100)));
    dc->DrawCircle(myCenter, myRadius);
    if (myRadius > 2) {
        dc->SetPen(wxPen(wxColour(50, 50, 50, 100), 2));
#ifdef __WXMSW__
        dc->DrawLine(myCenter.x, myCenter.y, myCenter.x + 1, myCenter.y + 1);
#else
        dc->DrawLine(myCenter.x, myCenter.y, myCenter.x, myCenter.y);
#endif
    }
}

/***************************************************************************/ /**
  @brief User select modify tool
  @author Lucien Schreiber (c) CREALP 2009
  @date 26 January 2009
  *******************************************************************************/
void tmEditManager::OnToolModify() {
    wxASSERT(m_Renderer);
    if (!IsModificationAllowed()) {
        return;
    }

    m_Renderer->SetTool(tmTOOL_MODIFY);

    _LoadSnappingStatus();

    m_Renderer->Refresh();
    m_Renderer->Update();
}

bool tmEditManager::_LoadSnappingStatus() {
    // load geometries and compute snapping status from selected feature if needed
    if (m_ArcPoints.GetCount() == 0) {
        m_ArcOID = m_SelectedData->GetSelectedUnique();
        tmLayerProperties* myLayerProperties = m_TocCtrl->GetEditLayer();
        wxASSERT(m_pDB);
        OGRGeometry* myGeometry = m_pDB->GeometryLoad(m_ArcOID, myLayerProperties->GetType());
        if (myGeometry == nullptr) {
            return false;
        }

        if (IsLayerSpatialType(LAYER_SPATIAL_LINE)) {
            auto* myLine = dynamic_cast<OGRLineString*>(myGeometry);
            wxASSERT(myLine);
            for (int i = 0; i < myLine->getNumPoints(); i++) {
                m_ArcPoints.push_back(new wxRealPoint(myLine->getX(i), myLine->getY(i)));
            }
            OGRGeometryFactory::destroyGeometry(myGeometry);
        } else if (IsLayerSpatialType(LAYER_SPATIAL_POINT)) {
            auto* myPt = dynamic_cast<OGRPoint*>(myGeometry);
            if (myPt) {
                m_ArcPoints.push_back(new wxRealPoint(myPt->getX(), myPt->getY()));
            }
            OGRGeometryFactory::destroyGeometry(myGeometry);
        } else {
            OGRGeometryFactory::destroyGeometry(myGeometry);
            wxFAIL_MSG(_T("Case not supported for points!!!"));
            return false;
        }

        for (unsigned int i = 0; i < m_SnapMem->GetCount(); i++) {
            long myLayerId = 0;
            int mySnapStatus = tmSNAPPING_OFF;
            m_SnapMem->GetSnappingInfo(i, myLayerId, mySnapStatus);
            tmLayerProperties* myActualLayer = m_TocCtrl->GetLayerById(myLayerId);
            if (myActualLayer == nullptr) {
                break;
            }

            tmGISData* myActualGISData = tmGISData::LoadLayer(myActualLayer);
            if (myActualGISData == nullptr) {
                break;
            }

            long myActualID = wxNOT_FOUND;
            if (myActualLayer == m_TocCtrl->GetEditLayer()) {
                myActualID = m_ArcOID;
            }

            // Search if vertex are snapped
            if (mySnapStatus == tmSNAPPING_BEGIN_END) {
                unsigned int added = m_ArcPoints.GetCount() -1;
                if(added == 0) added = 1;
                for (unsigned int v = 0; v < m_ArcPoints.GetCount(); v += added) {
                    if (myActualGISData->IsPointSnapped(*m_ArcPoints[v], mySnapStatus, myActualID)) {
                        int myItemIndex = m_ArcSnappedPointsIndexes.Index(v);
                        if (myItemIndex == wxNOT_FOUND) {
                            m_ArcSnappedPointsIndexes.Add(v);
                        }
                    }
                }
            } else {
                for (unsigned int v = 0; v < m_ArcPoints.GetCount(); v++) {
                    // TODO Implement IsPointSnapped for Shapefiles
                    if (myActualGISData->IsPointSnapped(*m_ArcPoints[v], mySnapStatus, myActualID)) {
                        int myItemIndex = m_ArcSnappedPointsIndexes.Index(v);
                        if (myItemIndex == wxNOT_FOUND) {
                            m_ArcSnappedPointsIndexes.Add(v);
                        }
                    }
                }
            }

            wxDELETE(myActualGISData);
        }
        return true;
    }
    return false;
}

void tmEditManager::OnToolEditShared() {
    wxASSERT(m_Renderer);
    m_Renderer->SetTool(tmTOOL_MODIFY_SHARED);
}

/***************************************************************************/ /**
  @brief Set tool to cut line tool
  @author Lucien Schreiber (c) CREALP 2009
  @date 05 February 2009
  *******************************************************************************/
void tmEditManager::OnToolCutLines() {
    wxASSERT(m_Renderer);
    m_Renderer->SetTool(tmTOOL_CUT_LINES);
}

/***************************************************************************/ /**
  @brief Set oriented point tool
  @author Lucien Schreiber (c) CREALP 2009
  @date 24 September 2009
  *******************************************************************************/
void tmEditManager::OnToolOrientedPoint() {
    wxASSERT(m_Renderer);

    if (!IsModificationAllowed()) return;

    if (!IsLayerTypeSelected(TOC_NAME_POINTS)) return;

    m_OrientedPt.Create(m_pDB, m_SelectedData->GetSelectedUnique());
    if (!m_OrientedPt.IsCorrectType()) {
        wxLogWarning(_("No orientation attributes found, or point hasn't any kind"));
        return;
    }
    m_Renderer->SetTool(tmTOOL_ORIENTED_POINTS);
}

void tmEditManager::OnToolVertexDelete() {
    m_Renderer->SetTool(tmTOOL_VERTEX_DELETE);
}

void tmEditManager::OnToolVertexInsert() {
    m_Renderer->SetTool(tmTOOL_VERTEX_INSERT);
}

/***************************************************************************/ /**
  @brief Checks that a layer is selected
  @details
  @return  true if : a layer is defined as editing layer
  @author Lucien Schreiber (c) CREALP 2009
  @date 28 January 2009
  *******************************************************************************/
bool tmEditManager::IsCorrectLayerSelected() {
    wxASSERT(m_TocCtrl);
    if (!m_TocCtrl->GetEditLayer()) {
        return false;
    }

    // ensure no selection from external layer
    if (m_SelectedData->GetCount() > 0) {
        if (m_TocCtrl->GetEditLayer()->GetID() != m_SelectedData->GetSelectedLayer()) {
            m_SelectedData->Clear();
            wxCommandEvent evt2(tmEVT_LM_UPDATE, wxID_ANY);
            m_ParentEvt->GetEventHandler()->QueueEvent(evt2.Clone());
        }
    }
    return true;
}

/***************************************************************************/ /**
  @brief Check that a layer of type specified is selected
  @param layertype type of layer. Allowed values are :
  - LAYER_SPATIAL_LINE
  - LAYER_SPATIAL_POINT
  - LAYER_SPATIAL_POLYGON
  @return  true if seleced layer is of specified type
  @author Lucien Schreiber (c) CREALP 2009
  @date 27 February 2009
  *******************************************************************************/
bool tmEditManager::IsLayerTypeSelected(int layertype) {
    wxASSERT(m_TocCtrl);
    tmLayerProperties* myEditLayer = m_TocCtrl->GetEditLayer();
    if (myEditLayer == nullptr) {
        wxLogWarning(_("No editing layer selected. Define an edit layer"));
        return false;
    }

    if (m_TocCtrl->GetEditLayer()->GetType() != layertype) {
        wxLogWarning(
            _("Layer isn't of correct type.") +
            wxString::Format(_("Please select a layer of type '%s'"), TM_GIS_SPATIAL_TYPES_STRING[layertype].c_str()));
        return false;
    }
    return true;
}

/***************************************************************************/ /**
  @brief Checks that an object is selected
  @details
  @return  true if an object is selected, false if no object or more than one
  objects are selected
  @author Lucien Schreiber (c) CREALP 2009
  @date 28 January 2009
  *******************************************************************************/
bool tmEditManager::IsObjectSelected() {
    if (m_SelectedData->GetCount() == 1) {
        return true;
    }
    return false;
}

/***************************************************************************/ /**
  @brief Check that minimum x objects are selected
  @param iNumbermin Number of minimum objects to be selected
  @return  true if minimum x objects are selected
  @author Lucien Schreiber (c) CREALP 2009
  @date 27 February 2009
  *******************************************************************************/
bool tmEditManager::IsObjectMinNumberSelected(unsigned int iNumbermin) {
    if (m_SelectedData->GetCount() >= iNumbermin) return true;

    wxLogDebug(_T("%d item selected, expected : %d"), m_SelectedData->GetCount(), iNumbermin);
    return false;
}

/***************************************************************************/ /**
  @brief Are we ready for drawing
  @details
  @return  True if all validation are ok for editing
  @author Lucien Schreiber (c) CREALP 2009
  @date 28 January 2009
  *******************************************************************************/
bool tmEditManager::IsDrawingAllowed() {
    if (IsCorrectLayerSelected() && m_EditStarted) return true;

    return false;
}

/***************************************************************************/ /**
  @brief Are we ready for drawing
  @details
  @return  True if all validation are ok for editing
  @author Lucien Schreiber (c) CREALP 2009
  @date 28 January 2009
  *******************************************************************************/
bool tmEditManager::IsModificationAllowed() {
    if (!IsCorrectLayerSelected()) {
        return false;
    }

    if (!IsObjectSelected() && m_ArcPoints.GetCount() == 0) {
        return false;
    }
    return true;
}

bool tmEditManager::IsModificationBezierAllowed() {
    if (!IsCorrectLayerSelected()) {
        return false;
    }

    if (m_BezierPoints.GetCount() < 2) {
        return false;
    }

    if (m_BezierPoints.GetCount() != m_BezierPointsControl.GetCount()) {
        return false;
    }

    return true;
}

bool tmEditManager::IsMultipleModificationAllowed() {
    if (!IsCorrectLayerSelected()) {
        return false;
    }

    if (!IsObjectMinNumberSelected(1)) {
        return false;
    }
    return true;
}

bool tmEditManager::IsLayerSpatialType(int layerType) {
    wxASSERT(m_TocCtrl);
    if (m_TocCtrl->GetEditLayer() == nullptr) {
        return false;
    }

    if (m_TocCtrl->GetEditLayer()->GetSpatialType() != layerType) {
        return false;
    }
    return true;
}

void tmEditManager::OnOrientedPtsDown(wxCommandEvent& event) {
    wxPoint* myPt = (wxPoint*)event.GetClientData();
    wxASSERT(myPt);

    m_OrientedPt.SetStartPoint(*myPt);
    m_DrawLine.CreateVertex(*myPt);

    delete myPt;
}

void tmEditManager::OnOrientedPtsMove(wxCommandEvent& event) {
    wxPoint* myPt = (wxPoint*)event.GetClientData();
    wxASSERT(myPt);

    if (!m_DrawLine.IsOK()) {
        delete myPt;
        return;
    }

    wxClientDC dc(m_Renderer);
    bool BDraw = m_DrawLine.DrawEditPart(&dc);
    wxASSERT(BDraw);
    bool bSetVertex = m_DrawLine.SetVertex(*myPt);
    wxASSERT(bSetVertex);

    // display angle
    m_OrientedPt.SetEndPoint(*myPt);
    int myOrient = m_OrientedPt.GetOrientationInt();
    if (myOrient != wxNOT_FOUND) {
        wxCommandEvent evt2(tmEVT_LM_ANGLE_CHANGED, wxID_ANY);
        evt2.SetInt(myOrient);
        m_ParentEvt->GetEventHandler()->QueueEvent(evt2.Clone());
    }

    delete myPt;
}

void tmEditManager::OnOrientedPtsUp(wxCommandEvent& event) {
    wxPoint* myPt = (wxPoint*)event.GetClientData();
    wxASSERT(myPt);

    if (!m_DrawLine.IsOK()) {
        delete myPt;
        return;
    }
    wxClientDC myDC(m_Renderer);
    m_DrawLine.DrawEditReset(&myDC);

    m_OrientedPt.SetEndPoint(*myPt);
    bool bUpdate = m_OrientedPt.Update();
    wxASSERT(bUpdate);

    // clearing status bar
    wxCommandEvent evt2(tmEVT_LM_ANGLE_CHANGED, wxID_ANY);
    evt2.SetInt(wxNOT_FOUND);
    m_ParentEvt->GetEventHandler()->QueueEvent(evt2.Clone());

    m_DrawLine.ClearVertex();

    m_Renderer->Refresh(false);
    m_Renderer->Update();
    delete myPt;
}

/***************************************************************************/ /**
  @brief Called when TOC start editing
  @author Lucien Schreiber (c) CREALP 2009
  @date 03 February 2009
  *******************************************************************************/
void tmEditManager::OnEditStart(wxCommandEvent& event) {
    wxConfigBase* myConfig = wxFileConfig::Get();
    wxASSERT(myConfig);
    wxString mySelColorText = myConfig->Read("GENERAL/selection_color", wxEmptyString);

    if (mySelColorText != wxEmptyString) {
        m_SelectionColour.Set(mySelColorText);
    }

    m_EditStarted = true;
    event.Skip();
}

/***************************************************************************/ /**
  @brief Called when TOC stop editing
  @author Lucien Schreiber (c) CREALP 2009
  @date 03 February 2009
  *******************************************************************************/
void tmEditManager::OnEditStop(wxCommandEvent& event) {
    m_EditStarted = false;

    if (m_ArcPoints.GetCount() > 0) {
        wxLogWarning(_("The editing mode was stopped before the line was finalized."));
        m_ArcPoints.Clear();
    }

    event.Skip();
}

/***************************************************************************/ /**
  @brief Called when user press enter / TAB in the #tmRenderer
  @author Lucien Schreiber (c) CREALP 2009
  @date 05 February 2009
  *******************************************************************************/
void tmEditManager::OnDrawFeatureValidate(wxCommandEvent& event) {
    if (!IsDrawingAllowed()) {
        return;
    }

    if (IsLayerSpatialType(LAYER_SPATIAL_LINE) && m_BezierPoints.GetCount() > 1) {
        BezierToLine(m_BezierSettings);
        BezierClear();
    }

    if (m_ArcPoints.GetCount() < 1) {
        return;
    }

    long myLineId = _SaveToDatabase();
    ArcClear();

    // set selection
    m_SelectedData->SetLayerID(m_TocCtrl->GetEditLayer()->GetID());
    m_SelectedData->SetSelected(myLineId);
    wxCommandEvent evt(tmEVT_SELECTION_DONE, wxID_ANY);
    m_ParentEvt->GetEventHandler()->QueueEvent(evt.Clone());

    // update display
    wxCommandEvent evt2(tmEVT_LM_UPDATE, wxID_ANY);
    m_ParentEvt->GetEventHandler()->QueueEvent(evt2.Clone());
}

long tmEditManager::_SaveToDatabase() {
    tmLayerProperties* layerprop = m_TocCtrl->GetEditLayer();
    if (layerprop == nullptr) {
        return wxNOT_FOUND;
    }

    OGRGeometry* myGeom = nullptr;
    OGRLineString myLineString;
    OGRPoint myPoint;
    OGRMultiPoint myMultiPoint;
    if (layerprop->GetSpatialType() == LAYER_SPATIAL_LINE) {
        if (m_ArcPoints.GetCount() <= 1) {
            return wxNOT_FOUND;
        }

        for (unsigned int i = 0; i < m_ArcPoints.GetCount(); i++) {
            myLineString.addPoint(m_ArcPoints[i]->x, m_ArcPoints[i]->y);
        }
        myGeom = &myLineString;
    } else if (layerprop->GetSpatialType() == LAYER_SPATIAL_POINT) {
        wxASSERT(m_ArcPoints.GetCount() == 1);
        myPoint.setX(m_ArcPoints[0]->x);
        myPoint.setY(m_ArcPoints[0]->y);
        myGeom = &myPoint;
    } else {
        wxFAIL_MSG(_("spatial type not supported!"));
    }

    wxASSERT(m_pDB);
    long myOid = wxNOT_FOUND;
    if (m_ArcOID != wxNOT_FOUND) {  // Updating
        m_pDB->GeometryUpdate(myGeom, m_ArcOID, layerprop->GetType());
        myOid = m_ArcOID;
    } else {
        myOid = m_pDB->GeometrySave(myGeom, layerprop->GetType());
    }
    return myOid;
}

/***************************************************************************/ /**
  @brief Called when user press ESC in the #tmRenderer
  @author Lucien Schreiber (c) CREALP 2009
  @date 05 February 2009
  *******************************************************************************/
void tmEditManager::OnDrawFeatureEscape(wxCommandEvent& event) {
    if (!IsDrawingAllowed()) {
        return;
    }

    wxClientDC myDC(m_Renderer);
    m_DrawLine.DrawEditReset(&myDC);
    m_DrawLine.ClearVertex();

    BezierClear();
    ArcClear();

    m_Renderer->Refresh();
    m_Renderer->Update();
}

/***************************************************************************/ /**
  @brief Called if user try to cut lines
  @author Lucien Schreiber (c) CREALP 2009
  @date 05 February 2009
  *******************************************************************************/
void tmEditManager::OnCutLines(wxCommandEvent& event) {
    // get mouse position
    wxPoint* mypCutPos = (wxPoint*)event.GetClientData();
    wxPoint myCutPos = wxPoint(*mypCutPos);
    delete mypCutPos;

    // some checks (1 object selected)
    if (!IsModificationAllowed()) {
        wxLogDebug(_T("Unable to cut line, select a line first"));
        return;
    }
    if (m_TocCtrl->GetEditLayer()->GetSpatialType() != LAYER_SPATIAL_LINE) {
        return;
    }

    // Get the selected layer
    tmGISDataVector* mySelLayer = (tmGISDataVector*)tmGISData::LoadLayer(m_TocCtrl->GetEditLayer());
    if (!mySelLayer) {
        return;
    }

    // display radius
    int icutRadius = tmSELECTION_DIAMETER;
    if (m_SnapMem->IsSnappingEnabled()) {
        icutRadius = m_SnapMem->GetTolerence();
        double dRadius = m_Scale->MetersToPixels(icutRadius);
        icutRadius = int(dRadius + 0.5);
    }

    m_Renderer->DrawCircleVideoInverse(myCutPos, icutRadius);
    m_Renderer->Update();
    wxMilliSleep(200);
    m_Renderer->DrawCircleVideoInverseClean();

    bool bCut = mySelLayer->CutLineAtVertex(m_SelectedData->GetSelectedUnique(), m_Scale->PixelToReal(myCutPos),
                                            tmSELECTION_DIAMETER, m_TocCtrl->GetEditLayer()->GetType());
    wxDELETE(mySelLayer);
    if (!bCut) {
        return;
    }

    // copy attribution only if layer is lines (not frame!)
    if (m_TocCtrl->GetEditLayer()->GetType() == TOC_NAME_LINES) {
        wxCommandEvent attribevt1(tmEVT_AM_COPY_ATTRIBUTION, wxID_ANY);
        attribevt1.SetExtraLong(m_SelectedData->GetSelectedUnique());
        wxArrayLong* myTempArray = new wxArrayLong();
        myTempArray->Add(m_pDB->DataBaseGetLastInsertedID());
        attribevt1.SetClientData(myTempArray);
        m_ParentEvt->GetEventHandler()->QueueEvent(attribevt1.Clone());
    }

    // update display
    wxCommandEvent evt2(tmEVT_LM_UPDATE, wxID_ANY);
    m_ParentEvt->GetEventHandler()->QueueEvent(evt2.Clone());

    // change cursor
    m_Renderer->SetTool(tmTOOL_SELECT);
}

/***************************************************************************/ /**
  @brief Called to display vertex position
  @details Respond to tmEVT_EV_DISPLAY_VERTEX_COORD event.
  @author Lucien Schreiber (c) CREALP 2009
  @date 24 February 2009
  *******************************************************************************/
void tmEditManager::OnShowVertexPosition(wxCommandEvent& event) {
    wxRealPoint* myPt = (wxRealPoint*)event.GetClientData();
    if (myPt == nullptr) return;

    wxLogDebug(_T("Showing vertex @ %.*f / %.*f"), 2, myPt->x, 2, myPt->y);

    // convert to px
    wxPoint myPxPt = m_Scale->RealToPixel(*myPt);
    delete myPt;

    m_Renderer->Refresh();
    m_Renderer->Update();

    m_Renderer->DrawCircleVideoInverse(myPxPt, 7);
    wxMilliSleep(200);
    m_Renderer->DrawCircleVideoInverseClean();
}

void tmEditManager::OnMenuInsertVertex(wxCommandEvent& event) {
    if (m_ArcModifyIndexPoint == wxNOT_FOUND || m_ArcActualPt == wxDefaultPosition) {
        return;
    }

    m_ArcPoints.Insert(m_ArcModifyIndexPoint, new wxRealPoint(m_ArcActualPt));

    m_Renderer->Refresh();
    m_Renderer->Update();
}

void tmEditManager::OnMenuDeleteVertex(wxCommandEvent& event) {
    if (m_ArcModifyIndexPoint == wxNOT_FOUND) {
        return;
    }

    m_ArcPoints.DeleteContents(true);
    m_ArcPoints.DeleteNode(m_ArcPoints.Item(m_ArcModifyIndexPoint));

    m_Renderer->Refresh();
    m_Renderer->Update();
}

/***************************************************************************/ /**
  @brief Search function
  @details This function is widly inspired from #tmLayerManager
  @param screenpt Coordinate of the clicked point
  @return  true if an object was found
  @author Lucien Schreiber (c) CREALP 2009
  @date 06 February 2009
  *******************************************************************************/
bool tmEditManager::SelectedSearch(const wxPoint& screenpt) {
    wxASSERT(IsDrawingAllowed());
    tmLayerProperties* pLayerprop = m_TocCtrl->GetEditLayer();
    wxASSERT(pLayerprop);

    // create rect from point
    int myRadius = tmSELECTION_DIAMETER / 2;
    wxRect myClickRect = wxRect(screenpt.x - myRadius, screenpt.y - myRadius, tmSELECTION_DIAMETER,
                                tmSELECTION_DIAMETER);

    tmRealRect myClickReal = m_Scale->PixelsToReal(myClickRect);
    tmGISData* myLayerData = tmGISData::LoadLayer(pLayerprop);
    wxASSERT(myLayerData);
    if (!myLayerData) return false;

    wxArrayLong* myArray = myLayerData->SearchData(myClickReal, pLayerprop->GetType());
    if (!myArray) return false;

    int myArrayCount = myArray->GetCount();
    if (myArrayCount == 0) {
        wxLogDebug(_T("No data found at specified coordinates"));
        return false;
    }
    wxLogDebug(_T("Number of features selected : %d"), myArrayCount);

    m_SelectedData->SetLayerID(pLayerprop->GetID());
    m_SelectedData->Clear();
    m_SelectedData->AddSelected(myArray);
    myArray->Clear();
    delete myArray;

    return true;
}

bool tmEditManager::EMGetSnappingCoord(wxRealPoint& pt) {
    // Snapping may be disabled using space key
    if (!m_SnapMem->IsSnappingEnabled()) {
        return false;
    }

    wxRealPoint* mySnapCoord = nullptr;
    mySnapCoord = EMIterateAllSnappingLayers(pt);

    if (mySnapCoord != nullptr) {
        pt.x = mySnapCoord->x;
        pt.y = mySnapCoord->y;
        wxDELETE(mySnapCoord);
        return true;
    }
    wxDELETE(mySnapCoord);
    return false;
}

/***************************************************************************/ /**
  @brief Iterate all layers in snapping memory
  @details Try to get the snapped coordinate for the clicked point
  @param clickedpoint The real coordinate of the clicked point
  @return  a valid wxRealPoint if snapped point found, null otherwise (don't
  forget to delete)
  @author Lucien Schreiber (c) CREALP 2009
  @date 30 January 2009
  *******************************************************************************/
wxRealPoint* tmEditManager::EMIterateAllSnappingLayers(const wxRealPoint& clickedpoint) {
    long myLayerId = 0;
    int mySnapStatus = tmSNAPPING_OFF;
    tmLayerProperties* myActualLayer = nullptr;
    wxArrayRealPoints mySnapPts;
    for (unsigned int i = 0; i < m_SnapMem->GetCount(); i++) {
        m_SnapMem->GetSnappingInfo(i, myLayerId, mySnapStatus);
        myActualLayer = m_TocCtrl->GetLayerById(myLayerId);
        if (!myActualLayer) continue;

        // search snapping for that layer
        tmGISData* myActualGISData = tmGISData::LoadLayer(myActualLayer);
        if (!myActualGISData) {
            continue;
        }

        // convert meters to real units
        double myRealUnits = m_Scale->MetersToRealUnits(m_SnapMem->GetTolerence());
        myActualGISData->GetSnapCoord(clickedpoint, myRealUnits, mySnapPts, mySnapStatus);
        wxDELETE(myActualGISData);
    }

    wxLogMessage("%d snapping points found!", (int)mySnapPts.GetCount());
    if (mySnapPts.GetCount() == 0) {
        return nullptr;
    }

    if (mySnapPts.GetCount() == 1) {
        return new wxRealPoint(mySnapPts.Item(0));
    }

    // compute closest point!
    int myMinItemIndex = 0;
    double myMinDistance = 0;
    for (unsigned int p = 0; p < mySnapPts.GetCount(); p++) {
        wxRealPoint mySubstrPtr = mySnapPts.Item(p) - clickedpoint;
        double myDistance = sqrt((mySubstrPtr.x * mySubstrPtr.x) + (mySubstrPtr.y * mySubstrPtr.y));
        if (p == 0) {
            myMinDistance = myDistance;
        } else {
            if (myDistance < myMinDistance) {
                myMinDistance = myDistance;
                myMinItemIndex = p;
            }
        }
    }
    return new wxRealPoint(mySnapPts.Item(myMinItemIndex));
}

/***************************************************************************/ /**
  @brief Delete the geometry and attribution of selected object
  @param clearSelection true if we sould clear selection ids, if you need to
  delete more stuff (attribution for exemple) set to false
  @return true if geometry are deleted
  @author Lucien Schreiber (c) CREALP 2009
  @date 05 February 2009
  *******************************************************************************/
bool tmEditManager::DeleteSelected(bool clearSelection) {
    // make some checks
    if (!IsDrawingAllowed() || m_SelectedData->GetCount() <= 0) {
        return false;
    }

    // delete ids from database
    wxArrayLong* mySelectedIds = m_SelectedData->GetSelectedValues();
    if (mySelectedIds == nullptr) {
        return false;
    }

    wxBusyCursor wait;
    for (unsigned int i = 0; i < mySelectedIds->GetCount(); i++) {
        wxArrayLong myFeatures = m_pDB->GetObjectsFromFeature(mySelectedIds->Item(i),
                                                              m_TocCtrl->GetEditLayer()->GetType());
        for (unsigned int f = 0; f < myFeatures.GetCount(); f++) {
            m_pDB->DeleteAdvancedAttribution(mySelectedIds->Item(i), m_pDB->GetSelectedLayerId(myFeatures.Item(f)));
        }
    }
    m_pDB->DeleteAttribution(mySelectedIds, m_TocCtrl->GetEditLayer()->GetType());
    m_pDB->GeometryDelete(mySelectedIds, m_TocCtrl->GetEditLayer()->GetType());
    delete mySelectedIds;

    if (clearSelection) {
        // set selection
        m_SelectedData->SetLayerID(m_TocCtrl->GetEditLayer()->GetID());
        m_SelectedData->Clear();
        wxCommandEvent evt(tmEVT_SELECTION_DONE, wxID_ANY);
        m_ParentEvt->GetEventHandler()->QueueEvent(evt.Clone());
    }

    // update display
    wxCommandEvent evt2(tmEVT_LM_UPDATE, wxID_ANY);
    m_ParentEvt->GetEventHandler()->QueueEvent(evt2.Clone());
    return true;
}

/***************************************************************************/ /**
  @brief Remove the last vertex from memory
  @details This function will only work while editing lines, calling this for
  points wont work
  @return  true if the last vertex was successfully removed
  @author Lucien Schreiber (c) CREALP 2009
  @date 05 February 2009
  *******************************************************************************/
bool tmEditManager::UndoLastVertex() {
    // try removing last bezier vertex. If not possible, remove last line vertex
    if (m_BezierPoints.GetCount() > 1) {
        if (m_BezierPoints.GetCount() == m_BezierPointsControl.GetCount()) {
            m_BezierPointsControl.pop_back();
            m_BezierPoints.pop_back();
        } else {
            m_BezierPoints.pop_back();
        }

        int mySnapIndex = m_BezierSnappedPointsIndexes.Index(m_BezierPoints.GetCount());
        if (mySnapIndex != wxNOT_FOUND) {
            m_BezierSnappedPointsIndexes.RemoveAt(mySnapIndex);
        }

        m_Renderer->Refresh();
        m_Renderer->Update();
        return true;
    }

    if (m_ArcPoints.GetCount() > 1) {
        m_ArcPoints.pop_back();

        int mySnapIndex = m_ArcSnappedPointsIndexes.Index(m_ArcPoints.GetCount());
        if (mySnapIndex != wxNOT_FOUND) {
            m_ArcSnappedPointsIndexes.RemoveAt(mySnapIndex);
        }

        m_Renderer->Refresh();
        m_Renderer->Update();
        return true;
    }
    return false;
}

bool tmEditManager::HasLastVertex() {
    if (m_ArcPoints.GetCount() > 1) {
        return true;
    }

    if (m_BezierPoints.GetCount() > 1) {
        return true;
    }

    return false;
}

/***************************************************************************/ /**
  @brief Number of selected features
  @return  the number of features selected
  @author Lucien Schreiber (c) CREALP 2009
  @date 12 May 2009
  *******************************************************************************/
int tmEditManager::GetSelectionCount() {
    return m_SelectedData->GetCount();
}

/***************************************************************************/ /**
  @brief Create intersections
  @details between the selected line and all crossed lines.
  @return  true if all works, false otherwise
  @author Lucien Schreiber (c) CREALP 2009
  @date 09 February 2009
  *******************************************************************************/
bool tmEditManager::CreateIntersections() {
    // checks (editing mode and 1 line selected)
    if (!IsObjectSelected()) {
        return false;
    }

    if (m_TocCtrl->GetEditLayer()->GetSpatialType() != LAYER_SPATIAL_LINE) {
        return false;
    }

    // Get the Layer (Line MySQL)
    tmGISDataVector* mySelLayer = (tmGISDataVector*)tmGISData::LoadLayer(m_TocCtrl->GetEditLayer());
    if (!mySelLayer) {
        return false;
    }

    OGRFeature* myFeature = mySelLayer->GetFeatureByOID(m_SelectedData->GetSelectedUnique());
    wxASSERT(myFeature);

    OGRLineString* myOGRSelLine = (OGRLineString*)myFeature->GetGeometryRef();
    wxASSERT(myOGRSelLine);

    // Get all line crossing
    wxArrayLong* myLinesCrossing = mySelLayer->SearchIntersectingGeometry(myOGRSelLine);
    if (myLinesCrossing == NULL || myLinesCrossing->GetCount() == 0) {
        OGRFeature::DestroyFeature(myFeature);
        wxLogWarning(_T("No intersections found"));
        return false;
    }

    OGRMultiLineString LinesCrossing;
    for (unsigned int j = 0; j < myLinesCrossing->GetCount(); j++) {
        OGRFeature* myTempFeature = mySelLayer->GetFeatureByOID(myLinesCrossing->Item(j));
        wxASSERT(myTempFeature);
        LinesCrossing.addGeometry(myTempFeature->GetGeometryRef());
        OGRFeature::DestroyFeature(myTempFeature);
    }

    // for selected lines, compute all intersections (with all lines)
    wxArrayLong myInsertedIDs1;
    OGRMultiLineString selectedsegments;
    mySelLayer->CutLineMultiple(myOGRSelLine, &LinesCrossing, selectedsegments);
    mySelLayer->SplitGeometry(&selectedsegments, m_SelectedData->GetSelectedUnique(), myInsertedIDs1,
                              m_TocCtrl->GetEditLayer()->GetType());

    // add attributions for new segment of selected line
    // only if lines (not frame)
    if (m_TocCtrl->GetEditLayer()->GetType() == TOC_NAME_LINES) {
        wxCommandEvent attribevt1(tmEVT_AM_COPY_ATTRIBUTION, wxID_ANY);
        attribevt1.SetExtraLong(m_SelectedData->GetSelectedUnique());
        wxArrayLong* myTempArray = new wxArrayLong(myInsertedIDs1);
        attribevt1.SetClientData(myTempArray);
        m_ParentEvt->GetEventHandler()->QueueEvent(attribevt1.Clone());
    }

    // compute intersections for other lines
    OGRMultiLineString myRes1;
    OGRMultiLineString myRes2;
    wxArrayLong myInsertedIDs2;
    wxCommandEvent attribevt2(tmEVT_AM_COPY_ATTRIBUTION, wxID_ANY);
    // for all crossing line, compute intersections
    for (unsigned int i = 0; i < myLinesCrossing->GetCount(); i++) {
        OGRLineString* myCrossedL = (OGRLineString*)LinesCrossing.getGeometryRef(i);
        if (myCrossedL) {
            mySelLayer->CutLineGeometry(myOGRSelLine, myCrossedL, myRes1, myRes2);
            mySelLayer->SplitGeometry(&myRes2, myLinesCrossing->Item(i), myInsertedIDs2,
                                      m_TocCtrl->GetEditLayer()->GetType());
            myRes1.empty();
            myRes2.empty();

            // send attribution message
            if (m_TocCtrl->GetEditLayer()->GetType() == TOC_NAME_LINES) {
                attribevt2.SetExtraLong(myLinesCrossing->Item(i));
                wxArrayLong* myTempArray2 = new wxArrayLong(myInsertedIDs2);
                attribevt2.SetClientData(myTempArray2);
                m_ParentEvt->GetEventHandler()->QueueEvent(attribevt2.Clone());
            }
            myInsertedIDs2.Clear();
        }
    }
    OGRFeature::DestroyFeature(myFeature);
    wxDELETE(myLinesCrossing);
    wxDELETE(mySelLayer);

    // Check lines for error (two vertex overlapping)
    wxArrayLong myErrLines;
    myErrLines = _GetErrorLines(myInsertedIDs1);
    if (myErrLines.GetCount() > 0) {
        wxLogDebug("Incorrect lines found, removing them!");
        m_SelectedData->Clear();
        m_SelectedData->AddSelected(&myErrLines);
        DeleteSelected();
        for (unsigned int i = 0; i < myErrLines.GetCount(); i++) {
            myInsertedIDs1.Remove(myErrLines.Item(i));
        }
    }

    // add segment to selection
    m_SelectedData->AddSelected(&myInsertedIDs1);
    wxCommandEvent evt(tmEVT_SELECTION_DONE, wxID_ANY);
    m_ParentEvt->GetEventHandler()->QueueEvent(evt.Clone());

    // update display
    wxCommandEvent evt2(tmEVT_LM_UPDATE, wxID_ANY);
    m_ParentEvt->GetEventHandler()->QueueEvent(evt2.Clone());

    // send statistics
    wxCommandEvent statevt(tmEVT_STAT_INTERSECTION, wxID_ANY);
    m_ParentEvt->GetEventHandler()->QueueEvent(statevt.Clone());

    return true;
}

wxArrayLong tmEditManager::_GetErrorLines(wxArrayLong linetocheck) {
    wxArrayLong myResults;
    wxASSERT(myResults.GetCount() == 0);

    // Get the Layer (Line MySQL)
    tmGISDataVector* mySelLayer = (tmGISDataVector*)tmGISData::LoadLayer(m_TocCtrl->GetEditLayer());
    if (!mySelLayer) {
        wxLogError(_("Checking intersected lines failed! Unable to get the 'in edition' layer!"));
        return myResults;
    }

    for (long i = 0; i < linetocheck.GetCount(); i++) {
        OGRFeature* myFeature = mySelLayer->GetFeatureByOID(linetocheck.Item(i));
        if (myFeature == nullptr) {
            wxLogError(_("Unable to get feature : %ld! Checking intersected lines failed"), linetocheck.Item(i));
            continue;
        }
        OGRLineString* myOGRSelLine = (OGRLineString*)myFeature->GetGeometryRef();
        wxASSERT(myOGRSelLine);

        // line error, only one vertex!
        if (myOGRSelLine->getNumPoints() == 1) {
            myResults.Add(linetocheck.Item(i));
            OGRFeature::DestroyFeature(myFeature);
            continue;
        }

        OGRPoint p1;
        OGRPoint p2;

        myOGRSelLine->getPoint(0, &p1);
        myOGRSelLine->getPoint(1, &p2);

        // line error 2 vertex overlapping
        if (p1.Equals(&p2) && myOGRSelLine->getNumPoints() == 2) {
            myResults.Add(linetocheck.Item(i));
        }

        OGRFeature::DestroyFeature(myFeature);
    }
    return myResults;
}

/***************************************************************************/ /**
  @brief Display a dialog for editing vertex position
  @details Checks are done internally for ensuring that :
  - An object is selected
  - Editing layer is selected
  @return  true if Editing vertex is allowed, false otherwise
  @author Lucien Schreiber (c) CREALP 2009
  @date 23 February 2009
  *******************************************************************************/
bool tmEditManager::EditVertexPosition() {
    // validation
    if (!IsModificationAllowed()) return false;

    // get the editing layers
    tmGISDataVector* mySelLayer = (tmGISDataVector*)tmGISData::LoadLayer(m_TocCtrl->GetEditLayer());
    if (!mySelLayer) return false;

    // getting geometry info
    long lSelectedOID = m_SelectedData->GetSelectedUnique();
    OGRFeature* myFeature = mySelLayer->GetFeatureByOID(lSelectedOID);
    if (!myFeature) return false;
    OGRGeometry* myGeom = myFeature->GetGeometryRef();
    wxASSERT(myGeom);
    OGRwkbGeometryType myType = wkbFlatten(myGeom->getGeometryType());

    // preparing dialog and dialog data
    EditVertexDLG myDlg(m_Renderer);
    myDlg.m_SelectedOID = lSelectedOID;
    myDlg.m_LayerType = m_TocCtrl->GetEditLayer()->GetSpatialType();
    OGRLineString* myLine = nullptr;
    OGRPoint* myPt = nullptr;

    switch (myType) {
        case wkbPoint:
            myPt = (OGRPoint*)myGeom;
            myDlg.m_VertexPts.Add(wxRealPoint(myPt->getX(), myPt->getY()));
            break;

        case wkbLineString:
            myLine = (OGRLineString*)myGeom;
            for (int i = 0; i < myLine->getNumPoints(); i++)
                myDlg.m_VertexPts.Add(wxRealPoint(myLine->getX(i), myLine->getY(i)));
            break;

        default:
            OGRFeature::DestroyFeature(myFeature);
            return false;
            break;
    }

    // displaying dialog
    if (myDlg.ShowModal() == wxID_OK) {
        // apply modification
        if (myPt) {
            wxASSERT(myDlg.m_VertexPts.GetCount() == 1);
            myPt->setX(myDlg.m_VertexPts.Item(0).x);
            myPt->setY(myDlg.m_VertexPts.Item(0).y);
        } else if (myLine) {
            myLine->empty();
            for (unsigned int j = 0; j < myDlg.m_VertexPts.GetCount(); j++) {
                myLine->addPoint(myDlg.m_VertexPts.Item(j).x, myDlg.m_VertexPts.Item(j).y);
            }
        }
        mySelLayer->UpdateGeometry(myGeom, myDlg.m_SelectedOID);
    }
    OGRFeature::DestroyFeature(myFeature);
    wxDELETE(mySelLayer);
    // update display
    wxCommandEvent evt2(tmEVT_LM_UPDATE, wxID_ANY);
    m_ParentEvt->GetEventHandler()->QueueEvent(evt2.Clone());

    return true;
}

/***************************************************************************/ /**
  @brief Merge selected lines
  @details  This function Checks the following rules:
  - Minimum two objects of type lines should been selected.
  - Editing mode should be turned on.
  Merging is done without user input if all selected lines share the same attribution.
  If a line is different, a dialog-box is displayed allowing the user to choice
  the attribution he want to keep for the new merged line.
  @return  true if merging was successfull or allowed, see above
  @author Lucien Schreiber (c) CREALP 2009
  @date 27 February 2009
  *******************************************************************************/
bool tmEditManager::MergeSelectedLines() {
    // verifications
    if (m_TocCtrl->GetEditLayer()->GetType() != TOC_NAME_LINES &&
        m_TocCtrl->GetEditLayer()->GetType() != TOC_NAME_FRAME) {
        return false;
    }

    if (!IsObjectMinNumberSelected(2)) {
        return false;
    }

    // getting selections ids
    wxArrayLong* mySelectedIDs = m_SelectedData->GetSelectedValues();
    wxASSERT(mySelectedIDs);

    // merge lines
    tmGISDataVector* myLayer = (tmGISDataVector*)tmGISData::LoadLayer(m_TocCtrl->GetEditLayer());
    wxASSERT(myLayer);
    OGRGeometry* myMergeGeom = nullptr;
    OGRMultiLineString* mySelGeom = (OGRMultiLineString*)myLayer->GetGeometryColByOID(mySelectedIDs);
    bool bMergeSuccess = myLayer->LinesMerge(mySelGeom, &myMergeGeom);

    OGRGeometryFactory::destroyGeometry(mySelGeom);
    if (!bMergeSuccess) {
        wxMessageBox(_("Selected lines are not adjacent"), _("Merge failed"), wxOK | wxICON_ERROR);
        return false;
    }

    // don't search attribution for frame.
    long myKeptOid = wxNOT_FOUND;
    if (m_TocCtrl->GetEditLayer()->GetType() == TOC_NAME_LINES) {
        // get attributions
        tmAttributionBasicArray myAttributions;
        tmAttributionDataLine myAttrib(m_SelectedData->GetSelectedValues(), m_pDB);
        if (!myAttrib.GetInfoBasicArray(myAttributions)) {
            wxLogDebug(_T("Unable to get attributions from selected lines"));
            return false;
        }

        // FIXME: remove, this is temp logging code
        for (unsigned int k = 0; k < myAttributions.GetCount(); k++) {
            wxString sTmp = wxString::Format(_T("Values for OID : %ld are : "), myAttributions.Item(k).m_Oid);
            for (unsigned int m = 0; m < myAttributions.Item(k).m_Values.GetCount(); m++) {
                sTmp.Append(wxString::Format(_T("%ld, "), myAttributions.Item(k).m_Values.Item(m)));
            }

            wxLogDebug(sTmp);
        }
        // END

        // compare attributions
        int iLineToKeep = 0;
        wxArrayString myConcatenedAttrib;
        if (!tmAttributionData::IsAttributionSimilar(myAttributions)) {
            // get attribution name
            if (!myAttrib.GetConcatenedBasicName(myAttributions, myConcatenedAttrib)) {
                wxLogError(_T("Error getting concatenated basic values"));
                return false;
            }

            // display dialog
            wxSingleChoiceDialog myDlg(m_ParentEvt, _("Select the attribution to be kept"), _("Merge lines"),
                                       myConcatenedAttrib);
            if (myDlg.ShowModal() == wxID_CANCEL) return false;

            iLineToKeep = myDlg.GetSelection();
        }

        // remove lines
        myKeptOid = myAttributions.Item(iLineToKeep).m_Oid;
        wxLogDebug(_T("We keep : OID %ld"), myKeptOid);
        mySelectedIDs->RemoveAt(iLineToKeep);
    } else {
        // arbitriary keep line 0!
        myKeptOid = mySelectedIDs->Item(0);
        mySelectedIDs->RemoveAt(0);
    }

    m_pDB->GeometryDelete(mySelectedIDs, m_TocCtrl->GetEditLayer()->GetType());
    m_pDB->DeleteAttribution(mySelectedIDs, m_TocCtrl->GetEditLayer()->GetType());

    // update number of selected features
    m_SelectedData->Clear();
    m_SelectedData->SetSelected(myKeptOid);
    wxCommandEvent evt(tmEVT_SELECTION_DONE, wxID_ANY);
    m_ParentEvt->GetEventHandler()->QueueEvent(evt.Clone());

    // update geometry
    wxASSERT(myMergeGeom);
    myLayer->UpdateGeometry(myMergeGeom, myKeptOid);
    OGRGeometryFactory::destroyGeometry(myMergeGeom);

    // update display
    wxCommandEvent evt2(tmEVT_LM_UPDATE, wxID_ANY);
    m_ParentEvt->GetEventHandler()->QueueEvent(evt2.Clone());

    return true;
}

void tmEditManager::OnSetRenderFocus(wxCommandEvent& event) {
    wxASSERT(m_Renderer);
    m_Renderer->SetFocus();
}

void tmEditManager::OnEditSharedDown(wxCommandEvent& event) {
    wxLogMessage(_("Searching shared node"));
    wxPoint* myTempPt = (wxPoint*)event.GetClientData();
    wxASSERT(myTempPt);
    wxPoint myLocalPt = *myTempPt;
    wxDELETE(myTempPt);

    if (!SelectedSearch(myLocalPt)) {
        return;
    }

    // update display and selection
    wxCommandEvent evt2(tmEVT_LM_UPDATE, wxID_ANY);
    m_ParentEvt->GetEventHandler()->QueueEvent(evt2.Clone());
    wxCommandEvent evt(tmEVT_SELECTION_DONE, wxID_ANY);
    m_ParentEvt->GetEventHandler()->QueueEvent(evt.Clone());

    // create rect from point
    int myRadius = tmSELECTION_DIAMETER / 2;
    wxRect myClickRect = wxRect(myLocalPt.x - myRadius, myLocalPt.y - myRadius, tmSELECTION_DIAMETER,
                                tmSELECTION_DIAMETER);
    tmRealRect myClickReal = m_Scale->PixelsToReal(myClickRect);
    OGRGeometry* myRect = myClickReal.GetPolygonGeometry();
    wxASSERT(myRect);

    // clear shared nodes
    m_SharedNodes.Clear();

    // search end nodes
    wxArrayLong* mySelIds = m_SelectedData->GetSelectedValues();
    tmGISDataVector* mySelLayer = (tmGISDataVector*)tmGISData::LoadLayer(m_TocCtrl->GetEditLayer());
    if (mySelLayer == nullptr) {
        return;
    }

    for (unsigned int i = 0; i < mySelIds->GetCount(); i++) {
        OGRFeature* myFeature = mySelLayer->GetFeatureByOID(mySelIds->Item(i));
        wxASSERT(myFeature);
        OGRLineString* myLine = (OGRLineString*)myFeature->GetGeometryRef();
        wxASSERT(myLine);
        OGRPoint myNode;
        int myTotalPoints = myLine->getNumPoints();
        myLine->getPoint(0, &myNode);
        int myNodeID = wxNOT_FOUND;
        int myNodePreviousID = wxNOT_FOUND;
        bool bFoundNode = false;

        // search start node
        if (myNode.Within(myRect)) {
            wxLogMessage(_("Found node : 1"));
            myNodeID = 0;
            myNodePreviousID = 1;
            wxRealPoint myCoord(myLine->getX(myNodeID), myLine->getY(myNodeID));
            wxRealPoint myPreviousCoord(myLine->getX(myNodePreviousID), myLine->getY(myNodePreviousID));
            tmSharedNodeEdit mySharedNode(mySelIds->Item(i), myNodeID, m_Scale->RealToPixel(myCoord),
                                          m_Scale->RealToPixel(myPreviousCoord));
            m_SharedNodes.Add(mySharedNode);
            bFoundNode = true;
        }

        // search end node
        myLine->getPoint(myTotalPoints - 1, &myNode);
        if (myNode.Within(myRect)) {
            wxLogMessage(_("Found node : %d"), myTotalPoints - 2);
            myNodeID = myTotalPoints - 1;
            myNodePreviousID = myTotalPoints - 2;
            wxRealPoint myCoord(myLine->getX(myNodeID), myLine->getY(myNodeID));
            wxRealPoint myPreviousCoord(myLine->getX(myNodePreviousID), myLine->getY(myNodePreviousID));
            tmSharedNodeEdit mySharedNode(mySelIds->Item(i), myNodeID, m_Scale->RealToPixel(myCoord),
                                          m_Scale->RealToPixel(myPreviousCoord));
            m_SharedNodes.Add(mySharedNode);
            bFoundNode = true;
        }
        OGRFeature::DestroyFeature(myFeature);

        if (!bFoundNode) {
            wxLogError(_("No Node found for line : %ld"), mySelIds->Item(i));
        }
    }

    if (m_SharedNodes.GetCount() < 2) {
        m_SharedNodes.Clear();
    }
    OGRGeometryFactory::destroyGeometry(myRect);
}

void tmEditManager::OnEditSharedUp(wxCommandEvent& event) {
    wxPoint* myTempPt = (wxPoint*)event.GetClientData();
    wxASSERT(myTempPt);

    if (m_SharedNodes.GetCount() == 0) {
        wxDELETE(myTempPt);
        return;
    }

    tmGISDataVector* mySelLayer = (tmGISDataVector*)tmGISData::LoadLayer(m_TocCtrl->GetEditLayer());
    if (mySelLayer == nullptr) {
        m_SharedNodes.Clear();
        wxDELETE(myTempPt);
        return;
    }

    // clear DC
    {
        wxClientDC dc(m_Renderer);
        wxDCOverlay overlaydc(m_OverlaySharedNodes, &dc);
        overlaydc.Clear();
    }
    m_OverlaySharedNodes.Reset();

    wxRealPoint myNewCoord = m_Scale->PixelToReal(*myTempPt);
    for (unsigned int i = 0; i < m_SharedNodes.GetCount(); i++) {
        OGRFeature* myFeature = mySelLayer->GetFeatureByOID(m_SharedNodes.Item(i).GetLineID());
        wxASSERT(myFeature);
        OGRLineString* myLine = (OGRLineString*)myFeature->GetGeometryRef();
        wxASSERT(myLine);

        myLine->setPoint(m_SharedNodes.Item(i).GetVertexID(), myNewCoord.x, myNewCoord.y, 0);
        mySelLayer->UpdateGeometry(myLine, m_SharedNodes.Item(i).GetLineID());
        OGRFeature::DestroyFeature(myFeature);
    }

    m_SharedNodes.Clear();
    wxDELETE(myTempPt);

    // update display with msg
    wxCommandEvent evt2(tmEVT_LM_UPDATE, wxID_ANY);
    m_ParentEvt->GetEventHandler()->QueueEvent(evt2.Clone());
}

void tmEditManager::OnEditSharedMove(wxCommandEvent& event) {
    wxPoint* myTempPt = (wxPoint*)event.GetClientData();
    wxASSERT(myTempPt);

    wxClientDC myDC(m_Renderer);
    wxDCOverlay myOverlayDC(m_OverlaySharedNodes, &myDC);
    myOverlayDC.Clear();
#ifdef __WXMAC__
    myDC.SetPen(*wxGREY_PEN);
#else
    myDC.SetPen(wxPen(*wxLIGHT_GREY, 2, wxPENSTYLE_SOLID));
#endif

    for (unsigned int i = 0; i < m_SharedNodes.GetCount(); i++) {
        m_SharedNodes.Item(i).SetCoordVertex(*myTempPt);
        m_SharedNodes.Item(i).DrawLine(&myDC);
    }
    wxDELETE(myTempPt);
}

bool tmEditManager::FlipLine() {
    if (!IsLayerTypeSelected(LAYER_SPATIAL_LINE)) {
        return false;
    }

    // Get the Layer (Line MySQL)
    tmGISDataVector* mySelLayer = (tmGISDataVector*)tmGISData::LoadLayer(m_TocCtrl->GetEditLayer());
    if (mySelLayer == nullptr) {
        return false;
    }

    wxArrayLong* mySelected = m_SelectedData->GetSelectedValues();
    wxASSERT(mySelected);
    wxBusyCursor myBusyCursor;

    for (unsigned int f = 0; f < mySelected->GetCount(); f++) {
        OGRFeature* myFeature = mySelLayer->GetFeatureByOID(mySelected->Item(f));
        wxASSERT(myFeature);
        if (myFeature == nullptr) {
            continue;
        }

        // don't delete, internally geometry.
        OGRLineString* myOGRSelLine = (OGRLineString*)myFeature->GetGeometryRef();
        wxASSERT(myOGRSelLine);

#ifdef GDAL_COMPUTE_VERSION /* only available in GDAL 1.10 or later */
        myOGRSelLine->reversePoints();
        mySelLayer->UpdateGeometry(myOGRSelLine, myFeature->GetFID());
#else
        OGRLineString* myTmpLine = (OGRLineString*)OGRGeometryFactory::createGeometry(wkbLineString);
        wxASSERT(myTmpLine);
        for (int i = myOGRSelLine->getNumPoints() - 1; i >= 0; i--) {
            OGRPoint myPoint;
            myOGRSelLine->getPoint(i, &myPoint);
            myTmpLine->addPoint(&myPoint);
        }
        mySelLayer->UpdateGeometry(myTmpLine, myFeature->GetFID());
        OGRGeometryFactory::destroyGeometry(myTmpLine);
#endif
        wxLogMessage(_("Line  %ld flipped"), myFeature->GetFID());
        OGRFeature::DestroyFeature(myFeature);
    }
    wxDELETE(mySelLayer);

    // wxNotificationMessage myMessage( _("Flipping Done!"), wxString::Format(_("%ld line(s) flipped!"),
    // mySelected->GetCount()),m_ParentEvt); myMessage.Show();
    // update display
    wxCommandEvent evt2(tmEVT_LM_UPDATE, wxID_ANY);
    m_ParentEvt->GetEventHandler()->QueueEvent(evt2.Clone());
    return true;
}

bool tmEditManager::SmoothLine() {
    wxASSERT(m_Renderer);
    m_Renderer->SetTool(tmTOOL_SELECT);

    wxBusyCursor myBusyCursor;

    if (!IsModificationAllowed()) {
        return false;
    }

    if (!IsLayerTypeSelected(LAYER_SPATIAL_LINE)) {
        return false;
    }

    _LoadSnappingStatus();

    _ProcessChaikin();

    m_Renderer->Refresh();
    m_Renderer->Update();

    return true;
}

void tmEditManager::_ProcessChaikin() {
    // Minimum of 3 points
    if (m_ArcPoints.GetCount() < 3) {
        return;
    }

    wxRealPointList newPts;

    // First point stays the same
    newPts.push_back(new wxRealPoint(m_ArcPoints[0]->x, m_ArcPoints[0]->y));

    for (int i = 1; i < m_ArcPoints.GetCount(); ++i) {
        wxRealPoint* pt1 = m_ArcPoints[i - 1];
        wxRealPoint* pt2 = m_ArcPoints[i];

        double xQ = (3.0 / 4.0) * pt1->x + (1.0 / 4.0) * pt2->x;
        double yQ = (3.0 / 4.0) * pt1->y + (1.0 / 4.0) * pt2->y;
        double xR = (1.0 / 4.0) * pt1->x + (3.0 / 4.0) * pt2->x;
        double yR = (1.0 / 4.0) * pt1->y + (3.0 / 4.0) * pt2->y;

        newPts.push_back(new wxRealPoint(xQ, yQ));
        newPts.push_back(new wxRealPoint(xR, yR));
    }

    // Last point stays the same
    int iLast = (int)m_ArcPoints.GetCount() - 1;
    newPts.push_back(new wxRealPoint(m_ArcPoints[iLast]->x, m_ArcPoints[iLast]->y));

    m_ArcPoints.DeleteContents(true);
    m_ArcPoints.Clear();

    m_ArcPoints = newPts;
}

tmSharedNodeEdit::tmSharedNodeEdit(long lineid, int vertexid, const wxPoint& coord, const wxPoint& coordprevious) {
    m_LineID = lineid;
    m_VertexID = vertexid;
    m_CoordVertex = coord;
    m_CoordVertexPrevious = coordprevious;
}

tmSharedNodeEdit::~tmSharedNodeEdit() {}

void tmSharedNodeEdit::DrawLine(wxClientDC* dc, wxPoint* point) {
    wxASSERT(dc);
    wxPoint myTempPoint = m_CoordVertex;
    if (point) {
        myTempPoint = *point;
    }
    dc->DrawLine(m_CoordVertexPrevious, myTempPoint);
}

#include <wx/arrimpl.cpp>  // This is a magic incantation which must be done!
WX_DEFINE_OBJARRAY(tmArraySharedNodes);
