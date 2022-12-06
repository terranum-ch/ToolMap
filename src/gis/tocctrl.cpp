#include "tocctrl.h"
#include "tmtocctrlmenu.h"

#include "tocbitmaps.h"
#include "tocrenderer.h"

DEFINE_EVENT_TYPE(tmEVT_LM_REMOVE)
DEFINE_EVENT_TYPE(tmEVT_LM_ADD)
DEFINE_EVENT_TYPE(tmEVT_LM_UPDATE)
DEFINE_EVENT_TYPE(tmEVT_LM_SHOW_PROPERTIES)
DEFINE_EVENT_TYPE(tmEVT_LM_SHOW_LABELS)
DEFINE_EVENT_TYPE(tmEVT_LM_TOC_EDITED)
DEFINE_EVENT_TYPE(tmEVT_EM_EDIT_START)
DEFINE_EVENT_TYPE(tmEVT_EM_EDIT_STOP)
DEFINE_EVENT_TYPE(tmEVT_TOC_SELECTION_CHANGED)

TocCtrl::TocCtrl(wxWindow *parent, wxWindowID id)
    : wxDataViewCtrl(parent, id, wxDefaultPosition, wxDefaultSize, wxDV_SINGLE | wxDV_NO_HEADER) {
  m_DragNodeStart = nullptr;
  m_DragNodeEnd = nullptr;
  m_EditingLayer = nullptr;

  // Setting model
  wxObjectDataPtr<wxDataViewModel> my_model(new TocCtrlModel);
  wxDataViewCtrl::AssociateModel(my_model.get());
  EnableDragSource(wxDF_UNICODETEXT);
  EnableDropTarget(wxDF_UNICODETEXT);

  // Column definition
  auto *cr = new tocRenderer(wxDATAVIEW_CELL_ACTIVATABLE, (wxDataViewTreeCtrl *)this);
  auto *column5 = new wxDataViewColumn("custom", cr, 0, wxCOL_WIDTH_DEFAULT, wxALIGN_LEFT, wxDATAVIEW_COL_RESIZABLE);
  wxDataViewCtrl::AppendColumn(column5);

  // events
  this->Bind(wxEVT_DATAVIEW_ITEM_BEGIN_DRAG, &TocCtrl::OnDragndropBegin, this);
  this->Bind(wxEVT_DATAVIEW_ITEM_DROP_POSSIBLE, &TocCtrl::OnDragndropPossible, this);
  this->Bind(wxEVT_DATAVIEW_ITEM_DROP, &TocCtrl::OnDragndropDrop, this);
  this->Bind(wxEVT_DATAVIEW_ITEM_ACTIVATED, &TocCtrl::OnMouseClick, this);
  this->Bind(wxEVT_DATAVIEW_ITEM_CONTEXT_MENU, &TocCtrl::OnMouseRightClick, this);
  this->Bind(wxEVT_DATAVIEW_SELECTION_CHANGED, &TocCtrl::OnLayerSelectionChanged, this);

  // contextual menu event
  this->Bind(wxEVT_MENU, &TocCtrl::OnMenuRemoveItem, this, ID_TOCMENU_REMOVE);
  this->Bind(wxEVT_MENU, &TocCtrl::OnMenuAddGroup, this, ID_TOCMENU_ADD_GROUP);
  this->Bind(wxEVT_MENU, &TocCtrl::OnMenuRenameGroup, this, ID_TOCMENU_RENAME_GROUP);
  this->Bind(wxEVT_MENU, &TocCtrl::OnMenuRemoveItem, this, ID_TOCMENU_REMOVE_GROUP);
  this->Bind(wxEVT_MENU, &TocCtrl::OnMenuShowProperties, this, ID_TOCMENU_PROPERTIES);
  this->Bind(wxEVT_MENU, &TocCtrl::OnMenuPropertiesSave, this, ID_TOCMENU_PROPERTIES_SAVE);
  this->Bind(wxEVT_MENU, &TocCtrl::OnMenuPropertiesLoad, this, ID_TOCMENU_PROPERTIES_LOAD);
  this->Bind(wxEVT_MENU, &TocCtrl::OnMenuVertex, this, ID_TOCMENU_SHOW_VERTEX_NONE, ID_TOCMENU_SHOW_VERTEX_BEGIN_END);
  this->Bind(wxEVT_MENU, &TocCtrl::OnMenuEditing, this, ID_TOCMENU_EDIT_LAYER);
  this->Bind(wxEVT_MENU, &TocCtrl::OnMenuShowLabels, this, ID_TOCMENU_LABELS);
}

