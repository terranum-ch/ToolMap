/***************************************************************************
 editvertex_dlg.h
 Dialog for manuel editing vertex position
 -------------------
 copyright : (C) 2009 CREALP Lucien Schreiber
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

#ifndef _TM_EDIT_VERTEX_DLG_H_
#define _TM_EDIT_VERTEX_DLG_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

// Include wxWidgets' headers
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/grid.h>      // for grid definition
#include <wx/statline.h>  // for staticline

#include "../core/tmarraysize.h"          // for array of real
#include "../gis/tmlayerpropertiesdef.h"  // for LAYER_LINE definitions
#include "wxflatbutton.h"                 // for flat buttons.

#define ID_BTN_ADD_VERTEX 22100
#define ID_BTN_REMOVE_VERTEX 22101
#define ID_BTN_DISPLAYVERTEX 22102

const int tmVERTEX_PRECISION = 10;

// EVENT
DECLARE_EVENT_TYPE(tmEVT_EV_DISPLAY_VERTEX_COORD, -1);

// margin for systems
const int gSizeMargin = 10;

/***************************************************************************/ /**
  @brief Display a dialog for editing vertex position
  @details The displayed dialog contain a wxGrid with two columns displaying
  coordinates. It may be used for lines and for points. The pseudo-code bellow
  indicate how to use this class
  @code
  EditVertexDLG myDlg (parent);
  // init member data for data transfert
  myDlg.m_SelectedOID = selectedoid;
  myDlg.m_LayerType = LAYER_SPATIAL_LINE;
  myDlg.m_VertexPts = wxArrayRealPoints;

  // now display the dialog
  myDlg.ShowDialogModal();
  @endcode
  @author Lucien Schreiber (c) CREALP 2009
  @date 24 February 2009
  *******************************************************************************/
class EditVertexDLG : public wxDialog {
 private:
  // member controls
  wxGrid *m_VertexGrid;
  wxFlatButton *m_DisplayVertexPosBtn;
  wxStatusBar *m_Status;
  wxFlatButton *m_BtnAddVertex;
  wxFlatButton *m_BtnRemoveVertex;
  wxButton *m_BtnUpdate;

  // member functions
  void CreateControls();

  // status function
  void SetStatusSelectedID(long oid);

  void SetStatusNumberVertex(int ivertexnumber);

  // buttons update functions
  void UpdateAddVertexButton(bool selection);

  void UpdateRemoveVertexButton(bool selection);

  void UpdateHighlightVertexButton(bool selection);

  void UpdateSaveButton();

  // validation function
  bool IsAllCellsNumber(int &col, int &row);

  // Grid functions
  bool GridInsertLine(int iIndex = -1, wxRealPoint *pt = NULL);

  bool GridClear();

  int GridGetSelection();

  // event function
  void OnVertexAdd(wxCommandEvent &event);

  void OnVertexRemove(wxCommandEvent &event);

  void OnVertexHighlight(wxCommandEvent &event);

  void OnIdleTime(wxIdleEvent &event);

  void OnSave(wxCommandEvent &event);

  DECLARE_EVENT_TABLE();

 public:
  // ctor and dtor
  EditVertexDLG(wxWindow *parent, wxWindowID id = wxID_ANY, const wxString &title = _("Edit Vertex"),
                const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize,
                long style = wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER);

  ~EditVertexDLG();

  // member for data exchange
  wxArrayRealPoints m_VertexPts;
  long m_SelectedOID;
  int m_LayerType;

  // data transfert, called automatically
  virtual bool TransferDataToWindow();

  virtual bool TransferDataFromWindow();
};

#endif
