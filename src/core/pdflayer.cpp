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
	
	m_pdfObjects = new PdfObjects(this);
	m_pdfAttributs = new PdfAttributs(this);
}



PdfLayer::~PdfLayer() {
	wxDELETE(m_pdfObjects);
	wxDELETE(m_pdfAttributs);
	wxDELETE(m_prjLayer);
}



bool PdfLayer::Generate(wxPdfDocument * pdf) {
	wxASSERT(pdf);
	pdf->SetFont(pdf->GetFontFamily(), "B", GetDocumentParent()->GetFontSize());
	pdf->Cell(0, GetDocumentParent()->GetLineSpacing(),
			  wxString::Format("%s (%s)",
							   m_prjLayer->m_LayerName,
							   PRJDEF_LAYERS_TYPE_STRING[m_prjLayer->m_LayerType]),
			  wxPDF_BORDER_FRAME);
	pdf->Ln();
	// write objects
	wxASSERT(m_pdfObjects);
	m_pdfObjects->Generate(pdf);
	pdf->Ln(GetDocumentParent()->GetLineSpacing());
	m_pdfAttributs->Generate(pdf);
	return true;
}



#include <wx/arrimpl.cpp>
WX_DEFINE_OBJARRAY(ArrayPdfLayer);











PdfObjects::PdfObjects(PdfLayer * parentlayer) {
	m_pdfLayerParent = parentlayer;
	wxASSERT(m_pdfLayerParent);
}



PdfObjects::~PdfObjects() {
}



bool PdfObjects::Generate(wxPdfDocument * pdf) {
	wxASSERT(m_pdfLayerParent);
	const ProjectDefMemoryLayers * layer = m_pdfLayerParent->GetPrjLayer();
	wxASSERT(layer);
	
	pdf->SetFont(pdf->GetFontFamily(), "I", 
				 m_pdfLayerParent->GetDocumentParent()->GetFontSize());
	// Column widths
    double colw[2] = {20,80};
	double linespace = m_pdfLayerParent->GetDocumentParent()->GetLineSpacing() / 3 * 2;
	
	// header
	pdf->Ln();
	
	pdf->Cell((colw[0] + colw[1]), m_pdfLayerParent->GetDocumentParent()->GetLineSpacing(),
			  _("Objects"), wxPDF_BORDER_FRAME, 1, wxPDF_ALIGN_CENTER);
	pdf->Cell(colw[0],m_pdfLayerParent->GetDocumentParent()->GetLineSpacing(),
			  _("Code"), wxPDF_BORDER_FRAME, 0, wxPDF_ALIGN_CENTER);
	pdf->Cell(colw[1],m_pdfLayerParent->GetDocumentParent()->GetLineSpacing(),
			  _("Description"), wxPDF_BORDER_FRAME, 0, wxPDF_ALIGN_CENTER);
	pdf->Ln();
	
	pdf->SetFont(pdf->GetFontFamily(), "", 
				 m_pdfLayerParent->GetDocumentParent()->GetFontSize());

	for (unsigned int i = 0; i<layer->m_pLayerObjectArray.GetCount(); i++) {
		pdf->Cell(colw[0], linespace,
				  wxString::Format("%ld",layer->m_pLayerObjectArray.Item(i)->m_ObjectCode),
				  wxPDF_BORDER_LEFT | wxPDF_BORDER_RIGHT, 0, wxPDF_ALIGN_CENTER);
		pdf->Cell(colw[1], linespace,
				  wxString::Format("%s", layer->m_pLayerObjectArray.Item(i)->m_ObjectName),
				  wxPDF_BORDER_LEFT | wxPDF_BORDER_RIGHT, 0);
		pdf->Ln(linespace);
	}
	pdf->Cell((colw[0]+colw[1]),0,"",wxPDF_BORDER_TOP);
	
	
	return true;
}








PdfAttributs::PdfAttributs(PdfLayer * parentlayer) {
	m_pdfLayerParent = parentlayer;
	wxASSERT(m_pdfLayerParent);
}



PdfAttributs::~PdfAttributs() {
}



bool PdfAttributs::Generate(wxPdfDocument * pdf) {
	double linenormal = m_pdfLayerParent->GetDocumentParent()->GetLineSpacing();
	double linesmall = linenormal / 3.0 * 2.0;
	bool bHasAttributs = false;
	const ProjectDefMemoryLayers * layer = m_pdfLayerParent->GetPrjLayer();
	wxASSERT(layer);

	
	pdf->Ln();
	pdf->Cell(100, linenormal, _("Attributs"), wxPDF_BORDER_FRAME, 1, wxPDF_ALIGN_CENTER);
	for (unsigned int i = 0; i<layer->m_pLayerFieldArray.GetCount(); i++) {
		bHasAttributs = true;
		pdf->Cell(100, linesmall, layer->m_pLayerFieldArray.Item(i)->m_Fieldname,
				  wxPDF_BORDER_LEFT | wxPDF_BORDER_RIGHT, 1);
	}
	
	if (bHasAttributs == true) {
		pdf->Cell(100, 0, "", wxPDF_BORDER_TOP);
	}
	
	pdf->Ln();
	
	return true;
}




