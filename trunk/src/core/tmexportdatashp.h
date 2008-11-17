/***************************************************************************
								tmexportdatashp.h
                    Class for the process of exporting data as SHP
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


#ifndef _TM_EXPORTDATA_SHP_H_
#define _TM_EXPORTDATA_SHP_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif


#include "tmexportdata.h"		// for parent class definition
#include "../gis/tmgisdatavectorshp.h"	// for shp


class tmExportDataSHP : public tmExportData
	{
	private:
		tmGISDataVectorSHP  m_Shp;
		OGRPolygon * m_Frame;
		// private function
		void InitMemberValues();
		
	protected:
	public:
		// ctor
		tmExportDataSHP();
		tmExportDataSHP (DataBaseTM * database);
		void Create (DataBaseTM * database);
		~tmExportDataSHP();

		
		// create export file
		virtual bool CreateEmptyExportFile (ProjectDefMemoryLayers * myLayer, 
											const wxString & path);
		
		virtual bool AddOptFields (PrjMemFieldArray * myfields);
		virtual bool AddGenericFields (int iSize);
		
		// frame function
		virtual void SetFrame (wxRealPoint * points, int nbvertex);

		
		// writing functions
		virtual bool WriteLines (ProjectDefMemoryLayers * myLayer);
		virtual bool WritePoints (ProjectDefMemoryLayers * myLayer);
		virtual bool WritePolygons (ProjectDefMemoryLayers * myLayer);
		
		// adding field data
		virtual bool AddSimpleDataToLine (ProjectDefMemoryLayers * myLayer);
		
	};




#endif

