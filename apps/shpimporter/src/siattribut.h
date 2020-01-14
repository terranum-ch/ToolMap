/***************************************************************************
 siattribut.h
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
#ifndef _SIATTRIBUT_H_
#define _SIATTRIBUT_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include "database.h"
#include "databaseresult.h"

class siAttribut;

enum SIATTRIBUT_OPERATION {
  SIATTRIBUT_OPERATION_VALUE = 0,
  SIATTRIBUT_OPERATION_REPLACE,
  SIATTRIBUT_OPERATION_COPY,
  SIATTRIBUT_OPERATION_FIELD,
  SIATTRIBUT_OPERATION_UNKNOWN = -1
};

class siAttributValue {
 private:
  siAttribut* m_ParentAttribut;
  wxString m_ValueIn;
  wxString m_ValueOut;
  long m_ValueOutCode;

  bool _LoadOperationValue(const wxString& text, DataBase* database, long layerindex);
  bool _LoadOperationReplace(const wxString& text, DataBase* database, long layerindex);
  bool _GetCatalogDatabaseID(DataBase* database);

 public:
  siAttributValue(siAttribut* parent);
  virtual ~siAttributValue();
  void Reset();

  bool LoadFromText(const wxString& text, DataBase* database, long layerindex);
  inline const long GetValueOutCode() const;
  inline const wxString GetValueIn() const;
};

inline const long siAttributValue::GetValueOutCode() const {
  return m_ValueOutCode;
}

inline const wxString siAttributValue::GetValueIn() const {
  return m_ValueIn;
}

WX_DECLARE_OBJARRAY(siAttributValue*, siAttributValueArray);

/*************************************************************************************/ /**
 @brief Attribut class
 @author Lucien Schreiber copyright CREALP
 @date 04 janvier 2013
 *****************************************************************************************/
class siAttribut {
 private:
  wxString m_AttributNameIn;
  wxString m_AttributNameOut;
  wxArrayLong m_AttributFilterIDs;
  SIATTRIBUT_OPERATION m_AttributOperation;
  siAttributValueArray m_Values;
  long m_AttributIDReal;

  void _ClearAttributValueArray();
  long _GetIDForValueIn(const wxString& valueintxt);

 public:
  siAttribut();
  virtual ~siAttribut();

  bool LoadFromArray(const wxArrayString& attribtxt, DataBase* database, long layerindex);
  bool Process(OGRFeature* feature, DataBase* database, long layerindex, long databaseid, long kind);
  void Reset();

  inline const SIATTRIBUT_OPERATION GetAttributOperation() const;
  inline const wxString GetAttributNameIn() const;
  inline const wxString GetAttributNameOut() const;
  wxArrayLong* GetAttributFilterIDsRef();
  inline const long GetAttributIDReal() const;
};

inline const SIATTRIBUT_OPERATION siAttribut::GetAttributOperation() const {
  return m_AttributOperation;
}

inline const wxString siAttribut::GetAttributNameIn() const {
  return m_AttributNameIn;
}

inline const wxString siAttribut::GetAttributNameOut() const {
  return m_AttributNameOut;
}

inline const long siAttribut::GetAttributIDReal() const {
  return m_AttributIDReal;
}

WX_DECLARE_OBJARRAY(siAttribut*, siAttributArray);

#endif
