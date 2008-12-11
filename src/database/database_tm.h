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
#include "../gis/tmlayermanager.h"			// GIS definition 
#include "../gui/shortcut_defs.h"			// for key definitions (F1,...)


// TABLES NAMES
const wxString TABLE_NAME_LAYERS = _T("thematic_layers");
const wxString TABLE_NAME_OBJECTS = _T("dmn_layer_object");
const wxString TABLE_NAME_LAYER_AT = _T("layer_at");
const wxString TABLE_NAME_PRJ_SETTINGS = _T("prj_settings");
const wxString TABLE_NAME_LAYER_TYPE = _T("dmn_layer_type");
const wxString TABLE_NAME_SCALE			= _T("zoom_level");
const wxString TABLE_NAME_TOC		= _T("prj_toc");
const wxString TABLE_NAME_QUERIES	= _T("prj_queries");
const wxString TABLE_NAME_SHORTCUT_DMN = _T("dmn_shortcut_key");
const wxString TABLE_NAME_SHORTCUT_LIST = _T("shortcut_list");

const wxString TABLE_NAME_GIS_GENERIC[] = 
{
	_T("generic_lines"),
	_T("generic_points"),
	_T("generic_labels"),
	_T("generic_notes"),
	_T("generic_frame")
};

const wxString TABLE_NAME_GIS_ATTRIBUTION[] = 
{
_T("generic_aat"),
_T("generic_pat"),
_T("generic_lat"),
_T("generic_notes")
};

// DATABASE VERSION IS
const int TM_DATABASE_VERSION = 217;






class DataBaseTM : public DataBase
	{
	private:
		int m_iDBLayerIndex;
		
		// field operations private (creation)
		bool AddTableIfNotExist (const wxString & TableName);
		bool DeleteTableIfExist (const wxString & TableName);
		
	public:
		DataBaseTM();
		~DataBaseTM();
		
		
		// general database operations
		bool FillLayerTableTypeData ();
		bool FillDefaultScaleData ();
		bool FillShortCutTable ();
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
		bool AddLayerPolygonDefaultBorder (ProjectDefMemoryLayers * myLayer);
		void SetActiveLayerId (ProjectDefMemoryLayers * myLayer);
		int GetActiveLayerId () {return m_iDBLayerIndex;}
		int GetNextLayer (ProjectDefMemoryLayers * myLayer);
		bool UpdateLayer (ProjectDefMemoryLayers * myLayer, wxString & sSqlSentence);
		bool DeleteLayer (const wxArrayLong & deletelist, wxString & sSqlSentence);
		int GetLayerID (TOC_GENERIC_NAME layertype);
		
		// Database object operations
		bool AddObject (ProjectDefMemoryObjects * myObject, int DBlayerIndex=-1);
		bool EditObject (ProjectDefMemoryObjects * myObject );
		bool DataBaseGetNextResultAsObject(ProjectDefMemoryObjects * object, int ilayertype);
		int  DeleteMultipleObjects (PrjDefMemManage * pProjet);
		void DeleteLayersObjects (int iLayer, wxString & sSqlSentence);
		
		// field operations
		bool AddField (ProjectDefMemoryFields * myField, int DBlayerIndex = -1);
		bool GetFields (PrjMemFieldArray & fieldarray, ProjectDefMemoryLayers * actuallayer);
		int GetFieldsFromDB (PrjDefMemManage * myPrj);
		bool UpdateField(ProjectDefMemoryFields * myField, int iLayer, wxString & sSqlSentence);
		bool DeleteField (wxArrayString & myFields, int iLayer, wxString & sSqlSentence);
		
		// database query
		bool GetObjectListByLayerType(int ilayertype, bool bOrder = FALSE);
		wxArrayString GetLayerNameByType (int ilayertype);
		
		// database Scale operations
		long GetNextScaleValue (long & DBindex, bool bFirst);
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
		
		// database TOC operations
		bool InitTOCGenericLayers();
		tmLayerProperties * GetNextTOCEntry();
		long AddTOCLayer (tmLayerProperties * item);
		bool RemoveTOCLayer (const long & itemid);
		void PrepareTOCStatusUpdate(wxString & sentence, 
									tmLayerProperties * item, int itemRank=0, 
									const wxString & symbology = wxEmptyString);
		
		// database query operations
		bool GetNextQueries (long & qid, wxString & name, wxString & description, bool bfirst); 
		bool GetQueriesById (const long & qid, int & target,
							 wxString & name, wxString & description);
		bool EditQueries (int target, const wxString & name, 
						  const wxString & description, long qid = -1);
		bool DeleteQuery (long qid);
		
		// database shortcut operations
		bool GetNextShortcutByLayerType (int layer_type, wxString & key, 
										wxString & description, bool bFirstLoop);
	};




#endif
