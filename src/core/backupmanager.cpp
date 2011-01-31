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
#include "projectmanager.h"

BackupFile::BackupFile() {
    m_OutFileName.Clear();
    m_InDirectory.Clear();
    m_Comment = wxEmptyString;
    m_Author = wxEmptyString;
    m_Date = wxDateTime();
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






/***************** BACKUP MANAGER **********************/
BackupManager::BackupManager(ProjectManager * prjmanager) {
    wxASSERT(prjmanager);
    m_ProjectManager = prjmanager;
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



bool BackupManager::Backup(const BackupFile & fileinfo) {
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
    
    
	wxFFileOutputStream outf(fileinfo.GetOutputName().GetFullPath());
	if (!outf.Ok()){
		wxLogError(_("Could not open file: '%s'"), fileinfo.GetOutputName().GetFullName());
		return false;
	}
	wxZipOutputStream outzip(outf);
	outzip.PutNextDirEntry(m_ProjectManager->GetDatabase()->DataBaseGetName());
	
	// loop for adding all files
	for (unsigned int i = 0; i<myFilesToBackup.GetCount(); i++){
		wxFileName fn1 (fileinfo.GetInputDirectory().GetFullPath(), myFilesToBackup.Item(i));
		wxFileInputStream f1stream(fn1.GetFullPath());
		if (!f1stream.Ok()){
			wxLogError(wxT("Error opening file: '%s'"), myFilesToBackup.Item(i));
			return false;
		}
		
        /*
		// incrementing progress dialog
		if (!ProgDlg.Update(dIncrement))
		{
			bCompleted = FALSE;
			wxLogMessage(_("Backup into %s cancelled by user"), dbkfilename.GetFullName().c_str());
			break;
		}
		dIncrement  = dIncrement + dStep;
		if (dIncrement > 50)
			dIncrement = 50;*/
		
		
		// realy adding files into zip
		outzip.PutNextEntry(m_ProjectManager->GetDatabase()->DataBaseGetName() +
                            wxFileName::GetPathSeparator() +
                            myFilesToBackup.Item(i));
		outzip << f1stream;
	}
	
    SetMetadata(fileinfo, &outzip);

	if (outzip.Close() == false){
		wxLogError(_("Error Closing file"));
		return false;
	}
	outf.Close();
	wxLogMessage(_("Database backuped into : '%s'"), fileinfo.GetOutputName().GetFullName());
    
    
    return true;
}



bool BackupManager::Restore(const BackupFile & fileinfo) {
    return false;
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
        wxLogMessage(myXMLcomment);
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
        return false;
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

