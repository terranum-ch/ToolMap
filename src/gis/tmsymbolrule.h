/***************************************************************************
 tmsymbolrule.h
 -------------------
 copyright            : (C) 2012 CREALP Lucien Schreiber 
 email                : lucien.schreiber at crealp dot vs dot ch
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef _TMSYMBOLRULE_H_
#define _TMSYMBOLRULE_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "tmlayerpropertiesdef.h"

class tmSymbolVector;

class tmSymbolRule {
  private:
    TM_GIS_SPATIAL_TYPES m_SpatialType;
    wxString m_AttributFilter;
    tmSymbolVector * m_SymbolData;
    wxString m_RuleName;
    bool m_Active;


    tmSymbolVector  * _NewSymbolVectorBasedOnSpatType(TM_GIS_SPATIAL_TYPES spatialtype);
    
  public:
    tmSymbolRule(TM_GIS_SPATIAL_TYPES spatialtype, tmSymbolVector * symbol = NULL);
    virtual ~tmSymbolRule();

    tmSymbolRule(tmSymbolRule & source);
    tmSymbolRule(const tmSymbolRule & source);

    wxBrush GetBrush();
    wxPen GetPen();

    inline const TM_GIS_SPATIAL_TYPES GetSpatialType() const;

    inline const bool IsActive() const;
    void SetActive(bool value);

    inline const wxString GetAttributFilter() const;
    void SetAttributFilter(wxString value);

    inline const wxString GetRuleName() const;
    void SetRuleName(wxString value);

    inline const tmSymbolVector * GetSymbolData() const;

    tmSymbolRule &operator = (const tmSymbolRule & source);
};


inline const TM_GIS_SPATIAL_TYPES tmSymbolRule::GetSpatialType() const {
  return m_SpatialType;
}



inline const bool tmSymbolRule::IsActive() const {
  return m_Active;
}



inline const wxString tmSymbolRule::GetAttributFilter() const {
  return m_AttributFilter;
}



inline const wxString tmSymbolRule::GetRuleName() const {
  return m_RuleName;
}



inline const tmSymbolVector * tmSymbolRule::GetSymbolData() const {
  return m_SymbolData;
}

WX_DECLARE_OBJARRAY(tmSymbolRule *, tmSymbolRuleArray);

#endif
