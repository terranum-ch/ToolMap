/***************************************************************************
 bezierethz
 -------------------
 copyright            : (C) 2013 CREALP Lucien Schreiber 
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

#ifndef _BEZIER_ETHZ_H_
#define _BEZIER_ETHZ_H_

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

WX_DECLARE_LIST(wxRealPoint, wxRealPointList);


class BezierEthz
{
public:
    BezierEthz(double width, int maxpoints);

    ~BezierEthz();

    void Init(wxRealPoint P1, wxRealPoint P2, wxRealPoint P3, wxRealPoint P4);

    wxRealPointList *GetPointListRef()
    { return &m_Points; }

private:
    double m_Width;
    int m_MaxPoints;
    wxRealPointList m_Points;

    void _BezierPoint(const wxRealPoint &A, const wxRealPoint &B, wxRealPoint &result, double t);

    bool _Segmentation(const wxRealPoint &P1, const wxRealPoint &P2, const wxRealPoint &P3, const wxRealPoint &P4,
                       double t, wxRealPoint &result);

    bool _IsPointUsefull(const wxRealPoint &PBefore, const wxRealPoint &P, const wxRealPoint &PAfter);

};


#endif
