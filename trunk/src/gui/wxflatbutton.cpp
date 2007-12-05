/***************************************************************************
								wxflatbutton.cpp
                    Creation of a flat button on every plateform
					uses wxButton flat or wxToggleButton (on mac)
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

#include "wxflatbutton.h"


/******************************  wxFlatButton (for mac) *************************/


#if (__WXMAC__)
wxFlatButton::wxFlatButton()
{
	InitwxFlatButton();
}


wxFlatButton::wxFlatButton(wxWindow * parent, wxWindowID id, const wxString & label)
: wxToggleButton(parent,id,label,wxDefaultPosition, wxFLATBUTTON_SIZE)
{
	InitwxFlatButton();
	
}


wxFlatButton::~wxFlatButton()
{
}


IMPLEMENT_DYNAMIC_CLASS( wxFlatButton, wxToggleButton)

void wxFlatButton::InitwxFlatButton()
{
	
}




/******************************  wxFlatButton (win and linux)**********************/
#else 
wxFlatButton::wxFlatButton()
{
	InitwxFlatButton();
}


wxFlatButton::wxFlatButton(wxWindow * parent, wxWindowID id, const wxString & label)
: wxButton(parent,id,label,wxDefaultPosition, wxFLATBUTTON_SIZE, wxFLATBUTTON_STYLE )
{
	InitwxFlatButton();
	
}


wxFlatButton::~wxFlatButton()
{
}


IMPLEMENT_DYNAMIC_CLASS( wxFlatButton, wxButton)

void wxFlatButton::InitwxFlatButton()
{
	
}


#endif
