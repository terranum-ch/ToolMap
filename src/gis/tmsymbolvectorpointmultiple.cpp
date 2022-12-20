/***************************************************************************
 tmsymbolvectorpointmultiple.cpp
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

#include "tmsymbolvectorpointmultiple.h"

tmSymbolVectorPointMultiple::tmSymbolVectorPointMultiple() {}

tmSymbolVectorPointMultiple::tmSymbolVectorPointMultiple(const tmSymbolVectorPointMultiple& origin) {
    m_ptMultipleSymbol.m_PanelNo = origin.m_ptMultipleSymbol.m_PanelNo;
    m_ptMultipleSymbol.m_Colour = origin.m_ptMultipleSymbol.m_Colour;
    m_ptMultipleSymbol.m_Radius = origin.m_ptMultipleSymbol.m_Radius;
    m_ptMultipleSymbol.m_GlobalTransparency = origin.m_ptMultipleSymbol.m_GlobalTransparency;

    m_ptMultipleSymbol.m_QueryID = origin.m_ptMultipleSymbol.m_QueryID;
    m_ptMultipleSymbol.m_SelColourMultiple = origin.m_ptMultipleSymbol.m_SelColourMultiple;
    m_ptMultipleSymbol.m_SelRadiusMultiple = origin.m_ptMultipleSymbol.m_SelRadiusMultiple;
    m_ptMultipleSymbol.m_SelVisible = origin.m_ptMultipleSymbol.m_SelVisible;
    m_ptMultipleSymbol.m_UnSelColourMultiple = origin.m_ptMultipleSymbol.m_UnSelColourMultiple;
    m_ptMultipleSymbol.m_UnSelRadiusMultiple = origin.m_ptMultipleSymbol.m_UnSelRadiusMultiple;
    m_ptMultipleSymbol.m_UnSelVisible = origin.m_ptMultipleSymbol.m_UnSelVisible;
}

tmSymbolVectorPointMultiple::~tmSymbolVectorPointMultiple() {}

tmSymbolDLG* tmSymbolVectorPointMultiple::GetSymbolDialog(wxWindow* parent, const wxPoint& dlgpos) {
    wxASSERT(GetDatabase());
    tmSymbolDLGPointMultiple* dlg = new tmSymbolDLGPointMultiple(parent, SYMBOL_TMSYMBOLDLG_IDNAME,
                                                                 SYMBOL_TMSYMBOLDLG_TITLE, dlgpos);
    dlg->SetDatabase(GetDatabase());
    dlg->SetTocType(GetTocName());
    dlg->SetDialogData(m_ptMultipleSymbol);
    return dlg;
}

bool tmSymbolVectorPointMultiple::GetDialogData(tmSymbolDLG* dlg) {
    m_ptMultipleSymbol = ((tmSymbolDLGPointMultiple*)dlg)->GetDialogData();
    return true;
}

bool tmSymbolVectorPointMultiple::Serialize(tmSerialize& s) {
    s.EnterObject();
    if (s.IsStoring()) {
        s << m_ptMultipleSymbol.m_PanelNo;
        s << m_ptMultipleSymbol.m_Colour;
        s << m_ptMultipleSymbol.m_Radius;
        s << m_ptMultipleSymbol.m_GlobalTransparency;

        s << m_ptMultipleSymbol.m_QueryID;
        s << m_ptMultipleSymbol.m_SelColourMultiple;
        s << m_ptMultipleSymbol.m_SelRadiusMultiple;
        s << m_ptMultipleSymbol.m_SelVisible;
        s << m_ptMultipleSymbol.m_UnSelColourMultiple;
        s << m_ptMultipleSymbol.m_UnSelRadiusMultiple;
        s << m_ptMultipleSymbol.m_UnSelVisible;
    } else {
        s >> m_ptMultipleSymbol.m_PanelNo;
        s >> m_ptMultipleSymbol.m_Colour;
        s >> m_ptMultipleSymbol.m_Radius;
        s >> m_ptMultipleSymbol.m_GlobalTransparency;

        s >> m_ptMultipleSymbol.m_QueryID;
        s >> m_ptMultipleSymbol.m_SelColourMultiple;
        s >> m_ptMultipleSymbol.m_SelRadiusMultiple;
        s >> m_ptMultipleSymbol.m_SelVisible;
        s >> m_ptMultipleSymbol.m_UnSelColourMultiple;
        s >> m_ptMultipleSymbol.m_UnSelRadiusMultiple;
        s >> m_ptMultipleSymbol.m_UnSelVisible;
    }
    s.LeaveObject();

    // return false when the archive encountered an error
    return s.IsOk();
}

wxColour tmSymbolVectorPointMultiple::GetColour() {
    return GetColourWithTransparency(m_ptMultipleSymbol.m_Colour, m_ptMultipleSymbol.m_GlobalTransparency);
}
