/***************************************************************************
 bezierethz
 -------------------
 copyright            : (C) 2013 CREALP Lucien Schreiber
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "bezierethz.h"

BezierEthz::BezierEthz(double width, int maxpoints) {
  m_Width = width;
  m_MaxPoints = maxpoints;
}

BezierEthz::~BezierEthz() {
  m_Points.DeleteContents(true);
  m_Points.Clear();
}

void BezierEthz::_BezierPoint(const wxRealPoint &A, const wxRealPoint &B, wxRealPoint &result, double t) {
  result.y = A.y + (double)(B.y - A.y) * t;
  result.x = A.x + (double)(B.x - A.x) * t;
}

bool BezierEthz::_Segmentation(const wxRealPoint &P1, const wxRealPoint &P2, const wxRealPoint &P3,
                               const wxRealPoint &P4, double t, wxRealPoint &result) {
  wxRealPoint L2;
  wxRealPoint L3;
  wxRealPoint R2;
  wxRealPoint R3;
  wxRealPoint H;

  if (t <= 0 || t > 1.0) {
    return false;
  }
  _BezierPoint(P1, P2, L2, t);
  _BezierPoint(P2, P3, H, t);
  _BezierPoint(L2, H, L3, t);
  _BezierPoint(P3, P4, R3, t);
  _BezierPoint(H, R3, R2, t);
  _BezierPoint(L3, R2, result, t);
  return true;
}

bool BezierEthz::_IsPointUsefull(const wxRealPoint &PBefore, const wxRealPoint &P, const wxRealPoint &PAfter) {
  wxPoint2DDouble pBefore(PBefore.x, PBefore.y);
  wxPoint2DDouble p(P.x, P.y);
  wxPoint2DDouble pAfter(PAfter.x, PAfter.y);

  double a = pBefore.GetDistance(p);
  double b = p.GetDistance(pAfter);
  double c = pBefore.GetDistance(pAfter);

  double s = 0.5 * (a + b + c);
  double area = sqrt(s * (s - a) * (s - b) * (s - c));
  double height = 2.0 * area / c;
  if (height <= m_Width) {
    return false;
  }
  return true;
}

void BezierEthz::Init(wxRealPoint P1, wxRealPoint P2, wxRealPoint P3, wxRealPoint P4) {
  double myLeftVal = (double)1.0 / m_MaxPoints;
  m_Points.DeleteContents(true);
  m_Points.Clear();

  // segmentation
  double myInterval = myLeftVal;
  while (1) {
    wxRealPoint myResult;
    if (!_Segmentation(P1, P2, P3, P4, myInterval, myResult)) {
      break;
    }
    myInterval += myLeftVal;
    m_Points.push_back(new wxRealPoint(myResult));
  }

  // simplification
  m_Points.DeleteContents(true);
  for (unsigned int i = 1; i < m_Points.GetCount() - 1; i++) {
    if (!_IsPointUsefull(*m_Points[i - 1], *m_Points[i], *m_Points[i + 1])) {
      m_Points.Erase(m_Points.Item(i));
      i = 1;
    }
  }
}

#include <wx/listimpl.cpp>

WX_DEFINE_LIST(wxRealPointList);
