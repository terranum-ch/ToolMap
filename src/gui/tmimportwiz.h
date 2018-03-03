/***************************************************************************
 tmimportwiz.h
 
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

#ifndef _TMIMPORTWIZ_H
#define _TMIMPORTWIZ_H

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/wizard.h>


class tmImport;

class ImportWizIntro;

class ImportWizInfo;

class ImportWizCSVOptions;

class ImportWizTarget;


class ImportWizard : public wxWizard
{
private:
    tmImport *m_Import;
    ImportWizIntro *m_PageIntro;
    ImportWizInfo *m_PageInfo;


public:
    ImportWizard(wxWindow *window, wxWindowID id);

    virtual ~ImportWizard();

    int ShowWizard();

    inline tmImport *GetImport() const;

    void SetImport(tmImport *value);

    void SetEnableControl(wxWindowID id, bool status);

};


inline tmImport *ImportWizard::GetImport() const
{
    return m_Import;
}

#endif
