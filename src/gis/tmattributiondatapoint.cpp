/***************************************************************************
							tmattributiondatapoint.cpp
							Class for attributing point
								-------------------
    copyright            : (C) 2008 CREALP Lucien Schreiber 
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


#include "tmattributiondatapoint.h"


/***************************************************************************//**
 @brief Default constructor
 @details Use the tmAttributionDataPoint::Create() function 
 @author Lucien Schreiber (c) CREALP 2008
 @date 06 November 2008
 *******************************************************************************/
tmAttributionDataPoint::tmAttributionDataPoint()
{
}



/***************************************************************************//**
 @brief Constructor
 @param selected Adress of an array of selected geometry objects (must be valid)
 @param database Adress of a #DataBaseTM object
 @author Lucien Schreiber (c) CREALP 2008
 @date 06 November 2008
 *******************************************************************************/
tmAttributionDataPoint::tmAttributionDataPoint(wxArrayLong * selected,DataBaseTM * database)
{
	Create(selected, database);
}



/***************************************************************************//**
 @brief Function for two step creation
 @details If using the default constructor, one must use this function for
 initialization to take place
 @param selected Adress of an array of selected geometry objects (must be valid)
 @param database Adress of a #DataBaseTM object
 @author Lucien Schreiber (c) CREALP 2008
 @date 06 November 2008
 *******************************************************************************/
void tmAttributionDataPoint::Create (wxArrayLong * selected,DataBaseTM * database)
{
	tmAttributionData::Create(selected,database);
	SetDataBaseTable(TABLE_NAME_GIS_ATTRIBUTION[1]);
}



/***************************************************************************//**
 @brief Destructor
 @details doesn't do anything actually
 @author Lucien Schreiber (c) CREALP 2008
 @date 06 November 2008
 *******************************************************************************/
tmAttributionDataPoint::~tmAttributionDataPoint()
{
	
}



/***************************************************************************//**
 @brief Basic attribution
 @details This function save the basic attribution data into the database
 @param panel Adress of a #AttribObjType_PANEL for getting selected values
 @return  true if the attribution was saved sucessfully into the database, false
 otherwise (error message is sent in debug mode)
 @author Lucien Schreiber (c) CREALP 2008
 @date 06 November 2008
 *******************************************************************************/
bool tmAttributionDataPoint::SetAttributeBasic (AttribObjType_PANEL * panel)
{
	if (!IsValid())
		return false;
	
	wxArrayLong myChekedValues;
	wxString myStatement = _T("");
	if (GetPanelValues(panel, myChekedValues)) // attribution
	{
		PrepareAttributionStatement(myStatement,
									TABLE_NAME_GIS_ATTRIBUTION[1],
									&myChekedValues);
	}
	else // cleaning data
	{
		PrepareCleaningStatement(myStatement,
								 TABLE_NAME_GIS_ATTRIBUTION[1]);
	}
		
	
	if (m_pDB->DataBaseQueryNoResults(myStatement)==false)
	{
		wxLogDebug(_T("Error attributing data"));
		return false;
	}
	return true;
}



/***************************************************************************//**
 @brief Get all selected values from the panel
 @param valueids if true was returned, valueids contain all value choosen for
 attribution
 @param panel Adress of a #AttribObjType_PANEL used for getting values from
 @return  true if there is selected values (attribution), false otherwise
 (cleaning)
 @author Lucien Schreiber (c) CREALP 2008
 @date 06 November 2008
 *******************************************************************************/
bool tmAttributionDataPoint::GetPanelValues (AttribObjType_PANEL * panel, 
											wxArrayLong & valueids)
{
	panel->GetSelectedValues(TOC_NAME_POINTS, valueids);
	
	if (valueids.GetCount() > 0)
		return true;
	
	return false;
}



/***************************************************************************//**
 @brief Retrive the values info
 @details Call this function only if there is one feature selected
 @param panel Adress of a #AttribObjType_PANEL used for setting values to
 @return  true if info where returned (object contain attributes)
 @author Lucien Schreiber (c) CREALP 2008
 @date 06 November 2008
 *******************************************************************************/
bool tmAttributionDataPoint::GetInfoBasic (AttribObjType_PANEL * panel)
{
	// checking
	if (!IsValid())
		return false;

	// getting values
	wxString sStatement = _T("");
	PrepareGetInfoStatement(sStatement, TABLE_NAME_GIS_ATTRIBUTION[1]);
	
	if (!m_pDB->DataBaseQuery(sStatement))
	{
		wxLogDebug(_T("Error getting info"));
		return false;
	}
	
	wxArrayLong mySelValues;
	m_pDB->DataBaseGetResults(mySelValues);
		
	// updating panel
	SetPanelValues(panel, mySelValues);

	return true;
}


bool tmAttributionDataPoint::GetInfoBasic (long oid,  wxArrayLong & objid,
										   wxArrayLong & objcode, wxArrayString & objname){
	
	return _GetInfoBasic(oid, objid, objcode, objname, 1);
}


/***************************************************************************//**
 @brief Set Panel values
 @details 
 @param panel Adress of a #AttribObjType_PANEL used for setting values to
 @param valueids contain all attribution value
 @author Lucien Schreiber (c) CREALP 2008
 @date 06 November 2008
 *******************************************************************************/
void tmAttributionDataPoint::SetPanelValues (AttribObjType_PANEL * panel,
											const wxArrayLong & valueids)
{
	panel->SetSelectedValues(TOC_NAME_POINTS, valueids);
}


/***************************************************************************//**
 @brief Get Layer attributed for selected geometry
 @details See tmAttributionData::PrepareGetAttributionLayersID() for description.
 @author Lucien Schreiber (c) CREALP 2009
 @date 16 March 2009
 *******************************************************************************/
bool tmAttributionDataPoint::GetAttributionLayersID (const long & geomid,
													 tmLayerValueArray & layersid)
{
	return tmAttributionData::PrepareGetAttributionLayersID(geomid,
															layersid,
															TABLE_NAME_GIS_ATTRIBUTION[1],
															LAYER_SPATIAL_POINT);	
}

bool tmAttributionDataPoint::GetAttributionLayersIDFull (const long & geomid,
													 tmLayerValueArray & layersid)
{
	return tmAttributionData::PrepareGetAttributionLayersID(geomid,
															layersid,
															TABLE_NAME_GIS_ATTRIBUTION[1],
															wxNOT_FOUND);	
}

