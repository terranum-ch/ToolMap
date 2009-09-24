/***************************************************************************
							tmpointorientattrib.h
                    Point orientation with attribution
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


#ifndef _TM_POINT_ORIENT_ATTRIB_H_
#define _TM_POINT_ORIENT_ATTRIB_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "tmpointorient.h"
#include "../database/database_tm.h"


class tmPointOrientAttrib : public tmPointOrient 
	{
	private:
		long m_Oid;
		long m_LayerId;
		DataBaseTM * m_DB;
		ProjectDefMemoryFields m_OrientField;
		
		void InitMemberValues();
		bool POA_IsOIDInited();
		bool POA_IsAttributed(long & attributedvalue);
		bool POA_HasOrientField();
		
	protected:
	public:
		tmPointOrientAttrib();
		void Create (DataBaseTM * database,  long oid);
		~tmPointOrientAttrib();
		
		virtual bool IsValid();
		bool Update ();
};



#endif
