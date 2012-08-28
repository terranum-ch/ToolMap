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
#include <wx/mstream.h>

class tmSymbol;
class tmSymbolRuleManager;

/***************************************************************************//**
 @brief Storing object of layer type
 @author Lucien Schreiber (c) CREALP 2008
 @date 07 July 2008
 *******************************************************************************/
class tmLayerProperties : public wxTreeItemData
	{
	private:
        wxFileName m_LayerName;
        long m_LayerID;
		TM_GIS_SPATIAL_TYPES m_LayerSpatialType;
 		bool m_LayerVisible;
		TOC_GENERIC_NAME m_LayerType;
		tmSymbol * m_LayerSymbol;
		int m_LayerVertexFlags;
		bool m_LayerEditing;
		tmSymbolRuleManager * m_SymbolRulesManager;
        
        void InitMemberValues();
		
	public:
		// init from string array
		bool InitFromArray(const wxArrayString & array, bool userelativepath, const wxString & prjpath);
		void InitSymbology (const wxString & itemBinSymbology);
		bool InitFromPathAndName (const wxString & path, const wxString & nameext,
								  const wxArrayString & supportedext);
		
        wxString GetNameDisplay();
        wxFileName GetName();
        void SetName(const wxFileName & filename){m_LayerName = filename;}
        inline const long GetID() const;
        void SetID(long value);
        inline const TM_GIS_SPATIAL_TYPES GetSpatialType() const;
        void SetSpatialType(TM_GIS_SPATIAL_TYPES value);
        inline const bool IsVisible() const;
        void SetVisible(bool value);
        inline const TOC_GENERIC_NAME GetType() const;
        void SetType(TOC_GENERIC_NAME value);
        inline const int GetVertexFlags() const;
        void SetVertexFlags(int value);
        inline const bool IsEditing() const;
        void SetEditing(bool value);
       
        tmSymbol * GetSymbolRef();
        void SetSymbolDirectly(tmSymbol * value);
        
        tmSymbolRuleManager * GetSymbolRuleManagerRef(){return m_SymbolRulesManager;}
		
		// constructor
		tmLayerProperties(){InitMemberValues();}
		tmLayerProperties (const tmLayerProperties & layerprop);
		
		~tmLayerProperties();
		
	};

inline const long tmLayerProperties::GetID() const {
    return m_LayerID;
}

inline const TM_GIS_SPATIAL_TYPES tmLayerProperties::GetSpatialType() const {
    return m_LayerSpatialType;
}

inline const bool tmLayerProperties::IsVisible() const {
    return m_LayerVisible;
}

inline const TOC_GENERIC_NAME tmLayerProperties::GetType() const {
    return m_LayerType;
}

inline const int tmLayerProperties::GetVertexFlags() const {
    return m_LayerVertexFlags;
}

inline const bool tmLayerProperties::IsEditing() const {
    return m_LayerEditing;
}






#endif
