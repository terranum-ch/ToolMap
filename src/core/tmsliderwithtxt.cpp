/***************************************************************************
 tmsliderwithtxt.cpp
                    Display a slider with associed text control
 Updating value in one control reflect on the other
 -------------------
 copyright : (C) 2007 CREALP Lucien Schreiber
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


IMPLEMENT_DYNAMIC_CLASS(tmSliderWithText, wxPanel)


tmSliderWithText::tmSliderWithText()
{
    Init();
}


tmSliderWithText::tmSliderWithText(wxWindow *parent, wxWindowID id,
                                   int ivalue, int imin, int imax,
                                   const wxString &unitvalue,
                                   const wxPoint &pos,
                                   const wxSize &size, long style)
{
    Init();
    Create(parent, id, ivalue, imin, imax, unitvalue, pos, size, style);
}


bool tmSliderWithText::Create(wxWindow *parent, wxWindowID id,
                              int ivalue, int imin, int imax,
                              const wxString &unitvalue,
                              const wxPoint &pos,
                              const wxSize &size, long style)
{
    wxPanel::Create(parent, id, pos, size, style);

    m_iLimitMin = imin;
    m_iLimitMax = imax;

    CreateControls(ivalue, imin, imax, unitvalue);

    // connect event
    m_Text->Bind(wxEVT_COMMAND_TEXT_UPDATED, &tmSliderWithText::OnTextUpdated, this);
    m_Slider->Bind(wxEVT_SCROLL_THUMBTRACK, &tmSliderWithText::OnSliderUpdated, this);

    if (GetSizer()) {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
    return true;
}


tmSliderWithText::~tmSliderWithText()
{
    m_Text->Unbind(wxEVT_COMMAND_TEXT_UPDATED, &tmSliderWithText::OnTextUpdated, this);
    m_Slider->Unbind(wxEVT_SCROLL_THUMBTRACK, &tmSliderWithText::OnSliderUpdated, this);

}


void tmSliderWithText::Init()
{
    m_Slider = NULL;
    m_Text = NULL;
    m_UnitValue = NULL;

    m_iLimitMin = 0;
    m_iLimitMax = 100;
}


void tmSliderWithText::CreateControls(int ivalue, int imin, int imax,
                                      const wxString &unitvalue)
{
    tmSliderWithText *itemPanel1 = this;

    wxBoxSizer *itemBoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
    itemPanel1->SetSizer(itemBoxSizer2);

    m_Slider = new wxSlider(itemPanel1, wxID_ANY, ivalue, imin, imax,
                            wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL);
    itemBoxSizer2->Add(m_Slider, 1, wxGROW | wxALL, 5);

    m_Text = new wxTextCtrl(itemPanel1, wxID_ANY, wxString::Format(_T("%d"), ivalue),
                            wxDefaultPosition, wxDefaultSize, 0);
    itemBoxSizer2->Add(m_Text, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

    m_UnitValue = new wxStaticText(itemPanel1, wxID_ANY, unitvalue,
                                   wxDefaultPosition, wxDefaultSize, 0);
    itemBoxSizer2->Add(m_UnitValue, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

}


int tmSliderWithText::SetSliderValue(int iNewValue)
{
    // check that value specified for slider is inside limits
    if (iNewValue < m_iLimitMin || iNewValue > m_iLimitMax) {
        wxLogDebug(_T("Value %d is out of limits : %d - %d"),
                   iNewValue, m_iLimitMin, m_iLimitMax);
        return m_Slider->GetValue();
    }

    // if checks passed, then set new value to the slider
    m_Slider->SetValue(iNewValue);
    return iNewValue;

}


void tmSliderWithText::OnTextUpdated(wxCommandEvent &event)
{
    event.Skip();
    int iNewValue = wxAtoi(event.GetString());
    int iValidValue = SetSliderValue(iNewValue);
    if (iValidValue != iNewValue)
        m_Text->SetValue(wxString::Format(_T("%d"), iValidValue));
}


void tmSliderWithText::OnSliderUpdated(wxScrollEvent &event)
{
    event.Skip();
    m_Text->SetValue(wxString::Format(_T("%d"), event.GetPosition()));

}


int tmSliderWithText::GetValue()
{
    return m_Slider->GetValue();
}


bool tmSliderWithText::SetValue(int iValue)
{
    int iValidValue = SetSliderValue(iValue);
    int iPassedValue = iValue;
    bool bReturn = TRUE;

    // check that value passed is in the limits
    // if not, set all to 0
    if (iValidValue != iValue) {
        iPassedValue = m_iLimitMin;
        bReturn = FALSE;
    }

    m_Slider->SetValue(iPassedValue);
    m_Text->SetValue(wxString::Format(_T("%d"), iPassedValue));

    return bReturn;
}
