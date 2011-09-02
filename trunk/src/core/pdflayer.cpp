/***************************************************************************
 pdflayer.cpp
 -------------------
 copyright            : (C) 2011 CREALP Lucien Schreiber 
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


#include "pdflayer.h"
#include "projectdefmemory.h"
#include "pdfdocument.h"

PdfLayer::PdfLayer(PdfDocument * parent, ProjectDefMemoryLayers * layer) {
	wxASSERT(layer);
	m_pdfDocumentParent = parent;
	m_prjLayer = new ProjectDefMemoryLayers();
	*m_prjLayer = *layer;
	wxASSERT(GetDocumentParent());
	wxASSERT(m_prjLayer);
}



PdfLayer::~PdfLayer() {
	wxDELETE(m_prjLayer);
}



bool PdfLayer::Generate(wxPdfDocument * pdf) {
	wxASSERT(pdf);
	pdf->SetFont(pdf->GetFontFamily(), "", GetDocumentParent()->GetFontSize());
	pdf->Cell(60, GetDocumentParent()->GetLineSpacing(),
			  wxString::Format("%s (%s)",
							   m_prjLayer->m_LayerName,
							   PRJDEF_LAYERS_TYPE_STRING[m_prjLayer->m_LayerType]),
			  wxPDF_BORDER_FRAME);
	pdf->Ln();
	return true;
}



#include <wx/arrimpl.cpp>
WX_DEFINE_OBJARRAY(ArrayPdfLayer);