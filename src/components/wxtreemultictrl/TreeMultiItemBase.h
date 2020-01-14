//---------------------------------------------------------------------------
// $RCSfile: TreeMultiItemBase.h $
// $Source: contrib/include/wx/treemultictrl/TreeMultiItemBase.h $
// $Revision: 1.84 $
// $Date: Jul 29, 2007 2:09:31 PM $
//---------------------------------------------------------------------------
// Author:      Jorgen Bodde
// Copyright:   (c) Jorgen Bodde
// License:     wxWidgets License
//---------------------------------------------------------------------------

#ifndef __TREEMULTIITEMBASE_HPP_
#define __TREEMULTIITEMBASE_HPP_

#ifdef __GNUG__
#pragma interface "TreeMultiItemBase.cpp"
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

// forward definitions
class TreeMultiItemRoot;

class TreeMultiItemWindow;

class TreeMultiItemNode;

class TreeMultiItemBase {
 protected:
  TreeMultiItemBase(TreeMultiItemNode *parent);

  /** Name property of this item, useful for assigning / coupling
      external variable reference to this item. It is not mandatory */
  wxString _name;

  /** Flag to indicate that this node is visible or not. The Node can be
      temporarily excluded by flagging it. This has also consequences for all
      the children of this node (if any) */
  bool _excluded;

  /** Type of instanced class */
  int _type;

  /** Calculated X, Y position */
  int _x, _y;

  /** Calculated width, height */
  int _width, _height;

#if (CHECKBOXVIEW)
  /** Checkbox flag (draw one or not) */
  bool _checkbox;

  /** Internal draw state. This is a unintelligent state, and should be
      updated when this node has some checked and some unchecked children
      then this should become a tri-stated item */
  int _checkboxState;
#endif

 public:
  virtual ~TreeMultiItemBase();

  // type of tree item

  int GetType() const {
    return _type;
  };

  TreeMultiItemNode *GetParent() const {
    return _parent;
  };

  /** Get name of this node */
  wxString GetName() const {
    return _name;
  };

  /** Set name of the item */
  void SetName(wxString const &NewName) {
    this->_name = NewName;
  }

  /** returns the instance pointer if the current node is
      a TreeMultiItemRoot, and NULL when it's not.
  */
  virtual TreeMultiItemRoot *IsTreeMultiItemRoot() const {
    return 0;
  };

  /** returns the instance pointer if the current node is
      a TreeMultiItemWindow, and NULL when it's not.
*/
  virtual TreeMultiItemWindow *IsTreeMultiItemWindow() const {
    return 0;
  };

  /** returns the instance pointer if the current node is
      a TreeMultiItemNode, and NULL when it's not.
*/
  virtual TreeMultiItemNode *IsTreeMultiItemNode() const {
    return 0;
  };

  /** Sets or resets the excluded flag. When excluded node is not visible */
  void SetExcluded(bool excluded) {
    _excluded = excluded;
  };

  /** Get / Set routine for X */
  void SetX(int x) {
    _x = x;
  };

  int GetX() const {
    return _x;
  };

  /** Get / Set routine for Y */
  void SetY(int y) {
    _y = y;
  };

  int GetY() const {
    return _y;
  };

  /** Get / Set routine for height */
  void SetHeight(int height) {
    _height = height;
  };

  int GetHeight() const {
    return _height;
  };

  /** Get / Set routine for width */
  void SetWidth(int width) {
    _width = width;
  };

  int GetWidth() const {
    return _width;
  };

  /** Returns true when this item is drawn somewhere in the
      tree. Whenever a parent of this node is collapsed, it
      is not visible and it is not necessary to perform i.e.
      redraw actions.

      It also returns false when this node is excluded from the
      tree.

      \sa wxTreeMultiCtrl::Exclude(), wxTreeMultiCtrl::Include())
  */
  bool IsVisible();

  /* Returns if this node is excluded from the tree. If this item
     is a Node, then all kids are excluded as well */
  bool IsExcluded() const {
    return _excluded;
  };

  //
  // item selection status handling

  /** checks if the item is selected */
  bool IsSelected(void) const {
    return this->m_Selected;
  }

  /** mark the current item as selected */
  void Select(void) {
    this->m_Selected = true;
  }

  /** toggle the selection status */
  void ToggleSelection(void) {
    this->m_Selected = !(this->m_Selected);
  }

  /** unmark the item */
  void Unselect(void) {
    this->m_Selected = false;
  }

#if (CHECKBOXVIEW)
  /** Sets checkbox or not. This does not influence the state of the checkbox */
  void SetCheckbox(bool value) {
    _checkbox = value;
  };

  /** Returns current state of the checkbox view */
  bool GetCheckbox() const {
    return _checkbox;
  };

  /** Sets checkbox state. If 0 it's unchecked, 1 = checked and 2 = tristate */
  virtual void SetCheckboxState(int state) {
    wxCHECK2(state < 3 && state >= 0, return );
    _checkboxState = state;
  };

  /** Returns current state of checkbox */
  int GetCheckboxState() const {
    return _checkboxState;
  };
#endif

 private:
  TreeMultiItemNode *_parent;

  // flags to indicate the status of the item:
  bool m_Selected;
};

#endif
