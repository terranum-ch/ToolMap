/***************************************************************************
								tmimgfunc.h
							Generic images functions
                             -------------------
    copyright            : (C) 2007 CREALP Lucien Schreiber 
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


#ifndef _TM_IMGFUNCT_H_
#define _TM_IMGFUNCT_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <wx/mstream.h>
#define wxGetBitmapFromMemory(name) _wxGetBitmapFromMemory(name ## _png, sizeof(name ## _png))

inline wxBitmap _wxGetBitmapFromMemory(const unsigned char *data, int length) {
	wxMemoryInputStream is(data, length);
	return wxBitmap(wxImage(is, wxBITMAP_TYPE_ANY, -1), -1);
}

#endif