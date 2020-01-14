/***************************************************************************
 tmlayerproperties.cpp
                    Class used for managing layer informations
 -------------------
 copyright : (C) 2007 CREALP Lucien Schreiber
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

// comment doxygen

#include "tmlayerproperties.h"
#include "tmsymbol.h"
#include "tmsymbolrule.h"


/***************************************************************************//**
 @brief Default values for tmLayerProperties
 @author Lucien Schreiber (c) CREALP 2008
 @date 07 July 2008
 *******************************************************************************/
void tmLayerProperties::InitMemberValues()
{
    m_LayerID = 0;
    m_LayerSpatialType = LAYER_SPATIAL_LINE;
    m_LayerVisible = true;
    m_LayerType = TOC_NAME_NOT_GENERIC;
    m_LayerSymbol = NULL;
    m_LayerVertexFlags = 0;
    m_LayerEditing = false;
    m_SymbolRulesManager = new tmSymbolRuleManager(this);
    m_LabelIsVisible = false;
    m_LabelDefinition = wxEmptyString;
}


/***************************************************************************//**
 @brief Init from a string array
 @details This may be used for initing an #tmLayerProperties object from a
 string array. No verifications are made on the array size
 @param array string array containing 6 values
 @return  allways TRUE
 @author Lucien Schreiber (c) CREALP 2008
 @date 10 July 2008
 *******************************************************************************/
bool tmLayerProperties::InitFromArray(const wxArrayString &array, bool userelativepath, const wxString &prjpath)
{
    long temptype = 0;
    long tempstatus = 0;
    long tempgeneric = 0;
    long templabel = 0;

    array.Item(0).ToLong(&m_LayerID);
    array.Item(1).ToLong(&temptype);
    m_LayerSpatialType = (TM_GIS_SPATIAL_TYPES) temptype;

    m_LayerName.Assign(array.Item(2), array.Item(3));

    array.Item(4).ToLong(&tempstatus);
    m_LayerVisible = (bool) tempstatus;

    array.Item(5).ToLong(&tempgeneric);
    m_LayerType = static_cast<TOC_GENERIC_NAME> (tempgeneric);

    InitSymbology(array.Item(6));

    wxString myVFlags = array.Item(7);
    m_LayerVertexFlags = wxAtoi(myVFlags.c_str());

    array.Item(8).ToLong(&templabel);
    if (templabel == 1) {
        m_LabelIsVisible = true;
    } else {
        m_LabelIsVisible = false;
    }
    m_LabelDefinition = array.Item(9);

    // relative path stuff only for support layer
    if (m_LayerType < TOC_NAME_NOT_GENERIC) {
        return true;
    }

    // try to find support file
    // 1. check if file exists
    // 2. Try to convert to absolute
    // 3. check if converted exists
    // if all fails revert to saved data.
    wxFileName myTempFile(m_LayerName);
    if (myTempFile.Exists() == true) {
        return true;
    }

    if (myTempFile.MakeAbsolute(prjpath) == false) {
        wxLogMessage(_T("Converting '%' to absolute path isn't possible"),
                     myTempFile.GetFullPath());
    } else {
        if (myTempFile.Exists() == true) {
            m_LayerName.Assign(myTempFile);
        } else {
            wxLogMessage(_("%s not found (relative or absolute!)"), myTempFile.GetFullPath());
        }
    }
    return true;

}


void tmLayerProperties::InitSymbology(const wxString &itemBinSymbology)
{
    // creating symbology object
    if (m_LayerSymbol) {
        wxLogError(_T("m_LayerSymbol allready defined, check code !"));
        return;
    }

    m_LayerSymbol = tmSymbol::CreateSymbolBasedOnType(GetSpatialType(), GetType());

    // unserialize object
    if (m_LayerSymbol && !itemBinSymbology.IsEmpty()) {
        //wxLogDebug(itemBinSymbology);
        tmSerialize in(itemBinSymbology);
        GetSymbolRuleManagerRef()->Serialize(in);
        //m_LayerSymbol->Serialize(in);
    }
}


tmLayerProperties::~tmLayerProperties()
{
    wxDELETE(m_LayerSymbol);
    wxDELETE(m_SymbolRulesManager);
}


/***************************************************************************//**
 @brief Init name, path and type member
 @details Init member based on path, name (with extension) and a list of
 supported GIS extension. Those extensions may be retreived using the function
 bellow.
 @code
 wxArrayString myAllExt = tmGISData::GetAllSupportedGISFormatsExtensions();
 @endcode
 @param path string containing the path
 @param nameext string containing the name and extension
 @param supportedext an array of all supported extension. See code above
 @return  TRUE if name and path aren't empty
 @author Lucien Schreiber (c) CREALP 2008
 @date 06 August 2008
 *******************************************************************************/
