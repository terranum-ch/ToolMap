/***************************************************************************
 pdflayer.h
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
#ifndef _PDFLAYER_H_
#define _PDFLAYER_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <wx/pdfdocument.h>


class ProjectDefMemoryLayers;
class PdfDocument;

class PdfLayer {
  private:
    //PdfObjects * m_pdfObjects;
    //PdfAttributs * m_pdfAttributs;
    PdfDocument * m_pdfDocumentParent;

    ProjectDefMemoryLayers * m_prjLayer;

  public:
    PdfLayer(PdfDocument * parent, ProjectDefMemoryLayers * layer);
    virtual ~PdfLayer();
    
	bool Generate(wxPdfDocument * pdf);
	
	inline const PdfDocument * GetDocumentParent() const;
};


inline const PdfDocument * PdfLayer::GetDocumentParent() const {
  return m_pdfDocumentParent;
}


WX_DECLARE_OBJARRAY(PdfLayer*, ArrayPdfLayer);
#endif
