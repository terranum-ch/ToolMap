/***************************************************************************
								diroperation.h
                    Super class for dealing with Directory operations
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

// comment doxygen


#ifndef DIROPERATION_H
#define DIROPERATION_H

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <wx/dir.h>
#include <wx/filename.h>


enum DIROP_SELECTDIR
{
	DIROP_PATH_ORIGIN = 0,
	DIROP_PATH_DESTINATION
};




/***************************************************************************//**
 @brief For directory operations (copy, delete...)
 @details This is intended to be a super class dealing with directory operation
 such as copy, move, count size of a directory and so on.
 @author Lucien Schreiber (c) CREALP 2007
 @date 05 February 2008
 *******************************************************************************/
class DirOperation
	{
	protected:
		wxString m_Path[2];
		//wxString m_DestinationPath;
		
		void InitMembers();
		
	public:
		DirOperation();
		DirOperation(wxString OriginPath, wxString DestinationPath = _T(""));
		~DirOperation();
		void InitPath(wxString OriginPath = _T(""), wxString DestinationPath = _T(""));
		
		double GetDirectorySize (DIROP_SELECTDIR dirselection = DIROP_PATH_ORIGIN);
		long GetAllDirectoryFiles(wxArrayString & filesNames,
								  DIROP_SELECTDIR dirselection = DIROP_PATH_ORIGIN);
		bool HasEnoughFreeSpace (double megabyteSize, 
								  DIROP_SELECTDIR dirselection = DIROP_PATH_ORIGIN);
		bool IsPathWritable (DIROP_SELECTDIR dirselection = DIROP_PATH_DESTINATION);
		
		bool CopyDirectory(const wxArrayString & filesNames,
						   bool showprogress = FALSE);

	};





#endif
