/***************************************************************************
 pdfexportwizard.h
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
#ifndef _PDFEXPORTWIZARD_H_
#define _PDFEXPORTWIZARD_H_

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/wizard.h>

#include "pdfdocument.h"


class PdfExportWizard : public wxWizard
{
private:
    wxWizardPageSimple *m_PageLayout;
    wxWizardPageSimple *m_PagePaper;
    wxWizardPageSimple *m_PageDecoration;

    // controls
    // page layout
    wxRadioButton *m_ExportOnePageCtrl;
    wxRadioButton *m_ExportMultiPageCtrl;
    wxRadioButton *m_ExportOneColCtrl;
    wxRadioButton *m_ExportTwoColCtrl;

    // page paper
    wxChoice *m_PaperSizeCtrl;
    wxRadioButton *m_PaperPortraitCtrl;
    wxRadioButton *m_PaperLandscapeCtrl;

    // page decoration
    wxCheckBox *m_DecorationSimpleCtrl;


    void _CreateControls();

    void OnRadioOnePageClick(wxCommandEvent &event);

    void OnRadioMultiplePageClick(wxCommandEvent &event);

    void OnLastPage(wxWizardEvent &event);

    wxString *m_TextBtnNext;


public:
    PdfExportWizard(wxWindow *parent, wxWindowID id = wxID_ANY, const wxString &title = _("Export Data Model Layout"));

    virtual ~PdfExportWizard();

    int ShowWizard();

    bool GetWizardChoices(PdfDocument &document);
};

#endif
