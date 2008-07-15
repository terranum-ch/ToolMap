/***************************************************************************
								tmgisdatavectorshp.h
                    class for dealing with vector SHP data
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


#ifndef _TM_GISDATAVECTOR_SHP_H_
#define _TM_GISDATAVECTOR_SHP_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "tmgisdatavector.h"
#include "ogrsf_frmts.h"


class tmGISDataVectorSHP : public tmGISDataVector
	{
	private:
		OGRDataSource       *m_Datasource;
		OGRLayer			*m_Layer;
	
	protected:
	public:
		tmGISDataVectorSHP();
		~tmGISDataVectorSHP();
		
		// implementing virtual function
		virtual bool Open (const wxString & filename);
		virtual tmRealRect GetMinimalBoundingRectangle();
		
	};



#endif
