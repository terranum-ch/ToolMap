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

#ifndef __TREEMULTIITEMNODE_HPP_
#define __TREEMULTIITEMNODE_HPP_

#ifdef __GNUG__
    #pragma interface "TreeMultiItemNode.cpp"
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif
#include "wx/treebase.h"


#include <wx/dynarray.h>
#include "TreeMultiItemBase.h"
#include "TreeMultiItemWindow.h"

/** TreeMultiItemNode
  * This class is the element class for WX_OBJ_ARRAY and it
  * should be filled with the member variables and methods to
  * manipulate each object inside the object array.
  *
  * See wxArray for all base methods manipulating the array
  * Clear() can be used to safely delete all objects.
  */

// declare the array class here

WX_DECLARE_OBJARRAY(TreeMultiItemBase, TreeMultiItemBaseArray);

class TreeMultiItemNode : public TreeMultiItemBase
{
private:
    TreeMultiItemBaseArray _items;

    /** Caption of this node. */
    wxString _caption;

    /** Indicates if this node is expanded (true) or collapsed (false) */
    bool _nodeExpanded;

    /** visual attributes of the node */
    wxTreeItemAttr m_Attributes;

public:
    TreeMultiItemNode(TreeMultiItemNode *parent, const wxString &caption = wxEmptyString,
                      const wxString &name = wxEmptyString);

    virtual ~TreeMultiItemNode();

    // isClass method
    virtual TreeMultiItemNode *IsTreeMultiItemNode() const
    {
        return (TreeMultiItemNode *) this;
    };

    // add methods
    void AddNode(TreeMultiItemBase *node);

    void InsertNode(TreeMultiItemBase *NodePtr, size_t Position);

    // delete methods
    void Clear();

    void DeleteNode(TreeMultiItemBase *node);

    void DeleteNode(int index);

    TreeMultiItemBase *RemoveNode(TreeMultiItemBase *node);

    TreeMultiItemBase *RemoveNode(int index);

    // get methods
    int GetNodeCount() const;

    TreeMultiItemBase *GetNode(int index) const;

    int Index(TreeMultiItemBase *node, bool searchFromEnd = false) const;

    TreeMultiItemBase *GetNodeNext(int &cookie) const;

    TreeMultiItemBase *First() const;

    TreeMultiItemBase *Last() const;

    /** Fold function sets or clears the expanded flag. Note when excluded from drawing, this
        will have no effect on redraw */
    void Fold(bool expand)
    {
        if (_nodeExpanded != expand)
            _nodeExpanded = expand;
    };

    /** Return caption */
    const wxString &GetCaption() const
    {
        return _caption;
    };

    /** Set caption */
    void SetCaption(const wxString &caption);


    /** Returns true if the node is expanded. Subnodes can still be collapsed though */
    bool IsExpanded() const
    {
        return _nodeExpanded;
    };


};

#endif

