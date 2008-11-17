/***************************************************************************
								tmexportmanager.h
                    Main class for dealing with export process
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


#ifndef _TM_EXPORTMANAGER_H_
#define _TM_EXPORTMANAGER_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif


#include "../database/database_tm.h"	// for database access
#include "projectdefmemory.h"			// for PojectDefMemoryLayers definition

#include "tmexportdatashp.h"			// for exporting data in SHP



class tmExportManager : public wxObject
	{
	private:
		DataBaseTM * m_pDB;
		wxWindow * m_Parent;
		PRJDEF_EXPORT_TYPE m_ExportType;
		wxFileName m_ExportPath;
		tmExportData * m_ExportData;

		
		// init values
		void InitMemberValues();
		
		// getting layers and fields
		PrjMemLayersArray * GetAllLayers ();
		PrjMemFieldArray * GetAllFieldsForLayer(ProjectDefMemoryLayers * layer);
		
		// export function
		bool ExportLayers (PrjMemLayersArray * layers);
		bool CreateExportLayer (ProjectDefMemoryLayers * layer);
		bool ExportGISData (ProjectDefMemoryLayers * layer);
		wxRealPoint * GetFrame (int & nbvertex);
		bool AddAttributionSimpleData (ProjectDefMemoryLayers * layer);
		
		// check path
		bool IsExportPathValid();
		
		// create tmExportData object
		tmExportData * CreateExportData ();
		
	protected:
	public:
		
		// construction and destruction
		tmExportManager();
		~tmExportManager();
		tmExportManager(wxWindow * parent, DataBaseTM * database);
		void Create (wxWindow * parent, DataBaseTM * database);
		
		// Export public function
		bool ExportAll (){ return false;}
		bool ExportSelected ();
};




#endif
