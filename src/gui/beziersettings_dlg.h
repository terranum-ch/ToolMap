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

class tmEditManager;
class tmRenderer;
class BezierSettings_DLG : public wxDialog
{
public:
    BezierSettings_DLG( wxWindow* parent, tmEditManager * editmanager, tmRenderer * renderer, wxWindowID id = wxID_ANY, const wxString& title = _("Bezier Settings"),
                       const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
                       long style = wxDEFAULT_DIALOG_STYLE );
    ~BezierSettings_DLG();
    
    virtual bool TransferDataToWindow();
    virtual bool TransferDataFromWindow();
    
    void SetBezierApproximationScale (double scale) {m_ApproximationValue = scale;}
    double GetBezierApproximationScale () {return m_ApproximationValue; }
    
private:
    void OnIdlePreview( wxIdleEvent& event );
    void OnCheckPreview( wxCommandEvent& event );
    void OnUpdateUIPreview( wxUpdateUIEvent& event ) ;
    
    void _CreateControls();
    
    wxSpinCtrlDouble* m_ApproximationCtrl;
    wxCheckBox* m_PreviewCtrl;
    
    double m_ApproximationValue;
    double m_ApproximationValuePrevious;
    tmEditManager * m_EditManager;
    tmRenderer * m_Renderer;
};



#endif
