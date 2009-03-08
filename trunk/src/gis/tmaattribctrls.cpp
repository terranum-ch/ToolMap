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



tmAAttribCtrlText::tmAAttribCtrlText(wxWindow * parent, wxWindowID id,
				  const wxPoint & 	pos,
				  const wxSize & 	size,
				  long 	style,
				  const wxString & 	name)
{
	tmAAttribCtrlText::Create(parent,id,pos,size,style,name);
}



void tmAAttribCtrlText::Create (wxWindow * parent, wxWindowID id ,
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
	bSizer24->Add( m_Label, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5); 
	

	m_Control = new wxTextCtrl( this, wxID_ANY, wxEmptyString,
							   wxDefaultPosition, wxSize( 200,-1 ), 0 );
	bSizer24->Add( m_Control, 1, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
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
	SetLabel(fieldinfo.m_Fieldname);
}






////////////////////////////////////////////////////////////////
////////////INTEGER  NUMBER  CONTROL //////////////////////////
//////////////////////////////////////////////////////////////
tmAAttribCtrlInteger::tmAAttribCtrlInteger()
{
	
}



tmAAttribCtrlInteger::tmAAttribCtrlInteger(wxWindow * parent, wxWindowID id,
									 const wxPoint & 	pos,
									 const wxSize & 	size,
									 long 	style,
									 const wxString & 	name)
{
	tmAAttribCtrlInteger::Create(parent,id,pos,size,style,name);
}



void tmAAttribCtrlInteger::Create (wxWindow * parent, wxWindowID id ,
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
	bSizer24->Add( m_Label, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5); 
	
	
	m_Control = new wxSpinCtrl( this, wxID_ANY, wxEmptyString,
							   wxDefaultPosition, wxSize( 120,-1 ));
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
	SetLabel(fieldinfo.m_Fieldname);
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



tmAAttribCtrlFloat::tmAAttribCtrlFloat(wxWindow * parent, wxWindowID id,
										   const wxPoint & 	pos,
										   const wxSize & 	size,
										   long 	style,
										   const wxString & 	name)
{
	tmAAttribCtrlFloat::Create(parent,id,pos,size,style,name);
}



void tmAAttribCtrlFloat::Create (wxWindow * parent, wxWindowID id ,
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
	bSizer24->Add( m_Label, 0, wxALL , 5); 
	
	
	wxBoxSizer* bSizer25;
	 bSizer25 = new wxBoxSizer( wxVERTICAL );
	
	m_Control = new wxTextCtrl( this, wxID_ANY, wxEmptyString,
							   wxDefaultPosition, wxSize( 120,-1 ),0,
	wxTextValidator(wxFILTER_NUMERIC));
	bSizer25->Add( m_Control, 1, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_ControlInfo = new wxStaticText( this, wxID_ANY, _T("Test"));
	bSizer25->Add( m_ControlInfo, 0, wxBOTTOM| wxLEFT | wxRIGHT | wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER, 5); 
	
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
	SetLabel(fieldinfo.m_Fieldname);
	
	wxASSERT (fieldinfo.m_FieldPrecision != 0);
	wxASSERT (fieldinfo.m_FieldScale != 0);
	
	wxString myExemple = wxEmptyString;
	for (int i = 0; i< fieldinfo.m_FieldPrecision; i++)
		myExemple.Append(_T("9"));
	
	myExemple.insert(fieldinfo.m_FieldPrecision - fieldinfo.m_FieldScale,
					 _T("."));
	myExemple.Prepend(_("Max : "));
	m_ControlInfo->SetLabel(myExemple);
	wxFont myFont (*wxNORMAL_FONT);
	myFont.SetStyle(wxFONTSTYLE_ITALIC);
	m_ControlInfo->SetFont(myFont);
		
	
}
