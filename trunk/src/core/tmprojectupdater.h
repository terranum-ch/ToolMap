/***************************************************************************
 tmprojectupdater.h
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

#ifndef _TMPROJECTUPDATER_H
#define _TMPROJECTUPDATER_H

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include "../database/database_tm.h"


class tmProjectUpdater {
private:
    DataBaseTM * m_pDB;
	
public:
    tmProjectUpdater(DataBaseTM * database);
    virtual ~tmProjectUpdater();
	
    bool IsUpdateNeeded();
    bool DoUpdate();
	
};
#endif
