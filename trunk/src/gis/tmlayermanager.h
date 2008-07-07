/***************************************************************************
								tmlayermanager.h
                    Central point for layer management 
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


#ifndef _TM_LAYERMANAGER_H_
#define _TM_LAYERMANAGER_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "tmtocctrl.h"					// class TOC ctrl
#include "../database/database_tm.h"	// class database



class tmLayerManager : public wxEvtHandler 
	{
	private:
		// members
		tmTOCCtrl * m_TOCCtrl;
		wxWindow * m_Parent;
		DataBaseTM * m_DB;
		
		void InitMemberValue();
		
		// TOC specific functions
		void FillTOCArray();
		
		
	public:
		tmLayerManager(wxWindow * parent, tmTOCCtrl * tocctrl );
		~tmLayerManager();
		
		// setter, getter
		void SetDataBase (DataBaseTM * db) {m_DB = db;}
		
		// Init layermanager
		bool InitLayerManager(DataBaseTM * db);
		bool UnInitLayerManager();
		
		
		
		
		// adding layer
		
		
		
	};





#endif
