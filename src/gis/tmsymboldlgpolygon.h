/***************************************************************************
 tmsymboldlgpolygon.h
 Display Polygon Symbology dialog
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


#ifndef _TM_SYMBOLDLGPOLYGON_H_
#define _TM_SYMBOLDLGPOLYGON_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <wx/listctrl.h>
#include <wx/notebook.h>
#include <wx/spinctrl.h>
#include <wx/imaglist.h>
#include <wx/wupdlock.h>

#include "tmsymboldlg.h"		// tmSymbolDlg declaration
#include "tmsymbolrule.h"


const int ID_SYMDLGPLG_PANEL = 10216;
const int ID_SYMDLGPLG_NOTEBOOK  = 10080;
const int ID_SYMDLGPLG_PANEL2 = 10081;
const int ID_SYMDLGPLG_BORDER_COLOR = 10082;
const int ID_SYMDLGPLG_FILL_PATTERN = 10083;
const int ID_SYMDLGPLG_BORDER_WIDTH = 10084;
const int ID_SYMDLGPLG_FILL_COLOR = 10087;
const int ID_SYMDLGPLG_PANEL3 = 10085;
const int ID_SYMDLGPLG_PANEL4 = 10088;
const int ID_SYMDLGPLG_TRANSPARENCY = 10086;


const int ID_BTN_CLASSIFY = 10330;
const int ID_BTN_ADD = 10331;
const int ID_BTN_REMOVE = 10332;
const int ID_BTN_REMOVEALL = 10333;

class tmLayerProperties;
class tmGISDataVectorSHP;
class DataListReportCtrl;

class tmSymbolDataPolygonUnique
{
public:
	int m_PanelNo;
	wxColour m_bColour;
	int m_bWidth;
	wxColour m_fColour;
	int m_fStyle;
	int m_GlobalTransparency;
    
	tmSymbolDataPolygonUnique(){
		m_PanelNo = 0;
		m_bColour = *wxBLACK;
		m_bWidth = 1;
		m_fColour = *wxBLACK;
		m_fStyle = 0;
		m_GlobalTransparency = 0;
	}
};



class tmSymbolDLGPolygon : public tmSymbolDLG
{
private:
    wxColourPickerCtrl * m_PolygonBorderColourCtrl;
    wxColourPickerCtrl * m_PolygonFillColourCtrl;
    wxSpinCtrl* m_PolygonBorderWidthCtrl;
    wxChoice* m_PolygonFillPattern;
    tmSliderWithText * m_TransparencySlider;
    
    tmSymbolDataPolygonUnique m_DlgData;
    
    void _Init();
    void CreateControlsPoly();
    
    virtual bool TransferDataToWindow();
    virtual bool TransferDataFromWindow();
    
    
    DECLARE_DYNAMIC_CLASS( tmSymbolDLGPolygon )
public:
    /// Constructors
    tmSymbolDLGPolygon();
    ~tmSymbolDLGPolygon();
    tmSymbolDLGPolygon( wxWindow* parent, wxWindowID id = SYMBOL_TMSYMBOLDLG_IDNAME,
                       const wxString& caption = SYMBOL_TMSYMBOLDLG_TITLE,
                       const wxPoint& pos = SYMBOL_TMSYMBOLDLG_POSITION,
                       const wxSize& size = SYMBOL_TMSYMBOLDLG_SIZE,
                       long style = SYMBOL_TMSYMBOLDLG_STYLE );
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_TMSYMBOLDLG_IDNAME,
                const wxString& caption = SYMBOL_TMSYMBOLDLG_TITLE,
                const wxPoint& pos = SYMBOL_TMSYMBOLDLG_POSITION,
                const wxSize& size = SYMBOL_TMSYMBOLDLG_SIZE,
                long style = SYMBOL_TMSYMBOLDLG_STYLE );
    
    void SetDialogData ( const tmSymbolDataPolygonUnique & data) {m_DlgData = data;}
    tmSymbolDataPolygonUnique GetDialogData () {return m_DlgData;}
};





/*************************************************************************************//**
@brief Symbology dialog supporting rules
@author Lucien Schreiber copyright CREALP
@date 27 août 2012
*****************************************************************************************/
class tmSymbolDLGPolyRule : public tmSymbolDLG {
private:
    wxNotebook* m_SymbologyTypeCtrl;
    wxChoice* m_CategoryColumnCtrl;
    DataListReportCtrl * m_SymbolListCtrl;
    wxButton* m_ClassifyBtn;
    wxButton* m_AddBtn;
    wxButton* m_RemoveBtn;
    wxButton* m_RemoveAllBtn;
    wxImageList * m_ImgList;
    
    // unique
    wxColourPickerCtrl * m_PolygonBorderColourCtrl;
    wxColourPickerCtrl * m_PolygonFillColourCtrl;
    wxSpinCtrl* m_PolygonBorderWidthCtrl;
    wxChoice* m_PolygonFillPattern;
    tmSliderWithText * m_TransparencySlider;
    
    tmSymbolRuleArray m_Rules;
    tmLayerProperties * m_LayerProperties;
    tmGISDataVectorSHP * m_GISData;
    
    wxString m_SelectedField;    
    tmSymbolDataPolygonUnique m_PolyUniqueStyle;

    
    void _CreateControls();
    void _LoadTableData();
    wxBitmap _CreateColorBitmap(const wxBrush & brush, const wxPen & pen);

    
    void OnBtnClassify(wxCommandEvent & event);
    void OnBtnAdd(wxCommandEvent & event);
    void OnBtnRemove(wxCommandEvent & event);
    void OnBtnRemoveAll(wxCommandEvent & event);
    
    virtual bool TransferDataToWindow();
    virtual bool TransferDataFromWindow();
    DECLARE_EVENT_TABLE();
        
public:
    tmSymbolDLGPolyRule(wxWindow * parent, tmLayerProperties * layerproperties = NULL, wxWindowID id = SYMBOL_TMSYMBOLDLG_IDNAME, const wxString & caption = SYMBOL_TMSYMBOLDLG_TITLE, const wxPoint & pos = SYMBOL_TMSYMBOLDLG_POSITION, const wxSize & size = SYMBOL_TMSYMBOLDLG_SIZE, long style = SYMBOL_TMSYMBOLDLG_STYLE);
    virtual ~tmSymbolDLGPolyRule();
    bool Create(wxWindow * parent, wxWindowID id = SYMBOL_TMSYMBOLDLG_IDNAME, const wxString & caption = SYMBOL_TMSYMBOLDLG_TITLE, const wxPoint & pos = SYMBOL_TMSYMBOLDLG_POSITION, const wxSize & size = SYMBOL_TMSYMBOLDLG_SIZE, long style = SYMBOL_TMSYMBOLDLG_STYLE );
    
    tmSymbolRuleArray * GetRulesRef(){return &m_Rules;}
    
    int GetSelectedPanel();
    inline const wxString GetSelectedField() const;
    inline const tmSymbolDataPolygonUnique GetPolyUniqueStyle() const;
    void SetPolyUniqueStyle(tmSymbolDataPolygonUnique value);
    void SetSelectedField(wxString value);
    void SetSelectedPanel(int panelindex);
};


inline const wxString tmSymbolDLGPolyRule::GetSelectedField() const {
    return m_SelectedField;
}

inline const tmSymbolDataPolygonUnique tmSymbolDLGPolyRule::GetPolyUniqueStyle() const {
    return m_PolyUniqueStyle;
}







#endif
