#ifndef TOOMAP_BITMAPS_H
#define TOOMAP_BITMAPS_H

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

class Bitmaps {
 public:
  static wxString SvgToolbar[];
  static wxString SvgLogo;
  static wxString ColorBlack;
  static wxString ColorWhite;
  enum ID_TOOLBAR {
    SELECT = 0, ZOOM_FIT,
  };
  static wxString GetColor();
  static wxBitmap GetToolbarBitmap(Bitmaps::ID_TOOLBAR id, const wxSize& size = wxSize(32, 32));
  static wxBitmap GetLogo(const wxSize& size = wxSize(32, 32));
};

#endif
