/***************************************************************************
 lscrashreport.cpp
 -------------------
 copyright            : (C) 2012 CREALP Lucien Schreiber
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "lscrashreport.h"

#include "../../gui/bitmaps.h"
#include "version.h"

void lsCrashReport_DLG::OnLinkClicked(wxHyperlinkEvent& event) {
    // wxLogWarning(_("Not implemented :-P directory is: %s"), m_CrashDirectory);
    wxLaunchDefaultApplication(m_CrashDirectory);
}

void lsCrashReport_DLG::_CreateControls() {
    this->SetSizeHints(wxDefaultSize, wxDefaultSize);

    wxBoxSizer* bSizer43;
    bSizer43 = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer* bSizer51;
    bSizer51 = new wxBoxSizer(wxHORIZONTAL);

    wxStaticBitmap* m_bitmap2;
    m_bitmap2 = new wxStaticBitmap(this, wxID_ANY, Bitmaps::GetMiscBitmap(Bitmaps::ID_MISC::BUG, wxSize(48, 48)),
                                   wxDefaultPosition, wxDefaultSize, 0);
    bSizer51->Add(m_bitmap2, 0, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 5);

    m_InfoTextCtrl = new wxStaticText(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE);
    m_InfoTextCtrl->Wrap(-1);
    bSizer51->Add(m_InfoTextCtrl, 0, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 5);

    bSizer43->Add(bSizer51, 0, wxEXPAND | wxALL, 5);

    wxBoxSizer* bSizer45;
    bSizer45 = new wxBoxSizer(wxVERTICAL);

    wxStaticText* m_staticText31;
    m_staticText31 = new wxStaticText(
        this, wxID_ANY, _("Please send us this error report to help fix the problem and improve this software"),
        wxDefaultPosition, wxDefaultSize, 0);
    m_staticText31->Wrap(-1);
    bSizer45->Add(m_staticText31, 0, wxALL, 5);

    wxStaticBoxSizer* sbSizer5;
    sbSizer5 = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, _("description")), wxVERTICAL);

    wxFlexGridSizer* fgSizer4;
    fgSizer4 = new wxFlexGridSizer(2, 2, 0, 0);
    fgSizer4->AddGrowableCol(1);
    fgSizer4->SetFlexibleDirection(wxBOTH);
    fgSizer4->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

    wxStaticText* m_staticText33;
    m_staticText33 = new wxStaticText(this, wxID_ANY, _("email (optional):"), wxDefaultPosition, wxDefaultSize, 0);
    m_staticText33->Wrap(-1);
    fgSizer4->Add(m_staticText33, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

    m_CrashEmailCtrl = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
    fgSizer4->Add(m_CrashEmailCtrl, 1, wxALL | wxEXPAND, 5);

    wxStaticText* m_staticText34;
    m_staticText34 = new wxStaticText(this, wxID_ANY, _("Problem description:"), wxDefaultPosition, wxDefaultSize, 0);
    m_staticText34->Wrap(-1);
    fgSizer4->Add(m_staticText34, 0, wxALL, 5);

    m_CrashDescCtrl = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(-1, 100), wxTE_MULTILINE);
    fgSizer4->Add(m_CrashDescCtrl, 1, wxALL | wxEXPAND, 5);

    sbSizer5->Add(fgSizer4, 1, wxEXPAND, 5);

    bSizer45->Add(sbSizer5, 1, wxEXPAND | wxALL, 5);

    wxBoxSizer* bSizer54;
    bSizer54 = new wxBoxSizer(wxHORIZONTAL);

    wxStaticText* m_staticText35;
    m_staticText35 = new wxStaticText(this, wxID_ANY, _("To see what this report contains"), wxDefaultPosition,
                                      wxDefaultSize, 0);
    m_staticText35->Wrap(-1);
    bSizer54->Add(m_staticText35, 0, wxALL, 5);

    m_DisplayInfoLinkCtrl = new wxHyperlinkCtrl(this, wxID_ANY, _("click here"), wxT("http://www.wxformbuilder.org"),
                                                wxDefaultPosition, wxDefaultSize, wxHL_DEFAULT_STYLE);
    bSizer54->Add(m_DisplayInfoLinkCtrl, 0, wxALL, 5);

    bSizer45->Add(bSizer54, 0, wxEXPAND | wxALL, 5);

    wxBoxSizer* bSizer55;
    bSizer55 = new wxBoxSizer(wxHORIZONTAL);

    bSizer55->Add(0, 0, 1, wxEXPAND, 5);

    wxButton* m_button37;
    m_button37 = new wxButton(this, wxID_OK, _("&Send"), wxDefaultPosition, wxDefaultSize, 0);
    m_button37->SetDefault();
    bSizer55->Add(m_button37, 0, wxALL, 5);

    wxButton* m_button38;
    m_button38 = new wxButton(this, wxID_CANCEL, _("E&xit"), wxDefaultPosition, wxDefaultSize, 0);
    bSizer55->Add(m_button38, 0, wxALL, 5);

    bSizer45->Add(bSizer55, 0, wxEXPAND | wxALL, 5);

    bSizer43->Add(bSizer45, 1, wxEXPAND, 5);

    this->SetSizer(bSizer43);
    // this->Layout();
    // bSizer43->Fit( this );

    // Connect Events
    m_DisplayInfoLinkCtrl->Connect(wxEVT_COMMAND_HYPERLINK, wxHyperlinkEventHandler(lsCrashReport_DLG::OnLinkClicked),
                                   NULL, this);
}

lsCrashReport_DLG::lsCrashReport_DLG(wxWindow* parent, const wxString& softname, const wxString& tempdirectory,
                                     wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size,
                                     long style)
    : wxDialog(parent, id, title, pos, size, style) {
    _CreateControls();
    m_InfoTextCtrl->SetLabel(
        wxString::Format(_("%s has encountered an unexpected problem and will close. "), softname));
    m_InfoTextCtrl->SetFont(m_InfoTextCtrl->GetFont().Bold());
    m_CrashDirectory = tempdirectory;
    this->Layout();
    GetSizer()->Fit(this);
}

lsCrashReport_DLG::~lsCrashReport_DLG() {}

wxString lsCrashReport_DLG::GetEmail() {
    return m_CrashEmailCtrl->GetValue();
}

wxString lsCrashReport_DLG::GetDescription() {
    return m_CrashDescCtrl->GetValue();
}

/*************************************************************************************/ /**
 Crash report class
 *****************************************************************************************/
