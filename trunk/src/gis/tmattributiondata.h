/***************************************************************************
							tmattributiondata.h
                    Main class for attribution process
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

// comment doxygen


#ifndef _TM_ATTRIBUTION_DATA_H_
#define _TM_ATTRIBUTION_DATA_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "../gui/attribution_obj_type.h" // attribution panel
#include "tmattributionbasicarray.h"	// for storing attribution into array

class DataBaseTM;

/***************************************************************************//**
 @brief Class for attributing data
 @details This class is the bridge between #tmAttributionManager and the data
 stored in the database. The open design based on class hierarchy allows us to
 attributes different objets if needed in the futur
 @author Lucien Schreiber (c) CREALP 2008
 @date 06 November 2008
 *******************************************************************************/
class tmAttributionData : public wxObject
	{
	private:
		void InitMemberValues();
		
	protected:
		wxArrayLong *	m_SelIDs;
		DataBaseTM *	m_pDB;
		
		wxString m_TableName;
		
		// checking function
		virtual bool IsValid ();
		
		// database function
		void PrepareAttributionStatement (wxString & statement,
										  const wxString & tablename,
										  wxArrayLong * checkedVal);
		void PrepareCleaningStatement (wxString & statement, const wxString & tablename);
		void PrepareGetInfoStatement (wxString & statement, const wxString & tablename);
		void PrepareGetInfoMultipleStatement (wxString & statement, const wxString & tablename);
		bool PrepareGetAttributionLayersID (const long & geomid, tmLayerValueArray & layersid,
											const wxString & tablename, int layertype);

		bool _GetInfoBasic (long oid, wxArrayLong & objid,
							wxArrayLong & objcode, wxArrayString & objname, int layertype);
	public:
		// ctor - dtor
		tmAttributionData();
		tmAttributionData(wxArrayLong * selected, DataBaseTM * database);
		virtual void Create (wxArrayLong * selected,  DataBaseTM * database);
		~tmAttributionData();
		
		// init database table
		virtual void SetDataBaseTable (const wxString & tablename){m_TableName = tablename;}
		
		// attribution
		virtual bool SetAttributeBasic (AttribObjType_PANEL * panel) {return false;}
		virtual bool SetAttributeBasicValues(wxArrayLong * values);
		virtual bool CleanAttributesAdvanced (long objectid, PrjDefMemManage * prjdef, PRJDEF_LAYERS_TYPE layertype);
		virtual bool SetAttributesAdvanced(long objectid, PrjMemLayersArray * layers, const wxArrayString & values);
		
		// copy attribution
		virtual bool CopyAttributesBasic (const long & copyfrom);
		
		// info
		virtual bool GetInfoBasic (AttribObjType_PANEL * panel) {return false;}
		virtual bool GetInfoBasic (long oid, wxArrayLong & objid,
								   wxArrayLong & objcode, wxArrayString & objname){return false;}
		virtual bool GetInfoBasicValues (const long & selected, 
										 wxArrayLong & values);
		virtual bool GetInfoBasicArray(tmAttributionBasicArray & values);
		static bool IsAttributionSimilar (const tmAttributionBasicArray & values);
		bool GetBasicNameFromID (const tmAttributionBasic & myAttribObj, 
								 wxArrayString & txtvalues);
		bool GetConcatenedBasicName (const tmAttributionBasicArray & myAttrib,
									 wxArrayString & concatenedattrib);
		virtual bool GetAttributesAdvanced (long objectid,
											PrjMemLayersArray * layers,
											wxArrayString & values);
		// only for one layer
		bool GetAdvancedAttribution (ProjectDefMemoryLayers * layer,
									wxArrayString & values,
                                    wxArrayString & codes,
									long selected);
		bool GetAdvancedAttribution (int layerid, long geomoid, wxArrayString & values);
		
		virtual bool GetAttributionLayersID (const long & geomid, tmLayerValueArray & layersid){return false;}
		virtual bool GetAttributionLayersIDFull (const long & geomid, tmLayerValueArray & layersid){return false;}
	};




#endif
