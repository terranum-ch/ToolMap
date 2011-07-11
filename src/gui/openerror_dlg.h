/***************************************************************************
 openerror_dlg.h
 Display the error message dialog
 -------------------
 copyright            : (C) 2008 CREALP Lucien Schreiber 
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

#ifndef __OPEN_ERROR_DLG_H__
#define __OPEN_ERROR_DLG_H__

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/statline.h>
#include <wx/artprov.h>
#include <wx/valgen.h>


#define ID_OPENERROR_DLG 1000
#define ID_OPENERROR_BMP 1001

enum tmPROJECT_OPEN_ERROR
{
	OPEN_DB_FAILED = 0,
	OPEN_OK = 1,
	OPEN_NOT_TM_DB = 2,
	OPEN_NOT_GOOD_VERSION = 100
};


class tmOpenError_DLG : public wxDialog 
{
	protected:
		wxStaticBitmap* m_dlgbmp;
		wxStaticText* m_ErrorTxt;
		wxButton* m_OkBtn;
	
	public:
		tmOpenError_DLG( wxWindow* parent, 
					 int iActDBVersion,
					 const wxString& title,
					 wxWindowID id = ID_OPENERROR_DLG,
					 const wxPoint& pos = wxDefaultPosition,
					 const wxSize& size = wxDefaultSize,
					 long style = wxDEFAULT_DIALOG_STYLE);
		~tmOpenError_DLG();
	
};




class tmOpenRecentError_DLG : public wxDialog {
private:
	wxStaticText* m_ErrorMsgCtrl;
	wxCheckBox* m_RemoveCheckCtrl;
	bool m_RemoveCheck;
	
    void _CreateControls(const wxString & prjname);
	
public:
    tmOpenRecentError_DLG(wxWindow * parent, wxWindowID id, 
						  const wxString & title,
						  const wxString & prjname,
						  const wxPoint & pos = wxDefaultPosition,
						  const wxSize & size = wxDefaultSize,
						  long style = wxDEFAULT_DIALOG_STYLE);
    virtual ~tmOpenRecentError_DLG();

	inline const bool GetRemoveFromRecent() const;
};



inline const bool tmOpenRecentError_DLG::GetRemoveFromRecent() const {
	return m_RemoveCheck;
}

#endif
