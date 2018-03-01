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
#include "../core/projectdefmemory.h"        // for transfering data directely
#include "../core/prjdefmemmanage.h"        // for transfering data directely
#include "../core/wxdirpickerctrlbest.h"    // for PATH_ERROR definition;
#include "../gui/projectproperties_dlg.h"    // for scale RANK function
#include "../gis/tmlayermanager.h"            // GIS definition
#include "../gui/shortcut_defs.h"            // for key definitions (F1,...)
#include "../core/tmsnappingmemory.h"        // for snapping defintions.

class BezierSettingsData;


// TABLES NAMES
const wxString TABLE_NAME_LAYERS = _T("thematic_layers");
const wxString TABLE_NAME_OBJECTS = _T("dmn_layer_object");
const wxString TABLE_NAME_LAYER_AT = _T("layer_at");
const wxString TABLE_NAME_PRJ_SETTINGS = _T("prj_settings");
const wxString TABLE_NAME_LAYER_TYPE = _T("dmn_layer_type");
const wxString TABLE_NAME_SCALE = _T("zoom_level");
const wxString TABLE_NAME_TOC = _T("prj_toc");
const wxString TABLE_NAME_QUERIES = _T("prj_queries");
const wxString TABLE_NAME_SHORTCUT_DMN = _T("dmn_shortcut_key");
const wxString TABLE_NAME_SHORTCUT_LIST = _T("shortcut_list");
const wxString TABLE_NAME_SNAPPING = _T("prj_snapping");
const wxString TABLE_NAME_STAT = _T("prj_stats");
const wxString TABLE_NAME_LANG_DEF = _T("lang_def");
const wxString TABLE_NAME_AT_LIST = _T("dmn_layer_attribut");
const wxString TABLE_NAME_AT_MIX = _T("dmn_attribut_value");
const wxString TABLE_NAME_AT_CATALOG = _T("dmn_catalog");
const wxString TABLE_NAME_EXPORT_POLY = _T("export_poly");

const wxString TABLE_NAME_GIS_GENERIC[] =
        {
                _T("generic_lines"),
                _T("generic_points"),
                _T("generic_labels"),
                _T("generic_notes"),
                _T("generic_frame")
        };

static const int TABLE_NAME_GIS_GENERIC_NUMBER = 5;

const wxString TABLE_NAME_GIS_ATTRIBUTION[] =
        {
                _T("generic_aat"),
                _T("generic_pat"),
                _T("generic_lat"),
                _T("generic_notes")
        };


// DATABASE VERSION IS
const int TM_DATABASE_VERSION = 230;


enum tmDB_OPEN_STATUS
{
    tmDB_OPEN_FAILED = 0,
    tmDB_OPEN_OK = 1,
    tmDB_OPEN_FAILED_NOT_TM_DB = 2,
    tmDB_OPEN_ERR_NOT_FOUND = 3,
    tmDB_OPEN_FAILED_WRONG_VERSION = 100
};


class DataBaseTM : public DataBase
{
private:
    int m_iDBLayerIndex;

    // field operations private (creation)
    bool AddTableIfNotExist(const wxString &TableName);

    bool DeleteTableIfExist(const wxString &TableName);

    // general database operations
    bool FillLayerTableTypeData();

    bool FillDefaultScaleData();

    bool FillShortCutTable();

    //bool TableEmpty (const wxString & tablename);
    bool CreateEmptyTMDatabase();
    //bool _AddEnumField(ProjectDefMemoryFields * field, int layerindex);

public:
    DataBaseTM();

    ~DataBaseTM();

    // utility function
    bool CreateLangDefData();

    // database operations
    bool TableExist(const wxString &tablename);                                        //[TESTED]
    int OpenTMDatabase(const wxString &pathname);                                        //[TESTED]
    bool CreateTMDatabase(PrjDefMemManage *pPrjDefinition);                            //[TESTED]

    // Database project operations
    bool IsProjectDataDefined();

    bool SetProjectData(PrjDefMemManage *pPrjDefinition);

    bool GetProjectData(PrjDefMemManage *pPrjDefinition);

    bool InitProjectWithStartingWizard(PrjDefMemManage *pPrjDefinition);

    int GetDatabaseToolMapVersion();                                                    //[TESTED]
    bool SetProjectExportData(int iExportType, const wxString &spath);

    bool SetProjectBackupPath(const wxString &spath);

    int GetProjectExportData(int &iExportType, wxString &spath);

    int GetProjectBackupPath(wxString &spath);


    // Database layer operations
    bool AddLayer(ProjectDefMemoryLayers *myLayer);

    bool AddLayerPolygonDefaultBorder(ProjectDefMemoryLayers *myLayer);

    void SetActiveLayerId(ProjectDefMemoryLayers *myLayer);

    int GetActiveLayerId()
    { return m_iDBLayerIndex; }

    int GetNextLayer(ProjectDefMemoryLayers *myLayer);

    bool UpdateLayer(ProjectDefMemoryLayers *myLayer, wxString &sSqlSentence);

