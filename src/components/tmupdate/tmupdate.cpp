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
	
}


tmUpdate::~tmUpdate()
{
	
}


bool tmUpdate::IsServerResponding (int * errorcode)
{
	
	wxHTTP myHttp;
	
	myHttp.SetTimeout(10);
	bool bConnecStatus = myHttp.Connect(_T("www.crealp.ch"));
	
	if (bConnecStatus == false)
	{
		wxLogError(_T("Error, server responded : %d"), myHttp.GetResponse());
		return false;
	}
	
	//if (wxApp::IsMainLoopRunning() == false)
	//	return false;
	
	wxInputStream *stream = myHttp.GetInputStream(_T("/down/toolmap/tmlatestversion.txt"));
	if (stream == NULL)
	{
		wxLogError(_T("Unable to get the steam"));
		return false;
	}
	
	char szBuff[1025];
	wxString data=_T("");
	// On lit une partie des données disponibles
	stream->Read(szBuff,1024);
	while (stream->LastRead())
	{
		// On s'assure que le texte téléchargé se termine bien
		// par un caratère zéro
		szBuff[stream->LastRead()]='\0';
		// On fait la conversion si nécessaire
		data+= wxString::FromAscii(szBuff);
		// Et on essaie de lire la partie suivante
		stream->Read(szBuff,1024);
	}
	delete stream;
	wxLogDebug(data);
	
	return true;
}

bool tmUpdate::IsNewVersionAvaillable(wxString & versionName)
{
	return false;
}
