/***************************************************************************
								tmaattribwindow.h
                    Display the Advanced Attribution Window
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


#ifndef _TM_AATTRIBWINDOW_H_
#define _TM_AATTRIBWINDOW_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif


#include <wx/treectrl.h>	//FIXME: Remove this header when using wxTreeMultiCtrl




/***************************************************************************//**
 @brief Advanced Attribution Window
 @details This class deals with the advanced attribution window. It call the
 tmAAttribTree and display dynamic control based on user created database model
 @author Lucien Schreiber (c) CREALP 2009
 @date 05 March 2009
 *******************************************************************************/
class tmAAttribWindow : public wxDialog 
	{
	private:
		// member controls
		wxTreeCtrl* m_AAttribTree;
		
		
		// member function
		void InitMemberValue ();
		void CreateControls ();
		
	protected:
		
	public:
		tmAAttribWindow( wxWindow* parent, wxWindowID id = wxID_ANY,
						const wxString& title = wxEmptyString,
						const wxPoint& pos = wxDefaultPosition,
						const wxSize& size = wxDefaultSize,
						long style = wxDEFAULT_DIALOG_STYLE|wxMAXIMIZE_BOX|wxRESIZE_BORDER );
		~tmAAttribWindow();
		
	};






#endif
