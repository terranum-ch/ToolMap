/***************************************************************************
 pdflayer.cpp
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

#include "pdflayer.h"

#include "pdfdocument.h"
#include "projectdefmemory.h"

void PdfLayer::_UpdateColWidth() {
    // layers columns
    m_ColWidthLayer.Clear();
    m_ColWidthLayer.Add(m_pdfDocumentParent->GetUsablePageWidth() * 0.8);
    m_ColWidthLayer.Add(m_pdfDocumentParent->GetUsablePageWidth() * 0.2);

    // object columns
    m_ColWidthObjects.Clear();
    if (m_pdfDocumentParent->IsTwoColsLayout()) {
        m_ColWidthObjects.Add((m_pdfDocumentParent->GetUsablePageWidth() - 10) * 0.2 / 2.0);
        m_ColWidthObjects.Add((m_pdfDocumentParent->GetUsablePageWidth() - 10) * 0.8 / 2.0);
    } else {
        m_ColWidthObjects.Add(m_pdfDocumentParent->GetUsablePageWidth() * 0.2);
        m_ColWidthObjects.Add(m_pdfDocumentParent->GetUsablePageWidth() * 0.8);
    }

    // attributs columns
    m_ColWidthAttributs.Clear();
    if (m_pdfDocumentParent->IsTwoColsLayout()) {
        m_ColWidthAttributs.Add((m_pdfDocumentParent->GetUsablePageWidth() - 10) * 0.2 / 2.0);
        m_ColWidthAttributs.Add((m_pdfDocumentParent->GetUsablePageWidth() - 10) * 0.2 / 2.0);
        m_ColWidthAttributs.Add((m_pdfDocumentParent->GetUsablePageWidth() - 10) * 0.6 / 2.0);
    } else {
        m_ColWidthAttributs.Add(m_pdfDocumentParent->GetUsablePageWidth() * 0.2);
        m_ColWidthAttributs.Add(m_pdfDocumentParent->GetUsablePageWidth() * 0.2);
        m_ColWidthAttributs.Add(m_pdfDocumentParent->GetUsablePageWidth() * 0.6);
    }
}

void PdfLayer::_GenerateObjects() {
    wxPdfDocument* mypPdf = m_pdfDocumentParent->GetPdfRef();
    wxASSERT(mypPdf);
    wxASSERT(m_prjLayer);

    double myLineNormal = m_pdfDocumentParent->GetLineSpacing();
    double myLineSmall = myLineNormal * 2.0 / 3.0;

    // header
    mypPdf->SetFont(mypPdf->GetFontFamily(), "BI", m_pdfDocumentParent->GetFontSize());
    mypPdf->Ln();
    mypPdf->Cell(m_ColWidthObjects.Item(0) + m_ColWidthObjects.Item(1), myLineNormal, _("Objects"), wxPDF_BORDER_FRAME,
                 1, wxPDF_ALIGN_CENTER);
    mypPdf->Cell(m_ColWidthObjects.Item(0), myLineNormal, _("Code"), wxPDF_BORDER_FRAME, 0, wxPDF_ALIGN_CENTER);
    mypPdf->Cell(m_ColWidthObjects.Item(1), myLineNormal, _("Description"), wxPDF_BORDER_FRAME, 0, wxPDF_ALIGN_CENTER);
    mypPdf->Ln();
    mypPdf->SetFont(mypPdf->GetFontFamily(), "", m_pdfDocumentParent->GetFontSize());

    int fill = 0;
    if (m_pdfDocumentParent->IsDecorated()) {
        mypPdf->SetFillColour(220);
    }

    // objects
    for (unsigned int i = 0; i < m_prjLayer->m_pLayerObjectArray.GetCount(); i++) {
        mypPdf->Cell(m_ColWidthObjects.Item(0), myLineSmall, m_prjLayer->m_pLayerObjectArray.Item(i)->m_ObjectCode,
                     wxPDF_BORDER_LEFT | wxPDF_BORDER_RIGHT, 0, wxPDF_ALIGN_CENTER, fill);

        // special case for lines building polygons
        wxString myAsterisk = wxEmptyString;
        if (m_prjLayer->m_pLayerObjectArray.Item(i)->m_ObjectType == LAYER_LINE &&
            m_prjLayer->m_LayerType == LAYER_POLYGON) {
            myAsterisk = "*";
        }

        mypPdf->Cell(m_ColWidthObjects.Item(1), myLineSmall,
                     wxString::Format("%s%s", m_prjLayer->m_pLayerObjectArray.Item(i)->m_ObjectName, myAsterisk),
                     wxPDF_BORDER_LEFT | wxPDF_BORDER_RIGHT, 0, wxPDF_ALIGN_LEFT, fill);

        mypPdf->Ln(myLineSmall);

        if (m_pdfDocumentParent->IsDecorated()) {
            fill = 1 - fill;
        }
    }
    mypPdf->Cell(m_ColWidthObjects.Item(0) + m_ColWidthObjects.Item(1), 0, "", wxPDF_BORDER_TOP);
}

void PdfLayer::_GenerateAttributs() {
    wxPdfDocument* mypPdf = m_pdfDocumentParent->GetPdfRef();
    wxASSERT(mypPdf);
    wxASSERT(m_prjLayer);

    double myLineNormal = m_pdfDocumentParent->GetLineSpacing();
    double myLineSmall = myLineNormal * 2.0 / 3.0;

    // header
    mypPdf->Ln(myLineNormal);
    mypPdf->SetFont(mypPdf->GetFontFamily(), "BI", m_pdfDocumentParent->GetFontSize());
    mypPdf->Cell(m_ColWidthAttributs.Item(0) + m_ColWidthAttributs.Item(1) + m_ColWidthAttributs.Item(2), myLineNormal,
                 _("Attributes"), wxPDF_BORDER_FRAME, 1, wxPDF_ALIGN_CENTER);

    mypPdf->Cell(m_ColWidthAttributs.Item(0), myLineNormal, _("Name"), wxPDF_BORDER_FRAME, 0, wxPDF_ALIGN_CENTER);
    mypPdf->Cell(m_ColWidthAttributs.Item(1), myLineNormal, _("Type"), wxPDF_BORDER_FRAME, 0, wxPDF_ALIGN_CENTER);
    mypPdf->Cell(m_ColWidthAttributs.Item(2), myLineNormal, _("List of values"), wxPDF_BORDER_FRAME, 0,
                 wxPDF_ALIGN_CENTER);
    mypPdf->Ln();
    mypPdf->SetFont(mypPdf->GetFontFamily(), "", m_pdfDocumentParent->GetFontSize());

    // fields
    bool bHasAttributs = false;
    for (unsigned int i = 0; i < m_prjLayer->m_pLayerFieldArray.GetCount(); i++) {
        bHasAttributs = true;
        mypPdf->Cell(m_ColWidthAttributs.Item(0), myLineSmall, m_prjLayer->m_pLayerFieldArray.Item(i)->m_Fieldname,
                     wxPDF_BORDER_LEFT | wxPDF_BORDER_RIGHT | wxPDF_BORDER_TOP, 0);
        _GenerateAttributsDefinition(m_prjLayer->m_pLayerFieldArray.Item(i));
        mypPdf->Ln();
    }

    if (bHasAttributs) {
        mypPdf->Cell(m_ColWidthAttributs.Item(0) + m_ColWidthAttributs.Item(1) + m_ColWidthAttributs.Item(2), 0, "",
                     wxPDF_BORDER_TOP);
    }
}

void PdfLayer::_GenerateAttributsDefinition(ProjectDefMemoryFields* field) {
    wxPdfDocument* mypPdf = m_pdfDocumentParent->GetPdfRef();
    wxASSERT(mypPdf);
    wxASSERT(field);
    double myLineSmall = m_pdfDocumentParent->GetLineSpacing() * 2.0 / 3.0;

    switch (field->m_FieldType) {
        case TM_FIELD_TEXT:
            mypPdf->Cell(m_ColWidthAttributs.Item(1), myLineSmall,
                         wxString::Format(_("%s (max: %d characters)"), PRJDEF_FIELD_TYPE_STRING[TM_FIELD_TEXT],
                                          field->m_FieldPrecision),
                         wxPDF_BORDER_LEFT | wxPDF_BORDER_RIGHT | wxPDF_BORDER_TOP, 0);
            mypPdf->Cell(m_ColWidthAttributs.Item(2), myLineSmall, "", wxPDF_BORDER_RIGHT | wxPDF_BORDER_TOP, 0);
            break;

        case TM_FIELD_ENUMERATION: {
            int fill = 0;
            if (m_pdfDocumentParent->IsDecorated()) {
                mypPdf->SetFillColour(220);
            }

            for (unsigned int i = 0; i < field->m_pCodedValueArray.GetCount(); i++) {
                if (i != 0) {
                    mypPdf->Ln();
                    mypPdf->Cell(m_ColWidthAttributs.Item(0), myLineSmall, "", wxPDF_BORDER_LEFT | wxPDF_BORDER_RIGHT,
                                 0);
                }

                wxString myAttributText = wxString::Format(_T("%s  (%s)"),
                                                           field->m_pCodedValueArray.Item(i)->m_ValueName,
                                                           field->m_pCodedValueArray.Item(i)->m_ValueCode);
                if (i == 0) {
                    mypPdf->Cell(m_ColWidthAttributs.Item(1), myLineSmall,
                                 PRJDEF_FIELD_TYPE_STRING[TM_FIELD_ENUMERATION],
                                 wxPDF_BORDER_LEFT | wxPDF_BORDER_RIGHT | wxPDF_BORDER_TOP, 0, wxPDF_ALIGN_LEFT);
                    mypPdf->Cell(m_ColWidthAttributs.Item(2), myLineSmall, myAttributText,
                                 wxPDF_BORDER_RIGHT | wxPDF_BORDER_TOP, 0, wxPDF_ALIGN_LEFT, fill);

                } else {
                    mypPdf->Cell(m_ColWidthAttributs.Item(1), myLineSmall, "", wxPDF_BORDER_LEFT, 0);
                    mypPdf->Cell(m_ColWidthAttributs.Item(2), myLineSmall, myAttributText,
                                 wxPDF_BORDER_LEFT | wxPDF_BORDER_RIGHT, 0, wxPDF_ALIGN_LEFT, fill);
                }

                if (m_pdfDocumentParent->IsDecorated()) {
                    fill = 1 - fill;
                }
            }
        } break;

        case TM_FIELD_INTEGER:
            mypPdf->Cell(m_ColWidthAttributs.Item(1), myLineSmall, PRJDEF_FIELD_TYPE_STRING[TM_FIELD_INTEGER],
                         wxPDF_BORDER_LEFT | wxPDF_BORDER_RIGHT | wxPDF_BORDER_TOP, 0);
            mypPdf->Cell(m_ColWidthAttributs.Item(2), myLineSmall, "", wxPDF_BORDER_RIGHT | wxPDF_BORDER_TOP, 0);

            break;

        case TM_FIELD_DATE:
            mypPdf->Cell(m_ColWidthAttributs.Item(1), myLineSmall, PRJDEF_FIELD_TYPE_STRING[TM_FIELD_DATE],
                         wxPDF_BORDER_LEFT | wxPDF_BORDER_RIGHT | wxPDF_BORDER_TOP, 0);
            mypPdf->Cell(m_ColWidthAttributs.Item(2), myLineSmall, "", wxPDF_BORDER_RIGHT | wxPDF_BORDER_TOP, 0);

            break;

        case TM_FIELD_FLOAT: {
            wxString myPrecisionScale = wxEmptyString;
            for (int i = 0; i < field->m_FieldPrecision - field->m_FieldScale; i++) {
                myPrecisionScale.Append("9");
            }
            myPrecisionScale.Append(".");
            for (int i = 0; i < field->m_FieldScale; i++) {
                myPrecisionScale.Append("9");
            }

            mypPdf->Cell(
                m_ColWidthAttributs.Item(1), myLineSmall,
                wxString::Format(_("%s (max: %s)"), PRJDEF_FIELD_TYPE_STRING[TM_FIELD_FLOAT], myPrecisionScale),
                wxPDF_BORDER_LEFT | wxPDF_BORDER_RIGHT | wxPDF_BORDER_TOP, 0);
            mypPdf->Cell(m_ColWidthAttributs.Item(2), myLineSmall, "", wxPDF_BORDER_RIGHT | wxPDF_BORDER_TOP, 0);
        } break;

        default:
            mypPdf->Cell(m_ColWidthAttributs.Item(1), myLineSmall, "NOT SUPPORTED NOW!",
                         wxPDF_BORDER_LEFT | wxPDF_BORDER_TOP, 0);
            mypPdf->Cell(m_ColWidthAttributs.Item(2), myLineSmall, "NOT SUPPORTED NOW!",
                         wxPDF_BORDER_LEFT | wxPDF_BORDER_RIGHT | wxPDF_BORDER_TOP, 0);
            break;
    }
}

PdfLayer::PdfLayer(PdfDocument* parent, ProjectDefMemoryLayers* layer) {
    wxASSERT(layer);
    wxASSERT(parent);

    m_pdfDocumentParent = parent;
    m_prjLayer = new ProjectDefMemoryLayers();
    *m_prjLayer = *layer;
    wxASSERT(m_prjLayer);
}

PdfLayer::~PdfLayer() {
    wxDELETE(m_prjLayer);
}

bool PdfLayer::Generate() {
    _UpdateColWidth();
    wxPdfDocument* mypPdf = m_pdfDocumentParent->GetPdfRef();
    wxASSERT(mypPdf);
    int myDecorate = 0;
    if (m_pdfDocumentParent->IsDecorated()) {
        myDecorate = 1;
    }

    // layer title
    if (myDecorate == 1) {
        mypPdf->SetFillColour(0);
        mypPdf->SetTextColour(*wxWHITE);
    }

    mypPdf->SetFont(mypPdf->GetFontFamily(), "B", m_pdfDocumentParent->GetFontSize() + 2);
    mypPdf->Cell(m_ColWidthLayer.Item(0), m_pdfDocumentParent->GetLineSpacing(), m_prjLayer->m_LayerName,
                 wxPDF_BORDER_TOP | wxPDF_BORDER_LEFT | wxPDF_BORDER_BOTTOM, 0, wxPDF_ALIGN_LEFT, myDecorate);
    mypPdf->Cell(m_ColWidthLayer.Item(1), m_pdfDocumentParent->GetLineSpacing(),
                 PRJDEF_LAYERS_TYPE_STRING[m_prjLayer->m_LayerType],
                 wxPDF_BORDER_TOP | wxPDF_BORDER_RIGHT | wxPDF_BORDER_BOTTOM, 0, wxPDF_ALIGN_RIGHT, myDecorate);
    mypPdf->Ln();
    mypPdf->SetFont(mypPdf->GetFontFamily(), "", m_pdfDocumentParent->GetFontSize());
    if (myDecorate == 1) {
        mypPdf->SetFillColour(0);
        mypPdf->SetTextColour(*wxBLACK);
    }
    double myYPosStart = mypPdf->GetY();

    // write objects
    _GenerateObjects();
    double myYPosStopObj = mypPdf->GetY();
    double myYPosStopAttrib = myYPosStopObj;

    // write attributs
    if (m_prjLayer->m_pLayerFieldArray.GetCount() > 0) {
        if (m_pdfDocumentParent->IsTwoColsLayout()) {
            mypPdf->SetLeftMargin(5 + mypPdf->GetLeftMargin() + m_pdfDocumentParent->GetUsablePageWidth() / 2.0);
            mypPdf->SetX(5 + mypPdf->GetLeftMargin() + m_pdfDocumentParent->GetUsablePageWidth() / 2.0);
            mypPdf->SetY(myYPosStart);
        }
        _GenerateAttributs();
        myYPosStopAttrib = mypPdf->GetY();
    }

    if (myYPosStopObj > myYPosStopAttrib && myYPosStopAttrib > myYPosStart) {
        wxLogDebug("Object greater than attributes");
        mypPdf->SetY(myYPosStopObj);
    }

    // page break
    // if (myYPosStopAttrib > myYPosStart) {

    // }

    // indicate a page break!
    /*if (mypPdf->GetY() < myYPosStart) {
        wxLogDebug ("position y :%f position y start: %f", mypPdf->GetY(), myYPosStart);
        mypPdf->SetY(0);
    }
    else if (myYPosStopObj > mypPdf->GetY()) {
        wxLogDebug ("position stop y :%f position y : %f", myYPosStopObj, mypPdf->GetY());
        mypPdf->SetY(myYPosStopObj);
    }*/

    mypPdf->Ln(m_pdfDocumentParent->GetLineSpacing());

    mypPdf->SetLeftMargin(10);
    mypPdf->SetX(10);
    return true;
}

