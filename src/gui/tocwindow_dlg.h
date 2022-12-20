/***************************************************************************
 tocwindow_dlg.h
 Display the TOC, try using the wxDrawerWindow class
 -------------------
 copyright : (C) 2007 CREALP Lucien Schreiber
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

#ifndef TOCWINDOW_DLG_H
#define TOCWINDOW_DLG_H

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

// Include wxWidgets' headers
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/aui/dockart.h>
#include <wx/aui/framemanager.h>
#include <wx/tglbtn.h>

#include "../gis/tocctrl.h"
#include "wxflatbutton.h"

#define ID_TOCWINDOW_DLG 10035
#define ID_TREECTRL1 10127
#define ID_DLGTOC_ADD 10147
#define ID_DLGTOC_REMOVE 10145
#define SYMBOL_TOCWINDOW_DLG_STYLE wxCAPTION | wxRESIZE_BORDER | wxSYSTEM_MENU | wxCLOSE_BOX
#define SYMBOL_TOCWINDOW_DLG_TITLE _("Table of content")
#define SYMBOL_TOCWINDOW_DLG_IDNAME ID_TOCWINDOW_DLG
#define SYMBOL_TOCWINDOW_DLG_SIZE wxSize(220, 150)
#define SYMBOL_TOCWINDOW_DLG_POSITION wxDefaultPosition

class TocWindowContent : public wxEvtHandler {
  private:
    DECLARE_DYNAMIC_CLASS(TocWindowContent);

    TocCtrl* m_TocCtrl;

  public:
    TocWindowContent();

    ~TocWindowContent();

    /// Creates the controls and sizers
    wxSizer* CreateControls(wxWindow* parent, bool call_fit = TRUE, bool set_sizer = TRUE);

    virtual void Show() {
        ;
    }

    virtual void Hide() {
        ;
    }

    virtual bool IsShown() {
        return FALSE;
    }

    TocCtrl* GetTocCtrl() {
        return m_TocCtrl;
    }
};

class TocWindowDlgGen : public TocWindowContent {
  private:
    DECLARE_DYNAMIC_CLASS(TocWindowDlgGen);

    void Init();

    wxAuiManager* m_TocAui;
    wxPanel* m_ContentFrame;
    wxWindow* m_ParentEvt;

    // DECLARE_EVENT_TABLE();

  public:
    TocWindowDlgGen();

    TocWindowDlgGen(wxAuiManager* myAuiManager, wxWindow* parent, wxWindowID id = ID_TOCWINDOW_DLG,
                    const wxString& title = SYMBOL_TOCWINDOW_DLG_TITLE);

    ~TocWindowDlgGen();

    virtual void Show();

    virtual void Hide();

    virtual bool IsShown();

    /*void OnPressRemoveLayers(wxCommandEvent & event);
    void OnPressAddLayers(wxCommandEvent & event);*/
};

#endif
