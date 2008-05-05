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
	// get the backup path from database
	int iPathStatus = m_pDatabase->GetProjectBackupPath(m_DatabasePath);
	if(iPathStatus == PATH_OK)
	{
		wxLogDebug(_T("Backup / restore path found : %s"),m_DatabasePath.c_str());
		
	}
	else 
	{
		wxMessageBox(_("No path specified or path invalid \n"
					   "for backups or restore operations,\n\n"
					   "Please go to Project->Edit Project->Settings...\n"
					   "and specify a valid path."),
					 _("No valid path found"), 
					 wxICON_ERROR | wxOK);
	}
	
	
	return iPathStatus;
}


//bool IsPathsSpecified();




/******************************************** BACKUP CLASS **********************/



Backup::Backup(DataBaseTM * pDB) : BackupRestore(pDB)
{

}

