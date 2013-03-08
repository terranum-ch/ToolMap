/***************************************************************************
 silayer.cpp
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

#include "silayer.h"
#include "siparam.h"

siLayer::siLayer(const wxString & layerpath, DataBase * database) {
    wxASSERT(database);
    m_Database = database;
    m_LayerNameIn.SetPath(layerpath);
    m_LayerIndexOut = wxNOT_FOUND;
    m_LayerType = SILAYER_TYPE_UNKNOWN;
    m_LayerTypeName = wxEmptyString;
}



siLayer::~siLayer() {
}


int siLayer::_GetEmptyBlockStop(int startpos) {
    wxTextFile myFile (m_RuleFileName.GetFullPath());
    if (myFile.Open() == false) {
        return wxNOT_FOUND;
    }
    
    int iStopPos = wxNOT_FOUND;
    siParam myParam;
    for (unsigned int i = startpos; i< myFile.GetLineCount(); i++) {
        if (myParam.IsEmpty(myFile.GetLine(i))== true) {
            iStopPos = i;
            break;
        }
        // last line
        if (i == myFile.GetLineCount() -1) {
            iStopPos = i+1;
            break;
        }
    }
    myFile.Close();
    return iStopPos;
}



bool siLayer::_LoadRuleIntoArray(int start, int stop, wxArrayString * array) {
    if (array == NULL) {
        return false;
    }
    array->Clear();
    
    if (stop < start) {
        wxLogError(_("Unable to load data into array, start and stop incorrect!"));
        return false;
    }
    
    wxTextFile myFile (m_RuleFileName.GetFullPath());
    if (myFile.Open() == false) {
        return false;
    }
    
    for (unsigned int i = start; i< stop ; i++) {
        array->Add(myFile.GetLine(i));
    }
    myFile.Close();
    return true;
}



bool siLayer::_ProcessFeature(OGRFeature * feature) {
    // check if kind exists otherwise ignore feature
    long myshpkind = feature->GetFieldAsInteger((const char *) m_Kind.GetKindNameIn().mb_str(wxConvUTF8));
    long myDbKind = m_Kind.GetRealKind(myshpkind);
    if (myDbKind == wxNOT_FOUND) {
        m_ProcessFeatureSkipped++;
        return true;
    }

    // check if geometry exists in the database. If yes, get it's ID
    char    *pszWKT = NULL;
    feature->GetGeometryRef()->exportToWkt(&pszWKT);
    wxString myTxtGeometry (pszWKT);
    OGRFree(pszWKT);
    wxString myExistQuery = _T("SELECT * FROM generic_lines WHERE ST_Equals(GeomFromText(\"%s\") , OBJECT_GEOMETRY)");
    if (m_Database->DataBaseQuery(wxString::Format(myExistQuery, myTxtGeometry)) == false) {
        return false;
    }
    bool bCopyGeometry = true;
    long myDatabaseId = wxNOT_FOUND;
    if (m_Database->DataBaseHasResults() == true) {
        wxArrayLong myIDs;
        m_Database->DataBaseGetResults(myIDs);
        if (myIDs.GetCount() != 1) {
            wxLogWarning(_("Overlapping geometries found. FID: %ld, Number of overlapping: %d"), feature->GetFID(), myIDs.GetCount());
        }
        myDatabaseId = myIDs.Item(0);
        bCopyGeometry = false;
        wxLogDebug(_("Geometry allready existing, databaseid = %ld"), myDatabaseId);
    }

    // copy geometry
    if (bCopyGeometry == true){
        if (wkbFlatten(feature->GetGeometryRef()->getGeometryType()) == wkbPoint) {
            OGRPoint * myPt = (OGRPoint*) feature->GetGeometryRef();
            if (myPt->getX() < 1000 || myPt->getY() < 1000) {
                wxLogError(_("Incorrect point found on fid: %ld, WKT: %s"), feature->GetFID(), myTxtGeometry);
            }
        }
        wxString myQuery = _T("INSERT INTO %s (OBJECT_GEOMETRY) VALUES (GeometryFromText('%s'))");
        if (m_Database->DataBaseQueryNoResults(wxString::Format(myQuery, m_LayerTypeName, myTxtGeometry))==false){
            return false;
        }
        myDatabaseId = m_Database->DataBaseGetLastInsertedID();
    }
    
    // migrate KIND
    if (myDbKind == wxNOT_FOUND) {
        wxLogError(_("Error getting Kind for FID: %ld (Kind searched: %ld)"), myDatabaseId, myshpkind);
        return false;
    }
    
    wxString myLayerAAT = wxEmptyString;
    switch (m_LayerType) {
        case SILAYER_TYPE_LINE:
            myLayerAAT = _T("generic_aat");
            break;
            
        case SILAYER_TYPE_POINT:
            myLayerAAT = _T("generic_pat");
            break;
            
        default:
            myLayerAAT = _T("generic_lat");
            break;
    }
    
    wxString myQuery = wxString::Format(_T("INSERT INTO %s VALUES (%ld, %ld)"), myLayerAAT, myDbKind, myDatabaseId);
    if (m_Database->DataBaseQueryNoResults(myQuery)==false) {
        wxLogError(_("Adding Kind for feature %ld Failed"), myDatabaseId);
        return false;
    }
    
    // Here process attributs!!!!
    bool bError = true;
    for (unsigned int i = 0; i< m_Attributs.GetCount(); i++) {
        siAttribut * myAttribut = m_Attributs.Item(i);
        wxASSERT(myAttribut);
        if (myAttribut->Process(feature, m_Database, m_LayerIndexOut, myDatabaseId, myshpkind)==false) {
            wxLogError(_("Processing attribut: %s failed!"), myAttribut->GetAttributNameIn());
            bError = false;
        }
    }
    return bError;
}



void siLayer::_ClearAttributArray() {
    int iNbItem = m_Attributs.GetCount();
    for (int i = iNbItem-1; i >= 0; i--) {
        siAttribut * myAttribut = m_Attributs.Item(i);
        wxASSERT(myAttribut);
        wxDELETE(myAttribut);
        m_Attributs.RemoveAt(i);
    }
    wxASSERT(m_Attributs.GetCount() == 0);
}



bool siLayer::LoadFromFile(const wxString & filename) {
    m_RuleFileName = wxFileName(filename);
    _ClearAttributArray();
    
    wxTextFile myFile (filename);
    if(myFile.Open()==false){
        wxLogError(_("Unable to open: %s"), filename);
        return false;
    }
    
    // getting parameters
    bool bError = false;
    siParam myParam;
    wxString myLayerInTxt = myParam.GetParam(myFile.GetLine(0), _T("LAYER_IN"), bError);
    if (bError == true) {
        myFile.Close();
        wxLogError(_("Unable to get LAYER_IN"));
        return false;
    }
    
    wxString myLayerOutTxt = myParam.GetParam(myFile.GetLine(1), _T("LAYER_OUT"), bError);
    if (bError == true) {
        myFile.Close();
        wxLogError(_("Unable to get LAYER_OUT"));
        return false;
    }
    
    wxString myLayerOutTypeTxt = myParam.GetParam(myFile.GetLine(2), _T("LAYER_OUT_TM"), bError);
    if (bError == true) {
        myFile.Close();
        wxLogError(_("Unable to get LAYER_OUT_TM"));
        return false;
    }
    myFile.Close();
    
    // processing and checking parameters
    m_LayerNameIn.SetName(myLayerInTxt);
    m_LayerNameIn.SetExt(_T("shp"));
    if(m_LayerNameIn.Exists() == false){
        wxLogError(_("Layer didn't exists"));
        return false;
    }
    
    m_LayerIndexOut = wxNOT_FOUND;
    if(m_Database->DataBaseQuery(wxString::Format(_T("SELECT LAYER_INDEX FROM thematic_layers WHERE LAYER_NAME =\"%s\""), myLayerOutTxt))==false){
        return false;
    }
    if(m_Database->DataBaseGetNextResult(m_LayerIndexOut)==false){
        return false;
    }
    m_Database->DataBaseClearResults();
    
    if (myLayerOutTypeTxt == _T("generic_points")) {
        m_LayerType = SILAYER_TYPE_POINT;
    } else if (myLayerOutTypeTxt == _T("generic_lines")){
        m_LayerType = SILAYER_TYPE_LINE;
    }else if (myLayerOutTypeTxt == _T("generic_labels")){
        m_LayerType = SILAYER_TYPE_POLYGON;
    }
    m_LayerTypeName = myLayerOutTypeTxt;
    wxLogMessage(_("Layer in is: %s (ID: %ld)"), m_LayerNameIn.GetFullName(), m_LayerIndexOut);
    
    // load kind
    int iKindStart = 4;
    int iKindStop = _GetEmptyBlockStop(iKindStart);
    wxLogMessage(_("Kind found starting from %d to %d"), iKindStart, iKindStop);
    
    wxArrayString myKindDef;
    if (_LoadRuleIntoArray(iKindStart, iKindStop, &myKindDef)==false) {
        return false;
    }
    
    if(m_Kind.LoadFromArray(myKindDef,m_Database)==false){
        wxLogError(_("Loading Kind failed!"));
        return false;
    }
    
    // load attributs
    int iAttributStart = wxNOT_FOUND;
    int iAttributStop = iKindStop;
    wxArrayString myAttributArray;
    while (1) {
        iAttributStart = iAttributStop+1;
        iAttributStop = _GetEmptyBlockStop(iAttributStart);
        if (iAttributStart == iAttributStop || iAttributStop == wxNOT_FOUND) {
            break;
        }
        
        wxLogMessage(_("Loading attribut from (line %d, to line %d)"), iAttributStart, iAttributStop);
        if(_LoadRuleIntoArray(iAttributStart, iAttributStop, &myAttributArray)==false){
            wxLogError(_("Loading attribut failed!"));
            continue;
        }
        
        siAttribut * myAttribut = new siAttribut();
        if(myAttribut->LoadFromArray(myAttributArray, m_Database, m_LayerIndexOut)==false){
            wxLogError(_("Loading attribut failed!"));
            continue;
        }
        m_Attributs.Add(myAttribut);
    }
    return true;
}



bool siLayer::Process() {
    m_ProcessFeatureSkipped = 0;
    OGRDataSource * pods = OGRSFDriverRegistrar::Open((const char*) m_LayerNameIn.GetFullPath().mb_str(wxConvUTF8), false);
    if (pods == NULL) {
        wxLogError(_("Opening %s failed!"), m_LayerNameIn.GetFullName());
        return false;
    }
    OGRLayer * poLayer = pods->GetLayer(0);
    if (poLayer == NULL) {
        wxLogError(_("Opening layer in %s failed!"), m_LayerNameIn.GetFullName());
        OGRDataSource::DestroyDataSource(pods);
        return false;
    }
    
    long myFeatureCount = poLayer->GetFeatureCount();
    wxLogMessage(_("%ld Features to process into %s"), myFeatureCount, m_LayerNameIn.GetFullName());
    
    OGRFeature *poFeature;
    poLayer->ResetReading();
    while( (poFeature = poLayer->GetNextFeature()) != NULL ){
        wxASSERT(poFeature);
        if (_ProcessFeature(poFeature)==false) {
            wxLogError(_("Processing feature %ld Failed!"), poFeature->GetFID());
        }
        OGRFeature::DestroyFeature(poFeature);
    }
    OGRDataSource::DestroyDataSource(pods);
    wxLogMessage(_("%ld feature processed (%ld skipped!)"), myFeatureCount-m_ProcessFeatureSkipped, m_ProcessFeatureSkipped);
    return true;
}


const siKind siLayer::GetKindRef() {
    return m_Kind;
}

