/***************************************************************************
 tmabout_dlg.h
 about dialog (inspired by firefox about)
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

#ifndef _TM_ABOUT_DLG_H_
#define _TM_ABOUT_DLG_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

// Include wxWidgets' headers
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#define ID_ABOUT_BTN 22420

#include <wx/html/htmlwin.h>
#include <wx/hyperlink.h>
// TODO: Change this for using wxGenericStaticBitmap (2.9.0)
// #include <wx/generic/statbmpg.h> // for big static bitmap

const wxString g_CodeName = "CHIASSO";

class tmAboutDLG : public wxDialog {
  private:
    wxStaticText* m_TextVersion;
    wxPanel* m_PanelLicence;
    wxPanel* m_PanelAuthor;
    wxButton* m_Button;

    void CreateControls(wxWindow* parent);

    wxString GetVersionText();

    wxString GetComponentsVersion();

    wxString GetButtonText();

    void OnButton(wxCommandEvent& event);

    DECLARE_EVENT_TABLE();

  protected:
  public:
    tmAboutDLG(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString,
               const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
               long style = wxDEFAULT_DIALOG_STYLE);

    ~tmAboutDLG();
};

#endif
