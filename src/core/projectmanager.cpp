/***************************************************************************
								projectmanager.h
			Class used for opening, closing, backuping the project
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


#include "projectmanager.h"
#include "toolmap.h"
#include "tmstats.h"
#include "../database/database_tm.h"        // for database and TM database operations
#include "../database/tmprojectmaintenance.h"
#include "../database/tmprojectmerge.h"
#include "backupmanager.h"


IMPLEMENT_CLASS(ProjectManager, wxObject);

/***************************************************************************//**
 @brief Constructor
 @author Lucien Schreiber (c) CREALP 2007
 @date 10 March 2008
 *******************************************************************************/
ProjectManager::ProjectManager(wxFrame *parent)
{
    bProjectIsOpen = FALSE;
    m_DB = NULL;
    m_Parent = parent;
    m_pMManager = NULL;
    m_ParentStatus = NULL;
    m_LayerManager = NULL;
    m_AttribManager = NULL;
    m_QueriesPanel = NULL;
    m_ShortcutPanel = NULL;
    m_SnappingPanel = NULL;
    m_EditManager = NULL;
    m_PrjMem = NULL;
    m_ToolManager = NULL;
    m_StatManager = NULL;

    m_Obj = new ObjectManager();
}


/***************************************************************************//**
 @brief Destructor
 @author Lucien Schreiber (c) CREALP 2007
 @date 10 March 2008
 *******************************************************************************/
ProjectManager::~ProjectManager()
{
    wxDELETE(m_PrjMem);
    wxDELETE(m_DB);

    // closing database only at the program end.
    /*if (m_DB != NULL)
    {
        delete m_DB;
        m_DB = NULL;
    }*/
}


/***************************************************************************//**
 @brief Create a new Toolmap project
 @details This function is called for creating a new embedded database and
 storing data from project definition
 @return  return TRUE if a new project was created succesfully
 @author Lucien Schreiber (c) CREALP 2007
 @date 10 March 2008
 *******************************************************************************/
bool ProjectManager::CreateNewProject()
{
    PrjDefMemManage PrjDefinition;
    //bool bReturn = FALSE;


    // first step, displaying the new project dialog
    ProjectDefNew *myNewDBDlg = new ProjectDefNew(m_Parent, &PrjDefinition);
    if (myNewDBDlg->ShowModal() != wxID_OK) {
        // this is needed, bug #26
        myNewDBDlg->m_DlgPd_Proj_Name->SetFocus();
        wxLogDebug(_T("New project creation cancelled"));
        delete myNewDBDlg;
        return FALSE;
    }
    delete myNewDBDlg;

    // close
    CloseProject();
    m_DB = new DataBaseTM();

    // Create new empty project
    {
        wxBusyInfo wait(_("Please wait, creating empty project..."), m_Parent);

        if (m_DB->CreateTMDatabase(&PrjDefinition) == false) {
            CloseProject();
            //delete wait;
            return false;
        }
    }

    // fill the project
    ProjectDefDLG *myNewProjDlg = new ProjectDefDLG(m_Parent, &PrjDefinition);
    if (myNewProjDlg->ShowModal() == wxID_OK) {

        if (m_DB->InitProjectWithStartingWizard(&PrjDefinition) == false) {
            delete myNewProjDlg;
            wxLogError(_T("Error passing data to database"));
            // change menu status
            //m_pMManager->SetStatus(MENU_DB_CLOSED);
            CloseProject();
            return false;
        }
    }
    delete myNewProjDlg;


    // add default queries
    if (PMAddDefaultQueries() == false)
        return false;



    // open the newly created project
    if (OpenProject(PrjDefinition.m_PrjPath +
                    wxFileName::GetPathSeparator() +
                    PrjDefinition.m_PrjName) != tmDB_OPEN_OK) {
        //m_pMManager->SetStatus(MENU_DB_CLOSED);
        CloseProject();
        return false;
    }


    return true;



    /*DatabaseNewPrj * myNewPrjDB = new DatabaseNewPrj();
    myNewPrjDB->SetPrjDefMemory(&PrjDefinition);

    if (!myNewPrjDB->CreateEmptyProject())
    {
        wxLogError(_("Database name and / or path are invalid"));
        delete myNewPrjDB;
        delete wait;
        return FALSE;
    }
    delete wait;

    // passing basic data (validity of the database)
    if (!myNewPrjDB->SetProjectData(&PrjDefinition))
    {
        wxLogError(_T("Error passing basic data to the database, database is unvalid !"));
        return FALSE;
    }


    // update menu status
    m_DB = myNewPrjDB;
    m_pMManager->AddFileToRecent(myNewPrjDB->DataBaseGetPath() +
                                 wxFileName::GetPathSeparator() +
                                 myNewPrjDB->DataBaseGetName());
    m_pMManager->SetStatus(MENU_DB_OPENED);
    m_pMManager->UpdateMenusStatus();


    // create TOC toolmap layers tables entry
    m_DB->InitTOCGenericLayers();


    // fill the project
    ProjectDefDLG * myNewProjDlg = new ProjectDefDLG(m_Parent, &PrjDefinition);
    if(myNewProjDlg->ShowModal() == wxID_OK)
    {

        if(!myNewPrjDB->PassProjectDataToDB())
        {
            delete myNewProjDlg;
            wxLogError(_T("Error passing data to database"));
            // change menu status
            m_pMManager->SetStatus(MENU_DB_CLOSED);
            CloseProject();
            return FALSE;
        }
    }
    delete myNewProjDlg;


    // open the newly created project
    if (OpenProject(PrjDefinition.m_PrjPath +
                    wxFileName::GetPathSeparator() +
                    PrjDefinition.m_PrjName) != OPEN_OK)
    {
        m_pMManager->SetStatus(MENU_DB_CLOSED);
        CloseProject();
        return FALSE;
    }


    return TRUE;*/
}