wxString PdfLayer::GetName() {
    wxASSERT(m_prjLayer);
    return m_prjLayer->m_LayerName;
}

int PdfLayer::GetLayerTypeOrder() {
    wxASSERT(m_prjLayer);
    if (m_prjLayer->m_LayerType == LAYER_POINT) {
        return 0;
    }

    if (m_prjLayer->m_LayerType == LAYER_LINE) {
        return 1;
    }

    if (m_prjLayer->m_LayerType == LAYER_POLYGON) {
        return 2;
    }

    wxFAIL;
    return wxNOT_FOUND;
}

double PdfLayer::GetObjectsWidth(wxPdfDocument* pdf) {
    wxASSERT(pdf);
    wxASSERT(m_prjLayer);

    double maxcodewidth = 0;
    double maxdescwidth = 0;
    for (unsigned int i = 0; i < m_prjLayer->m_pLayerObjectArray.GetCount(); i++) {
        wxString myCodeAsTxt = m_prjLayer->m_pLayerObjectArray.Item(i)->m_ObjectCode;
        double codewidth = pdf->GetStringWidth(myCodeAsTxt);
        double descwidth = pdf->GetStringWidth(m_prjLayer->m_pLayerObjectArray.Item(i)->m_ObjectName);
        maxcodewidth = MAX(maxcodewidth, codewidth);
        maxdescwidth = MAX(maxdescwidth, descwidth);
    }

    // code width = 20 %
    // description width = 4 * 20 %
    double myMax = MAX(maxcodewidth, maxdescwidth / 4.0);
    return myMax * 5.0;
}

