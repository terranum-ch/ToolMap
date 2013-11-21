/***************************************************************************
 tmsnappingmemory.h
 Store snapping status into memory for faster access
 -------------------
 copyright            : (C) 2007 CREALP Lucien Schreiber
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

#ifndef _TM_SNAPPING_MEMORY_H_
#define _TM_SNAPPING_MEMORY_H_

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif


const int tmSNAPPING_OFF			= 0;
const int tmSNAPPING_VERTEX			= 1;
const int tmSNAPPING_BEGIN_END		= 2;
const wxString tmSNAPPING_TEXT_YES = wxTRANSLATE("Yes");

const wxString tmSNAPPING_TEXT [] = {_T(""), wxTRANSLATE("all vertex"), wxTRANSLATE("begin / end")};


class tmSnappingObject
{
private:
    void InitMemberValues();
    
public:
    long m_LayerID;
    int m_SnappingStatus;
    
    tmSnappingObject();
    ~tmSnappingObject();
};
// Creating a list of MemoryObjects
WX_DECLARE_OBJARRAY(tmSnappingObject, tmSnappingObjArray);



/***************************************************************************//**
 @brief Store snapping status into memory
 @details Snapping is stored in 
 - The database
 - The memory. 
 Snapping is also stored in memory for faster direct access to them during editing.
 When layers are added or removed, they are stored directly into the database but the
 snapping status is only stored when project is closed.
 @author Lucien Schreiber (c) CREALP 2009
 @date 21 January 2009
 *******************************************************************************/
class tmSnappingMemory : public wxObject
{
private:
    tmSnappingObjArray m_Snapping;
    int m_Tolerence;
    
protected:
    int FindSnappingItem (const long & lid);
    
public:
    tmSnappingMemory();
    ~tmSnappingMemory();
    
    // snapping operations
    void AddSnappingMemory (long lid, int snapstatus);
    unsigned int GetCount() {return m_Snapping.GetCount();}
    bool DeleteSnappingMemory (const long & lid);
    int GetSnappingMemoryStatus (const long & lid);
    bool SetSnappingMemoryStatus  (const long & lid, int snapstatus);
    bool GetSnappingInfo (unsigned int iIndex, long & lid, int & snapstatus);
    void Clear(){m_Snapping.Clear();}
    void ClearSnappingStatus ();
    
    // tolerence operations
    void SetTolerence (int tolereance) {m_Tolerence = tolereance;}
    int GetTolerence () {return m_Tolerence;}
    
    // misc function
    bool IsSnappingEnabled ();
};


#endif
