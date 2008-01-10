/***************************************************************************
							prjdefmemmanage.h
						Manage the project in memory 
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


#ifndef PRJDEFMEMMANAGE_H
#define PRJDEFMEMMANAGE_H

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif


// for Memory class definition
#include "projectdefmemory.h"


class PrjDefMemManage
	{
	private:
		// variables
		ProjectDefMemoryLayers * m_pActiveLayer;
		int m_iActualObj;
		
		// member functions
		void InitDefaultValues();
		
	public:
		// variables
		wxString m_PrjPath;
		wxString m_PrjName;
		PRJDEF_UNIT_TYPE m_PrjUnitType;
		PRJDEF_PROJ_TYPE m_PrjProjType;
		PrjMemLayersArray * m_PrjLayerArray;
	
		
		// ctor - dtor
		PrjDefMemManage();
		~PrjDefMemManage();
		
		
		// layer members functions
		ProjectDefMemoryLayers *	AddLayer();
		int							RemoveLayer(int iIndex = -1);// remove last item by default...
		bool						RemoveLayer(const wxString & layerName);
		ProjectDefMemoryLayers *	FindLayer(const wxString & layerName);
		void						SetActiveLayer(ProjectDefMemoryLayers * ActiveLayer)
													{m_pActiveLayer = ActiveLayer;}
		ProjectDefMemoryLayers *	GetActiveLayer(){return m_pActiveLayer;}
	
		// object members functions
		ProjectDefMemoryObjects *	AddObject ();
		int							RemoveObject (int iIndex = -1); // remove last
		bool						RemoveObject(const wxString & ObjectName);
		ProjectDefMemoryObjects *	FindObject(const wxString & ObjectName);
		ProjectDefMemoryObjects *	GetNextObjects();
		int							GetCountObject();
		
		
	};

#endif

