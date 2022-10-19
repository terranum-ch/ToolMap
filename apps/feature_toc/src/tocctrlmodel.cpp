#include "tocctrlmodel.h"

#include "bitmaps.h"
#include "toccustomrenderer.h"

TocCtrlModel::TocCtrlModel() {
  // TODO: Move this test code outside the model, maybe in the frame
  m_root = new TocCtrlModelNode(NULL, "Project");
  auto *my_group1 = new TocCtrlModelNode(m_root, "Construction");
  m_root->Append(my_group1);

  auto *my_node = new TocCtrlModelNode(my_group1, "Line", true, 2, true);
  my_group1->Append(my_node);
  auto *my_node2 = new TocCtrlModelNode(my_group1, "Point", false, 2, false);
  my_group1->Append(my_node2);


  auto *my_group2 = new TocCtrlModelNode(m_root, "Support");
  m_root->Append(my_group2);

  auto *my_node3 = new TocCtrlModelNode(my_group2, "Shapefile", false, 1, false);
  my_group2->Append(my_node3);
  auto *my_node4 = new TocCtrlModelNode(my_group2, "Raster", true, 3, false);
  my_group2->Append(my_node4);
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
