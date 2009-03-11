/***************************************************************************
									tmaattribctrls.h
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


#ifndef _TM_AATTRIBCTRLS_H_
#define _TM_AATTRIBCTRLS_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "../core/projectdefmemory.h"		// for fields definitions
#include <wx/spinctrl.h>					// for spin ctrl
//#include <wx/generic/spinctrg.h>			// for float spin ctrl
#include <wx/datectrl.h>					// for date picker ctrl


const wxString LABELDEFAULT = _T("MyLabel");


/***************************************************************************//**
 @brief Abstract class for tmAAttribTree Items
 @details This may be used as the abstract base class for creating items to use
 into the tmAAttribTree. This way, all items share the same methods for
 accessing and setting data without regarding the item type (text, choice,
 etc.) This abstract class is used by :
 - tmAAttribCtrlText
 - tmAAttribCtrlChoice
 - tmAAttribCtrlInteger
 @author Lucien Schreiber (c) CREALP 2009
 @date 08 March 2009
 *******************************************************************************/
class tmAAttribCtrl  : public wxPanel
	{
	private:
	protected:
		wxStaticText* m_Label;
		
		// panel control
		void SetPanelColour ();
		
	public:
		tmAAttribCtrl(){;}
		tmAAttribCtrl(wxWindow * parent, 
					  const ProjectDefMemoryFields & fieldinfo,
					  wxWindowID id = wxID_ANY,
					  const wxPoint & 	pos = wxDefaultPosition,
					  const wxSize & 	size = wxDefaultSize,
					  long 	style = wxTAB_TRAVERSAL,
					  const wxString & 	name = _T("tmAAttribCtrl")){;}
		virtual void Create (wxWindow * parent,
							 const ProjectDefMemoryFields & fieldinfo,
							 wxWindowID id = wxID_ANY,
							 const wxPoint & 	pos = wxDefaultPosition,
							 const wxSize & 	size = wxDefaultSize,
							 long 	style = wxTAB_TRAVERSAL,
							 const wxString & 	name = _T("tmAAttribCtrl")) = 0;
		~tmAAttribCtrl(){;}
		
		// label operations
		virtual void SetLabel(const wxString & labeltext);
		virtual wxString GetLabel ();
		
		// control operations
		virtual void SetControlValue(const wxString & value) = 0;
		virtual wxString GetControlValue () = 0;
	};

// Creating a list of Ctrls
class tmAAttribCtrlStruct : public wxObject
	{
	public:
		tmAAttribCtrlStruct() {m_Ctrl = NULL;}
		tmAAttribCtrl * m_Ctrl;
	};

WX_DECLARE_OBJARRAY(tmAAttribCtrlStruct, tmAAttribCtrlArray);


/***************************************************************************//**
 @brief tmAAttribTree item of type TextCtrl
 @details This control define a panel with a static text SetLabel(), GetLabel()
 and a textctrl. Use SetControlValue(), GetControlValue() to access the
 textctrl.
 @author Lucien Schreiber (c) CREALP 2009
 @date 08 March 2009
 *******************************************************************************/
class tmAAttribCtrlText : public tmAAttribCtrl
	{
	private:
		wxTextCtrl* m_Control;
		wxStaticText * m_ControlInfo;
		void SetProperties (const ProjectDefMemoryFields & fieldinfo);

	protected:
	public:
		tmAAttribCtrlText();
		tmAAttribCtrlText(wxWindow * parent, 
						  const ProjectDefMemoryFields & fieldinfo,
						  wxWindowID id = wxID_ANY,
						  const wxPoint & 	pos = wxDefaultPosition,
						  const wxSize & 	size = wxDefaultSize,
						  long 	style = wxTAB_TRAVERSAL,
						  const wxString & 	name = _T("tmAAttribCtrlText"));
		void Create (wxWindow * parent,
					 const ProjectDefMemoryFields & fieldinfo,
					 wxWindowID id = wxID_ANY,
					 const wxPoint & 	pos = wxDefaultPosition,
					 const wxSize & 	size = wxDefaultSize,
					 long 	style = wxTAB_TRAVERSAL,
					 const wxString & 	name = _T("tmAAttribCtrlText"));
		~tmAAttribCtrlText();
		
		
		// control operations
		virtual void SetControlValue(const wxString & value);
		virtual wxString GetControlValue ();
};




class tmAAttribCtrlInteger : public tmAAttribCtrl
	{
	private:
		wxSpinCtrl * m_Control;
		void SetProperties (const ProjectDefMemoryFields & fieldinfo);
		
	protected:
	public:
		tmAAttribCtrlInteger();
		tmAAttribCtrlInteger(wxWindow * parent,
							 const ProjectDefMemoryFields & fieldinfo,
							 wxWindowID id = wxID_ANY,
							 const wxPoint & 	pos = wxDefaultPosition,
							 const wxSize & 	size = wxDefaultSize,
							 long 	style = wxTAB_TRAVERSAL,
							 const wxString & 	name = _T("tmAAttribCtrlInteger"));
		void Create (wxWindow * parent,
					 const ProjectDefMemoryFields & fieldinfo,
					 wxWindowID id = wxID_ANY,
					 const wxPoint & 	pos = wxDefaultPosition,
					 const wxSize & 	size = wxDefaultSize,
					 long 	style = wxTAB_TRAVERSAL,
					 const wxString & 	name = _T("tmAAttribCtrlInteger"));
		~tmAAttribCtrlInteger();
		
		
		// control operations
		virtual void SetControlValue(const wxString & value);
		virtual wxString GetControlValue ();
	};




