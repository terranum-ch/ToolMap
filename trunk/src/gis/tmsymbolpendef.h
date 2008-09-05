/***************************************************************************
								tmsymbolpendef.h
                    Pen and brush definitions for tm symbology
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


#ifndef _TM_SYMBOL_PENDEF_H_
#define _TM_SYMBOL_PENDEF_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif


/***************************************************************************//**
 @brief definitions of pen
 @details Used for drawing lines (#tmSymbolVectorLine) but also outline of
 polygons (#tmSymbolVectorPolygon)
 @author Lucien Schreiber (c) CREALP 2008
 @date 05 September 2008
 *******************************************************************************/
enum tmSYMBOLPENSYLES
{
	tmSOLID		= wxSOLID,
	tmDOT		= wxDOT,
	tmDASH		= wxLONG_DASH,
	tmDOT_DASH	= wxDOT_DASH,
};

/***************************************************************************//**
 @brief Name string of pen
 @details Used for drawing lines (#tmSymbolVectorLine) but also outline of
 polygons (#tmSymbolVectorPolygon)
 @author Lucien Schreiber (c) CREALP 2008
 @date 05 September 2008
 *******************************************************************************/
const wxString tmSYMBOLPENSTYLES_NAME[] =
{
_("Solid line"),
_("Dotted line"),
_("Dashed line"),
_("Dot-dashed line")
};


/***************************************************************************//**
 @brief Definitions of filling mode
 @details Used for filling polygons (#tmSymbolVectorPolygon)
 @author Lucien Schreiber (c) CREALP 2008
 @date 05 September 2008
 *******************************************************************************/
enum tmSYMBOLFILLSTYLES
{
	tmFILL_SOLID = wxSOLID,
	tmFILL_DIAGONAL_BACK = wxBDIAGONAL_HATCH,
	tmFILL_DIAGONAL_FWD = wxFDIAGONAL_HATCH,
	tmFILL_CROSS	= wxCROSS_HATCH,
	tmFILL_VERTICAL = wxVERTICAL_HATCH
};



/***************************************************************************//**
 @brief Name of filling modes
 @details Used for filling polygons (#tmSymbolVectorPolygon)
 @author Lucien Schreiber (c) CREALP 2008
 @date 05 September 2008
 *******************************************************************************/
const wxString tmSYMBOLFILLSTYLES_NAME[] =
{
	_("Solid fill"),
	_("Backward Diagonal hatch"),
	_("Forward Diagonal hatch"),
	_("Cross hatch"),
	_("Vertical hatch")
};


#endif


