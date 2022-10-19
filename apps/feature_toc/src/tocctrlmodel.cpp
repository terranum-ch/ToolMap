//
// Created by Lucien Schreiber on 27.09.22.
//

#include "tocctrlmodel.h"
#include "bitmaps.h"

TocCtrlModel::TocCtrlModel() {
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
    TocCtrlModelNode *node = (TocCtrlModelNode *) item.GetID();
    // variant = node->m_title;
    wxDataViewCheckIconText my_data;
    my_data.SetText(node->m_title);
    my_data.SetCheckedState(wxCHK_CHECKED);
    if (!node->m_checked) {
        my_data.SetCheckedState(wxCHK_UNCHECKED);
    }

    switch (node->m_image_index) {
        case 0: // folder icon
            my_data.SetBitmapBundle(wxBitmapBundle::FromSVG(feature_toc_bitmaps::toc_folder, wxSize(16,16)));
            break;
        case 1: // shapefile icon
            my_data.SetBitmapBundle(wxBitmapBundle::FromSVG(feature_toc_bitmaps::toc_shapefile, wxSize(16,16)));
            break;
        case 2: // database icon
            my_data.SetBitmapBundle(wxBitmapBundle::FromSVG(feature_toc_bitmaps::toc_database, wxSize(16,16)));
            break;
        case 3: // image icon
            my_data.SetBitmapBundle(wxBitmapBundle::FromSVG(feature_toc_bitmaps::toc_image, wxSize(16,16)));
            break;
        default:
            wxLogError("This image index isn't supported!");
            break;
    }
    variant << my_data;

    if (col != 0) {
        wxLogError("TocCtrlModel::GetValue: wrong column %d", col);
    }
}

bool TocCtrlModel::SetValue(const wxVariant &variant,
                            const wxDataViewItem &item, unsigned int col) {
    wxASSERT(item.IsOk());


    TocCtrlModelNode *node = (TocCtrlModelNode *) item.GetID();
    wxDataViewCheckIconText mydata;
    mydata << variant;
    node->m_title = mydata.GetText();
    node->m_checked = true;
    if (mydata.GetCheckedState() != wxCHK_CHECKED) {
        node->m_checked = false;
    }


    return true;
    /*
    switch (col)
    {
        case 0:
            node->m_title = variant.GetString();
            return true;
        case 1:
            node->m_artist = variant.GetString();
            return true;
        case 2:
            node->m_year = variant.GetLong();
            return true;
        case 3:
            node->m_quality = variant.GetString();
            return true;

        default:
            wxLogError( "TocCtrlModel::SetValue: wrong column" );
    }
    return false;*/
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
  return 1;
}

wxString TocCtrlModel::GetColumnType(unsigned int) const {
  return wxEmptyString;
}
