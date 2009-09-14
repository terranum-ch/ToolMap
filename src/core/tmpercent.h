/***************************************************************************
								tmpercent.h
                    For computing percent value (progress dialog)
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


#ifndef _TM_PERCENT_H_
#define _TM_PERCENT_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

class tmPercent : public wxObject
	{
	private:
		bool m_inited;
		long m_TotalValue;
		double m_Increment;
		int m_PreviousPercent;
		long m_ActualValue;
		
		void InitMemberValue();
		
	protected:
	public:
		// ctor - dtor
		tmPercent();
		tmPercent(long value);
		void Create(long value);
		~tmPercent();
		
		void SetValue (long actualvalue);
		bool IsNewStep();
		int GetPercent();
		
};





#endif
