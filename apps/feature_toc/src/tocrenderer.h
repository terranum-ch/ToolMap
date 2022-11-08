#ifndef FEATURE_TOC_TOCRENDERER_H
#define FEATURE_TOC_TOCRENDERER_H

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif
#include <wx/dataview.h>

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Used for data exchange between tocRenderer and TocCtrlModel
////////////////////////////////////////////////////////////////////////////////////////////////////
class tocRendererData : public wxVariantData {
 public:
  tocRendererData();
  virtual tocRendererData *Clone() const;
  virtual bool Eq(wxVariantData &data) const;
  virtual wxString GetType() const;

  wxString m_layer_name;
  int m_image_index;
  bool m_is_editing;
  bool m_is_visible;

 protected:
  virtual ~tocRendererData();
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Custom renderer for TocCtrl
////////////////////////////////////////////////////////////////////////////////////////////////////
class tocRenderer : public wxDataViewCustomRenderer {
 public:
  // This renderer can be either activatable or editable, for demonstration
  // purposes. In real programs, you should select whether the user should be
  // able to activate or edit the cell and it doesn't make sense to switch
  // between the two -- but this is just an example, so it doesn't stop us.
  explicit tocRenderer(wxDataViewCellMode mode, wxDataViewTreeCtrl *parent);

  virtual bool Render(wxRect rect, wxDC *dc, int state) wxOVERRIDE;

  virtual bool ActivateCell(const wxRect &cell, wxDataViewModel *model, const wxDataViewItem &item,
                            unsigned int WXUNUSED(col), const wxMouseEvent *mouseEvent) wxOVERRIDE;

  virtual wxSize GetSize() const wxOVERRIDE;

  virtual bool SetValue(const wxVariant &value) wxOVERRIDE;

  virtual bool GetValue(wxVariant &value) const wxOVERRIDE;

  virtual bool HasEditorCtrl() const wxOVERRIDE;

  virtual wxWindow *CreateEditorCtrl(wxWindow *parent, wxRect labelRect, const wxVariant &value) wxOVERRIDE;

  virtual bool GetValueFromEditorCtrl(wxWindow *ctrl, wxVariant &value) wxOVERRIDE;

  void SetColour(const wxColour & normal_col, const wxColour & dark_col);
  wxColour GetColourNormal ();
  wxColour GetColourDark ();

 private:
  wxString m_layer_name;
  int m_image_index;
  bool m_is_editing;
  bool m_is_visible;

  wxImageList m_image_list;
  wxDataViewCtrl * m_parent_ctrl;
  wxColour m_color_normal = *wxBLACK;
  wxColour m_color_dark = *wxWHITE;
  void _create_bitmaps();
};

#endif  // FEATURE_TOC_TOCRENDERER_H
