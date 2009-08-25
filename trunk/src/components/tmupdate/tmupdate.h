/***************************************************************************
								tmupdate.h
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


#ifndef _TM_UPDATE_H_
#define _TM_UPDATE_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <curl/curl.h>		// for web acces
#include <wx/sstream.h>
#include <wx/fileconf.h>	// for file configuration

// comment to avoid verbose mode
// works only in debug mode
//#define __UP_VERBOSE__ 


// CALLBACK FOR CURL
size_t wxcurl_str_write(void* ptr, size_t size, size_t nmemb, void* stream);


class tmUpdate : public wxObject
	{
	private:
		// curl member data
		CURL * m_UPCurlHandle;
		CURLcode m_UPCurlError;
		
		// fixed parameters 
		long m_UPConnectimeout;
		const char * m_UPServerInfo;
		wxString m_UPServerBin;
		
		
		long m_UPActualVersion;
		
		// config parameters
		bool m_UPShouldSaveParameters;
		bool m_UPCheckOnStart;
		bool m_UPProxyUse;
		wxString m_UPProxyDefinition;
		
		
		// results
		wxString m_UPLatestVersion;
		
		
		// functions
		void	UPInit();
		bool	UPPrepareConnection();
		bool	UPPrepareProxy();

		bool	UPPerform();
		bool	UPPerformReceiveData(wxString & data);
		
		bool	UPGetLatestVersion();
		int		UPExtractSVNNumber(const wxString & textversion);
		void	UPGetErrorVerbose();
		
		bool    UPLoadParameters();
		bool	UPSaveParameters();
		
	public:
		tmUpdate();
		~tmUpdate();
		
		
		bool IsNewVersionAvaillable();
		bool IsServerResponding ();
		
		int GetErrorMessage();
		
		// setter & getter
		void SetActualVersion (long myVersion){m_UPActualVersion = myVersion;}
		wxString GetNewVersionName (){return m_UPLatestVersion;}
		wxString GetDownloadLink ();
		
		// saving & getting parameters
		void SetParameters (bool checkonstart = true,
							bool useproxy = false,
							const wxString & proxyinfo = wxEmptyString);
		void GetParameters (bool & checkonstart,
							bool & useproxy,
							wxString & proxyinfo);
		
		
		bool IsCheckOnStart();
		
	};





#endif
