/***************************************************************************
 beziersettings_dlg.h
 -------------------
 copyright            : (C) 2013 CREALP Lucien Schreiber
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
#ifndef _BEZIER_SETTINGS_H_
#define _BEZIER_SETTINGS_H_

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/spinctrl.h>
#include <wx/notebook.h>

class tmEditManager;

class tmRenderer;


class BezierSettingsData
{
public:
    enum
    {
        AGG = 0, ETHZ = 1
    } method;
    double agg_approximation;
    double ethz_width;
    int ethz_max_points;

    BezierSettingsData()
    {
        method = AGG;
        agg_approximation = 0.2;
        ethz_width = 1;
        ethz_max_points = 10;
    }

    inline bool operator==(const BezierSettingsData &b) const
    {
        if (method != b.method) {
            return false;
        }
        if (agg_approximation != b.agg_approximation) {
            return false;
        }
        if (ethz_width != b.ethz_width) {
            return false;
        }
        if (ethz_max_points != b.ethz_max_points) {
            return false;
        }
        return true;
    }

};


class BezierSettings_DLG : public wxDialog
{
public:
    BezierSettings_DLG(wxWindow *parent, tmEditManager *editmanager, tmRenderer *renderer, wxWindowID id = wxID_ANY,
                       const wxString &title = _("Bezier Settings"),
                       const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize,
                       long style = wxDEFAULT_DIALOG_STYLE);

    ~BezierSettings_DLG();

    virtual bool TransferDataToWindow();

    virtual bool TransferDataFromWindow();

    void SetBezierSettings(BezierSettingsData data)
    { m_Data = data; }

    BezierSettingsData GetBezierSettings()
    { return m_Data; }

private:
    void OnIdlePreview(wxIdleEvent &event);

    void OnCheckPreview(wxCommandEvent &event);

    void OnUpdateUIPreview(wxUpdateUIEvent &event);

    void _CreateControls();

    void _GetDataFromControl(BezierSettingsData *data);

    wxNotebook *m_NotebookMethodCtrl;
    wxSpinCtrlDouble *m_ApproximationCtrl;
    wxSpinCtrlDouble *m_WidthToleranceCtrl;
    wxSpinCtrl *m_MaxNumPointsCtrl;
    wxCheckBox *m_PreviewCtrl;

    //double m_ApproximationValue;
    //double m_ApproximationValuePrevious;
    tmEditManager *m_EditManager;
    tmRenderer *m_Renderer;
    BezierSettingsData m_Data;
    BezierSettingsData m_PreviousData;
};


#endif
