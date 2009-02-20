/***************************************************************************
								tmarraysize.h
							Array of wxSize items
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


#ifndef _TM_ARRAY_SIZE_H_
#define _TM_ARRAY_SIZE_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif


/*class tmSize
	{
	private:
		void InitMemberValues();
		
		
	public:
		tmSize();
		~tmSize();
		
		wxSize m_Size;
	};*/
WX_DECLARE_OBJARRAY(wxSize, tmArraySize);		



#endif
