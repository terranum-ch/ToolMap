/***************************************************************************
 vrrealrect.h
 support intersection with negative width / height
 -------------------
 copyright            : (C) 2010 CREALP Lucien Schreiber 
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
#ifndef _VRREALRECT_H
#define _VRREALRECT_H

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/geometry.h>



class vrRealRect : public wxRect2DDouble {
  public:
    vrRealRect();
    vrRealRect(const vrRealRect & source);
    vrRealRect(double left, double top, double width, double height);
    virtual ~vrRealRect();


    virtual vrRealRect Intersect(const vrRealRect & rect1);
    bool IsOk();

};
#endif
