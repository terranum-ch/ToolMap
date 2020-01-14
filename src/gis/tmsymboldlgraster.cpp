/***************************************************************************
 tmsymboldlgraster.cpp
 Display Raster Symbology dialog
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
    m_TransparencySlider = NULL;
    m_MultiplyCtrl = NULL;
    m_DlgData.m_GlobalTransparency = 0;
    m_DlgData.m_DoMultiplyRaster = false;
}


void tmSymbolDLGRaster::CreateControls()
{
    wxBoxSizer *itemBoxSizer8 = new wxBoxSizer(wxVERTICAL);
    m_SymbolPanel->SetSizer(itemBoxSizer8);


    wxStaticBoxSizer* sbSizer2;
    sbSizer2 = new wxStaticBoxSizer( new wxStaticBox( m_SymbolPanel, wxID_ANY, _("Transparency") ), wxHORIZONTAL );
    m_TransparencySlider = new tmSliderWithText(m_SymbolPanel, ID_SYMDLGRASTER_TRANSPARENCY,
                                                0, 0, 100, _T("%"));
    sbSizer2->Add(m_TransparencySlider, 1, wxGROW | wxALL, 5);
    itemBoxSizer8->Add(sbSizer2, 0, wxEXPAND, 5);


    wxStaticBoxSizer* sbSizer1;
    sbSizer1 = new wxStaticBoxSizer( new wxStaticBox( m_SymbolPanel, wxID_ANY, _("Fusion") ), wxHORIZONTAL );
    m_MultiplyCtrl = new wxCheckBox( m_SymbolPanel, wxID_ANY, _("Multiply"), wxDefaultPosition, wxDefaultSize, 0 );
    sbSizer1->Add( m_MultiplyCtrl, 0, wxALL, 5 );
    itemBoxSizer8->Add( sbSizer1, 0, wxEXPAND, 5 );

    // adapt dialog size to new controls added
    SetSizeHint();
}


bool tmSymbolDLGRaster::TransferDataToWindow()
{
    m_TransparencySlider->SetValue(m_DlgData.m_GlobalTransparency);
    m_MultiplyCtrl->SetValue(m_DlgData.m_DoMultiplyRaster);
    return true;
}


bool tmSymbolDLGRaster::TransferDataFromWindow()
{
    m_DlgData.m_GlobalTransparency = m_TransparencySlider->GetValue();
    m_DlgData.m_DoMultiplyRaster = m_MultiplyCtrl->GetValue();
    return true;
}



