/***************************************************************************
								tmvalidator.cpp
							Create various validator
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


#include "tmvalidator.h"


/***************************************************************************//**
 @brief Constructor
 @details Construct a control validator, all style referenced for wxValidator
 (see wxWidgets doc) may be used plus the following :
 - #tmFILTER_EXCLUDE_CHAR_RESTRICTED : allow only character (without accents) and
 numerics plus underscore and "-".
 - #tmFILTER_EXCLUDE_CHAR_DATABASE : allow
 the same as above minus the "-" character wich isn't compatible with MySQL's
 database
 - #tmFILTER_EXCLUDE_CHAR_NUMERIC_STRICT : allow only numeric char
 (0123456789) nothing else. wxFILTER_NUMERIC is more flexible, it allows some
 character like "." or "e".
 @param style one of the style described above
 @author Lucien Schreiber (c) CREALP 2008
 @date 20 May 2008
 *******************************************************************************/
tmValidator::tmValidator(long style)
{
	// if we arn't trying to use one of the new 
	// style, then use normal style
	if (!SetEnhancedStyle(style))	
		m_validatorStyle = style;
}



/***************************************************************************//**
 @brief Create a char restricted validator
 @details used for style = #tmFILTER_EXCLUDE_CHAR_RESTRICTED
 @author Lucien Schreiber (c) CREALP 2008
 @date 20 May 2008
 *******************************************************************************/
void tmValidator::SetCharRestricted()
{
	wxArrayString mylist;
	GetAlphaList(mylist);
	GetStrictNumList(mylist);
	
	mylist.Add(_T("-"));mylist.Add(_T("_"));
	
	SetStyle(wxFILTER_INCLUDE_CHAR_LIST);
	SetIncludes(mylist);
}


/***************************************************************************//**
 @brief Create a database restricted validator
 @details used for style = #tmFILTER_EXCLUDE_CHAR_DATABASE
 @author Lucien Schreiber (c) CREALP 2008
 @date 20 May 2008
 *******************************************************************************/
void tmValidator::SetCharDataBase ()
{
	wxArrayString mylist;
	GetAlphaList(mylist);
	GetStrictNumList(mylist);
	
	mylist.Add(_T("_"));
	
	SetStyle(wxFILTER_INCLUDE_CHAR_LIST);
	SetIncludes(mylist);	
}


/***************************************************************************//**
 @brief Create a strict numeric validator
 @details used for style = #tmFILTER_EXCLUDE_CHAR_NUMERIC_STRICT
 @author Lucien Schreiber (c) CREALP 2008
 @date 20 May 2008
 *******************************************************************************/
void tmValidator::SetCharNumericStric()
{
	wxArrayString mylist;
	GetStrictNumList(mylist);
	
	SetStyle(wxFILTER_INCLUDE_CHAR_LIST);
	SetIncludes(mylist);
}


/***************************************************************************//**
 @brief Is the passed style an enhanced one
 @details Create an object of the enhanced style if needed. Otherwise we use the
 standard wxTextValidator styles
 @param style one of the style. Allowed style are all of the wxTextValidator
 style plus those described in the tmValidator::tmValidator() constructor
 @return  TRUE if an enhanced style was passed, FALSE otherwise
 @author Lucien Schreiber (c) CREALP 2008
 @date 20 May 2008
 *******************************************************************************/
bool tmValidator::SetEnhancedStyle (long style)
{
	if (style & tmFILTER_EXCLUDE_CHAR_RESTRICTED)
	{
		SetCharRestricted();
		return TRUE;
	}
	
	if (style & tmFILTER_EXCLUDE_CHAR_DATABASE)
	{
		SetCharDataBase();
		return TRUE;
	}

	if (style & tmFILTER_EXCLUDE_CHAR_NUMERIC_STRICT)
	{
		SetCharNumericStric();
		return TRUE;
	}
	
	if (style & tmFILTER_EXCLUDE_CHAR_COMPLEX) {
		wxArrayString myList;
		GetExcludeChars(myList);
		SetStyle(wxFILTER_EXCLUDE_CHAR_LIST);
		SetExcludes(myList);
		return true;
	}
	
	return FALSE;
}


