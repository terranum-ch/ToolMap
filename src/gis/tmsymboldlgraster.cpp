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


IMPLEMENT_DYNAMIC_CLASS( tmSymbolDLGRaster, tmSymbolDLG )



BEGIN_EVENT_TABLE( tmSymbolDLGRaster, tmSymbolDLG )
END_EVENT_TABLE()


tmSymbolDLGRaster::tmSymbolDLGRaster()
{
    Init();
}



tmSymbolDLGRaster::tmSymbolDLGRaster( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, caption, pos, size, style);
}



bool tmSymbolDLGRaster::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    tmSymbolDLG::Create( parent, id, caption, pos, size, style );
	    
	CreateControls();
	
	return true;
}




tmSymbolDLGRaster::~tmSymbolDLGRaster()
{

}



void tmSymbolDLGRaster::Init()
{
	m_UseTransparencyColourCtrl	= NULL;
	m_TransparencyColourCtrl	= NULL;
	m_TransparencySlider		= NULL;
}



void tmSymbolDLGRaster::CreateControls()
{
	wxString sFunction = wxString::FromAscii(__FUNCTION__);
	wxString sFunctionLineError = wxString::Format( _T("%s line %d : "),
												   sFunction.c_str(), __LINE__); 
	wxString sErrMsg = wxString::Format(_T("%s Undefined m_Notebook"), sFunctionLineError.c_str());
	wxASSERT_MSG(m_NoteBook,sErrMsg);

	
	
	wxPanel* itemPanel7 = new wxPanel( m_NoteBook, ID_SYMDLGRASTER_PANEL, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer8 = new wxBoxSizer(wxVERTICAL);
    itemPanel7->SetSizer(itemBoxSizer8);
	
    wxStaticBox* itemStaticBoxSizer9Static = new wxStaticBox(itemPanel7, wxID_ANY, _("Color transparency"));
    wxStaticBoxSizer* itemStaticBoxSizer9 = new wxStaticBoxSizer(itemStaticBoxSizer9Static, wxVERTICAL);
    itemBoxSizer8->Add(itemStaticBoxSizer9, 1, wxGROW|wxALL, 5);
    m_UseTransparencyColourCtrl = new wxCheckBox( itemPanel7, 
												 ID_SYMDLGRASTER_USE_TRANSPARENCY_COLOR, 
												 _("Use color transparency"), 
												 wxDefaultPosition, wxDefaultSize, 0 );
    m_UseTransparencyColourCtrl->SetValue(false);
    itemStaticBoxSizer9->Add(m_UseTransparencyColourCtrl, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	
    m_TransparencyColourCtrl = new tmColourPickerCtrl( itemPanel7, ID_SYMDLGRASTER_TRANSPARENCY_COLOR);
    itemStaticBoxSizer9->Add(m_TransparencyColourCtrl, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	
    wxStaticBox* itemStaticBoxSizer12Static = new wxStaticBox(itemPanel7, wxID_ANY, _("Transparency"));
    wxStaticBoxSizer* itemStaticBoxSizer12 = new wxStaticBoxSizer(itemStaticBoxSizer12Static, wxHORIZONTAL);
    itemBoxSizer8->Add(itemStaticBoxSizer12, 0, wxGROW|wxALL, 5);
    m_TransparencySlider = new tmSliderWithText(itemPanel7, ID_SYMDLGRASTER_TRANSPARENCY,
												0, 0, 100, _T("%"));
    itemStaticBoxSizer12->Add(m_TransparencySlider, 1, wxGROW|wxALL, 5);
	
    m_NoteBook->AddPage(itemPanel7, _("Raster Symbology"));
	m_NoteBook->ChangeSelection(1);

	
	// adapt dialog size to new controls added
	SetSizeHint();
}



bool tmSymbolDLGRaster::TransferDataToWindow()
{
	m_UseTransparencyColourCtrl->SetValue(m_DlgData.m_bUseColorTransparency);
	m_TransparencyColourCtrl->SetColour(m_DlgData.m_TransparencyColour);
	m_TransparencySlider->SetValue(m_DlgData.m_GlobalTransparency);
	
	return TRUE;
}



bool tmSymbolDLGRaster::TransferDataFromWindow()
{
	m_DlgData.m_bUseColorTransparency = m_UseTransparencyColourCtrl->GetValue();
	m_DlgData.m_TransparencyColour = m_TransparencyColourCtrl->GetColour();
	m_DlgData.m_GlobalTransparency = m_TransparencySlider->GetValue();
	
	return TRUE;
}



