/***************************************************************************
 tmgisdatarasterweb
 -------------------
 copyright            : (C) 2013 CREALP Lucien Schreiber
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "tmgisdatarasterweb.h"

#include "tmrenderer.h"

tmGISDataRasterWeb::tmGISDataRasterWeb() {
  m_FileType = _T("Web Raster");
  m_ClassType = tmGIS_RASTER_WEB;
}

tmGISDataRasterWeb::~tmGISDataRasterWeb() {}

wxString tmGISDataRasterWeb::GetDataSizeAsHtml(int iPrecision) {
  wxString myResult = _("<U><B>Web Raster Size</B></U><BR>No Size!");
  return myResult;
}

bool tmGISDataRasterWeb::SetSpatialFilter(tmRealRect filter, int type) {
  // check if view moved !
  if (m_FilterCoordLocal == filter) {
    return true;
  }
  m_FilterCoordLocal = filter;

  // convert project coordinates into web coordinates and loads data into tmwebframe
  wxRealPoint xymin = GetCoordConvert()->GetPointGoogle(wxRealPoint(filter.x_min, filter.y_min));
  wxRealPoint xymax = GetCoordConvert()->GetPointGoogle(wxRealPoint(filter.x_max, filter.y_max));
  m_FilterCoordWeb = tmRealRect(xymin.x, xymin.y, xymax.x, xymax.y);

  // get image information
  tmRealRect myImgRealCoord = GetMinimalBoundingRectangle();
  if (myImgRealCoord == tmRealRect(0, 0, 0, 0)) {
    return false;
  }

  if (myImgRealCoord.Clip(m_FilterCoordWeb, m_ClippedCoord)) {
    // image visible,
    // clip image with spatial filter
    m_PxImgFilter = ConvertClipedImage(myImgRealCoord, m_ClippedCoord);
  }
  return true;
}
