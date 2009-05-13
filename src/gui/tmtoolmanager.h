/***************************************************************************
								tmtoolmanager.h
                    Take control of all tools (Dangling nodes, etc.)
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


#ifndef _TM_TOOLMANAGER_H_
#define _TM_TOOLMANAGER_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif


#include "danglingnodes_dlg.h"				// for dangling nodes dialog support
#include "../gis/tmtocctrl.h"				// for TOC ctrl 
#include "../gis/tmselecteddatamemory.h"	// for selection data
#include "../database/database_tm.h"		// for database
#include "../gis/tmrenderer.h"				// for GIS rendering

class tmToolManager : public wxEvtHandler
	{
	private:
		// defined by ctor
		wxWindow * m_Parent;
		tmTOCCtrl * m_TOC;
		tmSelectedDataMemory * m_Selected;
		tmRenderer * m_Renderer;
		tmGISScale * m_Scale;
		
		
		// defined by function or event.
		DataBaseTM * m_pDB;
		
		void InitMemberValues();
		
		
		
	protected:
	public:
		tmToolManager(wxWindow * parent,
					  tmTOCCtrl * toc,
					  tmSelectedDataMemory * seldata,
					  tmRenderer * renderer,
					  tmGISScale * scale);
		~tmToolManager();
		
		void SetDatabase(DataBaseTM * db){m_pDB = db;}
		
		bool FindDanglingNodes(){return false;}
		
		
}; 




#endif
