/***************************************************************************
 tmgisdatarasteregrid.h
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


#ifndef _TM_GISDATARASTER_EGRID_H_
#define _TM_GISDATARASTER_EGRID_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif


#include "tmgisdataraster.h"
#include "gdal_priv.h"


class tmGISDataRasterEGRID : public tmGISDataRaster
{
private:

protected:
public:
    tmGISDataRasterEGRID();

    ~tmGISDataRasterEGRID();

    virtual wxString GetDataSizeAsHtml(int iPrecision = 2);

};


#endif
