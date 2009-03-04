
#ifndef wxtreemultievent_header
#define wxtreemultievent_header

#include "wx/event.h"

#include "wxTreeMultiCtrl.h"


/**
   \section treemultievent Event handlers for wxTreeMultiCtrl
   
   Event handlers for wxTreeMultiCtrl are similar to wxTreeEvents. But they are
   optimized for the wxTreeMultiCtrl class.
*/

class WXDLLEXPORT wxTreeMultiEvent : public wxNotifyEvent
{
public:
 ///
 /// The default constructor
 ///
  wxTreeMultiEvent(wxEventType EventType=wxEVT_NULL, wxTreeMultiItem const& InitItem=wxTreeMultiItem());

 // inherited functions:
  // wxEvent:
   // cloning:
    virtual wxEvent* Clone() const {return new wxTreeMultiEvent(*this);}

 // item handling:
  wxTreeMultiItem GetItem(void) const
  {
    return this->m_Item;
  }
  void SetItem(wxTreeMultiItem const& NewItem)
  {
    this->m_Item = NewItem;
  }
  
 // old item handling:
  wxTreeMultiItem GetOldItem(void) const
  {
    return this->m_OldItem;
  }
  void SetOldItem(wxTreeMultiItem const& NewItem)
  {
    this->m_OldItem = NewItem;
  }
  
protected:
private:
 // variables:
  wxTreeMultiItem m_Item, m_OldItem;
  
 // class identification:
  DECLARE_DYNAMIC_CLASS(wxTreeMultiEvent)
};


// event handler function:
typedef void (wxEvtHandler::*wxTreeMultiEventFunction)(wxTreeMultiEvent&);

// event types:
BEGIN_DECLARE_EVENT_TYPES()
    DECLARE_EVENT_TYPE(wxEVT_COMMAND_TREE_MULTI_BEGIN_DRAG,        -1)
    DECLARE_EVENT_TYPE(wxEVT_COMMAND_TREE_MULTI_BEGIN_RDRAG,       -1)
    DECLARE_EVENT_TYPE(wxEVT_COMMAND_TREE_MULTI_BEGIN_LABEL_EDIT,  -1)
    DECLARE_EVENT_TYPE(wxEVT_COMMAND_TREE_MULTI_END_LABEL_EDIT,    -1)
    DECLARE_EVENT_TYPE(wxEVT_COMMAND_TREE_MULTI_DELETE_ITEM,       -1)
    DECLARE_EVENT_TYPE(wxEVT_COMMAND_TREE_MULTI_GET_INFO,          -1)
    DECLARE_EVENT_TYPE(wxEVT_COMMAND_TREE_MULTI_SET_INFO,          -1)
    DECLARE_EVENT_TYPE(wxEVT_COMMAND_TREE_MULTI_ITEM_EXPANDED,     -1)
    DECLARE_EVENT_TYPE(wxEVT_COMMAND_TREE_MULTI_ITEM_EXPANDING,    -1)
    DECLARE_EVENT_TYPE(wxEVT_COMMAND_TREE_MULTI_ITEM_COLLAPSED,    -1)
    DECLARE_EVENT_TYPE(wxEVT_COMMAND_TREE_MULTI_ITEM_COLLAPSING,   -1)
    DECLARE_EVENT_TYPE(wxEVT_COMMAND_TREE_MULTI_SEL_CHANGED,       -1)
    DECLARE_EVENT_TYPE(wxEVT_COMMAND_TREE_MULTI_SEL_CHANGING,      -1)
    DECLARE_EVENT_TYPE(wxEVT_COMMAND_TREE_MULTI_KEY_DOWN,          -1)
    DECLARE_EVENT_TYPE(wxEVT_COMMAND_TREE_MULTI_ITEM_ACTIVATED,    -1)
    DECLARE_EVENT_TYPE(wxEVT_COMMAND_TREE_MULTI_ITEM_RIGHT_CLICK,  -1)
    DECLARE_EVENT_TYPE(wxEVT_COMMAND_TREE_MULTI_ITEM_MIDDLE_CLICK, -1)
    DECLARE_EVENT_TYPE(wxEVT_COMMAND_TREE_MULTI_END_DRAG,          -1)
    DECLARE_EVENT_TYPE(wxEVT_COMMAND_TREE_MULTI_STATE_IMAGE_CLICK, -1)
    DECLARE_EVENT_TYPE(wxEVT_COMMAND_TREE_MULTI_ITEM_GETTOOLTIP,   -1)
    DECLARE_EVENT_TYPE(wxEVT_COMMAND_TREE_MULTI_ITEM_MENU,         -1)
END_DECLARE_EVENT_TYPES()

#define wxTreeMultiEventHandler(func) (wxObjectEventFunction)(wxEventFunction) wxStaticCastEvent(wxTreeMultiEventFunction, &func)


