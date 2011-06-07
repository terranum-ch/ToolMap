/***************************************************************************
								tmarraysize.h
							Array of wxSize items
                             -------------------
    copyright            : (C) 2009 CREALP Lucien Schreiber 
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


#ifndef _TM_ARRAY_SIZE_H_
#define _TM_ARRAY_SIZE_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif


class tmRealPointDist{
private:
	wxRealPoint coordinate;
    wxRealPoint origin;
	
public:
    tmRealPointDist() : coordinate(), origin() { }
    tmRealPointDist(double xx, double yy, const wxRealPoint & originorigin);
    tmRealPointDist(const tmRealPointDist& pt);
	
	double GetDistFromOrigin() const;
	wxRealPoint GetOrigin() const {return origin;}
	wxRealPoint GetCoordinate() const {return coordinate;}
	
	// copy function
	tmRealPointDist &operator = (const tmRealPointDist & source);
};

WX_DECLARE_OBJARRAY(tmRealPointDist, wxArrayRealPointsDist);
WX_DECLARE_OBJARRAY(wxSize, tmArraySize);	
WX_DECLARE_OBJARRAY(wxRealPoint, wxArrayRealPoints);
WX_DECLARE_OBJARRAY(wxPoint, wxArrayPoints);


#endif
