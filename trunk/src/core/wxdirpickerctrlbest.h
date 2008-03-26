/***************************************************************************
							wxdirpickerctrlbest.h
                    Upgrade a wxDirPickerCtrl control for TM
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


#ifndef _WX_DIRPICKERCTRL_BEST_
#define _WX_DIRPICKERCTRL_BEST_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif


#include <wx/filepicker.h>


#ifdef __WXMSW__ || __WXMAC__
const long PICKER_BEST_STYLE = wxDIRP_DIR_MUST_EXIST | wxDIRP_USE_TEXTCTRL;
#else
const long PICKER_BEST_STYLE = wxDIRP_DEFAULT_STYLE;	
#endif


class wxDirPickerCtrlBest : public wxDirPickerCtrl
	{
	private:

	public:
		wxDirPickerCtrlBest(wxWindow * parent, wxWindowID id, const wxString & path = wxEmptyString,
							const wxString & message = _("Select a folder"),
							const wxPoint & pos = wxDefaultPosition, const wxSize & size = wxDefaultSize, 
							long style = PICKER_BEST_STYLE);
		~wxDirPickerCtrlBest();
	};






#endif
