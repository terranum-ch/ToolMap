/***************************************************************************
								tmsliderwithtxt.cpp
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

#include "tmsliderwithtxt.h"



tmSliderWithTxt::tmSliderWithTxt()
{
	InitMemberValues();
}



void tmSliderWithTxt::InitMemberValues()
{
	m_Slider	= NULL;
	m_Text		= NULL;
}



tmSliderWithTxt::tmSliderWithTxt(wxWindow * parent, wxWindowID id, int min, 
								 int max, int value, const wxPoint & position,
								 const  wxSize & size)
{
	InitMemberValues();
	Create(parent, id,min, max, value, position, size);
}



void tmSliderWithTxt::Create (wxWindow * parent, wxWindowID id, int min, 
							  int max, int value, const  wxPoint  & position,
							  const wxSize & size)
{
	wxSlider::Create(parent, id, value, min, max, position, size);
	/* create sizer and controls
	wxBoxSizer* sizerbox = new wxBoxSizer(wxHORIZONTAL);
	
    m_Slider = new wxSlider( parent, id, value, min, max, position, size, wxSL_HORIZONTAL);
    sizerbox->Add(m_Slider, 1, wxGROW|wxALL, 5);
	
	
	
	//m_Text = new wxTextCtrl( parent, id+1, wxString::Format(_T("%d"), value),
	//						wxDefaultPosition, wxDefaultSize, 0 );
    //sizerbox->Add(m_Text, 0, wxGROW|wxALL, 5);*/
	
}



tmSliderWithTxt::~tmSliderWithTxt()
{
	if (m_Slider)
		delete m_Slider;
	
	if (m_Text)
		delete m_Text;
	
}


