/***************************************************************************
								tmaattribctrls.cpp
							Controls for the tmAAttribTree
                             -------------------
    copyright            : (C) 2009 CREALP Lucien Schreiber
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

#include "tmaattribctrls.h"


////////////////////////////////////////////////////////////////
////////////ABSTRACT BASE CLASS      //////////////////////////
//////////////////////////////////////////////////////////////
#include <wx/arrimpl.cpp>
WX_DEFINE_OBJARRAY (tmAAttribCtrlArray);


void tmAAttribCtrl::SetLabel(const wxString &labeltext)
{
    m_Label->SetLabel(labeltext);
}


wxString tmAAttribCtrl::GetLabel() const
{
    wxASSERT (m_Label);
    return m_Label->GetLabel();
}


void tmAAttribCtrl::SetPanelColour(const wxColour &colour)
{
    this->SetBackgroundColour(colour);
}


////////////////////////////////////////////////////////////////
////////////      FULL SELECT TEXT  CONTROL ////////////////////
//////////////////////////////////////////////////////////////
BEGIN_EVENT_TABLE(tmFullSelectTextCtrl, wxTextCtrl)
                EVT_SET_FOCUS(tmFullSelectTextCtrl::OnFocus)
                EVT_IDLE(tmFullSelectTextCtrl::OnIdle)
END_EVENT_TABLE()

tmFullSelectTextCtrl::tmFullSelectTextCtrl()
{
    m_bDoSelectAll = false;
}


tmFullSelectTextCtrl::tmFullSelectTextCtrl(wxWindow *parent, wxWindowID id, const wxString &value,
                                           const wxPoint &pos, const wxSize &size,
                                           long style, const wxValidator &validator,
                                           const wxString &name)
//: wxTextCtrl(parent, id, value, pos, size, style, validator, name)
{
    m_bDoSelectAll = false;
    Create(parent, id, value, pos, size, style, validator, name);
}


void tmFullSelectTextCtrl::Create(wxWindow *parent, wxWindowID id, const wxString &value,
                                  const wxPoint &pos, const wxSize &size,
                                  long style, const wxValidator &validator,
                                  const wxString &name)
{
    wxTextCtrl::Create(parent, id, value, pos, size, style, validator, name);
}


tmFullSelectTextCtrl::~tmFullSelectTextCtrl()
{
}


void tmFullSelectTextCtrl::OnFocus(wxFocusEvent &event)
{
    m_bDoSelectAll = true;
    event.Skip();
}


void tmFullSelectTextCtrl::OnIdle(wxIdleEvent &event)
{
    if (m_bDoSelectAll) {
        // Needs to be delayed until after all other mouse-down
        // related events have been processed
        SetSelection(-1, -1);
        m_bDoSelectAll = false;
    }
}





////////////////////////////////////////////////////////////////
////////////      FULL SELECT SPIN  CONTROL ////////////////////
//////////////////////////////////////////////////////////////
BEGIN_EVENT_TABLE(tmFullSelectSpinCtrl, wxSpinCtrl)
                EVT_SET_FOCUS(tmFullSelectSpinCtrl::OnFocus)
                EVT_IDLE(tmFullSelectSpinCtrl::OnIdle)
                EVT_KILL_FOCUS(tmFullSelectSpinCtrl::OnLooseFocus)
END_EVENT_TABLE()

tmFullSelectSpinCtrl::tmFullSelectSpinCtrl()
{
    m_bDoSelectAll = false;
}


tmFullSelectSpinCtrl::tmFullSelectSpinCtrl(wxWindow *parent, wxWindowID id, const wxString &value,
                                           const wxPoint &pos, const wxSize &size,
                                           long style, int min, int max, int initial,
                                           const wxString &name)
{
    m_bDoSelectAll = false;
    Create(parent, id, value, pos, size, style, min, max, initial, name);
}


void tmFullSelectSpinCtrl::Create(wxWindow *parent, wxWindowID id, const wxString &value,
                                  const wxPoint &pos, const wxSize &size,
                                  long style, int min, int max, int initial,
                                  const wxString &name)
{
    wxSpinCtrl::Create(parent, id, value, pos, size, style, min, max, initial, name);
}


tmFullSelectSpinCtrl::~tmFullSelectSpinCtrl()
{
}


void tmFullSelectSpinCtrl::OnFocus(wxFocusEvent &event)
{
    m_bDoSelectAll = true;
    event.Skip();
}


void tmFullSelectSpinCtrl::OnLooseFocus(wxFocusEvent &event)
{
    int myValue = GetValue();
    if (myValue == GetMin()) {
        SetValue(0);
    }
    event.Skip();
}


void tmFullSelectSpinCtrl::OnIdle(wxIdleEvent &event)
{
    if (m_bDoSelectAll) {
        // Needs to be delayed until after all other mouse-down
        // related events have been processed
        SetSelection(-1, -1);
        m_bDoSelectAll = false;
    }
}


////////////////////////////////////////////////////////////////
////////////      TEXT       CONTROL //////////////////////////
//////////////////////////////////////////////////////////////
tmAAttribCtrlText::tmAAttribCtrlText()
{
    m_Control = NULL;
    m_ControlInfo = NULL;
}


tmAAttribCtrlText::tmAAttribCtrlText(wxWindow *parent,
                                     const ProjectDefMemoryFields &fieldinfo,
                                     wxWindowID id,
                                     const wxPoint &pos,
                                     const wxSize &size,
                                     long style,
                                     const wxString &name)
{
    tmAAttribCtrlText::Create(parent, fieldinfo, id, pos, size, style, name);
}


void tmAAttribCtrlText::Create(wxWindow *parent,
                               const ProjectDefMemoryFields &fieldinfo,
                               wxWindowID id,
                               const wxPoint &pos,
                               const wxSize &size,
                               long style,
                               const wxString &name)
{
    wxPanel::Create(parent, id, pos, size, style, name);
    SetPanelColour();
    wxBoxSizer *bSizer24;
    bSizer24 = new wxBoxSizer(wxHORIZONTAL);
    m_Label = new wxStaticText(this, wxID_ANY,
                               LABELDEFAULT,
                               wxDefaultPosition,
                               wxDefaultSize, 0);
    SetLabel(fieldinfo.m_Fieldname);
    m_Label->SetFont(wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT));
    bSizer24->Add(m_Label, 0, wxRIGHT | wxTOP | wxBOTTOM | wxALIGN_CENTER_VERTICAL, 5);

    wxBoxSizer *bSizer25 = new wxBoxSizer(wxHORIZONTAL);
    m_Control = new tmFullSelectTextCtrl(this, wxID_ANY, wxEmptyString,
                                         wxDefaultPosition, wxSize(200, -1), 0);

    m_ControlInfo = new wxStaticText(this, wxID_ANY, _T("Label"));

    SetProperties(fieldinfo);
    bSizer25->Add(m_Control, 0, wxALL | wxALIGN_CENTER_VERTICAL, 0);
    bSizer25->Add(m_ControlInfo, 0, wxLEFT | wxALIGN_CENTER_VERTICAL, 5);


    bSizer24->Add(bSizer25, 1, wxALL | wxALIGN_CENTER_VERTICAL, 2);

    SetSizer(bSizer24);
    Layout();
    bSizer24->Fit(this);


}


tmAAttribCtrlText::~tmAAttribCtrlText()
{

}


void tmAAttribCtrlText::SetControlValue(const wxString &value)
{
    m_Control->SetValue(value);
}


wxString tmAAttribCtrlText::GetControlValue()
{
    return m_Control->GetValue();
}


void tmAAttribCtrlText::SetProperties(const ProjectDefMemoryFields &fieldinfo)
{
    //Set max Length of wxTextCtrl
    wxASSERT (fieldinfo.m_FieldPrecision != 0);
    m_Control->SetMaxLength(fieldinfo.m_FieldPrecision);

    m_ControlInfo->SetLabel(wxString::Format(_T("(%d)"),
                                             fieldinfo.m_FieldPrecision));
}


////////////////////////////////////////////////////////////////
////////////INTEGER  NUMBER  CONTROL //////////////////////////
//////////////////////////////////////////////////////////////
tmAAttribCtrlInteger::tmAAttribCtrlInteger()
{
    m_Control = NULL;
}


tmAAttribCtrlInteger::tmAAttribCtrlInteger(wxWindow *parent,
                                           const ProjectDefMemoryFields &fieldinfo,
                                           wxWindowID id,
                                           const wxPoint &pos,
                                           const wxSize &size,
                                           long style,
                                           const wxString &name)
{
    tmAAttribCtrlInteger::Create(parent, fieldinfo, id, pos, size, style, name);
}


void tmAAttribCtrlInteger::Create(wxWindow *parent,
                                  const ProjectDefMemoryFields &fieldinfo,
                                  wxWindowID id,
                                  const wxPoint &pos,
                                  const wxSize &size,
                                  long style,
                                  const wxString &name)
{
    wxPanel::Create(parent, id, pos, size, style, name);
    wxBoxSizer *bSizer24;
    SetPanelColour();
    bSizer24 = new wxBoxSizer(wxHORIZONTAL);
    m_Label = new wxStaticText(this, wxID_ANY,
                               LABELDEFAULT,
                               wxDefaultPosition,
                               wxDefaultSize, 0);
    SetLabel(fieldinfo.m_Fieldname);
    m_Label->SetFont(wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT));
    bSizer24->Add(m_Label, 0, wxRIGHT | wxTOP | wxBOTTOM | wxALIGN_CENTER_VERTICAL, 5);


    m_Control = new tmFullSelectSpinCtrl(this, wxID_ANY, wxEmptyString,
                                         wxDefaultPosition, wxSize(120, -1));
    SetProperties(fieldinfo);
    bSizer24->Add(m_Control, 1, wxALL | wxALIGN_CENTER_VERTICAL, 2);


    SetSizer(bSizer24);
    Layout();
    bSizer24->Fit(this);


}


tmAAttribCtrlInteger::~tmAAttribCtrlInteger()
{

}


void tmAAttribCtrlInteger::SetControlValue(const wxString &value)
{
    m_Control->SetValue(wxAtoi(value));
}


wxString tmAAttribCtrlInteger::GetControlValue()
{
    return wxString::Format(_T("%d"), m_Control->GetValue());
}


void tmAAttribCtrlInteger::SetEmptyValue()
{
    m_Control->SetValue(0);
    m_Control->SetValue("");
}


void tmAAttribCtrlInteger::SetProperties(const ProjectDefMemoryFields &fieldinfo)
{
    // MAX INT for 32 bits system is : 4294967296, to allow
    // negative values : 2147483647
    m_Control->SetRange(-2147483647, 2147483647);
}


///////////////////////////////////////////////////////
//////////// FLOAT NUMBER  CONTROL ///////////////////
//////////////////////////////////////////////////////
tmAAttribCtrlFloat::tmAAttribCtrlFloat()
{
    m_Control = NULL;
    m_ControlInfo = NULL;
}


tmAAttribCtrlFloat::tmAAttribCtrlFloat(wxWindow *parent,
                                       const ProjectDefMemoryFields &fieldinfo,
                                       wxWindowID id,
                                       const wxPoint &pos,
                                       const wxSize &size,
                                       long style,
                                       const wxString &name)
{
    tmAAttribCtrlFloat::Create(parent, fieldinfo, id, pos, size, style, name);
}


void tmAAttribCtrlFloat::Create(wxWindow *parent,
                                const ProjectDefMemoryFields &fieldinfo,
                                wxWindowID id,
                                const wxPoint &pos,
                                const wxSize &size,
                                long style,
                                const wxString &name)
{
    wxPanel::Create(parent, id, pos, size, style, name);
    SetPanelColour();
    wxBoxSizer *bSizer24;
    bSizer24 = new wxBoxSizer(wxHORIZONTAL);
    m_Label = new wxStaticText(this, wxID_ANY,
                               LABELDEFAULT,
                               wxDefaultPosition,
                               wxDefaultSize, 0);
    SetLabel(fieldinfo.m_Fieldname);
    m_Label->SetFont(wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT));
    bSizer24->Add(m_Label, 0, wxRIGHT | wxTOP | wxBOTTOM, 5);


    wxBoxSizer *bSizer25;
    bSizer25 = new wxBoxSizer(wxVERTICAL);

    m_Control = new tmFullSelectTextCtrl(this, wxID_ANY, wxEmptyString,
                                         wxDefaultPosition, wxSize(120, -1), 0,
                                         wxTextValidator(wxFILTER_NUMERIC));
    bSizer25->Add(m_Control, 1, wxALL, 5);
    m_ControlInfo = new wxStaticText(this, wxID_ANY, _T("Test"));
    bSizer25->Add(m_ControlInfo, 0, wxBOTTOM | wxLEFT | wxRIGHT, 5); // | wxALIGN_CENTER, 5);
    SetProperties(fieldinfo);

    bSizer24->Add(bSizer25, 1, wxEXPAND, 2);

    SetSizer(bSizer24);
    Layout();
    bSizer24->Fit(this);


}


tmAAttribCtrlFloat::~tmAAttribCtrlFloat()
{

}


void tmAAttribCtrlFloat::SetControlValue(const wxString &value)
{
    m_Control->SetValue(value);
}


wxString tmAAttribCtrlFloat::GetControlValue()
{
    return m_Control->GetValue();
}


void tmAAttribCtrlFloat::SetEmptyValue()
{
    m_Control->SetValue("");
}


void tmAAttribCtrlFloat::SetProperties(const ProjectDefMemoryFields &fieldinfo)
{
    //SetLabel(fieldinfo.m_Fieldname);

    wxASSERT (fieldinfo.m_FieldPrecision != 0);
    wxASSERT (fieldinfo.m_FieldScale != 0);

    wxString myExemple = wxEmptyString;
    for (int i = 0; i < fieldinfo.m_FieldPrecision; i++)
        myExemple.Append(_T("9"));

    myExemple.insert(fieldinfo.m_FieldPrecision - fieldinfo.m_FieldScale,
                     _T("."));
    myExemple.Prepend(_("Max : "));
    m_ControlInfo->SetLabel(myExemple);
    wxFont myFont = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
    myFont.SetStyle(wxFONTSTYLE_ITALIC);
    m_ControlInfo->SetFont(myFont);


}


///////////////////////////////////////////////////////
//////////// FLOAT NUMBER  CONTROL ///////////////////
//////////////////////////////////////////////////////
tmAAttribCtrlEnum::tmAAttribCtrlEnum()
{
    InitMemberValues();
}


tmAAttribCtrlEnum::tmAAttribCtrlEnum(wxWindow *parent,
                                     const ProjectDefMemoryFields &fieldinfo,
                                     wxWindowID id,
                                     const wxPoint &pos,
                                     const wxSize &size,
                                     long style,
                                     const wxString &name)
{
    tmAAttribCtrlEnum::Create(parent, fieldinfo, id, pos, size, style, name);
}


void tmAAttribCtrlEnum::Create(wxWindow *parent,
                               const ProjectDefMemoryFields &fieldinfo,
                               wxWindowID id,
                               const wxPoint &pos,
                               const wxSize &size,
                               long style,
                               const wxString &name)
{
    wxPanel::Create(parent, id, pos, size, style, name);
    InitMemberValues();
    SetPanelColour();

    // copy field locally
    m_Field = fieldinfo;

    // creating label
    wxBoxSizer *bSizer24;
    bSizer24 = new wxBoxSizer(wxHORIZONTAL);
    m_Label = new wxStaticText(this, wxID_ANY, LABELDEFAULT, wxDefaultPosition, wxDefaultSize, 0);
    SetLabel(m_Field.m_Fieldname);
    m_Label->SetFont(wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT));
    bSizer24->Add(m_Label, 0, wxRIGHT | wxTOP | wxBOTTOM | wxALIGN_CENTER_VERTICAL, 5);

    wxSizer *mySizer = CreateChoiceControl(m_Field.m_pCodedValueArray);
    wxASSERT (mySizer);
    bSizer24->Add(mySizer, 1, wxEXPAND, 2);

    SetSizer(bSizer24);
    Layout();
    bSizer24->Fit(this);
}


wxSizer *tmAAttribCtrlEnum::CreateChoiceControl(const PrjMemFieldCodedValArray &valarray)
{
    wxASSERT (valarray.GetCount() > 0);
    wxBoxSizer *bSizer25 = new wxBoxSizer(wxHORIZONTAL);

    // 3 radio controls
    if (valarray.GetCount() <= AATTRIB_ENUM_RADIO_NUMBER) {
        m_iNumRadios = 0;
        for (unsigned int i = 0; i < valarray.GetCount(); i++) {
            // set first radio as new group
            if (i == 0) {
                m_ControlRadios[i] = new wxRadioButton(this, wxID_ANY, valarray.Item(i)->m_ValueName, wxDefaultPosition,
                                                       wxDefaultSize, wxRB_GROUP);
            } else {
                m_ControlRadios[i] = new wxRadioButton(this, wxID_ANY, valarray.Item(i)->m_ValueName);
            }
            m_iNumRadios++;
            bSizer25->Add(m_ControlRadios[i], 0, wxALL, 5);
        }
    } else // choice control
    {
        wxArrayString myValues;
        for (unsigned int i = 0; i < valarray.GetCount(); i++)
            myValues.Add(valarray.Item(i)->m_ValueName);

        m_ControlChoice = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                                       myValues);
        bSizer25->Add(m_ControlChoice, 0, wxALL, 5);
        m_ControlChoice->SetSelection(0);
    }

    return bSizer25;
}


void tmAAttribCtrlEnum::InitMemberValues()
{
    m_ControlChoice = NULL;
    for (unsigned int i = 0; i < AATTRIB_ENUM_RADIO_NUMBER; i++)
        m_ControlRadios[i] = NULL;

    m_iNumRadios = 0;
}


bool tmAAttribCtrlEnum::IsChoiceList()
{
    if (m_ControlChoice)
        return true;
    return false;
}


tmAAttribCtrlEnum::~tmAAttribCtrlEnum()
{
}


void tmAAttribCtrlEnum::SetControlValue(const wxString &value)
{
    if (IsChoiceList()) // CHOICE
    {
        if (m_ControlChoice->SetStringSelection(value) == false) {
            wxLogError(_T("Value %s not found in RadioBox"), value.c_str());
        }
    } else // RADIO BUTTON
    {
        wxASSERT (m_iNumRadios > 0);
        for (int i = 0; i < m_iNumRadios; i++) {
            if (m_ControlRadios[i]->GetLabelText() == value) {
                m_ControlRadios[i]->SetValue(true);
            } else {
                m_ControlRadios[i]->SetValue(false);
            }
        }
    }

}


wxString tmAAttribCtrlEnum::GetControlValue()
{
    if (IsChoiceList()) // CHOICE
    {
        return m_ControlChoice->GetStringSelection();
    } else // RADIO BUTTON
    {
        wxString myRetVal = wxEmptyString;
        for (int i = 0; i < m_iNumRadios; i++) {
            if (m_ControlRadios[i]->GetValue() == true) {
                //myRetVal << m_Field.m_pCodedValueArray.Item(i)->m_ValueID;
                myRetVal = m_ControlRadios[i]->GetLabelText();
                break;
            }
        }
        return myRetVal;
    }
}


void tmAAttribCtrlEnum::SetEmptyValue()
{
    if (IsChoiceList()) {
        m_ControlChoice->SetSelection(wxNOT_FOUND);
    } else {
        for (int i = 0; i < m_iNumRadios; i++) {
            m_ControlRadios[i]->SetValue(false);
        }
    }
}


///////////////////////////////////////////////////////
//////////// DATE  CONTROL ///////////////////
//////////////////////////////////////////////////////
tmAAttribCtrlDate::tmAAttribCtrlDate()
{
    m_Control = NULL;
}


tmAAttribCtrlDate::tmAAttribCtrlDate(wxWindow *parent,
                                     const ProjectDefMemoryFields &fieldinfo,
                                     wxWindowID id,
                                     const wxPoint &pos,
                                     const wxSize &size,
                                     long style,
                                     const wxString &name)
{
    tmAAttribCtrlDate::Create(parent, fieldinfo, id, pos, size, style, name);
}


void tmAAttribCtrlDate::Create(wxWindow *parent,
                               const ProjectDefMemoryFields &fieldinfo,
                               wxWindowID id,
                               const wxPoint &pos,
                               const wxSize &size,
                               long style,
                               const wxString &name)
{
    wxPanel::Create(parent, id, pos, size, style, name);
    SetPanelColour();
    wxBoxSizer *bSizer24;
    bSizer24 = new wxBoxSizer(wxHORIZONTAL);
    m_Label = new wxStaticText(this, wxID_ANY,
                               LABELDEFAULT,
                               wxDefaultPosition,
                               wxDefaultSize, 0);
    SetLabel(fieldinfo.m_Fieldname);
    m_Label->SetFont(wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT));
    bSizer24->Add(m_Label, 0, wxRIGHT | wxTOP | wxBOTTOM, 5);


    m_Control = new wxDatePickerCtrl(this, wxID_ANY);
    bSizer24->Add(m_Control, 1, wxALL | wxALIGN_CENTER_VERTICAL, 2);

    SetSizer(bSizer24);
    Layout();
    bSizer24->Fit(this);


}


tmAAttribCtrlDate::~tmAAttribCtrlDate()
{
    delete m_Control;
    m_Control = NULL;
}


void tmAAttribCtrlDate::SetControlValue(const wxString &value)
{
    // only date passed as : YYYY-MM-DD
    wxASSERT(value.GetChar(4) == '-');


    wxDateTime myDate;
    if (myDate.ParseDate(value) == true) {
        m_Control->SetValue(myDate);
    } else {
        wxLogError(_T("Error parsing date in %s"), value.c_str());
    }

}


wxString tmAAttribCtrlDate::GetControlValue()
{
    wxDateTime myDate = m_Control->GetValue();
    wxString myDateText = myDate.FormatISODate();
    wxLogDebug(_T("Date returned is : %s"), myDateText.c_str());
    return myDateText;
}


///////////////////////////////////////////////////////
//////////// SAFE DATE   CONTROL /////////////////////
//////////////////////////////////////////////////////
tmAAttribCtrlSafeDate::tmAAttribCtrlSafeDate()
{
    m_Control = NULL;
    m_ControlInfo = NULL;
}


tmAAttribCtrlSafeDate::tmAAttribCtrlSafeDate(wxWindow *parent,
                                             const ProjectDefMemoryFields &fieldinfo,
                                             wxWindowID id,
                                             const wxPoint &pos,
                                             const wxSize &size,
                                             long style,
                                             const wxString &name)
{
    tmAAttribCtrlSafeDate::Create(parent, fieldinfo, id, pos, size, style, name);
}


void tmAAttribCtrlSafeDate::Create(wxWindow *parent,
                                   const ProjectDefMemoryFields &fieldinfo,
                                   wxWindowID id,
                                   const wxPoint &pos,
                                   const wxSize &size,
                                   long style,
                                   const wxString &name)
{
    wxPanel::Create(parent, id, pos, size, style, name);
    SetPanelColour();
    wxBoxSizer *bSizer24;
    bSizer24 = new wxBoxSizer(wxHORIZONTAL);
    m_Label = new wxStaticText(this, wxID_ANY,
                               LABELDEFAULT,
                               wxDefaultPosition,
                               wxDefaultSize, 0);
    SetLabel(fieldinfo.m_Fieldname);
    m_Label->SetFont(wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT));
    bSizer24->Add(m_Label, 0, wxRIGHT | wxTOP | wxBOTTOM, 5);


    wxBoxSizer *bSizer25;
    bSizer25 = new wxBoxSizer(wxVERTICAL);

    m_Control = new tmFullSelectTextCtrl(this, wxID_ANY, wxEmptyString,
                                         wxDefaultPosition, wxSize(120, -1), 0,
                                         wxTextValidator(wxFILTER_NUMERIC));
    bSizer25->Add(m_Control, 1, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    m_ControlInfo = new wxStaticText(this, wxID_ANY, _T("Test"));
    bSizer25->Add(m_ControlInfo, 0, wxBOTTOM | wxLEFT | wxRIGHT | wxALIGN_CENTER_VERTICAL, 5); // | wxALIGN_CENTER, 5);
    SetProperties(fieldinfo);

    bSizer24->Add(bSizer25, 1, wxEXPAND, 2);

    SetSizer(bSizer24);
    Layout();
    bSizer24->Fit(this);


}


tmAAttribCtrlSafeDate::~tmAAttribCtrlSafeDate()
{

}


void tmAAttribCtrlSafeDate::SetControlValue(const wxString &value)
{
    m_Control->SetValue(value);
}


wxString tmAAttribCtrlSafeDate::GetControlValue()
{
    return m_Control->GetValue();
}


void tmAAttribCtrlSafeDate::SetEmptyValue()
{
    m_Control->SetValue("");
}


void tmAAttribCtrlSafeDate::SetProperties(const ProjectDefMemoryFields &fieldinfo)
{
    m_ControlInfo->SetLabel(_("Format : YYYY-MM-DD"));
    wxFont myFont = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
    myFont.SetStyle(wxFONTSTYLE_ITALIC);
    m_ControlInfo->SetFont(myFont);
}


