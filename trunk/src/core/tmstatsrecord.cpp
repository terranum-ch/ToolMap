/***************************************************************************
 tmstatsrecord.cpp
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

#include "tmstatsrecord.h"
#include "../database/database_tm.h"
#include "../database/databaseresult.h"
#include "tmstats.h"

tmStatsRecords::tmStatsRecords(DataBaseTM * database) {
	m_pDB = database;
	wxASSERT(m_pDB);
}



tmStatsRecords::~tmStatsRecords() {
}



long tmStatsRecords::Create(const tmStatsData & data) {
	wxASSERT(m_pDB);
	if (data.m_TimeStart.IsValid() == false) {
		wxLogError(_("Unable to create statistics record, start time isn't valid!"));
		return wxNOT_FOUND;
	}
	
	wxString myQuery = wxString::Format(_T("INSERT INTO %s (DATE_START) VALUE (\"%s %s\")"),
										TABLE_NAME_STAT,
										data.m_TimeStart.FormatISODate(),
										data.m_TimeStart.FormatISOTime());
	if (m_pDB->DataBaseQueryNoResults(myQuery) == false) {
		return wxNOT_FOUND;
	}
	return m_pDB->DataBaseGetLastInsertedID();
}



bool tmStatsRecords::Add(long recordid, const tmStatsData & data) {
	wxASSERT(m_pDB);
	
	wxString myQuery = wxString::Format(_T("UPDATE %s SET CLICK=CLICK + %ld, ATTRIBUTION=ATTRIBUTION + %ld,")
										_T("INTERSECTION = INTERSECTION+%ld WHERE STAT_ID=%ld"),
										TABLE_NAME_STAT,
										data.m_NbClick,
										data.m_NbAttribution,
										data.m_NbIntersection,
										recordid);
	if (m_pDB->DataBaseQueryNoResults(myQuery) == false) {
		return false;
	}
	return true;
}



bool tmStatsRecords::Load(long recordid, tmStatsData & data) {
	wxASSERT(m_pDB);
	wxString myQuery = wxString::Format(_T("SELECT DATE_START, CLICK, ATTRIBUTION, INTERSECTION,") 
										_T("TIMEDIFF(DATE_END,DATE_START) ")
										_T("FROM %s WHERE STAT_ID = %ld"),
										TABLE_NAME_STAT,
										recordid);
	if (m_pDB->DataBaseQuery(myQuery) == false) {
		return false;
	}
	
	DataBaseResult myResult;
	if(m_pDB->DataBaseGetResults(&myResult)==false){
		wxLogWarning(_("No statistics results returned for selected id (%ld)!"), recordid);
		return false;
	}
	data.Reset();
	wxString myDateValueTxt = wxEmptyString;
	wxString myIntervalTxt = wxEmptyString;
	long myClick = 0;
	long myAttrib = 0;
	long myIntersect = 0;
	
	myResult.NextRow();
	myResult.GetValue(0, myDateValueTxt);
	myResult.GetValue(1, myClick);
	myResult.GetValue(2, myAttrib);
	myResult.GetValue(3, myIntersect);
	myResult.GetValue(4, myIntervalTxt);
	
	data.m_Id = recordid;
	data.m_TimeStart.ParseISOCombined(myDateValueTxt, ' ');
	data.m_NbClick = myClick;
	data.m_NbIntersection = myIntersect;
	data.m_NbAttribution = myAttrib;
	
	wxDateTime myTempDTime;
	myTempDTime.ParseISOTime(myIntervalTxt);
	data.m_TimeElapsed = wxTimeSpan(myTempDTime.GetHour(),
									myTempDTime.GetMinute(),
									myTempDTime.GetSecond());
	return true;
}



bool tmStatsRecords::LoadTotal(tmStatsData & data) {
	wxASSERT(m_pDB);
	wxString myQuery = wxString::Format(_T("SELECT SUM(CLICK), SUM(ATTRIBUTION), SUM(INTERSECTION),")
										_T("SUM(HOUR(TIMEDIFF(DATE_END, DATE_START))), ")
										_T("SUM(MINUTE(TIMEDIFF(DATE_END, DATE_START))), ")
										_T("SUM(SECOND(TIMEDIFF(DATE_END, DATE_START))) FROM %s"),
										TABLE_NAME_STAT);
	data.Reset();
	if (m_pDB->DataBaseQuery(myQuery) == false) {
		return false;
	}
	
	DataBaseResult myResults;
	if(m_pDB->DataBaseGetResults(&myResults)==false){
		return false;
	}

	long myClick = wxNOT_FOUND;
	long myAttrib = wxNOT_FOUND;
	long myIntersect = wxNOT_FOUND;
	long myHour = wxNOT_FOUND;
	long myMin = wxNOT_FOUND;
	long mySec = wxNOT_FOUND;
	
	myResults.NextRow();
	myResults.GetValue(0, myClick);
	myResults.GetValue(1, myAttrib);
	myResults.GetValue(2, myIntersect);
	myResults.GetValue(3, myHour);
	myResults.GetValue(4, myMin);
	myResults.GetValue(5, mySec);

	data.m_NbClick = myClick;
	data.m_NbAttribution = myAttrib;
	data.m_NbIntersection = myIntersect;
	data.m_TimeElapsed = wxTimeSpan(myHour, myMin, mySec);
	return true;
}



bool tmStatsRecords::Delete(long recordid) {
	wxASSERT(m_pDB);
	wxString myQuery = wxString::Format(_T("DELETE FROM %s WHERE STAT_ID = %ld"),
									   TABLE_NAME_STAT,
									   recordid);
	if (m_pDB->DataBaseQueryNoResults(myQuery) == false) {
		return false;
	}
	return true;
}


bool tmStatsRecords::ExportAll(const wxFileName & filename) {
	return false;
}


long tmStatsRecords::GetCount() {
	wxASSERT(m_pDB);
	wxString myQuery = wxString::Format(_T("SELECT COUNT(*) FROM %s"), TABLE_NAME_STAT);
	if (m_pDB->DataBaseQuery(myQuery) == false) {
		return wxNOT_FOUND;
	}
	
	long myNbRecords = wxNOT_FOUND;
	if( m_pDB->DataBaseGetNextResult(myNbRecords)==false){
		myNbRecords = wxNOT_FOUND;
	}
	m_pDB->DataBaseClearResults();
	return myNbRecords;
}

