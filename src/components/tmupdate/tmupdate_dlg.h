/***************************************************************************
								tmupdate_dlg.h
						Display dialog for web update
                             -------------------
    copyright            : (C) 2009 CREALP Lucien Schreiber 
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


#ifndef _TM_UPDATE_DLG_H_
#define _TM_UPDATE_DLG_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif


#include <wx/hyperlink.h>
#include <wx/statline.h>
#include "tmupdate.h"


#define ID_DLGUP_DOWNLOAD 22300
#define ID_DLGUP_OPTIONS1 22301
#define ID_DLGUP_OPT_STARTUP 22302
#define ID_DLGUP_OPT_PROXY 22303
//#define ID_DLGUP_CLOSE 22304
//#define ID_DLGUP_OPTIONS2 22305

///////////////////////////////////////////////////////////////////////////////
/// Class tmUpdate_DLG
///////////////////////////////////////////////////////////////////////////////
class tmUpdate_DLG : public wxDialog 
	{
	private:
		// ctrls
		wxBoxSizer* m_MainSizer;
		wxPanel* tmUpdate_Panel_Sucess;
		wxStaticText* m_TextVersion;
		wxHyperlinkCtrl* m_ChangesHyperlink;
		wxPanel* tmUpdate_Panel_Option;
		wxCheckBox* m_UpdateCheckbox;
		wxCheckBox* m_ProxyCheckbox;
		wxStaticText* m_ProxyText1;
		wxTextCtrl* m_ProxyTextCtrl;
		wxStaticText* m_ProxyText2;
		wxPanel* tmUpdate_Panel_Failure;
		wxStaticText* m_ErrorText;
		wxButton* m_OptionBtn1;
		wxButton * m_OptionBtn2;
		wxButton* m_DownLoadBtn;
		
		void UPDLGUpdateLayout();
		tmUpdate * m_Update;
		void UPDLGCheckProxy (bool bEnable = true);
		
		// event function
		void OnShowOptions (wxCommandEvent & event);
		void OnShowProxy (wxCommandEvent & event);
		void OnButtonClose (wxCommandEvent & event);
		
				
		DECLARE_EVENT_TABLE();
		
		
	public:
		tmUpdate_DLG(wxWindow* parent, wxWindowID id = wxID_ANY, 
					 const wxString& title = _("Updates"), 
					 const wxPoint& pos = wxDefaultPosition,
					 const wxSize& size = wxDefaultSize,
					 long style = wxDEFAULT_DIALOG_STYLE); //|wxRESIZE_BORDER );
		~tmUpdate_DLG();
		
		void SetNoConnection();
		void SetNoNewVersion();
		void SetNewVersion(tmUpdate * pupdate);

		
	};





#endif