bool ProjectManager::PMAddDefaultQueries()
{
    wxASSERT(m_DB);

    wxString myQueryTemplate = _T("SELECT o.OBJECT_ID FROM %s AS o ")
            _T("WHERE o.OBJECT_ID NOT IN (SELECT a.OBJECT_GEOM_ID FROM %s AS a)");


    bool breturn = m_DB->EditQueries(TOC_NAME_LINES, _("Lines without attribution"),
                                     wxString::Format(myQueryTemplate,
                                                      TABLE_NAME_GIS_GENERIC[TOC_NAME_LINES].c_str(),
                                                      TABLE_NAME_GIS_ATTRIBUTION[TOC_NAME_LINES].c_str()));
    if (breturn == false) {
        wxLogError(_("Error adding default query [LINES]"));
        return false;
    }

    breturn = m_DB->EditQueries(TOC_NAME_POINTS, _("Points without attribution"),
                                wxString::Format(myQueryTemplate,
                                                 TABLE_NAME_GIS_GENERIC[TOC_NAME_POINTS].c_str(),
                                                 TABLE_NAME_GIS_ATTRIBUTION[TOC_NAME_POINTS].c_str()));
    if (breturn == false) {
        wxLogError(_("Error adding default query [POINTS]"));
        return false;
    }

    breturn = m_DB->EditQueries(TOC_NAME_LABELS, _("Labels without attribution"),
                                wxString::Format(myQueryTemplate,
                                                 TABLE_NAME_GIS_GENERIC[TOC_NAME_LABELS].c_str(),
                                                 TABLE_NAME_GIS_ATTRIBUTION[TOC_NAME_LABELS].c_str()));
    if (breturn == false) {
        wxLogError(_("Error adding default query [LABELS]"));
        return false;
    }
    return true;
}



