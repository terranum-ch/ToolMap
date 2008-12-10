/***************************************************************************
								tmoptionbutton.h
                    Display A button calling a contextual menu
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

/*!
 \page tmOptionButton "Option Button"
 
 For better space utilisation and based on standard controls in Mac OS sdk, we have
 developped the tmOptionButton, which is basically a Bitmap button displaying a menu.
 See picture bellow for an outline of tmOptionButton.
 
 \image html tmoptionbutton.png
 
 */



#ifndef _TM_OPTION_BUTTON_H_
#define _TM_OPTION_BUTTON_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif


/***************************************************************************//**
 @brief Option button
 @details This class display an option button (based on wxBitmapButton) and when
 clicked, it shows the attached menu
 @author Lucien Schreiber (c) CREALP 2008
 @date 10 December 2008
 *******************************************************************************/
class tmOptionButton : public wxBitmapButton
	{
	private:
		wxMenu * m_Menu;
		
		void InitMemberValues();
		
		void OnPressButton(wxCommandEvent & event);
		
	protected:
	public:
		tmOptionButton();
		tmOptionButton(wxWindow * parent, wxWindowID id, wxMenu * menu);
		bool Create(wxWindow * parent, wxWindowID id, wxMenu * menu);
		~tmOptionButton();
};




#endif