// standard events similar or identical to wxTreeEvent:
#define EVT_TREE_MULTI_BEGIN_DRAG (id,fn) wx__DECLARE_EVT1(wxEVT_COMMAND_TREE_BEGIN_DRAG, id,wxTreeMultiEventHandler(fn))
#define EVT_TREE_MULTI_BEGIN_RDRAG(id,fn) wx__DECLARE_EVT1(wxEVT_COMMAND_TREE_BEGIN_RDRAG,id,wxTreeMultiEventHandler(fn))
#define EVT_TREE_MULTI_END_DRAG   (id,fn) wx__DECLARE_EVT1(wxEVT_COMMAND_TREE_END_DRAG,   id,wxTreeMultiEventHandler(fn))
#define EVT_TREE_MULTI_BEGIN_LABEL_EDIT(id, fn) wx__DECLARE_EVT1(wxEVT_COMMAND_TREE_BEGIN_LABEL_EDIT, id, wxTreeMultiEventHandler(fn))
#define EVT_TREE_MULTI_END_LABEL_EDIT(id, fn) wx__DECLARE_EVT1(wxEVT_COMMAND_TREE_END_LABEL_EDIT, id, wxTreeMultiEventHandler(fn))
#define EVT_TREE_MULTI_GET_INFO(id, fn) wx__DECLARE_EVT1(wxEVT_COMMAND_TREE_GET_INFO, id, wxTreeMultiEventHandler(fn))
#define EVT_TREE_MULTI_SET_INFO(id, fn) wx__DECLARE_EVT1(wxEVT_COMMAND_TREE_SET_INFO, id, wxTreeMultiEventHandler(fn))
#define EVT_TREE_MULTI_ITEM_EXPANDED(id, fn) wx__DECLARE_EVT1(wxEVT_COMMAND_TREE_MULTI_ITEM_EXPANDED, id, wxTreeMultiEventHandler(fn))
#define EVT_TREE_MULTI_ITEM_EXPANDING(id, fn) wx__DECLARE_EVT1(wxEVT_COMMAND_TREE_MULTI_ITEM_EXPANDING, id, wxTreeMultiEventHandler(fn))
#define EVT_TREE_MULTI_ITEM_COLLAPSED(id, fn) wx__DECLARE_EVT1(wxEVT_COMMAND_TREE_MULTI_ITEM_COLLAPSED, id, wxTreeMultiEventHandler(fn))
#define EVT_TREE_MULTI_ITEM_COLLAPSING(id, fn) wx__DECLARE_EVT1(wxEVT_COMMAND_TREE_MULTI_ITEM_COLLAPSING, id, wxTreeMultiEventHandler(fn))
#define EVT_TREE_MULTI_SEL_CHANGED(id, fn) wx__DECLARE_EVT1(wxEVT_COMMAND_TREE_SEL_CHANGED, id, wxTreeMultiEventHandler(fn))
#define EVT_TREE_MULTI_SEL_CHANGING(id, fn) wx__DECLARE_EVT1(wxEVT_COMMAND_TREE_SEL_CHANGING, id, wxTreeMultiEventHandler(fn))
#define EVT_TREE_MULTI_KEY_DOWN(id, fn) wx__DECLARE_EVT1(wxEVT_COMMAND_TREE_KEY_DOWN, id, wxTreeMultiEventHandler(fn))
#define EVT_TREE_MULTI_DELETE_ITEM(id, fn) wx__DECLARE_EVT1(wxEVT_COMMAND_TREE_DELETE_ITEM, id, wxTreeMultiEventHandler(fn))
#define EVT_TREE_MULTI_ITEM_ACTIVATED(id, fn) wx__DECLARE_EVT1(wxEVT_COMMAND_TREE_MULTI_ITEM_ACTIVATED, id, wxTreeMultiEventHandler(fn))
#define EVT_TREE_MULTI_ITEM_MENU(id, fn) wx__DECLARE_EVT1(wxEVT_COMMAND_TREE_MULTI_ITEM_MENU, id, wxTreeMultiEventHandler(fn))
#define EVT_TREE_MULTI_ITEM_RIGHT_CLICK(id, fn) wx__DECLARE_EVT1(wxEVT_COMMAND_TREE_MULTI_ITEM_RIGHT_CLICK, id, wxTreeMultiEventHandler(fn))
#define EVT_TREE_MULTI_ITEM_MIDDLE_CLICK(id, fn) wx__DECLARE_EVT1(wxEVT_COMMAND_TREE_MULTI_ITEM_MIDDLE_CLICK, id, wxTreeMultiEventHandler(fn))
#define EVT_TREE_MULTI_STATE_IMAGE_CLICK(id, fn) wx__DECLARE_EVT1(wxEVT_COMMAND_TREE_STATE_IMAGE_CLICK, id, wxTreeMultiEventHandler(fn))
#define EVT_TREE_MULTI_ITEM_GETTOOLTIP(id, fn) wx__DECLARE_EVT1(wxEVT_COMMAND_TREE_MULTI_ITEM_GETTOOLTIP, id, wxTreeMultiEventHandler(fn))

#endif
