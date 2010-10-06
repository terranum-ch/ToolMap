/***************************************************************************
								tmupdate.cpp
		Deals with update process by cheking internet for new versions
                             -------------------
    copyright            : (C) 2009 CREALP Lucien Schreiber
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

#include "tmupdate.h"


tmUpdate::tmUpdate()
{

	UPInit();
}



void tmUpdate::UPInit()
{
	//INIT CURL
	m_UPCurlError = curl_global_init(CURL_GLOBAL_ALL);
	m_UPCurlHandle = curl_easy_init();
	wxASSERT(m_UPCurlHandle);

	m_UPConnectimeout = 1000;
	m_UPServerInfo = "http://www.crealp.ch/down/toolmap/tmlatestversion.txt";
	m_UPServerBin = _T("http://www.crealp.ch/index.php?option=com_content&task=view&id=342&Itemid=311");

	m_UPActualVersion = wxNOT_FOUND;
	m_UPLatestVersion = wxEmptyString;

	m_UPShouldSaveParameters = false;
	m_UPCheckOnStart = true;
	m_UPProxyUse = false;
	m_UPProxyDefinition = wxEmptyString;

	// load parameters
	UPLoadParameters();
}


tmUpdate::~tmUpdate()
{
	curl_easy_cleanup(m_UPCurlHandle);
	if (m_UPShouldSaveParameters == true)
		UPSaveParameters();
}



bool tmUpdate::UPPrepareConnection()
{
	wxASSERT(m_UPCurlHandle);
	wxString sErr = _("Error preparing connection");

	CURLcode  m_UPCurlError = curl_easy_setopt(m_UPCurlHandle, CURLOPT_URL,m_UPServerInfo);
	if(m_UPCurlError != CURLE_OK)
	{
		wxLogDebug(sErr);
		return false;
	}

	m_UPCurlError = curl_easy_setopt(m_UPCurlHandle, CURLOPT_TIMEOUT_MS, m_UPConnectimeout);
	if(m_UPCurlError != CURLE_OK)
	{
		wxLogDebug(sErr);
		return false;
	}

	if (UPPrepareProxy() == false)
		return false;


	return true;
}



bool tmUpdate::UPPrepareProxy()
{
	wxASSERT(m_UPCurlHandle);
	wxString sErr = wxString::Format(_("Error with following proxy : %s"),
									 m_UPProxyDefinition.c_str());

	// not using a proxy
	if (m_UPProxyUse == false)
		return true;

	if (m_UPProxyDefinition == wxEmptyString)
	{
		wxLogError(sErr);
		return false;
	}

	char cproxy[1024];
	strcpy(cproxy, (const char*)m_UPProxyDefinition.mb_str(wxConvUTF8));
	m_UPCurlError = curl_easy_setopt(m_UPCurlHandle, CURLOPT_PROXY, cproxy);
	if (m_UPCurlError != CURLE_OK)
	{
		wxLogError(sErr);
		return false;
	}
	return true;
}


bool tmUpdate::UPPerformReceiveData(wxString & data)
{
	wxASSERT(m_UPCurlHandle);
	wxString sErr = _("Error preparing reception of data : %d");

	m_UPCurlError = curl_easy_setopt(m_UPCurlHandle, CURLOPT_WRITEFUNCTION, wxcurl_str_write);

	if(m_UPCurlError != CURLE_OK)
	{
		wxLogError(wxString::Format(sErr, GetErrorMessage()));
		return false;
	}


	wxStringOutputStream  buffer;
	m_UPCurlError = curl_easy_setopt(m_UPCurlHandle, CURLOPT_WRITEDATA,(void*)& buffer);
	if(m_UPCurlError != CURLE_OK)
	{
		wxLogError(wxString::Format(sErr, GetErrorMessage()));
		return false;
	}

	if (UPPerform() == false)
		return false;

	data = buffer.GetString();
	wxLogDebug(data);
	return true;
}


bool tmUpdate::UPPerform()
{
	wxASSERT(m_UPCurlHandle);

	UPGetErrorVerbose();

	m_UPCurlError = curl_easy_perform(m_UPCurlHandle);

	if (m_UPCurlError != CURLE_OK)
	{
		wxLogWarning(_("ToolMap wasn't able to check for updates.\nMake sure your computer is connected\nto the Internet and try again."));
		return false;
	}
	return true;
}



bool tmUpdate::IsServerResponding ()
{
	if (UPPrepareConnection() == false)
		return false;

	if (UPPerform() == false)
		return false;

	return true;
}



void tmUpdate::UPGetErrorVerbose()
{
#ifdef __WXDEBUG__
#ifdef __UP_VERBOSE__
	wxASSERT(m_UPCurlHandle);
	m_UPCurlError = curl_easy_setopt(m_UPCurlHandle, CURLOPT_VERBOSE, true);
#endif
#endif
}


int tmUpdate::GetErrorMessage()
{
	return (int) m_UPCurlError;
}




bool tmUpdate::UPGetLatestVersion()
{
	if (UPPrepareConnection() == false)
		return false;

	if (UPPrepareProxy() == false)
		return false;

	wxString myLatestNum = wxEmptyString;
	if (UPPerformReceiveData(myLatestNum) == false)
		return false;

	m_UPLatestVersion = myLatestNum;
	return true;
}


int tmUpdate::UPExtractSVNNumber(const wxString & textversion)
{
	if (textversion.IsEmpty())
		return wxNOT_FOUND;

	int myPos = textversion.Find('.', true);
	if (myPos == wxNOT_FOUND)
		return wxNOT_FOUND;

	long mySvnNumber = 0;
	textversion.Mid(textversion.Len() -myPos).ToLong(&mySvnNumber);
	return (int) mySvnNumber;
}


bool tmUpdate::IsNewVersionAvaillable()
{
	if (UPGetLatestVersion() == false)
		return false;

	int iNewVersion = UPExtractSVNNumber(m_UPLatestVersion);
	if (iNewVersion == wxNOT_FOUND)
		return false;


	if (m_UPActualVersion >= iNewVersion)
		return false;

	wxLogDebug(_T("New version availlable : actual : %d, new : %d"), m_UPActualVersion, iNewVersion);
	return true;
}


wxString tmUpdate::GetDownloadLink ()
{
	if (m_UPLatestVersion == wxEmptyString)
		return wxEmptyString;

	return m_UPServerBin;
}


bool tmUpdate::UPLoadParameters()
{
	wxFileConfig myConfig;

	if(myConfig.Exists(_T("UPDATE"))==false)
	{
		//wxLogDebug(_T("No config file found"));
		return false;
	}

	myConfig.SetPath(_T("UPDATE"));
	myConfig.Read(_T("check_on_start"), &m_UPCheckOnStart);
	myConfig.Read(_T("proxy_use"), &m_UPProxyUse);
	myConfig.Read(_T("proxy_info"), &m_UPProxyDefinition);
	return true;
}


bool tmUpdate::UPSaveParameters()
{

	if (m_UPShouldSaveParameters == false)
		return false;

	wxFileConfig  myConfig;
	wxLogDebug(_T("Saving parameters into config file : %s"),
			   myConfig.GetAppName().c_str());

	myConfig.SetPath(_T("UPDATE"));
	myConfig.Write(_T("check_on_start"), m_UPCheckOnStart);
	myConfig.Write(_T("proxy_use"), m_UPProxyUse);
	myConfig.Write(_T("proxy_info"), m_UPProxyDefinition);
	myConfig.Flush();

	m_UPShouldSaveParameters = false;
	return true;
}


void tmUpdate::SetParameters (bool checkonstart, bool useproxy, const wxString & proxyinfo)
{
	m_UPCheckOnStart = checkonstart;
	m_UPProxyUse = useproxy;
	m_UPProxyDefinition = proxyinfo;

	// when params are changed, need save...
	m_UPShouldSaveParameters = true;
}

void tmUpdate::GetParameters (bool & checkonstart,bool & useproxy,wxString & proxyinfo)
{
	checkonstart = m_UPCheckOnStart;
	useproxy = m_UPProxyUse;
	proxyinfo = m_UPProxyDefinition;
}


bool tmUpdate::IsCheckOnStart()
{
	return m_UPCheckOnStart;
}




size_t wxcurl_str_write(void* ptr, size_t size, size_t nmemb, void* stream)
{
	size_t iRealSize = size * nmemb;

	wxOutputStream* pBuf = (wxOutputStream*)stream;

	if(pBuf)
	{
		pBuf->Write(ptr, iRealSize);

		return pBuf->LastWrite();
	}

	return 0;

}

