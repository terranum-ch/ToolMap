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

// comment doxygen

#include "tmvalidator.h"

tmValidator::tmValidator(long style)
{
	if (style & tmFILTER_EXCLUDE_CHAR_RESTRICTED)
		SetCharRestricted();
}


void tmValidator::SetCharRestricted()
{
	wxArrayString mylist;
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
	
	mylist.Add(_T("1")); mylist.Add(_T("7"));
	mylist.Add(_T("2")); mylist.Add(_T("8"));
	mylist.Add(_T("3")); mylist.Add(_T("9"));
	mylist.Add(_T("4")); mylist.Add(_T("0"));
	mylist.Add(_T("5")); 
	mylist.Add(_T("6")); 
	
	mylist.Add(_T("-"));mylist.Add(_T("_"));
	
	SetStyle(wxFILTER_INCLUDE_CHAR_LIST);
	SetIncludes(mylist);

}

