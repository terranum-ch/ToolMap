/***************************************************************************
								backup.cpp
                    Process for backup and restoring projects
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


#include "backup.h"

/********************************** BACKUPRESTORE CLASS **********************/

/***************************************************************************//**
 @brief Constructor
 @details Needs a valid #DataBaseTM pointer because path name will be retrieved from
 the database.
 @param pDB Valid pointer from #DataBaseTM object
 @author Lucien Schreiber (c) CREALP 2007
 @date 05 May 2008
 *******************************************************************************/
BackupRestore::BackupRestore(DataBaseTM * pDB)
{
	// init members
	m_pDatabase = pDB;
	m_DatabasePath = _T("");
	m_UserDefinedPath = _T("");
	
	
	GetDatabasePath();
}

/***************************************************************************//**
 @brief Destructor
 @details Do nothing for the moment
 @author Lucien Schreiber (c) CREALP 2007
 @date 05 May 2008
 *******************************************************************************/
BackupRestore::~BackupRestore()
{
	
}


/***************************************************************************//**
 @brief Return the Destination path
 @details Get the path from the Database and return the path if definied
 @return  a value of type #PATH_ERROR, PATH_OK if all is correct
 @author Lucien Schreiber (c) CREALP 2007
 @date 05 May 2008
 *******************************************************************************/
int BackupRestore::GetDatabasePath()
{
	// error msg
	wxString sErrMsg =	_("No path specified or path invalid \n"); 
	sErrMsg.Append(		_("for backups or restore operations,\n\n"));
	sErrMsg.Append(		_("Please go to Project->Edit Project->Settings...\n")); 
	sErrMsg.Append(		_("and specify a valid path."));
	
	// get the backup path from database
	int iPathStatus = m_pDatabase->GetProjectBackupPath(m_DatabasePath);
	if(iPathStatus == PATH_OK)
	{
		wxLogDebug(_T("Backup / restore path found : %s"),m_DatabasePath.c_str());
		
	}
	else 
	{
		wxMessageBox(sErrMsg,_("No valid path found"),wxICON_ERROR | wxOK);
	}
		
	return iPathStatus;
}


/***************************************************************************//**
 @brief Check if path are specified
 @details This function must be used to check if both (Database and user
 specified) path aren't null.
 @note No backups or restore operations could be
 done with empty paths.
 @return  TRUE if both paths aren't empty, FALSE otherwise
 @author Lucien Schreiber (c) CREALP 2007
 @date 05 May 2008
 *******************************************************************************/
bool BackupRestore::IsPathsSpecified()
{
	if (m_DatabasePath.IsEmpty() && m_DatabasePath.IsEmpty())
	{
		wxLogDebug(_T("Database path or user path are empty !"));
		return FALSE;
	}
	return TRUE;
}




/******************************************** BACKUP CLASS **********************/


/***************************************************************************//**
 @brief Constructor of the backup object
 @details This constructor init origin directory with values of the actual
 loaded database
 @param pDB pointer to an object of type #DataBaseTM
 @author Lucien Schreiber (c) CREALP 2007
 @date 06 May 2008
 *******************************************************************************/
Backup::Backup(DataBaseTM * pDB) : BackupRestore(pDB)
{
	// load the origin path name from the database
	m_UserDefinedPath = pDB->DataBaseGetPath() + 
						wxFileName::GetPathSeparator() + 
						pDB->DataBaseGetName();
	if (m_UserDefinedPath.Length() <= 1)
	{
		wxLogError(_T("Error, database name seems to be incorrect : %s"), 
				   m_UserDefinedPath.c_str());
		m_UserDefinedPath = _T("");
	}
	
}

