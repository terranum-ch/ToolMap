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
#include "tmlayerpropertiesdef.h"
#include "tmsymbol.h"



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
		tmSymbol * m_LayerSymbol;
		
		// init from string array
		bool InitFromArray(const wxArrayString & array);
		void InitSymbology ();
		bool InitFromPathAndName (const wxString & path, const wxString & nameext,
								  const wxArrayString & supportedext);
		wxString GetFileExtension ();
		wxString GetDisplayName ();
		
		
		// constructor
		tmLayerProperties(){InitMemberValues();}
		~tmLayerProperties();
	};






#endif
