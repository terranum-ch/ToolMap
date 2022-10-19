//
// Created by Lucien Schreiber on 27.09.22.
//

#include "tocctrlmodel.h"
#include "bitmaps.h"

TocCtrlModel::TocCtrlModel() {
    m_image_list.Create(16,16, true, 4);
    m_image_list.Add(wxBitmapBundle::FromSVG(feature_toc_bitmaps::toc_folder, wxSize(16,16)).GetBitmap(wxSize(16,16)));
    m_image_list.Add(wxBitmapBundle::FromSVG(feature_toc_bitmaps::toc_shapefile, wxSize(16,16)).GetBitmap(wxSize(16,16)));
    m_image_list.Add(wxBitmapBundle::FromSVG(feature_toc_bitmaps::toc_database, wxSize(16,16)).GetBitmap(wxSize(16,16)));
    m_image_list.Add(wxBitmapBundle::FromSVG(feature_toc_bitmaps::toc_image, wxSize(16,16)).GetBitmap(wxSize(16,16)));

    m_root = new TocCtrlModelNode(NULL, "Group");
    TocCtrlModelNode * my_node = new TocCtrlModelNode(m_root, "Shapefile", true, 1);
    m_root->Append(my_node);
    TocCtrlModelNode * my_node2 = new TocCtrlModelNode(m_root, "Shapefile2", true, 1);
    m_root->Append(my_node2);
    TocCtrlModelNode * my_group2 = new TocCtrlModelNode(m_root, "Group2");
    m_root->Append(my_group2);

}

void TocCtrlModel::Delete(const wxDataViewItem &item) {
    TocCtrlModelNode *node = (TocCtrlModelNode *) item.GetID();
    if (!node)      // happens if item.IsOk()==false
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

void TocCtrlModel::GetValue(wxVariant &variant,
                            const wxDataViewItem &item, unsigned int col) const {
  wxASSERT(item.IsOk());
  TocCtrlModelNode *node = (TocCtrlModelNode *)item.GetID();
  wxDataViewIconText my_data;
  wxDataViewCheckIconText my_data_check;

  switch (col) {
    case 0:
      variant = node->m_title;
      break;
  }

  //  switch (col) {
//    case 0:
//      my_data.SetText(node->m_title);
//      my_data.SetIcon(m_image_list.GetIcon(node->m_image_index));
//      variant << my_data;
//      break;
//    case 1:
//      my_data_check.SetCheckedState(wxCHK_CHECKED);
//      if (!node->m_checked) {
//        my_data_check.SetCheckedState(wxCHK_UNCHECKED);
//      }
//      variant << my_data_check;
//      break;
//    default:
//      wxLogError("Column not managed!");
//      break;
//  }
}

bool TocCtrlModel::SetValue(const wxVariant &variant,
                            const wxDataViewItem &item, unsigned int col) {
    wxASSERT(item.IsOk());


    TocCtrlModelNode *node = (TocCtrlModelNode *) item.GetID();
    wxDataViewIconText mydata;
    wxDataViewCheckIconText mydata_check;

    switch (col) {
      case 0:
        node->m_title = variant.GetString();
      return true;
    }

    //    switch (col) {
//      case 0:
//        mydata << variant;
//        node->m_title = mydata.GetText();
//        node->m_image_index = 0;
//        for (int i = 0; i < m_image_list.GetImageCount(); i++) {
//          if (m_image_list.GetIcon(i).IsSameAs(mydata.GetIcon())) {
//            node->m_image_index = i;
//          }
//        }
//        return true;
//
//      case 1:
//        mydata_check << variant;
//        node->m_checked = true;
//        if (mydata_check.GetCheckedState() != wxCHK_CHECKED) {
//          node->m_checked = false;
//        }
//        return true;
//
//      default:
//        wxLogError("Column not managed!");
//        break;
//    }
    return false;
}

bool TocCtrlModel::IsEnabled(const wxDataViewItem &item,
                             unsigned int col) const {
    wxASSERT(item.IsOk());

    TocCtrlModelNode *node = (TocCtrlModelNode *) item.GetID();

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
    if (!item.IsOk())
        return wxDataViewItem(0);

    TocCtrlModelNode *node = (TocCtrlModelNode *) item.GetID();

//    // "MyMusic" also has no parent
//    if (node == m_root)
//        return wxDataViewItem(0);

    return wxDataViewItem((void *) node->GetParent());
}

bool TocCtrlModel::IsContainer(const wxDataViewItem &item) const {
    // the invisible root node can have children
    // (in our model always "MyMusic")
    if (!item.IsOk())
        return true;

    TocCtrlModelNode *node = (TocCtrlModelNode *) item.GetID();
    return node->IsContainer();
}

unsigned int TocCtrlModel::GetChildren(const wxDataViewItem &parent,
                                       wxDataViewItemArray &array) const {
    TocCtrlModelNode *node = (TocCtrlModelNode *) parent.GetID();
    if (!node) {
        array.Add(wxDataViewItem((void *) m_root));
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
        array.Add(wxDataViewItem((void *) child));
    }

    return count;
}
unsigned int TocCtrlModel::GetColumnCount() const {
  return 2;
}

wxString TocCtrlModel::GetColumnType(unsigned int) const {
  return wxEmptyString;
}
