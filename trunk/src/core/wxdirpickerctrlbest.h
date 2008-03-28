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


// conditional style different for unix...
#if defined(__WXMSW__) || defined(__WXMAC__)
const long PICKER_BEST_STYLE = wxDIRP_DIR_MUST_EXIST | wxDIRP_USE_TEXTCTRL;
#else
const long PICKER_BEST_STYLE = wxDIRP_DEFAULT_STYLE;	
#endif


// enumeration for path
/***************************************************************************//**
 @brief Enumeration for path
 @details Describe the type of path, mainly used as a return value
 @author Lucien Schreiber (c) CREALP 2007
 @date 28 March 2008
 *******************************************************************************/
enum PATH_ERROR
{
	PATH_OK		= 0, ///<  No errors where detected with the path
	PATH_EMPTY =  1, ///<  Path stored into the database is empty
	PATH_INVALID = 2, ///<  The returned path isn't valid (for exemple because of different OS)
	PATH_DATABASE_ERROR = 4 ///< Error retrieving the path from the database
};


class wxDirPickerCtrlBest : public wxDirPickerCtrl
	{
	private:

	public:
		wxDirPickerCtrlBest(wxWindow * parent, wxWindowID id, const wxString & path = wxEmptyString,
							const wxString & message = _("Select a folder"),
							const wxPoint & pos = wxDefaultPosition, const wxSize & size = wxDefaultSize, 
							long style = PICKER_BEST_STYLE);
		~wxDirPickerCtrlBest();
		
		void SetPathWithError(PATH_ERROR flags, const wxString & spath);
	};






#endif
