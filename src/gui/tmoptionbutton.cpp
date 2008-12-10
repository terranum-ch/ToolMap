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


/***************************************************************************//**
 @brief Constructor
 @details This is the two step constructor, you need to call
 tmOptionButton::Create() in order to create correctly the tmOptionButton
 @author Lucien Schreiber (c) CREALP 2008
 @date 10 December 2008
 *******************************************************************************/
tmOptionButton::tmOptionButton()
{
	InitMemberValues();
}



/***************************************************************************//**
 @brief Constructor
 @details This is the full constructor
 @param parent Adress of a valid parent
 @param id ID of the control
 @param menu Attached menu. It will be automatically displayed when button is
 pressed. The menu must be deleted by owner, when not more needed
 @author Lucien Schreiber (c) CREALP 2008
 @date 10 December 2008
 *******************************************************************************/
tmOptionButton::tmOptionButton(wxWindow * parent, wxWindowID id, wxMenu * menu)
{
	InitMemberValues();
	Create(parent, id, menu);
}



/***************************************************************************//**
 @brief Two step contructor
 @details When using the default constructor, you need to call this function in
 order to init correctly the control
 @param parent Adress of a valid parent
 @param id ID of the control
 @param menu Attached menu. It will be automatically displayed when button is
 pressed. The menu must be deleted by owner, when not more needed
 @return Allways true
 @author Lucien Schreiber (c) CREALP 2008
 @date 10 December 2008
 *******************************************************************************/
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



/***************************************************************************//**
 @brief Destructor
 @details Do nothing actually
 @author Lucien Schreiber (c) CREALP 2008
 @date 10 December 2008
 *******************************************************************************/
tmOptionButton::~tmOptionButton()
{
	
}



/***************************************************************************//**
 @brief Init default values
 @details To member data
 @author Lucien Schreiber (c) CREALP 2008
 @date 10 December 2008
 *******************************************************************************/
void tmOptionButton::InitMemberValues()
{
	m_Menu = NULL;
}



/***************************************************************************//**
 @brief Respond to button pressed
 @details This funciton display the attached menu when button is pressed
 @author Lucien Schreiber (c) CREALP 2008
 @date 10 December 2008
 *******************************************************************************/
void tmOptionButton::OnPressButton(wxCommandEvent & event)
{
	wxASSERT(m_Menu);
	PopupMenu(m_Menu);
	event.Skip();
}

