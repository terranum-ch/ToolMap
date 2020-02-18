/***************************************************************************
 databaseresult.h
 -------------------
 copyright            : (C) 2010 CREALP Lucien Schreiber
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef _DATABASERESULT_H
#define _DATABASERESULT_H

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>
// Include wxWidgets' headers
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "database.h"
#include "mysql.h"
#include "ogrsf_frmts.h"  // OGR accessing

class DataBaseResult {
 private:
  MYSQL_RES **m_ResultSet;
  MYSQL_ROW m_Row;
  long m_RowIndex;
  tmArrayULong m_RowLengths;

 protected:
  bool _GetRowLength();

 public:
  DataBaseResult();

  DataBaseResult(MYSQL_RES **results);

  void Create(MYSQL_RES **results);

  ~DataBaseResult();

  bool HasResults();

  int GetColCount();

  long GetRowCount();

  bool GetColName(wxArrayString &fields);

  bool GetValue(int col, wxString &value);

  bool GetValue(int col, long &value);

  bool GetValue(int col, OGRGeometry **geometry);

  bool NextRow();

  bool IsRowOk();

  bool GotoRow(long row);
};

#endif
