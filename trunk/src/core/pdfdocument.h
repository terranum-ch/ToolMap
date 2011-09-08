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
#include "pdflayer.h"


class PrjDefMemManage;

class PdfDocument {
private:
    ArrayPdfLayer m_pdfLayers;
	
    wxString m_prjName;
    wxPdfDocument m_pdf;
    wxPaperSize m_PaperSize;
    wxPrintOrientation m_PaperOrientation;
    int m_FontSize;
	double m_LineSpacing;
	double m_UsablePageWidth;
	
    bool _GenerateTitle();
	void _UpdatePageWidth();
	
public:
    PdfDocument(PrjDefMemManage * project);
    virtual ~PdfDocument();
	
    bool Generate(const wxFileName & filename);
	
	inline const int GetFontSize() const;	
	inline const double GetLineSpacing() const;
    
	void SetLineSpacing(double value);	
	void SetFontSize(int value);
	void SetPaperSize(wxPaperSize value);
    void SetPaperOrientation(wxPrintOrientation value);
	
    double GetUsablePageWidth();
	bool IsLandscape();
};


inline const int PdfDocument::GetFontSize() const {
	return m_FontSize;
}



inline const double PdfDocument::GetLineSpacing() const {
	return m_LineSpacing;
}


#endif
