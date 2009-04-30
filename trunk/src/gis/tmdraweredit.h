/***************************************************************************
								tmdraweredit.h
                    Drawing data during editing process
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


#ifndef _TM_DRAWER_EDIT_H_
#define _TM_DRAWER_EDIT_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "../core/tmarraysize.h"

class tmDrawerEditLine : public wxObject
	{
	private:
		wxRealPoint * m_LeftPT;
		wxRealPoint * m_Pt;
		wxRealPoint * m_RightPT;
	
		bool DLIsVertexCreated();
		void DLVertexDelete();
		
	protected:
	
	public:
		tmDrawerEditLine();
		~tmDrawerEditLine();
		
		bool CreateVertex(const wxArrayRealPoints & pts, int index);
		bool IsEndVertex ();
		
	};





#endif
