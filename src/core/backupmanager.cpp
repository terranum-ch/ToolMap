/***************************************************************************
 backupmanager.cpp
 -------------------
 copyright            : (C) 2011 CREALP Lucien Schreiber 
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


#include "backupmanager.h"
#include "../database/database_tm.h"
#include "tmpercent.h"



BackupFile::BackupFile() {
    m_OutFileName.Clear();
    m_InDirectory.Clear();
    m_Comment = wxEmptyString;
    m_Author = wxEmptyString;
    m_Date = wxDateTime();
	m_UseDate = true;
    wxASSERT(m_Date.IsValid() == false);
}




BackupFile::~BackupFile() {
}



bool BackupFile::IsValid() const{
    if (m_OutFileName.IsOk() == false) {
        return false;
    }
    
    if (m_InDirectory.IsOk() == false) {
        return false;
    }
    
    if (m_Date.IsValid() == false) {
        return false;
    }
    return true;
}



void BackupFile::SetOutputName(wxFileName value) {
  m_OutFileName = value;
}



void BackupFile::SetInputDirectory(wxFileName value) {
  m_InDirectory = value;
}



void BackupFile::SetComment(wxString value) {
  m_Comment = value;
}



void BackupFile::SetAuthor(wxString value) {
  m_Author = value;
}



void BackupFile::SetDate(wxDateTime value) {
  m_Date = value;
}


void BackupFile::SetUseDate(bool value) {
	m_UseDate = value;
}




/***************** BACKUP MANAGER **********************/
BackupManager::BackupManager(DataBaseTM * database) {
    m_Database = database;
}



BackupManager::~BackupManager() {
}



void BackupManager::_ListMySQLFiles(const wxString & directory, wxArrayString & files) {
    wxDir myDir (directory);	
	wxRegEx FileSpec(wxT(".opt|.frm|.MYD|.MYI|.xml"));
	wxString myActualFileName;
    // iterates all files in directory
	bool cont = myDir.GetFirst(&myActualFileName, wxEmptyString, wxDIR_FILES);
	while (cont)
	{
		if (FileSpec.Matches(myActualFileName)){
			files.Add(myActualFileName);
		}
		else {
			wxLogError(_T("File '%s' will not be backuped"), myActualFileName);
		}		
		cont = myDir.GetNext(&myActualFileName);
	}	
}



bool BackupManager::Backup(const BackupFile & fileinfo, wxWindow * progressparent) {
    // some checks
    if (fileinfo.IsValid() == false) {
        return false;
    }
    
    if (wxDirExists(fileinfo.GetOutputName().GetPath()) == false ||
        wxDirExists(fileinfo.GetInputDirectory().GetFullPath()) == false) {
        wxLogError(_("Backup directory doesn't exists!"));
        return false;
    }
    
    // getting list of files to backup !
    wxArrayString myFilesToBackup;
    _ListMySQLFiles(fileinfo.GetInputDirectory().GetFullPath(), myFilesToBackup);
    
	// Append date to filename if required
	wxFileName myFileOut (fileinfo.GetOutputName());
    if (fileinfo.IsUsingDate() == true) {
		myFileOut.SetName(myFileOut.GetName() + "-" + fileinfo.GetDate().FormatISODate() +
						  "-" + fileinfo.GetDate().Format(_T("%H%M%S")));		
	}
    
	wxFFileOutputStream outf(myFileOut.GetFullPath());
	if (!outf.Ok()){
		wxLogError(_("Could not open file: '%s'"), myFileOut.GetFullName());
		return false;
	}
	wxZipOutputStream outzip(outf);
	outzip.PutNextDirEntry(m_Database->DataBaseGetName());
	
	// progress window
	wxProgressDialog * myProgressDialog = NULL;
	if (progressparent != NULL) {
		myProgressDialog = new wxProgressDialog(_("Backup project"), wxEmptyString, tmPERCENT);
	}
	tmPercent myPercent(myFilesToBackup.GetCount());
	
	// loop for adding all files
	for (unsigned int i = 0; i<myFilesToBackup.GetCount(); i++){
		wxFileName fn1 (fileinfo.GetInputDirectory().GetFullPath(), myFilesToBackup.Item(i));
		wxFileInputStream f1stream(fn1.GetFullPath());
		if (!f1stream.Ok()){
			wxLogError(wxT("Error opening file: '%s'"), myFilesToBackup.Item(i));
			return false;
		}
		
		// increment progress
		myPercent.SetValue(i);
		if (myProgressDialog != NULL && myPercent.IsNewStep()) {
			myProgressDialog->Update(myPercent.GetPercent());
		}
		
		// realy adding files into zip
		outzip.PutNextEntry(m_Database->DataBaseGetName() +
                            wxFileName::GetPathSeparator() +
                            myFilesToBackup.Item(i));
		outzip << f1stream;
	}
	wxDELETE(myProgressDialog);
	
    SetMetadata(fileinfo, &outzip);

	if (outzip.Close() == false){
		wxLogError(_("Error Closing file"));
		return false;
	}
	outf.Close();
	wxLogMessage(_("Database backuped into: '%s'"), myFileOut.GetFullName());
    
    
    return true;
}



