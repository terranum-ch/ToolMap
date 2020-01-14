/***************************************************************************
 datalistreportctrl.h
 -------------------
 copyright            : (C) 2010 CREALP Lucien Schreiber
 ***************************************************************************/

#ifndef _DATALISTREPORTCTRL_H_
#define _DATALISTREPORTCTRL_H_

// wxWidgets include
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/dcgraph.h>
#include <wx/listctrl.h>
#include <wx/renderer.h>

enum DATALIST_COLTYPE {
  DATALIST_COLTYPE_TEXT = 0,
  DATALIST_COLTYPE_NUMBER,
  DATALIST_COLTYPE_COLOUR,
  DATALIST_COLTYPE_IMAGE,
  DATALIST_COLTYPE_DATE,
  DATALIST_COLTYPE_TIME
};

class DataListReportCtrl : public wxListCtrl {
 private:
  int _Compare(DATALIST_COLTYPE coltype, const wxString &text1, const wxString &text2, bool ascending);

  wxString _GetTextSpecial(long index, int col, DATALIST_COLTYPE typecol);

  wxColour m_ColourNormal;

  wxColour m_ColourGrayed;

  void _Init();

 public:
  DataListReportCtrl(wxWindow *parent, wxWindowID id, const wxPoint &pos = wxDefaultPosition,
                     const wxSize &size = wxDefaultSize, long style = wxLC_REPORT);

  bool Create(wxWindow *parent, wxWindowID id, const wxPoint &pos = wxDefaultPosition,
              const wxSize &size = wxDefaultSize, long style = wxLC_REPORT);

  virtual ~DataListReportCtrl();

  long GetSelectedFirst();

  wxString GetText(long index, int col = 0);

  void SetText(long index, const wxString &text, int col = 0);

  int GetItemColumnImage(long index, int col = 0);

  void SetColumnImage(int imageindex, int col);

  // compare functions
  void SortList(int col, int lowindex, int highindex, DATALIST_COLTYPE typecol, bool ascending = true);

  void SwapRow(int row1, int row2);

  // bitmap creation function
  wxBitmap CreateArrowDown(const wxSize &size = wxSize(16, 16));

  wxBitmap CreateArrowUp(const wxSize &size = wxSize(16, 16));

  wxColour GetTextColorNormal() {
    return m_ColourNormal;
  }

  wxColour GetTextColorGrayed() {
    return m_ColourGrayed;
  }
};

#endif
