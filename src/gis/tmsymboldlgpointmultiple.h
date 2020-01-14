/***************************************************************************
 tmSymbolDLGPointMultiplemultiple.h
 Display Multiple Point Symbology dialog
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



#ifndef _TM_SYMBOLDLGPOINT_MULTIPLE_H_
#define _TM_SYMBOLDLGPOINT_MULTIPLE_H_

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/notebook.h>
#include <wx/spinctrl.h>

#include "tmsymboldlgpoint.h"  // tmSymbolDlg declaration

class tmSymbolDataPointMultiple : public tmSymbolDataPointUnique
{
public:
    int m_PanelNo;

    // multiple symbology
    long m_QueryID;
    wxColour m_SelColourMultiple;
    int m_SelRadiusMultiple;
    bool m_SelVisible;

    wxColour m_UnSelColourMultiple;
    int m_UnSelRadiusMultiple;
    int m_UnSelVisible;

    tmSymbolDataPointMultiple()
    {
        m_PanelNo = 0;
        m_QueryID = wxNOT_FOUND;
        m_SelColourMultiple = *wxBLUE;
        m_SelRadiusMultiple = 5;
        m_SelVisible = true;
        m_UnSelColourMultiple = *wxGREEN;
        m_UnSelRadiusMultiple = 5;
        m_UnSelVisible = true;
    }
};


class tmSymbolDLGPointMultiple : public tmSymbolDLG
{
private:
    wxColourPickerCtrl *m_PointColourCtrl;
    wxSpinCtrl *m_PointWidthCtrl;
    tmSliderWithText *m_TransparencySlider;

    wxNotebook *m_SymbologyTypeCtrl;
    wxChoice *m_MQueryCtrl;
    wxColourPickerCtrl *m_MSelColourCtrl;
    wxSpinCtrl *m_MSelWidthCtrl;
    wxCheckBox *m_MSelVisibleCtrl;
    wxColourPickerCtrl *m_MUnSelColourCtrl;
    wxSpinCtrl *m_MUnSelWidthCtrl;
    wxCheckBox *m_MUnSelVisibleCtrl;

    tmSymbolDataPointMultiple m_DlgData;
    TOC_GENERIC_NAME m_TocType;

    void _Init();

    void CreateControlsPoint();

    virtual bool TransferDataToWindow();

    virtual bool TransferDataFromWindow();

DECLARE_DYNAMIC_CLASS(tmSymbolDLGPointMultiple)

public:
    /// Constructors
    tmSymbolDLGPointMultiple();

    ~tmSymbolDLGPointMultiple();

    tmSymbolDLGPointMultiple(wxWindow *parent, wxWindowID id = SYMBOL_TMSYMBOLDLG_IDNAME,
                             const wxString &caption = SYMBOL_TMSYMBOLDLG_TITLE,
                             const wxPoint &pos = SYMBOL_TMSYMBOLDLG_POSITION,
                             const wxSize &size = SYMBOL_TMSYMBOLDLG_SIZE,
                             long style = SYMBOL_TMSYMBOLDLG_STYLE);

    bool Create(wxWindow *parent, wxWindowID id = SYMBOL_TMSYMBOLDLG_IDNAME,
                const wxString &caption = SYMBOL_TMSYMBOLDLG_TITLE,
                const wxPoint &pos = SYMBOL_TMSYMBOLDLG_POSITION,
                const wxSize &size = SYMBOL_TMSYMBOLDLG_SIZE,
                long style = SYMBOL_TMSYMBOLDLG_STYLE);


    void SetDialogData(const tmSymbolDataPointMultiple &data)
    { m_DlgData = data; }

    tmSymbolDataPointMultiple GetDialogData()
    { return m_DlgData; }

    void SetTocType(TOC_GENERIC_NAME toctype)
    { m_TocType = toctype; }


};


#endif