wxString lsCrashReport::_CreateStyleSheet() {
    wxString myStyleSheet = _T(
        "body { \
                               font-family: Arial, Helvetica, sans-serif; \
                               background-color: #FFF; \
                               width: 90%; \
                               padding-right: 5%; \
                               padding-left: 5%; \
                               } \
                               \
                               h1 { \
                               font-size: 24px; \
                               border-width: 2 px; \
                               border-color: #F00; \
                               border-style: solid; \
                               margin:0px; \
                               padding: 2px; \
                               } \
                               \
                               h2 { \
                               font-size: 16px; \
                               line-height: normal; \
                               margin-top: 0px; \
                               margin-bottom: 0px; \
                               padding-top: 20px; \
                               padding-bottom: 5px; \
                               } \
                               \
                               p { \
                               padding: 0px; \
                               margin: 0px; \
                               } \
                               \
                               .crash { \
                               font-size: 12px; \
                               font-style: italic; \
                               padding-top: 5px; \
                               }");
    return myStyleSheet;
}

wxString lsCrashReport::_CreateGeneralInfo() {
    wxString myGeneralInfo = _T(
        "<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01//EN\" \
                                 \"http://www.w3.org/TR/html4/strict.dtd\"> \
                                 <html lang=\"en\"> \
                                 <head> \
                                 <meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"> \
                                 <title>Bug Report - General information</title> \
                                 <link href=\"crash.css\" rel=\"stylesheet\" type=\"text/css\">  \
                                 </head> \
                                 <body> \
                                 <h1>General information</h1> \
                                 <p class=\"crash\">Crash report generated on : %s by '%s'</p>  \
                                 <h2>OS Description</h2> \
                                 <p>%s</p> \
                                 <h2>Availlable Free Memory</h2> \
                                 <p>%s</p> \
                                 <h2>Host Name</h2> \
                                 <p>%s</p> \
                                 <h2>User ID</h2> \
                                 <p>%s</p> \
                                 <h2>User Name</h2> \
                                 <p>%s</p> \
                                 <h2>Software version</h2> \
                                 <p>%s</p> \
                                 </body> \
                                 </html>");

    wxString mySoftVersion;
    mySoftVersion << ToolMap_MAJOR_VERSION << "." << ToolMap_MINOR_VERSION << "." << GIT_NUMBER;

    return wxString::Format(myGeneralInfo, wxDateTime::Now().FormatISOCombined('-'), m_SoftName, wxGetOsDescription(),
                            wxGetFreeMemory().ToString(), wxGetHostName(), wxGetUserId(), wxGetUserName(),
                            mySoftVersion);
}

