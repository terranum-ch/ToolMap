/***************************************************************************
 tmsymbolrule.h
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
#ifndef _TMSYMBOLRULE_H_
#define _TMSYMBOLRULE_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

// Include wxWidgets' headers
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/clrpicker.h>

#include "tmlayerpropertiesdef.h"
#include "../components/wxserialize/tmserialize.h"        // for object serialization

class tmSymbolVector;

class tmSymbolDLG;

class tmLayerProperties;

const int ID_SYMBOLEDIT_COLORCTRL = 10370;


class tmSymbolRule
{
private:
    TM_GIS_SPATIAL_TYPES m_SpatialType;
    wxString m_AttributFilter;
    tmSymbolVector *m_SymbolData;
    wxString m_RuleName;
    bool m_Active;

    int _GetRandomNumberForColor();

    tmSymbolVector *_NewSymbolVectorBasedOnSpatType(TM_GIS_SPATIAL_TYPES spatialtype);

public:
    tmSymbolRule(TM_GIS_SPATIAL_TYPES spatialtype, tmSymbolVector *symbol = NULL);

    virtual ~tmSymbolRule();

    tmSymbolRule(tmSymbolRule &source);

    wxBrush GetBrush();

    wxPen GetPen();

    bool IsOriented();

    inline const TM_GIS_SPATIAL_TYPES GetSpatialType() const;

    inline const bool IsActive() const;

    void SetActive(bool value);

    inline const wxString GetAttributFilter() const;

    void SetAttributFilter(wxString value);

    inline const wxString GetRuleName() const;

    void SetRuleName(wxString value);

    void SetRandomColor();

    static void InitRandomGenerator();

    inline tmSymbolVector *GetSymbolData();

    tmSymbolRule &operator=(tmSymbolRule &source);
};


inline const TM_GIS_SPATIAL_TYPES tmSymbolRule::GetSpatialType() const
{
    return m_SpatialType;
}


inline const bool tmSymbolRule::IsActive() const
{
    return m_Active;
}


inline const wxString tmSymbolRule::GetAttributFilter() const
{
    return m_AttributFilter;
}


inline const wxString tmSymbolRule::GetRuleName() const
{
    return m_RuleName;
}


inline tmSymbolVector *tmSymbolRule::GetSymbolData()
{
    return m_SymbolData;
}

WX_DECLARE_OBJARRAY(tmSymbolRule *, tmSymbolRuleArray);


/***************************************************************************//**
@brief Symbol Rule manager
@author Lucien Schreiber (c) CREALP 2012
@date 28 aout 2012
*******************************************************************************/
class tmSymbolRuleManager
{
private:
    tmSymbolRuleArray m_Rules;

    int m_DlgSelectedPanel;
    wxString m_DlgSelectedFieldname;
    tmLayerProperties *m_LayerProperties;

    void _CreateAttributIndex(tmLayerProperties *layerproperties);

public:
    tmSymbolRuleManager(tmLayerProperties *layerproperties);

    virtual ~tmSymbolRuleManager();

    inline tmSymbolRuleArray *GetRulesRef();

    bool ShowSymbolRuleDlg(wxWindow *parent, const wxPoint &position);

    bool IsUsingRules();

    wxString GetFieldName()
    { return m_DlgSelectedFieldname; }

    bool Serialize(tmSerialize &s);

    tmSymbolRuleManager &operator=(const tmSymbolRuleManager &source);

    // utilities functions
    static void RuleArrayClear(tmSymbolRuleArray *rules);

    static void RuleArrayCopy(const tmSymbolRuleArray *srcrules, tmSymbolRuleArray *dstrules);
};


inline tmSymbolRuleArray *tmSymbolRuleManager::GetRulesRef()
{
    return &m_Rules;
}


/*************************************************************************************//**
@brief Edit rule symbology dialog
@author Lucien Schreiber copyright CREALP
@date 29 aout 2012
*****************************************************************************************/
class tmSymbolRuleEdit_DLG : public wxDialog
{
private:
    tmSymbolRule *m_Rule;

    wxButton *m_ColourCtrl;
    wxTextCtrl *m_NameCtrl;
    wxTextCtrl *m_AttributeCtrl;
    wxCheckBox *m_EnabledCtrl;

    void _CreateControls();

    virtual bool TransferDataFromWindow();

    virtual bool TransferDataToWindow();

    void OnSymbologyEdit(wxCommandEvent &event);

DECLARE_EVENT_TABLE();

public:
    tmSymbolRuleEdit_DLG(wxWindow *parent, tmSymbolRule *rule, wxWindowID id = wxID_ANY,
                         const wxString &caption = _("Edit Symbology"), const wxPoint &pos = wxDefaultPosition,
                         const wxSize &size = wxDefaultSize);

    virtual ~tmSymbolRuleEdit_DLG();

    inline tmSymbolRule *GetRule() const;
};


inline tmSymbolRule *tmSymbolRuleEdit_DLG::GetRule() const
{
    return m_Rule;
}


#endif
