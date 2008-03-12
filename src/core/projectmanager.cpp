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


bool ProjectManager::OpenProject(const wxString & path)
{
	bool bReturn = FALSE;
	
	// close any existing project
	CloseProject();
	
	
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

