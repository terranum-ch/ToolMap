/***************************************************************************
 tmpointorient.cpp
                    for dealing with point orientation
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

#include "tmpointorient.h"


tmPointOrient::tmPointOrient()
{
    InitMemberValues();
}


void tmPointOrient::InitMemberValues()
{
    m_StartPoint = wxDefaultPosition;
    m_EndPoint = wxDefaultPosition;
}


tmPointOrient::~tmPointOrient()
{

}


void tmPointOrient::SetStartPoint(const wxPoint &start)
{
    m_StartPoint = start;
}


void tmPointOrient::SetEndPoint(const wxPoint &end)
{
    m_EndPoint = end;
}


bool tmPointOrient::IsValid()
{
    if (m_StartPoint == wxDefaultPosition ||
        m_EndPoint == wxDefaultPosition) {
        wxLogError(_("Orientation not correctly inited"));
        return false;
    }

    if (m_StartPoint == m_EndPoint) {
        wxLogError(_("Not enough data for computing orientation"));
        return false;
    }
    return true;
}


int tmPointOrient::GetOrientationInt()
{
    return (int) GetOrientationDouble();
}


double tmPointOrient::GetOrientationDouble()
{
    if (IsValid() == false)
        return wxNOT_FOUND;

    // get two vectors
    wxPoint2DDouble myVector1 = wxPoint2DDouble(m_EndPoint.x - m_StartPoint.x, m_EndPoint.y - m_StartPoint.y);
    wxPoint2DDouble myVectorVert = wxPoint2DDouble(0, -1);

    // vector length for normalize
    myVector1.Normalize();

    double myDotProduct = myVectorVert.GetDotProduct(myVector1);
    double myCrossProduct = myVectorVert.GetCrossProduct(myVector1);

    double myCosRadVal = acos(myDotProduct);
    double mySinRadVal = asin(myCrossProduct);
    if (mySinRadVal < 0)
        myCosRadVal = 2 * M_PI - myCosRadVal;


    //double myRadialVal = atan2(myVector1.m_y, myVector1.m_x) - atan2(myVectorVert.m_y, myVectorVert.m_x);
    return 180 * myCosRadVal / M_PI;
}


