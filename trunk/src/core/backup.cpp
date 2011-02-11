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

wxDEFINE_SCOPED_PTR(wxZipEntry, wxZipEntryPtr);


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
	
	m_DBName = m_pDatabase->DataBaseGetName();
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
	if(iPathStatus != PATH_OK)
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


/***************************************************************************//**
 @brief List all compatible files
 @details This function lists all files the program must backuped. Only files
 with following extensions are stored into the files parameter :
 - .opt
 - .frm
 - .myd
 - .myi
 @param sDir the dir where the files are located
 @param files an array filled with files name to backup
 @return  the number of files found in the folder (also files not
 backuped) in the array
 @author Lucien Schreiber (c) CREALP 2007
 @date 06 May 2008
 *******************************************************************************/
long BackupRestore::ListDirFiles (const wxString & sDir, wxArrayString & files)
{
	wxDir listdir (sDir);
	long iCountFiles = 0;
	
	// file filter for only those filetypes
	wxRegEx FileSpec(wxT(".opt|.frm|.MYD|.MYI"));
	
	if (!listdir.IsOpened())
	{
		return -1;
	}
	
	wxString filename;
	
	// iterates all files in directory
	bool cont = listdir.GetFirst(&filename, wxEmptyString, wxDIR_FILES);
	while (cont)
	{
		if (FileSpec.Matches(filename))
		{
			files.Add(filename);
		}
		else 
		{
			wxLogError(_T("File %s will not be backuped"), filename.c_str());
		}
		
		iCountFiles ++;
		cont = listdir.GetNext(&filename);
	}
	
	wxLogDebug(_T("Number of files for backup : %ld"),iCountFiles);
	return iCountFiles;
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


/***************************************************************************//**
 @brief Construct a backup file name
 @details This function may be called for creating a backup file name of the
 following type : databasename-2008-05-30-13:12:54.zip
 @return  the new backup filename + path if all goes OK, otherwise empty string
 is returned
 @author Lucien Schreiber (c) CREALP 2007
 @date 06 May 2008
 *******************************************************************************/
wxString Backup::CreateBackupName ()
{
	wxString dbname = GetDatabaseName();
	if (dbname.IsEmpty())
	{
		wxLogError(_T("Database name is empty ! "));
		return wxEmptyString;
	}
	
	wxDateTime dt = wxDateTime::Now();
	wxString filename = dbname;
	filename.Append( _T("-") + GetDateAsString() + dt.Format(_T("-%H%M%S.zip")));
					
					// dt.FormatISODate() + _T("-") + dt.FormatISOTime() + _T(".zip"));	
	
	wxFileName bkpname (GetDirDestination(),filename);
	if (!bkpname.IsOk())
	{
		wxLogDebug(_T("Backup file name seems not to be correct : ") + bkpname.GetFullPath());
		return wxEmptyString;
	}
	
	return bkpname.GetFullPath();
}


/***************************************************************************//**
 @brief Return actual date as string
 @return the actual date as string (YYYYMMDD)
 @author Lucien Schreiber (c) CREALP 2007
 @date 07 May 2008
 *******************************************************************************/
wxString Backup::GetDateAsString()
{
	wxDateTime dt = wxDateTime::Now();
	return dt.Format(_T("%Y%m%d"));
}



/***************************************************************************//**
 @brief Save files to Zip
 @details This functions save the passed files array to zip file
 To avoid
 errors, one can call BackupRestore::ListDirFiles() before.
 @code
 Backup myBackup (m_DB);
 wxArrayString myFiles;
 
 if(myBackup.IsPathsSpecified())
 {
	myBackup.ListDirFiles(myBackup.GetDirOrigin(), myFiles);
	myBackup.Save(myFiles);
 
	return TRUE;
 }
 @endcode
 @param files String array containing ONLY files name, Directory can be
 retrieved with Backup::GetDirDestination() or Backup::GetDirOrigin()
 @return  True if backup was successfull, false otherwise (see error message)
 @author Lucien Schreiber (c) CREALP 2007
 @date 07 May 2008
 *******************************************************************************/
bool Backup::Save (const wxArrayString & files)
{
	// get and check directory
	wxString fromdir	= GetDirOrigin();
	wxString todir		= GetDirDestination();
	
	if (!wxDirExists(fromdir))
	{
		wxLogError(_T("Backup directory dosen't exist : ") + fromdir);
		return FALSE;
	}
	
	if (!wxDirExists(todir))
	{
		wxLogError(_T("Backup directory dosen't exist : ") + todir);
		return FALSE;		
	}
	
	// get and check future backup filename
	wxString dbbackupname = CreateBackupName();
	
	if (wxFileExists(dbbackupname))
	{
		wxLogError(_("Backup with same name allready exists : ") + dbbackupname);
		return FALSE;
	}

	
	// open stream to file
	wxFFileOutputStream outf(dbbackupname);
	
	if (!outf.Ok())
	{
		wxLogError(wxT("Could not open file: ") + dbbackupname);
		return false;
	}
	wxZipOutputStream outzip(outf);
	

	
	// the progress dialog
	wxFileName dbkfilename (dbbackupname);
	wxString mydlgtext =	_("Backup of the database : ") + GetDatabaseName() +
							_(" In progress.");
	wxProgressDialog ProgDlg (_("Backup in progress"),mydlgtext,50,NULL,
							  wxPD_CAN_ABORT |  wxPD_AUTO_HIDE | wxPD_APP_MODAL);
	
	// compute progress increment
	double dStep = files.GetCount();
	if (dStep > 50)
		dStep = dStep / 50;
	else
		dStep = 50 / dStep;
	double dIncrement = dStep;
	
	
	bool bCompleted = TRUE;
	
	// adding directory into archive
	wxString dirname = GetDatabaseName() + GetDateAsString();
	outzip.PutNextDirEntry(dirname);
	
	// loop for adding all files
	for (unsigned int i = 0; i<files.GetCount(); i++)
	{
		wxFileName fn1 (GetDirOrigin(), files.Item(i));
		wxFileInputStream f1stream(fn1.GetFullPath());
		
		if (!f1stream.Ok())
		{
			wxLogError(wxT("Error opening file: ") + files.Item(i));
			return false;
		}
		
		// incrementing progress dialog
		if (!ProgDlg.Update(dIncrement))
		{
			bCompleted = FALSE;
			wxLogMessage(_("Backup into %s cancelled by user"), dbkfilename.GetFullName().c_str());
			break;
		}
		dIncrement  = dIncrement + dStep;
		if (dIncrement > 50)
			dIncrement = 50;
		
		
		// realy adding files into zip
		outzip.PutNextEntry(dirname + wxFileName::GetPathSeparator() + files.Item(i));
		outzip << f1stream;
		
	}
	
	if (!outzip.Close())
	{
		wxLogError(wxT("Error during outzip.Close()"));
		return false;
	}
	
	outf.Close();
	
	
	if (bCompleted)
		wxLogMessage(_("Database backuped into : ") + dbkfilename.GetFullName());
	
	//wxLogDebug(_T("Backup DONE is : ")  + dbbackupname);
	return TRUE;
}



