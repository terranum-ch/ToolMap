/***************************************************************************
 tmarraysize.cpp
  Array of wxSize items
 -------------------
 copyright : (C) 2009 CREALP Lucien Schreiber
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

#include "tmarraysize.h"

tmRealPointDist::tmRealPointDist(double xx, double yy, const wxRealPoint &originorigin)
{
    coordinate = wxRealPoint(xx, yy);
    origin = originorigin;
}


tmRealPointDist::tmRealPointDist(const tmRealPointDist &pt)
{
    coordinate = pt.GetCoordinate();
    origin = pt.GetOrigin();
}


double tmRealPointDist::GetDistFromOrigin() const
{
    double distance1_x = coordinate.x - origin.x;
    double distance1_y = coordinate.y - origin.y;
    double dist1 = sqrt((distance1_x * distance1_x) + (distance1_y * distance1_y));
    return dist1;
}


tmRealPointDist &tmRealPointDist::operator=(const tmRealPointDist &source)
{
    coordinate = source.GetCoordinate();
    origin = source.GetOrigin();
    return *this;
}


#include <wx/arrimpl.cpp>
WX_DEFINE_OBJARRAY (tmArraySize);
WX_DEFINE_OBJARRAY (wxArrayRealPoints);
WX_DEFINE_OBJARRAY (wxArrayRealPointsDist);
WX_DEFINE_OBJARRAY (wxArrayPoints);

