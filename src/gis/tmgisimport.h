/***************************************************************************
								tmgisimport.h
						Import data into project
                             -------------------
    copyright            : (C) 2009 CREALP Lucien Schreiber 
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

// comment doxygen


#ifndef _TM_GISDATA_IMPORT_H_ 
#define _TM_GISDATA_IMPORT_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "tmgisdatavector.h"
#include "../database/database_tm.h"

class tmGISImport : public wxObject
	{
	private:
		tmGISDataVector * m_Vector;
		
		void InitMember();
		
		
	protected:
	public:
		tmGISImport();
		~tmGISImport();
		
		bool Open (const wxString & filename);
		bool IsOpen ();
		
		TM_GIS_SPATIAL_TYPES GetSpatialType ();
		long GetFeatureCount();
		bool IsImportAllowed();
		bool IsImportIntoAllowed (const TOC_GENERIC_NAME & importintotype);	
		
		bool Import(DataBaseTM * projectdb, const TOC_GENERIC_NAME & importintotype);
	};




#endif
