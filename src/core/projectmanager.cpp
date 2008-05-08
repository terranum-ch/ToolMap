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
IMPLEMENT_CLASS(ProjectManager, wxObject);

/***************************************************************************//**
 @brief Constructor 
 @author Lucien Schreiber (c) CREALP 2007
 @date 10 March 2008
 *******************************************************************************/
ProjectManager::ProjectManager(wxWindow * parent)
{
	bProjectIsOpen = FALSE;
	m_DB = NULL;
	m_Parent = parent;
}


/***************************************************************************//**
 @brief Destructor 
 @author Lucien Schreiber (c) CREALP 2007
 @date 10 March 2008
 *******************************************************************************/
ProjectManager::~ProjectManager()
{
	// closing database only at the program end.
	if (m_DB != NULL)
	{
			delete m_DB;
	}
	
	// closing the library
	DataBase::DataBaseCloseLibrary();
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
	bool bReturn = FALSE;
	
	ProjectDefDLG * myNewProjDlg = new ProjectDefDLG(m_Parent, &PrjDefinition);
	if(myNewProjDlg->ShowModal() == wxID_OK)
	{
		wxBusyCursor wait;

		// Database new project creation
		DatabaseNewPrj * myNewPrjDB = new DatabaseNewPrj();
		
		myNewPrjDB->SetPrjDefMemory(&PrjDefinition);
		
		// create the project on disk
		if (myNewPrjDB->CreateEmptyProject())
		{
			if(myNewPrjDB->PassProjectDataToDB())
			{
				m_DB = myNewPrjDB;
				bReturn = TRUE;
			}
			
			// Error message if something goes wrong
			// we try to close the database
			if (bReturn == FALSE)
			{
				wxLogError(_("Error creating the new project"));
				CloseProject();
			}
			
		}
		delete myNewProjDlg;
		return TRUE;
	}
	return bReturn;
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
bool ProjectManager::EditProject ()
{
	wxLogDebug(_T("Editing the project"));
	
	PrjDefMemManage * pPrjDefinition = NULL;
	// load data from DB --> PrjDefMemManage
	pPrjDefinition = m_DB->GetProjectDataFromDB();
	if (pPrjDefinition != NULL)
	{
		
		ProjectDefDLG * myNewProjDlg = new ProjectDefDLG(m_Parent, pPrjDefinition, TRUE);
		if(myNewProjDlg->ShowModal() == wxID_OK)
		{
			
			// modify data 
			m_DB->UpdateDataBaseProject(pPrjDefinition);
			
		}
		
		delete myNewProjDlg;
		delete pPrjDefinition;
	}
	
	
	return FALSE;
}


/***************************************************************************//**
 @brief Backup the project
 @return  TRUE if the backup was succes
 @author Lucien Schreiber (c) CREALP 2007
 @date 05 May 2008
 *******************************************************************************/
bool ProjectManager::BackupProject ()
{
	Backup myBackup (m_DB);
	wxArrayString myFiles;
	long lTotfiles = 0;
	wxString dlgmsg = _("Some files weren't backuped. More informations\n");
	dlgmsg += _("may be found in the log window.\n\n");
	dlgmsg += _("To show the log window : Window -> Log Window...");
	
	if(myBackup.IsPathsSpecified())
	{
		
		lTotfiles = myBackup.ListDirFiles(myBackup.GetDirOrigin(), myFiles);
		myBackup.Save(myFiles);
		
		// show message if some files aren't backuped
		if (lTotfiles > (signed) myFiles.Count())
		{
			wxMessageBox(dlgmsg, _("Some files not backuped"), 
						 wxICON_INFORMATION | wxOK);
		}
		return TRUE;
	}
	return FALSE;
}



/***************************************************************************//**
 @brief Close the active database
 @details This function may be called as often as you want, check is made and
 database is closed only if previously opened.
 @author Lucien Schreiber (c) CREALP 2007
 @date 11 March 2008
 *******************************************************************************/
void ProjectManager::CloseProject()
{
	if (m_DB != NULL)
	{
		wxLogDebug(_T("Closing the database object"));
		delete m_DB;
		m_DB = NULL;
	}
}



bool ProjectManager::IsDataBasePath(const wxString & path)
{
	if (!wxDir::FindFirst(path, TABLE_NAME_PRJ_SETTINGS + _T(".MYD"), wxDIR_FILES).IsEmpty())
		return TRUE;
	
	return FALSE;
}


bool ProjectManager::OpenProject(const wxString & path)
{
	bool bReturn = FALSE;
	
	// close any existing project
	CloseProject();
	
	// 0 check if the folder contain something like a database file
	if (IsDataBasePath(path))
	{
		
		
		
		// 1 check, is connection with library ok on specified path 
		m_DB = new DataBaseTM();
		if (m_DB->DataBaseOpen(path,LANG_UTF8))
		{
		// 2. check, contain an embedded database ?
		// we check if a table exists
		if (m_DB->DataBaseTableExist(TABLE_NAME_PRJ_SETTINGS))
		{
		// 3. check, for version number
		if (m_DB->GetDatabaseToolMapVersion() >= TM_DATABASE_VERSION)
		{
		// project is now open !
		bReturn = TRUE;
		}
		}
		
		
		// check the tables for cbReturn = TRUE;
		}
		else
		{
		CloseProject();
		}
	}
		
	//bReturn = TRUE;
	
	
	
	return bReturn;
}





bool ProjectManager::EditProjectObjectDefinition ()
{
	bool bReturn = FALSE;
	
	// display the dialogs for editing the project's object definition.
	// DO NOT CALL THIS BEFORE OPENING A DB
	ProjectEditObjectDefinitionDLG * myDlg = new ProjectEditObjectDefinitionDLG(m_Parent, m_DB);
	if (myDlg->ShowModal() == wxID_SAVE)
	{
		wxLogDebug(_T("Object definition saved into the DB"));
		bReturn = TRUE;
	}
	
	delete myDlg;
	return bReturn;
}



bool ProjectManager::EditProjectSettings ()
{
	bool bReturn = FALSE;
	
	// display the dialogs for editing the project's object definition.
	// DO NOT CALL THIS BEFORE OPENING A DB
	ProjectPropertiesDLG * myDLG = new ProjectPropertiesDLG(m_Parent, m_DB);
	if(myDLG->ShowModal() == wxID_SAVE)
	{
		wxLogDebug(_T("Changes applied into database"));
		bReturn = TRUE;
	}
	delete myDLG;
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
	return wxEmptyString;
}

