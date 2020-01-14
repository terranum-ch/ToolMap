//---------------------------------------------------------------------------
// $RCSfile: TreeMultiItemWindow.cpp $
// $Source: contrib/src/treemultictrl/TreeMultiItemWindow.cpp $
// $Revision: 1.84 $
// $Date: Jul 29, 2007 2:09:31 PM $
//---------------------------------------------------------------------------
// Author:      Jorgen Bodde
// Copyright:   (c) Jorgen Bodde
// License:     wxWidgets License
//---------------------------------------------------------------------------

#ifdef __GNUG__
#pragma implementation "TreeMultiItemWindow.cpp"
#endif

/* for compilers that support precompilation
   includes "wx/wx.h" */

#include <wx/wxprec.h>

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#include "TreeMultiItemWindow.h"

/** TreeMultiItemWindow
 * This class is a possible end node for the tree. If you want multiple
 * types of end-nodes you should copy this file and adjust it with a
 * different class name.
 */

TreeMultiItemWindow::TreeMultiItemWindow(TreeMultiItemNode *parent, const wxString &name)
    : TreeMultiItemBase(parent),
      _window(0),
      _topSpacing(0),
      _frontSpacing(0),
      _span(false) {
  _name = name;
}

TreeMultiItemWindow::~TreeMultiItemWindow() {
  if (_window) _window->Destroy();
}

void TreeMultiItemWindow::AssignWindow(wxWindow *wnd) {
  // delete previous, assign new (even if zero)
  if (_window) _window->Destroy();

  _window = wnd;

  // recalculate the size
  if (wnd) {
    wxSize size;

    // resize the sized control by sizer or
    // by single window. For a sizer handled window
    // GetBestSize returns the optimal size

    /// \todo This might need rivision for retaining the sizer size
    /// maybe we need to check whether size is <> 0,0 before get best size

    if (wnd->GetSizer())
      size = wnd->GetBestSize();
    else
      size = wnd->GetSize();

    _height = size.GetHeight();
    _width = size.GetWidth();

  } else {
    _height = 0;
    _width = 0;
  }
}

#if (CHECKBOXVIEW)

void TreeMultiItemWindow::SetCheckboxState(int state) {
  TreeMultiItemBase::SetCheckboxState(state);

  // enable or disable the window
  if (GetCheckbox() && GetWindow() && state != 2) GetWindow()->Enable(state == 1);
};

#endif  // #if(CHECKBOXVIEW)
