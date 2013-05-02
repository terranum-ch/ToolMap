/***************************************************************************
 bezierpoints.h
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
#ifndef _BEZIER_POINTS_H_
#define _BEZIER_POINTS_H_

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <wx/list.h>

class tmGISScale;


class tmBezierPointInt {
public:
    tmBezierPointInt();
    tmBezierPointInt (const tmBezierPointInt & point);
    ~tmBezierPointInt();
    wxPoint GetInvertedControlPoint () {return m_Point - (m_Control - m_Point);}
    void Reset ();
    bool IsFullyDefined ();
    
    wxPoint m_Point;
    wxPoint m_Control;
};


WX_DECLARE_LIST(tmBezierPointInt, tmListBezierPointInt);








class tmBezierPointReal {
public:
    tmBezierPointReal();
    tmBezierPointReal (const tmBezierPointReal & point);
    tmBezierPointReal (const tmBezierPointInt & pointpx, tmGISScale * scale);
    ~tmBezierPointReal();
    wxRealPoint GetInvertedControlPoint () {return m_Point - (m_Control - m_Point);}
    void Reset ();
    bool IsFullyDefined ();
    
    wxRealPoint m_Point;
    wxRealPoint m_Control;
};

WX_DECLARE_LIST(tmBezierPointReal, tmListBezierPointReal);


#endif
