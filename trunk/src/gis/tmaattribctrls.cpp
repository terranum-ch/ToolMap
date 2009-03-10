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
void tmAAttribCtrl::SetLabel(const wxString & labeltext)
{
	m_Label->SetLabel(labeltext);
}


wxString tmAAttribCtrl::GetLabel ()
{
	wxASSERT (m_Label);
	return m_Label->GetLabel();
}





////////////////////////////////////////////////////////////////
////////////      TEXT       CONTROL //////////////////////////
//////////////////////////////////////////////////////////////
tmAAttribCtrlText::tmAAttribCtrlText()
{
	
}



tmAAttribCtrlText::tmAAttribCtrlText(wxWindow * parent,
									 const ProjectDefMemoryFields & fieldinfo,
									 wxWindowID id,
									 const wxPoint & 	pos,
									 const wxSize & 	size,
									 long 	style,
									 const wxString & 	name)
{
	tmAAttribCtrlText::Create(parent,fieldinfo,id,pos,size,style,name);
}



void tmAAttribCtrlText::Create (wxWindow * parent,
								const ProjectDefMemoryFields & fieldinfo,
								wxWindowID id ,
								const wxPoint & 	pos,
								const wxSize & 	size,
								long 	style,
								const wxString & 	name)
{
	wxPanel::Create(parent,id,pos,size,style,name);
	wxBoxSizer* bSizer24;
	bSizer24 = new wxBoxSizer( wxHORIZONTAL );
	m_Label = new wxStaticText( this, wxID_ANY,
							   LABELDEFAULT,
							   wxDefaultPosition,
							   wxDefaultSize, 0 );
	SetLabel(fieldinfo.m_Fieldname);
	m_Label->SetFont(wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT));
	bSizer24->Add( m_Label, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5); 
	
	wxBoxSizer * bSizer25 = new wxBoxSizer(wxHORIZONTAL);
	m_Control = new wxTextCtrl( this, wxID_ANY, wxEmptyString,
							   wxDefaultPosition, wxSize( 200,-1 ), 0 );
	
	m_ControlInfo = new wxStaticText(this, wxID_ANY, _T("Label"));
		
	SetProperties(fieldinfo);
	bSizer25->Add( m_Control, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0 );
	bSizer25->Add(m_ControlInfo,0,wxLEFT |wxALIGN_CENTER_VERTICAL,5);
	
	
	bSizer24->Add( bSizer25, 1, wxALL | wxALIGN_CENTER_VERTICAL, 5);
	
	SetSizer( bSizer24 );
	Layout();
	bSizer24->Fit( this );
	
	
}



tmAAttribCtrlText::~tmAAttribCtrlText()
{

}



void tmAAttribCtrlText::SetControlValue(const wxString & value)
{
	m_Control->SetValue(value);
}


wxString tmAAttribCtrlText::GetControlValue ()
{
	return m_Control->GetValue();
}


void tmAAttribCtrlText::SetProperties (const ProjectDefMemoryFields & fieldinfo)
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
	
}



tmAAttribCtrlInteger::tmAAttribCtrlInteger(wxWindow * parent,
										   const ProjectDefMemoryFields & fieldinfo,
										   wxWindowID id,
										   const wxPoint & 	pos,
										   const wxSize & 	size,
										   long 	style,
										   const wxString & 	name)
{
	tmAAttribCtrlInteger::Create(parent,fieldinfo, id,pos,size,style,name);
}



void tmAAttribCtrlInteger::Create (wxWindow * parent,
								   const ProjectDefMemoryFields & fieldinfo,
								   wxWindowID id ,
								   const wxPoint & 	pos,
								   const wxSize & 	size,
								   long 	style,
								   const wxString & 	name)
{
	wxPanel::Create(parent,id,pos,size,style,name);
	wxBoxSizer* bSizer24;
	bSizer24 = new wxBoxSizer( wxHORIZONTAL );
	m_Label = new wxStaticText( this, wxID_ANY,
							   LABELDEFAULT,
							   wxDefaultPosition,
							   wxDefaultSize, 0 );
	SetLabel(fieldinfo.m_Fieldname);
	m_Label->SetFont(wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT));
	bSizer24->Add( m_Label, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5); 
	
	
	m_Control = new wxSpinCtrl( this, wxID_ANY, wxEmptyString,
							   wxDefaultPosition, wxSize( 120,-1 ));
	SetProperties(fieldinfo);
	bSizer24->Add( m_Control, 1, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	SetSizer( bSizer24 );
	Layout();
	bSizer24->Fit( this );
	
	
}