//void TocCtrl::add_test_data() {
//  TocCtrlModel *model = GetTocModel();
//  auto *my_root = (TocCtrlModelNode *)model->GetRoot().GetID();
//  auto *group_const = model->NodeAdd(my_root, "Construction");
//  auto *line = model->NodeAdd(group_const, "Line", true, 2, true);
//  auto *point = model->NodeAdd(group_const, "Point", false, 2, false);
//
//  auto *group_support = model->NodeAdd(my_root, "Support");
//  auto *shape = model->NodeAdd(group_support, "Shapefile", false, 1, false);
//  auto *raster = model->NodeAdd(group_support, "Raster", true, 3, false);
//  auto *web = model->NodeAdd(group_support, "Web raster", true, 4, false);
//  ExpandAll();
//}

void TocCtrl::OnDragndropBegin(wxDataViewEvent &event) {
  wxDataViewItem item(event.GetItem());
  m_DragNodeStart = nullptr;
  m_DragNodeEnd = nullptr;

  // only allow drags for item, not containers

  if (GetTocModel()->GetRoot() == item) {
    wxLogMessage("Forbidding starting dragging Root");
    event.Veto();
    return;
  }
  auto *node = (TocCtrlModelNode *)item.GetID();
  m_DragNodeStart = node;

  auto *obj = new wxTextDataObject;
  obj->SetText(node->m_LayerProp->GetName().GetName());

  event.SetDataObject(obj);
  event.SetDragFlags(wxDrag_AllowMove);  // allows both copy and move

  wxLogMessage("Starting dragging \"%s\"", node->m_LayerProp->GetName().GetName());
}

void TocCtrl::OnDragndropPossible(wxDataViewEvent &event) {
  if (event.GetDataFormat() != wxDF_UNICODETEXT)
    event.Veto();
  else
    event.SetDropEffect(wxDragMove);  // check 'move' drop effect
}

void TocCtrl::OnDragndropDrop(wxDataViewEvent &event) {
  wxDataViewItem target_item(event.GetItem());
  m_DragNodeEnd = nullptr;

  if (event.GetDataFormat() != wxDF_UNICODETEXT) {
    event.Veto();
    return;
  }

  wxTextDataObject obj;
  obj.SetData(wxDF_UNICODETEXT, event.GetDataSize(), event.GetDataBuffer());
  auto *my_model = GetTocModel();
  m_DragNodeEnd = TocCtrlModel::ConvertFromDataViewItem(my_model->GetRoot());

  int my_proposed_drop_index = event.GetProposedDropIndex();
  if (target_item.IsOk()) {
    auto *target_node = TocCtrlModel::ConvertFromDataViewItem(target_item);
    m_DragNodeEnd = target_node;

    // veto if trying to drop on myself
    if (m_DragNodeEnd == m_DragNodeStart) {
      event.Veto();
      return;
    }

    // veto if trying to drop to a child of the container
    if (m_DragNodeStart->IsMyChildren(m_DragNodeEnd)) {
      wxLogError("Unable to drop on a child!");
      event.Veto();
      return;
    }

    // item dropped between elements
    if (my_model->IsContainer(target_item)) {
      if (my_proposed_drop_index == wxNOT_FOUND) {
        my_proposed_drop_index = 0;
      }
      wxLogMessage("Text '%s' dropped in container '%s' (proposed index = %d)", obj.GetText(),
                   my_model->NodeGetTitle(target_node), my_proposed_drop_index);
    } else {  // item dropped on another item
      TocCtrlModelNodePtrArray my_parent_array = target_node->GetParent()->GetChildren();
      my_proposed_drop_index = 0;
      for (int i = 0; i < my_parent_array.GetCount(); i++) {
        if (my_parent_array[i] == target_node) {
          my_proposed_drop_index = i;
        }
      }
      wxLogMessage("Text '%s' dropped on target_item '%s', index %d", obj.GetText(),
                   my_model->NodeGetTitle(target_node), my_proposed_drop_index);
    }
  } else {  // item dropped on background
    my_proposed_drop_index = wxNOT_FOUND;
    wxLogMessage("Text '%s' dropped on background (proposed index = %d)", obj.GetText(), my_proposed_drop_index);
  }
  my_model->NodeMove(m_DragNodeStart, m_DragNodeEnd, my_proposed_drop_index);
}

