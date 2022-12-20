//---------------------------------------------------------------------------
// $RCSfile$
// $Source$
// $Revision$
// $Date$
//---------------------------------------------------------------------------
// Author:      Jorgen Bodde
// Copyright:   (c) Jorgen Bodde
// License:     wxWidgets License
//---------------------------------------------------------------------------

#ifdef __GNUG__
#pragma implementation "TreeMultiItemNode.cpp"
#endif

/* for compilers that support precompilation
   includes "wx/wx.h" */

#include <wx/wxprec.h>

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#include <wx/arrimpl.cpp>

#include "TreeMultiItemNode.h"
WX_DEFINE_OBJARRAY(TreeMultiItemBaseArray);

/** TreeMultiItemNode
 * This class is a container holder for multiple TreeMultiItemBase classes.
 * Since both a TreeMultiItemNode and a TreeMultiItemWindow are a descendant
 * from a TreeMultiItemBase class, it can hold multiple of these.
 * In the case of a directory tree structure, it can hold multiple
 * directories, and leafs (files).
 */

// default constructor for container composite
TreeMultiItemNode::TreeMultiItemNode(TreeMultiItemNode* parent, const wxString& caption, const wxString& name)
    : TreeMultiItemBase(parent),
      _caption(caption),
      _nodeExpanded(true) {
    _name = name;
    Clear();
}

//------------------------------------------------------------

TreeMultiItemNode::~TreeMultiItemNode() {
    // delete all items on this level
    Clear();
}

//------------------------------------------------------------

void TreeMultiItemNode::AddNode(TreeMultiItemBase* node) {
    if (node) _items.Add(node);
}

void TreeMultiItemNode::InsertNode(TreeMultiItemBase* NodePtr, size_t Position) {
    if (NodePtr != nullptr) this->_items.Insert(NodePtr, Position);
}

//------------------------------------------------------------

void TreeMultiItemNode::DeleteNode(TreeMultiItemBase* node) {
    // this is wrong. The RemoveAt and Remove should delete
    // the object
    if (node) {
        int index = Index(node);
        if (index >= 0) _items.Detach(index);

        delete node;
    }
}

//------------------------------------------------------------

void TreeMultiItemNode::DeleteNode(int index) {
    // this is wrong. The RemoveAt and Remove should delete
    // the object
    if (index < (int)_items.Count() && index >= 0) _items.RemoveAt(index);
}

//------------------------------------------------------------

int TreeMultiItemNode::GetNodeCount() const {
    return _items.Count();
}

//------------------------------------------------------------

TreeMultiItemBase* TreeMultiItemNode::GetNode(int index) const {
    TreeMultiItemBase* value = 0;

    if (index < (int)_items.Count() && index >= 0) value = &_items.Item(index);

    return value;
}

//------------------------------------------------------------

TreeMultiItemBase* TreeMultiItemNode::RemoveNode(TreeMultiItemBase* node) {
    if (node) {
        int index = _items.Index(*node);
        if (index >= 0) _items.Detach(index);
    }

    return node;
}

//------------------------------------------------------------

TreeMultiItemBase* TreeMultiItemNode::RemoveNode(int index) {
    TreeMultiItemBase* value = 0;

    if (index < (int)_items.Count() && index > 0) {
        value = &_items.Item(index);
        _items.Detach(index);
    }

    return value;
}

//------------------------------------------------------------

int TreeMultiItemNode::Index(TreeMultiItemBase* node, bool searchFromEnd) const {
    wxCHECK(node, -1);

    return _items.Index(*node, searchFromEnd);
}

//------------------------------------------------------------

void TreeMultiItemNode::Clear() {
    _items.Clear();
}

//------------------------------------------------------------

TreeMultiItemBase* TreeMultiItemNode::GetNodeNext(int& cookie) const {
    TreeMultiItemBase* value = 0;

    if (cookie >= 0 && cookie < (int)_items.Count()) {
        value = &_items[cookie];
        cookie++;
    }

    return value;
}

//------------------------------------------------------------

TreeMultiItemBase* TreeMultiItemNode::First() const {
    if (this->_items.GetCount() > 0)
        return &(this->_items[0]);
    else
        return nullptr;
} /* TreeMultiItemNode::First() const */

TreeMultiItemBase* TreeMultiItemNode::Last() const {
    if (this->_items.GetCount() > 0)
        return &(this->_items.Last());
    else
        return nullptr;
} /* TreeMultiItemNode::Last() const */

//------------------------------------------------------------

void TreeMultiItemNode::SetCaption(const wxString& caption) {
    /* this is an uncontrolled set, please make sure that
       the main wxTreeMultiCtrl gets a RedrawFromNode */
    _caption = caption;
}
