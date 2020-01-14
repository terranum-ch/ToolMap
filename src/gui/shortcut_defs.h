/***************************************************************************
 shortcut_defs.h
 Some definitions used in DatabaseTM and 
 in Shortcut_Panel_Dlg
 -------------------
 copyright            : (C) 2007 CREALP Lucien Schreiber 
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


#ifndef _TM_SHORTCUT_DEFS_H_
#define _TM_SHORTCUT_DEFS_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

// Include wxWidgets' headers
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif


/***************************************************************************//**
 @brief Key used for shortcuts
 @author Lucien Schreiber (c) CREALP 2008
 @date 10 December 2008
 *******************************************************************************/
const wxString tmShortCutKeys[] = {_T("F1"), _T("F2"), _T("F3"),
                                   _T("F4"), _T("F5"), _T("F6"), _T("F7"),
                                   _T("F8"), _T("F9"), _T("F10"), _T("F11"), _T("F12")};


#endif
