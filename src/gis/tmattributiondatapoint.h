/***************************************************************************
 tmAttributionDataPoint.h
 Class for attributing Point
 -------------------
 copyright : (C) 2008 CREALP Lucien Schreiber
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

#ifndef _TM_ATTRIBUTION_DATA_POINT_H_
#define _TM_ATTRIBUTION_DATA_POINT_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

// Include wxWidgets' headers
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "tmattributiondata.h"

class tmAttributionDataPoint : public tmAttributionData {
  private:
  protected:
    bool GetPanelValues(AttribObjType_PANEL* panel, wxArrayLong& valueids);

    void SetPanelValues(AttribObjType_PANEL* panel, const wxArrayLong& valueids);

  public:
    // ctor - dtor
    tmAttributionDataPoint();

    tmAttributionDataPoint(wxArrayLong* selected, DataBaseTM* database);

    virtual void Create(wxArrayLong* selected, DataBaseTM* database);

    ~tmAttributionDataPoint();

    // attribution
    virtual bool SetAttributeBasic(AttribObjType_PANEL* panel);

    // info
    virtual bool GetInfoBasic(AttribObjType_PANEL* panel);

    virtual bool GetInfoBasic(long oid, wxArrayLong& objid, wxArrayString& objcode, wxArrayString& objname);

    virtual bool GetAttributionLayersID(const long& geomid, tmLayerValueArray& layersid);

    virtual bool GetAttributionLayersIDFull(const long& geomid, tmLayerValueArray& layersid);
};

#endif
