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

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif


class DataBaseTM;


/***************************************************************************//**
@brief Statistics data class
@author Lucien Schreiber (c) CREALP
@date 15 juillet 2011
*******************************************************************************/
class tmStatsData {
public:
    long m_Id;
    wxDateTime m_TimeStart;
    long m_NbClick;
    long m_NbAttribution;
    long m_NbIntersection;
    wxTimeSpan m_TimeElapsed;
	
	
	tmStatsData();
    virtual ~tmStatsData();
	
    void Reset();
	void ResetPartial();
    bool IsOk() const;
};






/***************************************************************************//**
@brief Statistics manager, link event to data and database records
@author Lucien Schreiber (c) CREALP
@date 15 juillet 2011
*******************************************************************************/
class tmStatsManager{
private:
    tmStatsData m_StatBufferData;
    DataBaseTM * m_Database;
    bool m_IsStarted;
    static const int m_BufferSize = 50;
	
    
    void _FlushBuffer();
    void _StartRecord();
    void _StopRecord();
	
    //void OnGetStatsMessage(wxCommandEvent & event);
	
public:
    tmStatsManager();
    virtual ~tmStatsManager();
    void Create(DataBaseTM * database);
	bool IsReady();
	
	void AppendToBuffer(long click, long attrib, long intersection);
    void ShowStatsDialog(wxWindow * parent);
};






/***************************************************************************//**
@brief Statistics dialog
@author Lucien Schreiber (c) CREALP
@date 15 juillet 2011
*******************************************************************************/
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
	
	const tmStatsData * m_DataActual;
    const tmStatsData * m_DataTotal;
	long m_DataTotalRecord;
	
	
    void OnRecordStart(wxCommandEvent & event);
    void OnRecordStop(wxCommandEvent & event);
    void OnExport(wxCommandEvent & event);

    void _CreateControls();
	void _UpdateControls();
	DECLARE_EVENT_TABLE();
  public:
    tmStats_DLG(wxWindow * parent, 
				const tmStatsData * actual,
				const tmStatsData * total,
				long nbrecords,
				wxWindowID id = wxID_ANY,
				const wxString & title = _("Statistics"), 
				const wxPoint & pos = wxDefaultPosition, 
				const wxSize & size = wxDefaultSize, 
				long style = wxDEFAULT_DIALOG_STYLE);
    virtual ~tmStats_DLG();

	void SetStarted(bool recordstarted);
 	
};
#endif
