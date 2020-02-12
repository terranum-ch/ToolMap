/***************************************************************************
 tmsymboldlgpoint.h
 Display Point Symbology dialog
 -------------------
 copyright : (C) 2007 CREALP Lucien Schreiber
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

#ifndef _TM_SYMBOLDLGPOINT_H_
#define _TM_SYMBOLDLGPOINT_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

// Include wxWidgets' headers
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/imaglist.h>
#include <wx/listctrl.h>
#include <wx/notebook.h>
#include <wx/spinctrl.h>
#include <wx/wupdlock.h>

#include "tmsymboldlg.h"  // tmSymbolDlg declaration
#include "tmsymbolrule.h"

const int ID_SYMDLGP_PANEL = 10216;
const int ID_SYMDLGP_NOTEBOOK = 10080;
const int ID_SYMDLGP_PANEL2 = 10081;
const int ID_SYMDLGP_COLOR = 10082;
const int ID_SYMDLGP_WIDTH = 10084;
const int ID_SYMDLGP_PANEL3 = 10085;
const int ID_SYMDLGP_PANEL4 = 10088;
const int ID_SYMDLGP_TRANSPARENCY = 10086;

const int ID_BTN_CLASSIFY_POINT = 10330;
const int ID_BTN_ADD_POINT = 10331;
const int ID_BTN_REMOVE_POINT = 10332;
const int ID_BTN_REMOVEALL_POINT = 10333;
const int ID_LIST_SYMBOL_POINT = 10334;

class tmLayerProperties;

class tmGISDataVectorSHP;

class DataListReportCtrl;

class tmSymbolDataPointUnique {
 public:
  wxColour m_Colour;
  int m_Radius;
  int m_GlobalTransparency;
  int m_PanelNo;

  tmSymbolDataPointUnique() {
    m_PanelNo = 0;
    m_Colour = *wxBLACK;
    m_Radius = 5;
    m_GlobalTransparency = 0;
  }
};

class tmSymbolDLGPoint : public tmSymbolDLG {
 private:
  wxColourPickerCtrl *m_PointColourCtrl;
  wxSpinCtrl *m_PointWidthCtrl;
  tmSliderWithText *m_TransparencySlider;

  tmSymbolDataPointUnique m_DlgData;

  void _Init();

  void CreateControlsPoint();

  virtual bool TransferDataToWindow();

  virtual bool TransferDataFromWindow();

  DECLARE_DYNAMIC_CLASS(tmSymbolDLGPoint)

  DECLARE_EVENT_TABLE()

 public:
  /// Constructors
  tmSymbolDLGPoint();

  ~tmSymbolDLGPoint();

  tmSymbolDLGPoint(wxWindow *parent, wxWindowID id = SYMBOL_TMSYMBOLDLG_IDNAME,
                   const wxString &caption = SYMBOL_TMSYMBOLDLG_TITLE, const wxPoint &pos = SYMBOL_TMSYMBOLDLG_POSITION,
                   const wxSize &size = SYMBOL_TMSYMBOLDLG_SIZE, long style = SYMBOL_TMSYMBOLDLG_STYLE);

  bool Create(wxWindow *parent, wxWindowID id = SYMBOL_TMSYMBOLDLG_IDNAME,
              const wxString &caption = SYMBOL_TMSYMBOLDLG_TITLE, const wxPoint &pos = SYMBOL_TMSYMBOLDLG_POSITION,
              const wxSize &size = SYMBOL_TMSYMBOLDLG_SIZE, long style = SYMBOL_TMSYMBOLDLG_STYLE);

  void SetDialogData(const tmSymbolDataPointUnique &data) {
    m_DlgData = data;
  }

  tmSymbolDataPointUnique GetDialogData() {
    return m_DlgData;
  }
};

/*************************************************************************************/ /**
 @brief Symbology dialog for points supporting rules
 @author Lucien Schreiber copyright CREALP
 @date 02 septembre 2012
 *****************************************************************************************/
class tmSymbolDLGPointRule : public tmSymbolDLG {
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
  wxColourPickerCtrl *m_PointColourCtrl;
  wxSpinCtrl *m_PointWidthCtrl;
  tmSliderWithText *m_TransparencySlider;

  tmSymbolRuleArray m_Rules;
  tmLayerProperties *m_LayerProperties;

  tmGISDataVectorSHP *m_GISData;
  wxString m_SelectedField;
  tmSymbolDataPointUnique m_PointUniqueStyle;

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
  tmSymbolDLGPointRule(wxWindow *parent, tmLayerProperties *layerproperties = nullptr,
                       wxWindowID id = SYMBOL_TMSYMBOLDLG_IDNAME, const wxString &caption = SYMBOL_TMSYMBOLDLG_TITLE,
                       const wxPoint &pos = SYMBOL_TMSYMBOLDLG_POSITION, const wxSize &size = SYMBOL_TMSYMBOLDLG_SIZE,
                       long style = SYMBOL_TMSYMBOLDLG_STYLE);

  virtual ~tmSymbolDLGPointRule();

  bool Create(wxWindow *parent, wxWindowID id = SYMBOL_TMSYMBOLDLG_IDNAME,
              const wxString &caption = SYMBOL_TMSYMBOLDLG_TITLE, const wxPoint &pos = SYMBOL_TMSYMBOLDLG_POSITION,
              const wxSize &size = SYMBOL_TMSYMBOLDLG_SIZE, long style = SYMBOL_TMSYMBOLDLG_STYLE);

  inline tmSymbolRuleArray *GetRulesRef() {
    return &m_Rules;
  };

  int GetSelectedPanel();

  inline const wxString GetSelectedField() const;

  inline tmSymbolDataPointUnique GetPointUniqueStyle();

  void SetPointUniqueStyle(tmSymbolDataPointUnique value);

  void SetSelectedField(wxString value);

  void SetSelectedPanel(int panelindex);
};

inline const wxString tmSymbolDLGPointRule::GetSelectedField() const {
  return m_SelectedField;
}

inline tmSymbolDataPointUnique tmSymbolDLGPointRule::GetPointUniqueStyle() {
  return m_PointUniqueStyle;
}

#endif
