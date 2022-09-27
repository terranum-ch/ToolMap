//
// Created by Lucien Schreiber on 27.09.22.
//

#include "tocctrlmodel.h"
#include "bitmaps.h"

TocCtrlModel::TocCtrlModel() {
    m_root = new TocCtrlModelNode(NULL, "Group");

//    // setup pop music
//    m_pop = new TocCtrlModelNode(m_root, "Pop music");
//    m_pop->Append(
//            new TocCtrlModelNode(m_pop, "You are not alone", "Michael Jackson", 1995));
//    m_pop->Append(
//            new TocCtrlModelNode(m_pop, "Yesterday", "The Beatles", -1 /* not specified */ ));
//    m_pop->Append(
//            new TocCtrlModelNode(m_pop, "Take a bow", "Madonna", 1994));
//    m_root->Append(m_pop);
//
//    // setup classical music
//    m_classical = new TocCtrlModelNode(m_root, "Classical music");
//    m_ninth = new TocCtrlModelNode(m_classical, "Ninth symphony",
//                                   "Ludwig van Beethoven", 1824);
//    m_classical->Append(m_ninth);
//    m_classical->Append(new TocCtrlModelNode(m_classical, "German Requiem",
//                                             "Johannes Brahms", 1868));
//    m_root->Append(m_classical);
//
}

/*
void TocCtrlModel::AddToClassical( const wxString &title, const wxString &artist,
                                       unsigned int year )
{
    if (!m_classical)
    {
        wxASSERT(m_root);

        // it was removed: restore it
        m_classical = new TocCtrlModelNode( m_root, "Classical music" );
        m_root->Append( m_classical );

        // notify control
        wxDataViewItem child( (void*) m_classical );
        wxDataViewItem parent( (void*) m_root );
        ItemAdded( parent, child );
    }

    // add to the classical music node a new node:
    TocCtrlModelNode *child_node =
        new TocCtrlModelNode( m_classical, title, artist, year );
    m_classical->Append( child_node );

    // FIXME: what's m_classicalMusicIsKnownToControl for?
    if (m_classicalMusicIsKnownToControl)
    {
        // notify control
        wxDataViewItem child( (void*) child_node );
        wxDataViewItem parent( (void*) m_classical );
        ItemAdded( parent, child );
    }
}
 */

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

    // is the node one of those we keep stored in special pointers?
    if (node == m_pop)
        m_pop = NULL;
    else if (node == m_classical)
        m_classical = NULL;
    else if (node == m_ninth)
        m_ninth = NULL;

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
    m_pop = NULL;
    m_classical = NULL;
    m_ninth = NULL;

    while (!m_root->GetChildren().IsEmpty()) {
        TocCtrlModelNode *node = m_root->GetChildren().Last();
        m_root->GetChildren().Remove(node);
        delete node;
    }

    Cleared();
}

int TocCtrlModel::Compare(const wxDataViewItem &item1, const wxDataViewItem &item2,
                          unsigned int column, bool ascending) const {
    wxASSERT(item1.IsOk() && item2.IsOk());
    // should never happen

    if (IsContainer(item1) && IsContainer(item2)) {
        wxVariant value1, value2;
        GetValue(value1, item1, 0);
        GetValue(value2, item2, 0);

        wxString str1 = value1.GetString();
        wxString str2 = value2.GetString();
        int res = str1.Cmp(str2);
        if (res) return res;

        // items must be different
        wxUIntPtr litem1 = (wxUIntPtr) item1.GetID();
        wxUIntPtr litem2 = (wxUIntPtr) item2.GetID();

        return litem1 - litem2;
    }

    return wxDataViewModel::Compare(item1, item2, column, ascending);
}

void TocCtrlModel::GetValue(wxVariant &variant,
                            const wxDataViewItem &item, unsigned int col) const {
    wxASSERT(item.IsOk());
    TocCtrlModelNode *node = (TocCtrlModelNode *) item.GetID();
    variant = node->m_title;
    wxDataViewCheckIconText *myText = new wxDataViewCheckIconText(node->m_title, wxBitmapBundle::FromSVG(
            feature_toc_bitmaps::toc_shapefile, wxSize(16, 16)), wxCHK_CHECKED);

    wxVariant *mytitle = new wxVariant(node->m_title);
    wxVariant *mybmp = new wxVariant(wxBitmapBundle::FromSVG(feature_toc_bitmaps::toc_shapefile, wxSize(16, 16)));
    wxVariant *mychecked = new wxVariant(wxCHK_CHECKED);

//    wxVariantList myList;
//    myList.Append(mytitle);
//    myList.Append(mybmp);
//    myList.Append(mychecked);
//    variant = myList;

    /*

    switch (col)

    cse 0:
        variant = node->m_title;
        break;
    case 1:
        variant = node->m_artist;
        break;
    case 2:
        if (node->m_year != -1)
            variant = (long) node->m_year;
        break;
    case 3:
        variant = node->m_quality;
        break;
    case 4:
        variant = 80L;  // all music is very 80% popular
        break;
    case 5:
        if (node->m_year == -1)
            variant = "n/a";
        else if (node->m_year < 1900)
            variant = "old";
        else
            variant = "new";
        break;

    default:
        wxLogError( "TocCtrlModel::GetValue: wrong column %d", col );
    }*/
}

bool TocCtrlModel::SetValue(const wxVariant &variant,
                            const wxDataViewItem &item, unsigned int col) {
    wxASSERT(item.IsOk());


    TocCtrlModelNode *node = (TocCtrlModelNode *) item.GetID();
    node->m_title = variant.GetString();
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
