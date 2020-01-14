/***************************************************************************
 tmsymbolvectorpolygon.h
 Deals with GIS polygon vector symbology and associed dialog
 -------------------
 copyright : (C) 2007 CREALP Lucien Schreiber
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


#ifndef _TM_SYMBOLVECTORPOLYGON_H_
#define _TM_SYMBOLVECTORPOLYGON_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "tmsymbolvector.h"    // tmSymbol class definition
#include "tmsymboldlgpolygon.h"


class tmSymbolVectorPolygon : public tmSymbolVector
{
private:
    tmSymbolDataPolygonUnique m_plgUniqueSymbol;

    virtual tmSymbolDLG *GetSymbolDialog(wxWindow *parent, const wxPoint &dlgpos);

    virtual bool GetDialogData(tmSymbolDLG *dlg);

protected:
public:
    tmSymbolVectorPolygon();

    tmSymbolVectorPolygon(const tmSymbolVectorPolygon &origin);

    ~tmSymbolVectorPolygon();

    wxColour GetBorderColour();

    wxColour GetFillColour();

    int GetBorderWidth()
    { return m_plgUniqueSymbol.m_bWidth; }

    wxBrushStyle GetFillStyle()
    { return tmSYMBOLFILLSTYLES[m_plgUniqueSymbol.m_fStyle]; }

    virtual int GetTransparency()
    { return m_plgUniqueSymbol.m_GlobalTransparency; }

    virtual void SetColour(const wxColour &colour)
    { m_plgUniqueSymbol.m_fColour = colour; }

    virtual bool Serialize(tmSerialize &s);

    tmSymbolDataPolygonUnique *GetSymbolData()
    { return &m_plgUniqueSymbol; }
};


#endif
