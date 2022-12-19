#include "tocctrl.h"

#include "../components/wxserialize/tmserialize.h"  // for object serialization
#include "tmsymbolrule.h"
#include "tmtocctrlmenu.h"
#include "tocrenderer.h"

DEFINE_EVENT_TYPE(tmEVT_LM_REMOVE)
DEFINE_EVENT_TYPE(tmEVT_LM_ADD)
DEFINE_EVENT_TYPE(tmEVT_LM_GROUP_ADD)
DEFINE_EVENT_TYPE(tmEVT_LM_UPDATE)
DEFINE_EVENT_TYPE(tmEVT_LM_SHOW_PROPERTIES)
DEFINE_EVENT_TYPE(tmEVT_LM_SHOW_LABELS)
DEFINE_EVENT_TYPE(tmEVT_LM_TOC_EDITED)
DEFINE_EVENT_TYPE(tmEVT_EM_EDIT_START)
DEFINE_EVENT_TYPE(tmEVT_EM_EDIT_STOP)
DEFINE_EVENT_TYPE(tmEVT_TOC_SELECTION_CHANGED)

TocCtrl::TocCtrl(wxWindow *parent, wxWindowID id)
    : wxDataViewCtrl(parent, id, wxDefaultPosition, wxDefaultSize, wxDV_MULTIPLE | wxDV_NO_HEADER) {
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

  if (GetTocModel()->GetRoot() == item) {
    wxLogMessage("Forbidding starting dragging Root");
    event.Veto();
    return;
  }

  auto *node = (TocCtrlModelNode *)item.GetID();

  auto *obj = new wxTextDataObject;
  obj->SetText(node->m_LayerProp->GetName().GetName());

  event.SetDataObject(obj);
  event.SetDragFlags(wxDrag_AllowMove);  // allows both copy and move
}

void TocCtrl::OnDragndropPossible(wxDataViewEvent &event) {
  if (event.GetDataFormat() != wxDF_UNICODETEXT)
    event.Veto();
  else
    event.SetDropEffect(wxDragMove);  // check 'move' drop effect
}

void TocCtrl::OnDragndropDrop(wxDataViewEvent &event) {
  wxDataViewItem targetItem(event.GetItem());
  TocCtrlModelNode *nodeEnd = nullptr;

  if (event.GetDataFormat() != wxDF_UNICODETEXT) {
    event.Veto();
    return;
  }

  auto *model = GetTocModel();

  // Get selected items
  wxDataViewItemArray selectedItems;
  int nItems = this->GetSelections(selectedItems);

  nodeEnd = TocCtrlModel::ConvertFromDataViewItem(model->GetRoot());

  int proposedDropIndex = event.GetProposedDropIndex();
  if (targetItem.IsOk()) {
    auto *targetNode = TocCtrlModel::ConvertFromDataViewItem(targetItem);
    nodeEnd = targetNode;

    // veto if trying to drop on myself
    for (auto item: selectedItems) {
      TocCtrlModelNode* nodeStart = TocCtrlModel::ConvertFromDataViewItem(item);
      if (nodeEnd == nodeStart) {
        event.Veto();
        return;
      }

      // veto if trying to drop to a child of the container
      if (nodeStart->IsMyChildren(nodeEnd)) {
        wxLogError("Unable to drop on a child!");
        event.Veto();
        return;
      }
    }

    // item dropped between elements
    if (model->IsContainer(targetItem)) {
      if (proposedDropIndex == wxNOT_FOUND) {
        proposedDropIndex = 0;
      }
    } else {  // item dropped on another item
      TocCtrlModelNodePtrArray parentArray = targetNode->GetParent()->GetChildren();
      proposedDropIndex = 0;
      for (int i = 0; i < parentArray.GetCount(); i++) {
        if (parentArray[i] == targetNode) {
          proposedDropIndex = i;
        }
      }
    }
  } else {  // item dropped on background
    proposedDropIndex = wxNOT_FOUND;
  }

  model->NodeMove(selectedItems, nodeEnd, proposedDropIndex);
}

