/***************************************************************************
 bezierpoints.cpp
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

#include "bezierpoints.h"
#include "../gis/tmgisscale.h"


tmBezierPointInt::tmBezierPointInt(){
    Reset();
}

tmBezierPointInt::tmBezierPointInt (const tmBezierPointInt & point){
    m_Point = point.m_Point;
    m_Control = point.m_Control;
}


tmBezierPointInt::~tmBezierPointInt(){
}


void tmBezierPointInt::Reset (){
    m_Point = wxDefaultPosition;
    m_Control = wxDefaultPosition;
}

bool tmBezierPointInt::IsFullyDefined (){
    if (m_Point == wxDefaultPosition) {
        return false;
    }
    if (m_Control == wxDefaultPosition){
        return false;
    }
    return true;
}



#include <wx/listimpl.cpp>
WX_DEFINE_LIST(tmListBezierPointInt);





tmBezierPointReal::tmBezierPointReal(){
    Reset();
}

tmBezierPointReal::tmBezierPointReal (const tmBezierPointReal & point){
    m_Point = point.m_Point;
    m_Control = point.m_Control;
}


tmBezierPointReal::tmBezierPointReal (const tmBezierPointInt & pointpx, tmGISScale * scale){
    wxASSERT(scale);
    if (pointpx.m_Point != wxDefaultPosition){
        m_Point = scale->PixelToReal(pointpx.m_Point);
    }
    if (pointpx.m_Control != wxDefaultPosition) {
        m_Control = scale->PixelToReal(pointpx.m_Control);
    }
}



tmBezierPointReal::~tmBezierPointReal(){
}


void tmBezierPointReal::Reset (){
    m_Point = wxRealPoint(-1,-1);
    m_Control = wxRealPoint(-1,-1);
}

bool tmBezierPointReal::IsFullyDefined (){
    if (m_Point == wxRealPoint(-1,-1)) {
        return false;
    }
    if (m_Control == wxRealPoint(-1,-1)){
        return false;
    }
    return true;
}


WX_DEFINE_LIST(tmListBezierPointReal);