wxString lsCrashReport::_CreateAddInfo(const wxString& email, const wxString& description) {
    wxString myAddInfo = _T(
        "<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01//EN\" \
                            \"http://www.w3.org/TR/html4/strict.dtd\"> \
                            <html lang=\"en\"> \
                            <head> \
                            <meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"> \
                            <title>Bug Report - Added information</title> \
                            <link href=\"crash.css\" rel=\"stylesheet\" type=\"text/css\">  \
                            </head> \
                            <body> \
                            <h1>Added information</h1> \
                            <p class=\"crash\">Crash report generated on : %s by '%s'</p>  \
                            <h2>Email</h2> \
                            <p><a href=\"mailto:%s\">%s</a></p> \
                            <h2>Description</h2> \
                            <p>%s</p> \
                            </body> \
                            </html>");
    wxString myDesc = description;
    myDesc.Replace(_T("\n"), _T("<br>"));

    return wxString::Format(myAddInfo, wxDateTime::Now().FormatISOCombined('-'), m_SoftName, email, email, myDesc);
}

lsCrashReport::lsCrashReport(const wxString& softname) {
    m_SoftName = softname;
    m_ReportZipName = wxEmptyString;
    m_Silent = false;
    m_Report = nullptr;
}

lsCrashReport::~lsCrashReport() {
    wxDELETE(m_Report);
}

void lsCrashReport::AddFileToReport(const wxString& filename) {
    m_AddedFileNames.Add(filename);
}

bool lsCrashReport::PrepareReport(wxDebugReport::Context ctx, bool silent) {
    m_Silent = silent;
    m_ReportZipName = wxEmptyString;

    // preparing report
    m_Report = new wxDebugReportCompress();
    m_Report->AddAll(ctx);
    m_Report->AddText(_T("crash.css"), _CreateStyleSheet(), _T("stylesheet"));
    m_Report->AddText(_T("general.html"), _CreateGeneralInfo(), _("general information"));

    // add user specified files
    for (unsigned int i = 0; i < m_AddedFileNames.GetCount(); i++) {
        m_Report->AddFile(m_AddedFileNames[i], wxString::Format(_("user added file : %d"), i));
    }

    // Report name
    wxString baseName = BuildReportBaseName();
    m_Report->SetCompressedFileBaseName(baseName);

    if (silent) {
        m_Report->Process();
        m_ReportZipName = m_Report->GetCompressedFileName();
        m_Report->Reset();
    }

    lsCrashReport_DLG myDlg(nullptr, m_SoftName, m_Report->GetDirectory());
    if (myDlg.ShowModal() != wxID_OK) {
        return false;
    }

    m_Report->AddText(_T("added.html"), _CreateAddInfo(myDlg.GetEmail(), myDlg.GetDescription()),
                      _("added information"));
    m_Report->Process();
    m_ReportZipName = m_Report->GetCompressedFileName();
    m_Report->Reset();
    return true;
}