tmAAttribCtrlInteger::~tmAAttribCtrlInteger()
{
	
}


void tmAAttribCtrlInteger::SetControlValue(const wxString & value)
{
	m_Control->SetValue(wxAtoi(value));
}


wxString tmAAttribCtrlInteger::GetControlValue ()
{
	return wxString::Format(_T("%d"), m_Control->GetValue());
}


void tmAAttribCtrlInteger::SetProperties (const ProjectDefMemoryFields & fieldinfo)
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
	
}



tmAAttribCtrlFloat::tmAAttribCtrlFloat(wxWindow * parent,
									   const ProjectDefMemoryFields & fieldinfo,
									   wxWindowID id,
									   const wxPoint & 	pos,
									   const wxSize & 	size,
									   long 	style,
									   const wxString & 	name)
{
	tmAAttribCtrlFloat::Create(parent,fieldinfo, id,pos,size,style,name);
}



void tmAAttribCtrlFloat::Create (wxWindow * parent,
								 const ProjectDefMemoryFields & fieldinfo,
								 wxWindowID id ,
								 const wxPoint & 	pos,
								 const wxSize & 	size,
								 long 	style,
								 const wxString & 	name)
{
	wxPanel::Create(parent,id,pos,size,style,name);
	wxBoxSizer* bSizer24;
	bSizer24 = new wxBoxSizer( wxHORIZONTAL );
	m_Label = new wxStaticText( this, wxID_ANY,
							   LABELDEFAULT,
							   wxDefaultPosition,
							   wxDefaultSize, 0 );
	SetLabel(fieldinfo.m_Fieldname);
	m_Label->SetFont(wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT));
	bSizer24->Add( m_Label, 0, wxALL , 5); 
	
	
	wxBoxSizer* bSizer25;
	 bSizer25 = new wxBoxSizer( wxVERTICAL );
	
	m_Control = new wxTextCtrl( this, wxID_ANY, wxEmptyString,
							   wxDefaultPosition, wxSize( 120,-1 ),0,
	wxTextValidator(wxFILTER_NUMERIC));
	bSizer25->Add( m_Control, 1, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	m_ControlInfo = new wxStaticText( this, wxID_ANY, _T("Test"));
	bSizer25->Add( m_ControlInfo, 0, wxBOTTOM| wxLEFT | wxRIGHT | wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER, 5); 
	SetProperties(fieldinfo);
	
	bSizer24->Add( bSizer25, 1, wxEXPAND, 5 );
	
	SetSizer( bSizer24 );
	Layout();
	bSizer24->Fit( this );
	
	
}



tmAAttribCtrlFloat::~tmAAttribCtrlFloat()
{
	
}


void tmAAttribCtrlFloat::SetControlValue(const wxString & value)
{
	m_Control->SetValue(value);
}


wxString tmAAttribCtrlFloat::GetControlValue ()
{
	return m_Control->GetValue();
}


void tmAAttribCtrlFloat::SetProperties (const ProjectDefMemoryFields & fieldinfo)
{
	//SetLabel(fieldinfo.m_Fieldname);
	
	wxASSERT (fieldinfo.m_FieldPrecision != 0);
	wxASSERT (fieldinfo.m_FieldScale != 0);
	
	wxString myExemple = wxEmptyString;
	for (int i = 0; i< fieldinfo.m_FieldPrecision; i++)
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
	
}



tmAAttribCtrlEnum::tmAAttribCtrlEnum(wxWindow * parent,
									 const ProjectDefMemoryFields & fieldinfo,
									 wxWindowID id,
									 const wxPoint & 	pos,
									 const wxSize & 	size,
									 long 	style,
									 const wxString & 	name)
{
	tmAAttribCtrlEnum::Create(parent,fieldinfo,id,pos,size,style,name);
}



