/***************************************************************************
 tmdraweredit.cpp
 Drawing data during editing process
 -------------------
 copyright : (C) 2009 CREALP Lucien Schreiber
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

#include "tmdraweredit.h"

tmDrawerEditLine::tmDrawerEditLine() {
    m_LeftPT = nullptr;
    m_Pt = nullptr;
    m_RightPT = nullptr;
    m_Index = wxNOT_FOUND;
    m_EditPen = *wxBLACK_PEN;
}

tmDrawerEditLine::~tmDrawerEditLine() {
    if (m_Pt != nullptr) {
        DLVertexDelete();
    }

    wxASSERT(m_LeftPT == nullptr);
    wxASSERT(m_RightPT == nullptr);
}

bool tmDrawerEditLine::IsOK() {
    if (m_Pt != nullptr) return true;

    return false;
}

void tmDrawerEditLine::DLVertexDelete() {
    wxASSERT(m_Pt != nullptr);
    delete m_Pt;
    m_Pt = nullptr;

    if (m_LeftPT != nullptr) {
        delete m_LeftPT;
        m_LeftPT = nullptr;
    }

    if (m_RightPT != nullptr) {
        delete m_RightPT;
        m_RightPT = nullptr;
    }

    m_Index = wxNOT_FOUND;
}

int tmDrawerEditLine::DLGetCountPoints() {
    wxASSERT(m_Pt != nullptr);
    if (IsEndVertex()) return 2;

    return 3;
}

wxPoint* tmDrawerEditLine::DLGetPoints() {
    wxASSERT(m_Pt != nullptr);
    wxPoint* pPts = new wxPoint[DLGetCountPoints()];

    int i = 0;

    if (m_LeftPT != nullptr) {
        pPts[i].x = m_LeftPT->x;
        pPts[i].y = m_LeftPT->y;
        i++;
    }

    pPts[i].x = m_Pt->x;
    pPts[i].y = m_Pt->y;
    i++;

    if (m_RightPT != nullptr) {
        pPts[i].x = m_RightPT->x;
        pPts[i].y = m_RightPT->y;
    }

    return pPts;
}

void tmDrawerEditLine::DLReorderVertex() {
    if (IsEndVertex()) {
        if (m_RightPT != nullptr) {
            wxASSERT(m_LeftPT == nullptr);
            m_LeftPT = new wxPoint(*m_RightPT);
            delete m_RightPT;
            m_RightPT = nullptr;
        }
    }
}

bool tmDrawerEditLine::CreateVertex(const wxArrayPoints& pts, int index) {
    wxASSERT(index >= 0);
    if (IsOK()) DLVertexDelete();

    if (pts.GetCount() <= 1) {
        wxLogError(_("Not enough vertex"));
        return false;
    }

    if (index >= (signed)pts.GetCount()) {
        wxString myErr = _("Trying to get a vertex outside of array");
        wxLogError(myErr);
        return false;
    }

    wxASSERT(m_Pt == nullptr);
    m_Pt = new wxPoint(pts.Item(index));
    m_Index = index;

    if (index - 1 >= 0) m_LeftPT = new wxPoint(pts.Item(index - 1));

    if (index + 1 < (signed)pts.GetCount()) m_RightPT = new wxPoint(pts.Item(index + 1));

    DLReorderVertex();
    return true;
}

bool tmDrawerEditLine::CreateVertex(const wxPoint& pt, wxPoint* left, wxPoint* right, int index) {
    if (IsOK()) DLVertexDelete();

    if (left == nullptr && right == nullptr) {
        wxLogDebug(_("Left or right should not be NULL"));
        return false;
    }

    wxASSERT(m_Pt == nullptr);
    m_Pt = new wxPoint(pt);
    m_Index = index;

    if (left != nullptr) m_LeftPT = new wxPoint(*left);

    if (right != nullptr) m_RightPT = new wxPoint(*right);

    DLReorderVertex();
    return true;
}

bool tmDrawerEditLine::CreateVertex(const wxPoint& pt) {
    if (IsOK()) DLVertexDelete();

    wxASSERT(m_Pt == nullptr);
    m_Pt = new wxPoint(pt);

    m_LeftPT = new wxPoint(pt);
    return true;
}

bool tmDrawerEditLine::IsEndVertex() {
    if (m_LeftPT == nullptr || m_RightPT == nullptr) return true;

    return false;
}

bool tmDrawerEditLine::SetVertex(const wxPoint& pt) {
    if (!IsOK()) {
        wxLogDebug(
            _T("Error setting vertex, no existing vertex ! Use")
            _T(" CreateVertex() function first"));
        return false;
    }

    wxASSERT(m_Pt);
    m_Pt->x = pt.x;
    m_Pt->y = pt.y;
    return true;
}

void tmDrawerEditLine::SetSymbology(wxColour col, int width) {
    m_EditPen.SetColour(col);
    m_EditPen.SetWidth(width);
}

bool tmDrawerEditLine::DrawEditPart(wxClientDC* pdc) {
    if (!IsOK()) {
        wxLogDebug(_T("Error CreateVertex() first"));
        return false;
    }

    wxASSERT(pdc != nullptr);

    wxDCOverlay myOverlayDC(m_Overlay, pdc);
    myOverlayDC.Clear();
#ifdef __WXMAC__
    pdc->SetPen(*wxGREY_PEN);
#else
    pdc->SetPen(wxPen(*wxLIGHT_GREY, 2, wxPENSTYLE_SOLID));
#endif

    int iNumPts = DLGetCountPoints();
    wxASSERT(iNumPts >= 2 && iNumPts <= 3);
    wxPoint* myPts = DLGetPoints();
    wxASSERT(myPts);
    pdc->DrawLines(iNumPts, myPts);
    wxDELETEA(myPts);

    return true;
}

void tmDrawerEditLine::DrawEditReset(wxClientDC* pdc) {
    {
        wxDCOverlay overlaydc(m_Overlay, pdc);
        overlaydc.Clear();
    }
    m_Overlay.Reset();
}

bool tmDrawerEditLine::ClearVertex() {
    if (!IsOK()) {
        wxLogDebug(_T("Nothing to clear, CreateVertex() first"));
        return false;
    }

    DLVertexDelete();
    return true;
}
