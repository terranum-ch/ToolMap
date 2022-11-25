#ifndef FEATURE_TOC_TOCCTRL_H
#define FEATURE_TOC_TOCCTRL_H

#include "wx/wxprec.h"

#ifndef WX_PRECOMP

#include "wx/wx.h"

#endif

#include <wx/dataview.h>

#include "tocctrlmodel.h"
#include "tmlayerproperties.h"


// EVENT FOR TOC CTRL
DECLARE_EVENT_TYPE(tmEVT_LM_REMOVE, -1)
DECLARE_EVENT_TYPE(tmEVT_LM_ADD, -1)
DECLARE_EVENT_TYPE(tmEVT_LM_UPDATE, -1)
DECLARE_EVENT_TYPE(tmEVT_LM_SHOW_PROPERTIES, -1)
DECLARE_EVENT_TYPE(tmEVT_LM_SHOW_LABELS, -1);
DECLARE_EVENT_TYPE(tmEVT_LM_TOC_EDITED, -1)
DECLARE_EVENT_TYPE(tmEVT_EM_EDIT_START, -1)
DECLARE_EVENT_TYPE(tmEVT_EM_EDIT_STOP, -1)
DECLARE_EVENT_TYPE(tmEVT_TOC_SELECTION_CHANGED, -1)

class TocCtrl : public wxDataViewCtrl {
 public:
  TocCtrl(wxWindow *parent, wxWindowID id);

  void add_test_data();

  void ExpandAll();

  TocCtrlModel *GetTocModel();

  void SetColour(const wxColour &colour_normal = wxNullColour, const wxColour &colour_dark = wxNullColour);
  wxColour GetColourNormal();
  wxColour GetColourDark();

  // toc ctrl function for toolmap
  tmLayerProperties * GetSelectionLayer();
  void SetSelectedLayer(int layerID);


 private:
  void OnDragndropBegin(wxDataViewEvent &event);
  void OnDragndropPossible(wxDataViewEvent &event);
  void OnDragndropDrop(wxDataViewEvent &event);
  void OnValueChanged(wxDataViewEvent &event);

  TocCtrlModelNode * m_drag_node_start;
  TocCtrlModelNode * m_drag_node_end;
};

#endif  // FEATURE_TOC_TOCCTRL_H