void TocCtrl::OnMouseClick(wxDataViewEvent &event) {
  // Send message show/hide to layermanager
  auto node = TocCtrlModel::ConvertFromDataViewItem( event.GetItem());
  if (node == TocCtrlModel::ConvertFromDataViewItem(GetTocModel()->GetRoot())){
    wxLogWarning(_("Hiding all the project does nothing!"));
    return;
  }

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

void TocCtrl::SelectLayerByID(int layerID) {
  tmLayerProperties *layerprop = nullptr;
  bool reset = true;
  while (true) {
    layerprop = IterateLayers(reset);
    reset = false;
    if (!layerprop) {
      break;
    }
    if (layerprop->GetID() == layerID) break;
  }

  if (!layerprop){
    return;
  }

  auto node = TocCtrlModel::ConvertFromNode(GetNodeFromLayerID(layerprop->GetID()));
  wxDataViewCtrl::UnselectAll();
  wxDataViewCtrl::Select(node);
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
  if (item->GetLayerParentId() == 0) {
    model->NodeAdd(TocCtrlModel::ConvertFromDataViewItem(model->GetRoot()), item);
  }
  else {
    auto parent_node = GetNodeFromLayerID(item->GetLayerParentId());
    model->NodeAdd(parent_node, item);
  }
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
  if (layerProp && layerProp->GetType() < TOC_NAME_GROUP){
    menu.Check(ID_TOCMENU_EDIT_LAYER, layerProp->IsEditing());
  }
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
  wxCommandEvent evt(tmEVT_LM_GROUP_ADD, wxID_ANY);
  GetEventHandler()->QueueEvent(evt.Clone());
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

void TocCtrl::OnMenuPropertiesSave(wxCommandEvent &event) {
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
  wxFileName layer_filename = layer_prop->GetName();
  layer_filename.SetExt("tly");
  wxString save_filepath =
      wxSaveFileSelector(_("Save symbology"), layer_filename.GetExt(), layer_filename.GetFullName(), this);
  if (save_filepath == wxEmptyString) {
    return;
  }

  // prepare serialization object and write to file
  tmSerialize out;
  layer_prop->GetSymbolRuleManagerRef()->Serialize(out);

  wxFile myFile(save_filepath, wxFile::write);
  if (!myFile.IsOpened()) {
    wxLogError(_("Error creating file: '%s'"), save_filepath);
    return;
  }
  wxString spatial_type_string;
  spatial_type_string << layer_prop->GetSpatialType();
  myFile.Write(layer_filename.GetName() << _T("\n"));
  myFile.Write(spatial_type_string << _T("\n"));
  myFile.Write(out.GetString());
  myFile.Close();
}

void TocCtrl::OnMenuPropertiesLoad(wxCommandEvent &event) {
  wxDataViewItem item = GetSelection();
  if (!item.IsOk()){
    return;
  }

  auto node = TocCtrlModel::ConvertFromDataViewItem(item);
  if (node->IsContainer()){
    wxLogError("Unable to display symbology for container");
    return ;
  }

  wxString myLoadFilePathTxt = wxLoadFileSelector(_("Load symbology"), _T("tly"), wxEmptyString, this);
  if (myLoadFilePathTxt == wxEmptyString) {
    return;
  }
  // try to load the symbology file
  wxTextFile myFile;
  if (!myFile.Open(myLoadFilePathTxt)) {
    wxLogError(_("Unable to open symbology file: %s"), myLoadFilePathTxt);
    return;
  }

  // load selected layer information
  auto *layer_prop = node->m_LayerProp;
  wxASSERT(layer_prop);
  wxFileName myLayerName = layer_prop->GetName();
  wxString mySpatialType;
  mySpatialType << layer_prop->GetSpatialType();

  // check layer name stored
  wxString myLayerNameInTxtFile = myFile.GetFirstLine();
  if (myLayerNameInTxtFile != myLayerName.GetName()) {
    wxMessageDialog myDlg(
        this,
        wxString::Format(_T("This symbology was saved for layer: %s. Apply for selected layer?"), myLayerNameInTxtFile),
        _T("Different layer names"), wxYES_NO | wxCANCEL);
    if (myDlg.ShowModal() != wxID_YES) {
      return;
    }
  }
  // check geometry stored
  wxString myFileSpatialType = myFile.GetNextLine();
  if (myFileSpatialType != mySpatialType) {
    wxMessageDialog myDlg(this,
                          wxString::Format(_T("This symbology was saved for '%s'. Apply for '%s'?"),
                                           TM_GIS_SPATIAL_TYPES_STRING[wxAtoi(myFileSpatialType)],
                                           TM_GIS_SPATIAL_TYPES_STRING[wxAtoi(mySpatialType)]),
                          _T("Different spatial types"), wxYES_NO | wxCANCEL);
    if (myDlg.ShowModal() != wxID_YES) {
      return;
    }
  }
  // load serialisation
  wxString mySymbologyTxt = myFile.GetNextLine();
  if (mySymbologyTxt.IsEmpty()) {
    wxLogError(_("No symbology of empty symbology found in: %s"), myLoadFilePathTxt);
    return;
  }

  // escaping character are only needed for storing in the database.
  mySymbologyTxt.Replace(_T("\\\""), _T("\""));
  tmSerialize in(mySymbologyTxt);
  layer_prop->GetSymbolRuleManagerRef()->Serialize(in);

  // update display
  wxCommandEvent evt(tmEVT_LM_UPDATE, wxID_ANY);
  GetEventHandler()->QueueEvent(evt.Clone());

  // save new status
  wxCommandEvent evtSave(tmEVT_LM_TOC_EDITED, wxID_ANY);
  GetEventHandler()->QueueEvent(evtSave.Clone());
}

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

void TocCtrl::OnMenuShowLabels(wxCommandEvent &event) {
  wxDataViewItem item = GetSelection();
  if (!item.IsOk()) {
    return;
  }
  auto node = TocCtrlModel::ConvertFromDataViewItem(item);
  if (node == nullptr){
    return;
  }
  wxASSERT(node->m_LayerProp->GetSymbolRef());
  wxCommandEvent Evt(tmEVT_LM_SHOW_LABELS, wxID_ANY);
  Evt.SetClientData(item);
  GetEventHandler()->QueueEvent(Evt.Clone());
}

void TocCtrl::OnMenuEditing(wxCommandEvent &event) {
  wxDataViewItem item = GetSelection();
  if (!item.IsOk()) {
    return;
  }

  // start editing
  if (event.IsChecked()) {
    StopEditing(false);
    auto node = TocCtrlModel::ConvertFromDataViewItem(item);
    if (node->IsContainer()) {
      wxLogError("Unable to Edit container");
      return;
    }

    tmLayerProperties *layer_prop = node->m_LayerProp;
    wxASSERT(layer_prop);
    layer_prop->SetEditing(true);
    SetEditLayer(layer_prop);
    GetTocModel()->ItemChanged(item);

    // sent message
    wxCommandEvent evt(tmEVT_EM_EDIT_START, wxID_ANY);
    GetEventHandler()->QueueEvent(evt.Clone());
  } else {
    StopEditing(true);
  }
}

void TocCtrl::StopEditing(bool send_message) {
  // if no layer in edition, do nothing.
  if (!GetEditLayer()){
    return;
  }

  // search for the layer in edition and stop edition
  // we can only edit one layer at a time.
  bool reset = true;
  while (true){
    auto layer = IterateLayers(reset);
    reset = false;
    if (!layer){
      break;
    }
    if (layer == GetEditLayer()){
      layer->SetEditing(false);
      SetEditLayer(nullptr);
      TocCtrlModelNode * node = GetNodeFromLayerID(layer->GetID());
      wxASSERT(node);
      GetTocModel()->ItemChanged(TocCtrlModel::ConvertFromNode(node));
      break;
    }
  }

  if (send_message) {
    wxCommandEvent evt(tmEVT_EM_EDIT_STOP, wxID_ANY);
    GetEventHandler()->QueueEvent(evt.Clone());
  }
}

TocCtrlModelNode *TocCtrl::GetNodeFromLayerID(long layer_id) {
  auto root = TocCtrlModel::ConvertFromDataViewItem(GetTocModel()->GetRoot());
  TocCtrlModelNodePtrArray my_array_nodes;
  root->GetAllChildRecursive(my_array_nodes);
  for (auto node : my_array_nodes) {
    if (node->m_LayerProp->GetID() == layer_id){
      return node;
    }
  }
  return nullptr;
}

bool TocCtrl::IsAllParentLayerVisible(long layer_id) {
  auto node = GetNodeFromLayerID(layer_id);
  if (!node){
    wxLogError(_("Node: %ld not found!!"), layer_id);
    return false;
  }

  while (true){
    auto parent = node->GetParent();
    if  (parent == nullptr || parent == TocCtrlModel::ConvertFromDataViewItem(GetTocModel()->GetRoot())){
      break;
    }
    if (!parent->m_LayerProp->IsVisible()){
      return false;
    }
    node = parent;
  }
  return true;
}
