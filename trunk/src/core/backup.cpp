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
 @return  the number of files in the array
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
			iCountFiles ++;
		}
		else 
		{
			wxLogDebug(_T("Files %s not matching file specs..."), filename.c_str());
		}

		cont = listdir.GetNext(&filename);
	}
	
	wxLogDebug(_T("Number of files for backup : %d"),iCountFiles);
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


bool Backup::TestZip(wxString filename1, wxString filename2)
{
	// file verifications
	if (!wxFileExists(filename1))
	{
		wxLogError(wxT("File does not exist: ") + filename1);
		return false;
	}
	
	if (!wxFileExists(filename2))
	{
		wxLogError(wxT("File does not exist: ") + filename2);
		return false;
	}
	
	//wxFFileInputStream inf(filename2);
	wxFFileOutputStream outf(filename2);
	
	if (!outf.Ok())
	{
		wxLogError(wxT("Could not open file: ") + filename2);
		return false;
	}
	
	wxZipOutputStream outzip(outf);
	
	
	/*wxString tempfilename = wxFileName::CreateTempFileName(_T("rls"));
	wxFFileOutputStream outf(tempfilename);
	
	if (!outf.Ok())
	{
		wxLogError(wxT("Could not open file: ") + tempfilename);
		return false;
	}
	
	wxZipInputStream inzip(inf);
	wxZipOutputStream outzip(outf);
	wxZipEntryPtr entry;
	
	outzip.CopyArchiveMetaData(inzip);
	
	
	
	while (entry.reset(inzip.GetNextEntry()), entry.get() != NULL)
	{
		if (fn1.GetFullName() == entry->GetName())
		{
			// delete any existing file with the same name
			// (otherwise there would be two entries with the same name)
			continue;
		}
		if (!outzip.CopyEntry(entry.release(), inzip))
		{
			break;
		}
	}
	
	if (!inzip.Eof())
	{
		wxLogError(wxT("Error during zip copy"));
		return false;
	}*/
	
	// here actually add filename1
	
	wxFileInputStream f1stream(filename1);
	wxFileName fn1(filename1);
	
	if (!f1stream.Ok())
	{
		wxLogError(wxT("Error opening file: ") + filename1);
		return false;
	}
	
	
	outzip.PutNextEntry(fn1.GetFullName());
	
	//outzip << f1stream;
	outzip.Write(f1stream);
	
	if (!outzip.Close())
	{
		wxLogError(wxT("Error during outzip.Close()"));
		return false;
	}
	
	outf.Close();
	
	/*if (!wxCopyFile(tempfilename, filename2))
	{
		wxLogError(wxT("Error during wxCopyFile"));
		return false;
	}
	
	wxRemoveFile(tempfilename);*/
	
	return true;
	
	
	
}


/*
void CreateZip(wxString zipFile, wxArrayString files)
{
	//progress_set_text(wxString::Format(wxT("Creating ZIP file...")));
	wxFileOutputStream f(zipFile);
	if (!f.IsOk()) return;
	wxZipOutputStream zf(f);
	wxDataOutputStream bin(zf);
	
	for (unsigned i = 0; i < files.size(); i++) {
		zf.PutNextEntry(GetFn(files[i]));
	//	ArchiveFile(bin, files[i]);
		//
	//	progress_set_progress(0.8+0.2*(i+1.0)/files.size());
	//	progress_set_text(wxString::Format(
	//									   wxT("Creating ZIP file...%u of %u (%s)"),
	//									   i+1, files.size(), GetFn(files[i]).c_str()), i+files.size());
	}
}*/