bool BackupManager::Restore(const BackupFile & fileinfo, wxWindow * progressparent) {
    // some checks
    if (fileinfo.IsValid() == false) {
        return false;
    }
    
    // ensure file exists
    if (wxFileExists(fileinfo.GetOutputName().GetFullPath())==false) {
        wxLogError(_("Backup file: '%s' doesn't exists!"),
                   fileinfo.GetOutputName().GetFullPath());
        return false;
    }
    
    // remove directory if existing!
    if (wxDirExists(fileinfo.GetInputDirectory().GetFullPath()) == true) {
        wxLogMessage(_("Directory: '%s' exists and will be removed"),
                     fileinfo.GetInputDirectory().GetFullPath());
        if (wxFileName::Rmdir(fileinfo.GetInputDirectory().GetFullPath(),
                              wxPATH_RMDIR_RECURSIVE) == false) {
            wxLogError(_("'%s' could not be removed!"),
                       fileinfo.GetInputDirectory().GetFullPath()); 
            return false;
        } 
    }
    
    // create directory
    if(wxFileName::Mkdir(fileinfo.GetInputDirectory().GetFullPath())==false){
        wxLogError(_("Creating: '%s' failed!"), fileinfo.GetInputDirectory().GetFullPath());
        return false;
    }
    
    // extract from backup
    wxScopedPtr<wxZipEntry> entry;
    wxFFileInputStream in(fileinfo.GetOutputName().GetFullPath());
    wxZipInputStream zip(in);
    int myCount = 0;
    while (entry.reset(zip.GetNextEntry()), entry.get() != NULL)
    {
        if (entry->IsDir() == true) {
            continue;
        }
        // access meta-data
        wxString name = entry->GetName();
        wxFileName myZipNameOnly (name);
        wxFileName myZipName (fileinfo.GetInputDirectory().GetFullPath(),
                              myZipNameOnly.GetFullName());
		
		zip.OpenEntry(*entry.get());
        if (zip.CanRead() == false) {
			wxLogError(_T("Can not read zip entry '") + entry->GetName() + _T("'."));
			return false;
        }
		
		wxFileOutputStream myOut(myZipName.GetFullPath());
		if (myOut.IsOk() == false) {
			wxLogError(_("Error writing: '%s'"), myZipName.GetFullName());
			return false;
		}
        zip.Read(myOut);
        myCount++;
    }
	wxLogMessage(_("%d file(s) restored!"), myCount);
	return true;
}



bool BackupManager::GetFileInfo(const wxFileName & file, BackupFile & fileinfo) {
    fileinfo.SetOutputName(file);
    
    // read comment and author
    wxFFileInputStream outf(file.GetFullPath());
	if (outf.IsOk() == false){
		wxLogError(_("Could not open file: '%s'"), file.GetFullName());
		return false;
	}
	
    { 
        wxZipInputStream inzip(outf);
        wxString myXMLcomment = inzip.GetComment();
        if (myXMLcomment.IsEmpty() == false) {
            wxStringInputStream myXMLStream (myXMLcomment);
            wxXmlDocument doc;
            if (doc.Load(myXMLStream) != false) {
                wxXmlNode *child = doc.GetRoot()->GetChildren();
                while (child) {
                    if (child->GetName() == _T("Author")) {
                        fileinfo.SetAuthor(child->GetAttribute(_T("Name"), wxEmptyString));
                    }
                    else if (child->GetName() == _T("Comment")) {
                        fileinfo.SetComment(child->GetAttribute(_T("Text"), wxEmptyString));
                    }
                    child = child->GetNext();
                }
            }
        }
    }
    
    // read date
    wxString myFileName = file.GetName();
    wxString myDateTimeString = myFileName.AfterFirst('-');
    if (myDateTimeString.IsEmpty()) {
        fileinfo.SetUseDate(false);
		return true;
    }
    
    // old date format
    if (myDateTimeString.Len() == 15) {
        wxString myTemp = myDateTimeString.Mid(0, 4);
        myTemp.Append(_T("-"));
        myTemp.Append(myDateTimeString.Mid(4,2));
        myTemp.Append(_T("-"));
        myTemp.Append(myDateTimeString.Mid(6));
        wxLogMessage("Modified date is : " + myTemp);
        myDateTimeString = myTemp;
    }
    
    if (myDateTimeString.Len() != 17) {
        return false;
    }

    wxDateTime myDate;
    wxString::const_iterator end;
    bool bSuccess = myDate.ParseFormat(myDateTimeString, _T("%Y-%m-%d-%H%M%S"), &end);
    
    if (bSuccess == false) {
        wxLogMessage(_("Error parsing date : '%s'"), myDateTimeString);
        return false;
    }
    fileinfo.SetDate(myDate);
    return true;
}



bool BackupManager::SetMetadata(const BackupFile & fileinfo, wxZipOutputStream * zip) {
    if (fileinfo.IsValid() == false || zip == NULL) {
        return false;
    }
    
    // prepare XML document into a wxString
    wxXmlDocument myXMLDoc;
    wxXmlNode * root = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("metadata_backup"));
    myXMLDoc.SetRoot(root);
     
    wxXmlNode * node;
    node = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("Author"));
    node->AddAttribute(_T("Name"), fileinfo.GetAuthor());
    root->AddChild(node);
    
    node = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("Comment"));
    node->AddAttribute(_T("Text"), fileinfo.GetComment());
    root->AddChild(node);
    wxStringOutputStream myStringStream;
    myXMLDoc.Save(myStringStream);
    
    zip->SetComment(myStringStream.GetString());
    return true;
}

