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

#include <wx/valgen.h>



class DataBaseTM;
class tmOpenError_DLG : public wxDialog 
{
private:
	wxPanel* m_ErrPanelDB;
	wxStaticText* m_ErrDBTextCtrl;
	wxTextCtrl* m_ErrDBLogCtrl;
	wxPanel* m_ErrPanelProjVersion;
	wxStaticText* m_ErrVerTextCtrl;
	wxButton* m_ErrVerBackupConvertCtrl;
	wxButton* m_ErrVerConvertCtrl;
	wxPanel* m_ErrPanelTMVersion;
	wxStaticText* m_ErrTMTextCtrl;
	wxButton* m_ErrTMDownloadCtrl;
	
	DataBaseTM * m_pDB;
	
	void _CreateControls(const wxString & projname, int iActDBVersion);
	
	void OnConvertBackup( wxCommandEvent& event );
	void OnConvert( wxCommandEvent& event );
	void OnDownload( wxCommandEvent& event );
	
	
public:
	tmOpenError_DLG( wxWindow* parent, 
					int iActDBVersion,
					const wxString& projname,
					DataBaseTM * database,
					wxWindowID id = wxID_ANY,
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