void TocCtrl::OnMouseClick(wxDataViewEvent &event) {
  auto node = TocCtrlModel::ConvertFromDataViewItem( event.GetItem());
  if (node->IsContainer()){
    wxLogError("Activation of folder isn't implemented!");
    return;
  }

  // Send message show/hide to layermanager
  wxCommandEvent evt(tmEVT_LM_UPDATE, wxID_ANY);
  evt.SetInt((int)node->m_LayerProp->IsVisible());
  GetEventHandler()->QueueEvent(evt.Clone());
}

void TocCtrl::ExpandAll() {
  wxDataViewItem my_root = dynamic_cast<TocCtrlModel *>(GetModel())->GetRoot();
  wxDataViewItemArray my_root_childs;
  GetModel()->GetChildren(my_root, my_root_childs);
  for (int i = 0; i < my_root_childs.GetCount(); i++) {
    Expand(my_root_childs[i]);
  }
}

void TocCtrl::SetColour(const wxColour &colour_normal, const wxColour &colour_dark) {
  auto renderer = dynamic_cast<tocRenderer *>(wxDataViewCtrl::GetColumn(0)->GetRenderer());
  renderer->SetColour(colour_normal, colour_dark);
  wxWindow::Refresh();
}

wxColour TocCtrl::GetColourNormal() {
  auto renderer = dynamic_cast<tocRenderer *>(wxDataViewCtrl::GetColumn(0)->GetRenderer());
  return renderer->GetColourNormal();
}

wxColour TocCtrl::GetColourDark() {
  auto renderer = dynamic_cast<tocRenderer *>(wxDataViewCtrl::GetColumn(0)->GetRenderer());
  return renderer->GetColourDark();
}

TocCtrlModel *TocCtrl::GetTocModel() {
  return dynamic_cast<TocCtrlModel *>(GetModel());
}

tmLayerProperties *TocCtrl::GetSelectionLayer() {
  wxDataViewItem selected = GetSelection();
  if (!selected.IsOk()){
    return nullptr;
  }
  auto *selected_node = TocCtrlModel::ConvertFromDataViewItem(selected);
  wxASSERT(selected_node->m_LayerProp);
  return selected_node->m_LayerProp;
}

void TocCtrl::SetSelectedLayer(int layerID) {
    wxFAIL_MSG("Not implemented!");
}

tmLayerProperties *TocCtrl::GetLayerByPath(const wxString &layerPath) {
  bool bReset = true;
  wxFileName layer_filename(layerPath);
  while (true){
    tmLayerProperties *layer = IterateLayers(bReset);
    bReset = false;
    if (layer == nullptr){
      break ;
    }
    if (layer->GetName() == layer_filename){
      return layer;
    }
  }
  return nullptr;
}

tmLayerProperties *TocCtrl::GetLayerById(long layer_id) {
  bool bReset = true;
  while (true){
    tmLayerProperties *layer = IterateLayers(bReset);
    bReset = false;
    if (layer == nullptr){
      break ;
    }
    if (layer->GetID() == layer_id){
      return layer;
    }
  }
  return nullptr;
}

tmLayerProperties *TocCtrl::GetLayerByName(const wxString &layerName) {
  bool bReset = true;
  while (true){
    tmLayerProperties *layer = IterateLayers(bReset);
    bReset = false;
    if (layer == nullptr){
      break ;
    }
    if (layer->GetName() == layerName){
      return layer;
    }
  }
  return nullptr;
}

tmLayerProperties *TocCtrl::IterateLayers(bool ResetToLast) {
  if (ResetToLast){
    m_IterateNodeArray.Clear();
    TocCtrlModelNode * root =  TocCtrlModel::ConvertFromDataViewItem(GetTocModel()->GetRoot());
    root->GetAllChildRecursive(m_IterateNodeArray, nullptr);
    m_IterateNodeIndex = m_IterateNodeArray.GetCount() -1;
    if (m_IterateNodeIndex < 0){ // no data in the toc
      return nullptr;
    }
    tmLayerProperties * layerprop = m_IterateNodeArray.Item(m_IterateNodeIndex)->m_LayerProp;
    wxASSERT(layerprop);
    m_IterateNodeIndex--;
    return layerprop;
  }

  if (m_IterateNodeIndex >= 0 && m_IterateNodeArray.GetCount() > 0){
    tmLayerProperties * layerprop = m_IterateNodeArray.Item(m_IterateNodeIndex)->m_LayerProp;
    wxASSERT(layerprop);
    m_IterateNodeIndex--;
    return layerprop;
  }

  m_IterateNodeIndex = wxNOT_FOUND;
  m_IterateNodeArray.Clear();
  return nullptr;
}

