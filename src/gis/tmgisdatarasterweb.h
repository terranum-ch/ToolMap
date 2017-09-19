/***************************************************************************
 tmgisdatarasterweb
 -------------------
 copyright            : (C) 2013 CREALP Lucien Schreiber
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

#ifndef _TM_GISDATARASTER_WEB_H_
#define _TM_GISDATARASTER_WEB_H_

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "tmgisdataraster.h"

class tmWebFrame;
class tmGISDataRasterWeb : public tmGISDataRaster
{
private:
    tmRealRect m_FilterCoordLocal;
    tmRealRect m_FilterCoordWeb;
    tmWebFrame * m_WebFrameRef;
    static const double m_AvaillableResolutions [];
    double _GetClosestAvaillableResolution(tmRealRect bounds, double resolution);
    double _GetResolution (tmRealRect bounds);

public:
    tmGISDataRasterWeb();
    ~tmGISDataRasterWeb();
    
    // void SetWebFrameRef (tmWebFrame * webframe){m_WebFrameRef = webframe;}
    // tmWebFrame * GetWebFrameRef() {return m_WebFrameRef;}
    
    virtual wxString GetDataSizeAsHtml (int iPrecision = 2);
	// virtual bool Open (const wxString & filename, bool bReadWrite = TRUE);
    
    // virtual tmRealRect GetMinimalBoundingRectangle();
    virtual bool SetSpatialFilter (tmRealRect filter, int type);
//	virtual bool IsImageInsideVisibleArea ();
    virtual tmRealRect GetImageClipedCoordinates () {return m_FilterCoordLocal;}
	//virtual CPLErr GetImageData(unsigned char **imgbuf, unsigned int   *imglen,
    //                            unsigned char **maskbuf, unsigned int   *masklen,
    //                            wxSize imgSize);

};

#endif
