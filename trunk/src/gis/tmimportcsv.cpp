/***************************************************************************
 tmimportcsv.cpp
 
 -------------------
 copyright            : (C) 2010 CREALP Lucien Schreiber 
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


#include "tmimportcsv.h"
#include "../database/database_tm.h"
#include "tmgisdatavector.h"

tmImportCSV::tmImportCSV() {
	m_Xcolumn = wxNOT_FOUND;
	m_Ycolumn = wxNOT_FOUND;
}

tmImportCSV::~tmImportCSV() {
}

bool tmImportCSV::Open(const wxFileName & filename) {
	if(tmImport::Open(filename)==false){
		return false;
	}
	
	return true;
}

wxArrayString tmImportCSV::ListColumns() {
}



void tmImportCSV::SetColumn(int x, int y) {
	m_Xcolumn = x;
	m_Ycolumn = y;
}



bool tmImportCSV::Import(DataBaseTM * database, wxProgressDialog * progress) {
	return false;
}