void tmAAttribCtrlEnum::Create (wxWindow * parent,
								const ProjectDefMemoryFields & fieldinfo,
								wxWindowID id ,
								const wxPoint & 	pos,
								const wxSize & 	size,
								long 	style,
								const wxString & 	name)
{
	wxPanel::Create(parent,id,pos,size,style,name);
	InitMemberValues();
	
	// creating label
	wxBoxSizer* bSizer24;
	bSizer24 = new wxBoxSizer( wxHORIZONTAL );
	m_Label = new wxStaticText( this, wxID_ANY,
							   LABELDEFAULT,
							   wxDefaultPosition,
							   wxDefaultSize, 0 );
	SetLabel(fieldinfo.m_Fieldname);
	m_Label->SetFont(wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT));
	bSizer24->Add( m_Label, 0, wxALL | wxALIGN_CENTER_VERTICAL , 5); 
	
	wxSizer * mySizer = CreateChoiceControl(fieldinfo.m_pCodedValueArray);
	wxASSERT (mySizer);
	bSizer24->Add( mySizer, 1, wxEXPAND, 5 );
		
	SetSizer( bSizer24 );
	Layout();
	bSizer24->Fit( this );
	
	
}


wxSizer * tmAAttribCtrlEnum::CreateChoiceControl (PrjMemFieldCodedValArray * valarray)
{
	wxASSERT (valarray);
	wxASSERT (valarray->GetCount() > 0);
	wxBoxSizer * bSizer25 = new wxBoxSizer(wxHORIZONTAL);
	
	// 3 radio controls
	if (valarray->GetCount() <= AATTRIB_ENUM_RADIO_NUMBER)
	{
		m_iNumRadios = 0;
		for (unsigned int i = 0; i< valarray->GetCount();i++)
		{
			m_ControlRadios[i] = new wxRadioButton(this,wxID_ANY,
												   valarray->Item(i).m_ValueName);
			m_iNumRadios++;
			bSizer25->Add(m_ControlRadios[i], 0, wxALL, 5 );
			
		}
		// default true
		m_ControlRadios[0]->SetValue(true);
			
	}
	else // choice control
	{
		wxArrayString myValues;
		for (unsigned int i = 0; i< valarray->GetCount();i++)
			myValues.Add(valarray->Item(i).m_ValueName);
			
		m_ControlChoice = new wxChoice(this,wxID_ANY,wxDefaultPosition,wxDefaultSize,
									   myValues);
		bSizer25->Add(m_ControlChoice, 0, wxALL, 5);
		m_ControlChoice->SetSelection(0);
	}
	
	return bSizer25;
}



void tmAAttribCtrlEnum::InitMemberValues ()
{
	m_ControlChoice = NULL;
	for (unsigned int i = 0; i< AATTRIB_ENUM_RADIO_NUMBER;i ++)
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


void tmAAttribCtrlEnum::SetControlValue(const wxString & value)
{
	if (IsChoiceList()) // CHOICE
	{
		if (m_ControlChoice->SetStringSelection(value)==false)
		{
			wxLogError(_T("Value %s not found in RadioBox"), value.c_str());
		}			
	}
	else // RADIO BUTTON
	{
		wxASSERT (m_iNumRadios > 0);
		for (int i = 0; i< m_iNumRadios;i++)
		{
			if (m_ControlRadios[i]->GetLabelText() == value)
			{
				m_ControlRadios[i]->SetValue(true);
			}
			else
			{
				m_ControlRadios[i]->SetValue(false);
			}
		}
	}

}


wxString tmAAttribCtrlEnum::GetControlValue ()
{
	if (IsChoiceList()) // CHOICE
	{
		return m_ControlChoice->GetStringSelection();
	}
	else // RADIO BUTTON
	{
		wxString myRetVal = wxEmptyString;
		for (int i = 0; i< m_iNumRadios;i++)
		{
			if (m_ControlRadios[i]->GetValue() == true)
			{
				myRetVal = m_ControlRadios[i]->GetLabelText();
				break;
			}
		}
		
		if (myRetVal.IsEmpty())
		{
			wxLogError(_T("No value returned by radio button, check an item"));
		}
		return myRetVal;
	}
}




///////////////////////////////////////////////////////
//////////// FLOAT NUMBER  CONTROL ///////////////////
//////////////////////////////////////////////////////
tmAAttribCtrlDate::tmAAttribCtrlDate()
{
	
}



tmAAttribCtrlDate::tmAAttribCtrlDate(wxWindow * parent,
									   const ProjectDefMemoryFields & fieldinfo,
									   wxWindowID id,
									   const wxPoint & 	pos,
									   const wxSize & 	size,
									   long 	style,
									   const wxString & 	name)
{
	tmAAttribCtrlDate::Create(parent,fieldinfo, id,pos,size,style,name);
}



void tmAAttribCtrlDate::Create (wxWindow * parent,
								 const ProjectDefMemoryFields & fieldinfo,
								 wxWindowID id ,
								 const wxPoint & 	pos,
								 const wxSize & 	size,
								 long 	style,
								 const wxString & 	name)
{
	wxPanel::Create(parent,id,pos,size,style,name);
	wxBoxSizer* bSizer24;
	bSizer24 = new wxBoxSizer( wxHORIZONTAL );
	m_Label = new wxStaticText( this, wxID_ANY,
							   LABELDEFAULT,
							   wxDefaultPosition,
							   wxDefaultSize, 0 );
	SetLabel(fieldinfo.m_Fieldname);
	m_Label->SetFont(wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT));
	bSizer24->Add( m_Label, 0, wxALL , 5); 
	
	
	
	m_Control = new wxDatePickerCtrl ( this, wxID_ANY);
	bSizer24->Add( m_Control, 1, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	SetSizer( bSizer24 );
	Layout();
	bSizer24->Fit( this );
	
	
}



tmAAttribCtrlDate::~tmAAttribCtrlDate()
{
	delete m_Control;
	m_Control = NULL;
}


void tmAAttribCtrlDate::SetControlValue(const wxString & value)
{
	// only date passed as : YYYY-MM-DD
	wxASSERT(value.GetChar(4) == '-');
	
	
	
	wxDateTime myDate;
	if (myDate.ParseDate(value)!=NULL)
	{
		m_Control->SetValue(myDate);
	}
	else
		wxLogError(_T("Error parsing date in %s"), value.c_str());
	
}


wxString tmAAttribCtrlDate::GetControlValue ()
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
	
}



