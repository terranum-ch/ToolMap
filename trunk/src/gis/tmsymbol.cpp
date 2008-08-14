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
#include "tmsymbolvector.h"
#include "tmsymbolraster.h"

tmSymbol::tmSymbol()
{
	
	
}



tmSymbol::~tmSymbol()
{
	
	
}


tmSymbolDLG *  tmSymbol::GetSymbolDialog (wxWindow * parent, const wxPoint & dlgpos)
{
	return new tmSymbolDLG(parent,SYMBOL_TMSYMBOLDLG_IDNAME,
						   SYMBOL_TMSYMBOLDLG_TITLE,
						   dlgpos);
	
}


int tmSymbol::ShowSymbologyDialog (wxWindow * parent, const wxPoint & dlgpos)
{
	tmSymbolDLG * mydlg = GetSymbolDialog(parent,dlgpos);

	if (mydlg->ShowModal() == wxID_OK)
	{
		wxLogDebug(_T("dialog : OK"));
	}
		
	delete mydlg;
	
	return 0;
}


tmSymbol * tmSymbol::CreateSymbolBasedOnType (TM_GIS_SPATIAL_TYPES spattype)
{
	switch (spattype)
	{
		case LAYER_SPATIAL_LINE:
		case LAYER_SPATIAL_POINT:
		case LAYER_SPATIAL_POLYGON:
			return tmSymbolVector::CreateSymbolVectorBasedOnType(spattype);
			break;
			
		case LAYER_SPATIAL_RASTER:
			return new tmSymbolRaster();
			break;
			
		default:
			return new tmSymbol();
			break;
	}
	
}
