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
class DataBaseTM;

class ImportDataWizard : public tmWizardImport
{
private:
    tmImport *m_Import;
    DataBaseTM *m_pDatabase;

    void ResetFilePicker(const wxWizardPageSimple *page);

    void SetXYColumnsOptions() const;

    void GetXYColumnsSelection() const;

    void SetTargetsOptions() const;

    void GetTargetSelection() const;

    void SetLayerOptions() const;

    void GetLayerSelection() const;

protected:
    int GetPageIndex(wxWizardPage* page);

    void OnWizardBeforePageChanged(wxWizardEvent &event);

    void OnWizardFinished(wxWizardEvent &event);

    void EnableNextButton(bool state = true);

    void EnableBackButton(bool state = true);

    inline void SetControlEnable(int id, bool state);

    void OnFileChanged(wxFileDirPickerEvent &event);

public:
    ImportDataWizard(wxWindow *window, wxWindowID id, DataBaseTM *database);

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