tmLayerProperties *TocCtrl::GetEditLayer(){
  return m_EditingLayer;
}

void TocCtrl::SetEditLayer(tmLayerProperties *mEditingLayer) {
  m_EditingLayer = mEditingLayer;
}

void TocCtrl::SetProjectName(const wxString &project_name) {
  // root is already defined, change name
  auto root = TocCtrlModel::ConvertFromDataViewItem(GetTocModel()->GetRoot());
  wxASSERT(root);
  root->m_LayerProp->SetName(wxFileName("", project_name));
}

bool TocCtrl::InsertLayer(tmLayerProperties *item) {
  wxString myDisplayName = item->GetNameDisplay();
  auto * model = GetTocModel();
  model->NodeAdd(TocCtrlModel::ConvertFromDataViewItem(model->GetRoot()), item);
  return true;
}

unsigned int TocCtrl::GetCountLayers() {
  auto * model = GetTocModel();
  auto * root = TocCtrlModel::ConvertFromDataViewItem(model->GetRoot());
  wxASSERT(root);
  TocCtrlModelNodePtrArray node_array;
  root->GetAllChildRecursive(node_array);
  return node_array.GetCount();
}

bool TocCtrl::UpdateLayerName(tmLayerProperties *item, const wxString &newName) {
  auto *root = TocCtrlModel::ConvertFromDataViewItem(GetTocModel()->GetRoot());
  wxASSERT(root);

  TocCtrlModelNodePtrArray iterateNodeArray;
  root->GetAllChildRecursive(iterateNodeArray);

  for (auto node : iterateNodeArray) {
    if (node->m_LayerProp == item) {
      wxFileName name = node->m_LayerProp->GetName();
      name.SetFullName(newName);
      node->m_LayerProp->SetName(name);
    }
  }
  return true;
}

bool TocCtrl::RemoveLayer(int database_layer_id) {
  auto *model = GetTocModel();
  auto *root = TocCtrlModel::ConvertFromDataViewItem(GetTocModel()->GetRoot());
  wxASSERT(root);

  TocCtrlModelNodePtrArray iterateNodeArray;
  root->GetAllChildRecursive(iterateNodeArray);
  for (auto node : iterateNodeArray) {
    if (node->m_LayerProp->GetID() == database_layer_id){
      model->Delete(TocCtrlModel::ConvertFromNode(node));
      return true;
    }
  }
  return false;
}

void TocCtrl::OnMouseRightClick(wxDataViewEvent &event) {
  tmLayerProperties *layerProp = nullptr;
  if (event.GetItem().IsOk()) {
    layerProp = TocCtrlModel::ConvertFromDataViewItem(event.GetItem())->m_LayerProp;
  }

  tmTOCCtrlMenu menu(layerProp, 0, 0);
  PopupMenu(&menu);
  event.Skip();
}

void TocCtrl::OnLayerSelectionChanged(wxDataViewEvent &event) {
  wxCommandEvent evt(tmEVT_TOC_SELECTION_CHANGED, wxID_ANY);
  GetEventHandler()->QueueEvent(evt.Clone());
  event.Skip();
}

void TocCtrl::OnMenuRemoveItem(wxCommandEvent &event) {
  wxDataViewItem item = GetSelection();
  if (!item.IsOk()){
    return;
  }

  auto node = TocCtrlModel::ConvertFromDataViewItem(item);
  if (node->IsContainer() && node->GetChildCount() > 0){
    wxLogError("Unable to remove group: %d child(ren) inside!", node->GetChildCount());
    return ;
  }

  tmLayerProperties * layer_prop = node->m_LayerProp;
  wxASSERT(layer_prop);
  if (layer_prop->GetType() < TOC_NAME_GROUP){
    wxLogMessage(_("Not allowed to remove generic layers from project"));
    return ;
  }

  wxCommandEvent evt(tmEVT_LM_REMOVE, wxID_ANY);
  evt.SetExtraLong(layer_prop->GetID());
  if (RemoveLayer(layer_prop->GetID())) {
    GetEventHandler()->QueueEvent(evt.Clone());
  }
}

