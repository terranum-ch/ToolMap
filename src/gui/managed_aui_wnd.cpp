/***************************************************************************
								managed_aui_wnd.cpp
						Main class for managed aui windows
                              -------------------
    copyright            : (C) 2007 CREALP Lucien Schreiber 
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



#include "managed_aui_wnd.h"

ManagedAuiWnd::ManagedAuiWnd(wxAuiManager * AuiManager) : m_AuiMgr(AuiManager), m_AuiPaneInfo(NULL)
{
	wxLogDebug(_T("ManagedAuiWnd Ctor called %p"), m_AuiMgr);
}


ManagedAuiWnd::~ManagedAuiWnd()
{
	
}


void ManagedAuiWnd::InitValues()
{

}

