/***************************************************************************
 tmsymbolvector.h
 Deals with GIS vector symbology and associed dialog
 -------------------
 copyright            : (C) 2007 CREALP Lucien Schreiber
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

#ifndef _TM_SYMBOLVECTOR_H_
#define _TM_SYMBOLVECTOR_H_

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "tmsymbol.h"
#include "tmsymboldlg.h"

class tmSymbolVector : public tmSymbol
{
private:
    virtual tmSymbolDLG *GetSymbolDialog(wxWindow *parent, const wxPoint &dlgpos);

public:
    tmSymbolVector();

    ~tmSymbolVector();

    static tmSymbolVector *CreateSymbolVectorBasedOnType(TM_GIS_SPATIAL_TYPES spattype, TOC_GENERIC_NAME tocname);

    static tmSymbolVector *CreateCopySymbolVectorBasedOnType(TM_GIS_SPATIAL_TYPES spattype, TOC_GENERIC_NAME tocname,
                                                             tmSymbol *origin);

    wxColour GetColourWithTransparency(const wxColour &col, int trans);

    virtual void SetColour(const wxColour &colour)
    { ; }

    virtual int
    ShowLabelDialog(wxWindow *parent, tmLayerProperties *properties, const wxPoint &pos = wxDefaultPosition);
};


#endif
