/***************************************************************************
								diroperation.cpp
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


#include "diroperation.h"

IMPLEMENT_DYNAMIC_CLASS(DirOperation, wxObject)

DirOperation::DirOperation()
{
	// init default members values
	InitMembers();
}


DirOperation::DirOperation(wxString OriginPath, wxString DestinationPath)
{
	// init default members values
	InitMembers();
	
	m_Path[DIROP_PATH_ORIGIN] = OriginPath;
	m_Path[DIROP_PATH_DESTINATION] = DestinationPath;
	
}




DirOperation::~DirOperation()
{

}


void DirOperation::InitPath(wxString OriginPath, wxString DestinationPath)
{
	m_Path[DIROP_PATH_ORIGIN] = OriginPath;
	m_Path[DIROP_PATH_DESTINATION] = DestinationPath;
}


void DirOperation::InitMembers()
{
	// init default members values
	m_Path[DIROP_PATH_ORIGIN] = _T("");
	m_Path[DIROP_PATH_DESTINATION] = _T("");
}


double DirOperation::GetDirectorySize (DIROP_SELECTDIR dirselection)
{
	wxLongLong myBigSize;
	if (wxDirExists(m_Path[dirselection]))
	{
		// compute the size
		myBigSize = wxDir::GetTotalSize(m_Path[dirselection]);
		return myBigSize.ToDouble() / 1048576;
	}
	
	return -1;
}


long DirOperation::GetAllDirectoryFiles(wxArrayString & filesNames,
										DIROP_SELECTDIR dirselection)
{
	// clear the array
	filesNames.Clear();
	
	if (wxDirExists(m_Path[dirselection]))
	{
		wxDir::GetAllFiles(m_Path[dirselection], &filesNames);
		return filesNames.GetCount();
	}
	return -1;
}


bool DirOperation::HasEnoughFreeSpace (double megabyteSize,
									   DIROP_SELECTDIR dirselection){
	wxLongLong myFreeSizeBytes;
	wxGetDiskSpace(m_Path[dirselection], NULL, &myFreeSizeBytes);
	
	// avoid division problem
	if (myFreeSizeBytes > 0){
		double myFreeSizeMB = myFreeSizeBytes.ToDouble() / 1048576;
		wxLogDebug(_T("Free space is : %.*f [MB]"),3, myFreeSizeMB);
		if (myFreeSizeMB > megabyteSize)
			return true;
	}
	return false;
}


bool DirOperation::IsPathWritable (DIROP_SELECTDIR dirselection)
{
	// try to create the directory, if an error occur 
	// we return FALSE.
	
	return wxMkdir(m_Path[dirselection]);
}


bool DirOperation::CopyDirectory(const wxArrayString & filesNames, bool showprogress)
{
	wxBusyCursor * wait = NULL;
	wxString myfileName = _T("");
	wxFileName myFileDestinationPathName;
	wxString myFileDestinationName;
	
	bool bResult = TRUE;
	
	if (showprogress){
		wait = new wxBusyCursor();
	}
	
	long lNumOfFiles = filesNames.GetCount();
	
	// copy loop
	for (long l=0; l<lNumOfFiles; l++)
	{
		myfileName = filesNames.Item(l);
		myFileDestinationPathName.Assign(myfileName);
		myFileDestinationName = myFileDestinationPathName.GetFullName();
		myFileDestinationPathName.Assign(m_Path[DIROP_PATH_DESTINATION], myFileDestinationName);
		
		bResult = bResult && wxCopyFile(myfileName, myFileDestinationPathName.GetFullPath());
	}
	
	wxDELETE(wait);
	return bResult;
}




