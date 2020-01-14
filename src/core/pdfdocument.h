/***************************************************************************
 pdfdocument.h
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
#ifndef _PDFDOCUMENT_H_
#define _PDFDOCUMENT_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/filename.h>
#include <wx/pdfdocument.h>
#include "pdflayer.h"


#ifndef MAX
#  define MIN(a, b)      ((a<b) ? a : b)
#  define MAX(a, b)      ((a>b) ? a : b)
#endif


class PrjDefMemManage;

class PdfDocument
{
private:
    ArrayPdfLayer m_pdfLayers;

    wxString m_prjName;
    wxPdfDocument *m_pdf;
    wxPaperSize m_PaperFormat;
    double m_PaperWidth;
    double m_PaperHeight;

    wxPrintOrientation m_PaperOrientation;
    int m_FontSize;
    double m_LineSpacing;
    double m_UsablePageWidth;
    bool m_Decorate;
    bool m_TwoCols;
    bool m_PageBreak;
    bool m_OnePage;

    bool _GenerateTitle();

    void _UpdatePageWidth();

    void _ComputeOnePageSize(double &width, double &height);

    bool _OrderLayers();

public:
    PdfDocument(PrjDefMemManage *project);

    virtual ~PdfDocument();

    bool Generate(const wxFileName &filename);

    inline const int GetFontSize() const;

    inline const double GetLineSpacing() const;

    inline const bool IsDecorated() const;

    void SetDecorate(bool value);

    void SetLineSpacing(double value);

    void SetFontSize(int value);

    void SetPaperFormat(wxPaperSize value);

    void SetPaperSize(double width, double height);

    void SetPaperOrientation(wxPrintOrientation value);

    double GetUsablePageWidth();

    bool IsLandscape();

    wxPdfDocument *GetPdfRef();

    inline const bool IsTwoColsLayout() const;

    void SetTwoColsLayout(bool value);

    inline const bool HasPageBreak() const;

    void SetPageBreak(bool value);

    void SetOnePage(bool value);
};


inline const int PdfDocument::GetFontSize() const
{
    return m_FontSize;
}


inline const double PdfDocument::GetLineSpacing() const
{
    return m_LineSpacing;
}


inline const bool PdfDocument::IsDecorated() const
{
    return m_Decorate;
}


inline const bool PdfDocument::IsTwoColsLayout() const
{
    return m_TwoCols;
}


inline const bool PdfDocument::HasPageBreak() const
{
    return m_PageBreak;
}


#endif
