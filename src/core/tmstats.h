/***************************************************************************
 tmstats.h
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

#ifndef _TMSTATS_H
#define _TMSTATS_H

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif



class tmStats_DLG : public wxDialog {
  private:
	// controls
	wxStaticBoxSizer* m_SessionSizerCtrl;
	wxStaticText* m_SessionClickCtrl;
	wxStaticText* m_SessionAttribCtrl;
	wxStaticText* m_SessionIntersectCtrl;
	wxStaticText* m_SessionElapsedTimeCtrl;
	wxStaticBoxSizer* m_TotalSizerCtrl;
	wxStaticText* m_TotalClickCtrl;
	wxStaticText* m_TotalAttribCtrl;
	wxStaticText* m_TotalIntersectCtrl;
	wxStaticText* m_TotalElapsedTimeCtrl;
	wxButton* m_BtnStartCtrl;
	wxButton* m_BtnStopCtrl;
	wxButton* m_BtnExportCtrl;
	wxButton* m_BtnCloseCtrl;
	
    void OnRecordStart(wxCommandEvent & event);
    void OnRecordStop(wxCommandEvent & event);
    void OnExport(wxCommandEvent & event);

    void _CreateControls();

  public:
    tmStats_DLG(wxWindow * parent, wxWindowID id, const wxString & title, 
				const wxPoint & pos = wxDefaultPosition, 
				const wxSize & size = wxDefaultSize, 
				long style = wxDEFAULT_DIALOG_STYLE);
    virtual ~tmStats_DLG();

    virtual bool TransferDataToWindow();
    virtual bool TransferDataFromWindow();
};
#endif
