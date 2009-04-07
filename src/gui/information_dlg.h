/***************************************************************************
								information_dlg.h
                    Display informations dialog for layers and features
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


#ifndef _INFORMATION_DLG_H_
#define _INFORMATION_DLG_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif


#include <wx/html/htmlwin.h>
#include <wx/notebook.h>


class InformationDLG : public wxDialog
	{
	private:
		// controls
		wxNotebook* m_notebook;
		wxHtmlWindow* m_LayerInfo;
		wxButton* m_CloseBtn;
		
		void CreateControls();
		void InitMembersValue();
		
	protected:
	public:
		InformationDLG( wxWindow* parent, wxWindowID id = wxID_ANY,
						const wxString& title = _("Informations"),
						const wxPoint& pos = wxDefaultPosition,
						const wxSize& size = wxDefaultSize,
						long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER|wxSTAY_ON_TOP );
		~InformationDLG();
		void SetMetaData (const wxString & metadata);

	};





#endif
