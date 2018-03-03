/***************************************************************************
 tmimportwizpages.h
 
 -------------------
 copyright            : (C) 2010 CREALP Lucien Schreiber 
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

#ifndef _TMIMPORTWIZPAGES_H
#define _TMIMPORTWIZPAGES_H

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/wizard.h>
#include <wx/filepicker.h>

class ImportWizard;

class ImportWizCSVOptions;

class ImportWizTarget;

class ImportWizIntro : public wxWizardPageSimple
{
private:
    ImportWizard *m_Parent;
    wxRadioButton *m_radioBtn1;
    wxRadioButton *m_radioBtn2;

    void _CreateControls();


public:
    ImportWizIntro(ImportWizard *parent, wxWizardPage *next);

    virtual ~ImportWizIntro();

    virtual bool TransferDataToWindow();

    virtual bool TransferDataFromWindow();
};


class ImportWizInfo : public wxWizardPage
{
private:
    ImportWizard *m_Parent;
    wxWizardPage *m_Prev;
    wxWizardPage *m_Next;
    ImportWizCSVOptions *m_PageCSVOptions;
    ImportWizTarget *m_PageTarget;
    wxFilePickerCtrl *m_FileCtrl;
    wxStaticText *m_InfoLabelCtrl1;
    wxStaticText *m_InfoValueCtrl1;
    wxStaticText *m_InfoLabelCtrl2;
    wxStaticText *m_InfoValueCtrl2;

    void _CreateControls();

    void OnOpenFile(wxFileDirPickerEvent &event);

public:
    ImportWizInfo(ImportWizard *parent);

    virtual ~ImportWizInfo();

    virtual wxWizardPage *GetPrev() const;

    void SetPreviousPage(wxWizardPage *prev)
    { m_Prev = prev; }

    virtual wxWizardPage *GetNext() const;

    virtual bool TransferDataToWindow();

    virtual bool TransferDataFromWindow();

};


class ImportWizCSVOptions : public wxWizardPageSimple
{
private:
    ImportWizard *m_Parent;
    wxChoice *m_XColCtrl;
    wxChoice *m_YColCtrl;

    void _CreateControls();

public:
    ImportWizCSVOptions(ImportWizard *parent, wxWizardPage *prev, wxWizardPage *next);

    virtual ~ImportWizCSVOptions();

    virtual bool TransferDataToWindow();

    virtual bool TransferDataFromWindow();

};


class ImportWizTarget : public wxWizardPageSimple
{
private:
    ImportWizard *m_Parent;
    wxChoice *m_TargetCtrl;

    void _CreateControls();

public:
    ImportWizTarget(ImportWizard *parent, wxWizardPage *prev, wxWizardPage *next);

    virtual ~ImportWizTarget();

    virtual bool TransferDataToWindow();

    virtual bool TransferDataFromWindow();

};

#endif