tmAAttribCtrlSafeDate::tmAAttribCtrlSafeDate(wxWindow * parent,
									   const ProjectDefMemoryFields & fieldinfo,
									   wxWindowID id,
									   const wxPoint & 	pos,
									   const wxSize & 	size,
									   long 	style,
									   const wxString & 	name)
{
	tmAAttribCtrlSafeDate::Create(parent,fieldinfo, id,pos,size,style,name);
}



void tmAAttribCtrlSafeDate::Create (wxWindow * parent,
								 const ProjectDefMemoryFields & fieldinfo,
								 wxWindowID id ,
								 const wxPoint & 	pos,
								 const wxSize & 	size,
								 long 	style,
								 const wxString & 	name)
{
	wxPanel::Create(parent,id,pos,size,style,name);
	wxBoxSizer* bSizer24;
	bSizer24 = new wxBoxSizer( wxHORIZONTAL );
	m_Label = new wxStaticText( this, wxID_ANY,
							   LABELDEFAULT,
							   wxDefaultPosition,
							   wxDefaultSize, 0 );
	SetLabel(fieldinfo.m_Fieldname);
	m_Label->SetFont(wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT));
	bSizer24->Add( m_Label, 0, wxALL , 5); 
	
	
	wxBoxSizer* bSizer25;
	bSizer25 = new wxBoxSizer( wxVERTICAL );
	
	m_Control = new wxTextCtrl( this, wxID_ANY, wxEmptyString,
							   wxDefaultPosition, wxSize( 120,-1 ),0,
							   wxTextValidator(wxFILTER_NUMERIC));
	bSizer25->Add( m_Control, 1, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	m_ControlInfo = new wxStaticText( this, wxID_ANY, _T("Test"));
	bSizer25->Add( m_ControlInfo, 0, wxBOTTOM| wxLEFT | wxRIGHT | wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER, 5); 
	SetProperties(fieldinfo);
	
	bSizer24->Add( bSizer25, 1, wxEXPAND, 5 );
	
	SetSizer( bSizer24 );
	Layout();
	bSizer24->Fit( this );
	
	
}



tmAAttribCtrlSafeDate::~tmAAttribCtrlSafeDate()
{
	
}


void tmAAttribCtrlSafeDate::SetControlValue(const wxString & value)
{
	m_Control->SetValue(value);
}


wxString tmAAttribCtrlSafeDate::GetControlValue ()
{
	return m_Control->GetValue();
}


void tmAAttribCtrlSafeDate::SetProperties (const ProjectDefMemoryFields & fieldinfo)
{
	m_ControlInfo->SetLabel(_("Format : YYYY-MM-DD"));
	wxFont myFont = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
	myFont.SetStyle(wxFONTSTYLE_ITALIC);
	m_ControlInfo->SetFont(myFont);
}


