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

// comment doxygen


#ifndef _TM_OPTION_BUTTON_H_
#define _TM_OPTION_BUTTON_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif


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
