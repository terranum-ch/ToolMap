/***************************************************************************
 tmSymbolVectorLineMultiplemultiple.cpp
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

#include "tmsymbolvectorlinemultiple.h"

tmSymbolVectorLineMultiple::tmSymbolVectorLineMultiple() {}

tmSymbolVectorLineMultiple::tmSymbolVectorLineMultiple(const tmSymbolVectorLineMultiple& origin) {
    m_lSymMultiple.m_PanelNo = origin.m_lSymMultiple.m_PanelNo;
    m_lSymMultiple.m_Colour = origin.m_lSymMultiple.m_Colour;
    m_lSymMultiple.m_Shape = origin.m_lSymMultiple.m_Shape;
    m_lSymMultiple.m_Width = origin.m_lSymMultiple.m_Width;

    m_lSymMultiple.m_QueryID = origin.m_lSymMultiple.m_QueryID;
    m_lSymMultiple.m_SelColourMultiple = origin.m_lSymMultiple.m_SelColourMultiple;
    m_lSymMultiple.m_SelShapeMultiple = origin.m_lSymMultiple.m_SelShapeMultiple;
    m_lSymMultiple.m_SelWidthMultiple = origin.m_lSymMultiple.m_SelWidthMultiple;

    m_lSymMultiple.m_UnSelColourMultiple = origin.m_lSymMultiple.m_UnSelColourMultiple;
    m_lSymMultiple.m_UnSelShapeMultiple = origin.m_lSymMultiple.m_UnSelShapeMultiple;
    m_lSymMultiple.m_UnSelWidthMultiple = origin.m_lSymMultiple.m_UnSelWidthMultiple;

    m_lSymMultiple.m_GlobalTransparency = origin.m_lSymMultiple.m_GlobalTransparency;
}

tmSymbolVectorLineMultiple::~tmSymbolVectorLineMultiple() {}

tmSymbolDLG* tmSymbolVectorLineMultiple::GetSymbolDialog(wxWindow* parent, const wxPoint& dlgpos) {
    wxASSERT(GetDatabase());
    tmSymbolDLGLineMultiple* dlg = new tmSymbolDLGLineMultiple(parent, SYMBOL_TMSYMBOLDLG_IDNAME,
                                                               SYMBOL_TMSYMBOLDLG_TITLE, dlgpos);
    dlg->SetDatabase(GetDatabase());
    dlg->SetDialogData(m_lSymMultiple);
    return dlg;
}

bool tmSymbolVectorLineMultiple::GetDialogData(tmSymbolDLG* dlg) {
    m_lSymMultiple = ((tmSymbolDLGLineMultiple*)dlg)->GetDialogData();
    return true;
}

bool tmSymbolVectorLineMultiple::Serialize(tmSerialize& s) {
    s.EnterObject();
    if (s.IsStoring()) {
        s << m_lSymMultiple.m_PanelNo;
        s << m_lSymMultiple.m_Colour;
        s << m_lSymMultiple.m_Shape;
        s << m_lSymMultiple.m_Width;

        s << m_lSymMultiple.m_SelColourMultiple;
        s << m_lSymMultiple.m_SelShapeMultiple;
        s << m_lSymMultiple.m_SelWidthMultiple;
        s << m_lSymMultiple.m_UnSelColourMultiple;
        s << m_lSymMultiple.m_UnSelShapeMultiple;
        s << m_lSymMultiple.m_UnSelWidthMultiple;

        s << m_lSymMultiple.m_GlobalTransparency;
        s << m_lSymMultiple.m_QueryID;
    } else {
        s >> m_lSymMultiple.m_PanelNo;
        s >> m_lSymMultiple.m_Colour;
        s >> m_lSymMultiple.m_Shape;
        s >> m_lSymMultiple.m_Width;

        s >> m_lSymMultiple.m_SelColourMultiple;
        s >> m_lSymMultiple.m_SelShapeMultiple;
        s >> m_lSymMultiple.m_SelWidthMultiple;
        s >> m_lSymMultiple.m_UnSelColourMultiple;
        s >> m_lSymMultiple.m_UnSelShapeMultiple;
        s >> m_lSymMultiple.m_UnSelWidthMultiple;

        s >> m_lSymMultiple.m_GlobalTransparency;
        s >> m_lSymMultiple.m_QueryID;
    }
    s.LeaveObject();

    // return false when the archive encountered an error
    return s.IsOk();
}

wxColour tmSymbolVectorLineMultiple::GetColour() {
    return GetColourWithTransparency(m_lSymMultiple.m_Colour, m_lSymMultiple.m_GlobalTransparency);
}
