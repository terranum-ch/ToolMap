/***************************************************************************
 tmsymboldlgline.h
 Display Line Symbology dialog
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
#ifndef _TM_SYMBOLDLGLINE_H_
#define _TM_SYMBOLDLGLINE_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

// Include wxWidgets' headers
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/listctrl.h>
#include <wx/notebook.h>
#include <wx/spinctrl.h>
#include <wx/imaglist.h>
#include <wx/wupdlock.h>

#include "tmsymboldlg.h"        // tmSymbolDlg declaration
#include "tmsymbolrule.h"


const int ID_SYMDLGL_PANEL = 10216;
const int ID_SYMDLGL_NOTEBOOK = 10080;
const int ID_SYMDLGL_PANEL2 = 10081;
const int ID_SYMDLGL_COLOR = 10082;
const int ID_SYMDLGL_PATTERN = 10083;
const int ID_SYMDLGL_WIDTH = 10084;
const int ID_SYMDLGL_PANEL3 = 10085;
const int ID_SYMDLGL_PANEL4 = 10088;
const int ID_SYMDLGL_TRANSPARENCY = 10086;

const int ID_BTN_CLASSIFY_LINE = 10330;
const int ID_BTN_ADD_LINE = 10331;
const int ID_BTN_REMOVE_LINE = 10332;
const int ID_BTN_REMOVEALL_LINE = 10333;
const int ID_LIST_SYMBOL_LINE = 10334;

class tmLayerProperties;

class tmGISDataVectorSHP;

class DataListReportCtrl;

class tmSymbolDataLineUnique
{
public:
    int m_PanelNo;
    wxColour m_Colour;
    int m_Shape;
    int m_Width;
    int m_GlobalTransparency;

    tmSymbolDataLineUnique()
    {
        m_PanelNo = 0;
        m_Colour = *wxBLACK;
        m_Shape = 0;
        m_Width = 1;
        m_GlobalTransparency = 0;
    }
};


class tmSymbolDLGLine : public tmSymbolDLG
{
private:
    wxColourPickerCtrl *m_LineColourCtrl;
    wxSpinCtrl *m_LineWidthCtrl;
    wxChoice *m_LinePatternCtrl;
    tmSliderWithText *m_TransparencySlider;

    tmSymbolDataLineUnique m_DlgData;

    void _Init();

    void CreateControlsLine();

    virtual bool TransferDataToWindow();

    virtual bool TransferDataFromWindow();


DECLARE_DYNAMIC_CLASS(tmSymbolDLGLine)

DECLARE_EVENT_TABLE()

public:
    /// Constructors
    tmSymbolDLGLine();

    ~tmSymbolDLGLine();

    tmSymbolDLGLine(wxWindow *parent, wxWindowID id = SYMBOL_TMSYMBOLDLG_IDNAME,
                    const wxString &caption = SYMBOL_TMSYMBOLDLG_TITLE,
                    const wxPoint &pos = SYMBOL_TMSYMBOLDLG_POSITION,
                    const wxSize &size = SYMBOL_TMSYMBOLDLG_SIZE,
                    long style = SYMBOL_TMSYMBOLDLG_STYLE);

    bool Create(wxWindow *parent, wxWindowID id = SYMBOL_TMSYMBOLDLG_IDNAME,
                const wxString &caption = SYMBOL_TMSYMBOLDLG_TITLE,
                const wxPoint &pos = SYMBOL_TMSYMBOLDLG_POSITION,
                const wxSize &size = SYMBOL_TMSYMBOLDLG_SIZE,
                long style = SYMBOL_TMSYMBOLDLG_STYLE);


    void SetDialogData(const tmSymbolDataLineUnique &data)
    { m_DlgData = data; }

    tmSymbolDataLineUnique GetDialogData()
    { return m_DlgData; }


};


/*************************************************************************************//**
@brief Symbology dialog for lines supporting rules
@author Lucien Schreiber copyright CREALP
@date 02 septembre 2012
*****************************************************************************************/
class tmSymbolDLGLineRule : public tmSymbolDLG
{
private:
    wxNotebook *m_SymbologyTypeCtrl;
    wxChoice *m_CategoryColumnCtrl;
    DataListReportCtrl *m_SymbolListCtrl;
    wxButton *m_ClassifyBtn;
    wxButton *m_AddBtn;
    wxButton *m_RemoveBtn;
    wxButton *m_RemoveAllBtn;
    wxImageList *m_ImgList;

    // unique
    wxColourPickerCtrl *m_LineColourCtrl;
    wxSpinCtrl *m_LineWidthCtrl;
    wxChoice *m_LinePatternCtrl;
    tmSliderWithText *m_TransparencySlider;

    tmSymbolRuleArray m_Rules;
    tmLayerProperties *m_LayerProperties;

    tmGISDataVectorSHP *m_GISData;
    wxString m_SelectedField;
    tmSymbolDataLineUnique m_LineUniqueStyle;

    void _CreateControls();

    void _LoadTableData();

    wxBitmap _CreateColorBitmap(const wxBrush &brush, const wxPen &pen);

    void OnBtnClassify(wxCommandEvent &event);

    void OnBtnAdd(wxCommandEvent &event);

    void OnBtnRemove(wxCommandEvent &event);

    void OnBtnRemoveAll(wxCommandEvent &event);

    void OnDoubleClick(wxListEvent &event);

    void OnUpdateUIBtnRemove(wxUpdateUIEvent &event);

    void OnUpdateUIBtnRemoveAll(wxUpdateUIEvent &event);

    virtual bool TransferDataToWindow();

    virtual bool TransferDataFromWindow();

DECLARE_EVENT_TABLE();

public:
    tmSymbolDLGLineRule(wxWindow *parent, tmLayerProperties *layerproperties = NULL,
                        wxWindowID id = SYMBOL_TMSYMBOLDLG_IDNAME, const wxString &caption = SYMBOL_TMSYMBOLDLG_TITLE,
                        const wxPoint &pos = SYMBOL_TMSYMBOLDLG_POSITION, const wxSize &size = SYMBOL_TMSYMBOLDLG_SIZE,
                        long style = SYMBOL_TMSYMBOLDLG_STYLE);

    virtual ~tmSymbolDLGLineRule();

    bool Create(wxWindow *parent, wxWindowID id = SYMBOL_TMSYMBOLDLG_IDNAME,
                const wxString &caption = SYMBOL_TMSYMBOLDLG_TITLE, const wxPoint &pos = SYMBOL_TMSYMBOLDLG_POSITION,
                const wxSize &size = SYMBOL_TMSYMBOLDLG_SIZE, long style = SYMBOL_TMSYMBOLDLG_STYLE);

    inline tmSymbolRuleArray *GetRulesRef()
    { return &m_Rules; };

    int GetSelectedPanel();

    inline const wxString GetSelectedField() const;

    inline const tmSymbolDataLineUnique GetLineUniqueStyle() const;

    void SetLineUniqueStyle(tmSymbolDataLineUnique value);

    void SetSelectedField(wxString value);

    void SetSelectedPanel(int panelindex);
};


inline const wxString tmSymbolDLGLineRule::GetSelectedField() const
{
    return m_SelectedField;
}


inline const tmSymbolDataLineUnique tmSymbolDLGLineRule::GetLineUniqueStyle() const
{
    return m_LineUniqueStyle;
}


#endif
