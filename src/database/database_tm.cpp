/***************************************************************************
 database_tm.cpp
 extension of the database class for ToolMap
 -------------------
 copyright : (C) 2007 CREALP Lucien Schreiber
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "database_tm.h"

#include "../gui/beziersettings_dlg.h"
#include "databaseresult.h"

DataBaseTM::DataBaseTM() {
    m_iDBLayerIndex = 0;
}

DataBaseTM::~DataBaseTM() {}

int DataBaseTM::OpenTMDatabase(const wxString& pathname) {
    wxFileName myPath(pathname, _T(""));
    if (!myPath.IsOk()) return tmDB_OPEN_FAILED;

    wxArrayString myDirs;
    myDirs = myPath.GetDirs();
    wxString myName = myDirs.Last();
    myPath.RemoveLastDir();

    if (!DataBaseOpen(myPath.GetFullPath(), myName)) return tmDB_OPEN_FAILED;

    if (!TableExist(TABLE_NAME_PRJ_SETTINGS)) return tmDB_OPEN_FAILED_NOT_TM_DB;

    int iToolMapVersion = GetDatabaseToolMapVersion();
    if (iToolMapVersion != TM_DATABASE_VERSION) {
        return iToolMapVersion;
    }

    wxLogDebug(_T("%s is a ToolMap database version : %d"), DataBaseGetName().c_str(), TM_DATABASE_VERSION);
    return tmDB_OPEN_OK;
}

bool DataBaseTM::CreateTMDatabase(PrjDefMemManage* pPrjDefinition) {
    if (!DataBaseCreateNew(pPrjDefinition->m_PrjPath, pPrjDefinition->m_PrjName)) {
        return false;
    }

    if (!CreateEmptyTMDatabase()) {
        return false;
    }

    if (!CreateLangDefData()) {
        return false;
    }

    if (!SetProjectData(pPrjDefinition)) {
        return false;
    }

    if (!InitTOCGenericLayers()) {
        return false;
    }

    return true;
}

/*************************** GENERAL DATABASE FUNCTION ****************************/
bool DataBaseTM::CreateEmptyTMDatabase() {
    wxString myNewPrjSentence1 =
        _T("CREATE  TABLE `dmn_layer_type` (")
        _T("  `TYPE_CD` INT UNSIGNED NOT NULL COMMENT 'Layer type code' ,")
        _T("  `TYPE_DESCRIPTION` VARCHAR(20) NOT NULL COMMENT 'Layer type description' ,")
        _T("  PRIMARY KEY (`TYPE_CD`) );")

        _T("CREATE  TABLE `thematic_layers` (")
        _T("  `LAYER_INDEX` INT UNSIGNED NOT NULL AUTO_INCREMENT COMMENT 'Theme index' ,")
        _T("  `TYPE_CD` INT UNSIGNED NOT NULL ,")
        _T("  `LAYER_NAME` VARCHAR(255) NOT NULL COMMENT 'Theme name' ,")
        _T("  `LAYER_DESC` VARCHAR(255) NULL COMMENT 'Theme description' ,")
        _T("  `REMARK` VARCHAR(255) NULL ,")
        _T("  PRIMARY KEY (`LAYER_INDEX`) ,")
        _T("  INDEX THEMATIC_LAYERS_FKIndex1 (`TYPE_CD` ASC) ,")
        _T("  CONSTRAINT `Rel_11`")
        _T("    FOREIGN KEY (`TYPE_CD` )")
        _T("    REFERENCES `dmn_layer_type` (`TYPE_CD` )")
        _T("    ON DELETE NO ACTION")
        _T("    ON UPDATE NO ACTION);")

        _T("CREATE  TABLE `dmn_layer_object` (")
        _T("  `OBJECT_ID` BIGINT NOT NULL AUTO_INCREMENT ,")
        _T("  `OBJECT_CD` VARCHAR(50) NULL COMMENT 'Feature code' ,")
        _T("  `OBJECT_TYPE_CD` INT UNSIGNED NOT NULL COMMENT 'Spatial object code' ,")
        _T("  `THEMATIC_LAYERS_LAYER_INDEX` INT UNSIGNED NOT NULL COMMENT 'Associated thematic layer' ,")
        _T("`OBJECT_DESC_0` VARCHAR(255) NOT NULL COMMENT 'Feature description' ,")
        _T("`OBJECT_DESC_1` VARCHAR(255) NULL ,")
        _T("`OBJECT_DESC_2` VARCHAR(255) NULL ,")
        _T("`OBJECT_DESC_3` VARCHAR(255) NULL ,")
        _T("`OBJECT_DESC_4` VARCHAR(255) NULL ,")

        _T("  `OBJECT_ISFREQ` BOOLEAN NOT NULL DEFAULT FALSE ,")
        _T("  `SYMBOL_CD` VARCHAR(20) NULL COMMENT 'Feature symbology' ,")
        _T("  `RANK` INT NULL ,")
        _T("  `REMARK` VARCHAR(100) NULL ,")
        _T("  PRIMARY KEY (`OBJECT_ID`) ,")
        _T("  INDEX DMN_LAYER_OBJECT_FKIndex2 (`THEMATIC_LAYERS_LAYER_INDEX` ASC, `OBJECT_ID` ASC) ,")
        _T("  CONSTRAINT `Contain`")
        _T("    FOREIGN KEY (`THEMATIC_LAYERS_LAYER_INDEX` )")
        _T("    REFERENCES `thematic_layers` (`LAYER_INDEX` )")
        _T("    ON DELETE CASCADE")
        _T("    ON UPDATE RESTRICT);")

        _T("CREATE  TABLE `generic_points` (")
        _T("  `OBJECT_ID` INT UNSIGNED NOT NULL AUTO_INCREMENT COMMENT 'Feature identifier' ,")
        _T("  `OBJECT_GEOMETRY` POINT NOT NULL COMMENT 'Feature geometry' ,")
        _T("  PRIMARY KEY (`OBJECT_ID`) );")

        _T("CREATE  TABLE `generic_labels` (")
        _T("  `OBJECT_ID` INT UNSIGNED NOT NULL AUTO_INCREMENT ,")
        _T("  `OBJECT_GEOMETRY` POINT NOT NULL ,")
        _T("  PRIMARY KEY (`OBJECT_ID`) );")

        _T("CREATE  TABLE `generic_lines` (")
        _T("  `OBJECT_ID` INT UNSIGNED NOT NULL AUTO_INCREMENT ,")
        _T("  `OBJECT_GEOMETRY` LINESTRING NOT NULL ,")
        _T("  PRIMARY KEY (`OBJECT_ID`) );")

        _T("CREATE  TABLE `generic_pat` (")
        _T("  `OBJECT_VAL_ID` INT UNSIGNED NOT NULL ,")
        _T("  `OBJECT_GEOM_ID` INT UNSIGNED NOT NULL ,")
        _T("  PRIMARY KEY (`OBJECT_VAL_ID`, `OBJECT_GEOM_ID`) ,")
        _T("  INDEX GENERIC_POINTS_has_DMN_LAYER_OBJECT_FKIndex1 (`OBJECT_VAL_ID` ASC) ,")
        _T("  INDEX GENERIC_POINTS_has_DMN_LAYER_OBJECT_FKIndex2 (`OBJECT_GEOM_ID` ASC) ,")
        _T("  CONSTRAINT `Rel_19`")
        _T("    FOREIGN KEY (`OBJECT_VAL_ID` )")
        _T("    REFERENCES `generic_points` (`OBJECT_ID` ),")
        _T("  CONSTRAINT `Rel_20`")
        _T("    FOREIGN KEY (`OBJECT_GEOM_ID`)")
        _T("    REFERENCES `dmn_layer_object` (`OBJECT_ID`));")

        _T("CREATE  TABLE `generic_lat` (")
        _T("  `OBJECT_VAL_ID` INT UNSIGNED NOT NULL ,")
        _T("  `OBJECT_GEOM_ID` INT UNSIGNED NOT NULL ,")
        _T("  PRIMARY KEY (`OBJECT_VAL_ID`, `OBJECT_GEOM_ID`) ,")
        _T("  INDEX GENERIC_LABELS_has_DMN_LAYER_OBJECT_FKIndex1 (`OBJECT_VAL_ID` ASC) ,")
        _T("  INDEX GENERIC_LABELS_has_DMN_LAYER_OBJECT_FKIndex2 (`OBJECT_GEOM_ID` ASC) ,")
        _T("  CONSTRAINT `Rel_21`")
        _T("    FOREIGN KEY (`OBJECT_VAL_ID` )")
        _T("    REFERENCES `generic_labels` (`OBJECT_ID` ),")
        _T("  CONSTRAINT `Rel_22`")
        _T("    FOREIGN KEY (`OBJECT_GEOM_ID`)")
        _T("    REFERENCES `dmn_layer_object` (`OBJECT_ID`));")

        _T("CREATE  TABLE `generic_aat` (")
        _T("  `OBJECT_VAL_ID` INT UNSIGNED NOT NULL ,")
        _T("  `OBJECT_GEOM_ID` INT UNSIGNED NOT NULL ,")
        _T("  PRIMARY KEY (`OBJECT_VAL_ID`, `OBJECT_GEOM_ID`) ,")
        _T("  INDEX GENERIC_LINES_has_DMN_LAYER_OBJECT_FKIndex1 (`OBJECT_VAL_ID` ASC) ,")
        _T("  INDEX GENERIC_LINES_has_DMN_LAYER_OBJECT_FKIndex2 (`OBJECT_GEOM_ID` ASC) ,")
        _T("  CONSTRAINT `Rel_25`")
        _T("    FOREIGN KEY (`OBJECT_VAL_ID` )")
        _T("    REFERENCES `generic_lines` (`OBJECT_ID` ),")
        _T("  CONSTRAINT `Rel_26`")
        _T("    FOREIGN KEY (`OBJECT_GEOM_ID`)")
        _T("    REFERENCES `dmn_layer_object` (`OBJECT_ID`));")

        _T("CREATE  TABLE `generic_dmn` (")
        _T("  `CODE` INT UNSIGNED NOT NULL AUTO_INCREMENT COMMENT 'Code value' ,")
        _T("  `DESCRIPTION` VARCHAR(255) NULL COMMENT 'Code description' ,")
        _T("  PRIMARY KEY (`CODE`) );");

    wxString myNewPrjSentence2 =

        _T("CREATE  TABLE `prj_toc` (")
        _T("  `CONTENT_ID` INT UNSIGNED NOT NULL AUTO_INCREMENT ,")
        _T("  `PARENT_ID` INT UNSIGNED DEFAULT 0,")
        _T("  `TYPE_CD` INT UNSIGNED NOT NULL ,")
        _T("  `CONTENT_PATH` VARCHAR(255) NULL ,")
        _T("  `CONTENT_NAME` VARCHAR(255) NULL ,")
        _T("  `CONTENT_STATUS` BOOLEAN NULL DEFAULT 1 ,")
        _T("  `GENERIC_LAYERS` TINYINT NULL DEFAULT 100 ,")
        _T("  `RANK` INT NULL ,")
        _T("  `SYMBOLOGY` TEXT(65535) NULL ,")
        _T("  `VERTEX_FLAGS` TINYINT NULL  ,")
        _T("  `LABEL_VISIBLE` tinyint(1) DEFAULT '0',")
        _T("  `LABEL_DEF` mediumtext,")
        _T("  PRIMARY KEY (`CONTENT_ID`) ,")
        _T("  INDEX PRJ_TOC_FKIndex1 (`TYPE_CD` ASC) ,")
        _T("  CONSTRAINT `Rel_10`")
        _T("    FOREIGN KEY (`TYPE_CD` )")
        _T("    REFERENCES `dmn_layer_type` (`TYPE_CD` ));")

        _T("CREATE  TABLE `dmn_shortcut_key` (")
        _T("  `SHORTCUT_CD` INT UNSIGNED NOT NULL AUTO_INCREMENT ,")
        _T("  `SHORTCUT_KEY` VARCHAR(20) NOT NULL ,")
        _T("  `SHORTCUT_DESC` VARCHAR(255) NULL ,")
        _T("  PRIMARY KEY (`SHORTCUT_CD`) );")

        _T("CREATE  TABLE `shortcut_list` (")
        _T("  `OBJECT_ID` INT UNSIGNED NOT NULL ,")
        _T("  `SHORTCUT_CD` INT UNSIGNED NOT NULL ,")
        _T("  PRIMARY KEY (`OBJECT_ID`, `SHORTCUT_CD`) ,")
        _T("  INDEX SHORTCUT_KEYS_has_DMN_LAYER_OBJECT_FKIndex2 (`OBJECT_ID` ASC) ,")
        _T("  INDEX SHORTCUT_LIST_FKIndex2 (`SHORTCUT_CD` ASC) ,")
        _T("  CONSTRAINT `Rel_14`")
        _T("    FOREIGN KEY (`SHORTCUT_CD` )")
        _T("    REFERENCES `dmn_shortcut_key` (`SHORTCUT_CD` ),")
        _T("  CONSTRAINT `Rel_15`")
        _T("    FOREIGN KEY (`OBJECT_ID`)")
        _T("    REFERENCES `dmn_layer_object` (`OBJECT_ID`));")

        _T("CREATE  TABLE `generic_frame` (")
        _T("  `OBJECT_ID` INT UNSIGNED NOT NULL AUTO_INCREMENT COMMENT 'Feature identifier' ,")
        _T("  `OBJECT_GEOMETRY`  LINESTRING NOT NULL COMMENT 'Feature geometry' ,")
        _T("  PRIMARY KEY (`OBJECT_ID`) );")

        _T("CREATE  TABLE `zoom_level` (")
        _T("  `ZOOM_ID` INT UNSIGNED NOT NULL AUTO_INCREMENT ,")
        _T("  `SCALE_VALUE` INT UNSIGNED NOT NULL ,")
        _T("  `RANK` INT NULL ,")
        _T("  PRIMARY KEY (`ZOOM_ID`) );")

        _T("CREATE  TABLE `prj_settings` (")
        _T("  `SETTING_DBK` INT NOT NULL AUTO_INCREMENT ,")
        _T("  `PRJ_UNIT` VARCHAR(45) NOT NULL ,")
        _T("  `PRJ_PROJECTION` VARCHAR(45) NOT NULL ,")
        _T("  `PRJ_NAME` VARCHAR(45) NOT NULL ,")
        _T("  `PRJ_VERSION` INT NOT NULL ,")
        _T("  `PRJ_EXPORT_PATH` VARCHAR(255) NULL ,")
        _T("  `PRJ_EXPORT_TYPE` INT NULL DEFAULT 0 ,")
        _T("  `PRJ_LAST_EXPORTED` TEXT NULL ,")
        _T("  `PRJ_BACKUP_PATH` VARCHAR(255) NULL ,")
        _T("  `PRJ_AUTHORS` VARCHAR(255) NULL ,")
        _T("  `PRJ_SUMMARY` TEXT NULL ,")
        _T("  `PRJ_SNAP_TOLERENCE` INT NOT NULL DEFAULT 10 ,")
        _T("  `PRJ_LANG_ACTIVE` INT NOT NULL DEFAULT 0 ,")
        _T("  `PRJ_BEZIER_APPROX` FLOAT NOT NULL DEFAULT 0.5, ")
        _T("  `PRJ_BEZIER_WIDTH` FLOAT NOT NULL DEFAULT 1, ")
        _T("  `PRJ_BEZIER_NB_VERTEX` INT NOT NULL DEFAULT 10, ")
        _T("  `PRJ_BEZIER_METHOD` INT NOT NULL DEFAULT 0, ")  // method 0 = AGG
        _T("  PRIMARY KEY (`SETTING_DBK`) );")

        _T("CREATE  TABLE `generic_notes` (")
        _T("  `OBJECT_ID` INT UNSIGNED NOT NULL AUTO_INCREMENT ,")
        _T("  `OBJECT_GEOMETRY` POINT NOT NULL ,")
        _T("  `PRIORITY_CD` TINYINT NULL DEFAULT 0 ,")
        _T("  `CODE_CD` TINYINT NULL DEFAULT 0 ,")
        _T("  `AUTHOR` VARCHAR(255) NULL ,")
        _T("  `NOTE_DATE` TIMESTAMP NULL DEFAULT CURRENT_TIMESTAMP ,")
        _T("  `DESCRIPTION` TEXT NULL ,")
        _T("  `ISFIXED` BOOLEAN NULL DEFAULT 0 ,")
        _T("  PRIMARY KEY (`OBJECT_ID`) ); ")

        _T("CREATE TABLE `prj_queries` (")
        _T(" `QUERIES_ID` INTEGER NOT NULL AUTO_INCREMENT ,")
        _T(" `QUERIES_TARGET` INT NOT NULL , ")
        _T(" `QUERIES_NAME` VARCHAR(255) NOT NULL,")
        _T(" `QUERIES_CODE` VARCHAR(1000),")
        _T(" PRIMARY KEY (`QUERIES_ID`) );")

        _T("CREATE TABLE `prj_snapping`(")
        _T(" `TOC_ID` INTEGER NOT NULL,")
        _T(" `SNAPPING_TYPE` INTEGER NOT NULL,")
        _T("  PRIMARY KEY (`TOC_ID`),")
        _T("CONSTRAINT `Rel_30`")
        _T("  FOREIGN KEY (`TOC_ID`) REFERENCES `prj_toc` (`CONTENT_ID`));")

        _T("CREATE  TABLE `prj_stats` (")
        _T("`STAT_ID` INT NOT NULL AUTO_INCREMENT ,")
        _T("`DATE_START` DATETIME NOT NULL ,`CLICK` BIGINT UNSIGNED NULL DEFAULT 0 ,")
        _T("`ATTRIBUTION` BIGINT UNSIGNED NULL DEFAULT 0 , `INTERSECTION` BIGINT UNSIGNED NULL DEFAULT 0 ,")
        _T(" `DATE_END` TIMESTAMP NOT NULL default current_timestamp on update current_timestamp , PRIMARY KEY ")
        _T("(`STAT_ID`) );")

        _T("CREATE  TABLE IF NOT EXISTS `lang_def` (")
        _T("`LANG_ID` INT NOT NULL,")
        _T("`LANG_NAME` VARCHAR(50) NULL ,")
        _T("PRIMARY KEY (`LANG_ID`) );")

        _T("CREATE  TABLE IF NOT EXISTS `dmn_layer_attribut` (")
        _T("`ATTRIBUT_ID` INT NOT NULL AUTO_INCREMENT ,")
        _T("`LAYER_INDEX` INT NOT NULL ,")
        _T("`ATTRIBUT_NAME` VARCHAR(100) NULL ,")
        _T("PRIMARY KEY (`ATTRIBUT_ID`, `LAYER_INDEX`) );")

        _T("CREATE  TABLE IF NOT EXISTS `dmn_catalog` (")
        _T("`CATALOG_ID` INT NOT NULL AUTO_INCREMENT ,")
        _T("`CODE` VARCHAR(50) NULL ,")
        _T("`DESCRIPTION_0` VARCHAR(255) NULL ,")
        _T("`DESCRIPTION_1` VARCHAR(255) NULL ,")
        _T("`DESCRIPTION_2` VARCHAR(255) NULL ,")
        _T("`DESCRIPTION_3` VARCHAR(255) NULL ,")
        _T("`DESCRIPTION_4` VARCHAR(255) NULL ,")
        _T("PRIMARY KEY (`CATALOG_ID`) );")

        _T("CREATE  TABLE IF NOT EXISTS `dmn_attribut_value` (")
        _T("`ATTRIBUT_ID` INT NOT NULL ,")
        _T("`CATALOG_ID` INT NOT NULL ,")
        _T("PRIMARY KEY (`ATTRIBUT_ID`, `CATALOG_ID`) );")

        _T("CREATE  TABLE IF NOT EXISTS `export_poly` (")
        _T("   `LAYER_INDEX` INT NOT NULL ,")
        _T("   `RASTER_FACTOR` DOUBLE NULL ,")
        _T("   `NB_EXPORT` INT NULL ,")
        _T("   `PERCENT_SKIPPED` DOUBLE NULL ,")
        _T("   PRIMARY KEY (`LAYER_INDEX`));");

    // wxArrayString myArray = DataBaseCutRequest(myNewPrjSentence);
    if (!DataBaseQueryNoResults(myNewPrjSentence1)) {
        return false;
    }

    if (!DataBaseQueryNoResults(myNewPrjSentence2)) {
        return false;
    }

    // pass field data to the database
    if (!FillLayerTableTypeData()) {
        return false;
    }

    // pass scale data into the database
    if (!FillDefaultScaleData()) {
        return false;
    }

    if (!FillShortCutTable()) {
        return false;
    }

    return true;
}

