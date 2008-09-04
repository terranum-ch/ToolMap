/***************************************************************************
								tmserialize.h
                    for serializating object in a simple way
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


#ifndef _TM_SERIALIZE_H_
#define _TM_SERIALIZE_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <wx/sstream.h>

const int TMSERIALIZE_VERSION = 1; // increment each time something change


class tmSerialize
	{
	private:
		bool m_writeMode;
		wxOutputStream &m_odstr;
		wxInputStream &m_idstr;
		
		// temp vars for init
		wxString m_tmpostr;
		wxString m_tmpistr;
		wxStringOutputStream m_otmp;
		wxStringInputStream m_itmp;
		
		void SaveUint16(wxUint16 value);
		
		
	protected:
	public:
		tmSerialize(wxInputStream &stream);
		tmSerialize(wxOutputStream &stream);
		
		bool CanStore();
		
		
		
		virtual tmSerialize &operator << (bool value);
		virtual tmSerialize &operator << (wxString value);

	};





#endif
