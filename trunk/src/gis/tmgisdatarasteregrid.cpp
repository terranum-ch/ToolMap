/***************************************************************************
								tmgisdatarasteregrid.cpp
                    For dealing with raster ESRI's binary grid
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

#include "tmgisdatarasteregrid.h"


tmGISDataRasterEGRID::tmGISDataRasterEGRID()
{
	
	
}


tmGISDataRasterEGRID::~tmGISDataRasterEGRID()
{

}



bool tmGISDataRasterEGRID::Open (const wxString & filename, bool bReadWrite)
{
	// init parent member values
	tmGISData::Open(filename, bReadWrite);
	
	// convert utf wxString into char *
	//const char* ascii_str = 
	char * buffer = new char [filename.Length()+2];
	strcpy(buffer, (const char*)filename.mb_str(wxConvUTF8));
	
	// open the raster and return true if success
	m_DataSet = (GDALDataset*) GDALOpen(buffer, (GDALAccess) bReadWrite);
	if (m_DataSet == NULL)
	{
		//TODO: Change this with sprintf version for threads...
		wxLogDebug(_T("Unable to open tiff : %s"), filename.c_str());
		return FALSE;
	}
	
	return TRUE;
}



tmRealRect tmGISDataRasterEGRID::GetMinimalBoundingRectangle()
{
	// ASSERT m_DataSet
	wxString sFunction = wxString::FromAscii(__FUNCTION__);
	wxString sFunctionLineError = wxString::Format( _T("%s line %d : "),
												   sFunction.c_str(), __LINE__); 
	wxString sErrMsg = wxString::Format(_T("%s Error, m_DataSet not defined"),
										sFunctionLineError.c_str());
	wxASSERT_MSG(m_DataSet,sErrMsg);
	
	// getting bounding box
	double dCoord[6];
	if (m_DataSet->GetGeoTransform(dCoord) != CE_None)
		return tmRealRect(0,0,0,0);
	
	// selecting band 1
	m_RasterBand = m_DataSet->GetRasterBand(1);
	
	// computing bounding box
	
	tmRealRect myCoord (0,0,0,0);
	
	myCoord.x_min = dCoord[0];
	myCoord.y_max = dCoord[3];
	myCoord.y_min = myCoord.y_max + (m_DataSet->GetRasterYSize() * dCoord[5]);
	myCoord.x_max = myCoord.x_min + (m_DataSet->GetRasterXSize() * dCoord[1]);
	
	return myCoord;
}


