#include "tocctrlmodel.h"

#include "tocrenderer.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief TocCtrlModelNode
////////////////////////////////////////////////////////////////////////////////////////////////////

TocCtrlModelNode::TocCtrlModelNode(TocCtrlModelNode *parent, tmLayerProperties * layerprop) {
  m_Parent = parent;
  m_LayerProp = layerprop;
  m_Container = false;
  if (layerprop->GetType() == TOC_NAME_GROUP){
    m_Container = true;
  }
}

TocCtrlModelNode::TocCtrlModelNode(TocCtrlModelNode *parent, const wxString &folder, long id){
  m_Parent = parent;
  m_LayerProp = new tmLayerProperties();
  m_LayerProp->SetName(wxFileName("", folder));
  m_LayerProp->SetType(TOC_NAME_GROUP);
  m_LayerProp->SetID(id);
  m_Container = true;
}

TocCtrlModelNode::~TocCtrlModelNode() {
  // free all our children nodes
  size_t count = m_Children.GetCount();
  for (size_t i = 0; i < count; i++) {
    TocCtrlModelNode *child = m_Children[i];
    delete child;
  }
}

void TocCtrlModelNode::GetAllChildRecursive(TocCtrlModelNodePtrArray &node_array, TocCtrlModelNode *startnode) {
  TocCtrlModelNodePtrArray my_child_array;
  if (startnode == nullptr) {
    my_child_array = GetChildren();
  } else {
    wxASSERT(startnode);
    my_child_array = startnode->GetChildren();
  }
  for (unsigned int i = 0; i < my_child_array.GetCount(); i++) {
    TocCtrlModelNode *my_node = my_child_array[i];
    node_array.Add(my_node);
    if (my_node->IsContainer()) {
      GetAllChildRecursive(node_array, my_node);
    }
  }
}

bool TocCtrlModelNode::IsMyChildren(TocCtrlModelNode *node) {
  TocCtrlModelNodePtrArray my_array;
  GetAllChildRecursive(my_array, nullptr);
  for (unsigned int i = 0; i < my_array.GetCount(); i++) {
    if (my_array[i] == node) {
      return true;
    }
  }
  return false;
}

bool TocCtrlModelNode::TocCtrlModelNode::IsContainer() const {
  return m_Container;
}

TocCtrlModelNode *TocCtrlModelNode::GetParent() {
  return m_Parent;
}

TocCtrlModelNodePtrArray &TocCtrlModelNode::GetChildren() {
  return m_Children;
}

void TocCtrlModelNode::Insert(TocCtrlModelNode *child, unsigned int n) {
  m_Children.Insert(child, n);
}

void TocCtrlModelNode::Append(TocCtrlModelNode *child) {
  m_Children.Add(child);
}

