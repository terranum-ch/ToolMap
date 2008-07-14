/***************************************************************************
								tmgisdatavector.h
                   Super class for dealing with vector GIS data
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


#ifndef _TM_GISDATAVECTOR_H_
#define _TM_GISDATAVECTOR_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "tmgisdata.h"



enum tmGISDATA_VECTOR_TYPES
{
	tmGIS_VECTOR_SHAPEFILE = 0,
	tmGIS_VECTOR_MYSQL
};

static wxString tmGISDATA_VECTOR_TYPE_WILDCARDS[] = 
{_("Esri's shapefiles (*.shp)|*.shp"),
_("MySQL geometric table (*.MYD)|*.MYD")};


class tmGISDataVector : public tmGISData
	{
	private:
	protected:
	public:
		// ctor, dtor
		tmGISDataVector();
		~tmGISDataVector();
		
		// static functions
		static wxString GetAllVectorGISFormatsWildcards();
		static tmGISDataVector * CreateGISVectorBasedOnType (const int & gis_format_index);
		
	};



#endif
