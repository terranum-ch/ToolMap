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


void PdfLayer::_UpdateColumnWidth(){
	m_ColumnWidth.Clear();
	m_ColumnWidth.Add(GetDocumentParent()->GetUsablePageWidth() * 0.8);
	m_ColumnWidth.Add(GetDocumentParent()->GetUsablePageWidth() * 0.2);
}


bool PdfLayer::Generate(wxPdfDocument * pdf) {	
	_UpdateColumnWidth();
	wxASSERT(pdf);
	pdf->SetFont(pdf->GetFontFamily(), "B", GetDocumentParent()->GetFontSize());

	pdf->Cell(m_ColumnWidth.Item(0), GetDocumentParent()->GetLineSpacing(),
			  m_prjLayer->m_LayerName,
			  wxPDF_BORDER_TOP |wxPDF_BORDER_LEFT | wxPDF_BORDER_BOTTOM, 0);
	pdf->Cell(m_ColumnWidth.Item(1), GetDocumentParent()->GetLineSpacing(),
			  PRJDEF_LAYERS_TYPE_STRING[m_prjLayer->m_LayerType],
			  wxPDF_BORDER_TOP | wxPDF_BORDER_RIGHT | wxPDF_BORDER_BOTTOM, 0,
			  wxPDF_ALIGN_RIGHT);
	pdf->Ln();
	
	double myYPos = pdf->GetY();
	
	// write objects
	wxASSERT(m_pdfObjects);
	m_pdfObjects->Generate(pdf);
	
	if (GetDocumentParent()->IsLandscape() == true) {
		pdf->SetLeftMargin(5 + pdf->GetLeftMargin() + GetDocumentParent()->GetUsablePageWidth() / 2.0);
		pdf->SetX(5 + pdf->GetLeftMargin() + GetDocumentParent()->GetUsablePageWidth() / 2.0);
		pdf->SetY(myYPos);
	}
	pdf->Ln(GetDocumentParent()->GetLineSpacing());

	if (m_prjLayer->m_pLayerFieldArray.GetCount() > 0) {
		wxASSERT(m_pdfAttributs);
		m_pdfAttributs->Generate(pdf);
	}
		
	pdf->SetLeftMargin(10);
	pdf->SetX(10);
	
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


void PdfObjects::_UpdateColumnWidth(){
	m_ColumnWidth.Clear();
	if (m_pdfLayerParent->GetDocumentParent()->IsLandscape() == true) {
		m_ColumnWidth.Add((m_pdfLayerParent->GetDocumentParent()->GetUsablePageWidth() - 10) * 0.2 / 2.0) ;
		m_ColumnWidth.Add((m_pdfLayerParent->GetDocumentParent()->GetUsablePageWidth() - 10) * 0.8 / 2.0) ;
	}
	else {
		m_ColumnWidth.Add(m_pdfLayerParent->GetDocumentParent()->GetUsablePageWidth() * 0.2);
		m_ColumnWidth.Add(m_pdfLayerParent->GetDocumentParent()->GetUsablePageWidth() * 0.8);
	}
	
	
}


bool PdfObjects::Generate(wxPdfDocument * pdf) {
	_UpdateColumnWidth();
	wxASSERT(m_pdfLayerParent);
	const ProjectDefMemoryLayers * layer = m_pdfLayerParent->GetPrjLayer();
	wxASSERT(layer);
	
	pdf->SetFont(pdf->GetFontFamily(), "BI", 
				 m_pdfLayerParent->GetDocumentParent()->GetFontSize());
	double linespace = m_pdfLayerParent->GetDocumentParent()->GetLineSpacing() / 3 * 2;
	
	// header
	pdf->Ln();
	
	pdf->Cell(m_ColumnWidth.Item(0) + m_ColumnWidth.Item(1),
			  m_pdfLayerParent->GetDocumentParent()->GetLineSpacing(),
			  _("Objects"), wxPDF_BORDER_FRAME, 1, wxPDF_ALIGN_CENTER);
	pdf->Cell(m_ColumnWidth.Item(0),m_pdfLayerParent->GetDocumentParent()->GetLineSpacing(),
			  _("Code"), wxPDF_BORDER_FRAME, 0, wxPDF_ALIGN_CENTER);
	pdf->Cell(m_ColumnWidth.Item(1),m_pdfLayerParent->GetDocumentParent()->GetLineSpacing(),
			  _("Description"), wxPDF_BORDER_FRAME, 0, wxPDF_ALIGN_CENTER);
	pdf->Ln();
	
	pdf->SetFont(pdf->GetFontFamily(), "", 
				 m_pdfLayerParent->GetDocumentParent()->GetFontSize());

	for (unsigned int i = 0; i<layer->m_pLayerObjectArray.GetCount(); i++) {
		pdf->Cell(m_ColumnWidth.Item(0), linespace,
				  wxString::Format("%ld",layer->m_pLayerObjectArray.Item(i)->m_ObjectCode),
				  wxPDF_BORDER_LEFT | wxPDF_BORDER_RIGHT, 0, wxPDF_ALIGN_CENTER);
		pdf->Cell(m_ColumnWidth.Item(1), linespace,
				  wxString::Format("%s", layer->m_pLayerObjectArray.Item(i)->m_ObjectName),
				  wxPDF_BORDER_LEFT | wxPDF_BORDER_RIGHT, 0);
		pdf->Ln(linespace);
	}
	pdf->Cell(m_ColumnWidth.Item(0) + m_ColumnWidth.Item(1),0,"",wxPDF_BORDER_TOP);
	
	
	return true;
}








PdfAttributs::PdfAttributs(PdfLayer * parentlayer) {
	m_pdfLayerParent = parentlayer;
	wxASSERT(m_pdfLayerParent);
}



PdfAttributs::~PdfAttributs() {
}


void PdfAttributs::_UpdateColumnWidth(){
	m_ColumnWidth.Clear();
	
	if (m_pdfLayerParent->GetDocumentParent()->IsLandscape() == true) {
		m_ColumnWidth.Add((m_pdfLayerParent->GetDocumentParent()->GetUsablePageWidth() - 10) * 0.4 / 2.0) ;
		m_ColumnWidth.Add((m_pdfLayerParent->GetDocumentParent()->GetUsablePageWidth() - 10) * 0.6 / 2.0) ;
	}
	else {
		m_ColumnWidth.Add(m_pdfLayerParent->GetDocumentParent()->GetUsablePageWidth() * 0.4);
		m_ColumnWidth.Add(m_pdfLayerParent->GetDocumentParent()->GetUsablePageWidth() * 0.6);
	}
	
}



void PdfAttributs::_GenerateFieldDef (wxPdfDocument * pdf, ProjectDefMemoryFields * field){
	double linenormal = m_pdfLayerParent->GetDocumentParent()->GetLineSpacing();
	double linesmall = linenormal / 3.0 * 2.0;
	
	switch (field->m_FieldType) {
		case TM_FIELD_TEXT:
			pdf->Cell(m_ColumnWidth.Item(1), linesmall, wxString::Format(_("%s (max: %d characters)"),
															PRJDEF_FIELD_TYPE_STRING[TM_FIELD_TEXT],
															field->m_FieldPrecision),
					 wxPDF_BORDER_LEFT | wxPDF_BORDER_RIGHT, 0);
			break;
			
		case TM_FIELD_ENUMERATION:
			for (unsigned int i = 0; i<field->m_pCodedValueArray.GetCount(); i++) {
				if (i != 0) {
					pdf->Ln();
					pdf->Cell(m_ColumnWidth.Item(0), linesmall, "",wxPDF_BORDER_LEFT | wxPDF_BORDER_RIGHT, 0);
				}
				pdf->Cell(m_ColumnWidth.Item(1), linesmall, field->m_pCodedValueArray.Item(i)->m_ValueName, wxPDF_BORDER_LEFT | wxPDF_BORDER_RIGHT, 0);
			}
			break;
			
		case TM_FIELD_INTEGER:
			pdf->Cell(m_ColumnWidth.Item(1), linesmall, PRJDEF_FIELD_TYPE_STRING[TM_FIELD_INTEGER],
					  wxPDF_BORDER_LEFT | wxPDF_BORDER_RIGHT, 0);
			break;
			
			
		case TM_FIELD_DATE:
			pdf->Cell(m_ColumnWidth.Item(1), linesmall, PRJDEF_FIELD_TYPE_STRING[TM_FIELD_DATE],
					  wxPDF_BORDER_LEFT | wxPDF_BORDER_RIGHT, 0);
			break;
			
			
		case TM_FIELD_FLOAT:
		{
			wxString myPrecisionScale = wxEmptyString;
			for (int i = 0; i< field->m_FieldPrecision; i++) {
				myPrecisionScale.Append("9");
			}
			myPrecisionScale.Append(".");
			for (int i = 0; i< field->m_FieldScale; i++) {
				myPrecisionScale.Append ("9");
			}
			
			pdf->Cell(m_ColumnWidth.Item(1), linesmall, 
					  wxString::Format(_("%s (max: %s)"),
									   PRJDEF_FIELD_TYPE_STRING[TM_FIELD_FLOAT],
									   myPrecisionScale),
					  wxPDF_BORDER_LEFT | wxPDF_BORDER_RIGHT, 0);
		}
			break;
			
			
			
			
		default:
			pdf->Cell(m_ColumnWidth.Item(1), linesmall, "NOT SUPPORTED NOW!", wxPDF_BORDER_LEFT | wxPDF_BORDER_RIGHT, 0);
			break;
	}
	
}



bool PdfAttributs::Generate(wxPdfDocument * pdf) {
	_UpdateColumnWidth();
	double linenormal = m_pdfLayerParent->GetDocumentParent()->GetLineSpacing();
	double linesmall = linenormal / 3.0 * 2.0;
	bool bHasAttributs = false;
	const ProjectDefMemoryLayers * layer = m_pdfLayerParent->GetPrjLayer();
	wxASSERT(layer);

	
	pdf->Ln();
	
	pdf->SetFont(pdf->GetFontFamily(), "BI", 
				 m_pdfLayerParent->GetDocumentParent()->GetFontSize());
	pdf->Cell(m_ColumnWidth.Item(0) + m_ColumnWidth.Item(1), linenormal, _("Attributs"), wxPDF_BORDER_FRAME, 1, wxPDF_ALIGN_CENTER);
	pdf->SetFont(pdf->GetFontFamily(), "", 
				 m_pdfLayerParent->GetDocumentParent()->GetFontSize());
	for (unsigned int i = 0; i<layer->m_pLayerFieldArray.GetCount(); i++) {
		bHasAttributs = true;
		pdf->Cell(m_ColumnWidth.Item(0), linesmall, layer->m_pLayerFieldArray.Item(i)->m_Fieldname,
				  wxPDF_BORDER_LEFT | wxPDF_BORDER_RIGHT, 0);
		_GenerateFieldDef(pdf, layer->m_pLayerFieldArray.Item(i));
		pdf->Ln();
	}
	
	if (bHasAttributs == true) {
		pdf->Cell(m_ColumnWidth.Item(0) + m_ColumnWidth.Item(1), 0, "", wxPDF_BORDER_TOP);
	}
	
	pdf->Ln();
	
	return true;
}




