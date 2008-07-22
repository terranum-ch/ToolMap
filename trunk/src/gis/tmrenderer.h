/***************************************************************************
								tmrenderer.h
                    Deals with the main renderer windows
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


#ifndef _TM_GISRENDERER_H_
#define _TM_GISRENDERER_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <wx/scrolwin.h>	// for scrolled window base


// EVENT FOR GIS RENDERER CTRL
DECLARE_EVENT_TYPE(tmEVT_LM_SIZE_CHANGED, -1)


class tmRenderer : public wxScrolledWindow
	{
	private:
		wxBitmap * m_bmp;
		
		DECLARE_EVENT_TABLE()
	protected:
		 
		
	public:
		tmRenderer(wxWindow * parent, wxWindowID id);
		~tmRenderer(){;}
		
		void OnSizeChange(wxSizeEvent & event);
		
		void OnPaint(wxPaintEvent & event);
		
		void SetBitmapStatus(wxBitmap * bmp = NULL) {m_bmp = bmp;}

		
	};




#endif
