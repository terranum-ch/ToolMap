/***************************************************************************
								tmaattribctrls.cpp
		Definition of advanced attribution controls such as the tmAAttribTree
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

#include "tmaattribctrls.h"



/***************************************************************************//**
 @brief Two step constructor.
 @details Call Create when this constructor is called to build up the
 tmAAttribTree
 @author Lucien Schreiber (c) CREALP 2009
 @date 05 March 2009
 *******************************************************************************/
tmAAttribTree::tmAAttribTree()
{
	
}


/***************************************************************************//**
 @brief Default constructor
 @author Lucien Schreiber (c) CREALP 2009
 @date 05 March 2009
 *******************************************************************************/
tmAAttribTree::tmAAttribTree(wxWindow * parent,wxWindowID id, const wxPoint & pos,
							 const wxSize & size,long style, const wxValidator & validator,
							 const wxString & name)
{
	Create(parent, id, pos, size, style, validator, name);
}



/***************************************************************************//**
 @brief Two step creation
 @details Whenever the control is created without any parameters, use Create to
 actually create it. Don't access the control's public methods before this is
 called
 @author Lucien Schreiber (c) CREALP 2009
 @date 05 March 2009
 *******************************************************************************/
bool tmAAttribTree::Create (wxWindow * parent,wxWindowID id, const wxPoint & pos,
			 const wxSize & size,long style, const wxValidator & validator,
			 const wxString & name)
{
	wxTreeMultiCtrl::Create(parent,id,pos,size,style,validator,name);
	
	return true;
}



/***************************************************************************//**
 @brief destructor
 @author Lucien Schreiber (c) CREALP 2009
 @date 05 March 2009
 *******************************************************************************/
tmAAttribTree::~tmAAttribTree()
{
	
}
