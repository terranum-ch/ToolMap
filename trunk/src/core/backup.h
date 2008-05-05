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

#include "../database/database_tm.h"

class BackupRestore : public wxObject
	{
	private:
		DataBaseTM * m_pDatabase;
		
	protected:
		wxString m_DatabasePath;
		wxString m_UserDefinedPath;
		
		
	public:
		BackupRestore(DataBaseTM * pDB);
		~BackupRestore();
		
		int GetDatabasePath();
		bool IsPathsSpecified(){return TRUE;}
		
		
		virtual wxString GetDirOrigin (){ return wxEmptyString;}
		virtual wxString GetDirDestination (){ return wxEmptyString;}
	};



class Backup : public BackupRestore
	{
	public:
		Backup(DataBaseTM * pDB);
		~Backup(){;}
		
		virtual wxString GetDirOrigin (){ return m_UserDefinedPath;}
		virtual wxString GetDirDestination (){ return m_DatabasePath;}
		
	};



#endif
