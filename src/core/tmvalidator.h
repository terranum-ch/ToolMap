/***************************************************************************
								tmvalidator.h
							Create various validator
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



#ifndef _TMVALIDATOR_H_
#define _TMVALIDATOR_H_



// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#define tmFILTER_EXCLUDE_CHAR_RESTRICTED 0x0100


class tmValidator : public wxTextValidator
	{
	private:
		void SetCharRestricted();
		
	public:
		tmValidator(long style = wxFILTER_NONE);
	};






#endif
