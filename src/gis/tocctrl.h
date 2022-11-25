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

  //void add_test_data();

  void ExpandAll();

  TocCtrlModel *GetTocModel();

  void SetColour(const wxColour &colour_normal = wxNullColour, const wxColour &colour_dark = wxNullColour);
  wxColour GetColourNormal();
  wxColour GetColourDark();

  // toc ctrl function for toolmap
  tmLayerProperties *GetSelectionLayer();
  void SetSelectedLayer(int layerID);
  tmLayerProperties *GetLayerByPath(const wxString &layerPath);
  tmLayerProperties *GetLayerByName(const wxString &layerName); // TODO: Is this function really needed, duplicate of GetLayerByPath
  tmLayerProperties *GetLayerById(long layer_id);
  tmLayerProperties *IterateLayers(bool ResetToLast);
  tmLayerProperties *GetEditLayer();
  void SetEditLayer(tmLayerProperties *mEditingLayer);
  void SetProjectName(const wxString &project_name);
  bool InsertLayer(tmLayerProperties *item);
  unsigned int GetCountLayers();
  bool UpdateLayerName(tmLayerProperties *item, const wxString &newName);
  bool RemoveLayer(int database_layer_id);
  

 private:
  void OnDragndropBegin(wxDataViewEvent &event);
  void OnDragndropPossible(wxDataViewEvent &event);
  void OnDragndropDrop(wxDataViewEvent &event);
  void OnMouseClick(wxDataViewEvent &event);
  void OnMouseRightClick(wxDataViewEvent & event);

  TocCtrlModelNode * m_drag_node_start;
  TocCtrlModelNode * m_drag_node_end;
  int m_iterate_node_index = wxNOT_FOUND;
  TocCtrlModelNodePtrArray m_iterate_node_array;
  tmLayerProperties * m_editing_layer;

};

#endif  // FEATURE_TOC_TOCCTRL_H
