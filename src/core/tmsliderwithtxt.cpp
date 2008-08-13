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
	m_Unit		= NULL;
	
	m_limitmin = 0;
	m_limitmax = 100;
}



tmSliderWithTxt::tmSliderWithTxt(wxWindow * parent, 
								 wxWindowID idslider,
								 wxWindowID idtext,
								 int min, int max, int value,
								 const wxString & unit,
								 const wxPoint & position,
								 const wxSize & size) 
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
	m_limitmin = min;
	m_limitmax = max;
	
	
	wxPanel::Create(parent, wxID_ANY, position, size);
	
	// create sizer
	wxBoxSizer * mysizer = new wxBoxSizer(wxHORIZONTAL);
	SetSizer(mysizer);
	
	
	// create controls
    m_Slider = new wxSlider( this, idslider, value, min, max, position, size, wxSL_HORIZONTAL);
    mysizer->Add(m_Slider, 1, wxGROW|wxALL, 5);
	
	m_Text = new wxTextCtrl( this, idtext, wxString::Format(_T("%d"), value),
							wxDefaultPosition, wxDefaultSize, 0 ); 
	mysizer->Add(m_Text, 0, wxGROW|wxALL, 5);
	
	m_Unit = new wxStaticText(this, wxID_ANY, unit);
	mysizer->Add(m_Unit, 0, wxALL, 5);
	
	
	
	// connect event
	m_Slider->Connect(idslider, wxEVT_SCROLL_CHANGED, wxScrollEventHandler(tmSliderWithTxt::OnSliderChange));
	m_Text->Connect(idtext,  wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(tmSliderWithTxt::OnTextChange));
	
}



tmSliderWithTxt::~tmSliderWithTxt()
{
	if (m_Slider)
		delete m_Slider;
	
	if (m_Text)
		delete m_Text;
	
}



void tmSliderWithTxt::OnSliderChange (wxScrollEvent & event)
{
	m_Text->SetLabel(wxString::Format(_T("%d"), event.GetInt()));
	wxLogDebug(_T("value changed"));
}


void tmSliderWithTxt::OnTextChange (wxCommandEvent & event)
{
	int iActualValue = 0;
	iActualValue = wxAtoi(event.GetString());
	
	/*if (iActualValue <= m_limitmin && iActualValue >= m_limitmax)
	{	
		wxLogDebug(_T("value : %d is out of the limits : %d - %d"),
				   iActualValue, m_limitmin, m_limitmax);
		iActualValue = m_limitmin;
		m_Text->SetLabel(wxString::Format(_T("%d"), iActualValue));
	}
	
	m_Slider->SetValue(iActualValue);*/
	SetSliderValue(iActualValue);
	
}


int tmSliderWithTxt::SetSliderValue (int inewValue)
{
	wxASSERT(m_Slider);
	
	if(inewValue <= m_limitmin && inewValue >= m_limitmax)
	{
		wxLogDebug(_T("value : %d is out of the limits : %d - %d"),
				   inewValue, m_limitmin, m_limitmax);
		return m_Slider->GetValue();
	}
	
	m_Slider->SetValue(inewValue);
	return inewValue;
}