class tmAAttribCtrlFloat : public tmAAttribCtrl
	{
	private:
		wxTextCtrl * m_Control;
		wxStaticText * m_ControlInfo;
		void SetProperties (const ProjectDefMemoryFields & fieldinfo);
		
	protected:
	public:
		tmAAttribCtrlFloat();
		tmAAttribCtrlFloat(wxWindow * parent,
						   const ProjectDefMemoryFields & fieldinfo,
						   wxWindowID id = wxID_ANY,
						   const wxPoint & 	pos = wxDefaultPosition,
						   const wxSize & 	size = wxDefaultSize,
						   long 	style = wxTAB_TRAVERSAL,
						   const wxString & 	name = _T("tmAAttribCtrlFloat"));
		void Create (wxWindow * parent,
					 const ProjectDefMemoryFields & fieldinfo,
					 wxWindowID id = wxID_ANY,
					 const wxPoint & 	pos = wxDefaultPosition,
					 const wxSize & 	size = wxDefaultSize,
					 long 	style = wxTAB_TRAVERSAL,
					 const wxString & 	name = _T("tmAAttribCtrlFloat"));
		~tmAAttribCtrlFloat();
		
	
		// control operations
		virtual void SetControlValue(const wxString & value);
		virtual wxString GetControlValue ();

	};



const unsigned int AATTRIB_ENUM_RADIO_NUMBER = 3;
class tmAAttribCtrlEnum : public tmAAttribCtrl
	{
	private:
		wxChoice * m_ControlChoice;
		wxRadioButton * m_ControlRadios[AATTRIB_ENUM_RADIO_NUMBER];
		int m_iNumRadios;
		
		// private init function
		void InitMemberValues ();
		bool IsChoiceList();
		
		// control creating
		wxSizer * CreateChoiceControl (PrjMemFieldCodedValArray * valarray);
		
		
		
	protected:
	public:
		tmAAttribCtrlEnum();
		tmAAttribCtrlEnum(wxWindow * parent,
						  const ProjectDefMemoryFields & fieldinfo,
						  wxWindowID id = wxID_ANY,
						  const wxPoint & 	pos = wxDefaultPosition,
						  const wxSize & 	size = wxDefaultSize,
						  long 	style = wxTAB_TRAVERSAL,
						  const wxString & 	name = _T("tmAAttribCtrlEnum"));
		void Create (wxWindow * parent,
					 const ProjectDefMemoryFields & fieldinfo,
					 wxWindowID id = wxID_ANY,
					 const wxPoint & 	pos = wxDefaultPosition,
					 const wxSize & 	size = wxDefaultSize,
					 long 	style = wxTAB_TRAVERSAL,
					 const wxString & 	name = _T("tmAAttribCtrlEnum"));
		~tmAAttribCtrlEnum();
		
		
		// control operations
		virtual void SetControlValue(const wxString & value);
		virtual wxString GetControlValue ();
		
	};



class tmAAttribCtrlDate : public tmAAttribCtrl
	{
	private:
		wxDatePickerCtrl * m_Control;
		
	protected:
	public:
		tmAAttribCtrlDate();
		tmAAttribCtrlDate(wxWindow * parent,
						   const ProjectDefMemoryFields & fieldinfo,
						   wxWindowID id = wxID_ANY,
						   const wxPoint & 	pos = wxDefaultPosition,
						   const wxSize & 	size = wxDefaultSize,
						   long 	style = wxTAB_TRAVERSAL,
						   const wxString & 	name = _T("tmAAttribCtrlDate"));
		void Create (wxWindow * parent,
					 const ProjectDefMemoryFields & fieldinfo,
					 wxWindowID id = wxID_ANY,
					 const wxPoint & 	pos = wxDefaultPosition,
					 const wxSize & 	size = wxDefaultSize,
					 long 	style = wxTAB_TRAVERSAL,
					 const wxString & 	name = _T("tmAAttribCtrlDate"));
		~tmAAttribCtrlDate();
		
		
		// control operations
		virtual void SetControlValue(const wxString & value);
		virtual wxString GetControlValue ();
		
	};



class tmAAttribCtrlSafeDate : public tmAAttribCtrl
	{
	private:
		wxTextCtrl * m_Control;
		wxStaticText * m_ControlInfo;
		void SetProperties (const ProjectDefMemoryFields & fieldinfo);
		
	protected:
	public:
		tmAAttribCtrlSafeDate();
		tmAAttribCtrlSafeDate(wxWindow * parent,
						  const ProjectDefMemoryFields & fieldinfo,
						  wxWindowID id = wxID_ANY,
						  const wxPoint & 	pos = wxDefaultPosition,
						  const wxSize & 	size = wxDefaultSize,
						  long 	style = wxTAB_TRAVERSAL,
						  const wxString & 	name = _T("tmAAttribCtrlSafeDate"));
		void Create (wxWindow * parent,
					 const ProjectDefMemoryFields & fieldinfo,
					 wxWindowID id = wxID_ANY,
					 const wxPoint & 	pos = wxDefaultPosition,
					 const wxSize & 	size = wxDefaultSize,
					 long 	style = wxTAB_TRAVERSAL,
					 const wxString & 	name = _T("tmAAttribCtrlSafeDate"));
		~tmAAttribCtrlSafeDate();
		
		
		// control operations
		virtual void SetControlValue(const wxString & value);
		virtual wxString GetControlValue ();
		
	};



#endif
