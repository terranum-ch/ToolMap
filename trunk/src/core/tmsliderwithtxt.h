/***************************************************************************
								tmsliderwithtxt.h
                    Display a slider with associed text control
				Updating value in one control reflect on the other
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


#ifndef _TM_SLIDERWITHTXT_H_
#define _TM_SLIDERWITHTXT_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

//#include <wx/control.h>
#include <wx/slider.h>

class tmSliderWithTxt : public wxBoxSizer
	{
	private:
		wxSlider * m_Slider;
		wxTextCtrl * m_Text;
		wxStaticText * m_Unit;
		
		void InitMemberValues();
		
		// event functions
		void OnSliderChange (wxScrollEvent & event);
		void OnTextChange (wxCommandEvent & event);
		
	protected:
	public:
		tmSliderWithTxt();
		tmSliderWithTxt(wxWindow * parent, 
						wxWindowID idslider,
						wxWindowID idtext,
						int min, int max, int value,
						const wxString & unit = _T(""),
						const wxPoint & position = wxDefaultPosition,
						const wxSize & size = wxDefaultSize);		
		void Create (wxWindow * parent, 
					 wxWindowID idslider,
					 wxWindowID idtext,
					 int min, int max, int value,
					 const wxString & unit = _T(""),
					 const wxPoint & position = wxDefaultPosition,
					 const wxSize & size = wxDefaultSize);
		~tmSliderWithTxt();
				
};








#endif