bool DataBaseTM::TableExist(const wxString& tablename) {
    wxString mySentence = _T("SHOW TABLES LIKE \"") + tablename + _T("\"");
    if (!DataBaseQuery(mySentence)) return false;

    if (!DataBaseHasResults()) return false;

    DataBaseClearResults();
    return true;
}

/*************************** PROJECT DATABASE FUNCTION ****************************/
bool DataBaseTM::FillLayerTableTypeData() {
    wxString sSentence = _T("");
    bool bReturnValue = false;

    wxASSERT(TableExist(TABLE_NAME_LAYER_TYPE));

    // fill the field from the layer type table
    // only if the table is empty...
    for (int i = 0; i < PRJDEF_LAYERS_TYPE_NUMBER; i++) {
        sSentence.Printf(_T("INSERT INTO %s VALUES (%d,\"%s\")"), TABLE_NAME_LAYER_TYPE.c_str(), i,
                         PRJDEF_LAYERS_TYPE_STRING[i].c_str());

        // in case of error send debug message
        if (!DataBaseQueryNoResults(sSentence)) {
            wxLogError(_("Error filling data into the [%s] tables"), TABLE_NAME_LAYER_TYPE.c_str());
        } else
            bReturnValue = true;
    }

    return bReturnValue;
}

/***************************************************************************/ /**
  @brief Fill default data into scale table
  @details This function fills default scales into the scale table (ZOOM_LEVEL) if
  the table is empty. Otherwise FALSE is returned. Default scales are :
  - 1:5000
  - 1:10000
  - 1:25000
  - 1:50000
  @return  FALSE if the table isn't empty
  @author Lucien Schreiber (c) CREALP 2007
  @date 27 March 2008
  *******************************************************************************/
bool DataBaseTM::FillDefaultScaleData() {
    bool bReturnValue = FALSE;
    wxString sValue = _T("INSERT INTO ") + TABLE_NAME_SCALE + _T("(SCALE_VALUE) VALUES ");
    wxString sSentence = _T("");
    int iScale[] = {5000, 10000, 25000, 50000};
    int iScaleNum = (sizeof(iScale) / sizeof(int));

    // prepare statement
    for (int i = 0; i < iScaleNum; i++) {
        sSentence.Append(sValue);
        sSentence.Append(wxString::Format(_T("(%d); "), iScale[i]));
    }

    // execute statement
    if (DataBaseQuery(sSentence)) {
        bReturnValue = TRUE;
    }

    if (!bReturnValue) {
        wxLogError(_T("Error filling scale table. Already filled of request error ?"));
        wxLogError(DataBaseGetLastError());
    }

    return bReturnValue;
}

/***************************************************************************/ /**
  @brief Fill database with shortcut values
  @details Default values are passed into table when a new project is created.
  Values are :
  - F1
  - F2
  - ...
  - F12
  @author Lucien Schreiber (c) CREALP 2008
  @date 10 December 2008
  *******************************************************************************/
bool DataBaseTM::FillShortCutTable() {
    wxString sValue = _T("INSERT INTO ") + TABLE_NAME_SHORTCUT_DMN + _T(" (SHORTCUT_CD, SHORTCUT_KEY) VALUES ");
    wxString sSentence = _T("");

    int iNbKey = sizeof(tmShortCutKeys) / sizeof(wxString);
    for (int i = 0; i < iNbKey; i++) {
        sSentence.Append(sValue);
        sSentence.Append(wxString::Format(_T("(%d, \"%s\"); "), i + 1, tmShortCutKeys[i].c_str()));
    }

    if (!DataBaseQuery(sSentence)) {
        wxLogError(_("Error filling shortcut table"));
        return false;
    }

    return true;
}

bool DataBaseTM::CreateLangDefData() {
    wxString myTxtTemplate = _T("REPLACE INTO %s VALUES (%d, \"%s\"); ");
    wxString myLangues[] = {wxT("Undefined"), wxT("Undefined"), wxT("Undefined"), wxT("Undefined"), wxT("Undefined")};
    wxString myQuery = wxEmptyString;
    for (int i = 0; i < (sizeof(myLangues) / sizeof(wxString)); i++) {
        myQuery.append(wxString::Format(myTxtTemplate, TABLE_NAME_LANG_DEF, i, myLangues[i]));
    }
    return DataBaseQueryNoResults(myQuery);
}

/***************************************************************************/ /**
  @brief Check if project data are defined
  @details This function check the existance of a line into the PRJ_SETTINGS
  table.
  @return  TRUE if a line exist containing project data (in the PRJ_SETTINGS
  table)
  @author Lucien Schreiber (c) CREALP 2007
  @date 25 March 2008
  *******************************************************************************/
bool DataBaseTM::IsProjectDataDefined() {
    // check if the project line exist
    wxString sSentence = _T("SELECT * FROM ") + TABLE_NAME_PRJ_SETTINGS + _T(" WHERE SETTING_DBK =1");
    if (DataBaseQuery(sSentence)) {
        // if ok the project settings exist
        if (DataBaseHasResults()) {
            DataBaseClearResults();
            return true;
        }
    }
    return false;
}

bool DataBaseTM::SetProjectData(PrjDefMemManage* pPrjDefinition) {
    // prepare data
    wxString sUnit = PRJDEF_UNIT_TYPE_STRING[pPrjDefinition->m_PrjUnitType];
    wxString sProj = PRJDEF_PROJ_TYPE_STRING[pPrjDefinition->m_PrjProjType];
    wxString sSentence = _T("");

    if (!IsProjectDataDefined()) {
        sSentence = wxString::Format(
            _T("INSERT INTO %s (PRJ_UNIT, PRJ_PROJECTION,")
            _T("PRJ_NAME, PRJ_VERSION, PRJ_AUTHORS, PRJ_SUMMARY) ")
            _T(" VALUES (\"%s\",\"%s\",\"%s\",%d,\"%s\",\"%s\")"),
            TABLE_NAME_PRJ_SETTINGS.c_str(), sUnit.c_str(), sProj.c_str(), pPrjDefinition->m_PrjName.c_str(),
            TM_DATABASE_VERSION, pPrjDefinition->m_PrjAuthors.c_str(), pPrjDefinition->m_PrjSummary.c_str());

    } else
        sSentence = wxString::Format(
            _T("UPDATE %s SET ")
            _T("PRJ_UNIT = \"%s\", PRJ_PROJECTION = \"%s\", ")
            _T("PRJ_NAME = \"%s\", PRJ_VERSION = %d, ")
            _T("PRJ_AUTHORS = \"%s\", PRJ_SUMMARY = \"%s\""),
            TABLE_NAME_PRJ_SETTINGS.c_str(), sUnit.c_str(), sProj.c_str(), pPrjDefinition->m_PrjName.c_str(),
            TM_DATABASE_VERSION, pPrjDefinition->m_PrjAuthors.c_str(), pPrjDefinition->m_PrjSummary.c_str());

    // processing request
    if (DataBaseQueryNoResults(sSentence)) {
        return true;
    }

    wxLogError(_T("Editing project settings failed!"));
    return false;
}

/***************************************************************************/ /**
  @brief Retrieve basic project data
  @details This may be used for filling a #PrjDefMemManage object with basic data
  from project such as Project name, path, units,... In facts all stuff from the
  Project Definition dialog (except the spatial model)
  @param pPrjDefinition a pointer to an existing #PrjDefMemManage object, will be
  used for storing project data (path, name,...)
  @return bool return TRUE if data was found from the Database
  @author Lucien Schreiber (c) CREALP 2007
  @date 14 April 2008
  *******************************************************************************/
bool DataBaseTM::GetProjectData(PrjDefMemManage* pPrjDefinition) {
    int i = 0;
    wxString sSentence = wxString::Format(
        _T("SELECT PRJ_UNIT, PRJ_PROJECTION,")
        _T("PRJ_AUTHORS, PRJ_SUMMARY FROM %s"),
        TABLE_NAME_PRJ_SETTINGS.c_str());

    wxString myError = _T("Error getting project data from the Database");
    if (!DataBaseQuery(sSentence)) {
        wxLogError(myError);
        return false;
    }

    wxArrayString myResults;
    if (!DataBaseGetNextResult(myResults)) {
        wxLogError(myError);
        return false;
    }
    DataBaseClearResults();
    wxASSERT(myResults.GetCount() == 4);

    // UNITS
    bool bFoundUnit = false;
    for (i = 0; i < PRJDEF_UNIT_TYPE_NUMBER; i++) {
        if (myResults.Item(0) == PRJDEF_UNIT_TYPE_STRING[i]) {
            pPrjDefinition->m_PrjUnitType = (PRJDEF_UNIT_TYPE)i;
            bFoundUnit = true;
            break;
        }
    }
    if (!bFoundUnit) {
        wxLogWarning(_("Units unknown (%s)\ndefaulting to Meters!"), myResults.Item(0));
    }

    // PROJECTION
    bool bFoundProjection = false;
    for (i = 0; i < PRJDEF_PROJ_TYPE_NUMBER; i++) {
        if (myResults.Item(1) == PRJDEF_PROJ_TYPE_STRING[i]) {
            pPrjDefinition->m_PrjProjType = (PRJDEF_PROJ_TYPE)i;
            bFoundProjection = true;
            break;
        }
    }
    if (!bFoundProjection) {
        wxLogWarning(_("Projection unknown (%s)\ndefaulting to Swiss projectiong (CH1903)!"), myResults.Item(1));
    }

    pPrjDefinition->m_PrjName = DataBaseGetName();
    pPrjDefinition->m_PrjPath = DataBaseGetPath();
    pPrjDefinition->m_PrjAuthors = myResults.Item(2);
    pPrjDefinition->m_PrjSummary = myResults.Item(3);
    return true;
}

bool DataBaseTM::InitProjectWithStartingWizard(PrjDefMemManage* pPrjDefinition) {
    unsigned int indexLayer = 0;
    unsigned int indexObject = 0;
    unsigned int indexField = 0;
    bool bReturnValue = TRUE;

    /// adding layers, and for each layer add object and fields related to this layer
    for (indexLayer = 0; indexLayer < pPrjDefinition->m_PrjLayerArray.GetCount(); indexLayer++) {
        ProjectDefMemoryLayers* myMemLayers = pPrjDefinition->FindLayer(indexLayer);
        if (myMemLayers != nullptr) {
            if (AddLayer(myMemLayers)) {
                // if layer is polygon, add a default line for border
                // checks are done internally
                AddLayerPolygonDefaultBorder(myMemLayers);

                // Adding Objects
                for (indexObject = 0; indexObject < myMemLayers->m_pLayerObjectArray.GetCount(); indexObject++) {
                    ProjectDefMemoryObjects* myMemObj = pPrjDefinition->FindObject(indexObject);
                    if (myMemObj != nullptr) {
                        bReturnValue = bReturnValue && AddObject(myMemObj);
                        wxASSERT(bReturnValue);
                    }
                }

                // adding fields
                for (indexField = 0; indexField < myMemLayers->m_pLayerFieldArray.GetCount(); indexField++) {
                    ProjectDefMemoryFields* myMemField = pPrjDefinition->FindField(indexField);
                    if (myMemField != nullptr) {
                        bReturnValue = bReturnValue && AddField(myMemField);
                        wxASSERT(bReturnValue);
                    }
                }
            }
        }
    }

    if (!bReturnValue)
        wxLogDebug(_T("Error : adding project object, fields, or setting project data into the database"));
    else
        wxLogDebug(_T("Project data successfully copied into the database"));
    return bReturnValue;
}

int DataBaseTM::GetDatabaseToolMapVersion() {
    wxString sSentence = _T("SELECT (PRJ_VERSION) FROM " + TABLE_NAME_PRJ_SETTINGS);
    long myVersion = 0;
    int iReturnedVersion = wxNOT_FOUND;
    if (DataBaseQuery(sSentence)) {
        if (DataBaseGetNextResult(myVersion)) iReturnedVersion = (int)myVersion;

        DataBaseClearResults();
    }
    return iReturnedVersion;
}

