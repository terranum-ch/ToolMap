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
#include "tmgisdata.h"					// for GISdata
#include "tmgisdatavectormysql.h"		// for direct access to GIS mysql




/***************************************************************************//**
 @brief GIS class for dealing with layers
 @details This is the super class for dealing with GIS. This class is the
 central point dealing with : 
 - Database (#DataBaseTM)
 - TOC controls (#tmTOCCtrl)
 - GIS progress indicator (#tmProgressIndicator)
 - ...
 @author Lucien Schreiber (c) CREALP 2008
 @date 07 July 2008
 *******************************************************************************/
class tmLayerManager : public wxEvtHandler 
	{
	private:
		tmTOCCtrl * m_TOCCtrl;
		wxWindow * m_Parent;
		DataBaseTM * m_DB;
		
		// init member values to default
		void InitMemberValue();
		
		// TOC specific functions
		void FillTOCArray();
		bool SaveTOCStatus();
		
		// layer specific functions
		tmGISData * LoadLayer (tmLayerProperties * layerProp);
		
		DECLARE_EVENT_TABLE()
		
	public:
		// ctor / dtor
		tmLayerManager(wxWindow * parent, tmTOCCtrl * tocctrl );
		~tmLayerManager();
		
			
		// Init layermanager
		bool InitLayerManager(DataBaseTM * db);
		bool UnInitLayerManager();
		
		
		// layers operations
		void RemoveLayer (wxCommandEvent & event);
		void AddLayer (wxCommandEvent & event);
		
		bool LoadProjectLayers();
		
		
		
	};





#endif