bool ProjectManager::_copy_directory(wxString from, wxString to){
    wxString SLASH = wxFILE_SEP_PATH;

    // append a slash if there is not one (for easier parsing)
    // because who knows what people will pass to the function.
    if (to[to.length()-1] != SLASH) {
        to += SLASH;
    }
    // for both dirs
    if (from[from.length()-1] != SLASH) {
        from += SLASH;
    }

    // first make sure that the source dir exists
    if(!wxDir::Exists(from)) {
        wxLogError(from + " does not exist.  Can not copy directory.");
    }
    else {
        // check on the destination dir
        // if it doesn't exist...
        if(!wxDir::Exists(to)) {
            // if it doesn't get created
            if(!wxFileName::Mkdir(to, 0777, wxPATH_MKDIR_FULL)) {
                // Send an error
                wxLogError(to + " could not be created.");
                // And exit gracefully
                return false;
            }
        }

        // The directories to traverse
        wxArrayString myDirs;
        myDirs.Add("");

        // loop through each directory.. storing all sub directories
        // and copying over all files.. the final iteration of one loop
        // should begin an iteration for any subdirectories discovered
        // on the previous pass
        // (rather than pragma, unsigned int will shut the MS compiler up)
        for (unsigned int i = 0; i < myDirs.size(); i++) {

            // get the next directory
            wxDir nextDir(from + myDirs[i]);

            // check that it exists in destination form
            if(!wxDir::Exists(to + myDirs[i])) {
                // if it doesn't, then create it
                if(!wxFileName::Mkdir(to + myDirs[i], 0777, wxPATH_MKDIR_FULL)) {
                    // If it doesn't create, error
                    wxLogError(to + myDirs[i] + " could not be created.");
                    // And exit gracefully
                    return false;
                }
            }

            // get the first file in the next directory
            wxString nextFile;
            bool process = nextDir.GetFirst(&nextFile);

            // and while there are still files to process
            while (process) {

                // If this file is a directory
                if(wxDir::Exists(from+nextFile)) {
                    // then append it for creation/copying
                    myDirs.Add(nextFile + SLASH);   // only add the difference
                }
                else {

                    // otherwise just go ahead and copy the file over
                    if(!wxCopyFile(from + myDirs[i] + nextFile,
                                   to   + myDirs[i] + nextFile)) {
                        // error if we couldn't
                        wxLogError("Could not copy " +
                                   from + myDirs[i] + nextFile + " to "
                                   + to + myDirs[i] + nextFile);
                    }
                }
                // and get the next file
                process = nextDir.GetNext(&nextFile);
            }
        }
        return true;
    }
    return false;
}


/***************************************************************************//**
 @brief Edit the project (layers,...)
 @details This function may be called for editing the project (layers,
 fields,...).
 @note do not call this function if no DB is opened.
 @return  TRUE if all went ok during project editing
 @author Lucien Schreiber (c) CREALP 2007
 @date 10 April 2008
 *******************************************************************************/
bool ProjectManager::EditProject(int notebooknumber)
{
    wxASSERT (m_PrjMem);
    bool bReturn = true;

    // copy project definition
    PrjDefMemManage myPrjDef;
    myPrjDef = *m_PrjMem;

    ProjectDefDLG *myNewProjDlg = new ProjectDefDLG(m_Parent, &myPrjDef, TRUE);
    myNewProjDlg->SetNotebook(notebooknumber);
    if (myNewProjDlg->ShowModal() == wxID_OK) {
        // modify data
        if (m_DB->UpdateDataBaseProject(&myPrjDef)) {
            wxLogDebug(_T("Database modified"));
            LoadProjectDefintion(1);
        } else {
            wxLogError(_("Project definition update error"));
            bReturn = false;
            // reload actual project
            LoadProjectDefintion(2);
        }
    }
    wxDELETE(myNewProjDlg);

    wxASSERT(m_PrjMem);
    m_AttribManager->InitAttributionManager(m_DB, m_PrjMem);
    m_LayerManager->SetMemoryProject(m_PrjMem);
    return bReturn;
}



