/***************************************************************************
								backup.h
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

/***************************************************************************//**
 @file backup.h
 @brief Contain code for backuping and restoring projects
 @author Lucien Schreiber (c) CREALP 2007
 @date 05 May 2008
 *******************************************************************************/


#ifndef _BACKUP_H
#define _BACKUP_H

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "../database/database_tm.h"	// TM2 database operations
#include "wx/wfstream.h"				// file stream operations
#include "wx/zipstrm.h"					// zip file stream operations
#include <wx/ptr_scpd.h>				// scoped pointer
#include <wx/regex.h>					// regular expresion for files specs.
#include <wx/progdlg.h>					// the progress dialog

/***************************************************************************//**
 @brief Used for generic backup restore operations
 @author Lucien Schreiber (c) CREALP 2007
 @date 06 May 2008
 *******************************************************************************/
class BackupRestore : public wxObject
	{
	private:
		int GetDatabasePath();
		DataBaseTM * m_pDatabase;
		wxString m_DBName;
		
	protected:
		
		wxString m_DatabasePath;
		wxString m_UserDefinedPath;
		
		
	public:
		// ctor - dtor
		BackupRestore(DataBaseTM * pDB);
		~BackupRestore();
		
		
		bool IsPathsSpecified();
		long ListDirFiles (const wxString & sDir, wxArrayString & files);
		
		// getters and setters 
		virtual wxString GetDirOrigin (){ return wxEmptyString;}
		virtual wxString GetDirDestination (){ return wxEmptyString;}
		wxString GetDatabaseName(){return m_DBName;}
	};



/***************************************************************************//**
 @brief Used for Backup of the project
 @author Lucien Schreiber (c) CREALP 2007
 @date 06 May 2008
 *******************************************************************************/
class Backup : public BackupRestore
	{
	private:
		
		
	protected:
		wxString CreateBackupName ();
		wxString GetDateAsString();
		
	public:
		Backup(DataBaseTM * pDB);
		~Backup(){;}
		
		virtual wxString GetDirOrigin (){ return m_UserDefinedPath;}
		virtual wxString GetDirDestination (){ return m_DatabasePath;}
		
		bool Save (const wxArrayString & files);
		bool TestZip (wxString filename1, wxString filename2);
		
	};

wxDECLARE_SCOPED_PTR(wxZipEntry, wxZipEntryPtr);

#endif
