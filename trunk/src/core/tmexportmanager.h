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
#include <wx/busyinfo.h>				// for busy info dialog, remove if no more needed
#include <wx/progdlg.h>					// the progress dialog


class tmExportManager : public wxObject
	{
	private:
		DataBaseTM * m_pDB;
		wxWindow * m_Parent;
		PRJDEF_EXPORT_TYPE m_ExportType;
		wxFileName m_ExportPath;
		tmExportData * m_ExportData;

		PrjDefMemManage * m_ProjMem;
		
		// init values
		void InitMemberValues();
		
		// export function
		bool ExportLayers (PrjMemLayersArray * layers);
		bool CreateExportLayer (ProjectDefMemoryLayers * layer);
		bool ExportGISData (ProjectDefMemoryLayers * layer);
		wxRealPoint * GetFrame (int & nbvertex);
		bool AddAttributionSimpleData (ProjectDefMemoryLayers * layer);
		bool AddAttributionAdvanced (ProjectDefMemoryLayers * layer);
		
		// check path
		bool IsExportPathValid();
		
		// create tmExportData object
		tmExportData * CreateExportData ();
		
		// progress function and variables
		wxProgressDialog * m_ProgressDlg;
		wxBusyInfo * m_ProgressBusy;
		wxString m_ProgressText;
		void CreateProgress(int iNbLayers, const wxString & layername);
		bool UpdateProgress(int iActualLayer, const wxString & layername);
		void DeleteProgress();
		
	protected:
	public:
		
		// construction and destruction
		tmExportManager();
		~tmExportManager();
		tmExportManager(wxWindow * parent, DataBaseTM * database);
		void Create (wxWindow * parent, DataBaseTM * database);
		
		// Export public function
		bool ExportAll (PrjDefMemManage * localprojdef);
		bool ExportSelected (PrjDefMemManage * localprojdef);
};




#endif
