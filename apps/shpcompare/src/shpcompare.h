/***************************************************************************
 shpcompare.h
 -------------------
 copyright            : (C) 2012 CREALP Lucien Schreiber 
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
#ifndef _SHPCOMPARE_H_
#define _SHPCOMPARE_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <wx/filename.h>
#include "ogrsf_frmts.h"

class ShpCompare {
private:
    wxFileName m_ReferenceFileName;
    wxArrayString m_FilesNamesToCheck;
    wxArrayString m_Messages;
    wxArrayString m_Errors;
    
    bool _DoCompareOneFile(OGRLayer * referencelayer, const wxString & filename);
    bool _DoCompareFields (OGRFieldDefn * reffield, OGRFieldDefn * testfield);
    
public:
    ShpCompare();
    virtual ~ShpCompare();
    
    inline const wxString GetReferenceFileName() const;
    void SetReferenceFileName(const wxString  & filename);
    
    const wxArrayString * GetFilesNamesToCheckRef() const;
    void AddFileNameToCheck(const wxString & filename);
    
    bool DoCompare(int resultslimit = 500);
    
    inline const wxArrayString * GetMessagesRef() const;
    inline const wxArrayString * GetErrorsRef() const;
    bool HasErrors();
};


inline const wxString ShpCompare::GetReferenceFileName() const {
  return m_ReferenceFileName.GetFullPath();
}



inline const wxArrayString * ShpCompare::GetMessagesRef() const {
  return &m_Messages;
}



inline const wxArrayString * ShpCompare::GetErrorsRef() const {
  return &m_Errors;
}



#endif
