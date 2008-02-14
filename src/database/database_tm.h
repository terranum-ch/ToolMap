/***************************************************************************
								database_tm.h
                    extension of the database class for ToolMap
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


#ifndef DATABASE_TM_H
#define DATABASE_TM_H

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "database.h"
#include "../core/projectdefmemory.h"		// for transfering data directely

// TABLES NAMES
const wxString TABLE_NAME_LAYERS = _T("THEMATIC_LAYERS");
const wxString TABLE_NAME_OBJECTS = _T("DMN_LAYER_OBJECT");
const wxString TABLE_NAME_LAYER_AT = _T("LAYER_AT");




class DataBaseTM : public DataBase
	{
	private:
		int m_iDBLayerIndex;
		
		// field operations private
		bool AddTableIfNotExist (const int & iLayerIndex);
		bool CreateFieldInteger (ProjectDefMemoryFields * myField, wxString TableName);
		bool CreateFieldText (ProjectDefMemoryFields * myField, wxString TableName);
		bool CreateFieldDouble (ProjectDefMemoryFields * myField, wxString TableName);
		bool CreateFieldDate (ProjectDefMemoryFields * myField, wxString TableName);
		
		
	public:
		DataBaseTM();
		~DataBaseTM();
		
		bool CreateEmptyTMDatabase();
		
		// Database layer operations
		bool AddLayer(ProjectDefMemoryLayers * myLayer); 
		void SetActiveLayerId (ProjectDefMemoryLayers * myLayer);
		int GetActiveLayerId () {return m_iDBLayerIndex;}
		
		// Database object operations
		bool AddObject (ProjectDefMemoryObjects * myObject, int DBlayerIndex=-1);
		
		// field operations
		bool AddField (ProjectDefMemoryFields * myField, int DBlayerIndex = -1);
	};




#endif
