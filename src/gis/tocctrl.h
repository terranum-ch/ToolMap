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
DECLARE_EVENT_TYPE(tmEVT_LM_ADD_GROUP, -1)
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

  // toc ctrl function for toolmap
  tmLayerProperties *GetSelectionLayer();
  void SelectLayerByID(int layerID);
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
  // dnd actions
  void OnDragndropBegin(wxDataViewEvent &event);
  void OnDragndropPossible(wxDataViewEvent &event);
  void OnDragndropDrop(wxDataViewEvent &event);

  // control actions
  void OnMouseClick(wxDataViewEvent &event);
  void OnMouseRightClick(wxDataViewEvent & event);
  void OnLayerSelectionChanged(wxDataViewEvent & event);

  // contextual menu actions
  void OnMenuRemoveItem(wxCommandEvent &event);
  void OnMenuAddGroup(wxCommandEvent &event);
  void OnMenuRenameGroup(wxCommandEvent &event);
  void OnMenuShowProperties(wxCommandEvent &event);
  void OnMenuPropertiesSave(wxCommandEvent &event);
  void OnMenuPropertiesLoad(wxCommandEvent &event);
  void OnMenuVertex(wxCommandEvent &event);
  void OnMenuShowLabels(wxCommandEvent &event);
  void OnMenuEditing(wxCommandEvent &event);

  TocCtrlModelNode *m_DragNodeStart;
  TocCtrlModelNode *m_DragNodeEnd;
  int m_IterateNodeIndex = wxNOT_FOUND;
  TocCtrlModelNodePtrArray m_IterateNodeArray;
  tmLayerProperties *m_EditingLayer;
  TocCtrlModelNode *GetNodeFromLayerID(long layer_id);
  void StopEditing(bool send_message);
};

#endif  // FEATURE_TOC_TOCCTRL_H
