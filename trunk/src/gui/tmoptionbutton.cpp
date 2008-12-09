/***************************************************************************
								name.cpp
                    Display a button calling a contextual menu
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

#include "tmoptionbutton.h"

// include option bitmap
#include "../img/tmimgfunc.h"	// for image processing
#include "../img/img_option_button.cpp"


tmOptionButton::tmOptionButton()
{
	InitMemberValues();
}


tmOptionButton::tmOptionButton(wxWindow * parent, wxWindowID id, wxMenu * menu)
{
	InitMemberValues();
	Create(parent, id, menu);
}


bool tmOptionButton::Create(wxWindow * parent, wxWindowID id, wxMenu * menu)
{
	wxASSERT(menu);
	m_Menu = menu;
	wxBitmap optionbitmap (wxGetBitmapFromMemory(option_button));
	wxBitmapButton::Create(parent, id, optionbitmap);
	
	Connect(id, wxEVT_COMMAND_BUTTON_CLICKED,
			wxCommandEventHandler(tmOptionButton::OnPressButton));
	
	
	return true;
}


tmOptionButton::~tmOptionButton()
{
	
}


void tmOptionButton::InitMemberValues()
{
	m_Menu = NULL;
}


void tmOptionButton::OnPressButton(wxCommandEvent & event)
{
	wxASSERT(m_Menu);
	PopupMenu(m_Menu);
	event.Skip();
}
