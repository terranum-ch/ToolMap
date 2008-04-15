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
#include "../core/prjdefmemmanage.h"		// for transfering data directely
#include "../core/wxdirpickerctrlbest.h"	// for PATH_ERROR definition;
#include "../gui/projectproperties_dlg.h"	// for scale RANK function 


// TABLES NAMES
const wxString TABLE_NAME_LAYERS = _T("THEMATIC_LAYERS");
const wxString TABLE_NAME_OBJECTS = _T("DMN_LAYER_OBJECT");
const wxString TABLE_NAME_LAYER_AT = _T("LAYER_AT");
const wxString TABLE_NAME_PRJ_SETTINGS = _T("PRJ_SETTINGS");
const wxString TABLE_NAME_LAYER_TYPE = _T("DMN_LAYER_TYPE");
const wxString TABLE_NAME_SCALE			= _T("ZOOM_LEVEL");

// DATABASE VERSION IS
const int TM_DATABASE_VERSION = 204;


class DataBaseTM : public DataBase
	{
	private:
		int m_iDBLayerIndex;
		
		// field operations private (creation)
		bool AddTableIfNotExist (const wxString & TableName);
		bool CreateFieldInteger (ProjectDefMemoryFields * myField, const wxString & TableName);
		bool CreateFieldText (ProjectDefMemoryFields * myField, const wxString & TableName);
		bool CreateFieldDouble (ProjectDefMemoryFields * myField, const wxString & TableName);
		bool CreateFieldDate (ProjectDefMemoryFields * myField, const wxString & TableName);
		bool CreateFieldEnum (ProjectDefMemoryFields * myField, const wxString & TableName);
		
	public:
		DataBaseTM();
		~DataBaseTM();
		
		
		// general database operations
		bool FillLayerTableTypeData ();
		bool FillDefaultScaleData ();
		bool CreateEmptyTMDatabase();
		
		// Database project operations
		bool IsProjectDataDefined ();
		bool SetProjectData (PrjDefMemManage * pPrjDefinition);
		bool GetProjectData (PrjDefMemManage * pPrjDefinition);
		int	 GetDatabaseToolMapVersion ();
		bool SetProjectExportData (int iExportType, const wxString & spath);
		bool SetProjectBackupPath (const wxString & spath);
		int	 GetProjectExportData (int & iExportType, wxString &spath);
		int  GetProjectBackupPath (wxString & spath);
		
		
		// Database layer operations
		bool AddLayer(ProjectDefMemoryLayers * myLayer); 
		void SetActiveLayerId (ProjectDefMemoryLayers * myLayer);
		int GetActiveLayerId () {return m_iDBLayerIndex;}
		int GetNextLayer (ProjectDefMemoryLayers * myLayer);
		bool UpdateLayer (ProjectDefMemoryLayers * myLayer, wxString & sSqlSentence);
		
		// Database object operations
		bool AddObject (ProjectDefMemoryObjects * myObject, int DBlayerIndex=-1);
		bool EditObject (ProjectDefMemoryObjects * myObject );
		bool DataBaseGetNextResultAsObject(ProjectDefMemoryObjects * object, int ilayertype);
		int  DeleteMultipleObjects (PrjDefMemManage * pProjet);
		
		// field operations
		bool AddField (ProjectDefMemoryFields * myField, int DBlayerIndex = -1);
		
		// database query
		bool GetObjectListByLayerType(int ilayertype);
		wxArrayString GetLayerNameByType (int ilayertype);
		
		// database Scale operations
		long GetNextScaleValue (long & DBindex);
		bool EditScale (ProjectDefMemoryScale * myScaleObj);
		int DeleteMultipleScales (PrjDefMemManage * pProjet);
		
		// database rank operations
		bool SetRank (ListGenReport * list, int icol, 
					  const wxString & stable,
					  const wxString & sfield,
					  bool bStringType = FALSE,
					  const wxString & rankfield = _T("RANK"));
		bool SetScaleRank (ScaleList * list, int icol, 
						   const wxString & stable,
						   const wxString & sfield,
						   const wxString & rankfield = _T("RANK"));
		
		// projects operations
		PrjDefMemManage * GetProjectDataFromDB ();
		bool UpdateDataBaseProject (PrjDefMemManage * pProjDef);
		
	};




#endif
