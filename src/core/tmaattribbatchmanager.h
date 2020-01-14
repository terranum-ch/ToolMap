/***************************************************************************
 tmaattribbatchmanager.h

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

#ifndef _TMAATTRIBBATCHMANAGER_H_
#define _TMAATTRIBBATCHMANAGER_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

// Include wxWidgets' headers
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "projectdefmemory.h"

class PrjDefMemManage;

class DataBaseTM;

class tmSelectedDataMemory;

class tmAAttribCtrl;

class tmAAttribBatchManager {
 private:
  PrjDefMemManage *m_Project;
  DataBaseTM *m_DB;
  tmSelectedDataMemory *m_Selected;
  PRJDEF_LAYERS_TYPE m_SelLayerType;

  bool _GetSelectionSubset(long layerid, wxArrayLong &subselected);

  wxString _CreateListOfIds(const wxArrayLong *ids);

 public:
  tmAAttribBatchManager(PrjDefMemManage *project, DataBaseTM *database, tmSelectedDataMemory *selected,
                        PRJDEF_LAYERS_TYPE sellayertype);

  virtual ~tmAAttribBatchManager();

  bool GetTypes(PrjMemObjectsArray &objects, wxArrayInt &number, wxArrayLong &layerid);

  bool GetFields(long layerid, PrjMemFieldArray &fields);

  tmAAttribCtrl *GetValueControl(const ProjectDefMemoryFields &field, wxWindow *wnd);

  bool IsOk();

  int Attribute(long layerid, const ProjectDefMemoryFields &field, const wxString &value);
};

#endif
