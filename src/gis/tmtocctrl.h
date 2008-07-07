/***************************************************************************
							tmtocctrl.h
                    Contain description of the GIS toc ctrl
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


#ifndef _TM_TOCCTRL_H_
#define _TM_TOCCTRL_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif


#include "../core/projectdefmemory.h"	// for PRJDEF_LAYERS_TYPE
#include <wx/treectrl.h>


// TOC Generics values
// Definitions for the generic layers, we define the text and  the id of 
// each string.
static wxString TOC_GENERIC_NAME_STRING[] = 
{
	_("Lines"),
	_("Points"),
	_("Labels"),
	_("Annotations"),
	_("Not Generic layers")
};
enum TOC_GENERIC_NAME
{
	TOC_NAME_LINES = 0,
	TOC_NAME_POINTS,
	TOC_NAME_LABELS,
	TOC_NAME_ANNOTATIONS,
	TOC_NAME_NOT_GENERIC = 100
};
static const int TOC_GENERIC_NAME_NUMBER = 5;



/***************************************************************************//**
 @brief Storing object of layer type
 @author Lucien Schreiber (c) CREALP 2008
 @date 07 July 2008
 *******************************************************************************/
class tmLayerProperties : public wxObject
	{
	private:
		void InitMemberValues();
		
	public:
		long m_LayerID;
		PRJDEF_LAYERS_TYPE m_LayerType;
		wxString m_LayerPathOnly;
		wxString m_LayerNameExt;
		bool m_LayerVisible;
		int m_LayerIsGeneric;
		
		// init from string array
		bool InitFromArray(const wxArrayString & array);
		
		// constructor
		tmLayerProperties(){InitMemberValues();}
		~tmLayerProperties(){;}
	};

// Creating a list of MemoryFields
WX_DECLARE_OBJARRAY(tmLayerProperties, tmLayerPropertiesArray);

/***************************************************************************//**
 @brief GIS TOC class
 @details This object is in charge of all operations linked to the table of
 content (TOC)
 @author Lucien Schreiber (c) CREALP 2008
 @date 07 July 2008
 *******************************************************************************/
class tmTOCCtrl  : public wxTreeCtrl
	{
	private:
		wxTreeItemId m_root;
		tmLayerPropertiesArray m_TOCArray;
		
		// private functions
		void InitMemberValues();
		
	public:
		// construcor / destructor
		tmTOCCtrl (wxWindow * parent, wxWindowID id, wxSize size,  long style);
		~tmTOCCtrl(){;}
		
		// adding, removing layers functions
		void InsertProjectName (const wxString & prjname);
		bool InsertLayer(tmLayerProperties * item, long position = -1);
		//bool RemoveLayer (long position);
		void ClearAllLayers();
		//bool EditLayer (long position, tmLayerProperties & newitemdata);
		
		// moving item functions
		

		
	};



#endif