    bool DeleteLayer(const wxArrayLong &deletelist, PrjDefMemManage *projectdef, wxString &sSqlSentence);

    int GetLayerID(TOC_GENERIC_NAME layertype);

    // Database object operations
    bool AddObject(ProjectDefMemoryObjects *myObject, int DBlayerIndex = -1);

    bool EditObject(ProjectDefMemoryObjects *myObject);

    bool DataBaseGetNextResultAsObject(ProjectDefMemoryObjects *object, int ilayertype);

    int DeleteMultipleObjects(PrjDefMemManage *pProjet);

    void DeleteLayersObjects(int iLayer, wxString &sSqlSentence);

    bool LoadLayerObjects(ProjectDefMemoryLayers *layer);

    // field operations
    bool AddField(ProjectDefMemoryFields *myField);

    bool GetFields(PrjMemFieldArray &fieldarray, ProjectDefMemoryLayers *actuallayer);

    int GetFieldsFromDB(PrjDefMemManage *myPrj);

    bool UpdateField(ProjectDefMemoryFields *myField, int iLayer, wxString &sSqlSentence);

    bool DeleteField(wxArrayString &myFields, int iLayer, wxString &sSqlSentence);

    bool DeleteFields(int iLayer);

    // database query
    bool GetObjectListByLayerType(int ilayertype, bool bOrder = FALSE);

    wxArrayString GetLayerNameByType(int ilayertype);

    // database Scale operations
    long GetNextScaleValue(long &DBindex, bool bFirst);

    bool EditScale(ProjectDefMemoryScale *myScaleObj);

    int DeleteMultipleScales(PrjDefMemManage *pProjet);

    // database rank operations
    bool SetRank(ListGenReport *list, int icol,
                 const wxString &stable,
                 const wxString &sfield,
                 bool bStringType = FALSE,
                 const wxString &rankfield = _T("RANK"));

    bool SetScaleRank(ScaleList *list, int icol,
                      const wxString &stable,
                      const wxString &sfield,
                      const wxString &rankfield = _T("RANK"));

    // projects operations
    PrjDefMemManage *GetProjectDataFromDB();

    bool UpdateDataBaseProject(PrjDefMemManage *pProjDef);

    // database TOC operations
    bool InitTOCGenericLayers();

    tmLayerProperties *GetNextTOCEntry(bool userelativepath);

    long AddTOCLayer(tmLayerProperties *item);

    bool RemoveTOCLayer(const long &itemid);

    void PrepareTOCStatusUpdate(wxString &sentence,
                                tmLayerProperties *item, int itemRank = 0,
                                const wxString &symbology = wxEmptyString);

    // database query operations
    bool GetNextQueries(long &qid, wxString &name, wxString &description, bool bfirst);

    bool GetQueriesById(const long &qid, int &target,
                        wxString &name, wxString &description);

    bool EditQueries(int target, const wxString &name,
                     const wxString &description, long qid = -1);

    bool DeleteQuery(long qid);

    // database shortcut operations
    bool GetNextShortcutByLayerType(int layer_type, wxString &key,
                                    wxString &description, bool bFirstLoop);

    bool GetAllUnusedShortcuts(wxArrayString &keylist);

    bool GetNextShortCutObject(long &shortcutid, const int &key, bool bFirstLoop);

    bool EditShortcut(int shortcutkey, const wxString &description,
                      const wxArrayLong &types);

    bool DeleteShortcut(int shortcutkey);

    bool GetNextShortcutFull(bool bFirstLoop, int &layertype,
                             int &key, wxString &description,
                             long &shortcutvalue);

    // database snapping operations
    bool GetNextSnapping(long &lid, wxString &layername,
                         int &snapstatus, bool bfirstloop);

    bool GetValidLayersForSnapping(wxArrayLong &lids, wxArrayString &lnames);

    bool AddLayersSnapping(const wxArrayLong &lids, int snappingstatus);

    bool DeleteLayerSnapping(int layersid);

    bool SaveSnappingAllStatus(tmSnappingMemory *snapmemory);

    // database snapping tolerence operations
    bool SetSnappingTolerence(int iTolerence);

    int GetSnappingTolerence();

    // database geometry operations
    bool GeometryDelete(wxArrayLong *selected, int layertype);

    long GeometrySave(OGRGeometry *geometry, int layertype);

    bool GeometryUpdate(OGRGeometry *geometry, long oid, int layertype);

    OGRGeometry *GeometryLoad(long oid, int layertype);

    // attribution operations
    bool DeleteAttribution(wxArrayLong *selected, int layertype);

    wxArrayLong GetObjectsFromFeature(long selectedfeature, int layertype);

    long GetSelectedLayerId(long ojectid);

    bool DeleteAdvancedAttribution(long selectedobject, long selectedlayerid);

    bool SaveBezierSettings(BezierSettingsData *data);

    bool LoadBezierSettings(BezierSettingsData *data);

    // static path convertion
    static bool ConvertPath(wxString &path);
};


#endif