double PdfLayer::GetAttributsWidth(wxPdfDocument* pdf) {
    wxASSERT(pdf);
    wxASSERT(m_prjLayer);

    double maxFieldNameWidth = 0;
    double maxFieldContentWidth = 0;
    double maxFieldTypeWidth = 0;
    for (unsigned int f = 0; f < m_prjLayer->m_pLayerFieldArray.GetCount(); f++) {
        ProjectDefMemoryFields* myField = m_prjLayer->m_pLayerFieldArray.Item(f);
        wxASSERT(myField);

        // field name
        double myFieldWidth = pdf->GetStringWidth(myField->m_Fieldname);
        maxFieldNameWidth = MAX(maxFieldNameWidth, myFieldWidth);

        // field type
        switch (myField->m_FieldType) {
            case TM_FIELD_INTEGER:
            case TM_FIELD_DATE:
            case TM_FIELD_ENUMERATION:
                maxFieldTypeWidth = MAX(maxFieldTypeWidth,
                                        pdf->GetStringWidth(PRJDEF_FIELD_TYPE_STRING[myField->m_FieldType] + _T("  ")));
                break;

            case TM_FIELD_TEXT: {
                wxString myDummyText = wxString::Format(_("%s (max: %d characters)     "),
                                                        PRJDEF_FIELD_TYPE_STRING[TM_FIELD_TEXT],
                                                        myField->m_FieldPrecision);
                maxFieldTypeWidth = MAX(maxFieldTypeWidth, pdf->GetStringWidth(myDummyText));
            } break;

            case TM_FIELD_FLOAT: {
                wxString myPrecisionScale = wxEmptyString;
                myPrecisionScale.Pad(myField->m_FieldPrecision, '9');
                myPrecisionScale.Append(_T("."));

                wxString myDummyText = wxString::Format(_("%s (max: %s)     "),
                                                        PRJDEF_FIELD_TYPE_STRING[TM_FIELD_FLOAT], myPrecisionScale);
                maxFieldTypeWidth = MAX(maxFieldTypeWidth, pdf->GetStringWidth(myDummyText));
            } break;
        }

        // field content
        if (myField->m_FieldType == TM_FIELD_ENUMERATION) {
            for (unsigned i = 0; i < myField->m_pCodedValueArray.GetCount(); i++) {
                double myTempWidth = pdf->GetStringWidth(myField->m_pCodedValueArray.Item(i)->m_ValueName);
                maxFieldContentWidth = MAX(maxFieldContentWidth, myTempWidth);
            }
        }
    }

    // field name = 20%
    // field type = 20%
    // field content = 3 * 20%
    double myMax = MAX(maxFieldNameWidth, maxFieldTypeWidth);
    myMax = MAX(myMax, maxFieldContentWidth / 3.0);
    return myMax * 5.0;
}

