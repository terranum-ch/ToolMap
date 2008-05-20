/***************************************************************************
								tmchecklistboxrank.h
		Deals with checklistbox but add the ability to save the order
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


#ifndef _TMCHECKLISTBOXRANK_H_
#define _TMCHECKLISTBOXRANK_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "tmchecklistbox.h"

// menu items
const int tmCHECK_MENU_SAVE_RANK = 10005;


/***************************************************************************//**
 @brief Enhance the #tmCheckListBox class
 @details This derivated class add the ability to save the order of the list in
 a database in a rank column
 @todo add sample here !
 @see For all database function #DataBaseTM
 @author Lucien Schreiber (c) CREALP 2008
 @date 20 May 2008
 *******************************************************************************/
class tmCheckListBoxRank : public tmCheckListBox
	{
	private:
		void Init();
		
		bool AddToMenu();
		
	protected:
		
	public:
		~tmCheckListBoxRank();
		tmCheckListBoxRank(wxWindow * parent,
						   wxWindowID id,
						   const wxPoint & pos = wxDefaultPosition,
						   const wxSize & size = wxDefaultSize,
						   int nStrings = 0,
						   const wxString * choices = NULL,
						   long style = 0,
						   const wxValidator & validator = wxDefaultValidator,
						   const wxString & name = wxListBoxNameStr);
		
		bool Create(wxWindow * parent,
					wxWindowID id, 
					const wxPoint & pos = wxDefaultPosition,
					const wxSize & size = wxDefaultSize,
					int nStrings = 0,
					const wxString *choices = NULL,
					long style = 0,
					const wxValidator& validator = wxDefaultValidator,
					const wxString& name = wxListBoxNameStr);
	};


#endif
