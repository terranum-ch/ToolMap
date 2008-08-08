/***************************************************************************
								tmlayerproperties.h
                    Class used for managing layer informations 
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


#ifndef _TM_LAYERPROPERTIES_H_
#define _TM_LAYERPROPERTIES_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <wx/treectrl.h>
#include <wx/filename.h>

// TOC Generics values
// Definitions for the generic layers, we define the text and  the id of 
// each string.
static wxString TOC_GENERIC_NAME_STRING[] = 
{
	_("Lines"),
	_("Points"),
	_("Labels"),
	_("Notes"),
	_("Frame"),
	_("Not Generic layers")
};


// warning, max values are 127 because stored in a tiny int
// into the database
enum TOC_GENERIC_NAME
{
	TOC_NAME_LINES = 0,
	TOC_NAME_POINTS,
	TOC_NAME_LABELS,
	TOC_NAME_ANNOTATIONS,
	TOC_NAME_FRAME,
	TOC_NAME_NOT_GENERIC = 100,
	TOC_NAME_TIFF,
	TOC_NAME_EGRID,
	TOC_NAME_SHP,
	TOC_NAME_UNKNOWN = 120
};
static const int TOC_GENERIC_NAME_NUMBER = 6;



enum TM_GIS_SPATIAL_TYPES
{
	LAYER_SPATIAL_LINE = 0,
	LAYER_SPATIAL_POINT,
	LAYER_SPATIAL_POLYGON,
	LAYER_SPATIAL_RASTER,
	LAYER_SPATIAL_UNKNOWN,
	LAYER_ERR
};


/***************************************************************************//**
 @brief Storing object of layer type
 @author Lucien Schreiber (c) CREALP 2008
 @date 07 July 2008
 *******************************************************************************/
class tmLayerProperties : public wxTreeItemData
	{
	private:
		void InitMemberValues();
		
	public:
		long m_LayerID;
		TM_GIS_SPATIAL_TYPES m_LayerSpatialType;
		wxString m_LayerPathOnly;
		wxString m_LayerNameExt;
		bool m_LayerVisible;
		int m_LayerType;
		
		// init from string array
		bool InitFromArray(const wxArrayString & array);
		bool InitFromPathAndName (const wxString & path, const wxString & nameext,
								  const wxArrayString & supportedext);
		wxString GetFileExtension ();
		wxString GetDisplayName ();
		
		
		// constructor
		tmLayerProperties(){InitMemberValues();}
		~tmLayerProperties(){;}
	};






#endif
