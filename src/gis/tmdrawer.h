/***************************************************************************
								tmdrawer.h
						Draws GIS data into bitmap 
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


#ifndef _TM_DRAWER_H_
#define _TM_DRAWER_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "tmgisscale.h"



/***************************************************************************//**
 @brief In charge of all the drawing into bitmaps
 @author Lucien Schreiber (c) CREALP 2008
 @date 24 July 2008
 *******************************************************************************/
class tmDrawer : public wxObject 
	{
	private:
	protected:
	public:
		tmDrawer();
		~tmDrawer();
		
		// Extent Drawing
		bool DrawExtentIntoBitmap (wxBitmap * bitmap,  tmGISScale & scale);
		
		
	};




#endif
