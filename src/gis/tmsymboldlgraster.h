/***************************************************************************
								tmsymboldlgraster.h
							Display raster Symbology dialog
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


#ifndef _TM_SYMBOLDLGRASTER_H_
#define _TM_SYMBOLDLGRASTER_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif


#include "wx/notebook.h"
#include "wx/spinctrl.h"
#include "tmsymboldlg.h"        // tmSymbolDlg declaration


const int ID_SYMDLGRASTER_PANEL = 10216;
const int ID_SYMDLGRASTER_USE_TRANSPARENCY_COLOR = 10082;
const int ID_SYMDLGRASTER_TRANSPARENCY_COLOR = 10087;
const int ID_SYMDLGRASTER_TRANSPARENCY = 10086;


struct tmSymbolDataRaster
{
    int m_GlobalTransparency;
    bool m_DoMultiplyRaster;
};


class tmSymbolDLGRaster : public tmSymbolDLG
{
private:
    tmSliderWithText *m_TransparencySlider;
    wxCheckBox * m_MultiplyCtrl;

    tmSymbolDataRaster m_DlgData;

    void _Init();

    void CreateControls();

    virtual bool TransferDataToWindow();

    virtual bool TransferDataFromWindow();

DECLARE_DYNAMIC_CLASS(tmSymbolDLGRaster)

DECLARE_EVENT_TABLE()

public:
    /// Constructors
    tmSymbolDLGRaster();

    ~tmSymbolDLGRaster();

    tmSymbolDLGRaster(wxWindow *parent, wxWindowID id = SYMBOL_TMSYMBOLDLG_IDNAME,
                      const wxString &caption = SYMBOL_TMSYMBOLDLG_TITLE,
                      const wxPoint &pos = SYMBOL_TMSYMBOLDLG_POSITION,
                      const wxSize &size = SYMBOL_TMSYMBOLDLG_SIZE,
                      long style = SYMBOL_TMSYMBOLDLG_STYLE);

    bool Create(wxWindow *parent, wxWindowID id = SYMBOL_TMSYMBOLDLG_IDNAME,
                const wxString &caption = SYMBOL_TMSYMBOLDLG_TITLE,
                const wxPoint &pos = SYMBOL_TMSYMBOLDLG_POSITION,
                const wxSize &size = SYMBOL_TMSYMBOLDLG_SIZE,
                long style = SYMBOL_TMSYMBOLDLG_STYLE);


    void SetDialogData(const tmSymbolDataRaster &data)
    { m_DlgData = data; }

    tmSymbolDataRaster GetDialogData()
    { return m_DlgData; }


};


#endif