bool DataBaseTM::SetProjectExportData(int iExportType, const wxString& spath) {
    wxString sSentence = wxString::Format(
        _T("UPDATE %s SET PRJ_EXPORT_PATH = \"%s\", ")
        _T("PRJ_EXPORT_TYPE = %d; "),
        TABLE_NAME_PRJ_SETTINGS.c_str(), spath.c_str(), iExportType);
    if (DataBaseQuery(sSentence)) return TRUE;

    wxLogError(_T("Not able to update project export path : %s"), sSentence.c_str());
    return FALSE;
}

bool DataBaseTM::SetProjectLastExported(const wxArrayString& layers) {
    wxString layersList;
    for (const auto& layer : layers) {
        if (!layersList.IsEmpty()) {
            layersList.Append(";");
        }
        layersList.Append(layer);
    }

    wxString sSentence = wxString::Format(_T("UPDATE %s SET PRJ_LAST_EXPORTED = \"%s\"; "),
                                          TABLE_NAME_PRJ_SETTINGS.c_str(), layersList.c_str());
    if (DataBaseQuery(sSentence)) return TRUE;

    wxLogError(_T("Not able to update last exported layers : %s"), sSentence.c_str());
    return FALSE;
}

bool DataBaseTM::SetProjectBackupPath(const wxString& spath) {
    wxString sSentence = wxString::Format(_T("UPDATE %s SET PRJ_BACKUP_PATH = \"%s\"; "),
                                          TABLE_NAME_PRJ_SETTINGS.c_str(), spath.c_str());

    if (DataBaseQuery(sSentence)) return TRUE;

    wxLogError(_T("Not able to update project backup path : %s"), sSentence.c_str());
    return FALSE;
}

/***************************************************************************/ /**
  @brief Get the export data from the database
  @details Retrive the Export path and the export type from the database
  @param iExportType This value will be modified with one of the
  #PRJDEF_EXPORT_TYPE.
  @param spath This variable will be filled with the spath if return value is
  different of PATH_EMPTY
  @return  the returned values is a flag of type #PATH_ERROR
  @author Lucien Schreiber (c) CREALP 2007
  @date 28 March 2008
  *******************************************************************************/
int DataBaseTM::GetProjectExportData(int& iExportType, wxString& spath) {
    int iflagreturn = PATH_OK;
    wxArrayString myResults;

    wxString sSentence = _T("SELECT PRJ_EXPORT_TYPE, PRJ_EXPORT_PATH FROM ") + TABLE_NAME_PRJ_SETTINGS;
    if (DataBaseQuery(sSentence) && DataBaseHasResults()) {
        // get the first result as int
        bool bValue = DataBaseGetNextResult(myResults);
        wxASSERT(bValue);
        wxASSERT(myResults.GetCount() == 2);

        iExportType = wxAtoi(myResults.Item(0));
        spath = myResults.Item(1);

        if (spath.IsEmpty())
            iflagreturn = PATH_EMPTY;
        else {
            // check for path validity
            if (!wxFileName::DirExists(spath)) iflagreturn = PATH_INVALID;
        }

        DataBaseClearResults();

    } else  // error querying the database
    {
        wxLogDebug(
            _T("Unable to query the database for project ")
            _T("export data. or no results returned : %s"),
            sSentence.c_str());
        iflagreturn = PATH_DATABASE_ERROR;
    }
    return iflagreturn;
}

wxArrayString DataBaseTM::GetProjectLastExported() {
    wxArrayString layers;

    wxString sSentence = _T("SELECT PRJ_LAST_EXPORTED FROM ") + TABLE_NAME_PRJ_SETTINGS;
    if (DataBaseQuery(sSentence) && DataBaseHasResults()) {
        wxString sLayers;
        bool bValue = DataBaseGetNextResult(sLayers);
        wxASSERT(bValue);

        if (!sLayers.IsEmpty()) {
            wxStringTokenizer tokenizer(sLayers, ";");
            while (tokenizer.HasMoreTokens()) {
                layers.Add(tokenizer.GetNextToken());
            }
        }

        DataBaseClearResults();
    }

    return layers;
}

/***************************************************************************/ /**
  @brief Get the backup path from the database
  @details Retrive the Backup path type from the database
  @param spath This variable will be filled with the backup path from the
  database  if return value is different of PATH_EMPTY
  @return  the returned values is a flag of type #PATH_ERROR
  @author Lucien Schreiber (c) CREALP 2007
  @date 28 March 2008
  *******************************************************************************/
int DataBaseTM::GetProjectBackupPath(wxString& spath) {
    int iflagreturn = PATH_OK;

    wxString sSentence = _T("SELECT PRJ_BACKUP_PATH FROM ") + TABLE_NAME_PRJ_SETTINGS;
    if (DataBaseQuery(sSentence) && DataBaseHasResults()) {
        DataBaseGetNextResult(spath);
        DataBaseClearResults();

        if (spath.IsEmpty())
            iflagreturn = PATH_EMPTY;
        else {
            // check for path validity
            if (!wxFileName::DirExists(spath)) iflagreturn = PATH_INVALID;
        }

    } else  // error querying the database
    {
        wxLogDebug(
            _T("Unable to query the database for project ")
            _T("backup path. or no results returned : %s"),
            sSentence.c_str());
        iflagreturn = PATH_DATABASE_ERROR;
    }
    return iflagreturn;
}

/*************************** LAYER DATABASE FUNCTION ****************************/
bool DataBaseTM::AddLayer(ProjectDefMemoryLayers* myLayer) {
    wxString sSentence = _T("INSERT INTO ") + TABLE_NAME_LAYERS + _T(" (TYPE_CD, LAYER_NAME) VALUES ");
    wxString sValues = wxString::Format(_T("(%d,'%s')"), myLayer->m_LayerType, myLayer->m_LayerName.c_str());
    sSentence.Append(sValues);

    if (DataBaseQueryNoResults(sSentence)) {
        SetActiveLayerId(myLayer);
        return true;
    }
    return false;
}

/***************************************************************************/ /**
  @brief Add border default value for polygons layer
  @param myLayer pointer to a valid #ProjectDefMemoryLayers object
  @return  true if adding succesfull
  @author Lucien Schreiber (c) CREALP 2008
  @date 03 November 2008
  *******************************************************************************/
bool DataBaseTM::AddLayerPolygonDefaultBorder(ProjectDefMemoryLayers* myLayer) {
    if (myLayer->m_LayerType != LAYER_POLYGON || myLayer->m_LayerPolygonDefaultValue.IsEmpty()) return false;

    wxString sSentence = wxString::Format(
        _T("INSERT INTO ") + TABLE_NAME_OBJECTS +
            _T(" (OBJECT_CD, OBJECT_TYPE_CD, THEMATIC_LAYERS_LAYER_INDEX, OBJECT_DESC_0) VALUES ")
            _T("(%d, %d, %d, \"%s\")"),
        GetActiveLayerId() * 1000, LAYER_LINE, GetActiveLayerId(), myLayer->m_LayerPolygonDefaultValue.c_str());
    if (DataBaseQueryNoResults(sSentence))
        return true;
    else
        wxLogError(_("Error setting default border values"));
    return false;
}

void DataBaseTM::SetActiveLayerId(ProjectDefMemoryLayers* myLayer) {
    wxString sSentence = _T("SELECT LAYER_INDEX FROM ") + TABLE_NAME_LAYERS + _(" WHERE LAYER_NAME = ");
    wxString sValues = wxString::Format(_T("'%s'"), myLayer->m_LayerName.c_str());
    sSentence.Append(sValues);

    long myIndex = wxNOT_FOUND;
    if (DataBaseQuery(sSentence))  // query OK
    {
        if (DataBaseGetNextResult(myIndex)) m_iDBLayerIndex = myIndex;

        DataBaseClearResults();
    } else
        wxLogDebug(_T("Unable to find the layer [%s]"), myLayer->m_LayerName.c_str());
}

int DataBaseTM::GetNextLayer(ProjectDefMemoryLayers* myLayer) {
    wxArrayString myResults;
    wxString sSentence = wxString::Format(_T("SELECT LAYER_INDEX, TYPE_CD, LAYER_NAME FROM %s"),
                                          TABLE_NAME_LAYERS.c_str());

    // check if we have some results
    if (DataBaseHasResults()) {
        if (DataBaseGetNextResult(myResults)) {
            wxASSERT(myResults.Count() == 3);

            // LAYER ID
            myLayer->m_LayerID = wxAtoi(myResults.Item(0));

            // LAYER TYPE
            myLayer->m_LayerType = (PRJDEF_LAYERS_TYPE)wxAtoi(myResults.Item(1));

            // LAYER NAME
            myLayer->m_LayerName = myResults.Item(2);
            return 1;
        }
        DataBaseClearResults();
        return -1;

    } else {
        bool bQuery = DataBaseQuery(sSentence);
        wxASSERT(bQuery);

        // check that result != 0
        if (!DataBaseHasResults()) return -1;
    }
    return 0;
}

/***************************************************************************/ /**
  @brief Prepare statement for updating layer
  @details This function return the prepared SQL statement for updating or
  inserting data into the #TABLE_NAME_LAYERS.
  @note the most important thing is
  to be sure that myLayer->m_LayerID is correctly set for function's good
  behaviour.
  @param myLayer pointer to a #ProjectDefMemoryLayers object with new data. In
  case of modification the m_LayerID member must be set correctly
  @param sSqlSentence reference to a wxString in which we are going to put the
  issued sentence
  @return TRUE if a modification statement was issued or FALSE if we
  were preparing an INSERT statement
  @author Lucien Schreiber (c) CREALP 2007
  @date 15 April 2008
  *******************************************************************************/
bool DataBaseTM::UpdateLayer(ProjectDefMemoryLayers* myLayer, wxString& sSqlSentence) {
    wxString sInsertSentence = _T("");
    long myLastInsertedId = 0;

    // update or insert statement based on the m_layerID
    // greater than 1 for update and 0 if insert is required
    if (myLayer->m_LayerID > 0) {
        sSqlSentence.Append(wxString::Format(
            _T(" UPDATE %s SET TYPE_CD = %d, LAYER_NAME = \"%s\"")
            _T(" WHERE LAYER_INDEX = %d; "),
            TABLE_NAME_LAYERS.c_str(), myLayer->m_LayerType, myLayer->m_LayerName.c_str(), myLayer->m_LayerID));
        return true;
    } else {
        // we need to insert layer immediately to get the real layer_ID.
        // used later during the attribute table creation process.
        sInsertSentence = wxString::Format(
            _T(" INSERT INTO %s (TYPE_CD, LAYER_NAME) ")
            _T("VALUES (%d,\"%s\"); "),
            TABLE_NAME_LAYERS.c_str(), myLayer->m_LayerType, myLayer->m_LayerName.c_str());
        if (DataBaseQueryNoResults(sInsertSentence)) {
            // get the last inserted id
            myLastInsertedId = DataBaseGetLastInsertedID();
            if (myLastInsertedId != wxNOT_FOUND) {
                myLayer->m_LayerID = myLastInsertedId;
            } else
                wxLogError(_T("Error, last inserted ID not found : %ld"), myLastInsertedId);
        } else {
            wxLogError(_T("Problem inserting layer : %s"), sInsertSentence.c_str());
        }
        return false;
    }
    return false;
}

/***************************************************************************/ /**
  @brief Prepare statement for deleting layer(s)
  @details This append to the sentence the SQL code for deleting all layers which
  have their IDs stored into the deletelist array. This function operate on the
  #TABLE_NAME_LAYERS table.
  @param deletelist A wxArrayLong containing all ID we want to delete
  @param sSqlSentence SQL sentence which will be modified with the new delete
  statement
  @return  allways true for the moment
  @author Lucien Schreiber (c) CREALP 2007
  @date 16 April 2008
  *******************************************************************************/
bool DataBaseTM::DeleteLayer(const wxArrayLong& deletelist, PrjDefMemManage* projectdef, wxString& sSqlSentence) {
    for (unsigned int i = 0; i < deletelist.GetCount(); i++) {
        sSqlSentence.Append(wxString::Format(_T(" DELETE FROM %s WHERE LAYER_INDEX = %ld; "), TABLE_NAME_LAYERS.c_str(),
                                             deletelist.Item(i)));

        // delete associated field table (if existing)
        if (!DeleteTableIfExist(TABLE_NAME_LAYER_AT + wxString::Format(_T("%ld"), deletelist.Item(i)))) {
            wxLogError(_T("Error deleting table : %s%ld"), TABLE_NAME_LAYER_AT.c_str(), deletelist.Item(i));
        }

        // prepare delete attribution objects from the object table
        wxString sDeleteSentence = _T("");
        DeleteLayersObjects(deletelist.Item(i), sDeleteSentence);
        if (!DataBaseQueryNoResults(sDeleteSentence)) {
            wxLogError(_T("Error deleting objects (GIS or attribution) : %s"), sDeleteSentence.c_str());
        }

        // correctly delete enumeration fields
        DeleteFields(deletelist.Item(i));
    }
    return TRUE;
}

/***************************************************************************/ /**
  @brief Return the layer ID based on layer type
  @details This function works only for following "unique" layers :
  - TOC_NAME_LINES
  - TOC_NAME_POINTS,
  - TOC_NAME_LABELS,
  - TOC_NAME_ANNOTATIONS,
  - TOC_NAME_FRAME,
  for others type, wxNOT_FOUND is returned
  @param layertype One of the #TOC_GENERIC_NAME values, see above
  @return the layer ID or wxNOT_FOUND if an error occur or the layer dosen't
  exist
  @author Lucien Schreiber (c) CREALP 2008
  @date 11 November 2008
  *******************************************************************************/
int DataBaseTM::GetLayerID(TOC_GENERIC_NAME layertype) {
    wxString sStatement = wxString::Format(
        _T("SELECT CONTENT_ID FROM ") + TABLE_NAME_TOC + _T(" WHERE GENERIC_LAYERS=%d"), layertype);

    if (!DataBaseQuery(sStatement)) {
        wxASSERT_MSG(0, _T("Error during query"));
        return wxNOT_FOUND;
    }

    long mylID = wxNOT_FOUND;
    DataBaseGetNextResult(mylID);
    DataBaseClearResults();

    return (int)mylID;
}

/*************************** OBJECT DATABASE FUNCTION ****************************/
bool DataBaseTM::AddObject(ProjectDefMemoryObjects* myObject, int DBlayerIndex) {
    // get the selected layer of take the actual one
    if (DBlayerIndex == -1) {
        DBlayerIndex = GetActiveLayerId();
    }

    wxString sSentence = _T("INSERT INTO ") + TABLE_NAME_OBJECTS +
                         _T(" (OBJECT_CD, OBJECT_TYPE_CD, THEMATIC_LAYERS_LAYER_INDEX, OBJECT_DESC_0) VALUES ");
    wxString sValues = wxString::Format(_T("(\"%s\", %d, %d,\"%s\")"), myObject->m_ObjectCode, myObject->m_ObjectType,
                                        DBlayerIndex, myObject->m_ObjectName.c_str());
    sSentence.Append(sValues);
    if (DataBaseQueryNoResults(sSentence)) {
        return true;
    }

    wxLogDebug(_T("Unable to AddObject to the database"));
    return false;
}

/***************************************************************************/ /**
  @brief Insert or update an object
  @details This function insert or update an object in the database depending of
  the myObject->m_Object_ID value : bigger or egal to 0 means to update and
  smaller than 0 means to insert
  @param myObject parameter of the object to modify into the DB
  @return  TRUE if all went OK or FALSE if the query or something else dosen't
  work as espected
  @author Lucien Schreiber (c) CREALP 2007
  @date 18 March 2008
  *******************************************************************************/
