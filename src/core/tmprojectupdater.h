/***************************************************************************
 tmprojectupdater.h
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

#ifndef _TMPROJECTUPDATER_H
#define _TMPROJECTUPDATER_H

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "../database/database_tm.h"

enum tmPRJ_UPD_ERROR {
  tmPRJ_UPD_ERROR_OK = 1,   // No update error
  tmPRJ_UPD_ERROR_PROJECT,  // Error during updating project
  tmPRJ_UPD_ERROR_TM        // Error ToolMap is too old

};

class tmProjectUpdater {
 private:
  DataBaseTM *m_pDB;

  bool _SetVersion(int versionnumber);

  bool _220to221();

  bool _221to222();

  bool _222to223();

  bool _223to224();

  bool _224to225();

  bool _225to226();

  bool _226to227();

  bool _227to228();

  bool _228to229();

  bool _229to230();

  bool _230to231();

 public:
  tmProjectUpdater(DataBaseTM *database);

  virtual ~tmProjectUpdater();

  bool IsCorrectVersion();

  tmPRJ_UPD_ERROR DoUpdate();
};

#endif
