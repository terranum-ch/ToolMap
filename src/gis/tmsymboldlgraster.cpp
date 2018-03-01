/***************************************************************************
								tmsymboldlgraster.cpp
						Display Raster Symbology dialog
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

// comment doxygen

#include "tmsymboldlgraster.h"


IMPLEMENT_DYNAMIC_CLASS(tmSymbolDLGRaster, tmSymbolDLG)


BEGIN_EVENT_TABLE(tmSymbolDLGRaster, tmSymbolDLG)
END_EVENT_TABLE()


tmSymbolDLGRaster::tmSymbolDLGRaster()
{
    _Init();
}


tmSymbolDLGRaster::tmSymbolDLGRaster(wxWindow *parent, wxWindowID id, const wxString &caption, const wxPoint &pos,
                                     const wxSize &size, long style)
{
    _Init();
    Create(parent, id, caption, pos, size, style);
}


bool tmSymbolDLGRaster::Create(wxWindow *parent, wxWindowID id, const wxString &caption, const wxPoint &pos,
                               const wxSize &size, long style)
{
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    tmSymbolDLG::Create(parent, id, caption, pos, size, style);

    CreateControls();

    return true;
}


tmSymbolDLGRaster::~tmSymbolDLGRaster()
{

}


void tmSymbolDLGRaster::_Init()
{
    m_UseTransparencyColourCtrl = NULL;
    m_TransparencyColourCtrl = NULL;
    m_TransparencySlider = NULL;
}


void tmSymbolDLGRaster::CreateControls()
{
    wxBoxSizer *itemBoxSizer8 = new wxBoxSizer(wxVERTICAL);
    m_SymbolPanel->SetSizer(itemBoxSizer8);

    wxStaticBox *itemStaticBoxSizer12Static = new wxStaticBox(m_SymbolPanel, wxID_ANY, _("Transparency"));
    wxStaticBoxSizer *itemStaticBoxSizer12 = new wxStaticBoxSizer(itemStaticBoxSizer12Static, wxHORIZONTAL);
    itemBoxSizer8->Add(itemStaticBoxSizer12, 0, wxGROW | wxALL, 5);
    m_TransparencySlider = new tmSliderWithText(m_SymbolPanel, ID_SYMDLGRASTER_TRANSPARENCY,
                                                0, 0, 100, _T("%"));
    itemStaticBoxSizer12->Add(m_TransparencySlider, 1, wxGROW | wxALL, 5);

    // adapt dialog size to new controls added
    SetSizeHint();
}


bool tmSymbolDLGRaster::TransferDataToWindow()
{
    m_TransparencySlider->SetValue(m_DlgData.m_GlobalTransparency);
    return TRUE;
}


bool tmSymbolDLGRaster::TransferDataFromWindow()
{
    m_DlgData.m_GlobalTransparency = m_TransparencySlider->GetValue();
    return TRUE;
}



