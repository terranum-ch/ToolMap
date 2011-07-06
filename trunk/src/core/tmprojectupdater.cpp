/***************************************************************************
 tmprojectupdater.cpp
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

#include "tmprojectupdater.h"

tmProjectUpdater::tmProjectUpdater(DataBaseTM * database) {
	database = m_pDB;
	wxASSERT(m_pDB);
}



tmProjectUpdater::~tmProjectUpdater() {
	
}



bool tmProjectUpdater::IsUpdateNeeded() {
	return false;
}



bool tmProjectUpdater::DoUpdate() {
	return false;
}

