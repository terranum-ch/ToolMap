#include "tocctrlmodel.h"

#include "bitmaps.h"
#include "tocrenderer.h"

TocCtrlModel::TocCtrlModel() {
  m_root = new TocCtrlModelNode(nullptr, "Project");
}

void TocCtrlModel::Delete(const wxDataViewItem &item) {
  TocCtrlModelNode *node = (TocCtrlModelNode *)item.GetID();
  if (!node)  // happens if item.IsOk()==false
    return;

  wxDataViewItem parent(node->GetParent());
  if (!parent.IsOk()) {
    wxASSERT(node == m_root);

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
  while (!m_root->GetChildren().IsEmpty()) {
    TocCtrlModelNode *node = m_root->GetChildren().Last();
    m_root->GetChildren().Remove(node);
    delete node;
  }

  Cleared();
}

void TocCtrlModel::GetValue(wxVariant &variant, const wxDataViewItem &item, unsigned int col) const {
  wxASSERT(item.IsOk());
  wxASSERT(col == 0);  // our toc ctrl has only one column

  auto *node = (TocCtrlModelNode *)item.GetID();
  auto *my_data = new tocRendererData();
  my_data->m_layer_name = node->m_title;
  my_data->m_image_index = node->m_image_index;
  my_data->m_is_visible = node->m_checked;
  my_data->m_is_editing = node->m_editing;
  variant.SetData(my_data);
}

bool TocCtrlModel::SetValue(const wxVariant &variant, const wxDataViewItem &item, unsigned int col) {
  wxASSERT(item.IsOk());
  wxASSERT(col == 0);  // our toc ctrl has only one column

  auto *node = (TocCtrlModelNode *)item.GetID();
  auto *my_data = (tocRendererData *)variant.GetData();

  node->m_title = my_data->m_layer_name;
  node->m_checked = my_data->m_is_visible;
  node->m_image_index = my_data->m_image_index;
  node->m_editing = my_data->m_is_editing;

  return true;
}

bool TocCtrlModel::IsEnabled(const wxDataViewItem &item, unsigned int col) const {
  wxASSERT(item.IsOk());

  TocCtrlModelNode *node = (TocCtrlModelNode *)item.GetID();

  //    // disable Beethoven's ratings, his pieces can only be good
  //    if (col == 3 && node->m_artist.EndsWith("Beethoven"))
  //        return false;
  //
  //    // also disable editing the year when it's not specified, this doesn't work
  //    // because the editor needs some initial value
  //    if (col == 2 && node->m_year == -1)
  //        return false;

  // otherwise allow editing
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
    array.Add(wxDataViewItem((void *)m_root));
    return 1;
  }

  //    if (node == m_classical)
  //    {
  //        TocCtrlModel* model = const_cast<TocCtrlModel*>(this);
  //    }

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
  return 1;
}

wxString TocCtrlModel::GetColumnType(unsigned int) const {
  return "tocrenderer";
}

bool TocCtrlModel::IsChecked(const wxDataViewItem &item) const {
  auto *node = (TocCtrlModelNode *)item.GetID();
  if (!node) {
    return false;
  }
  return node->m_checked;
}

void TocCtrlModel::SetChecked(const wxDataViewItem &item, bool check) {
  auto *node = (TocCtrlModelNode *)item.GetID();
  if (!node) {
    return;
  }

  node->m_checked = check;
}

wxDataViewItem TocCtrlModel::GetRoot() const {
  return wxDataViewItem((void *)m_root);
}

/// Adding a branch node
/// \param parent parent node
/// \param branch branch name
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
TocCtrlModelNode *TocCtrlModel::NodeAdd(TocCtrlModelNode *parent, const wxString &title, bool checked, int image,
                                        bool editing) {
  // check that the node is a container or abort
  if (!parent->IsContainer()) {
    wxLogError("Parent node isn't a container, adding item not possible!");
    return nullptr;
  }

  auto *my_item = new TocCtrlModelNode(parent, title, checked, image, editing);
  parent->Append(my_item);
  ItemAdded(wxDataViewItem((void *)parent), wxDataViewItem((void *)my_item));
  return my_item;
}

TocCtrlModelNode *TocCtrlModel::NodeInsert(TocCtrlModelNode *parent, const wxString &title, bool checked, int image,
                                           bool editing, int index) {
  // check that the node is a container or abort
  if (!parent->IsContainer()) {
    wxLogError("Parent node isn't a container, adding item not possible!");
    return nullptr;
  }

  auto *my_item = new TocCtrlModelNode(parent, title, checked, image, editing);
  parent->Insert(my_item, index);
  ItemAdded(wxDataViewItem((void *)parent), wxDataViewItem((void *)my_item));
  return my_item;
}

wxDataViewItem TocCtrlModel::ConvertFromTocNode(const TocCtrlModelNode *node) {
  return wxDataViewItem((void *)node);
}

TocCtrlModelNode *TocCtrlModel::ConvertFromwxDataViewItem(const wxDataViewItem &item) {
  return (TocCtrlModelNode *)item.GetID();
}

/// Get The node text / title
/// \param node
/// \return the text for nodes and group
wxString TocCtrlModel::NodeGetTitle(TocCtrlModelNode *node) {
  if (node->IsContainer()) {
    return node->m_title;
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
  node->m_title = title;
  ItemChanged(TocCtrlModel::ConvertFromTocNode(node));
  return true;
}

/// Move a node from source to destination.
/// \param source
/// \param destination
/// \param proposed_index wxNOT_FOUND means move to the end, otherwise try to move to the specified index of the
/// destination container \return
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

  if (move_index == wxNOT_FOUND) {
    NodeAdd(real_destination, source->m_title, source->m_checked, source->m_image_index, source->m_editing);
  } else {
    NodeInsert(real_destination, source->m_title, source->m_checked, source->m_image_index, source->m_editing,
               move_index);
  }
  Delete(ConvertFromTocNode(source));
  return true;
}
