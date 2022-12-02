/***************************************************************************
 tmtoolmanager.h
 Take control of all tools (Dangling nodes, etc.)
 -------------------
 copyright            : (C) 2007 CREALP Lucien Schreiber
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef _TM_TOOLMANAGER_H_
#define _TM_TOOLMANAGER_H_

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "../database/database_tm.h"      // for database
#include "../gis/tmrenderer.h"            // for GIS rendering
#include "../gis/tmselecteddatamemory.h"  // for selection data
#include "../gis/tooldanglingnodes.h"     // for computing dangling nodes (engine class)
#include "danglingnodes_dlg.h"            // for dangling nodes dialog support

const int tmDANGLING_NODE_DRAW_SIZE = 4;
const wxColour tmDANGLING_NODE_DRAW_COLOUR = wxColour(*wxRED);

class tmToolManager : public wxEvtHandler {
 private:
  // defined by ctor
  wxWindow *m_Parent;
  tmSelectedDataMemory *m_Selected;
  tmRenderer *m_Renderer;
  tmGISScale *m_Scale;
  wxArrayRealPoints m_DanglingPts;
  DataBaseTM *m_pDB;

  void InitMemberValues();

  bool _IsOk();

  bool _SearchDanglingNodes(int selectedlayer, const wxArrayString &layersname);

  void _ClearDangling() {
    m_DanglingPts.Clear();
  }

 public:
  tmToolManager(wxWindow *parent, tmSelectedDataMemory *seldata, tmRenderer *renderer, tmGISScale *scale);

  ~tmToolManager();

  void SetDatabase(DataBaseTM *db) {
    m_pDB = db;
  }

  bool FindDanglingNodes();

  long DrawDanglingNodes(wxGCDC *dc);
};

#endif
