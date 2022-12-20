/***************************************************************************
 tmsymbolvectorpointmultiple.h
 Deals with GIS point vector symbology and associed dialog
 -------------------
 copyright            : (C) 2011 CREALP Lucien Schreiber
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef _TM_SYMBOLVECTORPOINT_MULTIPLE_H_
#define _TM_SYMBOLVECTORPOINT_MULTIPLE_H_

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "tmsymboldlgpointmultiple.h"  // tmSymbol class definition
#include "tmsymbolvectorpoint.h"

class tmSymbolVectorPointMultiple : public tmSymbolVectorPoint {
  private:
    tmSymbolDataPointMultiple m_ptMultipleSymbol;

    virtual tmSymbolDLG* GetSymbolDialog(wxWindow* parent, const wxPoint& dlgpos);

    virtual bool GetDialogData(tmSymbolDLG* dlg);

  public:
    tmSymbolVectorPointMultiple();

    tmSymbolVectorPointMultiple(const tmSymbolVectorPointMultiple& origin);

    ~tmSymbolVectorPointMultiple();

    virtual wxColour GetColour();

    virtual int GetRadius() {
        return m_ptMultipleSymbol.m_Radius;
    }

    virtual int GetTransparency() {
        return m_ptMultipleSymbol.m_GlobalTransparency;
    }

    virtual bool Serialize(tmSerialize& s);

    int GetSelectedSymbology() {
        return m_ptMultipleSymbol.m_PanelNo;
    }

    tmSymbolDataPointMultiple* GetSymbology() {
        return &m_ptMultipleSymbol;
    }
};

#endif