bool DataBaseTM::EditObject(ProjectDefMemoryObjects* myObject) {
    // get layer id
    ProjectDefMemoryLayers myLayer;
    myLayer.m_LayerName = myObject->m_ParentLayerName;

    // if layer name isn't empty
    if (!myLayer.m_LayerName.IsEmpty()) {
        SetActiveLayerId(&myLayer);

        // prepare the sentence for insert or update
        // sentence for insert
        wxString sInsert = wxString::Format(
            _T("INSERT INTO %s ")
            _T("(OBJECT_CD, OBJECT_TYPE_CD, THEMATIC_LAYERS_LAYER_INDEX, OBJECT_DESC_0, OBJECT_ISFREQ) ")
            _T("VALUES (\"%s\", %d, %d, \"%s\", %d)"),
            TABLE_NAME_OBJECTS.c_str(), myObject->m_ObjectCode, myObject->m_ObjectType, GetActiveLayerId(),
            myObject->m_ObjectName.c_str(), (int)myObject->m_ObjectFreq);
        // sentence for update
        wxString sUpdate = wxString::Format(
            _T("UPDATE %s ")
            _T("SET OBJECT_CD = \"%s\", OBJECT_TYPE_CD=%d, THEMATIC_LAYERS_LAYER_INDEX = %d,")
            _T("OBJECT_DESC_0 = \"%s\", OBJECT_ISFREQ = %d ")
            _T("WHERE OBJECT_ID = %ld"),
            TABLE_NAME_OBJECTS.c_str(), myObject->m_ObjectCode, myObject->m_ObjectType, GetActiveLayerId(),
            myObject->m_ObjectName.c_str(), (int)myObject->m_ObjectFreq, myObject->m_ObjectID);

        // if id >= 0 we update (item exist)
        // otherwise we insert (item dosen't exist)
        if (myObject->m_ObjectID >= 0) {
            wxLogDebug(sUpdate);
            if (DataBaseQueryNoResults(sUpdate)) return true;
        } else {
            if (DataBaseQueryNoResults(sInsert)) return true;
        }
    }

    wxLogDebug(_T("Error editing object into the database"));
    return FALSE;
}

bool DataBaseTM::DataBaseGetNextResultAsObject(ProjectDefMemoryObjects* object, int ilayertype) {
    wxASSERT(object);

    wxArrayString myRowResults;
    if (!DataBaseHasResults()) {
        return false;
    }

    if (!DataBaseGetNextResult(myRowResults)) {
        DataBaseClearResults();
        return false;
    }

    wxASSERT(myRowResults.GetCount() == 6);
    object->m_ObjectCode = myRowResults.Item(0);
    object->m_ObjectName = myRowResults.Item(1);
    long myType = 0;
    myRowResults.Item(2).ToLong(&myType);
    object->m_ObjectType = (short)myType;
    object->m_ParentLayerName = myRowResults.Item(3);

    long lFreq = 0;
    myRowResults.Item(4).ToLong(&lFreq);
    object->m_ObjectFreq = (PRJDEF_OBJECTS_FREQ)lFreq;
    myRowResults.Item(5).ToLong(&(object->m_ObjectID));

    return true;
}

int DataBaseTM::DeleteMultipleObjects(PrjDefMemManage* pProjet) {
    wxString sSentence = _T("");
    wxString sPattern = _T("");

    // prepare statement for multiple delete
    for (unsigned int i = 0; i < pProjet->m_StoreDeleteIDObj.GetCount(); i++) {
        sSentence.Append(wxString::Format(_T("DELETE FROM %s WHERE OBJECT_ID = %ld; "), TABLE_NAME_OBJECTS.c_str(),
                                          pProjet->m_StoreDeleteIDObj.Item(i)));
    }

    // execute statement
    if (!sSentence.IsEmpty() && DataBaseQueryNoResults(sSentence)) return pProjet->m_StoreDeleteIDObj.GetCount();

    return -1;
}

/***************************************************************************/ /**
  @brief Delete all objects from a layer
  @details Delete all objects belonging to a layers. May be used when one try to
  delete a layer.
  This function dosen't delete the table but issued a sentence
  for this purpose.
  @param iLayer Index of the layer (Starts at 1 in MySQL)
  @param sSqlSentence The sentence for storing the SQL sentence issued in the
  function
  @author Lucien Schreiber (c) CREALP 2007
  @date 28 April 2008
  *******************************************************************************/
void DataBaseTM::DeleteLayersObjects(int iLayer, wxString& sSqlSentence) {
    sSqlSentence.Append(wxString::Format(_T(" DELETE FROM %s WHERE THEMATIC_LAYERS_LAYER_INDEX = %d; "),
                                         TABLE_NAME_OBJECTS.c_str(), iLayer));
}

bool DataBaseTM::LoadLayerObjects(ProjectDefMemoryLayers* layer) {
    wxASSERT(layer->m_LayerID != wxNOT_FOUND);
    wxString myQuery = wxString::Format(
        "SELECT o.OBJECT_CD, o.OBJECT_DESC_0, OBJECT_TYPE_CD, l.LAYER_NAME, o.OBJECT_ISFREQ, o.OBJECT_ID "
        "FROM %s AS o LEFT JOIN %s as l ON "
        "(l.LAYER_INDEX=o.THEMATIC_LAYERS_LAYER_INDEX) "
        "WHERE o.THEMATIC_LAYERS_LAYER_INDEX = %d ORDER BY o.OBJECT_CD",
        TABLE_NAME_OBJECTS, TABLE_NAME_LAYERS, layer->m_LayerID);
    if (!DataBaseQuery(myQuery)) {
        wxLogDebug(myQuery);
        return false;
    }

    // clear existing objects
    unsigned int oCount = layer->m_pLayerObjectArray.GetCount();
    for (unsigned int i = 0; i < oCount; i++) {
        ProjectDefMemoryObjects* myObj = layer->m_pLayerObjectArray.Item(0);
        wxDELETE(myObj);
        layer->m_pLayerObjectArray.RemoveAt(0);
    }

    while (1) {
        ProjectDefMemoryObjects* myObj = new ProjectDefMemoryObjects();
        if (!DataBaseGetNextResultAsObject(myObj, layer->m_LayerType)) {
            wxDELETE(myObj);
            break;
        }
        wxASSERT(myObj);
        layer->m_pLayerObjectArray.Add(myObj);
    }
    return true;
}

/***************************************************************************/ /**
  @brief Delete a Table
  @details Delete a table if this table exists
  @param TableName Name of the table to delete
  @return  Return True if the table was deleted
  @author Lucien Schreiber (c) CREALP 2007
  @date 28 April 2008
  *******************************************************************************/
bool DataBaseTM::DeleteTableIfExist(const wxString& TableName) {
    wxString sSentence = wxString::Format(_T("DROP TABLE IF EXISTS %s; "), TableName.c_str());
    if (DataBaseQueryNoResults(sSentence)) return true;
    return false;
}

/*************************** FIELD DATABASE FUNCTION [ PRIVATE ] *******************/
bool DataBaseTM::AddTableIfNotExist(const wxString& TableName) {
    wxString sCreateTable1 = _T("CREATE  TABLE IF NOT EXISTS `");
    wxString sValues = wxString::Format(_T("%s` ("), TableName.c_str());
    wxString sCreateTable2 =
        _T("  `OBJECT_ID` INT UNSIGNED NOT NULL     ,")
        _T("  PRIMARY KEY (`OBJECT_ID`) ,")
        _T("  INDEX LAYER_ATX_FKIndex1 (`OBJECT_ID` ASC) ,")
        _T("  CONSTRAINT `Rel_09`")
        _T("    FOREIGN KEY (`OBJECT_ID` )")
        _T("    REFERENCES  `generic_points` (`OBJECT_ID` ));");
    sCreateTable1.Append(sValues + sCreateTable2);

    if (DataBaseQueryNoResults(sCreateTable1)) {
        return true;
    }
    wxLogDebug(_T("Table Creation [%s]... FAILED"), TableName.c_str());
    return false;
}

/*************************** FIELD DATABASE FUNCTION ****************************/
bool DataBaseTM::AddField(ProjectDefMemoryFields* myField) {
    int DBlayerIndex = GetActiveLayerId();
    wxString sTableName = wxString::Format(TABLE_NAME_LAYER_AT + _("%d"), DBlayerIndex);

    // first we must create the table if not exist
    bool bAdded = AddTableIfNotExist(sTableName);
    wxASSERT(bAdded);

    // then create the fields based upon the fields type
    // use the same function for updating
    wxString sSentence = _T("");
    UpdateField(myField, DBlayerIndex, sSentence);
    if (!DataBaseQueryNoResults(sSentence)) {
        wxLogError(_T("Unable to add field to the database : %s"), sSentence.c_str());
        return false;
    }
    return true;
}

/***************************************************************************/ /**
  @brief Fill an existing project with fields
  @details The myPrj parameter must point to a existing project in memory.
  containing all layers. This function will add all fields into this project
  @param myPrj Pointer to an existing #PrjDefMemManage containing all layers
  @return  return the number of fields added, or -1 if something goes wrong
  @author Lucien Schreiber (c) CREALP 2008
  @date 19 June 2008
  *******************************************************************************/
int DataBaseTM::GetFieldsFromDB(PrjDefMemManage* myPrj) {
    // some checks
    if (!myPrj) {
        wxLogDebug(_T("Pointer to project in memory is empty"));
        return -1;
    }

    // splitting field request (bug 473)
    wxString sql1 = _T("SELECT TABLE_NAME FROM INFORMATION_SCHEMA.TABLES WHERE TABLE_NAME LIKE \"" +
                       TABLE_NAME_LAYER_AT + "%\" AND TABLE_SCHEMA=\"" + DataBaseGetName() + "\"");
    if (!DataBaseQuery(sql1)) {
        wxLogError("Unable to get fields!: '%s'", sql1);
        return -1;
    }
    wxArrayString mylayers_table;
    if (!DataBaseGetResults(mylayers_table)) {
        wxLogDebug("No layers!!");
        return -1;
    }
    wxString mylayers_table_string;
    for (int i = 0; i < mylayers_table.GetCount(); i++) {
        mylayers_table_string += "\"";
        mylayers_table_string += mylayers_table.Item(i);
        mylayers_table_string += "\",";
    }
    mylayers_table_string.RemoveLast();

    wxString sql2 = _T(
        "SELECT"
        " CAST(SUBSTR(TABLE_NAME FROM 9) AS UNSIGNED) LAYER_INDEX,COLUMN_NAME, COLUMN_TYPE, COLUMN_COMMENT"
        " FROM INFORMATION_SCHEMA.COLUMNS"
        " WHERE TABLE_NAME IN (" +
        mylayers_table_string +
        ")"
        " AND TABLE_SCHEMA = \"" +
        DataBaseGetName() + "\" AND COLUMN_NAME NOT IN ('OBJECT_ID', 'LAYER_AT_ID') ORDER BY LAYER_INDEX;");

    if (!DataBaseQuery(sql2)) {
        wxLogDebug(_T("Error gettings fields from database : %s"), sql2.c_str());
        return -1;
    }

    int iNbFields = 0;
    long iRealLayerID = 0;
    wxArrayString myResults;
    ProjectDefMemoryLayers* myLayer = nullptr;
    ProjectDefMemoryFields* myField = nullptr;
    while (DataBaseGetNextResult(myResults))  // loop until no more results
    {
        // find layer by real id
        myResults.Item(0).ToLong(&iRealLayerID);
        myLayer = myPrj->FindLayerByRealID(iRealLayerID);
        if (!myLayer) {
            wxLogDebug(_T("Layer error, layer %ld not found"), iRealLayerID);
            break;
        }

        // add field and set values to this field
        iNbFields++;
        myField = myPrj->AddField();
        myField->SetValues(myResults.Item(1), myResults.Item(2), myResults.Item(3));
    }
    DataBaseClearResults();

    // get enumeration for all fields list (enumeration)
    wxString myQ = wxString::Format(
        _T("select l.ATTRIBUT_ID, l.LAYER_INDEX, l.ATTRIBUT_NAME, c.CATALOG_ID, c.CODE, c.DESCRIPTION_0  FROM %s l ")
        _T("LEFT ")
        _T("JOIN  (%s c, %s m) ON (l.ATTRIBUT_ID = m.ATTRIBUT_ID AND m.CATALOG_ID = c.CATALOG_ID) ORDER BY ")
        _T("l.LAYER_INDEX"),
        TABLE_NAME_AT_LIST, TABLE_NAME_AT_CATALOG, TABLE_NAME_AT_MIX);
    if (!DataBaseQuery(myQ)) {
        return iNbFields;
    }

    DataBaseResult myResult;
    DataBaseGetResults(&myResult);

    while (myResult.NextRow()) {
        long myFieldID = wxNOT_FOUND;
        long myLayerIndex = wxNOT_FOUND;
        wxString myFieldName;
        long myEnumID = wxNOT_FOUND;
        wxString myEnumCode;
        wxString myEnumName;

        wxASSERT(myResult.GetColCount() == 6);
        myResult.GetValue(0, myFieldID);
        myResult.GetValue(1, myLayerIndex);
        myResult.GetValue(2, myFieldName);
        myResult.GetValue(3, myEnumID);
        myResult.GetValue(4, myEnumCode);
        myResult.GetValue(5, myEnumName);

        ProjectDefMemoryLayers* myLayer = myPrj->FindLayerByRealID(myLayerIndex);
        wxASSERT(myLayer);
        if (myLayer == nullptr) {
            wxLogError(_T("Layer %ld not found!"), myLayerIndex);
            continue;
        }
        ProjectDefMemoryFields* myField = myPrj->FindField(myFieldName);
        wxASSERT(myField);
        if (myField == nullptr) {
            wxLogError(_T("Field %s not found!"), myFieldName);
            continue;
        }
        // change field from default integer to enum
        myField->m_FieldID = myFieldID;
        myField->m_FieldType = TM_FIELD_ENUMERATION;

        ProjectDefMemoryFieldsCodedVal* myCVal = new ProjectDefMemoryFieldsCodedVal();
        myCVal->m_ValueID = myEnumID;
        myCVal->m_ValueCode = myEnumCode;
        myCVal->m_ValueName = myEnumName;
        myField->m_pCodedValueArray.Add(myCVal);
    }
    return iNbFields;
}

/***************************************************************************/ /**
  @brief Get all fields for specified layer
  @param fieldarray This array contain all fields for specified layer
  @param actuallayer the layer for wich to retreive fields
  @return  true if fields where  returned, false otherwise
  @author Lucien Schreiber (c) CREALP 2008
  @date 13 November 2008
  *******************************************************************************/
bool DataBaseTM::GetFields(PrjMemFieldArray& fieldarray, ProjectDefMemoryLayers* actuallayer) {
    wxASSERT(actuallayer);

    wxString sStatmt =
        _T("SELECT CAST(SUBSTR(TABLE_NAME FROM 9) AS UNSIGNED) LAYER_INDEX,")
        _T(" COLUMN_NAME, COLUMN_TYPE, COLUMN_COMMENT FROM")
        _T(" INFORMATION_SCHEMA.COLUMNS Where table_schema = \"%s\" and")
        _T(" table_name = \"%s%d\" AND COLUMN_NAME NOT IN ('OBJECT_ID', 'LAYER_AT_ID')");

    wxString sSentence = wxString::Format(sStatmt, DataBaseGetName().c_str(), TABLE_NAME_LAYER_AT.c_str(),
                                          actuallayer->m_LayerID);
    if (!DataBaseQuery(sSentence)) {
        return false;
    }

    ProjectDefMemoryFields* myField = nullptr;
    wxArrayString myResults;
    while (DataBaseGetNextResult(myResults)) {
        myField = new ProjectDefMemoryFields();
        wxString myFieldName = myResults.Item(1);
        wxString myFieldDesc = myResults.Item(2);
        wxString myFieldComment = myResults.Item(3);

        myField->SetValues(myFieldName, myFieldDesc, myFieldComment);
        fieldarray.Add(myField);
    }
    DataBaseClearResults();

    // get enumeration for all fields list (enumeration)
    wxASSERT(actuallayer);
    wxString myQ = wxString::Format(
        _T("select l.ATTRIBUT_ID, l.LAYER_INDEX, l.ATTRIBUT_NAME, c.CATALOG_ID, c.CODE, c.DESCRIPTION_0  FROM %s l ")
        _T("LEFT ")
        _T("JOIN  (%s c, %s m) ON (l.ATTRIBUT_ID = m.ATTRIBUT_ID AND m.CATALOG_ID = c.CATALOG_ID) WHERE ")
        _T("l.LAYER_INDEX=%d ORDER BY l.ATTRIBUT_ID"),
        TABLE_NAME_AT_LIST, TABLE_NAME_AT_CATALOG, TABLE_NAME_AT_MIX, actuallayer->m_LayerID);
    if (!DataBaseQuery(myQ)) {
        return false;
    }

    DataBaseResult myResult;
    DataBaseGetResults(&myResult);

    while (myResult.NextRow()) {
        long myFieldID = wxNOT_FOUND;
        long myLayerIndex = wxNOT_FOUND;
        wxString myFieldName;
        long myEnumID = wxNOT_FOUND;
        wxString myEnumCode;
        wxString myEnumName;

        wxASSERT(myResult.GetColCount() == 6);
        myResult.GetValue(0, myFieldID);
        myResult.GetValue(1, myLayerIndex);
        myResult.GetValue(2, myFieldName);
        myResult.GetValue(3, myEnumID);
        myResult.GetValue(4, myEnumCode);
        myResult.GetValue(5, myEnumName);

        for (unsigned int f = 0; f < fieldarray.GetCount(); f++) {
            ProjectDefMemoryFields* myField = fieldarray[f];
            wxASSERT(myField);
            if (myField->m_Fieldname == myFieldName) {
                myField->m_FieldID = myFieldID;
                myField->m_FieldType = TM_FIELD_ENUMERATION;

                ProjectDefMemoryFieldsCodedVal* myCVal = new ProjectDefMemoryFieldsCodedVal();
                myCVal->m_ValueID = myEnumID;
                myCVal->m_ValueCode = myEnumCode;
                myCVal->m_ValueName = myEnumName;
                myField->m_pCodedValueArray.Add(myCVal);
                break;
            }
        }
    }
    return true;
}

