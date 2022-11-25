#ifndef FEATURE_TOC_TOCCTRL_H
#define FEATURE_TOC_TOCCTRL_H

#include "wx/wxprec.h"

#ifndef WX_PRECOMP

#include "wx/wx.h"

#endif

#include <wx/dataview.h>

#include "tocctrlmodel.h"

class TocCtrl : public wxDataViewCtrl {
 public:
  TocCtrl(wxWindow *parent, wxWindowID id);

  void add_test_data();

  void ExpandAll();

  TocCtrlModel *GetTocModel();

  void SetColour(const wxColour &colour_normal = wxNullColour, const wxColour &colour_dark = wxNullColour);
  wxColour GetColourNormal();
  wxColour GetColourDark();

  //    wxTreeListItem add_group(wxTreeListItem parent, const wxString & label);
  //    wxTreeListItem add_layer(wxTreeListItem parent, const wxString &label, int type);
 private:
  void OnDragndropBegin(wxDataViewEvent &event);
  void OnDragndropPossible(wxDataViewEvent &event);
  void OnDragndropDrop(wxDataViewEvent &event);
  void OnValueChanged(wxDataViewEvent &event);

  TocCtrlModelNode * m_drag_node_start;
  TocCtrlModelNode * m_drag_node_end;
};

#endif  // FEATURE_TOC_TOCCTRL_H