bool ProjectManager::BackupProject (const wxString & backup_comment) {
    // backup path exists ?
    wxString myBackupPath = wxEmptyString;
    if (GetDatabase()->GetProjectBackupPath(myBackupPath) != PATH_OK) {
        wxString sErrMsg = _("No path specified or path invalid \n");
        sErrMsg.Append(_("for backups or restore operations,\n\n"));
        sErrMsg.Append(_("Please go to Project->Edit Project->Settings...\n"));
        sErrMsg.Append(_("and specify a valid path."));
        wxMessageBox(sErrMsg, _("No valid path found"), wxICON_ERROR | wxOK);
        return false;
    }

    // create backup file
    BackupFile myBckFile;
    myBckFile.SetInputDirectory(wxFileName(GetDatabase()->DataBaseGetPath(),
                                           GetDatabase()->DataBaseGetName()));
    myBckFile.SetDate(wxDateTime::Now());
    myBckFile.SetOutputName(wxFileName(myBackupPath,
                                       GetDatabase()->DataBaseGetName(),
                                       "tmbk"));

    // ask for comment if backup_comment not empty.
    if (backup_comment == wxEmptyString) {
        wxTextEntryDialog myDlg(m_Parent, _("Backup comment:"), _("Backup"), wxEmptyString, wxOK | wxCENTRE);
        if (myDlg.ShowModal() == wxID_OK) {
            myBckFile.SetComment(myDlg.GetValue());
        }
    }
    else{
        myBckFile.SetComment(backup_comment);
    }

    wxBeginBusyCursor();
    wxLogMessage("filename for backup will be : " + myBckFile.GetOutputName().GetFullPath());
    BackupManager myBckManager(GetDatabase());

    // Don't display progress dialog under Mac... Toooo slow!
    wxWindow *myWnd = NULL;
#ifndef __WXMAC__
    myWnd = m_Parent;
#endif

    if (myBckManager.Backup(myBckFile, myWnd) == false) {
        wxLogError(_("Backup : '%s' Failed !"), myBckFile.GetOutputName().GetFullName());
        wxEndBusyCursor();
        return false;
    }
    wxEndBusyCursor();
    return true;
}


bool ProjectManager::MergeProjects(const wxString &slave_project_name, bool beVerbose) {
    wxASSERT(m_DB);
    wxFileName myMasterProjectFileName(GetDatabase()->DataBaseGetPath(), GetDatabase()->DataBaseGetName());

    //TODO: copy project into same directory if needed

    wxStopWatch sw;
    sw.Start(0);
    bool bCheckOk = false;
    tmProjectMerge myCheckMerger(myMasterProjectFileName.GetFullPath(), slave_project_name, GetDatabase());
    myCheckMerger.SetVerbose(beVerbose);
    // checking here
    if (beVerbose) {
        wxLogMessage(_("CHECKING..."));
    }
    if(myCheckMerger.CheckSimilar()==false) {
        wxString myErrors = _("Checking FAILED! see bellow\n") + wxJoin(myCheckMerger.GetErrors(), '\n');
        wxLogError(myErrors);
        return false;
    }

    wxLogMessage(_("OK projects are similar"));

    if (beVerbose) {
        wxLogMessage(_("Checking projects in %ld [ms]"), sw.Time());
    }

    sw.Start(0);
    // merging here
    if (beVerbose) {
        wxLogMessage(_("MERGING..."));
    }

    if(myCheckMerger.MergeIntoMaster()==false) {
        wxString myErrors = _("Merge FAILED! see bellow\n") + wxJoin(myCheckMerger.GetErrors(), '\n');
        wxLogError(myErrors);
        return false;
    }

    wxLogMessage(_("OK Project Merged into '%s' in %ld [ms]"), myMasterProjectFileName.GetFullPath(), sw.Time());
    return true;
}


/***************************************************************************//**
 @brief Close the active database
 @details This function may be called as often as you want, check is made and
 database is closed only if previously opened.
 @note All internal functions should work event if database is empty
 @author Lucien Schreiber (c) CREALP 2007
 @date 11 March 2008
 *******************************************************************************/
