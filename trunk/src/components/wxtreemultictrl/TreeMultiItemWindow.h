//---------------------------------------------------------------------------
// $RCSfile: TreeMultiItemWindow.h $
// $Source: contrib/include/wx/treemultictrl/TreeMultiItemWindow.h $
// $Revision: 1.73 $
// $Date: Jan 4, 2006 6:42:02 PM $
//---------------------------------------------------------------------------
// Author:      Jorgen Bodde
// Copyright:   (c) Jorgen Bodde
// License:     wxWidgets License
//---------------------------------------------------------------------------

#ifndef __TREEMULTIITEMWINDOW_HPP_
#define __TREEMULTIITEMWINDOW_HPP_

#ifdef __GNUG__
    #pragma interface "TreeMultiItemWindow.cpp"
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include <wx/dynarray.h>

#include "TreeMultiItemBase.h"

class TreeMultiItemWindow : public TreeMultiItemBase
{
private:
	/** The simple control, or complex sizer that represents this window
		in the TreeMultiCtrl */
	wxWindow *_window;

	/** Top and front extra spacings, added to X, Y */
	int _topSpacing, _frontSpacing;

	/** Spanning flag */
	bool _span;

public:
	TreeMultiItemWindow(TreeMultiItemNode *parent, const wxString &name = wxEmptyString);
	virtual ~TreeMultiItemWindow();

	// IsClass method
	virtual TreeMultiItemWindow *IsTreeMultiItemWindow() const {
		return (TreeMultiItemWindow *)this;
	};
	
	/** Assigns the given window to this TreeMultiItemWindow, and also destroys the 
		present window. If the assigned window is 0, this item is not drawn
		and removed from the tree visually */
	void AssignWindow(wxWindow *wnd);

	/** Get window method */
	wxWindow *GetWindow() {
		return _window;
	};

	/* Sets front spacing */
	void SetFrontSpacing(int frontSpacing) {
		_frontSpacing = frontSpacing;
	};
	/* Gets front spacing */
	int GetFrontSpacing() const {
		return _frontSpacing;
	};
	/* Sets top spacing */
	void SetTopSpacing(int topSpacing) {
		_topSpacing = topSpacing;
	};
	/* Gets top spacing */
	int GetTopSpacing() const {
		return _topSpacing;
	};
	
	/* Sets horizontal span, meaning when this is set the window
	   attached may be resized horizontally when the window is resized */
	void SetHorizontalSpan(bool span) {
		_span = span;
	};

	/* Sets horizontal span, meaning when this is set the window
	   attached may be resized horizontally when the window is resized */
	bool GetHorizontalSpan() const {
		return _span;
	};

#if(CHECKBOXVIEW)
	/** Inherited from base, to enable or disable window */
	virtual void SetCheckboxState(int state);
#endif
};

#endif