double PdfLayer::GetObjectsHeight(wxPdfDocument* pdf) {
    wxASSERT(pdf);
    wxASSERT(m_prjLayer);
    wxASSERT(m_pdfDocumentParent);

    double myLineHeight = m_pdfDocumentParent->GetLineSpacing();
    double myLineHeightSml = myLineHeight * 2.0 / 3.0;

    double myHeight = 0;
    myHeight += 3 * myLineHeight;  // Objects, Code - Descriptions lines (2)
    myHeight += m_prjLayer->m_pLayerObjectArray.GetCount() * myLineHeightSml;

    return myHeight;
}

double PdfLayer::GetAttributsHeight(wxPdfDocument* pdf) {
    wxASSERT(pdf);
    wxASSERT(m_prjLayer);
    wxASSERT(m_pdfDocumentParent);

    double myLineHeight = m_pdfDocumentParent->GetLineSpacing();
    double myLineHeightSml = myLineHeight * 2.0 / 3.0;

    double myHeight = 0;
    myHeight += myLineHeight;                                              // Attributs
    myHeight += m_prjLayer->m_pLayerFieldArray.GetCount() * myLineHeight;  // Attributs name + Desc
    for (unsigned int i = 0; i < m_prjLayer->m_pLayerFieldArray.GetCount(); i++) {
        ProjectDefMemoryFields* myField = m_prjLayer->m_pLayerFieldArray.Item(i);
        wxASSERT(myField);
        if (myField->m_FieldType == TM_FIELD_ENUMERATION) {
            myHeight += (myField->m_pCodedValueArray.GetCount() - 1) * myLineHeightSml;
        }
    }
    return myHeight;
}

#include <wx/arrimpl.cpp>
WX_DEFINE_OBJARRAY(ArrayPdfLayer);
