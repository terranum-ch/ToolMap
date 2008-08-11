/***************************************************************************
									tmsymbol.cpp
				Deals with GIS symbology and associed dialog
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

#include "tmsymbol.h"

tmSymbol::tmSymbol()
{
	
	
}



tmSymbol::~tmSymbol()
{
	
	
}



int tmSymbol::ShowSymbologyDialog (wxWindow * parent, const wxPoint & dlgpos)
{

	tmSymbolDLGLine * mydlg = new tmSymbolDLGLine(parent, SYMBOL_TMSYMBOLDLG_IDNAME,
										  SYMBOL_TMSYMBOLDLG_TITLE, 
										  dlgpos);
	//mydlg->CreateControlsLine();
	//mydlg->CreateControlsPoint();
	//mydlg->CreateControlsPolygon();
	//mydlg->CreateControlsRaster();
	if (mydlg->ShowModal() == wxID_OK)
	{
		wxLogDebug(_T("dialog : OK"));
	}
		
	delete mydlg;
	
	return 0;
}