void ProjectManager::CloseProject()
{
    wxASSERT (m_EditManager);

    // save the snapping informations
    m_SnappingPanel->SetDataBase(NULL);
    //m_SnappingPanel->SaveSnappingStatus();

    m_StatManager->Create(NULL);

    m_EditManager->SetDatabase(NULL);
    m_LayerManager->UnInitLayerManager();
    m_AttribManager->UnInitAttributionManager();

    wxDELETE(m_DB);
    bProjectIsOpen = false;
}


/***************************************************************************//**
 @brief Check if the path is a Toolmap path
 @details This function checks the existence of the #TABLE_NAME_PRJ_SETTINGS
 .MYD file.
 Upper and lower case are tested
 @param path The path we want to test
 @return  TRUE if the file exist in the path
 @author Lucien Schreiber (c) CREALP 2008
 @date 17 June 2008
 *******************************************************************************/
bool ProjectManager::IsDataBasePath(const wxString &path)
{
    // upper case
    if (!wxDir::FindFirst(path, TABLE_NAME_PRJ_SETTINGS + _T(".MYD"), wxDIR_FILES).IsEmpty())
        return TRUE;

    // lower case
    if (!wxDir::FindFirst(path, TABLE_NAME_PRJ_SETTINGS.Lower() + _T(".MYD"), wxDIR_FILES).IsEmpty())
        return TRUE;

    wxLogDebug(_T("Not a ToolMap project, file : %s not found in %s"),
               (TABLE_NAME_PRJ_SETTINGS + _T(".MYD")).c_str(),
               path.c_str());
    return FALSE;
}


/***************************************************************************//**
 @brief Open ToolMap project
 @details Try to open a toolmap project
 @param path path to the project
 @return  one of the #tmPROJECT_OPEN_ERROR. If database version isn't correct,
 return the not correct number as error code
 @author Lucien Schreiber (c) CREALP 2008
 @date 21 October 2008
 *******************************************************************************/
int ProjectManager::OpenProject(const wxString &path)
{
    // close any existing project
    CloseProject();

    // ensure path exists
    if (wxFileName::DirExists(path) == false) {
        wxLogMessage(_("Project '%s' doesn't exist."), path.c_str());
        return tmDB_OPEN_ERR_NOT_FOUND;
    }

    m_DB = new DataBaseTM();
    int mystatus = m_DB->OpenTMDatabase(path);

    if (mystatus != tmDB_OPEN_OK) {
        wxFileName myDirName(path, _T(""));
        wxArrayString myNames = myDirName.GetDirs();
        wxASSERT(m_DB);
        tmOpenError_DLG myDlg(m_Parent, mystatus, myNames.Item(myNames.GetCount() - 1), m_DB);
        if (myDlg.ShowModal() == wxID_CANCEL) {
            CloseProject();
            return mystatus;
        }
        return OpenProject(path);
    }

    // all is OK
    // updates the menu using the menu manager
    ///m_pMManager->SetStatus(MENU_DB_OPENED);
    m_pMManager->AddFileToRecent(path);

    // update objects to lists
    m_Obj->UpdateObjectLists(m_DB);

    // activate project opening
    m_QueriesPanel->LoadQueries(m_DB);

    // attribution manager
    LoadProjectDefintion(1);
    m_AttribManager->InitAttributionManager(m_DB, GetMemoryProjectDefinition());

    // load shortcuts
    m_ShortcutPanel->SetDataBase(m_DB);
    // load shortcuts for lines by default
    m_ShortcutPanel->LoadShortcutList(true);
    m_ShortcutPanel->SetProjectOpen(true);

    // load snapping
    m_SnappingPanel->SetDataBase(m_DB);
    m_SnappingPanel->LoadSnappingStatus();

    // LayerManager Job
    m_LayerManager->SetMemoryProject(GetMemoryProjectDefinition());
    m_LayerManager->InitLayerManager(m_DB);

    // edition manager
    m_EditManager->SetDatabase(m_DB);

    m_ToolManager->SetDatabase(m_DB);

    m_StatManager->Create(m_DB);

    // load bezier approximation
    // TODO: Load bezier settings from database
    //m_EditManager->SetBezierApproximationScale(m_DB->LoadBezierApproximationScale());


    // project is now open !
    bProjectIsOpen = TRUE;
    //myReturnVal = OPEN_OK;

    wxString myTitleBarText = g_ProgName + _T(" - ") + GetProjectName();
    m_Parent->SetTitle(myTitleBarText);

    // optimize project
    wxString myDatabaseSizeBefore = m_DB->DataBaseGetSize();
    tmProjectMaintenance myPrjMaintenance(wxEmptyString, m_DB);
    /*if (myPrjMaintenance.OptimizeTables() == false) {
        wxLogWarning(_("Project optimization failed!"));
    }*/

    if (myPrjMaintenance.ClearOrphans() == false) {
        wxArrayString myErrors = myPrjMaintenance.GetErrors();
        wxLogWarning(_("Cleaning orphans failed!"));
        for (unsigned int i = 0; i < myErrors.GetCount(); i++) {
            wxLogWarning(myErrors[i]);
        }
    } else {
        wxArrayString myInfos = myPrjMaintenance.GetMessages();
        for (unsigned int i = 0; i < myInfos.GetCount(); i++) {
            wxLogDebug(myInfos[i]);
        }
    }

    wxString myDatabaseSizeActual = m_DB->DataBaseGetSize();
    if (myDatabaseSizeBefore != myDatabaseSizeActual) {
        wxLogMessage(_("Project size optimized from '%s' to '%s'"), myDatabaseSizeBefore, myDatabaseSizeActual);
    }
    return (int) mystatus;
}


