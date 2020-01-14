/***************************************************************************
 silayer.h
 -------------------
 copyright            : (C) 2012 CREALP Lucien Schreiber
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef _SILAYER_H_
#define _SILAYER_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <wx/filename.h>
#include <wx/textfile.h>

#include "database.h"
#include "databaseresult.h"
#include "siattribut.h"
#include "sikind.h"

enum SILAYER_TYPE { SILAYER_TYPE_POINT = 0, SILAYER_TYPE_LINE, SILAYER_TYPE_POLYGON, SILAYER_TYPE_UNKNOWN = -1 };

class siProgressIndicator;
class siLayer {
 private:
  wxFileName m_RuleFileName;
  wxFileName m_LayerNameIn;
  long m_LayerIndexOut;
  DataBase* m_Database;
  siKind m_Kind;
  siAttributArray m_Attributs;
  SILAYER_TYPE m_LayerType;
  wxString m_LayerTypeName;
  long m_ProcessFeatureSkipped;
  siProgressIndicator* m_ProgressIndicator;

  int _GetEmptyBlockStop(int startpos);
  bool _LoadRuleIntoArray(int start, int stop, wxArrayString* array);
  bool _ProcessFeature(OGRFeature* feature);
  void _ClearAttributArray();

 public:
  siLayer(const wxString& layerpath, DataBase* database);
  virtual ~siLayer();

  bool LoadFromFile(const wxString& filename);
  int Process();

  const siKind GetKindRef();
  inline const SILAYER_TYPE GetLayerType() const;

  void SetProgressIndicator(siProgressIndicator* value);
};

inline const SILAYER_TYPE siLayer::GetLayerType() const {
  return m_LayerType;
}

#endif
