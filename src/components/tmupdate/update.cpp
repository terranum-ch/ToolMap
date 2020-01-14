/***************************************************************************
 update.cpp
 Deals with WebUpdate process by cheking internet for new versions
 -------------------
 copyright            : (C) 2009 CREALP Lucien Schreiber
 ***************************************************************************/

#include "update.h"
#include <wx/tokenzr.h>
#include "../../core/toolmap.h"


BEGIN_EVENT_TABLE(WebUpdateInformationBar, wxInfoBar)
                EVT_BUTTON (INFOBAR_DOWNLOAD_BUTTON, WebUpdateInformationBar::OnCommandDownload)
                EVT_THREAD (THREAD_MESSAGE_NEW_VERSION, WebUpdateInformationBar::OnNewVersion)
                EVT_THREAD (THREAD_MESSAGE_NONEW_VERSION, WebUpdateInformationBar::OnNoNewVersion)
                EVT_THREAD (THREAD_MESSAGE_NO_INTERNET, WebUpdateInformationBar::OnInternetFailed)
END_EVENT_TABLE()


void WebUpdateInformationBar::_ClearBar()
{
    Dismiss();
    if (FindWindowById(INFOBAR_DOWNLOAD_BUTTON, this) != NULL) {
        RemoveButton(INFOBAR_DOWNLOAD_BUTTON);
    }
}


void WebUpdateInformationBar::OnCommandDownload(wxCommandEvent &event)
{
    wxLaunchDefaultBrowser(WEBUPDATE_SERVER_DOWNLOAD);
}


void WebUpdateInformationBar::OnInternetFailed(wxThreadEvent &event)
{
    _ClearBar();
    ShowMessage(_T("Checking for update failed! Internet isn't available."));
}


void WebUpdateInformationBar::OnNewVersion(wxThreadEvent &event)
{
    _ClearBar();
    AddButton(INFOBAR_DOWNLOAD_BUTTON, _("Download"));
    wxString myMessage = wxString::Format(_("New version of %s is available: %s (%d)"),
                                          g_ProgName,
                                          event.GetString(),
                                          event.GetInt());
    ShowMessage(myMessage);
}


void WebUpdateInformationBar::OnNoNewVersion(wxThreadEvent &event)
{
    _ClearBar();
    wxString myMessage = wxString::Format(_("No new version of %s is available!"),
                                          g_ProgName);
    ShowMessage(myMessage);
}


WebUpdateInformationBar::WebUpdateInformationBar(wxWindow *parent) : wxInfoBar(parent, wxID_ANY)
{
    m_Parent = parent;
}


WebUpdateInformationBar::~WebUpdateInformationBar()
{
}


wxThread::ExitCode WebUpdateThread::Entry()
{
    wxMilliSleep(1000);

    CURLcode myError = curl_easy_perform(m_CurlHandle);
    if (myError != CURLE_OK) {
        if (m_msgNoInternet) {
            wxThreadEvent myEvent(wxEVT_COMMAND_THREAD, THREAD_MESSAGE_NO_INTERNET);
            wxQueueEvent(m_Parent, myEvent.Clone());
        }
        return (wxThread::ExitCode) 0;
    }

    wxString mydata = m_Buffer.GetString();

    wxStringTokenizer tokenizer(mydata, "\n");
    wxArrayString myValues;
    while (tokenizer.HasMoreTokens()) {
        myValues.Add(tokenizer.GetNextToken());
    }

    if (myValues.GetCount() != 2) {
        wxLogMessage("Error parsing new version informations, file corrupted ?");
        return (wxThread::ExitCode) 0;
    }


    long myInternetVersion = wxNOT_FOUND;
    myValues.Item(1).ToLong(&myInternetVersion);
    wxThreadEvent myEvent(wxEVT_COMMAND_THREAD, THREAD_MESSAGE_NEW_VERSION);
    myEvent.SetInt((int) myInternetVersion);
    myEvent.SetString(myValues.Item(0));

    if (myInternetVersion > m_ActualVersion && m_msgNewVersion == true) {
        myEvent.SetId(THREAD_MESSAGE_NEW_VERSION);
        wxQueueEvent(m_Parent, myEvent.Clone());
        return (wxThread::ExitCode) 0;
    }

    if (m_msgNoNewVersion == true) {
        myEvent.SetId(THREAD_MESSAGE_NONEW_VERSION);
        wxQueueEvent(m_Parent, myEvent.Clone());
    }
    return (wxThread::ExitCode) 0;
}


WebUpdateThread::WebUpdateThread(wxWindow *parent, const wxString &proxy)
{
    m_Parent = parent;
    CURLcode myCurlError = curl_global_init(CURL_GLOBAL_ALL);
    wxASSERT(myCurlError == CURLE_OK);
    m_CurlHandle = curl_easy_init();

    m_msgNewVersion = true;
    m_msgNoNewVersion = true;
    m_msgNoInternet = true;
    m_ActualVersion = wxNOT_FOUND;


    // Prepare connection
    wxString myServerInfo = WEBUPDATE_SERVER_UPDATE + "?t=" + wxDateTime::Now().FormatISOCombined('-');
    myCurlError = curl_easy_setopt(m_CurlHandle,
                                   CURLOPT_URL,
                                   (const char *) myServerInfo.mb_str(wxConvUTF8));
    wxASSERT(myCurlError == CURLE_OK);

    myCurlError = curl_easy_setopt(m_CurlHandle, CURLOPT_TIMEOUT_MS, WEBUPDATE_CONNECTION_TIMEOUT);
    wxASSERT(myCurlError == CURLE_OK);

    // prepare proxy if needed
    if (proxy.IsEmpty() == false) {
        myCurlError = curl_easy_setopt(m_CurlHandle, CURLOPT_PROXY,
                                       (const char *) proxy.mb_str(wxConvUTF8));
        wxASSERT(myCurlError == CURLE_OK);
    }

    // prepare reception of data
    myCurlError = curl_easy_setopt(m_CurlHandle, CURLOPT_WRITEFUNCTION, wxcurl_str_write);
    wxASSERT(myCurlError == CURLE_OK);

    myCurlError = curl_easy_setopt(m_CurlHandle, CURLOPT_WRITEDATA, (void *) &m_Buffer);
    wxASSERT(myCurlError == CURLE_OK);
}


WebUpdateThread::~WebUpdateThread()
{
    curl_easy_cleanup(m_CurlHandle);
}


bool WebUpdateThread::CheckNewVersion(long actualversion, bool msgNewVersion,
                                      bool msgNoNewVersion, bool msgNoInternet)
{
    m_msgNewVersion = msgNewVersion;
    m_msgNoNewVersion = msgNoNewVersion;
    m_msgNoInternet = msgNoInternet;
    m_ActualVersion = actualversion;

    Create();
    Run();
    return true;
}


size_t wxcurl_str_write(void *ptr, size_t size, size_t nmemb, void *stream)
{
    size_t iRealSize = size * nmemb;

    wxOutputStream *pBuf = (wxOutputStream *) stream;

    if (pBuf) {
        pBuf->Write(ptr, iRealSize);

        return pBuf->LastWrite();
    }

    return 0;

}







