/***************************************************************************
 lsversion_dlg.h
 
 -------------------
 copyright            : (C) 2010 CREALP Lucien Schreiber 
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

#ifndef _LSVERSION_DLG_H
#define _LSVERSION_DLG_H

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"
// Include wxWidgets' headers
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif



class lsVersionDlg : public wxDialog {
private:
	wxStaticBitmap* m_BmpCtrl;
	wxStaticText* m_TitleCtrl;
	wxStaticText* m_ProgNameCtrl;
	wxTextCtrl* m_ModulesCtrl;
	wxStaticText* m_CopyRightCtrl;
	
	void _CreateControls();
	
public:
    lsVersionDlg(wxWindow * parent, wxWindowID id, const wxString & title,
				 const wxPoint & pos = wxDefaultPosition,
				 const wxSize & size = wxDefaultSize,
				 long style = wxDEFAULT_DIALOG_STYLE);
    virtual ~lsVersionDlg();
	
    void SetBitmapLogo(const wxBitmap & bmp);
    void SetTitleAndCaption(wxString title);
    void SetSoftName(wxString name);
    void SetModulesName(wxString name);
    void SetCopyright(wxString name);
	
	virtual int ShowModal();
};
#endif

