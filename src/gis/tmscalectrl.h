/***************************************************************************
 tmscalectrl.h
 Contain scale control component
 -------------------
 copyright            : (C) 2007 CREALP Lucien Schreiber
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef _TM_SCALECTRL_H_
#define _TM_SCALECTRL_H_

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "../database/database_tm.h"    // for loading data from db
#include "../core/scale.h"                // for ScaleTM

// EVENT FOR SCALE CTRL
DECLARE_EVENT_TYPE(tmEVT_SCALE_USER_CHANGED, -1)

class tmScaleCtrlCombo : public wxComboBox
{
private:
    wxWindow *m_ParentEvent;
DECLARE_EVENT_TABLE();
protected:
public:
    tmScaleCtrlCombo();

    tmScaleCtrlCombo(wxWindow *parent, wxWindowID id,
                     const wxPoint &pos,
                     const wxSize &size,
                     const wxArrayString &arraystring);

    ~tmScaleCtrlCombo()
    { ; }

    void InitScaleFromDatabase(const wxArrayLong &scale_values);

    void SetValueScale(const long &scale);

    void OnUserSetValue(wxCommandEvent &event);

};

#endif
