/***************************************************************************
							tmAttributionDataLine.h
						Class for attributing line
                             -------------------
    copyright            : (C) 2008 CREALP Lucien Schreiber 
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


#ifndef _TM_ATTRIBUTION_DATA_LINE_H_
#define _TM_ATTRIBUTION_DATA_LINE_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "tmattributiondata.h"




class tmAttributionDataLine : public tmAttributionData
	{
	private:
		
	
	protected:
		bool GetPanelValues (AttribObjType_PANEL * panel, wxArrayLong & valueids);
		void SetPanelValues (AttribObjType_PANEL * panel, const wxArrayLong & valueids);
		
	public:
		// ctor - dtor
		tmAttributionDataLine();
		tmAttributionDataLine(wxArrayLong * selected,DataBaseTM * database);
		virtual void Create (wxArrayLong * selected,DataBaseTM * database);
		~tmAttributionDataLine();
		
		
		// attribution
		virtual bool SetAttributeBasic (AttribObjType_PANEL * panel);
		// copy attribution
		//virtual bool CopyAttributesBasic (const long & copyfrom,
		//								  const wxArrayLong & copyto);
		
		// info
		virtual bool GetInfoBasic (AttribObjType_PANEL * panel);
		virtual bool GetInfoBasic (long oid,  wxArrayLong & objid,
								   wxArrayLong & objcode, wxArrayString & objname);
		//virtual bool GetInfoBasicValues (const long & selected, 
		//								 wxArrayLong & values);
		virtual bool GetAttributionLayersID (const long & geomid, tmLayerValueArray & layersid);

		
		virtual long GetLayerID(long objectcode);
		
		
	};





#endif
