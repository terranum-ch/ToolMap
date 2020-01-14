/***************************************************************************
 tmsymbolvectorpolygon.cpp
 Deals with GIS polygon vector symbology and associed dialog
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

#include "tmsymbolvectorpolygon.h"

tmSymbolVectorPolygon::tmSymbolVectorPolygon() {
  m_plgUniqueSymbol.m_PanelNo = 0;
  m_plgUniqueSymbol.m_bColour = wxColour(*wxBLACK);
  m_plgUniqueSymbol.m_bWidth = 1;
  m_plgUniqueSymbol.m_fColour = wxColour(*wxWHITE);
  m_plgUniqueSymbol.m_fStyle = 0;
  m_plgUniqueSymbol.m_GlobalTransparency = 0;
}

tmSymbolVectorPolygon::tmSymbolVectorPolygon(const tmSymbolVectorPolygon &origin) {
  m_plgUniqueSymbol.m_PanelNo = origin.m_plgUniqueSymbol.m_PanelNo;
  m_plgUniqueSymbol.m_bColour = origin.m_plgUniqueSymbol.m_bColour;
  m_plgUniqueSymbol.m_bWidth = origin.m_plgUniqueSymbol.m_bWidth;
  m_plgUniqueSymbol.m_fColour = origin.m_plgUniqueSymbol.m_fColour;
  m_plgUniqueSymbol.m_fStyle = origin.m_plgUniqueSymbol.m_fStyle;
  m_plgUniqueSymbol.m_GlobalTransparency = origin.m_plgUniqueSymbol.m_GlobalTransparency;
}

tmSymbolVectorPolygon::~tmSymbolVectorPolygon() {}

tmSymbolDLG *tmSymbolVectorPolygon::GetSymbolDialog(wxWindow *parent, const wxPoint &dlgpos) {
  tmSymbolDLGPolygon *dlg = new tmSymbolDLGPolygon(parent, SYMBOL_TMSYMBOLDLG_IDNAME, SYMBOL_TMSYMBOLDLG_TITLE, dlgpos);
  dlg->SetDialogData(m_plgUniqueSymbol);
  return dlg;
}

bool tmSymbolVectorPolygon::GetDialogData(tmSymbolDLG *dlg) {
  m_plgUniqueSymbol = ((tmSymbolDLGPolygon *)dlg)->GetDialogData();
  return TRUE;
}

bool tmSymbolVectorPolygon::Serialize(tmSerialize &s) {
  s.EnterObject();
  if (s.IsStoring()) {
    s << m_plgUniqueSymbol.m_PanelNo;
    s << m_plgUniqueSymbol.m_bColour;
    s << m_plgUniqueSymbol.m_bWidth;
    s << m_plgUniqueSymbol.m_fColour;
    s << m_plgUniqueSymbol.m_fStyle;
    s << m_plgUniqueSymbol.m_GlobalTransparency;
  } else {
    s >> m_plgUniqueSymbol.m_PanelNo;
    s >> m_plgUniqueSymbol.m_bColour;
    s >> m_plgUniqueSymbol.m_bWidth;
    s >> m_plgUniqueSymbol.m_fColour;
    s >> m_plgUniqueSymbol.m_fStyle;
    s >> m_plgUniqueSymbol.m_GlobalTransparency;
  }
  s.LeaveObject();

  // return false when the archive encountered an error
  return s.IsOk();
}

wxColour tmSymbolVectorPolygon::GetBorderColour() {
  return GetColourWithTransparency(m_plgUniqueSymbol.m_bColour, m_plgUniqueSymbol.m_GlobalTransparency);
}

wxColour tmSymbolVectorPolygon::GetFillColour() {
  return GetColourWithTransparency(m_plgUniqueSymbol.m_fColour, m_plgUniqueSymbol.m_GlobalTransparency);
}
