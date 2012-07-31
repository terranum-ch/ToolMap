//---------------------------------------------------------------------------
// $RCSfile: TreeMultiItemBase.cpp $
// $Source: contrib/src/treemultictrl/TreeMultiItemBase.cpp $
// $Revision: 1.84 $
// $Date: Jul 29, 2007 2:09:31 PM $
//---------------------------------------------------------------------------
// Author:      Jorgen Bodde
// Copyright:   (c) Jorgen Bodde
// License:     wxWidgets License
//---------------------------------------------------------------------------

#ifdef __GNUG__
    #pragma implementation "TreeMultiItemBase.cpp"
#endif

/* for compilers that support precompilation
   includes "wx/wx.h" */

#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include "TreeMultiItemBase.h"
#include "TreeMultiItemNode.h"

/** TreeMultiItemBase
  * This class is the node of the whole tree. All the other classes in the
  * tree descend from this node. It contains the type of the class without
  * using RTTI. NOTE: this class cannot be constructed directly
  */

TreeMultiItemBase::TreeMultiItemBase(TreeMultiItemNode *parent)
	: _excluded(false)

	, _x(0)
	, _y(0)
    , _width(0)
    , _height(0)

#if(CHECKBOXVIEW)
    , _checkbox(false)
    , _checkboxState(0)
#endif
    , _parent(parent)
    , m_Selected(false)
{
}

TreeMultiItemBase::~TreeMultiItemBase()
{
}

bool TreeMultiItemBase::IsVisible()
{
	// are we excluded? then we are not visible as well

	if(IsExcluded())
		return false;

	// check every parent up until the last and if one of them is collapsed
	// we are not visible

	TreeMultiItemNode *p = GetParent();
	while(p)
	{
		if(!p->IsExpanded())
			return false;
		else
			p = p->GetParent();
	}

	return true;
}
