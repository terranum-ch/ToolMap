/***************************************************************************
 tmsymbolvectorpoint.cpp
 Deals with GIS point vector symbology and associed dialog
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

#include "tmsymbolvectorpoint.h"

tmSymbolVectorPoint::tmSymbolVectorPoint() {}

tmSymbolVectorPoint::tmSymbolVectorPoint(const tmSymbolVectorPoint& origin) {
    m_ptUniqueSymbol.m_Colour = origin.m_ptUniqueSymbol.m_Colour;
    m_ptUniqueSymbol.m_Radius = origin.m_ptUniqueSymbol.m_Radius;
    m_ptUniqueSymbol.m_GlobalTransparency = origin.m_ptUniqueSymbol.m_GlobalTransparency;
}

tmSymbolVectorPoint::~tmSymbolVectorPoint() {}

tmSymbolDLG* tmSymbolVectorPoint::GetSymbolDialog(wxWindow* parent, const wxPoint& dlgpos) {
    tmSymbolDLGPoint* dlg = new tmSymbolDLGPoint(parent, SYMBOL_TMSYMBOLDLG_IDNAME, SYMBOL_TMSYMBOLDLG_TITLE, dlgpos);
    dlg->SetDialogData(m_ptUniqueSymbol);
    return dlg;
}

bool tmSymbolVectorPoint::GetDialogData(tmSymbolDLG* dlg) {
    m_ptUniqueSymbol = ((tmSymbolDLGPoint*)dlg)->GetDialogData();
    return TRUE;
}

bool tmSymbolVectorPoint::Serialize(tmSerialize& s) {
    s.EnterObject();
    if (s.IsStoring()) {
        s << m_ptUniqueSymbol.m_Colour;
        s << m_ptUniqueSymbol.m_Radius;
        s << m_ptUniqueSymbol.m_GlobalTransparency;
    } else {
        s >> m_ptUniqueSymbol.m_Colour;
        s >> m_ptUniqueSymbol.m_Radius;
        s >> m_ptUniqueSymbol.m_GlobalTransparency;
    }
    s.LeaveObject();

    // return false when the archive encountered an error
    return s.IsOk();
}

wxColour tmSymbolVectorPoint::GetColour() {
    return GetColourWithTransparency(m_ptUniqueSymbol.m_Colour, m_ptUniqueSymbol.m_GlobalTransparency);
}
