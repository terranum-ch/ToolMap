/***************************************************************************
							tmattributiondatalabel.cpp
							Class for attributing label
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


#include "tmattributiondatalabel.h"


/***************************************************************************//**
 @brief Default constructor
 @details Use the tmAttributionDataLabel::Create() function 
 @author Lucien Schreiber (c) CREALP 2008
 @date 06 November 2008
 *******************************************************************************/
tmAttributionDataLabel::tmAttributionDataLabel()
{
}



/***************************************************************************//**
 @brief Constructor
 @param selected Adress of an array of selected geometry objects (must be valid)
 @param database Adress of a #DataBaseTM object
 @author Lucien Schreiber (c) CREALP 2008
 @date 06 November 2008
 *******************************************************************************/
tmAttributionDataLabel::tmAttributionDataLabel(wxArrayLong * selected,DataBaseTM * database)
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
void tmAttributionDataLabel::Create (wxArrayLong * selected,DataBaseTM * database)
{
	tmAttributionData::Create(selected,database);
	SetDataBaseTable(TABLE_NAME_GIS_ATTRIBUTION[2]);
}



/***************************************************************************//**
 @brief Destructor
 @details doesn't do anything actually
 @author Lucien Schreiber (c) CREALP 2008
 @date 06 November 2008
 *******************************************************************************/
tmAttributionDataLabel::~tmAttributionDataLabel()
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
bool tmAttributionDataLabel::SetAttributeBasic (AttribObjType_PANEL * panel)
{
	if (!IsValid())
		return false;
	
	wxArrayLong myChekedValues;
	wxString myStatement = _T("");
	if (GetPanelValues(panel, myChekedValues)) // attribution
	{
		PrepareAttributionStatement(myStatement,
									TABLE_NAME_GIS_ATTRIBUTION[2],
									&myChekedValues);
	}
	else // cleaning data
	{
		PrepareCleaningStatement(myStatement,
								 TABLE_NAME_GIS_ATTRIBUTION[2]);
	}
		
	
	if (m_pDB->DataBaseQueryNoResults(myStatement)==false)
	{
		wxLogDebug(_T("Error attributing data "));
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
bool tmAttributionDataLabel::GetPanelValues (AttribObjType_PANEL * panel, 
											wxArrayLong & valueids)
{
	// get values for frequent lines and less frequent
	panel->GetSelectedValues(TOC_NAME_LABELS, valueids, false);
	panel->GetSelectedValues(TOC_NAME_LABELS, valueids, true);
	
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
bool tmAttributionDataLabel::GetInfoBasic (AttribObjType_PANEL * panel)
{
	// checking
	if (!IsValid())
		return false;

	// getting values
	wxString sStatement = _T("");
	PrepareGetInfoStatement(sStatement, TABLE_NAME_GIS_ATTRIBUTION[2]);
	if (m_pDB->DataBaseQuery(sStatement)==false)
	{
		wxLogDebug(_T("Error getting info"));
		return false;
	}
	
	wxArrayLong mySelValues;
	if (m_pDB->DataBaseGetResults(mySelValues)==false)
		return false;
	
	// updating panel
	SetPanelValues(panel, mySelValues);
	return true;
}



/***************************************************************************//**
 @brief Set Panel values
 @details 
 @param panel Adress of a #AttribObjType_PANEL used for setting values to
 @param valueids contain all attribution value
 @author Lucien Schreiber (c) CREALP 2008
 @date 06 November 2008
 *******************************************************************************/
void tmAttributionDataLabel::SetPanelValues (AttribObjType_PANEL * panel,
											const wxArrayLong & valueids)
{
	panel->SetSelectedValues(TOC_NAME_LABELS, valueids);
}



/***************************************************************************//**
 @brief Get Layer attributed for selected geometry
 @details See tmAttributionData::PrepareGetAttributionLayersID() for description.
 @author Lucien Schreiber (c) CREALP 2009
 @date 16 March 2009
 *******************************************************************************/
bool tmAttributionDataLabel::GetAttributionLayersID (const long & geomid,
													 wxArrayLong & layersid)
{
	return tmAttributionData::PrepareGetAttributionLayersID(geomid,
															layersid,
															TABLE_NAME_GIS_ATTRIBUTION[2],
															LAYER_SPATIAL_POLYGON);	
}


