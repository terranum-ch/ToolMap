/***************************************************************************
								main_panel.h
                    Display the main panel where drawing occurs
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

// comment doxygen


#ifndef MAIN_PANEL_H
#define MAIN_PANEL_H

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "managed_aui_wnd.h"
//#include <wx/scrolwin.h>
#include "../gis/tmrenderer.h"    // for GIS renderer

#define SYMBOL_MAIN_PANEL_TITLE _("Main Panel")


class Main_PANEL : public ManagedAuiWnd
{
    wxSizer *CreateControls(wxWindow *parent,
                            bool call_fit = TRUE,
                            bool set_sizer = TRUE);

    tmRenderer *m_GISRenderer;
    wxAuiPaneInfo mPaneInfo;


public:
    Main_PANEL(wxWindow *parent, wxAuiManager *AuiManager);

    ~Main_PANEL();

    tmRenderer *GetGISRenderer()
    { return m_GISRenderer; }

};


#endif
