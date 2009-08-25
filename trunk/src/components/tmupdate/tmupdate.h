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

#include <curl/curl.h>
#include <wx/sstream.h>

// comment to avoid verbose mode
#define __UP_VERBOSE__ 


// CALLBACK FOR CURL
size_t wxcurl_str_write(void* ptr, size_t size, size_t nmemb, void* stream);


class tmUpdate : public wxObject
	{
	private:
		// curl member data
		CURL * m_UPCurlHandle;
		CURLcode m_UPCurlError;
		
		// parameters 
		long m_UPConnectimeout;
		const char * m_UPServer;
		long m_UPActualVersion;
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
		
	public:
		tmUpdate();
		~tmUpdate();
		
		
		bool IsNewVersionAvaillable();
		bool IsServerResponding ();
		
		int GetErrorMessage();
		
		
	};





#endif
