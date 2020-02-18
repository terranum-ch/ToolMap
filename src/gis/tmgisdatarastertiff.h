/***************************************************************************
 tmgisdatarastertiff.h
 For dealing with spatial raster data (*.tiff, *.tif)
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

#ifndef _TM_GISDATARASTER_TIFF_H_
#define _TM_GISDATARASTER_TIFF_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

// Include wxWidgets' headers
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "gdal_priv.h"
#include "tmgisdataraster.h"

class tmGISDataRasterTIFF : public tmGISDataRaster {
 private:
 protected:
 public:
  tmGISDataRasterTIFF();

  ~tmGISDataRasterTIFF();

  virtual wxString GetDataSizeAsHtml(int iPrecision = 2);
};

#endif