bool tmLayerProperties::InitFromPathAndName(const wxString &path,
                                            const wxString &nameext,
                                            const wxArrayString &supportedext)
{
    if (nameext.IsEmpty()) {
        return false;
    }

    m_LayerName.Assign(path, nameext);

    // init extension based on supported extension, TOC_NAME_UNKNOWN otherwise
    wxFileName myfileName(nameext);
    wxString myExt = myfileName.GetExt().MakeLower();

    for (unsigned int i = 0; i < supportedext.GetCount(); i++) {
        if (supportedext.Item(i).Contains(myExt)) {
            int myTempType = TOC_NAME_NOT_GENERIC + i + 1;
            m_LayerType = static_cast<TOC_GENERIC_NAME> (myTempType);
            break;
        }
    }

    // nothing found -> unknown
    if (m_LayerType == TOC_NAME_NOT_GENERIC) {
        m_LayerType = TOC_NAME_UNKNOWN;
    }

    return TRUE;
}




/*wxString tmLayerProperties::GetFileExtension()
{

 return filename.GetExt();
}*/



/***************************************************************************//**
 @brief Return the display name for using in the TOC
 @details Because in some case file name isn't very explicit we sould use this
 function for getting a good display name (exemple is ESRI binary grid)
 @return  A more explicit name
 @author Lucien Schreiber (c) CREALP 2008
 @date 06 August 2008
 *******************************************************************************/
wxString tmLayerProperties::GetNameDisplay()
{
    wxASSERT(m_LayerName.IsOk());

    wxString myWebName = wxEmptyString;

    // special behaviour for ESRI grid.
    switch (m_LayerType) {
        case TOC_NAME_EGRID:
            return m_LayerName.GetDirs().Last();
            break;

        case TOC_NAME_WEB:
            myWebName = m_LayerName.GetName();
            myWebName.Replace(_T("_"), _T(" "));
            return myWebName;
            break;

        case TOC_NAME_LINES:
        case TOC_NAME_POINTS:
        case TOC_NAME_LABELS:
        case TOC_NAME_ANNOTATIONS:
        case TOC_NAME_FRAME:
            return TOC_GENERIC_NAME_STRING[m_LayerType];
            break;

        default:
            break;
    }
    return m_LayerName.GetFullName();
}


wxFileName tmLayerProperties::GetName()
{
    return m_LayerName;
}


void tmLayerProperties::SetID(long value)
{
    m_LayerID = value;
}

void tmLayerProperties::SetSpatialType(TM_GIS_SPATIAL_TYPES value)
{
    m_LayerSpatialType = value;
}

void tmLayerProperties::SetVisible(bool value)
{
    m_LayerVisible = value;
}

void tmLayerProperties::SetType(TOC_GENERIC_NAME value)
{
    m_LayerType = value;
}

tmSymbol *tmLayerProperties::GetSymbolRef()
{
    return m_LayerSymbol;
}

void tmLayerProperties::SetSymbolDirectly(tmSymbol *value)
{
    m_LayerSymbol = value;
}

void tmLayerProperties::SetVertexFlags(int value)
{
    m_LayerVertexFlags = value;
}

void tmLayerProperties::SetEditing(bool value)
{
    m_LayerEditing = value;
}


/***************************************************************************//**
 @brief Copy constructor
 @author Lucien Schreiber (c) CREALP 2008
 @date 27 October 2008
 *******************************************************************************/
tmLayerProperties::tmLayerProperties(tmLayerProperties &layerprop)
{
    // just to be sure
    InitMemberValues();

    m_LayerID = layerprop.m_LayerID;
    m_LayerSpatialType = layerprop.m_LayerSpatialType;
    m_LayerName = layerprop.m_LayerName;
    m_LayerVisible = layerprop.m_LayerVisible;
    m_LayerType = layerprop.m_LayerType;

    m_LayerSymbol = tmSymbol::CreateCopySymbolBasedOnType(m_LayerSpatialType, GetType(),
                                                          layerprop.m_LayerSymbol);
    wxASSERT(m_SymbolRulesManager);
    *m_SymbolRulesManager = *(layerprop.GetSymbolRuleManagerRef());

    m_LayerVertexFlags = layerprop.m_LayerVertexFlags;
    m_LayerEditing = layerprop.m_LayerEditing;
    m_LabelIsVisible = layerprop.m_LabelIsVisible;
    m_LabelDefinition = layerprop.m_LabelDefinition;
}


