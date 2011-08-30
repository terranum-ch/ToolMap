/***************************************************************************
 pdfdocument.h
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
#ifndef _PDFDOCUMENT_H_
#define _PDFDOCUMENT_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/filename.h>
#include <wx/pdfdocument.h>

class PrjDefMemManage;

class PdfDocument {
  private:
    //PdfLayer m_pdfLayers[10];

    wxString m_prjName;
    wxPdfDocument m_pdf;
    wxPaperSize m_PaperSize;
    wxPrintOrientation m_PaperOrientation;
    int m_FontSize;
	double m_LineSpacing;

    bool _GenerateTitle();


  public:
    PdfDocument(PrjDefMemManage * project);
    virtual ~PdfDocument();

    bool Generate(const wxFileName & filename);

};
#endif
