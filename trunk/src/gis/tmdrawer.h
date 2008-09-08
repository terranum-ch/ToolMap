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
#include "tmlayerproperties.h"



/***************************************************************************//**
 @brief In charge of all the drawing into bitmaps
 @author Lucien Schreiber (c) CREALP 2008
 @date 24 July 2008
 *******************************************************************************/
class tmDrawer : public wxObject 
	{
	private:
		wxBitmap * m_bmp;
		tmGISScale m_scale;
		bool m_IsInitialised;
		
		bool DrawLines(tmLayerProperties * itemProp);
		
	protected:
	public:
		tmDrawer();
		~tmDrawer();
		
		void InitDrawer (wxBitmap * bitmap, tmGISScale & scale);
		
		// Extent Drawing
		bool DrawExtentIntoBitmap (int width = 1, const wxColour & col = wxColour(*wxBLACK));
		bool Draw (tmLayerProperties * itemProp);
		
		
	};




#endif
