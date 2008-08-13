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


const int ID_TMSLIDER_SLIDER = 10003;
const int ID_TMSLIDER_TEXT =	10002;
#define SYMBOL_TMSLIDERWITHTEXT_STYLE wxTAB_TRAVERSAL


class tmSliderWithText: public wxPanel
	{  
	private:
		wxSlider* m_Slider;
		wxTextCtrl* m_Text;
		wxStaticText * m_UnitValue;
		
		int m_iLimitMin;
		int m_iLimitMax;
		
		void Init();
		void CreateControls(int ivalue, int imin,int imax,
							const wxString & unitvalue);
		
		// event function
		void OnTextUpdated( wxCommandEvent & event);
		void OnSliderUpdated (wxScrollEvent & event);
		
		DECLARE_DYNAMIC_CLASS( tmSliderWithText )
		
	protected:
		int SetSliderValue (int iNewValue);
		
	public:
		
		tmSliderWithText();
		tmSliderWithText( wxWindow* parent, wxWindowID id,
						 int ivalue, int imin, int imax,
						 const wxString & unitvalue = _T(""),
						 const wxPoint& pos = wxDefaultPosition,
						 const wxSize& size = wxDefaultSize, 
						 long style = SYMBOL_TMSLIDERWITHTEXT_STYLE );
		
		bool Create(  wxWindow* parent, wxWindowID id ,
					int ivalue, int imin,int imax,
					const wxString & unitvalue = _T(""),
					const wxPoint& pos = wxDefaultPosition,
					const wxSize& size = wxDefaultSize, 
					long style = SYMBOL_TMSLIDERWITHTEXT_STYLE);		
		~tmSliderWithText();
		
		int GetValue();
		bool SetValue(int iValue);
	};



#endif
