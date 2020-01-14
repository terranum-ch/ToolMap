/***************************************************************************
 tmcrash_dlg.h
 Display crash dialog
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


#ifndef _TM_CRASH_DLG_H_
#define _TM_CRASH_DLG_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif


#include <wx/statline.h>

class tmCrash_DLG : public wxDialog
{
private:
    wxTextCtrl *m_CrashFile;

    void CreateControls(wxWindow *parent);

protected:

public:
    tmCrash_DLG(wxWindow *parent, const wxString &crashfilename,
                wxWindowID id = wxID_ANY, const wxString &title = _("Fatal exception"),
                const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize,
                long style = wxDEFAULT_DIALOG_STYLE);

    ~tmCrash_DLG();

};


#endif