/***************************************************************************/ /**
  @brief Prepare statement for updating fields
  @details This function prepare the SQL sentence used for updating the fields.
  The main concept is the ProjectDefMemoryFields::m_FieldID :
  - if this integer is greather than 0, we update the field
  - if this integer is egal to 0, we prepare a sentence for adding a new field
  @param myField a #ProjectDefMemoryFields object containing all the field data
  @param iLayer The layer index the field belong to.
  @param sSqlSentence the string containing the SQL sentence we want to modify
  @return  Allways true for the moment
  @author Lucien Schreiber (c) CREALP 2007
  @date 23 April 2008
  *******************************************************************************/
bool DataBaseTM::UpdateField(ProjectDefMemoryFields* myField, int iLayer, wxString& sSqlSentence) {
    bool IsNewField = true;
    if (myField->m_FieldID != wxNOT_FOUND) {
        IsNewField = false;
    }

    // special case for enumeration (new database structure see bug #217)
    if (myField->m_FieldType == TM_FIELD_ENUMERATION) {
        // add or update into dmn_layer_attribut (attributs list).
        wxString myQuery = wxEmptyString;
        if (IsNewField) {
            myQuery = wxString::Format(_T("INSERT INTO %s (LAYER_INDEX, ATTRIBUT_NAME) VALUES (%d, \"%s\")"),
                                       TABLE_NAME_AT_LIST, iLayer, myField->m_Fieldname);
        } else {
            myQuery = wxString::Format(_T("UPDATE %s SET ATTRIBUT_NAME=\"%s\" WHERE ATTRIBUT_ID=%d"),
                                       TABLE_NAME_AT_LIST, myField->m_Fieldname, myField->m_FieldID);
        }
        if (!DataBaseQueryNoResults(myQuery)) {
            return false;
        }

        if (IsNewField) {
            myField->m_FieldID = DataBaseGetLastInsertedID();
            wxASSERT(myField->m_FieldID != wxNOT_FOUND);
        }

        // add or update every values into catalog
        for (unsigned int i = 0; i < myField->m_pCodedValueArray.GetCount(); i++) {
            ProjectDefMemoryFieldsCodedVal* myFieldVal = myField->m_pCodedValueArray.Item(i);
            wxASSERT(myFieldVal);
            wxString myQ = wxEmptyString;
            if (myFieldVal->m_ValueID == wxNOT_FOUND) {
                myQ = wxString::Format(_T("INSERT INTO %s (CODE, DESCRIPTION_0) VALUES (\"%s\", \"%s\")"),
                                       TABLE_NAME_AT_CATALOG, myFieldVal->m_ValueCode, myFieldVal->m_ValueName);
            } else {
                myQ = wxString::Format(_T("UPDATE %s SET CODE=\"%s\", DESCRIPTION_0=\"%s\" WHERE CATALOG_ID=%ld"),
                                       TABLE_NAME_AT_CATALOG, myFieldVal->m_ValueCode, myFieldVal->m_ValueName,
                                       myFieldVal->m_ValueID);
            }
            if (!DataBaseQueryNoResults(myQ)) {
                continue;
            }

            if (myFieldVal->m_ValueID == wxNOT_FOUND) {
                myFieldVal->m_ValueID = DataBaseGetLastInsertedID();
            }
        }

        // add into mix table (only for not existing values)
        for (unsigned int i = 0; i < myField->m_pCodedValueArray.GetCount(); i++) {
            ProjectDefMemoryFieldsCodedVal* myFieldVal = myField->m_pCodedValueArray.Item(i);
            wxASSERT(myFieldVal);
            if (myFieldVal->m_ValueID == wxNOT_FOUND) {
                continue;
            }
            wxString myQ = wxString::Format(_T("REPLACE INTO %s VALUES (%d, %ld)"), TABLE_NAME_AT_MIX,
                                            myField->m_FieldID, myFieldVal->m_ValueID);
            if (!DataBaseQueryNoResults(myQ)) {
                return false;
            }
        }
    }

    wxString myTypeFieldTemp = wxEmptyString;
    if (IsNewField) {
        myField->GetStringTypeFromValues(myTypeFieldTemp);
        sSqlSentence.Append(wxString::Format(_T("ALTER TABLE %s%d ADD COLUMN %s %s; "), TABLE_NAME_LAYER_AT.c_str(),
                                             iLayer, myField->m_Fieldname.c_str(), myTypeFieldTemp.c_str()));
    } else {
        myField->GetStringTypeFromValues(myTypeFieldTemp);
        wxString myModifyQuery = wxString::Format(_T("ALTER TABLE %s%d CHANGE COLUMN %s %s %s; "),
                                                  TABLE_NAME_LAYER_AT.c_str(), iLayer, myField->m_FieldOldName.c_str(),
                                                  myField->m_Fieldname.c_str(), myTypeFieldTemp.c_str());
        sSqlSentence.Append(myModifyQuery);
    }
    return true;
}

/***************************************************************************/ /**
  @brief Prepare statement for deleting
  @details This prepare the SQL statement for deleting fields stored into the
  PrjDefMemManage::m_StoreDeleteFields.
  @param myFields the array containing the name of the fields we want to delete
  @param iLayer the zero based layer index of the fields (Number used for table
  name)
  @param sSqlSentence The string we filled with the issued SQL sentence
  @return  TRUE if the passed Array (myFields) is greater than 0
  @author Lucien Schreiber (c) CREALP 2007
  @date 28 April 2008
  *******************************************************************************/
bool DataBaseTM::DeleteField(wxArrayString& myFields, int iLayer, wxString& sSqlSentence) {
    sSqlSentence.Clear();
    int myNumberOfItems = myFields.GetCount();
    for (unsigned int i = myNumberOfItems; i > 0; i--) {
        sSqlSentence.Append(wxString::Format(_T("ALTER TABLE %s%d DROP COLUMN %s; "), TABLE_NAME_LAYER_AT.c_str(),
                                             iLayer, myFields.Item(i - 1).c_str()));
    }

    if (myNumberOfItems == 0) {
        return false;
    }

    for (unsigned int i = myNumberOfItems; i > 0; i--) {
        // delete enumerations from catalog if needed!
        wxString myQuery = wxString::Format(
            _T("SELECT ATTRIBUT_ID FROM %s WHERE LAYER_INDEX=%d AND ATTRIBUT_NAME=\"%s\""), TABLE_NAME_AT_LIST, iLayer,
            myFields.Item(i - 1));
        if (!DataBaseQuery(myQuery)) {
            wxLogError(_("Error selecting attribut id"));
            return false;
        }
        if (!DataBaseHasResults()) {
            // no results... go to the next field
            continue;
        }

        DataBaseResult* myResult = new DataBaseResult();
        DataBaseGetResults(myResult);
        wxASSERT(myResult->GetRowCount() == 1);
        myResult->NextRow();
        long myAttributID = wxNOT_FOUND;
        myResult->GetValue(0, myAttributID);
        wxDELETE(myResult);

        // delete from catalog,
        wxString myDeleteTxt = wxString::Format(
            _T("DELETE from %s WHERE CATALOG_ID IN (SELECT CATALOG_ID FROM %s WHERE ATTRIBUT_ID = %ld);"),
            TABLE_NAME_AT_CATALOG, TABLE_NAME_AT_MIX, myAttributID);
        myDeleteTxt.Append(
            wxString::Format(_T(" DELETE from %s WHERE ATTRIBUT_ID = %ld;"), TABLE_NAME_AT_MIX, myAttributID));
        myDeleteTxt.Append(
            wxString::Format(_T(" DELETE from %s WHERE ATTRIBUT_ID = %ld;"), TABLE_NAME_AT_LIST, myAttributID));

        if (!DataBaseQueryNoResults(myDeleteTxt)) {
            wxLogError(_T("Error deleting field (attribut_id = %ld)"), myAttributID);
            return false;
        }
    }
    return true;
}

bool DataBaseTM::DeleteFields(int iLayer) {
    // delete enumerations from catalog if needed!
    wxString myQuery = wxString::Format(_T("SELECT ATTRIBUT_ID FROM %s WHERE LAYER_INDEX=%d"), TABLE_NAME_AT_LIST,
                                        iLayer);
    if (!DataBaseQuery(myQuery)) {
        wxLogError(_("Error selecting attribut id"));
        return false;
    }
    if (!DataBaseHasResults()) {
        // no results... go to the next field
        return true;
    }

    DataBaseResult* myResult = new DataBaseResult();
    DataBaseGetResults(myResult);
    wxArrayLong myAttributIDs;
    for (long i = 0; i < myResult->GetRowCount(); i++) {
        myResult->NextRow();
        long myAttributID = wxNOT_FOUND;
        myResult->GetValue(0, myAttributID);
        myAttributIDs.Add(myAttributID);
    }
    wxDELETE(myResult);

    // delete from catalog,
    for (unsigned int c = 0; c < myAttributIDs.GetCount(); c++) {
        wxString myDeleteTxt = wxString::Format(
            _T("DELETE from %s WHERE CATALOG_ID IN (SELECT CATALOG_ID FROM %s WHERE ATTRIBUT_ID = %ld);"),
            TABLE_NAME_AT_CATALOG, TABLE_NAME_AT_MIX, myAttributIDs.Item(c));
        myDeleteTxt.Append(
            wxString::Format(_T(" DELETE from %s WHERE ATTRIBUT_ID = %ld;"), TABLE_NAME_AT_MIX, myAttributIDs.Item(c)));
        myDeleteTxt.Append(wxString::Format(_T(" DELETE from %s WHERE ATTRIBUT_ID = %ld;"), TABLE_NAME_AT_LIST,
                                            myAttributIDs.Item(c)));
        if (!DataBaseQueryNoResults(myDeleteTxt)) {
            wxLogError(_T("Error deleting field (attribut_id = %ld)"), myAttributIDs.Item(c));
            return false;
        }
    }
    return true;
}

/*************************** DATABASE QUERY FUNCTION ****************************/
/***************************************************************************/ /**
  @brief Retrive objects from the object table
  @details This functions may be called for getting all objects from within a
  layer type. This is used by the "Object definition dialog" but also by the
  "Attribution window".
  @param ilayertype the zero based type of layer, see #PRJDEF_LAYERS_TYPE
  @param bOrder Return the result in the "RANK" order if set to TRUE
  @return  FALSE if the query wasn't valid
  @author Lucien Schreiber (c) CREALP 2008
  @date 21 May 2008
  *******************************************************************************/
bool DataBaseTM::GetObjectListByLayerType(int ilayertype, bool bOrder) {
    wxString sSentence = wxString::Format(
        _T("SELECT OBJECT_CD, OBJECT_DESC_0, OBJECT_TYPE_CD, thematic_layers.LAYER_NAME, OBJECT_ISFREQ, OBJECT_ID ")
        _T("FROM dmn_layer_object LEFT JOIN (thematic_layers) ")
        _T("ON (thematic_layers.LAYER_INDEX=dmn_layer_object.THEMATIC_LAYERS_LAYER_INDEX)")
        _T("WHERE OBJECT_TYPE_CD = %d"),
        ilayertype);

    if (bOrder) sSentence.Append(_T(" ORDER BY RANK "));

    if (DataBaseQuery(sSentence)) {
        return true;
    }
    wxLogError(_T("Error getting the list of object by layertype : %s"), sSentence.c_str());
    return false;
}

/***************************************************************************/ /**
  @brief Return list of layers name for selected type
  @details This function returns all layers name corresponding to a layer type.
  Layer type are defined there : #PRJDEF_LAYERS_TYPE
  @note There is a special
  behaviour in case of ilayertype = LAYER_LINE (0). In this case the function
  returns all line layers and all polygons layers too. Because we should be able
  to define lines for creating polygons
  @see Bug #25
  @param ilayertype one of the #PRJDEF_LAYERS_TYPE
  @return  An array containing all layers name for the selected layer type
  @author Lucien Schreiber (c) CREALP 2008
  @date 31 October 2008
  *******************************************************************************/
wxArrayString DataBaseTM::GetLayerNameByType(int ilayertype) {
    wxArrayString myThematicResult;
    wxString sSentence = _T("");

    // normal behaviour
    if (ilayertype != LAYER_LINE) {
        sSentence = wxString::Format(_T("SELECT LAYER_NAME FROM %s WHERE TYPE_CD = %d"), TABLE_NAME_LAYERS.c_str(),
                                     ilayertype);
    } else  // special behavious, see documentation above.
    {
        sSentence = wxString::Format(_T("SELECT LAYER_NAME FROM %s WHERE TYPE_CD=%d OR TYPE_CD=%d"),
                                     TABLE_NAME_LAYERS.c_str(), LAYER_LINE, LAYER_POLYGON);
    }

    if (DataBaseQuery(sSentence)) {
        if (!DataBaseGetResults(myThematicResult)) {
            wxLogDebug(_("No result from database."));
        }
    }
    // wxLogDebug(_T("Number of results found : %d"), myThematicResult.GetCount());
    return myThematicResult;
}

/********************************** SCALE OPERATIONS **********************************/
long DataBaseTM::GetNextScaleValue(long& DBindex, bool bFirst) {
    long myResultScale = wxNOT_FOUND;
    wxArrayLong myResults;

    // no result, we process the sentence
    // bool dRes = DataBaseHasResults();
    if (!DataBaseHasResults() || bFirst) {
        wxString sSentence = wxString::Format(_T("SELECT ZOOM_ID, SCALE_VALUE FROM %s ORDER BY RANK"),
                                              TABLE_NAME_SCALE.c_str());

        if (!DataBaseQuery(sSentence)) {
            wxLogDebug(_T("Error getting scale data from the database : %s"), sSentence.c_str());
        }
    }

    if (!DataBaseGetNextResult(myResults)) {
        DataBaseClearResults();
        return wxNOT_FOUND;
    }

    wxASSERT(myResults.GetCount() == 2);

    DBindex = myResults.Item(0);
    myResultScale = myResults.Item(1);
    return myResultScale;
}

bool DataBaseTM::EditScale(ProjectDefMemoryScale* myScaleObj) {
    // prepare the sentence for insert or update
    // sentence for insert
    wxString sInsert = wxString::Format(
        _T("INSERT INTO %s ")
        _T("(SCALE_VALUE) ")
        _T("VALUES (%ld)"),
        TABLE_NAME_SCALE.c_str(), myScaleObj->m_ScaleValue);
    // sentence for update
    wxString sUpdate = wxString::Format(
        _T("UPDATE %s ")
        _T("SET SCALE_VALUE = %ld ")
        _T("WHERE ZOOM_ID = %ld"),
        TABLE_NAME_SCALE.c_str(), myScaleObj->m_ScaleValue, myScaleObj->m_DBScaleID);

    // if id >= 0 we update (item exist)
    // otherwise we insert (item dosen't exist)
    if (myScaleObj->m_DBScaleID >= 0) {
        if (DataBaseQueryNoResults(sUpdate)) return true;
    } else {
        if (DataBaseQueryNoResults(sInsert)) return true;
    }

    wxLogError(_("Error editing scale into the database"));
    return false;
}

