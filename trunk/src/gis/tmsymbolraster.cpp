/***************************************************************************
								tmsymbolraster.cpp
				Deals with GIS raster symbology and associed dialog
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

#include "tmsymbolraster.h"

#include <wx/mstream.h>
#include <wx/wfstream.h>

tmSymbolRaster::tmSymbolRaster()
{
	m_RasterData.m_bUseColorTransparency = FALSE;
	m_RasterData.m_TransparencyColour = wxColour(*wxBLACK);
	m_RasterData.m_GlobalTransparency = 0;

}



tmSymbolRaster::~tmSymbolRaster()
{
	
	
}


tmSymbolDLG * tmSymbolRaster::GetSymbolDialog (wxWindow * parent, const wxPoint & dlgpos)
{
	tmSymbolDLGRaster * myDlg = new tmSymbolDLGRaster(parent,SYMBOL_TMSYMBOLDLG_IDNAME,
													  SYMBOL_TMSYMBOLDLG_TITLE,
													  dlgpos);
	myDlg->SetDialogData(m_RasterData);
	
	return myDlg;
}


bool tmSymbolRaster::GetDialogData(tmSymbolDLG * dlg)
{
	m_RasterData = ((tmSymbolDLGRaster *) dlg)->GetDialogData();
	
	
	wxFileOutputStream file(_T("thisisatest.dat"));
	wxSerialize a(file);
	Serialize(a);
	
	
	return TRUE;
}



bool tmSymbolRaster::Serialize(wxSerialize &s)
{
	int colourred = m_RasterData.m_TransparencyColour.Red();
	int colourgreen = m_RasterData.m_TransparencyColour.Green(); 
	int colourblue = m_RasterData.m_TransparencyColour.Blue();
	
	
	if(s.IsStoring())
	{
		s << m_RasterData.m_bUseColorTransparency;
		s << colourred << colourgreen << colourblue;
		s << m_RasterData.m_GlobalTransparency;
		
	}
	else
	{
		s >> m_RasterData.m_bUseColorTransparency;
		s >> colourred >> colourgreen >> colourblue;
		s >> m_RasterData.m_GlobalTransparency;
		m_RasterData.m_TransparencyColour.Set(colourred, colourgreen, colourblue);
	}
	
	// return false when the archive encountered an error
	return s.IsOk();
}


