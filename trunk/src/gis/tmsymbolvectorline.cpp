/***************************************************************************
								tmsymbolvectorline.cpp
				Deals with GIS line vector symbology and associed dialog
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

#include "tmsymbolvectorline.h"


tmSymbolVectorLine::tmSymbolVectorLine()
{
	
	
}



tmSymbolVectorLine::~tmSymbolVectorLine()
{
	
}


tmSymbolDLG * tmSymbolVectorLine::GetSymbolDialog (wxWindow * parent, const wxPoint & dlgpos)
{
	return new tmSymbolDLGLine(parent,SYMBOL_TMSYMBOLDLG_IDNAME,
							   SYMBOL_TMSYMBOLDLG_TITLE,
							   dlgpos);
}

