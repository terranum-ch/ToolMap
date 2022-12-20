/***************************************************************************
 tmsymbol.cpp
 Deals with GIS symbology and associed dialog
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

#include "tmsymbol.h"

#include "../database/database_tm.h"
#include "tmsymbolraster.h"
#include "tmsymbolvector.h"

tmSymbol::tmSymbol() {
    m_pDB = nullptr;
    m_LayerTransparency = 0;
    m_TocName = TOC_NAME_UNKNOWN;
}

tmSymbol::~tmSymbol() {}

/***************************************************************************/ /**
  @brief Convert transparency
  @details Convert transparency from a percent basis (%) to a 0-255 colour basis.
  See documentation of wxColour for more informations
  @param itransparency the transparency to convert from. Based on a 0-100 percent
  scale
  @return  The new transparency : on a 0 (transparent) -255 (opaque) scale.
  @author Lucien Schreiber (c) CREALP 2008
  @date 11 September 2008
  *******************************************************************************/
char tmSymbol::ConvertTransparency(int itransparency) {
    const double iConvFactor = 2.55;
    double dNewTransparency = iConvFactor * itransparency;
    dNewTransparency = 255 - dNewTransparency;
    return (char)dNewTransparency;
}

tmSymbolDLG* tmSymbol::GetSymbolDialog(wxWindow* parent, const wxPoint& dlgpos) {
    return new tmSymbolDLG(parent, SYMBOL_TMSYMBOLDLG_IDNAME, SYMBOL_TMSYMBOLDLG_TITLE, dlgpos);
}

/***************************************************************************/ /**
  @brief Display the symbology dialog
  @details A personalized symbology dialog is displayed
  @param parent Adress of the parent (for the dialog)
  @param metadata String containing html formatted metadata for displaying in the
  first dialog tab.
  @param dlgpos Desired position for the dialog
  @return  wxID_OK if user select 'ok' in the symbology dialog, wxID_CANCEL
  otherwise
  @author Lucien Schreiber (c) CREALP 2008
  @date 15 October 2008
  *******************************************************************************/
int tmSymbol::ShowSymbologyDialog(wxWindow* parent, const wxPoint& dlgpos) {
    tmSymbolDLG* mydlg = GetSymbolDialog(parent, dlgpos);
    int iRetVal = wxID_CANCEL;

    if (mydlg->ShowModal() == wxID_OK) {
        GetDialogData(mydlg);
        iRetVal = wxID_OK;
    }

    wxDELETE(mydlg);
    return iRetVal;
}

// implemented in child class
int tmSymbol::ShowLabelDialog(wxWindow* parent, tmLayerProperties* properties, const wxPoint& pos) {
    return wxID_CANCEL;
}

tmSymbol* tmSymbol::CreateSymbolBasedOnType(TM_GIS_SPATIAL_TYPES spattype, TOC_GENERIC_NAME tocname) {
    switch (spattype) {
        case LAYER_SPATIAL_LINE:
        case LAYER_SPATIAL_POINT:
        case LAYER_SPATIAL_POLYGON:
            return tmSymbolVector::CreateSymbolVectorBasedOnType(spattype, tocname);
            break;

        case LAYER_SPATIAL_RASTER:
            return new tmSymbolRaster();
            break;

        default:
            return new tmSymbol();
            break;
    }
}

tmSymbol* tmSymbol::CreateCopySymbolBasedOnType(TM_GIS_SPATIAL_TYPES spattype, TOC_GENERIC_NAME tocname,
                                                tmSymbol* origin) {
    switch (spattype) {
        case LAYER_SPATIAL_LINE:
        case LAYER_SPATIAL_POINT:
        case LAYER_SPATIAL_POLYGON:
            return tmSymbolVector::CreateCopySymbolVectorBasedOnType(spattype, tocname, origin);
            break;

        case LAYER_SPATIAL_RASTER:
            return new tmSymbolRaster(*((tmSymbolRaster*)origin));
            break;

        default:
            return new tmSymbol();
            break;
    }
}

bool tmSymbol::GetDialogData(tmSymbolDLG* dlg) {
    wxLogDebug(_T("Getting data back from dialog not implemented"));
    return FALSE;
}

bool tmSymbol::Serialize(tmSerialize& s) {
    wxLogDebug(_T("Not able to serialize, not implemented actually"));
    return TRUE;
}
