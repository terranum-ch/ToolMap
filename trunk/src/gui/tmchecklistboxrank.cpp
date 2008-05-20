/***************************************************************************
								tmchecklistboxrank.cpp
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

#include "tmchecklistboxrank.h"

/***************************************************************************//**
 @brief Constructor for one steps creation
 @details see documentation for wxCheckListBox for parameters description
 @author Lucien Schreiber (c) CREALP 2008
 @date 20 May 2008
 *******************************************************************************/
tmCheckListBoxRank::tmCheckListBoxRank(wxWindow * parent,
									   wxWindowID id,
									   const wxPoint & pos ,
									   const wxSize & size ,
									   int nStrings ,
									   const wxString * choices,
									   long style ,
									   const wxValidator & validator ,
									   const wxString & name )
{
	Init();
	Create(parent, id, pos, size, nStrings, choices, style, validator, name);
}




/***************************************************************************//**
 @brief Constructor for two steps creation
 @details see documentation for wxCheckListBox for parameters description
 @author Lucien Schreiber (c) CREALP 2008
 @date 20 May 2008
 *******************************************************************************/
bool tmCheckListBoxRank::Create(wxWindow * parent,
			wxWindowID id, 
			const wxPoint & pos ,
			const wxSize & size ,
			int nStrings ,
			const wxString *choices ,
			long style ,
			const wxValidator& validator ,
			const wxString& name )
{
	
	if (!tmCheckListBox::Create(parent, id, pos, size, nStrings, choices, style, validator,name))
	{
		return FALSE;
	}
	
	if (style & tmLB_MENU)
	{
		// add menu entry and attach them to functions
		if (!AddToMenu())
		{
			return FALSE;
		}
		
		
		
		
	}
	else // tmLB_MENU not defined, this may cause problems
	{
		wxLogDebug(_T("Style menu not defined for tmCheckListboxRank, no enhanced functions actives"));
		return FALSE;
	}
	
	return TRUE;
}



/***************************************************************************//**
 @brief Destructor
 Do nothing actually
 @author Lucien Schreiber (c) CREALP 2008
 @date 20 May 2008
 *******************************************************************************/
tmCheckListBoxRank::~tmCheckListBoxRank()
{
	
}



/***************************************************************************//**
 @brief Init member values
 @author Lucien Schreiber (c) CREALP 2008
 @date 20 May 2008
 *******************************************************************************/
void tmCheckListBoxRank::Init()
{

}


/***************************************************************************//**
 @brief Add entry to the popup menu
 @details Following entry are added to the popup menu if the menu exists
 - Saving list order
 @return  TRUE if the menu exists
 @author Lucien Schreiber (c) CREALP 2008
 @date 20 May 2008
 *******************************************************************************/
bool tmCheckListBoxRank::AddToMenu()
{
	wxMenu * popupmenu = GetPopupMenu();
	
	if (popupmenu==NULL)
	{
		wxLogDebug(_T("Error : Pointer to the popup menu is NULL"));
		return FALSE;
	}
	
	popupmenu->AppendSeparator();
	popupmenu->Append(tmCHECK_MENU_SAVE_RANK, _("Save order"), wxEmptyString);
	return TRUE;
}



