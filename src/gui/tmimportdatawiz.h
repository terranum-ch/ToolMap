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


class ImportDataWizard : public tmWizardImport
{
public:
    ImportDataWizard(wxWindow *window, wxWindowID id);

    virtual ~ImportDataWizard();

    wxWizardPage *GetFirstPage() const
    {
        return m_pages.Item(0);
    }

};


#endif