void TocCtrl::OnMenuAddGroup(wxCommandEvent &event) {
  wxString newGroupName = wxGetTextFromUser("Group name");
  if (newGroupName.IsEmpty()) {
    return;
  }

  auto *model = GetTocModel();
  wxDataViewItem item = GetSelection();
  if (!item.IsOk()){
    // If no node is selected, add group at the end of the TOC
    model->NodeAdd(TocCtrlModel::ConvertFromDataViewItem(model->GetRoot()), newGroupName);
  } else {
    auto *node = TocCtrlModel::ConvertFromDataViewItem(item);
    if (!node->IsContainer()) {
      node = node->GetParent();
    }
    model->NodeAdd(node, newGroupName);
  }
}

void TocCtrl::OnMenuRenameGroup(wxCommandEvent &event) {
  wxDataViewItem item = GetSelection();
  if (!item.IsOk()){
    return;
  }

  auto *model = GetTocModel();
  auto *node = TocCtrlModel::ConvertFromDataViewItem(item);
  if (!node->IsContainer()){
    wxLogError("Changing name only works for groups!");
    return;
  }

  wxString newGroupName = wxGetTextFromUser("Group name", wxASCII_STR(wxGetTextFromUserPromptStr), model->NodeGetTitle(node));
  if (newGroupName.IsEmpty()){
    return;
  }
  model->NodeSetTitle(node, newGroupName);
}

void TocCtrl::OnMenuShowProperties(wxCommandEvent &event) {
  wxDataViewItem item = GetSelection();
  if (!item.IsOk()){
    return;
  }

  auto node = TocCtrlModel::ConvertFromDataViewItem(item);
  if (node->IsContainer()){
    wxLogError("Unable to display symbology for container");
    return ;
  }

  tmLayerProperties * layer_prop = node->m_LayerProp;
  wxASSERT(layer_prop);
  wxASSERT(layer_prop->GetSymbolRef());
  wxCommandEvent Evt(tmEVT_LM_SHOW_PROPERTIES, wxID_ANY);
  Evt.SetClientData(layer_prop);
  ProcessEvent(Evt);
}

void TocCtrl::OnMenuPropertiesSave(wxCommandEvent &event) {}
void TocCtrl::OnMenuPropertiesLoad(wxCommandEvent &event) {}

void TocCtrl::OnMenuVertex(wxCommandEvent &event) {
  wxDataViewItem item = GetSelection();
  if (!item.IsOk()){
    return;
  }

  auto node = TocCtrlModel::ConvertFromDataViewItem(item);
  if (node->IsContainer()){
    wxLogError("Unable to display symbology for container");
    return ;
  }

  tmLayerProperties * layer_prop = node->m_LayerProp;
  wxASSERT(layer_prop);

  // keep old value to avoid drawing if value not changed
  int oldflags = layer_prop->GetVertexFlags();
  switch (event.GetId()) {
    case ID_TOCMENU_SHOW_VERTEX_ALL:
      layer_prop->SetVertexFlags(tmDRAW_VERTEX_ALL);
      break;
    case ID_TOCMENU_SHOW_VERTEX_BEGIN_END:
      layer_prop->SetVertexFlags(tmDRAW_VERTEX_BEGIN_END);
      break ;
    case ID_TOCMENU_SHOW_VERTEX_NONE:
      layer_prop->SetVertexFlags(tmDRAW_VERTEX_NONE);
      break ;
  }
  if (layer_prop->GetVertexFlags() == oldflags){
    return;
  }
  wxCommandEvent evt(tmEVT_LM_UPDATE, wxID_ANY);
  GetEventHandler()->QueueEvent(evt.Clone());

  // TODO: Uncomment this code when saving is working...
  // wxCommandEvent evtSave(tmEVT_LM_TOC_EDITED, wxID_ANY);
  // GetEventHandler()->QueueEvent(evtSave.Clone());
}

void TocCtrl::OnMenuShowLabels(wxCommandEvent &event) {}

void TocCtrl::OnMenuEditing(wxCommandEvent &event) {
  // start editing
  if (event.IsChecked()){

  }else {

  }
}
