/***************************************************************************
 prjdefmemmanage.h
 Manage the project in memory
 -------------------
 copyright : (C) 2007 CREALP Lucien Schreiber
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

/*!
 \page Prj_def Project definition
 
 \section Introduction
 The project definition is stored in the embedded database but during the process
 of creating a new project or edition of an existing one, all will be temporally
 stored into the PrjDefMemManage class for allowing user to cancel any dialogs
 operation.
 
 \section Structure Structure of a project
 A project can be represented as a tree, the trunk is basicaly all project
 related settings such as project name, path, units, etc. and then branches are constituted
 of project layers. Each one of those layers contain properties such as layer name, type, etc.
 And then from thoses branches starts smaller branches constituted of fields or object (see figure).
 
  \image html structure_projet.jpg
 
 
 
 
 
 */



#ifndef PRJDEFMEMMANAGE_H
#define PRJDEFMEMMANAGE_H

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif


// for Memory class definition
#include "projectdefmemory.h"


class PrjDefMemManage
{
private:
    // variables
    ProjectDefMemoryLayers *m_pActiveLayer;
    ProjectDefMemoryFields *m_pActiveField;
    int m_iActualObj;
    int m_iActualField;
    int m_iActualCodedVal;
    int m_iActualLayers;

    // member functions
    void InitDefaultValues();

public:
    // variables
    wxString m_PrjPath;
    wxString m_PrjName;
    wxString m_PrjAuthors;
    wxString m_PrjSummary;
    PRJDEF_UNIT_TYPE m_PrjUnitType;
    PRJDEF_PROJ_TYPE m_PrjProjType;
    PrjMemLayersArray m_PrjLayerArray;
    PrjMemScaleArray m_ScaleArray;

    wxArrayLong m_StoreDeleteIDObj;
    wxArrayLong m_StoreDeleteScale;
    wxArrayLong m_StoreDeleteLayers;

    // ctor - dtor
    PrjDefMemManage();

    ~PrjDefMemManage();


    // layer members functions
    ProjectDefMemoryLayers *AddLayer();

    int RemoveLayer(int iIndex = -1);// remove last item by default...
    bool RemoveLayer(const wxString &layerName);

    ProjectDefMemoryLayers *FindLayer(const wxString &layerName);

    ProjectDefMemoryLayers *FindLayer(unsigned int iIndex = 0);

    ProjectDefMemoryLayers *FindLayerByRealID(unsigned int iIndex);

    void SetActiveLayer(ProjectDefMemoryLayers *ActiveLayer)
    { m_pActiveLayer = ActiveLayer; }

    ProjectDefMemoryLayers *GetActiveLayer()
    { return m_pActiveLayer; }

    int GetCountLayers()
    { return m_PrjLayerArray.GetCount(); }

    ProjectDefMemoryLayers *GetNextLayer();

    // object members functions
    ProjectDefMemoryObjects *AddObject();

    int RemoveObject(int iIndex = -1); // remove last
    bool RemoveObject(const wxString &ObjectName);

    ProjectDefMemoryObjects *FindObject(const wxString &ObjectName);

    ProjectDefMemoryObjects *FindObject(unsigned int iIndex = 0);

    ProjectDefMemoryObjects *GetNextObjects();

    int GetCountObject();

    // field members functions
    ProjectDefMemoryFields *AddField();

    int RemoveField(int iIndex = -1); // remove last
    bool RemoveField(const wxString &FieldName);

    ProjectDefMemoryFields *FindField(const wxString &FieldName);

    ProjectDefMemoryFields *FindField(unsigned int iIndex = 0);

    ProjectDefMemoryFields *GetNextField();

    int GetCountFields();

    void SetActiveField(ProjectDefMemoryFields *ActiveField)
    { m_pActiveField = ActiveField; }

    ProjectDefMemoryFields *GetActiveField()
    { return m_pActiveField; }

    // coded values belonging to fields
    ProjectDefMemoryFieldsCodedVal *AddCodedValue();

    int RemoveCodedValue(int iIndex = -1); // remove last
    bool RemoveCodedValue(const wxString &ValueName);

    ProjectDefMemoryFieldsCodedVal *FindCodedValue(const wxString &ValueName, int &IndexPos);

    ProjectDefMemoryFieldsCodedVal *GetNextCodedValue();

    int GetCountCodedValue();

    // scale functions
    ProjectDefMemoryScale *AddScale(const long &scale, const int &DBindex = -1);

    ProjectDefMemoryScale *FindScale(const long &oldscale);

    bool RemoveScale(const long &oldscale);

    // copy function
    PrjDefMemManage &operator=(const PrjDefMemManage &source);
};

#endif

