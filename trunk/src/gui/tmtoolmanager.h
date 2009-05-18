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
#include "../gis/tooldanglingnodes.h"		// for computing dangling nodes (engine class)



class tmToolManager : public wxEvtHandler
	{
	private:
		// defined by ctor
		wxWindow * m_Parent;
		tmTOCCtrl * m_TOC;
		tmSelectedDataMemory * m_Selected;
		tmRenderer * m_Renderer;
		tmGISScale * m_Scale;
		wxArrayRealPoints m_DanglingPts;
		
		
		// defined by function or event.
		DataBaseTM * m_pDB;
		
		// commun private functions
		void InitMemberValues();
		bool TMIsOk();
		
		// dangling nodes private functions
		bool TMGetLayers(wxArrayString & layersname);
		bool TMSearchDanglingNodes(int selectedlayer, 
								   const wxArrayString & layersname);
		bool TMGetLayersIDs (wxArrayLong & layersid, const wxArrayString & layersname);
		void TMClearDangling(){m_DanglingPts.Clear();}
		
	protected:
	public:
		tmToolManager(wxWindow * parent,
					  tmTOCCtrl * toc,
					  tmSelectedDataMemory * seldata,
					  tmRenderer * renderer,
					  tmGISScale * scale);
		~tmToolManager();
		
		void SetDatabase(DataBaseTM * db){m_pDB = db;}
		
		// find dangling nodes
		bool FindDanglingNodes();
		
}; 




#endif
