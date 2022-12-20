/***************************************************************************
 tmsymbolvectorlinemultiple.h
 -------------------
 copyright            : (C) 2011 CREALP Lucien Schreiber
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef _TM_SYMBOLVECTORLINE_MULTIPLE_H_
#define _TM_SYMBOLVECTORLINE_MULTIPLE_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

// Include wxWidgets' headers
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "tmsymboldlgline.h"
#include "tmsymboldlglinemultiple.h"
#include "tmsymbolvectorline.h"  // tmSymbol class definition

class tmSymbolVectorLineMultiple : public tmSymbolVectorLine {
  private:
    tmSymbolDataLineMultiple m_lSymMultiple;

    virtual tmSymbolDLG* GetSymbolDialog(wxWindow* parent, const wxPoint& dlgpos);

    virtual bool GetDialogData(tmSymbolDLG* dlg);

  public:
    tmSymbolVectorLineMultiple();

    tmSymbolVectorLineMultiple(const tmSymbolVectorLineMultiple& origin);

    ~tmSymbolVectorLineMultiple();

    virtual bool Serialize(tmSerialize& s);

    virtual wxColour GetColour();

    virtual int GetWidth() {
        return m_lSymMultiple.m_Width;
    }

    virtual int GetShape() {
        return tmSYMBOLPENSYLES[m_lSymMultiple.m_Shape];
    }

    virtual int GetTransparency() {
        return m_lSymMultiple.m_GlobalTransparency;
    }

    int GetSelectedSymbology() {
        return m_lSymMultiple.m_PanelNo;
    }

    tmSymbolDataLineMultiple* GetSymbology() {
        return &m_lSymMultiple;
    }
};

#endif
