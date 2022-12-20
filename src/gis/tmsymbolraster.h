/***************************************************************************
 tmsymbolraster.h
 Deals with GIS raster symbology and associed dialog
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

#ifndef _TM_SYMBOLRASTER_H_
#define _TM_SYMBOLRASTER_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

// Include wxWidgets' headers
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "tmsymbol.h"           // tmSymbol class definition
#include "tmsymboldlgraster.h"  // raster dialog definition

class tmSymbolRaster : public tmSymbol {
  private:
    tmSymbolDataRaster m_RasterData;

    virtual tmSymbolDLG* GetSymbolDialog(wxWindow* parent, const wxPoint& dlgpos);

    virtual bool GetDialogData(tmSymbolDLG* dlg);

  protected:
  public:
    tmSymbolRaster();

    tmSymbolRaster(const tmSymbolRaster& origin);

    ~tmSymbolRaster();

    virtual bool Serialize(tmSerialize& s);

    virtual int GetTransparency() {
        return m_RasterData.m_GlobalTransparency;
    }

    bool GetDoMultiply() {
        return m_RasterData.m_DoMultiplyRaster;
    }
};

#endif
