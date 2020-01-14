/***************************************************************************
 tmstatsrecord.h
 -------------------
 copyright            : (C) 2011 CREALP Lucien Schreiber
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef _TMSTATSRECORD_H
#define _TMSTATSRECORD_H

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/filename.h>

class DataBaseTM;

class tmStatsData;

class tmStatsRecords {
 private:
  DataBaseTM *m_pDB;

 public:
  tmStatsRecords(DataBaseTM *database);

  virtual ~tmStatsRecords();

  long Create(const tmStatsData &data);

  bool Add(long recordid, const tmStatsData &data);

  bool Load(long recordid, tmStatsData &data);

  bool LoadTotal(tmStatsData &data);

  bool Delete(long recordid);

  bool ExportAll(const wxFileName &filename);

  long GetCount();
};

#endif
