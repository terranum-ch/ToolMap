/***************************************************************************
								tmsymbolvector.cpp
				Deals with GIS vector symbology and associed dialog
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

#include "tmsymbolvector.h"
#include "tmsymbolvectorline.h"
#include "tmsymbolvectorlinemultiple.h"
#include "tmsymbolvectorpoint.h"
#include "tmsymbolvectorpolygon.h"


tmSymbolVector::tmSymbolVector()
{
	
	
}



tmSymbolVector::~tmSymbolVector()
{
	
}


tmSymbolDLG * tmSymbolVector::GetSymbolDialog (wxWindow * parent, const wxPoint & dlgpos)
{
	return new tmSymbolDLG(parent,SYMBOL_TMSYMBOLDLG_IDNAME,
							   SYMBOL_TMSYMBOLDLG_TITLE,
							   dlgpos);
}



/***************************************************************************//**
 @brief Get colour with transparency
 @param col the original colour
 @param trans the percent of transparency. 0% is opaque, 100% is fully
 transparent
 @return  The new colour containing the alpha chanel
 @author Lucien Schreiber (c) CREALP 2008
 @date 11 September 2008
 *******************************************************************************/
wxColour tmSymbolVector::GetColourWithTransparency (const wxColour & col, int trans)
{
	char cTrans = ConvertTransparency(trans);
	wxColour ColTrans (col.Red(), col.Green(), col.Blue(), cTrans);					   
	return ColTrans;
}

 

tmSymbolVector * tmSymbolVector::CreateSymbolVectorBasedOnType (TM_GIS_SPATIAL_TYPES spattype, TOC_GENERIC_NAME tocname)
{
	switch (spattype)
	{
		case LAYER_SPATIAL_LINE:
			if (tocname == TOC_NAME_LINES) {
				return new tmSymbolVectorLineMultiple();
			}
			else {
				return new tmSymbolVectorLine();
			}
			break;
		case LAYER_SPATIAL_POINT:
			return new tmSymbolVectorPoint();
			break;
		case LAYER_SPATIAL_POLYGON:
			return new tmSymbolVectorPolygon();
			break;
			
		default:
			wxLogDebug(_T("Symbology for this format not implemented now"));
			return NULL;
			break;
	}	
}



tmSymbolVector * tmSymbolVector::CreateCopySymbolVectorBasedOnType (TM_GIS_SPATIAL_TYPES  spattype,
																	tmSymbol * origin)
{
	switch (spattype)
	{
		case LAYER_SPATIAL_LINE:
			return new tmSymbolVectorLine(*((tmSymbolVectorLine*)origin));
			break;
		case LAYER_SPATIAL_POINT:
			return new tmSymbolVectorPoint(*((tmSymbolVectorPoint*)origin));
			break;
		case LAYER_SPATIAL_POLYGON:
			return new tmSymbolVectorPolygon(*((tmSymbolVectorPolygon*) origin));
			break;
			
		default:
			wxLogDebug(_T("Symbology for this format not implemented now"));
			return NULL;
			break;
	}
	
}