bool ProjectManager::EditProjectObjectDefinition()
{
    bool bReturn = false;

    // display the dialogs for editing the project's object definition.
    // DO NOT CALL THIS BEFORE OPENING A DB
    ProjectEditObjectDefinitionDLG *myDlg = new ProjectEditObjectDefinitionDLG(m_Parent, m_DB);
    if (myDlg->ShowModal() == wxID_SAVE) {
        wxLogDebug(_T("Object definition saved into the DB"));

        // update objects to lists
        m_Obj->UpdateObjectLists(m_DB);
        LoadProjectDefintion(1);
        bReturn = true;
    }

    wxDELETE(myDlg);
    return bReturn;
}


bool ProjectManager::EditObjectFrequency(int id)
{
    // Get object from the DB
    wxString sSelect = wxString::Format(_T("SELECT OBJECT_ID, OBJECT_ISFREQ FROM %s ")
                                        _T(" WHERE OBJECT_ID = %d"), TABLE_NAME_OBJECTS.c_str(), id);

    if (!m_DB->DataBaseQuery(sSelect))
        return false;

    if (!m_DB->DataBaseHasResults())
        return false;

    wxArrayString rowResults;
    if (!m_DB->DataBaseGetNextResult(rowResults)) {
        m_DB->DataBaseClearResults();
        return false;
    }

    long lFreq = 0;
    rowResults.Item(1).ToLong(&lFreq);

    // Toggle frequent / less frequent
    if (lFreq == 1) {
        lFreq = 0;
    } else {
        lFreq = 1;
    }

    m_DB->DataBaseClearResults();

    // Update the DB
    wxString sUpdate = wxString::Format(_T("UPDATE %s ")
                                        _T("SET OBJECT_ISFREQ = %ld ")
                                        _T("WHERE OBJECT_ID = %d"), TABLE_NAME_OBJECTS.c_str(), lFreq, id);

    wxLogDebug(sUpdate);
    if (!m_DB->DataBaseQueryNoResults(sUpdate)) {
        wxLogDebug(_T("The change as frequent / less frequent failed."));
        return false;
    }

    // Update objects to lists
    m_Obj->UpdateObjectLists(m_DB);
    LoadProjectDefintion(1);

    return true;
}

