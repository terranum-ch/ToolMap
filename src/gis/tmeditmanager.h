/***************************************************************************
								tmeditmanager.h
				Deals with editing data in construction layers
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


#ifndef _TM_EDIT_MANAGER_H_
#define _TM_EDIT_MANAGER_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif


#include "tmtocctrl.h"						// for TOC ctrl 
#include "tmselecteddatamemory.h"			// for selection data
#include "../database/database_tm.h"		// for database
//#include "tmmanagerevent.h"					// for shared event with other manager
#include "tmrenderer.h"						// for GIS rendering
#include "../core/tmsnappingmemory.h"		// for snapping 


/***************************************************************************//**
 @brief Deals with editing data
 @details This class is in charge of all stuff related to the editing process
 (points, lines)
 @author Lucien Schreiber (c) CREALP 2009
 @date 26 January 2009
 *******************************************************************************/
class tmEditManager : public wxEvtHandler
	{
	private:
		// defined by ctor
		wxWindow * m_ParentEvt;
		tmTOCCtrl * m_TOC;
		tmSelectedDataMemory * m_SelectedData;
		tmRenderer * m_Renderer;
		
		// defined by function or event.
		DataBaseTM * m_pDB;
		tmSnappingMemory * m_SnapMem;
		
		
		// FUNCTIONS
		void InitMemberValues();
		void OnSnappingChange (wxCommandEvent & event);
		
		
		DECLARE_EVENT_TABLE()
	protected:
		
	public:
		// ctor - dtor
		tmEditManager(wxWindow * parent,
					  tmTOCCtrl * toc,
					  tmSelectedDataMemory * seldata,
					  tmRenderer * renderer);
		~tmEditManager();
		
		//! @brief Set database functions.
		//! If database is valid then project is open.
		void SetDatabase (DataBaseTM * database){m_pDB = database;}
	
		//change tool functions
		void OnToolEdit ();
		void OnToolModify ();
	};




#endif
