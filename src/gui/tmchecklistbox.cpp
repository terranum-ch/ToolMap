/***************************************************************************
								tmchecklistbox.cpp
                    Base class for dealing with checklistbox with long
						item associed to every item
                             -------------------
    copyright            : (C) 2008 CREALP Lucien Schreiber 
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

#include "tmchecklistbox.h"



/***************************************************************************//**
 @brief Creator for two steps creation
 @author Lucien Schreiber (c) CREALP 2008
 see docs of wxCheckBoxList for parameter documentations
 @date 14 May 2008
 *******************************************************************************/
bool tmCheckListBox::Create(wxWindow * parent,
			wxWindowID id, 
			const wxPoint & pos,
			const wxSize & size,
			int nStrings,
			const wxString *choices,
			long style,
			const wxValidator& validator,
			const wxString& name) 
{
	if ( !wxCheckListBox::Create(parent, id, pos, size,
								 nStrings, choices, style, validator, name) )
	{
        return false;
	}
	
	return TRUE;
}


/***************************************************************************//**
 @brief Add an item in the list
 @details This add the item in the list and join the ID to this item.
 @param index The zero based position of the item to add, -1 means add item to
 the end of the list
 @param id the id to attach to this item
 @param name the name of the item
 @param checked Should we check this item, default is false
 @return  TRUE if item was added succesfully
 @author Lucien Schreiber (c) CREALP 2008
 @date 14 May 2008
 *******************************************************************************/
bool tmCheckListBox::AddItem (long index, long id,
							  const wxString & name, 
							  bool checked)
{
	Append(name);
	
	return TRUE;
}
