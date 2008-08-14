/***************************************************************************
								tmsymbolvectorpolygon.cpp
				Deals with GIS polygon vector symbology and associed dialog
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

#include "tmsymbolvectorpolygon.h"


tmSymbolVectorPolygon::tmSymbolVectorPolygon()
{
	
	
}



tmSymbolVectorPolygon::~tmSymbolVectorPolygon()
{
	
}


tmSymbolDLG * tmSymbolVectorPolygon::GetSymbolDialog (wxWindow * parent, const wxPoint & dlgpos)
{
	return new tmSymbolDLGPolygon(parent,SYMBOL_TMSYMBOLDLG_IDNAME,
							   SYMBOL_TMSYMBOLDLG_TITLE,
							   dlgpos);
}