wxString lsCrashReport::BuildReportBaseName() const {
    wxString version;
    version << ToolMap_MAJOR_VERSION << "." << ToolMap_MINOR_VERSION << "." << GIT_NUMBER;
    wxString os = wxT("unknown");
    wxOperatingSystemId osId = wxPlatformInfo::Get().GetOperatingSystemId();

    if (osId & wxOS_MAC)
        os = wxT("mac");
    else if (osId & wxOS_WINDOWS)
        os = wxT("win");
    else if (osId & wxOS_UNIX)
        os = wxT("unix");

    wxDateTime now = wxDateTime::Now();
    wxString date = now.Format("%Y-%m-%dT%H%M");
    wxString baseName = wxString::Format("%s_%s_%s_%s", m_SoftName, version, os, date);

    return baseName;
}

bool lsCrashReport::SendReportWeb(const wxString& serverurl, const wxString& proxy) {
    CURLcode myCurlError = curl_global_init(CURL_GLOBAL_ALL);
    CURL* easyhandle = curl_easy_init();
    if (easyhandle == nullptr) {
        wxLogError("Easy Handle is null");
        return false;
    }

    wxFileName myTempZipFileName(m_Report->GetCompressedFileName());
    curl_easy_setopt(easyhandle, CURLOPT_URL, (const char*)serverurl.mb_str(wxConvUTF8));

    curl_mime *mime;
    curl_mimepart *part;

    /* Create a mime handle. */
    mime = curl_mime_init(easyhandle);

    /* Add a simple part. */
    part = curl_mime_addpart(mime);
    curl_mime_name(part, "filename");
    curl_mime_data(part, (const char*)myTempZipFileName.GetFullName().mb_str(wxConvUTF8), CURL_ZERO_TERMINATED);

    /* Add a file part. */
    part = curl_mime_addpart(mime);
    curl_mime_filedata(part, (const char*)myTempZipFileName.GetFullPath().mb_str(wxConvUTF8));

    /* Add a simple part. */
    part = curl_mime_addpart(mime);
    curl_mime_name(part, "softname");
    curl_mime_data(part, (const char*)m_SoftName.mb_str(wxConvUTF8), CURL_ZERO_TERMINATED);

    /* Associate the multipart post with the easy handle. */
    curl_easy_setopt(easyhandle, CURLOPT_MIMEPOST, mime);

    if (!proxy.IsEmpty()) {
        curl_easy_setopt(easyhandle, CURLOPT_PROXY, (const char*)proxy.mb_str(wxConvUTF8));
    }

    // read response
    wxStringOutputStream myBuffer;
    myCurlError = curl_easy_setopt(easyhandle, CURLOPT_WRITEFUNCTION, wxcurl_string_write);
    myCurlError = curl_easy_setopt(easyhandle, CURLOPT_WRITEDATA, (void*)&myBuffer);
#if defined(__WIN32__)
    // Disable certificate check (does not work on Windows)
    myCurlError = curl_easy_setopt(easyhandle, CURLOPT_SSL_VERIFYPEER, false);
#endif

    myCurlError = curl_easy_perform(easyhandle); /* post away! */
    if (myCurlError != CURLE_OK) {
        wxString myErr(curl_easy_strerror(myCurlError));
        wxLogError(myErr);

        curl_easy_cleanup(easyhandle);
        curl_mime_free(mime);
        return false;
    }
    wxString myPageInfo = myBuffer.GetString();
    wxLogWarning(myPageInfo);
    curl_easy_cleanup(easyhandle);
    curl_mime_free(mime);
    return true;
}

bool lsCrashReport::SaveReportFile(const wxString& directory) {
    wxFileName myExistingFile(m_ReportZipName);
    wxFileName myCopiedFile(myExistingFile);
    myCopiedFile.SetPath(directory);
    return wxCopyFile(myExistingFile.GetFullPath(), myCopiedFile.GetFullPath());
}

size_t wxcurl_string_write(void* ptr, size_t size, size_t nmemb, void* stream) {
    size_t iRealSize = size * nmemb;
    wxOutputStream* pBuf = (wxOutputStream*)stream;
    if (pBuf) {
        pBuf->Write(ptr, iRealSize);
        return pBuf->LastWrite();
    }
    return 0;
}
