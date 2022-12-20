/***************************************************************************
 tmwindowposition.cpp
 Save and load windows position
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

#include "tmwindowposition.h"

tmWindowPosition::tmWindowPosition() {
    WP_LoadScreenSize();
}

tmWindowPosition::~tmWindowPosition() {}

bool tmWindowPosition::SaveScreenPosition() {
    return WP_SaveScreenSize();
}

bool tmWindowPosition::LoadPosition(const wxString& wndname, wxRect& pos) {
    wxString readedpos = wxEmptyString;
    if (!LoadPosition(wndname, readedpos)) return false;

    pos = WP_StringToPosition(readedpos);

    wxRect myActualSize = WP_GetActualScreenSize();
    if (wndname != _T("SCREEN") && HasScreenChanged()) {
        if (!Intersects(pos, wxSize(myActualSize.GetWidth(), myActualSize.GetHeight()))) {
            wxLogDebug(_T("Windows %s was outside screen"), wndname.c_str());
            return false;
        }
    }
    return true;
}

bool tmWindowPosition::LoadPosition(const wxString& wndname, wxString& postext) {
    wxConfigBase* pConfig = wxFileConfig::Get();
    wxASSERT(pConfig);
    if (!pConfig->Read("WINDOW_POSITION/" + wndname, &postext)) {
        wxLogMessage(_("No position stored for windows %s"), wndname.c_str());
        return false;
    }
    return true;
}

wxString tmWindowPosition::WP_PositionToString(wxRect pos) {
    wxString mySPos = wxString::Format(_T("%d;%d;%d;%d"), pos.GetX(), pos.GetY(), pos.GetWidth(), pos.GetHeight());
    return mySPos;
}

wxRect tmWindowPosition::WP_StringToPosition(const wxString& posstring) {
    wxStringTokenizer tokenizer(posstring, _T(";"));
    wxASSERT(tokenizer.CountTokens() == 4);

    long myTemp[] = {wxNOT_FOUND, wxNOT_FOUND, wxNOT_FOUND, wxNOT_FOUND};
    wxRect myRect;

    for (int i = 0; i < 4; i++) {
        tokenizer.GetNextToken().ToLong(&(myTemp[i]));
    }

    myRect.SetX(myTemp[0]);
    myRect.SetY(myTemp[1]);
    myRect.SetWidth(myTemp[2]);
    myRect.SetHeight(myTemp[3]);
    return myRect;
}

bool tmWindowPosition::SavePosition(const wxString& wndname, wxRect pos) {
    return SavePosition(wndname, WP_PositionToString(pos));
}

bool tmWindowPosition::SavePosition(const wxString& wndname, const wxString& postext) {
    wxString myStringPosition = wxEmptyString;
    wxConfigBase* pConfig = wxFileConfig::Get();
    wxASSERT(pConfig);
    if (!pConfig->Write("WINDOW_POSITION/" + wndname, postext)) {
        wxLogDebug(_T("Unable to write to config file"));
        return false;
    }
    return true;
}

bool tmWindowPosition::WP_SaveScreenSize() {
    wxRect myWndSize = WP_GetActualScreenSize();
    return SavePosition(_T("SCREEN"), myWndSize);
}

bool tmWindowPosition::WP_LoadScreenSize() {
    wxRect myWndSize = WP_GetActualScreenSize();
    if (!LoadPosition(_T("SCREEN"), myWndSize)) {
        m_OldScreenSize = wxSize(myWndSize.GetWidth(), myWndSize.GetHeight());
        return false;
    }

    m_OldScreenSize = wxSize(myWndSize.GetWidth(), myWndSize.GetHeight());
    return true;
}

bool tmWindowPosition::HasScreenChanged() {
    wxRect myActualSize = WP_GetActualScreenSize();
    if (m_OldScreenSize != wxSize(myActualSize.GetWidth(), myActualSize.GetHeight())) {
        wxLogDebug(_T("Screen size has changed"));
        return true;
    }

    return false;
}

wxRect tmWindowPosition::WP_GetActualScreenSize() {
    return wxRect(0, 0, wxSystemSettings::GetMetric(wxSYS_SCREEN_X, nullptr),
                  wxSystemSettings::GetMetric(wxSYS_SCREEN_Y, nullptr));
}

bool tmWindowPosition::Intersects(wxRect wndpos, wxSize screensize) {
    wxRect myScreenSize = wxRect(wxPoint(0, 0), screensize);
    return myScreenSize.Contains(wndpos);
}
