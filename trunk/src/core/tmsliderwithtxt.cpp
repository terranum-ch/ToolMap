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



tmSliderWithTxt::tmSliderWithTxt() : wxBoxSizer(wxHORIZONTAL)
{
	InitMemberValues();
}



void tmSliderWithTxt::InitMemberValues()
{
	m_Slider	= NULL;
	m_Text		= NULL;
	m_Unit		= NULL;
}



tmSliderWithTxt::tmSliderWithTxt(wxWindow * parent, 
								 wxWindowID idslider,
								 wxWindowID idtext,
								 int min, int max, int value,
								 const wxString & unit,
								 const wxPoint & position,
								 const wxSize & size) : wxBoxSizer(wxHORIZONTAL)
{
	InitMemberValues();
	Create(parent, idslider, idtext ,min, max, value, unit, position, size);
}



void tmSliderWithTxt::Create (wxWindow * parent, 
							  wxWindowID idslider,
							  wxWindowID idtext,
							  int min, int max, int value,
							  const wxString & unit,
							  const wxPoint & position,
							  const wxSize & size)
{
	
	// create controls
    m_Slider = new wxSlider( parent, idslider, value, min, max, position, size, wxSL_HORIZONTAL);
    Add(m_Slider, 1, wxGROW|wxALL, 5);
	
	m_Text = new wxTextCtrl( parent, idtext, wxString::Format(_T("%d"), value),
							wxDefaultPosition, wxDefaultSize, 0 ); 
	Add(m_Text, 0, wxGROW|wxALL, 5);
	
	m_Unit = new wxStaticText(parent, wxID_ANY, unit);
	Add(m_Unit, 0, wxALL, 5);
	
	// connect event
	//m_Slider->Connect(idslider, wxEVT_SCROLL_CHANGED, wxScrollEventHandler(tmSliderWithTxt::OnSliderChange));
	
}



tmSliderWithTxt::~tmSliderWithTxt()
{
	if (m_Slider)
		delete m_Slider;
	
	if (m_Text)
		delete m_Text;
	
}


