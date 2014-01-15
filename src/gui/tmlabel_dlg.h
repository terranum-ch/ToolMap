/***************************************************************************
 tmlabel_dlg
 -------------------
 copyright            : (C) 2014 CREALP Lucien Schreiber
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

#ifndef _TM_LABEL_DLG_H_
#define _TM_LABEL_DLG_H_

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <wx/html/htmlwin.h>

class tmLayerProperties;
class tmGISDataVectorSHP;

class tmLabelDLG : public wxDialog{
private:
    wxCheckBox* m_CheckActiveLabelCtrl;
    wxChoice* m_FieldCtrl;
    wxButton* m_AddFieldBtn;
    wxTextCtrl* m_TextCtrl;
    wxButton* m_AddTextBtn;
    wxButton* m_ClearBtn;
    wxHtmlWindow* m_PreviewCtrl;
    
    tmLayerProperties * m_LayerProperties;
    tmGISDataVectorSHP * m_GisData;
    
    void _CreateControls();
	
protected:
     void OnAddField( wxCommandEvent& event ) { event.Skip(); }
     void OnAddText( wxCommandEvent& event ) { event.Skip(); }
     void OnClear( wxCommandEvent& event ) { event.Skip(); }
    
	
public:
    tmLabelDLG( wxWindow* parent, tmLayerProperties * properties, const wxPoint& pos = wxDefaultPosition, wxWindowID id = wxID_ANY, const wxString& title = _("Labels"), const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER );
    ~tmLabelDLG();
};




#endif

