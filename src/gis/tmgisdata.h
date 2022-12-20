/***************************************************************************
 tmgisdata.h
 Main class for dealing with GIS data
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

#ifndef _TM_GISDATA_H_
#define _TM_GISDATA_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

// Include wxWidgets' headers
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/filename.h>  // for dealing with filename class

#include "../core/tmarraysize.h"  // for wxArrayRealPoints
#include "gdal_priv.h"            // GDAL ACCES C++
#include "geos_c.h"               // GEOS accessing
#include "ogrsf_frmts.h"          // OGR accessing
#include "tmgisscale.h"           // for dealing with scale and real rectangle.
#include "tmlayerproperties.h"    // for GIS spatial types and tmLayerProperties

class tmCoordConvert;

/***************************************************************************/ /**
  @brief Main class for dealing with GIS data
  @details This class may be used for accessing GIS data using GDAL / OGR driver
  see also : http://www.gdal.org/
  @author Lucien Schreiber (c) CREALP 2008
  @date 14 July 2008
  *******************************************************************************/
class tmGISData : public wxObject {
  private:
    static bool m_LogOn;
    wxString m_ShortFileName;
    wxString m_FullFileName;
    tmCoordConvert* m_CoordConvert;

    void InitMemberValue();

  protected:
    wxString GetMinimalBoundingRectangleAsHtml(int iprecision = 2);

    int m_ClassType;

  public:
    tmGISData();

    ~tmGISData();

    // get type of class
    int GetDataType() {
        return m_ClassType;
    }

    void SetCoordConvert(tmCoordConvert* coordconvert) {
        m_CoordConvert = coordconvert;
    }

    tmCoordConvert* GetCoordConvert() {
        return m_CoordConvert;
    }

    // static functions for init
    static void InitGISDrivers(bool bRaster = TRUE, bool bVector = TRUE);

    static wxString GetAllSupportedGISFormatsWildcards();

    static wxArrayString GetAllSupportedGISFormatsExtensions();

    static tmGISData* CreateGISBasedOnType(const int& gis_format_index);

    static tmGISData* CreateGISBasedOnExt(const wxString& extension);

    // load layer
    static tmGISData* LoadLayer(tmLayerProperties* layerprop);

    static void EnableLogging(bool enable = true) {
        m_LogOn = enable;
    }

    static bool IsLoggingEnabled() {
        return m_LogOn;
    }

    // gis function
    virtual bool Open(const wxString& filename, bool bReadWrite = TRUE);

    virtual bool Close() {
        return false;
    }

    virtual tmRealRect GetMinimalBoundingRectangle() {
        return tmRealRect(0, 0, 0, 0);
    }

    virtual TM_GIS_SPATIAL_TYPES GetSpatialType() {
        return LAYER_ERR;
    }

    // misc function
    wxString GetShortFileName() {
        return m_ShortFileName;
    }

    wxString GetFullFileName() {
        return m_FullFileName;
    }

    // metadata functions
    virtual wxString GetMetaDataAsHtml() {
        return wxEmptyString;
    }

    virtual wxString GetDataSizeAsHtml(int iPrecision = 2) {
        return wxEmptyString;
    }

    // search function
    virtual wxArrayLong* SearchData(const tmRealRect& rect, int type) {
        return nullptr;
    }

    virtual wxArrayLong* GetAllData() {
        return nullptr;
    }

    virtual bool GetSnapCoord(const wxRealPoint& clickpt, double buffersize, wxArrayRealPoints& snapppts,
                              int snaptype) {
        return false;
    }

    virtual bool IsPointSnapped(const wxRealPoint& point, int snaptype, long excludeoid = wxNOT_FOUND) {
        return false;
    }

    virtual bool CreateSpatialIndex(GDALProgressFunc progress, void* pfProgressData) {
        return false;
    }

    virtual int IsRaster() {
        return wxNOT_FOUND;
    }
};

#endif