/***************************************************************************//**
 @brief Add numeric values to array
 @param mylist Reference to a wxArrayString. Numeric values (0123456789) will be
 added into this array when calling this function
 @author Lucien Schreiber (c) CREALP 2008
 @date 20 May 2008
 *******************************************************************************/
void tmValidator::GetStrictNumList (wxArrayString & mylist)
{
	mylist.Add(_T("1")); mylist.Add(_T("7"));
	mylist.Add(_T("2")); mylist.Add(_T("8"));
	mylist.Add(_T("3")); mylist.Add(_T("9"));
	mylist.Add(_T("4")); mylist.Add(_T("0"));
	mylist.Add(_T("5")); 
	mylist.Add(_T("6")); 
}


/***************************************************************************//**
 @brief Add alphabetic values to array
 @param mylist Reference to a wxArrayString. Alphabetic values (upper and lower
 case) will be added into this array when calling this function
 @author Lucien Schreiber (c) CREALP 2008
 @date 20 May 2008
 *******************************************************************************/
void tmValidator::GetAlphaList (wxArrayString & mylist)
{
	mylist.Add(_T("a")); mylist.Add(_T("A"));
	mylist.Add(_T("b")); mylist.Add(_T("B"));
	mylist.Add(_T("c")); mylist.Add(_T("C"));
	mylist.Add(_T("d")); mylist.Add(_T("D"));
	mylist.Add(_T("e")); mylist.Add(_T("E"));
	mylist.Add(_T("f")); mylist.Add(_T("F"));
	mylist.Add(_T("g")); mylist.Add(_T("G"));
	mylist.Add(_T("h")); mylist.Add(_T("H"));
	mylist.Add(_T("i")); mylist.Add(_T("I"));
	mylist.Add(_T("j")); mylist.Add(_T("J"));
	mylist.Add(_T("k")); mylist.Add(_T("K"));
	mylist.Add(_T("l")); mylist.Add(_T("L"));
	mylist.Add(_T("m")); mylist.Add(_T("M"));
	mylist.Add(_T("n")); mylist.Add(_T("N"));
	mylist.Add(_T("o")); mylist.Add(_T("O"));
	mylist.Add(_T("p")); mylist.Add(_T("P"));
	mylist.Add(_T("q")); mylist.Add(_T("Q"));
	mylist.Add(_T("r")); mylist.Add(_T("R"));
	mylist.Add(_T("s")); mylist.Add(_T("S"));
	mylist.Add(_T("t")); mylist.Add(_T("T"));
	mylist.Add(_T("u")); mylist.Add(_T("U"));
	mylist.Add(_T("v")); mylist.Add(_T("V"));
	mylist.Add(_T("w")); mylist.Add(_T("W"));
	mylist.Add(_T("x")); mylist.Add(_T("X"));
	mylist.Add(_T("y")); mylist.Add(_T("Y"));
	mylist.Add(_T("z")); mylist.Add(_T("Z"));
}


void tmValidator::GetComplexList (wxArrayString & mylist){
	mylist.Add(_T("´")); mylist.Add(_T("`"));
	mylist.Add(_T("ê")); mylist.Add(_T("^"));
	mylist.Add(_T("â")); mylist.Add(_T("ç"));
	mylist.Add(_T("ä")); mylist.Add(_T("ö"));
	mylist.Add(_T("ô")); mylist.Add(_T("+"));
	mylist.Add(_T("ü")); mylist.Add(_T(" "));
	mylist.Add(_T("_")); mylist.Add(_T("-"));
	mylist.Add(_T(",")); mylist.Add(_T("."));
	mylist.Add(_T("'")); mylist.Add(_T("ï"));
	mylist.Add(_T("\x00E9"));
}

void tmValidator::GetExcludeChars (wxArrayString & mylist){
	mylist.Add(_T("\x0091")); mylist.Add(_T("\x0092"));
	mylist.Add(_T("\x0093")); mylist.Add(_T("\x0094"));
	mylist.Add(_T("\x0023")); mylist.Add(_T("\x0025"));
	mylist.Add(_T("\x002f")); mylist.Add(_T("\x0026"));
	mylist.Add(_T("\x05f3")); mylist.Add(_T("’"));
}

