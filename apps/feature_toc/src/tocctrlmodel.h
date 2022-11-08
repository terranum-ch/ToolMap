#ifndef FEATURE_TOC_TOCCTRLMODEL_H
#define FEATURE_TOC_TOCCTRLMODEL_H

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include <wx/dataview.h>

class TocCtrlModelNode;

WX_DEFINE_ARRAY_PTR(TocCtrlModelNode *, TocCtrlModelNodePtrArray);

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Node for TocCtrlModel
////////////////////////////////////////////////////////////////////////////////////////////////////
class TocCtrlModelNode {
 public:
  TocCtrlModelNode(TocCtrlModelNode *parent, const wxString &title, bool checked, int image, bool editing);
  TocCtrlModelNode(TocCtrlModelNode *parent, const wxString &branch);

  ~TocCtrlModelNode();

  void GetAllChildRecursive(TocCtrlModelNodePtrArray &node_array, TocCtrlModelNode *startnode = nullptr);
  bool IsMyChildren(TocCtrlModelNode *node);

  bool IsContainer() const;

  TocCtrlModelNode *GetParent();

  TocCtrlModelNodePtrArray &GetChildren();

  void Insert(TocCtrlModelNode *child, unsigned int n);

  void Append(TocCtrlModelNode *child);

  unsigned int GetChildCount() const;

  // public to avoid getters/setters
  wxString m_title;
  bool m_checked;
  int m_image_index;
  bool m_editing;
  bool m_container;

 private:
  TocCtrlModelNode *m_parent;
  TocCtrlModelNodePtrArray m_children;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Model for TocCtrl
////////////////////////////////////////////////////////////////////////////////////////////////////
class TocCtrlModel : public wxDataViewModel {
 public:
  TocCtrlModel();

  ~TocCtrlModel() {
    delete m_root;
  }

  // helper functions
  bool IsChecked(const wxDataViewItem &item) const;
  void SetChecked(const wxDataViewItem &item, bool check = true);

  wxString NodeGetTitle(TocCtrlModelNode *node);
  bool NodeSetTitle(TocCtrlModelNode *node, const wxString &title);

  TocCtrlModelNode *NodeAdd(TocCtrlModelNode *parent, const wxString &branch);
  TocCtrlModelNode *NodeAdd(TocCtrlModelNode *parent, const wxString &title, bool checked, int image, bool editing);
  TocCtrlModelNode *NodeInsert(TocCtrlModelNode *parent, const wxString &title, bool checked, int image, bool editing,
                               int index = 0);
  bool NodeMove(TocCtrlModelNode *source, TocCtrlModelNode *destination, int proposed_index = wxNOT_FOUND);

  // model function
  void Delete(const wxDataViewItem &item);

  void Clear();

  virtual void GetValue(wxVariant &variant, const wxDataViewItem &item, unsigned int col) const wxOVERRIDE;

  virtual bool SetValue(const wxVariant &variant, const wxDataViewItem &item, unsigned int col) wxOVERRIDE;

  virtual bool IsEnabled(const wxDataViewItem &item, unsigned int col) const wxOVERRIDE;

  virtual wxDataViewItem GetParent(const wxDataViewItem &item) const wxOVERRIDE;

  wxDataViewItem GetRoot() const;

  virtual bool IsContainer(const wxDataViewItem &item) const wxOVERRIDE;

  virtual unsigned int GetChildren(const wxDataViewItem &parent, wxDataViewItemArray &array) const wxOVERRIDE;

  virtual unsigned int GetColumnCount() const wxOVERRIDE;
  virtual wxString GetColumnType(unsigned int) const wxOVERRIDE;

  static wxDataViewItem ConvertFromTocNode(const TocCtrlModelNode *node);
  static TocCtrlModelNode *ConvertFromwxDataViewItem(const wxDataViewItem &item);

 private:
  TocCtrlModelNode *m_root;
  wxImageList m_image_list;

  // pointers to some "special" nodes of the tree:
  //    TocCtrlModelNode *m_pop;
  //    TocCtrlModelNode *m_classical;
  //    TocCtrlModelNode *m_ninth;
};

#endif  // FEATURE_TOC_TOCCTRLMODEL_H
