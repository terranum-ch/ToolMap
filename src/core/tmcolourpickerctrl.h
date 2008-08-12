/***************************************************************************
								tmcolourpickerctrl.h
                    Create a nice colour picker even on mac
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


#ifndef _TM_COLOURPICKERCTRL_H_
#define _TM_COLOURPICKERCTRL_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

//#include <wx/clrpicker.h>

#if (__WXMAC__) // SPECIFIC CODE FOR MAC

#include <wx/colordlg.h>

const wxSize tmCOLOURPICKERCTRL_SIZE (15,15);

class tmColourPickerCtrl : public wxBitmapButton
	{
	private:
		wxColour m_Colour;
		void InitMemberValues();
		wxBitmap CreateColourBitmap (const wxColour & col);
		
		// event function
		void OnChooseColour (wxCommandEvent & event);
				
	protected:
	public:
		tmColourPickerCtrl();
		tmColourPickerCtrl(wxWindow * parent, wxWindowID id,
						   const wxColour& colour = *wxBLACK,
						   const wxPoint & pos = wxDefaultPosition,
						   const wxSize & size = wxDefaultSize);
		~tmColourPickerCtrl();
		
		
		wxColour GetColour(){return m_Colour;}
		void SetColour(const wxColour & col);
		
		
		
	};


#else // UNDER WINDOWS AND LINUX, USE wxColourPickerCtrl

#include <wx/clrpicker.h>

class tmColourPickerCtrl : public wxColourPickerCtrl
	{
	private:
	protected:
	public:
		tmColourPickerCtrl(){;}
		tmColourPickerCtrl(wxWindow * parent, wxWindowID id,
						   const wxColour& colour = *wxBLACK,
						   const wxPoint & pos = wxDefaultPosition,
						   const wxSize & size = wxDefaultSize);
		~tmColourPickerCtrl(){;}
	};


#endif

#endif
