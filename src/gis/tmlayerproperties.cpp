/***************************************************************************
							tmlayerproperties.cpp
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

#include "tmlayerproperties.h"


/***************************************************************************//**
 @brief Default values for tmLayerProperties
 @author Lucien Schreiber (c) CREALP 2008
 @date 07 July 2008
 *******************************************************************************/
void tmLayerProperties::InitMemberValues()
{
	m_LayerID = 0;
	m_LayerSpatialType = LAYER_SPATIAL_LINE;
	m_LayerPathOnly = wxEmptyString;
	m_LayerNameExt = wxEmptyString;
	m_LayerVisible = TRUE;
	m_LayerType = TOC_NAME_NOT_GENERIC;
	m_LayerSymbol = NULL;
	m_DrawFlags = 0;
}


/***************************************************************************//**
 @brief Init from a string array
 @details This may be used for initing an #tmLayerProperties object from a
 string array. No verifications are made on the array size
 @param array string array containing 6 values
 @return  allways TRUE
 @author Lucien Schreiber (c) CREALP 2008
 @date 10 July 2008
 *******************************************************************************/
bool tmLayerProperties::InitFromArray(const wxArrayString & array)
{
	long temptype = 0;
	long tempstatus = 0;
	long tempgeneric = 0;
	
	array.Item(0).ToLong(&m_LayerID);
	array.Item(1).ToLong(&temptype);
	m_LayerSpatialType = (TM_GIS_SPATIAL_TYPES) temptype;
	
	m_LayerPathOnly = array.Item(2);
	m_LayerNameExt = array.Item(3);
	
	array.Item(4).ToLong(&tempstatus);
	m_LayerVisible = (bool) tempstatus;
	
	array.Item(5).ToLong(&tempgeneric);
	m_LayerType = (int) tempgeneric;
	
	InitSymbology(array.Item(6));
	
	wxString myVFlags = array.Item(7);
	m_DrawFlags = wxAtoi(myVFlags.c_str());
	
	
	return TRUE;
}



void tmLayerProperties::InitSymbology (const wxString & itemBinSymbology)
{
	// creating symbology object
	if (m_LayerSymbol)
	{
		wxLogError(_T("m_LayerSymbol allready defined, check code !"));
		return;
	}
	
	m_LayerSymbol = tmSymbol::CreateSymbolBasedOnType(m_LayerSpatialType);
	
	// unserialize object
	if (m_LayerSymbol && !itemBinSymbology.IsEmpty())
	{
		//wxLogDebug(itemBinSymbology);
		tmSerialize in(itemBinSymbology);
		m_LayerSymbol->Serialize(in);
	}
}



tmLayerProperties::~tmLayerProperties()
{
	if(m_LayerSymbol)
		delete m_LayerSymbol;
}



/***************************************************************************//**
 @brief Init name, path and type member
 @details Init member based on path, name (with extension) and a list of
 supported GIS extension. Those extensions may be retreived using the function
 bellow.
 @code
 wxArrayString myAllExt = tmGISData::GetAllSupportedGISFormatsExtensions();
 @endcode
 @param path string containing the path
 @param nameext string containing the name and extension
 @param supportedext an array of all supported extension. See code above
 @return  TRUE if name and path aren't empty
 @author Lucien Schreiber (c) CREALP 2008
 @date 06 August 2008
 *******************************************************************************/
bool tmLayerProperties::InitFromPathAndName (const wxString & path, 
											 const wxString & nameext,
											 const wxArrayString & supportedext)
{
	if (nameext.IsEmpty() || path.IsEmpty())
		return FALSE;
	
	m_LayerNameExt = nameext;
	m_LayerPathOnly = path;
	
	
	// init extension based on supported extension, TOC_NAME_UNKNOWN otherwise
	wxFileName myfileName (nameext);
	wxString myExt = myfileName.GetExt().MakeLower();
	
	for (unsigned int i= 0; i<supportedext.GetCount(); i++)
	{
		//wxLogDebug(_T("supported extension : %s"), supportedext.Item(i).c_str());
		if (supportedext.Item(i).Contains(myExt))
		{
			m_LayerType = TOC_NAME_NOT_GENERIC + i + 1;
			break;
		}
		
	}
	
	
	
	
	// nothing found -> unknown
	if (m_LayerType == TOC_NAME_NOT_GENERIC)
		m_LayerType = TOC_NAME_UNKNOWN;
	
	
	return TRUE;
}




wxString tmLayerProperties::GetFileExtension()
{
	wxFileName filename (m_LayerNameExt);
	return filename.GetExt();
}



/***************************************************************************//**
 @brief Return the display name for using in the TOC
 @details Because in some case file name isn't very explicit we sould use this
 function for getting a good display name (exemple is ESRI binary grid)
 @return  A more explicit name
 @author Lucien Schreiber (c) CREALP 2008
 @date 06 August 2008
 *******************************************************************************/
wxString tmLayerProperties::GetDisplayName ()
{
	wxFileName myDirName (m_LayerPathOnly, m_LayerNameExt);
	
	// special behaviour for ESRI grid.
	if (m_LayerType == TOC_NAME_EGRID)
	{
		if (myDirName.IsOk())
		{
			return myDirName.GetDirs().Last();
		}
		
		return _T("DIR_NAME_ERROR");
	}
	return m_LayerNameExt;
}



/***************************************************************************//**
 @brief Copy constructor
 @author Lucien Schreiber (c) CREALP 2008
 @date 27 October 2008
 *******************************************************************************/
tmLayerProperties::tmLayerProperties (const tmLayerProperties & layerprop)
{
	// just to be sure
	InitMemberValues();
	
	m_LayerID = layerprop.m_LayerID;
	m_LayerSpatialType = layerprop.m_LayerSpatialType;
	m_LayerPathOnly = layerprop.m_LayerPathOnly;
	m_LayerNameExt = layerprop.m_LayerNameExt;
	m_LayerVisible = layerprop.m_LayerVisible;
	m_LayerType = layerprop.m_LayerType;
	
	m_LayerSymbol = tmSymbol::CreateCopySymbolBasedOnType(m_LayerSpatialType,
														  layerprop.m_LayerSymbol);
	
	
	m_DrawFlags = layerprop.m_DrawFlags;
	
}