int DataBaseTM::DeleteMultipleScales(PrjDefMemManage* pProjet) {
    wxString sSentence = _T("");
    wxString sPattern = _T("");

    // prepare statement for multiple delete
    for (unsigned int i = 0; i < pProjet->m_StoreDeleteScale.GetCount(); i++) {
        sSentence.Append(wxString::Format(_T("DELETE FROM %s WHERE ZOOM_ID = %ld; "), TABLE_NAME_SCALE.c_str(),
                                          pProjet->m_StoreDeleteScale.Item(i)));
    }

    // execute statement
    if (DataBaseQueryNoResults(sSentence)) return pProjet->m_StoreDeleteIDObj.GetCount();

    wxLogDebug(_T("Error trying to delete scale from the database : %s"), sSentence.c_str());
    return -1;
}

/********************************** RANK DATABASE OPERATIONS **********************************/
/***************************************************************************/ /**
  @brief Update a rank field
  @details This may be used to update a rank field using items from a
  #ListGenReport. This way we can store the order of items wanted by the end
  user
  @param list a pointer to a #ListGenReport object containing items
  @param icol specify the column of interest in the list (zero based index)
  @param stable the name of the table in which we want to add rank values
  @param sfield the field name for the field containing values of the selected
  column (icol)
  @param bStringType TRUE if the column store textual values, FALSE otherwise. In
  fact this function prepend brackets if TRUE for dealing with strings with
  spaces (Default is FALSE)
  @param rankfield Name of the field where we want to update the rank (Default is
  field "RANK") this field must be of type integer
  @return  Return TRUE if the query pass without problem and FALSE otherwise
  @author Lucien Schreiber (c) CREALP 2007
  @date 01 April 2008
  *******************************************************************************/
bool DataBaseTM::SetRank(ListGenReport* list, int icol, const wxString& stable, const wxString& sfield,
                         bool bStringType, const wxString& rankfield) {
    wxString myText = _T("");
    wxString sSentence = _T("");

    // loop all items from the list and update the rank in one pass...
    for (int i = 0; i < list->GetItemCount(); i++) {
        myText = list->GetItemColText(i, icol);

        // if this is a string value we add brackets...
        if (bStringType) {
            myText.Prepend(_T("\""));
            myText.Append(_T("\""));
        }

        // prepare the sentence
        sSentence.Append(wxString::Format(_T("UPDATE %s SET %s = %d WHERE %s = %s; "), stable.c_str(),
                                          rankfield.c_str(), i, sfield.c_str(), myText.c_str()));
    }

    // process the sentence
    if (DataBaseQueryNoResults(sSentence)) {
        return true;
    }

    wxLogDebug(_T("Error updating RANK : %s"), sSentence.c_str());
    return false;
}

/***************************************************************************/ /**
  @brief Update a rank field for scale table
  @details This function is a specialized function of DataBaseTM::SetRank() used
  to update rank for scale table (because of scale table store only long values
  but displays 1: before the scale value
  @param list a pointer to a #ScaleList object containing items
  @param icol specify the column of interest in the list (zero based index)
  @param stable the name of the table in which we want to add rank values
  @param sfield the field name for the field containing values of the selected
  column (icol)
  @param rankfield Name of the field where we want to update the rank (Default is
  field "RANK") this field must be of type integer
  @return  Return TRUE if the query pass without problem and FALSE otherwise
  @author Lucien Schreiber (c) CREALP 2007
  @date 01 April 2008
  *******************************************************************************/
bool DataBaseTM::SetScaleRank(ScaleList* list, int icol, const wxString& stable, const wxString& sfield,
                              const wxString& rankfield) {
    long myScale = 0;
    wxString sSentence = _T("");

    // loop all items from the list and update the rank in one pass...
    for (int i = 0; i < list->GetItemCount(); i++) {
        myScale = list->GetScaleFromList(i);

        // prepare the sentence
        sSentence.Append(wxString::Format(_T("UPDATE %s SET %s = %d WHERE %s = %ld; "), stable.c_str(),
                                          rankfield.c_str(), i, sfield.c_str(), myScale));
    }

    // process the sentence
    if (DataBaseQueryNoResults(sSentence)) {
        return true;
    }

    wxLogDebug(_T("Error updating scale RANK : %s"), sSentence.c_str());
    return false;
}

/********************************** PROJECT DATABASE OPERATIONS **********************************/
PrjDefMemManage* DataBaseTM::GetProjectDataFromDB() {
    PrjDefMemManage* myPrjDef = new PrjDefMemManage();
    int iLayerAdded = 0, iReturnValue = 0;

    // Load General project data (path, name,...)
    if (GetProjectData(myPrjDef)) {
        // STEP 1
        // get all layers
        while (1) {
            ProjectDefMemoryLayers* mypLayer = myPrjDef->AddLayer();
            iReturnValue = GetNextLayer(mypLayer);

            // first turn return 0, then -1 if an error occur
            if (iReturnValue != 1) {
                // remove last layer
                myPrjDef->RemoveLayer();
                if (iReturnValue != 0) {
                    break;
                }
            } else {
                iLayerAdded++;
            }
        }

        // load objects for all layers
        for (unsigned int i = 0; i < myPrjDef->m_PrjLayerArray.GetCount(); i++) {
            ProjectDefMemoryLayers* myLayer = myPrjDef->m_PrjLayerArray.Item(i);
            wxASSERT(myLayer);
            LoadLayerObjects(myLayer);
        }

        // load fields
        GetFieldsFromDB(myPrjDef);

        // wxLogDebug(_T("Number of fields parsed %d"), iNumFieldAdded);
        // wxLogDebug(_T("Nb of layers found in db : %d"), iLayerAdded);

        return myPrjDef;
    }

    wxLogError(_T("Error loading basic project data"));

    wxDELETE(myPrjDef);

    return nullptr;
}

/***************************************************************************/ /**
  @brief Update database
  @details This function update the project's data. This is a "clever" function,
  only modified data is changed
  @param pProjDef Object of type #PrjDefMemManage containing new project values.
  @return  return TRUE if project was updated successfully
  @author Lucien Schreiber (c) CREALP 2007
  @date 14 April 2008
  *******************************************************************************/
bool DataBaseTM::UpdateDataBaseProject(PrjDefMemManage* pProjDef) {
    wxString sSentence = _T("");
    wxString sFieldSentence = _T("");
    wxString sDeleteString = _T("");
    ProjectDefMemoryLayers* pLayers = nullptr;
    bool bReturn = true;

    /********* PART 1 : BASIC PROJECT UPDATING *********************/
    if (SetProjectData(pProjDef)) {
        /********* PART 2 : DELETING AND CLEANING *********************/
        // we prepare the sentence for deleting layers
        // based on the delete array
        DeleteLayer(pProjDef->m_StoreDeleteLayers, pProjDef, sSentence);

        // execute the sentence for deleting
        if (!sSentence.IsEmpty()) {
            if (!DataBaseQueryNoResults(sSentence)) {
                wxLogError(_T("Error deleting layers : %s"), sSentence.c_str());
                bReturn = false;
                return bReturn;
            }
        }
        // clean the delete sentence
        sSentence.Clear();

        /********* PART 3 : INSERT AND UPDATE *********************/

        // prepare sentence for updating layers (insert or modify)
        for (int i = 0; i < pProjDef->GetCountLayers(); i++) {
            pLayers = pProjDef->GetNextLayer();
            UpdateLayer(pLayers, sSentence);

            // check if a table exists for fields
            if (TableExist(wxString::Format(_T("%s%d"), TABLE_NAME_LAYER_AT.c_str(), pLayers->m_LayerID))) {
                // check if we have some fields
                if (pProjDef->GetCountFields() > 0) {
                    // fields exists, we update, insert them
                    sFieldSentence.Clear();
                    for (int j = 0; j < pProjDef->GetCountFields(); j++) {
                        UpdateField(pProjDef->GetNextField(), pLayers->m_LayerID, sFieldSentence);
                    }

                    // modfiy the fields in the database
                    if (!DataBaseQueryNoResults(sFieldSentence)) {
                        wxLogError(_T("Error modifing field data in the database : %s"), sFieldSentence.c_str());
                        bReturn = false;
                        break;
                    }
                }

                // remove coded values if needed from everywhere
                for (int f = 0; f < pProjDef->GetCountFields(); f++) {
                    ProjectDefMemoryFields* myField = pProjDef->FindField(f);

                    // delete coded values from field before deleting the coded values themself.
                    for (unsigned int c = 0; c < myField->m_StoreDeleteCodedValues.GetCount(); c++) {
                        long myIdToDelete = myField->m_StoreDeleteCodedValues.Item(c);
                        wxString myQuery = wxString::Format(_T("UPDATE %s%d SET %s = NULL WHERE %s=%ld; "),
                                                            TABLE_NAME_LAYER_AT, pLayers->m_LayerID,
                                                            myField->m_Fieldname, myField->m_Fieldname, myIdToDelete);

                        myQuery.Append(wxString::Format(
                            _T("DELETE FROM %s WHERE CATALOG_ID=%ld; DELETE FROM %s WHERE CATALOG_ID=%ld;"),
                            TABLE_NAME_AT_CATALOG, myIdToDelete, TABLE_NAME_AT_MIX, myIdToDelete));

                        if (!DataBaseQueryNoResults(myQuery)) {
                            continue;
                        }
                    }
                }

                // process for deleting fields // normally the table should exists
                sDeleteString.Clear();
                if (pLayers->m_StoreDeleteFields.GetCount() > 0) {
                    DeleteField(pLayers->m_StoreDeleteFields, pLayers->m_LayerID, sDeleteString);

                    if (!DataBaseQueryNoResults(sDeleteString)) {
                        wxLogError(_T("Unable to delete Fields from table : %s%ld, %s"), TABLE_NAME_LAYER_AT.c_str(),
                                   pLayers->m_LayerID, sDeleteString.c_str());
                        bReturn = false;
                        break;
                    }
                }

            } else  // the table dosen't exist, need to create the table if fields > 0
            {
                if (pProjDef->GetCountFields() > 0) {
                    // create the table
                    if (!AddTableIfNotExist(wxString::Format(TABLE_NAME_LAYER_AT + _T("%d"), pLayers->m_LayerID))) {
                        wxLogError(_T("Unable to create the Field's table : %d"), pLayers->m_LayerID);
                        bReturn = false;
                        break;
                    }

                    // update, insert fields
                    sFieldSentence.Clear();
                    for (int j = 0; j < pProjDef->GetCountFields(); j++) {
                        UpdateField(pProjDef->GetNextField(), pLayers->m_LayerID, sFieldSentence);
                    }

                    // modfiy the fields in the database
                    if (!DataBaseQueryNoResults(sFieldSentence)) {
                        wxLogError(_T("Error modifing field data in the database : %s"), sFieldSentence.c_str());
                        bReturn = false;
                        break;
                    }
                }
            }
        }

        // execute the sentence for layers.
        if (!sSentence.IsEmpty() && !DataBaseQueryNoResults(sSentence)) {
            wxLogError(_T("Error updating project in the database"));
            bReturn = false;
        }

    } else
        bReturn = false;

    return bReturn;
}

/***************************************************************************/ /**
  @brief Init the TOC
  @details This function fill the TOC with generic layers which are :
  - <B>generic_points</B> : For storing all points
  - <B>generic_lines</B> : For storing all lines and limits of polygons
  - <B>generic_labels</B> : For storing all polygon's labels
  - <B>generic_notes</B> : For storing all notes, errors, questions.
  @note User could'nt remove those layers. Value of column GENERIC_LAYERS is linked to
  the #TOC_GENERIC_NAME
  @return  TRUE if table was succesfully filled, FALSE if an error occur (see log)
  @author Lucien Schreiber (c) CREALP 2008
  @date 26 June 2008
  *******************************************************************************/
bool DataBaseTM::InitTOCGenericLayers() {
    // check that a database is opened
    if (DataBaseGetName() == wxEmptyString) {
        wxLogDebug(_T("Database not opened, open database first"));
        return false;
    }

    int iTypes[] = {LAYER_LINE, LAYER_POINT, LAYER_POINT, LAYER_POINT, LAYER_LINE};

    wxString sSentence = _T("INSERT INTO ") + TABLE_NAME_TOC +
                         _T(" (TYPE_CD, ")
                         _T("CONTENT_NAME, GENERIC_LAYERS) VALUES (%d, \"%s\", %d); ");
    wxString sRealSentence = _T("");

    for (int i = TOC_NAME_LINES; i <= TOC_NAME_FRAME; i++) {
        sRealSentence.Append(
            wxString::Format(sSentence, iTypes[i], TOC_GENERIC_NAME_STRING[i].c_str(), TOC_NAME_LINES + i));
    }

    if (!DataBaseQueryNoResults(sRealSentence)) {
        wxLogDebug(_T("Error in TOC init : ") + sRealSentence);
        return false;
    }

    return true;
}

/***************************************************************************/ /**
  @brief Get entry from the TOC
  @details This function returns all entry from the TOC one by one. Only one
  request is issued and then each time this function is called the next layer is
  returned
  @return  a #tmLayerProperties pointer, or nullptr if no more layer to proceed
  @note if result isn't nullptr, you need to delete pointer after use.
  @author Lucien Schreiber (c) CREALP 2008
  @date 07 July 2008
  *******************************************************************************/
tmLayerProperties* DataBaseTM::GetNextTOCEntry(bool userelativepath) {
    wxArrayString myTempResults;
    if (!DataBaseHasResults()) {
        wxString sSentence =
            _T("SELECT CONTENT_ID, TYPE_CD, CONTENT_PATH, ")
            _T("CONTENT_NAME, CONTENT_STATUS, GENERIC_LAYERS, SYMBOLOGY, VERTEX_FLAGS, LABEL_VISIBLE, LABEL_DEF, ")
            _T("PARENT_ID FROM ") +
            TABLE_NAME_TOC + _T(" ORDER by RANK");

        if (!DataBaseQuery(sSentence)) {
            wxLogError(_T("Error getting layers from the TOC : %s"), sSentence.c_str());
            return nullptr;
        }
    }

    if (!DataBaseGetNextResult(myTempResults)) {
        DataBaseClearResults();
        return nullptr;
    }

    wxASSERT(myTempResults.GetCount() == 11);
    // parsing results
    tmLayerProperties* myLayerProp = new tmLayerProperties();
    myLayerProp->InitFromArray(myTempResults, userelativepath, DataBaseGetPath());
    myTempResults.Clear();
    return myLayerProp;
}

/***************************************************************************/ /**
  @brief Add TOC item to the database
  @details Called when a new layer is added.
  @param item The item to add to the database
  @return  The database ID of the added item or -1 if an error occur
  @author Lucien Schreiber (c) CREALP 2008
  @date 14 July 2008
  *******************************************************************************/
long DataBaseTM::AddTOCLayer(tmLayerProperties* item) {
    wxString myPath = item->GetName().GetPath();

    // converting path to windows path
    // do nothing if not a windows path.
    DataBaseTM::ConvertPath(myPath);

    wxString sSentence = wxString::Format(_T("INSERT INTO ") + TABLE_NAME_TOC +
                                              _T(" (TYPE_CD, CONTENT_PATH, CONTENT_NAME,")
                                              _T("CONTENT_STATUS, GENERIC_LAYERS) ") +
                                              _T(" VALUES ( %d, \"%s\", \"%s\", %d, %d ); "),
                                          item->GetSpatialType(), myPath, item->GetName().GetFullName().c_str(),
                                          item->IsVisible(), item->GetType());

    if (!DataBaseQueryNoResults(sSentence)) {
        wxLogError(_T("Error inserting layer into database : %s"), sSentence.c_str());
        return -1;
    }

    long myLID = DataBaseGetLastInsertedID();
    wxASSERT(myLID != wxNOT_FOUND);

    return myLID;
}

/***************************************************************************/ /**
  @brief Delete a TOC item from the database
  @details Called when a layer is removed.
  @param itemid database ID of the item to remove
  @return  TRUE if item sucessfully removed, FALSE otherwise
  @author Lucien Schreiber (c) CREALP 2008
  @date 14 July 2008
  *******************************************************************************/
