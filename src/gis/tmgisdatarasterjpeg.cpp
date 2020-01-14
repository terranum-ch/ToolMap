/***************************************************************************
 tmgisdatarasterjpeg.cpp
                   For dealing with spatial raster data (*.jpg, *.jpeg)
 -------------------
 copyright : (C) 2009 CREALP Lucien Schreiber
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

#include "tmgisdatarasterjpeg.h"


tmGISDataRasterJPEG::tmGISDataRasterJPEG()
{
    m_FileType = _T("GDAL JPEG Raster");
    m_ClassType = tmGIS_RASTER_JPEG;
}

tmGISDataRasterJPEG::~tmGISDataRasterJPEG()
{

}


/***************************************************************************//**
 @brief Get Size of The Jpeg raster
 @param iPrecision The number of digits required (default is 2)
 @return  An html compliant string containing the Jpeg size in an human
 readable format
 @author Lucien Schreiber (c) CREALP 2008
 @date 24 October 2008
 *******************************************************************************/
wxString tmGISDataRasterJPEG::GetDataSizeAsHtml(int iPrecision)
{
    wxString myResult = _("<U><B>Jpeg Raster Size</B></U><BR>");

    wxFileName myJpgName(GetFullFileName());
    myResult.Append(wxString::Format(_("Raster size : %s"),
                                     myJpgName.GetHumanReadableSize(_T("Error computing raster size"),
                                                                    iPrecision).c_str()));

    return myResult;
}


