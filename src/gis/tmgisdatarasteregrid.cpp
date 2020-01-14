/***************************************************************************
 tmgisdatarasteregrid.cpp
                    For dealing with raster ESRI's binary grid
 -------------------
 copyright : (C) 2007 CREALP Lucien Schreiber
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
    m_FileType = _T("GDAL ESRI's GRID");
    m_ClassType = tmGIS_RASTER_BINGRID;
}


tmGISDataRasterEGRID::~tmGISDataRasterEGRID()
{

}


/***************************************************************************//**
 @brief Get Size of The binary grid
 @param iPrecision The number of digits required (default is 2)
 @return  An html compliant string containing the Binary grid size in an human
 readable format
 @author Lucien Schreiber (c) CREALP 2008
 @date 24 October 2008
 *******************************************************************************/
wxString tmGISDataRasterEGRID::GetDataSizeAsHtml(int iPrecision)
{
    wxString myResult = _("<U><B>Binary GRID Size</B></U><BR>");

    // getting directory
    wxFileName myDirName(GetFullFileName());
    wxULongLong mySize = wxDir::GetTotalSize(myDirName.GetPath());

    if (mySize == wxInvalidSize) {
        myResult.Append(_("Error computing raster size<BR>"));
        return myResult;
    }

    // modifiy the size to be MB
    double dMegaBytes = (mySize.ToDouble() / 1024) / 1024;
    myResult.Append(wxString::Format(_("Raster size is : %.*f [Mb]<BR>"),
                                     iPrecision, dMegaBytes));
    return myResult;
}


