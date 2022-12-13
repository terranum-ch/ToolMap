#ifndef TOOMAP_BITMAPS_H
#define TOOMAP_BITMAPS_H

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

class Bitmaps {
 public:
  static wxString SvgToolbar[];
  static wxString SvgMisc[];
  static wxString SvgToc[];
  static wxString SvgLogo;
  static wxString ColorBlack;
  static wxString ColorWhite;

  enum ID_TOOLBAR {
    SELECT = 0, ZOOM_FIT, ZOOM, PREVIOUS, PAN, EDIT, KIND, ATTRIBUTE, INFO, MODIFY, VERTEX_MOVE
  };
  enum ID_MISC {
    BUG = 0, ZIP, SORT_DOWN, SORT_UP
  };
  enum ID_TOC {
    FOLDER = 0, SHAPEFILE, DATABASE, IMAGE, CHECK_ON, CHECK_OFF, PEN, WEB
  };

  static wxString GetColor();

  static wxBitmap GetToolbarBitmap(Bitmaps::ID_TOOLBAR id, const wxSize& size = wxSize(32, 32));
  static wxBitmap GetMiscBitmap(Bitmaps::ID_MISC id, const wxSize& size = wxSize(32, 32));
  static wxBitmap GetTocBitmap(Bitmaps::ID_TOC id, const wxSize& size = wxSize(16, 16));
  static wxBitmap GetLogo(const wxSize& size = wxSize(32, 32));
};

#endif
