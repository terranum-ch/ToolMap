/***************************************************************************
 tmaattribbatchmanager.cpp

 -------------------
 copyright            : (C) 2010 CREALP Lucien Schreiber 
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#include "tmaattribbatchmanager.h"
#include "prjdefmemmanage.h"
#include "../database/database_tm.h"
#include "../database/databaseresult.h"
#include "../gis/tmaattribctrls.h"


tmAAttribBatchManager::tmAAttribBatchManager(PrjDefMemManage *project,
                                             DataBaseTM *database,
                                             tmSelectedDataMemory *selected,
                                             PRJDEF_LAYERS_TYPE sellayertype)
{
    m_Project = project;
    m_DB = database;
    m_Selected = selected;
    m_SelLayerType = sellayertype;
}


tmAAttribBatchManager::~tmAAttribBatchManager()
{
}


bool tmAAttribBatchManager::GetTypes(PrjMemObjectsArray &objects, wxArrayInt &number, wxArrayLong &layerid)
{
    objects.Clear();
    number.Clear();
    layerid.Clear();

    if (IsOk() == false) {
        return false;
    }

    // create list of ids
    wxString myIdsText = _CreateListOfIds(m_Selected->GetSelectedValues());


    // create query
    wxString mySentence = wxString::Format(
            _T("SELECT a.OBJECT_VAL_ID, o.OBJECT_DESC_0, COUNT(*), o.THEMATIC_LAYERS_LAYER_INDEX FROM ")
                    _T("%s a LEFT JOIN %s  o ON a.OBJECT_VAL_ID = o.OBJECT_ID")
                    _T("  WHERE a.OBJECT_GEOM_ID IN (%s) GROUP BY a.OBJECT_VAL_ID"),
            TABLE_NAME_GIS_ATTRIBUTION[(int) m_SelLayerType].c_str(),
            TABLE_NAME_OBJECTS.c_str(),
            myIdsText.c_str());
    wxASSERT(m_DB);
    if (m_DB->DataBaseQuery(mySentence) == false) {
        return false;
    }


    if (m_DB->DataBaseHasResults() == false) {
        wxLogWarning(_("Selected data doesn't have any attribution"));
        return true;
    }

    // process results
    DataBaseResult myResults;
    m_DB->DataBaseGetResults(&myResults);
    for (int j = 0; j < myResults.GetRowCount(); j++) {
        ProjectDefMemoryObjects *myObj = new ProjectDefMemoryObjects();
        long myCount = 0;
        long myLayerid = 0;
        if (myResults.NextRow() == false) {
            break;
        }
        myResults.GetValue(0, myObj->m_ObjectID);
        myResults.GetValue(1, myObj->m_ObjectName);
        objects.Add(myObj);
        myResults.GetValue(2, myCount);
        number.Add(myCount);
        myResults.GetValue(3, myLayerid);
        layerid.Add(myLayerid);
    }
    return true;
}


bool tmAAttribBatchManager::GetFields(long layerid, PrjMemFieldArray &fields)
{

    // Clear fields from all values
    unsigned int fCount = fields.GetCount();
    for (unsigned int i = 0; i < fCount; i++) {
        ProjectDefMemoryFields *myField = fields.Item(0);
        wxDELETE(myField);
        fields.RemoveAt(0);
    }

    if (IsOk() == false) {
        return false;
    }

    if (layerid == wxNOT_FOUND) {
        wxLogError(_("Invalid layer ID (%ld)"), layerid);
        return false;
    }


    ProjectDefMemoryLayers *myLayer = m_Project->FindLayerByRealID(layerid);
    if (myLayer == NULL) {
        wxLogError(_("No layer found with %ld ID"), layerid);
        return false;
    }

    // check that layers are of similar spatial type
    if (m_SelLayerType != myLayer->m_LayerType) {
        return false;
    }

    for (int i = 0; i < m_Project->GetCountFields(); i++) {
        ProjectDefMemoryFields *mypField = m_Project->GetNextField();
        if (mypField == NULL) {
            wxLogError(_("Field %d returned NULL"), i);
            continue;
        }
        ProjectDefMemoryFields *myField = new ProjectDefMemoryFields();
        *myField = *mypField;
        fields.Add(myField);
    }
    return true;
}


tmAAttribCtrl *tmAAttribBatchManager::GetValueControl(const ProjectDefMemoryFields &field, wxWindow *wnd)
{

    if (IsOk() == false) {
        return NULL;
    }


    tmAAttribCtrl *mypControl = NULL;
    tmAAttribCtrlInteger *mypControlInt;
    tmAAttribCtrlFloat *mypControlFloat;
    tmAAttribCtrlEnum *mypControlEnum;
    tmAAttribCtrlText *mypControlText;
#ifdef __WXOSX__
    tmAAttribCtrlSafeDate * mypControlSafeDate;
#else
    tmAAttribCtrlDate *mypControlDate;
#endif

    switch (field.m_FieldType) {
        case TM_FIELD_INTEGER:
            mypControlInt = new tmAAttribCtrlInteger(wnd, field);
            mypControl = mypControlInt;
            break;

        case TM_FIELD_FLOAT:
            mypControlFloat = new tmAAttribCtrlFloat(wnd, field);
            mypControl = mypControlFloat;
            break;

        case TM_FIELD_ENUMERATION:
            mypControlEnum = new tmAAttribCtrlEnum(wnd, field);
            mypControl = mypControlEnum;
            break;

        case TM_FIELD_DATE:
#ifdef __WXOSX__
            mypControlSafeDate = new tmAAttribCtrlSafeDate(wnd, field);
            mypControl = mypControlSafeDate;
#else
            mypControlDate = new tmAAttribCtrlDate(wnd, field);
            mypControl = mypControlDate;
#endif

            break;

        default: // tmfieldText
            mypControlText = new tmAAttribCtrlText(wnd, field);
            mypControl = mypControlText;
            break;
    }
    return mypControl;
}


bool tmAAttribBatchManager::IsOk()
{
    if (m_Project == NULL) {
        wxLogError(_("Project isn't inited"));
        return false;
    }

    if (m_DB == NULL) {
        wxLogError(_("Database isn't inited"));
        return false;
    }

    if (m_Selected == NULL) {
        wxLogError(_("Selection isn't inited"));
        return false;
    }

    if (m_Selected->GetCount() == 0) {
        wxLogError(_("Nothing is selected, please select some objects first"));
        return false;
    }

    if (m_Selected->GetSelectedLayer() > TOC_NAME_LABELS + 1) {
        wxLogError(_("Batch advanced attribution isn't availlable for selected layer (%ld)"),
                   m_Selected->GetSelectedLayer());
        return false;
    }

    return true;
}


bool tmAAttribBatchManager::_GetSelectionSubset(long layerid, wxArrayLong &subselected)
{
    wxASSERT(m_Selected);
    wxASSERT(m_DB);
    subselected.Clear();


    wxString myIds = _CreateListOfIds(m_Selected->GetSelectedValues());
    if (myIds.IsEmpty()) {
        return false;
    }


    wxString myQuery = wxString::Format(_T("SELECT a.OBJECT_GEOM_ID FROM %s a LEFT JOIN %s o")
                                                _T(" ON a.OBJECT_VAL_ID = o.OBJECT_ID WHERE a.OBJECT_GEOM_ID IN")
                                                _T(" (%s) AND o.THEMATIC_LAYERS_LAYER_INDEX = %ld"),
                                        TABLE_NAME_GIS_ATTRIBUTION[(int) m_SelLayerType].c_str(),
                                        TABLE_NAME_OBJECTS.c_str(),
                                        myIds.c_str(),
                                        layerid);
    if (m_DB->DataBaseQuery(myQuery) == false) {
        return false;
    }

    if (m_DB->DataBaseHasResults() == false) {
        wxLogError(_("Getting subset of selected data failed, no results!"));
        return false;
    }

    m_DB->DataBaseGetResults(subselected);
    return true;
}


wxString tmAAttribBatchManager::_CreateListOfIds(const wxArrayLong *ids)
{
    if (ids == NULL) {
        wxLogError(_("Failed to create list of Ids, nothing selected!"));
        return wxEmptyString;
    }

    wxString myIdsText = wxEmptyString;
    for (unsigned int i = 0; i < ids->GetCount(); i++) {
        myIdsText.Append(wxString::Format(_T("%ld,"),
                                          ids->Item(i)));
    }
    myIdsText.RemoveLast(1);
    return myIdsText;
}


int tmAAttribBatchManager::Attribute(long layerid,
                                     const ProjectDefMemoryFields &field,
                                     const wxString &value)
{

    wxArrayLong mySubSet;
    if (_GetSelectionSubset(layerid, mySubSet) == false) {
        return wxNOT_FOUND;
    }

    // correct value for errors
    wxString myValueCleaned = wxEmptyString;
    m_DB->DataBaseStringEscapeQuery(value, myValueCleaned);

    // if enumeration, insert ID instead of text value
    if (field.m_FieldType == TM_FIELD_ENUMERATION) {
        for (unsigned int i = 0; i < field.m_pCodedValueArray.GetCount(); i++) {
            ProjectDefMemoryFieldsCodedVal *myVal = field.m_pCodedValueArray[i];
            wxASSERT(myVal);
            if (myVal->m_ValueName == value) {
                myValueCleaned = wxEmptyString;
                myValueCleaned << myVal->m_ValueID;
                break;
            }
        }
        if (myValueCleaned == wxEmptyString) {
            wxLogError(_("Unable to found: '%s' value, attribution aborted!"), value);
            return wxNOT_FOUND;
        }
    }


    wxString myQuery = wxEmptyString;
    for (unsigned int i = 0; i < mySubSet.GetCount(); i++) {
        myQuery.Append(wxString::Format(_T("INSERT INTO %s%ld (OBJECT_ID, %s) VALUES (%ld, \"%s\")")
                                                _T(" ON DUPLICATE KEY UPDATE %s=\"%s\";"),
                                        TABLE_NAME_LAYER_AT.c_str(),
                                        layerid,
                                        field.m_Fieldname.c_str(),
                                        mySubSet.Item(i),
                                        myValueCleaned.c_str(),
                                        field.m_Fieldname.c_str(),
                                        myValueCleaned.c_str()));
    }


    if (m_DB->DataBaseQueryNoResults(myQuery) == false) {
        return wxNOT_FOUND;
    }
    return (int) mySubSet.GetCount();
}