bool ProjectManager::EditProjectSettings()
{
    bool bReturn = FALSE;

    // display the dialogs for editing the project's object definition.
    // DO NOT CALL THIS BEFORE OPENING A DB
    ProjectPropertiesDLG myDLG(m_Parent, m_DB);
    if (myDLG.ShowModal() == wxID_SAVE) {
        m_LayerManager->InitScaleCtrlList();
        wxLogDebug(_T("Changes applied into database"));
        bReturn = TRUE;
    }
    //BUG: bug #23 Workaround for mac
    myDLG.SetScaleListFocus();

    //delete myDLG;
    return bReturn;
}

/***************************************************************************//**
 @brief Get the actual project name
 @return  Return a wxString containing the actual project name. This name isn't
 stored internally but we asked the DataBaseTM class
 @author Lucien Schreiber (c) CREALP 2007
 @date 11 March 2008
 *******************************************************************************/
wxString ProjectManager::GetProjectName()
{
    if (m_DB != NULL)
        return m_DB->DataBaseGetName();
    else {
        wxLogDebug(_T("Database pointer is null"));
    }

    return wxEmptyString;
}


/***************************************************************************//**
 @brief Load project defintion from database to memory
 @details Project defintion is used for :
 - Editing project
 - Advanced
 attribution
 @param message Message displayed during project loading :
 - 0 = No Message
 - 1= Loading project in progress
 - 2 = Cancelling in progress
 @return  true if loading project definition succeed false and an assert in
 debug mode
 @author Lucien Schreiber (c) CREALP 2009
 @date 12 March 2009
 *******************************************************************************/
bool ProjectManager::LoadProjectDefintion(short int message)
{
    wxASSERT (m_DB);
    wxASSERT (message >= 0 && message <= 2);
    wxASSERT (m_Parent);

    if (m_PrjMem) {
        wxDELETE(m_PrjMem);
        //m_PrjMem = new PrjDefMemManage();
    }

    wxStopWatch sw;
    wxString myWaitString = wxEmptyString;
    switch (message) {
        case 1:
            myWaitString = _("Please wait, loading project...");
            break;
        case 2:
            myWaitString = _("Please wait, cancelling in progress...");
            break;

        default:
            break;
    }
    wxBusyInfo *wait = NULL;
    if (!myWaitString.IsEmpty())
        wait = new wxBusyInfo(myWaitString, m_Parent);

    // load data from DB --> PrjDefMemManage
    m_PrjMem = m_DB->GetProjectDataFromDB();

    if (wait)
        delete wait;

    wxLogMessage(_T("Project Data loaded in : %ld [ms]"), sw.Time());

    wxASSERT (m_PrjMem);
    if (m_PrjMem == NULL)
        return false;

    m_LayerManager->SetMemoryProject(GetMemoryProjectDefinition());


    return true;
}


/***************************************************************************//**
 @brief Get the project definition
 @return  a valid PrjDefMemManage object (if project isn't initialized an
 wxASSERT is issued in debug mode
 @author Lucien Schreiber (c) CREALP 2009
 @date 16 March 2009
 *******************************************************************************/
PrjDefMemManage *ProjectManager::GetMemoryProjectDefinition()
{
    wxASSERT(m_PrjMem);
    return m_PrjMem;
}













/***************************** OBJECT MANAGER **********************************/

/***************************************************************************//**
 @brief Init values
 @author Lucien Schreiber (c) CREALP 2008
 @date 16 May 2008
 *******************************************************************************/
void ObjectManager::InitValues()
{
    m_panel = NULL;
}


/***************************************************************************//**
 @brief Update the point, the poly and the line list
 @author Lucien Schreiber (c) CREALP 2008
 @date 16 May 2008
 *******************************************************************************/
bool ObjectManager::UpdateObjectLists(DataBaseTM *pDB)
{
    if (!m_panel->UpdateObjectPointList(pDB)) {
        wxLogDebug(_T("Unable to update the Point list"));
        return FALSE;
    }

    if (!m_panel->UpdateObjectPolyList(pDB)) {
        wxLogDebug(_T("Unable to update the polygon list"));
        return FALSE;
    }

    if (!m_panel->UpdateObjectLineList(pDB)) {
        wxLogDebug(_T("Unable to update the Line list"));
        return FALSE;
    }


    return TRUE;
}