bool DataBaseTM::RemoveTOCLayer(const long& itemid) {
    wxString sSentence = wxString::Format(_T("DELETE FROM ") + TABLE_NAME_TOC + _T(" WHERE CONTENT_ID = %ld"), itemid);
    if (!DataBaseQueryNoResults(sSentence)) {
        wxLogDebug(_T("Not able to remove item from DB : %s"), sSentence.c_str());
        return false;
    }

    return true;
}

void DataBaseTM::PrepareTOCStatusUpdate(wxString& sentence, tmLayerProperties* item, int itemRank,
                                        const wxString& symbology) {
    wxString myPath = item->GetName().GetPath();
    // converting path to windows path
    // do nothing if not a windows path.
    DataBaseTM::ConvertPath(myPath);
    wxString myLabelDef = item->GetLabelDefinition();
    myLabelDef.Replace(_T("\\"), _T("\\\\"));
    myLabelDef.Replace(_T("'"), _T("\\'"));
    myLabelDef.Replace(_T("\""), _T("\\\""));

    sentence.Append(
        wxString::Format(_T("UPDATE ") + TABLE_NAME_TOC +
                             _T(" SET CONTENT_STATUS = %d, RANK=%d, SYMBOLOGY=\"%s\",")
                             _T(" VERTEX_FLAGS = %d, CONTENT_PATH=\"%s\", LABEL_VISIBLE = %d, LABEL_DEF = \"%s\",")
                             _T(" PARENT_ID=%ld ")
                             _T(" WHERE CONTENT_ID = %ld; "),
                         item->IsVisible(), itemRank, symbology.c_str(), item->GetVertexFlags(), myPath,
                         item->IsLabelVisible(), myLabelDef, item->GetLayerParentId(), item->GetID()));
}

/***************************************************************************/ /**
  @brief Get all queries
  @details Return all queries one after the other. Call this function
  recursivelly for getting all queries.
  @param qid return the ID of the query
  @param name return the name of the query
  @param description return the description of the query
  @param bfirst set to true during the first call for creating the MySQL statement
  @return  true if a query was returned successfull
  @author Lucien Schreiber (c) CREALP 2008
  @date 10 November 2008
  *******************************************************************************/
bool DataBaseTM::GetNextQueries(long& qid, wxString& name, wxString& description, bool bfirst) {
    wxString sStatement = _T("");
    if (!DataBaseHasResults() && bfirst) {
        sStatement = _T("SELECT * FROM ") + TABLE_NAME_QUERIES;
        if (!DataBaseQuery(sStatement)) {
            wxLogDebug(_T("Error getting queries"));
            return false;
        }
    }

    wxArrayString myResults;
    if (!DataBaseGetNextResult(myResults)) {
        DataBaseClearResults();
        return false;
    }

    wxASSERT(myResults.GetCount() == 4);

    /* no more results
    if (myResults.GetCount() == 0)
        return false;

    // invalid result
    if (myResults.GetCount() != 4)
    {
        wxLogDebug(_T("Error, number of data retreived doesn't correspond to what attended : %d"),
                   myResults.GetCount());
        return false;
    }*/

    qid = wxAtol(myResults.Item(0));
    name = myResults.Item(2);
    description = myResults.Item(3);

    return true;
}

/***************************************************************************/ /**
  @brief Get a queries by ID
  @details Return the corresponding queries based on ID.
  @param qid MySQL id of the searched queries
  @param target : one of the TOC_GENERIC_NAME value indicating to wich layers the
  queries is dedicated to.
  @param name return the query name
  @param description return the query description
  @return  true if a query returned, false otherwise
  @author Lucien Schreiber (c) CREALP 2008
  @date 10 November 2008
  *******************************************************************************/
bool DataBaseTM::GetQueriesById(const long& qid, int& target, wxString& name, wxString& description) {
    wxString sStatement = wxString::Format(_T("SELECT QUERIES_NAME, QUERIES_CODE, QUERIES_TARGET from ") +
                                               TABLE_NAME_QUERIES + _T(" WHERE QUERIES_ID=%ld;"),
                                           qid);
    if (!DataBaseQuery(sStatement)) {
        wxLogDebug(_T("Error getting query"));
        return false;
    }

    wxArrayString myResults;
    if (!DataBaseGetNextResult(myResults)) {
        DataBaseClearResults();
        return false;
    }
    DataBaseClearResults();
    wxASSERT(myResults.GetCount() == 3);

    name = myResults.Item(0);
    description = myResults.Item(1);
    target = wxAtoi(myResults.Item(2));
    return true;
}

/***************************************************************************/ /**
  @brief Edit or add a query
  @param target Int of the target (allowed values : #TOC_GENERIC_NAME)
  @param name Name of the query
  @param description SQL code of the query
  @param qid ID of the query for modification or -1 for adding query
  @return true if a query was modified or added and false otherwise
  @author Lucien Schreiber (c) CREALP 2008
  @date 10 November 2008
  *******************************************************************************/
bool DataBaseTM::EditQueries(int target, const wxString& name, const wxString& description, long qid) {
    wxString myConvertedDescription;
    if (!DataBaseStringEscapeQuery(description, myConvertedDescription)) {
        wxLogError(_("Unable to convert the SQL statement"));
        return false;
    }

    wxString sAddStatement = wxString::Format(_T("INSERT INTO ") + TABLE_NAME_QUERIES +
                                                  _T(" (QUERIES_TARGET, QUERIES_NAME, QUERIES_CODE)")
                                                  _T(" VALUES (%d,\"%s\", \"%s\")"),
                                              target, name.c_str(), myConvertedDescription.c_str());
    wxString sUpdStatement = wxString::Format(_T("UPDATE ") + TABLE_NAME_QUERIES +
                                                  _T(" SET QUERIES_NAME=\"%s\", ")
                                                  _T(" QUERIES_CODE=\"%s\", QUERIES_TARGET=%d ")
                                                  _T(" WHERE QUERIES_ID=%ld"),
                                              name.c_str(), myConvertedDescription.c_str(), target, qid);
    wxString sStatement = wxEmptyString;
    if (qid == -1)  // adding query
    {
        sStatement = sAddStatement;
    } else  // update statement
    {
        sStatement = sUpdStatement;
    }

    if (!DataBaseQueryNoResults(sStatement)) {
        return false;
    }
    return true;
}

/***************************************************************************/ /**
  @brief Delete a query
  @param qid id of the query to delete
  @return  true if the query was deleted
  @author Lucien Schreiber (c) CREALP 2008
  @date 10 November 2008
  *******************************************************************************/
bool DataBaseTM::DeleteQuery(long qid) {
    wxString sStatement = wxString::Format(_T("DELETE FROM ") + TABLE_NAME_QUERIES + _T(" WHERE QUERIES_ID=%ld"), qid);
    if (!DataBaseQueryNoResults(sStatement)) {
        return false;
    }
    return true;
}

/***************************************************************************/ /**
  @brief Load shortcut from the DB based on layer type
  @param layer_type The layer type, valid values are :
  - TOC_NAME_LINES
  - TOC_NAME_POINTS
  - TOC_NAME_LABELS
  @param key A wxString containing The key (F1...F12)
  @param description A wxString containing the shortcut description
  @param bFirstLoop Indicate if we are running the first loop.
  @return  true if a result was returned, false otherwise
  @author Lucien Schreiber (c) CREALP 2008
  @date 11 December 2008
  *******************************************************************************/
bool DataBaseTM::GetNextShortcutByLayerType(int layer_type, wxString& key, wxString& description, bool bFirstLoop) {
    wxString sValue =
        _T("SELECT DISTINCT a.SHORTCUT_KEY, a.SHORTCUT_DESC FROM %s a, %s b WHERE ")
        _T("b.SHORTCUT_CD = a.SHORTCUT_CD AND b.OBJECT_ID IN (SELECT c.OBJECT_ID FROM %s c ")
        _T("WHERE c.OBJECT_TYPE_CD = %d)");

    // do the query during the first loop
    if (!DataBaseHasResults() && bFirstLoop) {
        wxString sStatement = wxString::Format(sValue, TABLE_NAME_SHORTCUT_DMN.c_str(),
                                               TABLE_NAME_SHORTCUT_LIST.c_str(), TABLE_NAME_OBJECTS.c_str(),
                                               layer_type);

        if (!DataBaseQuery(sStatement)) {
            return false;
        }
    }

    wxArrayString myResults;
    if (!DataBaseGetNextResult(myResults)) {
        DataBaseClearResults();
        return false;
    }
    wxASSERT(myResults.GetCount() == 2);

    key = myResults.Item(0);
    description = myResults.Item(1);
    return true;
}

/***************************************************************************/ /**
  @brief Get All unused shortcut key
  @details All shortcut allready assigned (F1... F12) aren't returned into array.
  @param keylist wxArrayString will be filled with unused shortcut key
  @return  false if an error occur, true otherwise
  @author Lucien Schreiber (c) CREALP 2008
  @date 12 December 2008
  *******************************************************************************/
bool DataBaseTM::GetAllUnusedShortcuts(wxArrayString& keylist) {
    // static sentence, no parameter
    // SELECT  a.SHORTCUT_KEY FROM dmn_shortcut_key a LEFT JOIN shortcut_list b  ON  a.SHORTCUT_CD = b.SHORTCUT_CD WHERE
    // b.SHORTCUT_CD IS nullptr
    wxString sSentence = _T("SELECT a.SHORTCUT_KEY FROM ") + TABLE_NAME_SHORTCUT_DMN + _T(" a LEFT JOIN ") +
                         TABLE_NAME_SHORTCUT_LIST +
                         _T(" b ON  a.SHORTCUT_CD = b.SHORTCUT_CD WHERE  b.SHORTCUT_CD IS NULL");

    if (!DataBaseQuery(sSentence)) {
        return false;
    }

    if (!DataBaseGetResults(keylist)) return false;

    return true;
}

/***************************************************************************/ /**
  @brief Get checked values for given shortcut key
  @param shortcutid ID checked corresponding to the key shortcut
  @param key the shortcut key we are searching values for (F1....F12), values are
  : (1-12)
  @param bFirstLoop true during the first loop (do the query) and should be set
  to false otherwise
  @return  true if ID is correctly returned (shortcutid), false otherwise
  @author Lucien Schreiber (c) CREALP 2008
  @date 15 December 2008
  *******************************************************************************/
bool DataBaseTM::GetNextShortCutObject(long& shortcutid, const int& key, bool bFirstLoop) {
    wxString sSentence = wxString::Format(_T("SELECT OBJECT_ID FROM %s WHERE SHORTCUT_CD = %d; "),
                                          TABLE_NAME_SHORTCUT_LIST.c_str(), key);

    if (bFirstLoop) {
        if (!DataBaseQuery(sSentence)) {
            return false;
        }
    }

    if (!DataBaseGetNextResult(shortcutid)) {
        DataBaseClearResults();
        return false;
    }

    return true;
}

/***************************************************************************/ /**
  @brief Delete specified shortcut
  @details
  @param shortcutkey the shortcut ID used (1.... 12)
  @return  true if query was successfully passed
  @author Lucien Schreiber (c) CREALP 2008
  @date 15 December 2008
  *******************************************************************************/
bool DataBaseTM::DeleteShortcut(int shortcutkey) {
    wxString sSentence = wxString::Format(
        _T("DELETE FROM %s WHERE SHORTCUT_CD = %d; ")
        _T("UPDATE %s SET SHORTCUT_DESC = \"\" WHERE SHORTCUT_CD = %d; "),
        TABLE_NAME_SHORTCUT_LIST.c_str(), shortcutkey, TABLE_NAME_SHORTCUT_DMN.c_str(), shortcutkey);
    if (!DataBaseQueryNoResults(sSentence)) {
        return false;
    }

    return true;
}

/***************************************************************************/ /**
  @brief Append or edit a shortcut
  @param shortcutkey the index of the selected shortcut (1...12)
  @param description Description used for the shortcut
  @param types Array of integer containing the types of objects
  @return  true if the shortcut was edited successfully
  @author Lucien Schreiber (c) CREALP 2008
  @date 15 December 2008
  *******************************************************************************/
bool DataBaseTM::EditShortcut(int shortcutkey, const wxString& description, const wxArrayLong& types) {
    // update dmn_shortcut
    wxString sSentence = wxString::Format(
        _T("UPDATE %s SET SHORTCUT_DESC = ")
        _T(" \"%s\" WHERE SHORTCUT_CD = %d; "),
        TABLE_NAME_SHORTCUT_DMN.c_str(), description.c_str(), shortcutkey);
    sSentence.Append(
        wxString::Format(_T("DELETE FROM %s WHERE SHORTCUT_CD = %d; "), TABLE_NAME_SHORTCUT_LIST.c_str(), shortcutkey));

    for (unsigned int i = 0; i < types.GetCount(); i++)
        sSentence.Append(wxString::Format(_T("INSERT INTO %s VALUES (%ld,%d); "), TABLE_NAME_SHORTCUT_LIST.c_str(),
                                          types.Item(i), shortcutkey));

    if (!DataBaseQueryNoResults(sSentence)) {
        wxLogError(_("Error editing shortcut"));
        return false;
    }

    return true;
}

/***************************************************************************/ /**
  @brief Get Full informations for shortcuts
  @details This function is used for storing shortcuts into memory.
  @param bFirstLoop set to true if this is the first loop (issue the query)
  @param layertype get the layer type (see #PRJDEF_LAYERS_TYPE)
  @param key get the shortcut key (as int, between 1-12)
  @param description get the shortcut description
  @param shortcutvalue get the shortcut value
  @return  return true if all is correct, false otherwise
  @author Lucien Schreiber (c) CREALP 2008
  @date 18 December 2008
  *******************************************************************************/
bool DataBaseTM::GetNextShortcutFull(bool bFirstLoop, int& layertype, int& key, wxString& description,
                                     long& shortcutvalue) {
    wxString sSentence = wxString::Format(
        _T("SELECT  a.SHORTCUT_CD, c.OBJECT_TYPE_CD,")
        _T(" a.SHORTCUT_DESC, b.OBJECT_ID FROM %s a")
        _T(" LEFT JOIN (%s b, %s c) ON  (b.SHORTCUT_CD")
        _T(" = a.SHORTCUT_CD AND b.OBJECT_ID = c.OBJECT_ID)")
        _T(" WHERE b.SHORTCUT_CD = a.SHORTCUT_CD ORDER BY")
        _T(" a.SHORTCUT_CD"),
        TABLE_NAME_SHORTCUT_DMN.c_str(), TABLE_NAME_SHORTCUT_LIST.c_str(), TABLE_NAME_OBJECTS.c_str());
    // running query
    if (bFirstLoop) {
        if (!DataBaseQuery(sSentence)) {
            return false;
        }
    }

    wxArrayString myResultRow;
    if (!DataBaseGetNextResult(myResultRow)) {
        DataBaseClearResults();
        return false;
    }

    wxASSERT(myResultRow.GetCount() == 4);

    key = wxAtoi(myResultRow.Item(0));
    layertype = wxAtoi(myResultRow.Item(1));
    description = myResultRow.Item(2);
    myResultRow.Item(3).ToLong(&shortcutvalue);
    return true;
}

/***************************************************************************/ /**
  @brief Get the snapping from database
  @param lid return the layer id (CONTENT_ID of prj_toc table)
  @param layername return the layer name
  @param snapstatus return the snapping status 0 = no snapping, 1 = Snapping for
  vertex, 2 = Snapping for Begin/End (only for lines)
  @param bfirstloop set to true if the first loop to issue the query
  @return true if the query was successfull, false otherwise
  @author Lucien Schreiber (c) CREALP 2009
  @date 19 January 2009
  *******************************************************************************/
