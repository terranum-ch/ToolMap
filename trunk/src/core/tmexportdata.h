/***************************************************************************
								tmexportdata.h
                    main class for the process of exporting data
                             -------------------
    copyright            : (C) 2007 CREALP Lucien Schreiber 
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


#ifndef _TM_EXPORTDATA_H_
#define _TM_EXPORTDATA_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "../database/database_tm.h"	// for database access
#include "projectdefmemory.h"			// for ProjectDefMemoryLayers definition


/***************************************************************************//**
 @brief Parent class for exporting data
 @details This class is used by the #tmExportManager to abstract the access to
 the data and to allow us to implement new export format
 @author Lucien Schreiber (c) CREALP 2008
 @date 14 November 2008
 *******************************************************************************/
class tmExportData : public wxObject
	{
	private:
		void InitMemberValues();
		
	protected:
		DataBaseTM * m_pDB;
		wxString m_Extension;
		
		// protected functions
		wxFileName * GetFileName (ProjectDefMemoryLayers * myLayer,  const wxString & path);
		int GetSizeOfEnum (PrjMemFieldCodedValArray * mCodedVal);
		
		
	public:
		// ctor
		tmExportData();
		tmExportData (DataBaseTM * database);
		void Create (DataBaseTM * database);
		~tmExportData();
		
		// informations
		int GetSizeOfObjDesc (int layerindex);
		
		
		// create export file
		virtual bool CreateEmptyExportFile (ProjectDefMemoryLayers * myLayer, 
											const wxString & path){return false;}
		
		virtual bool AddOptFields (PrjMemFieldArray * myfields){return false;}
		virtual bool AddGenericFields (int iSize){return false;}
		
	
	};






#endif
