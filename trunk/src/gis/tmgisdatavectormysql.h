/***************************************************************************
								tmgisdatavectormysql.h
                    For dealing with spatial data from a mysql database
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


#ifndef _TM_GISDATAVECTOR_MYSQL_H_
#define _TM_GISDATAVECTOR_MYSQL_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif


#include "tmgisdatavector.h"
#include "../database/database_tm.h"



const wxString tmGISMYSQL_FIELD1 = _T("OBJECT_ID");
const wxString tmGISMYSQL_FIELD2 = _T("OBJECT_GEOMETRY");


class tmGISDataVectorMYSQL : public tmGISDataVector
	{
	private:
		static DataBaseTM * m_DB;
		

	protected:
		// checking db fields
		bool CheckGeometryFields(const wxString & tablename);
		
		// gis database function
		OGRGeometry * CreateDataBaseGeometry(MYSQL_ROW & row,
											 unsigned long * length,
											 int geometry_col=0);
		virtual wxString GetTableName (int type);
		
		
	public:
		tmGISDataVectorMYSQL();
		~tmGISDataVectorMYSQL();
		
		// special function for DB, use it first.
		static void SetDataBaseHandle(DataBaseTM * db) {m_DB = db;}
		
		// implementing virtual function
		virtual bool Open (const wxString & filename, bool bReadWrite = FALSE);
		virtual tmRealRect GetMinimalBoundingRectangle();
		
		virtual bool SetSpatialFilter (tmRealRect filter, int type);
		virtual wxRealPoint * GetNextDataLine (int & nbvertex);
		
		
	};



#endif