bool DataBaseTM::GetNextSnapping(long& lid, wxString& layername, int& snapstatus, bool bfirstloop) {
    wxString sSentence = wxString::Format(
        _T("SELECT a.TOC_ID, b.CONTENT_NAME, a.SNAPPING_TYPE")
        _T(" from %s AS a LEFT JOIN  (%s b)")
        _T(" ON (b.CONTENT_ID = a.TOC_ID) ORDER BY b.RANK;"),
        TABLE_NAME_SNAPPING.c_str(), TABLE_NAME_TOC.c_str());
    // if first loop
    if (bfirstloop) {
        if (!DataBaseQuery(sSentence)) {
            return false;
        }
    }

    wxArrayString myResultRow;
    if (!DataBaseGetNextResult(myResultRow)) {
        DataBaseClearResults();
        return false;
    }
    wxASSERT(myResultRow.GetCount() == 3);

    myResultRow.Item(0).ToLong(&lid);
    layername = myResultRow.Item(1);
    snapstatus = wxAtoi(myResultRow.Item(2));
    return true;
}

/***************************************************************************/ /**
  @brief Get all layers valid for snapping
  @details This function returns all layers that are :
  - Not allready in the snapping list
  - Valid a.k.a. Vector layers
  @param lids An array filled with list of arrays id
  @param lnames An array filled with list of arrays name
  @return  true if the query was successfull, false otherwise
  @author Lucien Schreiber (c) CREALP 2009
  @date 20 January 2009
  *******************************************************************************/
bool DataBaseTM::GetValidLayersForSnapping(wxArrayLong& lids, wxArrayString& lnames) {
    wxString sSentence = wxString::Format(
        _T("SELECT a.CONTENT_ID, a.CONTENT_NAME")
        _T(" FROM %s a LEFT JOIN (%s b)")
        _T(" ON (a.CONTENT_ID = b.TOC_ID) WHERE  b.TOC_ID")
        _T(" IS NULL AND a.TYPE_CD < %d ORDER BY a.RANK"),
        TABLE_NAME_TOC.c_str(), TABLE_NAME_SNAPPING.c_str(), LAYER_SPATIAL_RASTER);

    if (!DataBaseQuery(sSentence)) {
        return false;
    }

    long myResultlid = 0;
    wxArrayString myResultRow;
    while (DataBaseGetNextResult(myResultRow)) {
        wxASSERT(myResultRow.GetCount() == 2);
        myResultRow.Item(0).ToLong(&myResultlid);
        lids.Add(myResultlid);
        lnames.Add(myResultRow.Item(1));
    }
    DataBaseClearResults();
    return true;
}

/***************************************************************************/ /**
  @brief Save new snapping layers into database
  @details This function add into the database one or more layers used for
  snapping.
  @param lids ids of the layers to add
  @return  true if query was successfull, false otherwise
  @author Lucien Schreiber (c) CREALP 2009
  @date 20 January 2009
  *******************************************************************************/
bool DataBaseTM::AddLayersSnapping(const wxArrayLong& lids, int snappingstatus) {
    wxString sSentence = _T("INSERT INTO %s VALUES (%ld, %d); ");
    wxString sFullSentence = wxEmptyString;

    for (unsigned int i = 0; i < lids.GetCount(); i++) {
        sFullSentence.Append(wxString::Format(sSentence, TABLE_NAME_SNAPPING.c_str(), lids.Item(i), snappingstatus));
    }

    if (!DataBaseQueryNoResults(sFullSentence)) {
        return false;
    }

    return true;
}

/***************************************************************************/ /**
  @brief Delete a snapping layer from the database
  @param layersid the layers ID (TOC_ID field in prj_snapping table)
  @return  true if the item was deleted from the database, false otherwise
  @author Lucien Schreiber (c) CREALP 2009
  @date 20 January 2009
  *******************************************************************************/
bool DataBaseTM::DeleteLayerSnapping(int layersid) {
    wxString sSentence = wxString::Format(_T("DELETE FROM %s WHERE TOC_ID = %d; "), TABLE_NAME_SNAPPING.c_str(),
                                          layersid);
    return DataBaseQueryNoResults(sSentence);
}

/***************************************************************************/ /**
  @brief Save snapping status to the database
  @param snapmemory a valid #tmSnappingMemory object
  @return true if snapping status was saved successfully
  @author Lucien Schreiber (c) CREALP 2009
  @date 21 January 2009
  *******************************************************************************/
bool DataBaseTM::SaveSnappingAllStatus(tmSnappingMemory* snapmemory) {
    wxString sSentence = _T(" UPDATE %s SET SNAPPING_TYPE=%d WHERE TOC_ID=%ld; ");
    wxString sFullSentence = wxEmptyString;

    long myLayerindex = 0;
    int mySnappingStatus = tmSNAPPING_OFF;

    for (unsigned int i = 0; i < snapmemory->GetCount(); i++) {
        if (snapmemory->GetSnappingInfo(i, myLayerindex, mySnappingStatus)) {
            sFullSentence.Append(
                wxString::Format(sSentence, TABLE_NAME_SNAPPING.c_str(), mySnappingStatus, myLayerindex));
        }
    }

    if (sFullSentence.Len() == 0) {
        return true;
    }

    if (!DataBaseQueryNoResults(sFullSentence)) {
        wxLogError(_("Error saving snapping status"));
        return false;
    }

    return true;
}

/***************************************************************************/ /**
  @brief Save the tolerence into database
  @param iTolerence The tolerence
  @return true if the tolerence was successfully saved, false otherwise
  @author Lucien Schreiber (c) CREALP 2009
  @date 22 January 2009
  *******************************************************************************/
bool DataBaseTM::SetSnappingTolerence(int iTolerence) {
    wxString sSentence = wxString::Format(_T(" UPDATE %s SET PRJ_SNAP_TOLERENCE = %d;"),
                                          TABLE_NAME_PRJ_SETTINGS.c_str(), iTolerence);
    if (!DataBaseQueryNoResults(sSentence, false)) {
        wxLogMessage(_("Error saving snapping tolerence"));
        return false;
    }
    return true;
}

/***************************************************************************/ /**
  @brief Get the tolerence from the database
  @return  the tolerence value
  @author Lucien Schreiber (c) CREALP 2009
  @date 22 January 2009
  *******************************************************************************/
int DataBaseTM::GetSnappingTolerence() {
    long iSnappingTol = 0;
    wxString sSentence = wxString::Format(_T(" SELECT PRJ_SNAP_TOLERENCE FROM %s;"), TABLE_NAME_PRJ_SETTINGS.c_str());
    if (!DataBaseQuery(sSentence)) {
        wxLogError(_("Error getting snapping tolerence"));
        return (int)iSnappingTol;
    }

    if (!DataBaseGetNextResult(iSnappingTol)) iSnappingTol = 0;

    DataBaseClearResults();
    return (int)iSnappingTol;
}

/***************************************************************************/ /**
  @brief Delete geometry from the database
  @param selected An array of selected values. Caller must destroy the object.
  This function doesn't take ownership of the object
  @param layertype one of the #TOC_GENERIC_NAME values (must be <
  TOC_NAME_NOT_GENERIC)
  @return  true if geometry was successfully destroyed
  @author Lucien Schreiber (c) CREALP 2009
  @date 05 February 2009
  *******************************************************************************/
bool DataBaseTM::GeometryDelete(wxArrayLong* selected, int layertype) {
    wxString sSentence = wxString::Format(_T("DELETE FROM %s WHERE OBJECT_ID IN ("),
                                          TABLE_NAME_GIS_GENERIC[layertype].c_str());
    for (unsigned int i = 0; i < selected->GetCount(); i++) {
        sSentence.Append(wxString::Format(_T("%ld,"), selected->Item(i)));
    }
    sSentence.RemoveLast(1);
    sSentence.Append(_T(");"));

    if (!DataBaseQueryNoResults(sSentence)) {
        wxLogError(_("Error deleting selected geometry : %s"), sSentence.c_str());
        return false;
    }
    return true;
}

long DataBaseTM::GeometrySave(OGRGeometry* geometry, int layertype) {
    if (geometry == nullptr) {
        return wxNOT_FOUND;
    }

    CPLSetConfigOption("OGR_WKT_PRECISION", "15");

    char* myCharGeom = nullptr;
    geometry->exportToWkt(&myCharGeom);
    wxString mySGeom = wxString::FromAscii(myCharGeom);
    CPLFree(myCharGeom);

    wxString sSentence = wxString::Format(
        _T("INSERT INTO %s (OBJECT_GEOMETRY)")
        _T(" VALUES (GeomFromText('%s'));"),
        TABLE_NAME_GIS_GENERIC[layertype].c_str(), mySGeom.c_str());
    if (!DataBaseQueryNoResults(sSentence)) {
        wxLogError(_T("Error inserting geometry %s into database"), sSentence.c_str());
        return wxNOT_FOUND;
    }
    return DataBaseGetLastInsertedID();
}

bool DataBaseTM::GeometryUpdate(OGRGeometry* geometry, long oid, int layertype) {
    if (geometry == nullptr) {
        return false;
    }

    CPLSetConfigOption("OGR_WKT_PRECISION", "15");

    char* myCharGeom = nullptr;
    geometry->exportToWkt(&myCharGeom);
    wxString mySGeom = wxString::FromAscii(myCharGeom);
    CPLFree(myCharGeom);

    wxString sSentence = wxString::Format(
        _T("UPDATE %s SET OBJECT_GEOMETRY=")
        _T("GeomFromText('%s') WHERE OBJECT_ID=%ld"),
        TABLE_NAME_GIS_GENERIC[layertype].c_str(), mySGeom.c_str(), oid);
    if (!DataBaseQueryNoResults(sSentence)) {
        wxLogError(_("Error updating geometry : %s"), sSentence.c_str());
        return false;
    }
    return true;
}

OGRGeometry* DataBaseTM::GeometryLoad(long oid, int layertype) {
    wxString sSentence = wxString::Format(_T("SELECT (OBJECT_GEOMETRY) FROM %s WHERE OBJECT_ID=%ld;"),
                                          TABLE_NAME_GIS_GENERIC[layertype].c_str(), oid);
    if (!DataBaseQuery(sSentence)) {
        wxLogError(_("Error getting geometry for id: %ld"), oid);
        return nullptr;
    }

    tmArrayULong row_length;
    MYSQL_ROW row;
    if (!DataBaseGetNextRowResult(row, row_length)) {
        DataBaseClearResults();
        wxLogError(_("No geometry found for id : %ld"), oid);
        return nullptr;
    }

    wxASSERT(row_length.GetCount() == 1);
    OGRGeometry* geometry = nullptr;
    // Geometry columns will have the first 4 bytes contain the SRID.
    OGRGeometryFactory::createFromWkb(((unsigned char*)row[0]) + 4, NULL, &geometry, row_length.Item(0) - 4);
    DataBaseClearResults();
    return geometry;
}

/***************************************************************************/ /**
  @brief Delete attribution from the database
  @param selected An array of selected values. Caller must destroy the object.
  This function doesn't take ownership of the object
  @param layertype one of the #TOC_GENERIC_NAME values (must be <
  TOC_NAME_NOT_GENERIC)
  @return  true if attribution for selected geometry was successfully destroyed
  @author Lucien Schreiber (c) CREALP 2009
  @date 05 February 2009
  *******************************************************************************/
bool DataBaseTM::DeleteAttribution(wxArrayLong* selected, int layertype) {
    if (layertype > TOC_NAME_LABELS) return false;

    wxString sSentence = wxString::Format(_T("DELETE FROM %s WHERE OBJECT_GEOM_ID IN ("),
                                          TABLE_NAME_GIS_ATTRIBUTION[layertype].c_str());

    for (unsigned int i = 0; i < selected->GetCount(); i++) {
        sSentence.Append(wxString::Format(_T("%ld,"), selected->Item(i)));
    }
    sSentence.RemoveLast(1);
    sSentence.Append(_T(");"));

    if (!DataBaseQueryNoResults(sSentence)) {
        wxLogDebug(_T("Error deleting selected attribution : %s"), sSentence.c_str());
        return false;
    }
    return true;
}

wxArrayLong DataBaseTM::GetObjectsFromFeature(long selectedfeature, int layertype) {
    wxArrayLong myValues;
    wxASSERT(myValues.GetCount() == 0);
    if (selectedfeature == wxNOT_FOUND) {
        return myValues;
    }

    // don't try to get objects if not basic layers
    // (line, point, labels, notes)
    if (layertype < TOC_NAME_LINES || layertype > TOC_NAME_ANNOTATIONS) {
        return myValues;
    }

    wxString myQuery = wxString::Format(_T("SELECT OBJECT_VAL_ID FROM %s WHERE OBJECT_GEOM_ID = %ld"),
                                        TABLE_NAME_GIS_ATTRIBUTION[layertype], selectedfeature);
    if (!DataBaseQuery(myQuery)) {
        return myValues;
    }

    DataBaseGetResults(myValues);
    return myValues;
}

long DataBaseTM::GetSelectedLayerId(long objectid) {
    if (objectid == wxNOT_FOUND) {
        return wxNOT_FOUND;
    }

    wxString myQuery = wxString::Format(_T("SELECT THEMATIC_LAYERS_LAYER_INDEX FROM %s WHERE OBJECT_ID = %ld"),
                                        TABLE_NAME_OBJECTS, objectid);
    if (!DataBaseQuery(myQuery)) {
        return wxNOT_FOUND;
    }

    long mylayerid = wxNOT_FOUND;
    DataBaseGetNextResult(mylayerid);
    DataBaseClearResults();
    return mylayerid;
}

bool DataBaseTM::DeleteAdvancedAttribution(long selectedobject, long selectedlayerid) {
    if (selectedobject == wxNOT_FOUND || selectedlayerid == wxNOT_FOUND) {
        return false;
    }

    wxString myQuery = wxString::Format(_T("DELETE FROM %s%ld WHERE OBJECT_ID = %ld"), TABLE_NAME_LAYER_AT,
                                        selectedlayerid, selectedobject);
    if (!DataBaseQueryNoResults(myQuery, false)) {
        return false;
    }
    return true;
}

bool DataBaseTM::SaveBezierSettings(BezierSettingsData* data) {
    if (data == nullptr) {
        return false;
    }

    wxString myQuery = wxString::Format(
        _T("UPDATE prj_settings SET PRJ_BEZIER_APPROX=%s, PRJ_BEZIER_WIDTH=%s, PRJ_BEZIER_NB_VERTEX=%d, ")
        _T("PRJ_BEZIER_METHOD=%d"),
        wxString::FromDouble(data->agg_approximation), wxString::FromDouble(data->ethz_width), data->ethz_max_points,
        (int)data->method);
    if (!DataBaseQueryNoResults(myQuery)) {
        return false;
    }
    return true;
}

bool DataBaseTM::LoadBezierSettings(BezierSettingsData* data) {
    wxString myQuery =
        _T(
            "SELECT PRJ_BEZIER_APPROX, PRJ_BEZIER_WIDTH, PRJ_BEZIER_NB_VERTEX, PRJ_BEZIER_METHOD FROM prj_settings");
    if (!DataBaseQuery(myQuery)) {
        return false;
    }

    DataBaseResult myResult;
    DataBaseGetResults(&myResult);
    if (myResult.GetColCount() != 4) {
        wxLogError(_("Error loading Bezier settings from project!"));
        return false;
    }

    if (data == nullptr) {
        return false;
    }
    double myValueDouble = 0;
    wxString myDataTxt;
    myResult.GetValue(0, myDataTxt);
    myDataTxt.ToDouble(&myValueDouble);
    data->agg_approximation = myValueDouble;

    myResult.GetValue(1, myDataTxt);
    myDataTxt.ToDouble(&myValueDouble);
    data->ethz_width = myValueDouble;

    long myValue = 0;
    myResult.GetValue(2, myValue);
    data->ethz_max_points = myValue;

    long myMethod = 0;
    myResult.GetValue(3, myMethod);
    data->method = BezierSettingsData::AGG;
    if (myMethod != 0) {
        data->method = BezierSettingsData::ETHZ;
    }
    return true;
}

bool DataBaseTM::ConvertPath(wxString& path) {
    wxArrayString myNewNameArray;
    wxFileName myDirName = wxFileName::DirName(path);

    // get the separator
    wxString mySeparator = myDirName.GetPathSeparator();

    // if we don't detect a windows separator
    if (mySeparator != _T("\\")) return false;

    myNewNameArray = wxStringTokenize(path, _T("\\"));

    // clear the original path
    path.Clear();

    for (unsigned int i = 0; i < myNewNameArray.GetCount(); i++) {
        path += myNewNameArray.Item(i);
        path += _T("\\\\");  // add two \ for windows
    }
    // remove last \ (2x)
    path.RemoveLast(2);
    return true;
}
