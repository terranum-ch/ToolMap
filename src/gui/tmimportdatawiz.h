/***************************************************************************
 tmimportdatawiz.h

 -------------------
 copyright            : (C) 2018 TERRANUM Pascal Horton
 email                : pascal.horton at terranum dot ch
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef _TMIMPORTDATAWIZ_H
#define _TMIMPORTDATAWIZ_H

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "wizard_import_data.h"


class tmImport;
class ProjectManager;

class ImportDataWizard : public tmWizardImport
{
private:
    tmImport *m_Import;
    ProjectManager *m_PrjManager;
    wxString m_IgnoreLabel;
    wxString m_ImportButIgnoreValueLabel;
    wxString m_DoNotImportLabel;
    wxString m_AllObjectsLabel;

    void ResetFilePicker(const wxWizardPageSimple *page);

    void SetXYFieldsOptions() const;

    void GetXYFieldsSelection() const;

    void SetTargetsOptions() const;

    void GetTargetSelection() const;

    void SetLayerOptions() const;

    void GetLayerSelection() const;

    void SetKindFieldOptions() const;

    void GetKindFieldSelection() const;

    void SetKindOptions() const;

    void GetKindSelection() const;

    void SetAttributeOptions() const;

    void GetAttributeSelection() const;

    void SetEnumerationOptions() const;

    void GetEnumerationSelection() const;

protected:
    int GetPageIndex(wxWizardPage* page);

    void OnWizardBeforePageChanged(wxWizardEvent &event);

    void OnWizardFinished(wxWizardEvent &event);

    void EnableNextButton(bool state = true);

    void EnableBackButton(bool state = true);

    inline void SetControlEnable(int id, bool state);

    void OnFileChanged(wxFileDirPickerEvent &event);

    virtual void ToggleKindFieldSelection(wxCommandEvent &event);

public:
    ImportDataWizard(wxWindow *window, wxWindowID id, ProjectManager *prjManager);

    virtual ~ImportDataWizard();

    wxWizardPage *GetFirstPage() const
    {
        return m_pages.Item(0);
    }

    tmImport *GetImport() const
    {
        return m_Import;
    }
};


#endif