unsigned int TocCtrlModelNode::GetChildCount() const {
  return m_Children.GetCount();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief TocCtrlModel
////////////////////////////////////////////////////////////////////////////////////////////////////
TocCtrlModel::TocCtrlModel() {
  m_Root = new TocCtrlModelNode(nullptr, "Project");
}

void TocCtrlModel::Delete(const wxDataViewItem &item) {
  TocCtrlModelNode *node = (TocCtrlModelNode *)item.GetID();
  if (!node)  // happens if item.IsOk()==false
    return;

  wxDataViewItem parent(node->GetParent());
  if (!parent.IsOk()) {
    wxASSERT(node == m_Root);

    // don't make the control completely empty:
    wxLogError("Cannot remove the root item!");
    return;
  }

  // first remove the node from the parent's array of children;
  // NOTE: MyMusicTreeModelNodePtrArray is only an array of _pointers_
  //       thus removing the node from it doesn't result in freeing it
  node->GetParent()->GetChildren().Remove(node);

  // free the node
  delete node;

  // notify control
  ItemDeleted(parent, item);
}

void TocCtrlModel::Clear() {
  while (!m_Root->GetChildren().IsEmpty()) {
    TocCtrlModelNode *node = m_Root->GetChildren().Last();
    m_Root->GetChildren().Remove(node);
    delete node;
  }
  Cleared();
}

void TocCtrlModel::GetValue(wxVariant &variant, const wxDataViewItem &item, unsigned int col) const {
  wxASSERT(item.IsOk());
  wxASSERT(col == 0);  // our toc ctrl has only one column

  auto *node = (TocCtrlModelNode *)item.GetID();
  auto *my_data = new tocRendererData();
  my_data->m_layer_name = node->m_LayerProp->GetName().GetName();
  switch (node->m_LayerProp->GetType()) {
    case TOC_NAME_GROUP:
      my_data->m_ImageIndex = 0;
      break;
    case TOC_NAME_SHP:
      my_data->m_ImageIndex = 1;
      break;
    case TOC_NAME_LINES:
    case TOC_NAME_POINTS:
    case TOC_NAME_LABELS:
    case TOC_NAME_ANNOTATIONS:
    case TOC_NAME_FRAME:
      my_data->m_ImageIndex = 2;
      break;
    case TOC_NAME_TIFF:
    case TOC_NAME_EGRID:
    case TOC_NAME_JPEG:
      my_data->m_ImageIndex = 3;
      break;
    case TOC_NAME_WEB:
      my_data->m_ImageIndex = 4;
      break;
    default:
      wxLogError("Layer type not supported!");
      break;
  }
  my_data->m_IsEditing = node->m_LayerProp->IsEditing();
  my_data->m_IsVisible = node->m_LayerProp->IsVisible();
  variant.SetData(my_data);
}

bool TocCtrlModel::SetValue(const wxVariant &variant, const wxDataViewItem &item, unsigned int col) {
  wxASSERT(item.IsOk());
  wxASSERT(col == 0);  // our toc ctrl has only one column

  auto *node = (TocCtrlModelNode *)item.GetID();
  auto *my_data = (tocRendererData *)variant.GetData();

  // not usefull to change type or layer name.
  node->m_LayerProp->SetVisible(my_data->m_IsVisible);
  node->m_LayerProp->SetEditing(my_data->m_IsEditing);
  return true;
}

bool TocCtrlModel::IsEnabled(const wxDataViewItem &item, unsigned int col) const {
  wxASSERT(item.IsOk());
  // allow editing allways
  return true;
}

wxDataViewItem TocCtrlModel::GetParent(const wxDataViewItem &item) const {
  // the invisible root node has no parent
  if (!item.IsOk()) {
    return wxDataViewItem(0);
  }

  auto *node = (TocCtrlModelNode *)item.GetID();
  return wxDataViewItem((void *)node->GetParent());
}

bool TocCtrlModel::IsContainer(const wxDataViewItem &item) const {
  // the invisible root node can have children
  if (!item.IsOk()) {
    return true;
  }

  TocCtrlModelNode *node = (TocCtrlModelNode *)item.GetID();
  return node->IsContainer();
}

unsigned int TocCtrlModel::GetChildren(const wxDataViewItem &parent, wxDataViewItemArray &array) const {
  TocCtrlModelNode *node = (TocCtrlModelNode *)parent.GetID();
  if (!node) {
    array.Add(wxDataViewItem((void *)m_Root));
    return 1;
  }

  if (node->GetChildCount() == 0) {
    return 0;
  }

  unsigned int count = node->GetChildren().GetCount();
  for (unsigned int pos = 0; pos < count; pos++) {
    TocCtrlModelNode *child = node->GetChildren().Item(pos);
    array.Add(wxDataViewItem((void *)child));
  }

  return count;
}

unsigned int TocCtrlModel::GetColumnCount() const {
  // only one column
  return 1;
}

wxString TocCtrlModel::GetColumnType(unsigned int) const {
  return "tocrenderer";
}

bool TocCtrlModel::IsChecked(const wxDataViewItem &item) const {
  auto *node = TocCtrlModel::ConvertFromDataViewItem(item);
  if (!node) {
    return false;
  }
  return node->m_LayerProp->IsVisible();
}

void TocCtrlModel::SetChecked(const wxDataViewItem &item, bool check) {
  auto *node = TocCtrlModel::ConvertFromDataViewItem(item);
  if (!node) {
    return;
  }
  node->m_LayerProp->SetVisible(check);
}

wxDataViewItem TocCtrlModel::GetRoot() const {
  return TocCtrlModel::ConvertFromNode(m_Root);
}

/// Adding a container node
/// \param parent parent node
/// \param branch container name
/// \return the newly TocCtrlModelNode or a null pointer in case of error
TocCtrlModelNode *TocCtrlModel::NodeAdd(TocCtrlModelNode *parent, const wxString &branch) {
  // check that the node is a container or abort
  if (!parent->IsContainer()) {
    wxLogError("Parent node isn't a container, adding folder not possible!");
    return nullptr;
  }

  auto *my_group1 = new TocCtrlModelNode(parent, branch);
  parent->Append(my_group1);
  ItemAdded(wxDataViewItem((void *)parent), wxDataViewItem((void *)my_group1));
  return my_group1;
}

/// Append an item
/// \param parent (must be a container)
/// \param title
/// \param checked
/// \param image zero based image index (0 = folder, 1 = shapefile, 2 = database, 3 = image, 4= web)
/// \param editing
/// \return the newly TocCtrlModelNode or a null pointer in case of error
TocCtrlModelNode *TocCtrlModel::NodeAdd(TocCtrlModelNode *parent, tmLayerProperties * layerprop) {
  // check that the node is a container or abort
  if (!parent->IsContainer()) {
    wxLogError("Parent node isn't a container, adding item not possible!");
    return nullptr;
  }

  TocCtrlModelNode *item = nullptr;
  item = new TocCtrlModelNode(parent, layerprop);

  parent->Append(item);
  ItemAdded(wxDataViewItem((void *)parent), wxDataViewItem((void *)item));
  return item;
}

TocCtrlModelNode *TocCtrlModel::NodeInsert(TocCtrlModelNode *parent, const wxString &branch, int index) {
  // check that the node is a container or abort
  if (!parent->IsContainer()) {
    wxLogError("Parent node isn't a container, adding item not possible!");
    return nullptr;
  }

  auto *my_group1 = new TocCtrlModelNode(parent, branch);
  parent->Insert(my_group1, index);
  ItemAdded(wxDataViewItem((void *)parent), wxDataViewItem((void *)my_group1));
  return my_group1;
}

TocCtrlModelNode *TocCtrlModel::NodeInsert(TocCtrlModelNode *parent, tmLayerProperties * layerprop, int index) {
  // check that the node is a container or abort
  if (!parent->IsContainer()) {
    wxLogError("Parent node isn't a container, adding item not possible!");
    return nullptr;
  }

  auto *my_item = new TocCtrlModelNode(parent, layerprop);
  parent->Insert(my_item, index);
  ItemAdded(wxDataViewItem((void *)parent), wxDataViewItem((void *)my_item));
  return my_item;
}

wxDataViewItem TocCtrlModel::ConvertFromNode(const TocCtrlModelNode *node) {
  return wxDataViewItem((void *)node);
}

TocCtrlModelNode *TocCtrlModel::ConvertFromDataViewItem(const wxDataViewItem &item) {
  return (TocCtrlModelNode *)item.GetID();
}

/// Get The node text / title
/// \param node
/// \return the text for nodes and group
wxString TocCtrlModel::NodeGetTitle(TocCtrlModelNode *node) {
  if (node->IsContainer()) {
    return node->m_LayerProp->GetName().GetName();
  }
  return wxEmptyString;
}

/// Set node text / title
/// \param node
/// \param title
/// \return true for Container, false otherwise. It makes no sense to change item title!
bool TocCtrlModel::NodeSetTitle(TocCtrlModelNode *node, const wxString &title) {
  if (!node->IsContainer() || title.IsEmpty()) {
    return false;
  }
  node->m_LayerProp->SetName(wxFileName("", title));
  ItemChanged(TocCtrlModel::ConvertFromNode(node));
  return true;
}

/// Move a node or a container from source to destination.
/// \param source
/// \param destination
/// \param proposed_index wxNOT_FOUND means move to the end, otherwise try to move to the specified index of the
/// destination container
/// \return allways true
bool TocCtrlModel::NodeMove(TocCtrlModelNode *source, TocCtrlModelNode *destination, int proposed_index) {
  wxASSERT(source);
  wxASSERT(destination);

  // ensure that destination is a container
  TocCtrlModelNode *real_destination = destination;
  if (!destination->IsContainer()) {
    real_destination = destination->GetParent();
  }

  // ensure index isn't out of bounds
  int move_index = proposed_index;
  if (move_index != wxNOT_FOUND) {
    if (real_destination->GetChildCount() < move_index) {
      wxLogMessage("Changing index, child count = %d", real_destination->GetChildCount());
      move_index = wxNOT_FOUND;
    }
  }

  // moving container
  if (source->IsContainer()) {
    TocCtrlModelNode *new_container = nullptr;
    if (move_index == wxNOT_FOUND) {  // add to the end
      new_container = NodeAdd(real_destination, source->m_LayerProp->GetName().GetName());
    } else {  // insert
      new_container = NodeInsert(real_destination, source->m_LayerProp->GetName().GetName(), move_index);
    }
    NodeRecursiveAdd(new_container, source);
    NodeRecursiveRemove(source);
    Delete(TocCtrlModel::ConvertFromNode(source));
    return true;
  }

  // moving node
  if (move_index == wxNOT_FOUND) {
    NodeAdd(real_destination, source->m_LayerProp);
  } else {
    NodeInsert(real_destination, source->m_LayerProp,move_index);
  }
  Delete(ConvertFromNode(source));
  return true;
}

void TocCtrlModel::NodeRecursiveAdd(TocCtrlModelNode *parent, TocCtrlModelNode *start) {
  wxASSERT(parent);
  wxASSERT(start);
  TocCtrlModelNodePtrArray childs = start->GetChildren();
  for (unsigned int i = 0; i < childs.GetCount(); i++) {
    TocCtrlModelNode *item = childs[i];
    if (!item->IsContainer()) {
      NodeAdd(parent, item->m_LayerProp);
    } else {
      TocCtrlModelNode *group = NodeAdd(parent, item->m_LayerProp->GetName().GetName());
      NodeRecursiveAdd(group, item);
    }
  }
}

void TocCtrlModel::NodeRecursiveRemove(TocCtrlModelNode *start) {
  TocCtrlModelNodePtrArray my_array = start->GetChildren();
  for (int i = my_array.GetCount() - 1; i >= 0; i--) {
    TocCtrlModelNode *item = my_array[i];
    wxASSERT(item);
    if (!item->IsContainer()) {
      Delete(TocCtrlModel::ConvertFromNode(item));
    } else {
      NodeRecursiveRemove(item);
    }
  }
}
