/***************************************************************************
							tmgisdatavectormemory.h
                    For keeping line in memory during drawing
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


#ifndef _TM_GISDATA_VECTOR_MEMORY_H_
#define _TM_GISDATA_VECTOR_MEMORY_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif


#include "tmgisdatavector.h"
#include "ogrsf_frmts.h"
#include "../database/database_tm.h"	// for database support

class tmGISDataVectorMemory : public tmGISDataVector
	{
	private:
		OGRDataSource       *m_Datasource;
		OGRLayer			*m_Layer;
		OGRFeature			*m_Feature;
		
	
		
	protected:
		
		// load and save any geometry into database
		long SaveDatabaseGeometry (OGRGeometry * myGeom,
								   int ilayertype,
								   DataBaseTM * database);
		OGRGeometry * LoadDatabaseGeometry (long oid,
											int ilayertype,
											DataBaseTM * database);
		
		
	public:
		tmGISDataVectorMemory();
		~tmGISDataVectorMemory();
		
		// create & destroy feature (destroying automatically)
		bool CreateFeature();
		bool DestroyFeature();
		
		// vertex operations
		bool InsertVertex (const wxRealPoint & pt, int index = -1);
		bool RemoveVertex (int index = -1);
		int GetVertexCount();
		bool GetVertex (wxRealPoint & pt, int index = -1);
		bool GetVertexAll (wxArrayRealPoints & myPts);
		
		// saving /loading point
		long SavePointToDatabase (DataBaseTM * database, int layertype); 
		wxRealPoint GetPointFromDatabase (DataBaseTM * database, long oid,
										  int layertype);
		
		// saving / loading line
		long SaveLineToDatabase (DataBaseTM * database,	int layertype);
		bool GetLineFromDatabase (DataBaseTM * database, long oid, 
								  int layertype);

	
		
	};





#endif
