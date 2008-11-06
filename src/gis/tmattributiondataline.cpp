/***************************************************************************
							tmattributiondataline.cpp
							Class for attributing line
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


#include "tmattributiondataline.h"


/***************************************************************************//**
 @brief Default constructor
 @details Use the tmAttributionDataLine::Create() function 
 @author Lucien Schreiber (c) CREALP 2008
 @date 06 November 2008
 *******************************************************************************/
tmAttributionDataLine::tmAttributionDataLine()
{
}



/***************************************************************************//**
 @brief Constructor
 @param selected Adress of an array of selected geometry objects (must be valid)
 @param database Adress of a #DataBaseTM object
 @author Lucien Schreiber (c) CREALP 2008
 @date 06 November 2008
 *******************************************************************************/
tmAttributionDataLine::tmAttributionDataLine(wxArrayLong * selected,DataBaseTM * database)
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
void tmAttributionDataLine::Create (wxArrayLong * selected,DataBaseTM * database)
{
	tmAttributionData::Create(selected,database);
	SetDataBaseTable(_T("generic_aat"));
}



/***************************************************************************//**
 @brief Destructor
 @details doesn't do anything actually
 @author Lucien Schreiber (c) CREALP 2008
 @date 06 November 2008
 *******************************************************************************/
tmAttributionDataLine::~tmAttributionDataLine()
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
bool tmAttributionDataLine::SetAttributeBasic (AttribObjType_PANEL * panel)
{
	if (!IsValid())
		return false;
	
	return true;
}



/***************************************************************************//**
 @brief Get all selected values from the panel
 @param valueids if true was returned, valueids contain all value choosen for
 attribution
 @return  true if there is selected values (attribution), false otherwise
 (cleaning)
 @author Lucien Schreiber (c) CREALP 2008
 @date 06 November 2008
 *******************************************************************************/
bool tmAttributionDataLine::GetPanelValues (wxArrayLong & valueids)
{
	return true;
}


