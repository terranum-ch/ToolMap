/***************************************************************************
 lscrashreport.h
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
#ifndef _LSCRASHREPORT_H_
#define _LSCRASHREPORT_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <wx/hyperlink.h>
#include <wx/debugrpt.h>
#include <wx/mstream.h>
#include <wx/filename.h>
#include <wx/sstream.h>
#include <curl/curl.h>


class lsCrashReport_DLG : public wxDialog {
private:
    wxStaticText* m_InfoTextCtrl;
    wxTextCtrl * m_CrashEmailCtrl;
    wxTextCtrl * m_CrashDescCtrl;
    wxHyperlinkCtrl* m_DisplayInfoLinkCtrl;
    wxString m_CrashDirectory;
    wxBitmap * m_BmpCrash;

    void OnLinkClicked(wxHyperlinkEvent & event);
    
    void _CreateControls();
    void _CreateBitmaps();
    
public:
    lsCrashReport_DLG(wxWindow * parent, const wxString & softname, const wxString & tempdirectory, wxWindowID id = wxID_ANY, const wxString & title = _("Fatal exception"), const wxPoint & pos = wxDefaultPosition, const wxSize & size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE );
    ~lsCrashReport_DLG();
    
    wxString GetEmail();
    wxString GetDescription();
};



/*************************************************************************************//**
@brief Crash report class
@author Lucien Schreiber copyright CREALP
@date 06 september 2012
*****************************************************************************************/
class lsCrashReport {
private:
    wxString m_SoftName;
    wxString m_ReportZipName;
    bool m_Silent;
    wxArrayString m_AddedFileNames;
    wxDebugReportCompress * m_Report;
    
    wxString _CreateStyleSheet();
    wxString _CreateGeneralInfo();
    wxString _CreateAddInfo (const wxString & email, const wxString & description);
    
public:
    lsCrashReport(const wxString & softname);
    virtual ~lsCrashReport();
    
    void AddFileToReport(const wxString & filename);
    bool PrepareReport(wxDebugReport::Context ctx, bool silent = false);
    
    bool SendReportWeb(const wxString & serverurl, const wxString & proxy = wxEmptyString);
    bool SaveReportFile(const wxString & directory);
};

// CALLBACK FOR CURL
size_t wxcurl_string_write(void* ptr, size_t size, size_t nmemb, void* stream);

#endif
