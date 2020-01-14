/***************************************************************************
 menumanager.h
 Deals with menu activities (enable / disable)
 -------------------
 copyright            : (C) 2007 CREALP Lucien Schreiber
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/***************************************************************************//**
 @file menumanager.h
 @brief Deals with menu activation
 @details This class is the main point for enabling / disabling menu based on
 events.
 @author Lucien Schreiber (c) CREALP 2007
 @date 12 March 2008
 *******************************************************************************/



#ifndef _MENUMANAGER_H_
#define _MENUMANAGER_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

// Include wxWidgets' headers
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "../core/toolmap.h"
#include <wx/docview.h>            // for recent documents
#include <wx/fileconf.h>        // for using file config even on windows

/***************************************************************************//**
 @brief Deals with Recent files
 @author Lucien Schreiber (c) CREALP 2007
 @date 12 March 2008
*******************************************************************************/
class MenuManager : public wxObject
{
private:
    wxMenuBar *m_MenuBar;
    wxFileHistory *m_pFilesHistory;
DECLARE_CLASS(MenuManager);

public:
    MenuManager(wxMenuBar *menubar);

    ~MenuManager();

    // deals with recent files
    void InitializeRecentFilesHistory();

    void TerminateRecentFilesHistory();

    void AddFileToRecent(const wxString &spath);

    void RemoveFileFromRecent(int fileid);

    bool GetRecentFile(wxString &filepath, int fileid);
};


#endif
