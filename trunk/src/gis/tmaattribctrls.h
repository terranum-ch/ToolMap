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
		
		
	public:
		tmAAttribCtrl(){;}
		tmAAttribCtrl(wxWindow * parent, wxWindowID id = wxID_ANY,
					  const wxPoint & 	pos = wxDefaultPosition,
					  const wxSize & 	size = wxDefaultSize,
					  long 	style = wxTAB_TRAVERSAL,
					  const wxString & 	name = _T("tmAAttribCtrl")){;}
		virtual void Create (wxWindow * parent, wxWindowID id = wxID_ANY,
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
		
		// control properties
		virtual void SetProperties (const ProjectDefMemoryFields & fieldinfo) = 0;
	};



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
	protected:
	public:
		tmAAttribCtrlText();
		tmAAttribCtrlText(wxWindow * parent, wxWindowID id = wxID_ANY,
					  const wxPoint & 	pos = wxDefaultPosition,
					  const wxSize & 	size = wxDefaultSize,
					  long 	style = wxTAB_TRAVERSAL,
					  const wxString & 	name = _T("tmAAttribCtrl"));
		void Create (wxWindow * parent, wxWindowID id = wxID_ANY,
					 const wxPoint & 	pos = wxDefaultPosition,
					 const wxSize & 	size = wxDefaultSize,
					 long 	style = wxTAB_TRAVERSAL,
					 const wxString & 	name = _T("tmAAttribCtrl"));
		~tmAAttribCtrlText();
		
		
		// control operations
		virtual void SetControlValue(const wxString & value);
		virtual wxString GetControlValue ();
		
		// control properties
		virtual void SetProperties (const ProjectDefMemoryFields & fieldinfo);
		

};




class tmAAttribCtrlInteger : public tmAAttribCtrl
	{
	private:
		wxSpinCtrl * m_Control;
	protected:
	public:
		tmAAttribCtrlInteger();
		tmAAttribCtrlInteger(wxWindow * parent, wxWindowID id = wxID_ANY,
						  const wxPoint & 	pos = wxDefaultPosition,
						  const wxSize & 	size = wxDefaultSize,
						  long 	style = wxTAB_TRAVERSAL,
						  const wxString & 	name = _T("tmAAttribCtrl"));
		void Create (wxWindow * parent, wxWindowID id = wxID_ANY,
					 const wxPoint & 	pos = wxDefaultPosition,
					 const wxSize & 	size = wxDefaultSize,
					 long 	style = wxTAB_TRAVERSAL,
					 const wxString & 	name = _T("tmAAttribCtrl"));
		~tmAAttribCtrlInteger();
		
		
		// control operations
		virtual void SetControlValue(const wxString & value);
		virtual wxString GetControlValue ();
		
		// control properties
		virtual void SetProperties (const ProjectDefMemoryFields & fieldinfo);
		
		
	};




class tmAAttribCtrlFloat : public tmAAttribCtrl
	{
	private:
		wxTextCtrl * m_Control;
		wxStaticText * m_ControlInfo;
	protected:
	public:
		tmAAttribCtrlFloat();
		tmAAttribCtrlFloat(wxWindow * parent, wxWindowID id = wxID_ANY,
							 const wxPoint & 	pos = wxDefaultPosition,
							 const wxSize & 	size = wxDefaultSize,
							 long 	style = wxTAB_TRAVERSAL,
							 const wxString & 	name = _T("tmAAttribCtrl"));
		void Create (wxWindow * parent, wxWindowID id = wxID_ANY,
					 const wxPoint & 	pos = wxDefaultPosition,
					 const wxSize & 	size = wxDefaultSize,
					 long 	style = wxTAB_TRAVERSAL,
					 const wxString & 	name = _T("tmAAttribCtrl"));
		~tmAAttribCtrlFloat();
		
	
		// control operations
		virtual void SetControlValue(const wxString & value);
		virtual wxString GetControlValue ();
		
		// control properties
		virtual void SetProperties (const ProjectDefMemoryFields & fieldinfo);
		
		
	};


#endif
