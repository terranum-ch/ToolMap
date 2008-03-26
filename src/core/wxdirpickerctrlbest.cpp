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

#include "wxdirpickerctrlbest.h"





wxDirPickerCtrlBest::wxDirPickerCtrlBest(wxWindow * parent, wxWindowID id, const wxString & path,
					const wxString & message,
					const wxPoint & pos , const wxSize & size, 
										 long style) : wxDirPickerCtrl (parent, id, path,message, pos,size,style)
{
	
}


wxDirPickerCtrlBest::~wxDirPickerCtrlBest()
{
	
	
}

