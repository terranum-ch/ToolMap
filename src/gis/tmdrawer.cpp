/***************************************************************************
 tmdrawer.cpp
 Draws GIS data into bitmap
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

#include "tmdrawer.h"
#include "tmsymbolvectorlinemultiple.h"
#include "tmsymbolvectorpointmultiple.h"
#include "tmsymbolvectorpolygon.h"
#include "tmsymbolrule.h"
#include "../database/database_tm.h"
#include "../database/databaseresult.h"

bool tmDrawer::m_LogOn = true;

tmDrawer::tmDrawer(){
	m_bmp = NULL;
	m_IsInitialised = FALSE;

	// load image using GDAL
	// owned by image, do not destroy manually.
	imgbuf = NULL;
	imglen = 0;
	maskbuf = NULL;
	masklen= 0;
	m_ActuallayerID = 0;
	m_SelMem = NULL;
    m_scale = NULL;
}



tmDrawer::~tmDrawer(){

}



void tmDrawer::InitDrawer(wxBitmap * bitmap, tmGISScale * scale, const tmRealRect & filter)
{
	m_bmp = bitmap;
	m_scale = scale;
	m_spatFilter = filter;
	
	if (m_bmp && m_bmp->IsOk()){
		m_IsInitialised = true;
    }
	else{
		m_IsInitialised = false;
		if (IsLoggingEnabled()){
			wxLogDebug(_T("Error initing drawer \n"));
        }
	}
}


/***************************************************************************//**
 @brief Draw the max extent
 @details Draw a rectangle of specified colour and width corresponding to the 
 maximum extent of all visible layers
 @param width Width of the pen (in pixels)
 @param col Colour of the pen
 @return always true
 @author Lucien Schreiber (c) CREALP 2008
 @date 24 July 2008
 *******************************************************************************/
bool tmDrawer::DrawExtentIntoBitmap (int width, const wxColour & col)
{
	// check for initialisation
	if(!m_IsInitialised)
	{
		wxString sFunction = wxString::FromAscii(__FUNCTION__);
		wxString sFunctionLineError = wxString::Format( _T("%s line %d : "),
													   sFunction.c_str(), __LINE__); 
		wxString sErrMsg = wxString::Format(_T("%s Drawer not initialised"),
											sFunctionLineError.c_str());
		
		wxASSERT_MSG(0,sErrMsg);
	}
	
	
	wxMemoryDC temp_dc;
	temp_dc.SelectObject(*m_bmp);
	
	wxPen myPen (col, width);
	temp_dc.SetPen(myPen);
	temp_dc.SetBackground(*wxWHITE);
	
	
	tmRealRect myRealExt = m_scale->GetMaxLayersExtent();
	
	wxPoint pts[5];
	pts[0] = m_scale->RealToPixel(wxRealPoint(myRealExt.x_min, myRealExt.y_min));
	pts[1] = m_scale->RealToPixel(wxRealPoint(myRealExt.x_max, myRealExt.y_min));
	pts[2] = m_scale->RealToPixel(wxRealPoint(myRealExt.x_max, myRealExt.y_max));
	pts[3] = m_scale->RealToPixel(wxRealPoint(myRealExt.x_min, myRealExt.y_max));
	pts[4] = m_scale->RealToPixel(wxRealPoint(myRealExt.x_min, myRealExt.y_min));
	
	temp_dc.DrawLines(5, pts);
	
	
	wxBitmap nullbmp;
	temp_dc.SelectObject(nullbmp);
	
	return TRUE;
}



bool tmDrawer::Draw (tmLayerProperties * itemProp, tmGISData * pdata)
{
	m_ActuallayerID = itemProp->GetID();
	switch (itemProp->GetSpatialType())
	{
		case LAYER_SPATIAL_LINE:
			if (itemProp->GetType() == TOC_NAME_LINES) {
				DrawLinesEnhanced(itemProp, pdata);
			}else {
				DrawLines(itemProp, pdata);
			}
			break;
		case LAYER_SPATIAL_POINT:
			if (itemProp->GetType() == TOC_NAME_LABELS || itemProp->GetType() == TOC_NAME_POINTS) {
				DrawPointsEnhanced(itemProp, pdata);
			}
			else {
				DrawPoints(itemProp, pdata);
			}
			break;
		case LAYER_SPATIAL_POLYGON:
			DrawPolygons(itemProp, pdata);
			DrawVertexPoly(itemProp, pdata);
			break;
		case LAYER_SPATIAL_RASTER:
			DrawRaster(itemProp, pdata);
			break;
		default:
			fprintf(stderr, "%s line %d : Error - no drawer found for one object\n ",
					__FUNCTION__, __LINE__);
			return FALSE;
			break;
	}
	
	return TRUE;	
}


/***************************************************************************//**
 @brief Draw all lines
 @details This  function uses the symbology (#tmSymbol) and the GIS data
 (#tmGISData) for drawing all lines into the bitmap.
 @param itemProp contain all informations about the layer properties.
 @param pdata A valid object of tmGISData type
 @return  false if it wasen't able to define a spatial filter see :
 tmGISDataVectorMYSQL::SetSpatialFilter
 @author Lucien Schreiber (c) CREALP 2008
 @date 10 September 2008
 *******************************************************************************/
bool tmDrawer::DrawLines(tmLayerProperties * itemProp, tmGISData * pdata)
{
    if (itemProp->GetSymbolRuleManagerRef()->GetRulesRef()->GetCount() > 0 &&
        itemProp->GetSymbolRuleManagerRef()->IsUsingRules()==true) {
        return DrawLinesRules(itemProp, pdata);
    }
    
    tmSymbolVectorLine * pSymbol = (tmSymbolVectorLine*) itemProp->GetSymbolRef();
    bool bUseOriented = false;
    wxPenStyle myStyle ((wxPenStyle) pSymbol->GetShape());
    if (pSymbol->GetShape() == tmPENSTYLE_ORIENTED) {
        myStyle = wxPENSTYLE_SOLID;
        bUseOriented = true;
    }
    
	wxPen myPen (pSymbol->GetColour(),pSymbol->GetWidth(), myStyle);
	wxPen mySPen (m_SelMem->GetSelectionColour(), pSymbol->GetWidth());
	wxPen mySHaloPen (*wxWHITE, pSymbol->GetWidth() + 2);
	wxPen * myVPen = CreateVertexUniquePen(itemProp, pSymbol->GetWidth());
    wxPen myOrientedPen (myPen);
    myOrientedPen.SetStyle(wxPENSTYLE_DOT); 
	
	// define spatial filter
	tmGISDataVector * pVectLine = (tmGISDataVector*) pdata;
	if(!pVectLine->SetSpatialFilter(m_spatFilter,itemProp->GetType()))
	{
		if (IsLoggingEnabled()){
			wxLogDebug(_T("Error setting spatial filter"));
		}
		wxDELETE(myVPen);
		return false;
	}
	
	wxMemoryDC temp_dc;
	temp_dc.SelectObject(*m_bmp);
	wxGraphicsContext* pgdc = wxGraphicsContext::Create( temp_dc);

	int iNbVertex = 0;
    long mySkippedVertex = 0;
    long myTotalVertex = 0;
	while (1){
		iNbVertex = 0;
		long myOid = 0;
		wxRealPoint * pptsReal = pVectLine->GetNextDataLine(iNbVertex, myOid);
		if (pptsReal == NULL){
			break;
		}
        
        
        myTotalVertex += iNbVertex;
        // convert realpts to pxpts
        wxPoint * pptspx = new wxPoint[iNbVertex];
        m_PreviousPoint = wxDefaultPosition;
        for (int i = 0; i< iNbVertex; i++) {
            wxPoint myPt = m_scale->RealToPixel(pptsReal[i]);
            if (myPt == m_PreviousPoint) {
                pptspx[i] = wxDefaultPosition;
                ++mySkippedVertex;
            }else{
                pptspx[i] = myPt;
            }
            m_PreviousPoint = myPt;
        }
        wxDELETEA(pptsReal);
		
        // set pen
        wxPen myActualPen = myPen;
        if (m_ActuallayerID == m_SelMem->GetSelectedLayer()){
            if (m_SelMem->IsSelected(myOid)){
                // drawing halo
				if (m_SelMem->GetSelectionHalo() == true) {
					pgdc->SetPen(mySHaloPen);
					wxGraphicsPath myPath = pgdc->CreatePath();
					myPath.MoveToPoint(pptspx[0]);
					for (int i = 1; i< iNbVertex; i++)
                        if (pptspx[i] != wxDefaultPosition) {
                            myPath.AddLineToPoint(pptspx[i]);
                        }
					pgdc->StrokePath(myPath);
				}
                myActualPen = mySPen;
            }
        }

    
        pgdc->SetPen(myActualPen);
		// creating path
		wxGraphicsPath myPath = pgdc->CreatePath();
		myPath.MoveToPoint(pptspx[0]);
		for (int i = 1; i< iNbVertex; i++){
            if (pptspx[i] != wxDefaultPosition) {
                myPath.AddLineToPoint(pptspx[i]);
            }
        }
		pgdc->StrokePath(myPath);
        
        // oriented lines
        if(bUseOriented == true){
            _DrawOrientedLine(pgdc, pptspx, iNbVertex, myActualPen);
        }

		tmLayerProperties myProperty (*itemProp);
		// drawing all vertex for in edition line
		if (m_ActuallayerID == m_SelMem->GetSelectedLayer() &&
			myProperty.IsEditing() == true &&
			m_SelMem->GetCount() == 1 &&
			m_SelMem->IsSelected(myOid)){
			myProperty.SetVertexFlags(tmDRAW_VERTEX_ALL);
		}
		
		// drawing vertex
		DrawVertexLine(pgdc, pptspx, iNbVertex, &myProperty, myVPen);
		wxDELETEA(pptspx);
	}
	temp_dc.SelectObject(wxNullBitmap);
	wxDELETE(myVPen);
	wxDELETE(pgdc);
    
    wxLogDebug(_T("%ld/%ld vertex skipped! (%s%%)"), mySkippedVertex, myTotalVertex, wxString::FromDouble( 1.0 * mySkippedVertex / myTotalVertex * 100.0));
	return true;
}


bool tmDrawer::_SelectFeatureByQuery (long myQueryID, DataBaseTM * database, wxArrayLong & results){
	// get query
	int myTarget = wxNOT_FOUND;
	results.Clear();
	
	wxString myName = wxEmptyString;
	wxString myQueryCode = wxEmptyString;
	if (database->GetQueriesById(myQueryID, myTarget, myName, myQueryCode)==false) {
		wxLogWarning(_("Unable to get complex symbology.\nQuery chosen may have been deleted (Query id: %ld)\nSwitching to unique symbology!"),myQueryID);
		return false;
	}
	
	// run query
	if (database->DataBaseQuery(myQueryCode)==false) {
		wxLogError(_("Unable to load complex symbology (Query id: %ld)"),myQueryID);
		return false;
	}
	
	database->DataBaseGetResults(results);
	database->DataBaseClearResults();
	return true;
}


bool tmDrawer::_ExistsinResults (long Oid, const wxArrayLong & results){
	
	for (unsigned int i = 0; i<results.GetCount(); i++) {
		if (Oid == results.Item(i)) {
			return true;
		}
		
		/*if (results.Item(i) > Oid) {
			break;
		}*/
	}
	return false;
}



bool tmDrawer::DrawLinesEnhanced(tmLayerProperties * itemProp, tmGISData * pdata){
	tmSymbolVectorLineMultiple * pSymbol = (tmSymbolVectorLineMultiple*) itemProp->GetSymbolRef();
	
	// simple symbology
	if (pSymbol->GetSelectedSymbology() == 0) { 
		return DrawLines(itemProp, pdata);
	}
	
	// complex symbology
	tmSymbolDataLineMultiple * mySymbology = pSymbol->GetSymbology();
	wxASSERT(mySymbology);
	DataBaseTM * myDB = ((tmGISDataVectorMYSQL*) pdata)->GetDataBaseHandle();
	wxASSERT(myDB);
	wxArrayLong myResult;
	if (_SelectFeatureByQuery(mySymbology->m_QueryID,myDB, myResult)==false) {
        mySymbology->m_PanelNo = 0;
		return DrawLines(itemProp, pdata);
	}
	
	// create pens
    int myValidPenStyle (tmSYMBOLPENSYLES[mySymbology->m_SelShapeMultiple]);
    int myUnValidPenStyle (tmSYMBOLPENSYLES[mySymbology->m_UnSelShapeMultiple]);
    
    bool bUseValidOriented = false;
    if (myValidPenStyle == tmPENSTYLE_ORIENTED) {
        myValidPenStyle = wxPENSTYLE_SOLID;
        bUseValidOriented = true;
    }
    
    bool bUseUnValidOriented = false;
    if (myUnValidPenStyle == tmPENSTYLE_ORIENTED) {
        myUnValidPenStyle = wxPENSTYLE_SOLID;
        bUseUnValidOriented = true;
    }
    
    
	wxPen myValidPen (pSymbol->GetColourWithTransparency(mySymbology->m_SelColourMultiple,
														 mySymbology->m_GlobalTransparency),mySymbology->m_SelWidthMultiple, (wxPenStyle) myValidPenStyle);
	wxPen myUnValidPen (pSymbol->GetColourWithTransparency(mySymbology->m_UnSelColourMultiple,
														   mySymbology->m_GlobalTransparency), mySymbology->m_UnSelWidthMultiple, (wxPenStyle) myUnValidPenStyle);
	wxPen mySelectionValidPen (m_SelMem->GetSelectionColour(),  mySymbology->m_SelWidthMultiple);
	wxPen mySelectionUnValidPen (m_SelMem->GetSelectionColour(), mySymbology->m_UnSelWidthMultiple);

	wxPen mySelectionValidHaloPen (*wxWHITE,  mySymbology->m_SelWidthMultiple + 2);
	wxPen mySelectionUnValidHaloPen (*wxWHITE,  mySymbology->m_UnSelWidthMultiple + 2);
	
	wxPen myVertexValidPen (*CreateVertexUniquePen(itemProp, mySymbology->m_SelWidthMultiple));
	wxPen myVertexUnValidPen (*CreateVertexUniquePen(itemProp, mySymbology->m_UnSelWidthMultiple));

	// spatial filter
	tmGISDataVector * pVectLine = (tmGISDataVector*) pdata;
	if(pVectLine->SetSpatialFilter(m_spatFilter,itemProp->GetType()) == false){
		return false;
	}
	
	// drawing
	wxMemoryDC temp_dc;
	temp_dc.SelectObject(*m_bmp);
	wxGraphicsContext* pgdc = wxGraphicsContext::Create( temp_dc);
	
    long myTotalVertex = 0;
    long mySkippedVertex = 0;
    
	while (1)
	{
		int iNbVertex = wxNOT_FOUND;
		long myOid = wxNOT_FOUND;
		wxRealPoint * pptsReal = pVectLine->GetNextDataLine(iNbVertex, myOid);
		if (iNbVertex <= 1) {
			wxDELETEA(pptsReal);
			break;
		}
       
        myTotalVertex += iNbVertex;
        // convert realpts to pxpts
        wxPoint * pptspx = new wxPoint[iNbVertex];
        m_PreviousPoint = wxDefaultPosition;
        for (int i = 0; i< iNbVertex; i++) {
            wxPoint myPt = m_scale->RealToPixel(pptsReal[i]);
            if (myPt == m_PreviousPoint) {
                pptspx[i] = wxDefaultPosition;
                ++mySkippedVertex;
            }else{
                pptspx[i] = myPt;
            }
            m_PreviousPoint = myPt;
        }
        wxDELETEA(pptsReal);
		
		bool IsSelected = false;
		if (m_ActuallayerID == m_SelMem->GetSelectedLayer()){
			if (m_SelMem->IsSelected(myOid)){
				IsSelected = true;
			}
		}

		// search id into results
		bool IsValid = false;
		if (_ExistsinResults(myOid, myResult) == true) {
			IsValid = true;
		}
		
		// drawing halo
		if (IsSelected == true && m_SelMem->GetSelectionHalo() == true) {
			if (IsValid) {
				pgdc->SetPen(mySelectionValidHaloPen);
			}
			else {
				pgdc->SetPen(mySelectionUnValidHaloPen);
			}
			wxGraphicsPath myPath = pgdc->CreatePath();
			myPath.MoveToPoint(pptspx[0]);
			for (int i = 1; i< iNbVertex; i++){
                if (pptspx[i] != wxDefaultPosition) {
                    myPath.AddLineToPoint(pptspx[i]);
                }
			}
			pgdc->StrokePath(myPath);			
		}
		
		// choosing correct pen
        wxPen myActualPen;
		wxPen myVertexPen;
		if (IsValid==true) {
			myVertexPen = myVertexValidPen;
			if (IsSelected) {
                myActualPen = mySelectionValidPen;
			}
			else {
                myActualPen = myValidPen;
			}
		}
		else {
			myVertexPen = myVertexUnValidPen;
			if (IsSelected) {
                myActualPen = mySelectionUnValidPen;
			}
			else {
                myActualPen = myUnValidPen;
			}
		}
        pgdc->SetPen(myActualPen);
        
		// creating path
		wxGraphicsPath myPath = pgdc->CreatePath();
		myPath.MoveToPoint(pptspx[0]);
		for (int i = 1; i< iNbVertex; i++){
            if (pptspx[i] != wxDefaultPosition) {
                myPath.AddLineToPoint(pptspx[i]);
            }
		}
		pgdc->StrokePath(myPath);
		
        // oriented lines
        if(bUseValidOriented == true && IsValid == true){
            _DrawOrientedLine(pgdc, pptspx, iNbVertex, myActualPen);
        }
        if(bUseUnValidOriented == true && IsValid == false){
            _DrawOrientedLine(pgdc, pptspx, iNbVertex, myActualPen);
        }      
		
		tmLayerProperties myProperty (*itemProp);
		// drawing all vertex for in edition line
		if (m_ActuallayerID == m_SelMem->GetSelectedLayer() &&
			myProperty.IsEditing() == true &&
			m_SelMem->GetCount() == 1 &&
			m_SelMem->IsSelected(myOid)){
			myProperty.SetVertexFlags(tmDRAW_VERTEX_ALL);
		}
		
		// drawing vertex
		DrawVertexLine(pgdc, pptspx, iNbVertex, &myProperty, &myVertexPen);
		wxDELETEA(pptspx);
	}
	temp_dc.SelectObject(wxNullBitmap);
	wxDELETE(pgdc);
    
    wxLogDebug(_T("%ld/%ld vertex skipped! (%s%%)"), mySkippedVertex, myTotalVertex, wxString::FromDouble( 1.0 * mySkippedVertex / myTotalVertex * 100.0));
	return true;	
}




bool tmDrawer::DrawLinesRules (tmLayerProperties * itemProp, tmGISData * pdata){
    wxASSERT(itemProp->GetSymbolRuleManagerRef()->GetRulesRef()->GetCount() > 0);
	wxMemoryDC dc;
    dc.SelectObject(*m_bmp);
	wxGraphicsContext* pgdc = wxGraphicsContext::Create( dc);
    
    // define spatial filter
	tmGISDataVector * pVectLine = (tmGISDataVector*) pdata;
	if(!pVectLine->SetSpatialFilter(m_spatFilter,itemProp->GetType()))
	{
		if (IsLoggingEnabled()){
			wxLogError(_T("Error setting spatial filter"));
		}
		dc.SelectObject(wxNullBitmap);
		wxDELETE(pgdc);
		return false;
	}
        
    // process rules
    int iLoop = 0;
    tmSymbolRuleArray * myRulesArray = itemProp->GetSymbolRuleManagerRef()->GetRulesRef();
    wxASSERT(myRulesArray);
    long mySkippedVertex = 0;
    long myTotalVertex = 0;
    for (unsigned int s = 0; s < myRulesArray->GetCount(); s++) {
        tmSymbolRule * myRule = myRulesArray->Item(s);
        wxASSERT(myRule);
        if (myRule->GetAttributFilter() == wxEmptyString || myRule->IsActive() == false) {
            continue;
        }
        
        if(pVectLine->SetAttributFilter(myRule->GetAttributFilter())==false){
            continue;
        }
        
        wxPen myRulePen = myRule->GetPen();
        bool bUseOriented = false;
        if (myRulePen.GetStyle() == (wxPenStyle) tmPENSTYLE_ORIENTED) {
            myRulePen.SetStyle(wxPENSTYLE_SOLID);
            bUseOriented = true;
        }
        
        wxPen mySelectPen = myRulePen;
        mySelectPen.SetColour(m_SelMem->GetSelectionColour());
        mySelectPen.SetWidth(mySelectPen.GetWidth() + 1);
        pgdc->SetPen(myRulePen);
        wxPen myVertexPen (*CreateVertexUniquePen(itemProp, myRulePen.GetWidth()));
        
        while (1){
            int iNbVertex = wxNOT_FOUND;
            long myOid = wxNOT_FOUND;
            wxRealPoint * pptsReal = pVectLine->GetNextDataLine(iNbVertex, myOid);
            if (pptsReal == NULL || iNbVertex <= 1){
                wxDELETEA(pptsReal);
                break;
            }
            
            myTotalVertex += iNbVertex;
            // convert realpts to pxpts
            wxPoint * pptspx = new wxPoint[iNbVertex];
            m_PreviousPoint = wxDefaultPosition;
            for (int i = 0; i< iNbVertex; i++) {
                wxPoint myPt = m_scale->RealToPixel(pptsReal[i]);
                if (myPt == m_PreviousPoint) {
                    pptspx[i] = wxDefaultPosition;
                    ++mySkippedVertex;
                }else{
                    pptspx[i] = myPt;
                }
                m_PreviousPoint = myPt;
            }
            wxDELETEA(pptsReal);
            
            // set brush
            wxPen myActualPen = myRulePen;
            if (m_ActuallayerID == m_SelMem->GetSelectedLayer()){
                if (m_SelMem->IsSelected(myOid)){
                    myActualPen = mySelectPen;
                }
            }
            pgdc->SetPen(myActualPen);
            
            // creating path
            wxGraphicsPath myPath = pgdc->CreatePath();
            myPath.MoveToPoint(pptspx[0]);
            for (int i = 1; i< iNbVertex; i++){
                if (pptspx[i] != wxDefaultPosition) {
                    myPath.AddLineToPoint(pptspx[i]);
                }
            }
            pgdc->StrokePath(myPath);
            
            if (bUseOriented == true) {
                _DrawOrientedLine(pgdc, pptspx, iNbVertex, myActualPen);
            }
            
            tmLayerProperties myProperty (*itemProp);
            // drawing vertex
            DrawVertexLine(pgdc, pptspx, iNbVertex, &myProperty, &myVertexPen);
            wxDELETEA(pptspx);
            iLoop++;
        }
    }
    
    if (IsLoggingEnabled()){
		wxLogDebug(_T("%d Lines drawn"), iLoop);
    }
    wxLogDebug(_T("%ld/%ld vertex skipped! (%s%%)"), mySkippedVertex, myTotalVertex, wxString::FromDouble( 1.0 * mySkippedVertex / myTotalVertex * 100.0));
	dc.SelectObject(wxNullBitmap);
	wxDELETE(pgdc);
	return true;
}



bool tmDrawer::_DrawOrientedLine (wxGraphicsContext * gdc,wxPoint * pts, int nbpts, wxPen actualPen){
    wxASSERT(gdc);
    wxASSERT(pts);
    wxASSERT(nbpts > 1);
    
    wxGraphicsPath myPath = gdc->CreatePath();
    int myShift = -1 * (actualPen.GetWidth()+1);
    actualPen.SetStyle(wxPENSTYLE_DOT);
    gdc->SetPen(actualPen);
    bool HasfirstPoint = false;
    
    for (int i = 0; i< nbpts-1; i++) {
        wxPoint p1 = pts[i];
        wxPoint p2 = pts[i+1];
        
        // ignore skipped points
        if (p1 == wxDefaultPosition) {
            continue;
        }
        if ( p2 == wxDefaultPosition) {
            continue;
        }
        
        // compute vector and perpendicular
        double dx = p2.x - p1.x;
        double dy = p2.y - p1.y;
        
        double perpx = dy;
        double perpy = -dx;
        
        wxPoint p3 = p1;
        wxPoint p4 = p2;
        
        double len = sqrtf(perpx*perpx + perpy*perpy);
        if (len > 0) {
            perpx = perpx / len;
            perpy = perpy / len;
            
            perpx = perpx * myShift;
            perpy = perpy * myShift;
            
            p3.x = p1.x + perpx;
            p3.y = p1.y + perpy;
            
            p4.x = p2.x + perpx;
            p4.y = p2.y + perpy;
        }
        
        // new points
        if (HasfirstPoint == false) {
            myPath.MoveToPoint(p3);
            HasfirstPoint = true;
        }
        else{
            myPath.AddLineToPoint(p3);
        }
        myPath.AddLineToPoint(p4);
    }
    gdc->StrokePath(myPath);
    return true;
}



/***************************************************************************//**
 @brief Draw all points
 @details This functions draw all points using the new wxGraphicsContext
 engine.
 @note library must be compiled with options according to that.
 @param itemProp contain all informations about the layer's properties such as
 the symbology
 @param pdata pointer to the GIS data (see wxGISData) and derived.
 @return  false if not able to draw all points
 @author Lucien Schreiber (c) CREALP 2008
 @date 16 September 2008
 *******************************************************************************/
bool tmDrawer::DrawPoints (tmLayerProperties * itemProp, tmGISData * pdata)
{	
    if (itemProp->GetSymbolRuleManagerRef()->GetRulesRef()->GetCount() > 0 &&
        itemProp->GetSymbolRuleManagerRef()->IsUsingRules()==true) {
        return DrawPointsRules(itemProp, pdata);
    }
    
    // define spatial filter
	tmGISDataVector * pVectPoint = (tmGISDataVector*) pdata;
	if(!pVectPoint->SetSpatialFilter(m_spatFilter,itemProp->GetType()))
	{
		if (IsLoggingEnabled()){
			wxLogDebug(_T("Error setting spatial filter"));
        }
		return false;
	}
	
	wxMemoryDC dc;
	dc.SelectObject(*m_bmp);
	wxGraphicsContext * pgdc = wxGraphicsContext::Create( dc); 
	
	// create pen based on symbology
	tmSymbolVectorPoint * pSymbol = (tmSymbolVectorPoint*) itemProp->GetSymbolRef();
	wxPen myPen (pSymbol->GetColour(),pSymbol->GetRadius());
	wxPen mySPen (m_SelMem->GetSelectionColour(), pSymbol->GetRadius());
	wxPen mySHaloPen (*wxWHITE, pSymbol->GetRadius() + 2);

	
	pgdc->SetPen(myPen);
	
	// iterate for all points, will not work on a threaded version
	// because of all wxLogDebug commands
	bool bReturn = true;
	int iLoop = 0;
	long myOid = 0;
	wxPoint Intpts (0,0);
	bool bAsSelection = false;
	
	while (1)
	{
		wxRealPoint * pptsReal = pVectPoint->GetNextDataPoint(myOid);
		
		if(pptsReal == NULL)
		{
			if (IsLoggingEnabled()){
				wxLogDebug(_T("No point returned @loop : %d"), iLoop);
            }
			bReturn = FALSE;
			break;
		}
		
		// convert from real coordinates to screen coordinates
		Intpts = m_scale->RealToPixel(*pptsReal);
		
		// changing pen for selected data
		if (m_ActuallayerID == m_SelMem->GetSelectedLayer())
			if (m_SelMem->IsSelected(myOid))
			{
				if (m_SelMem->GetSelectionHalo() == true) {
					pgdc->SetPen(mySHaloPen);
#ifdef __WXMSW__
					pgdc->StrokeLine (Intpts.x, Intpts.y, Intpts.x + 0.1, Intpts.y + 0.1);
#else
					pgdc->StrokeLine (Intpts.x, Intpts.y, Intpts.x, Intpts.y);
#endif
				}
				
				pgdc->SetPen(mySPen);
				bAsSelection = true;
			}
			
#ifdef __WXMSW__
		pgdc->StrokeLine (Intpts.x, Intpts.y, Intpts.x + 0.1, Intpts.y + 0.1);
#else
		pgdc->StrokeLine (Intpts.x, Intpts.y, Intpts.x, Intpts.y);
#endif
		
		delete pptsReal;
		iLoop++;
		
		// returning to basic pen
		if (bAsSelection)
		{
			bAsSelection = false;
			pgdc->SetPen(myPen);
		}
		
	}
	
	if (IsLoggingEnabled()){
		wxLogDebug(_T("%d Points drawn"), iLoop);
    }
	
	dc.SelectObject(wxNullBitmap);
	wxDELETE(pgdc);
	return TRUE;
}





bool tmDrawer::DrawPointsEnhanced(tmLayerProperties * itemProp, tmGISData * pdata){
	tmSymbolVectorPointMultiple * pSymbol = (tmSymbolVectorPointMultiple*) itemProp->GetSymbolRef();
	
	// simple symbology
	if (pSymbol->GetSelectedSymbology() == 0) { 
		return DrawPoints(itemProp, pdata);
	}
	
	// load query
	tmSymbolDataPointMultiple * mySymbology = pSymbol->GetSymbology();
	wxASSERT(mySymbology);
	DataBaseTM * myDB = ((tmGISDataVectorMYSQL*) pdata)->GetDataBaseHandle();
	wxASSERT(myDB);
	wxArrayLong myResult;
	if (_SelectFeatureByQuery(mySymbology->m_QueryID,myDB, myResult)==false) {
        mySymbology->m_PanelNo = 0;
		return DrawPoints(itemProp, pdata);
	}
	
	// create pens
	wxPen myValidPen (pSymbol->GetColourWithTransparency(mySymbology->m_SelColourMultiple,
														 mySymbology->m_GlobalTransparency),
					  mySymbology->m_SelRadiusMultiple);
	wxPen myUnValidPen (pSymbol->GetColourWithTransparency(mySymbology->m_UnSelColourMultiple,
														   mySymbology->m_GlobalTransparency),
						mySymbology->m_UnSelRadiusMultiple);
	wxPen mySelectionValidPen (m_SelMem->GetSelectionColour(),  mySymbology->m_SelRadiusMultiple);
	wxPen mySelectionUnValidPen (m_SelMem->GetSelectionColour(), mySymbology->m_UnSelRadiusMultiple);
	
	wxPen mySelectionValidHaloPen (*wxWHITE,  mySymbology->m_SelRadiusMultiple + 2);
	wxPen mySelectionUnValidHaloPen (*wxWHITE, mySymbology->m_UnSelRadiusMultiple + 2);
	
	
	bool myValidVisible = mySymbology->m_SelVisible;
	bool myUnValidVisible = mySymbology->m_UnSelVisible;
	
	// spatial filter
	tmGISDataVector * pVectPoint = (tmGISDataVector*) pdata;
	if(pVectPoint->SetSpatialFilter(m_spatFilter,itemProp->GetType())==false){
		return false;
	}
	
	// drawing
	wxMemoryDC temp_dc;
	temp_dc.SelectObject(*m_bmp);
	wxGraphicsContext* pgdc = wxGraphicsContext::Create( temp_dc);
	while (1) {
		long myOid = wxNOT_FOUND;
		wxRealPoint * pptsReal = pVectPoint->GetNextDataPoint(myOid);
		if(pptsReal == NULL){
			break;
		}
		
		bool IsSelected = false;
		if (m_ActuallayerID == m_SelMem->GetSelectedLayer()){
			if (m_SelMem->IsSelected(myOid)){
				IsSelected = true;
			}
		}
		
		bool IsValid = false;
		if (_ExistsinResults(myOid, myResult)==true) {
			IsValid = true;
		}
		
		// unvisible pen
		if (IsValid == true && myValidVisible ==false) {
			wxDELETE(pptsReal);
			continue;
		}
		
		if (IsValid == false && myUnValidVisible == false) {
			wxDELETE(pptsReal);
			continue;
		}
		
		// convert from real coordinates to screen coordinates
		wxPoint Intpts (0,0);
		Intpts = m_scale->RealToPixel(*pptsReal);
		
		if (IsSelected == true) {
			if (m_SelMem->GetSelectionHalo() == true) {
				
				
				if (IsValid == true) {
					pgdc->SetPen(mySelectionValidHaloPen);
				}
				else {
					pgdc->SetPen(mySelectionUnValidHaloPen);
				}
#ifdef __WXMSW__
				pgdc->StrokeLine (Intpts.x, Intpts.y, Intpts.x + 0.1, Intpts.y + 0.1);
#else
				pgdc->StrokeLine (Intpts.x, Intpts.y, Intpts.x, Intpts.y);
#endif
			}
		}
		
		if (IsValid==true) {
			if (IsSelected == true) {
				pgdc->SetPen(mySelectionValidPen);
			}
			else {
				pgdc->SetPen(myValidPen);
			}
		}
		else {
			if (IsSelected == true) {
				pgdc->SetPen(mySelectionUnValidPen);
			}
			else {
				pgdc->SetPen(myUnValidPen);
			}
		}
		
		
#ifdef __WXMSW__
		pgdc->StrokeLine (Intpts.x, Intpts.y, Intpts.x + 0.1, Intpts.y + 0.1);
#else
		pgdc->StrokeLine (Intpts.x, Intpts.y, Intpts.x, Intpts.y);
#endif
		wxDELETE(pptsReal);
		
	}
	
	temp_dc.SelectObject(wxNullBitmap);
	wxDELETE(pgdc);
	return true;
}



bool tmDrawer::DrawPointsRules (tmLayerProperties * itemProp, tmGISData * pdata){
    wxASSERT(itemProp->GetSymbolRuleManagerRef()->GetRulesRef()->GetCount() > 0);
	wxMemoryDC dc;
    dc.SelectObject(*m_bmp);
	wxGraphicsContext* pgdc = wxGraphicsContext::Create( dc);
    
    // define spatial filter
	tmGISDataVector * pVectPoint = (tmGISDataVector*) pdata;
	if(!pVectPoint->SetSpatialFilter(m_spatFilter,itemProp->GetType()))
	{
		if (IsLoggingEnabled()){
			wxLogError(_T("Error setting spatial filter"));
		}
		dc.SelectObject(wxNullBitmap);
		wxDELETE(pgdc);
		return false;
	}
    
    // process rules
    int iLoop = 0;
    tmSymbolRuleArray * myRulesArray = itemProp->GetSymbolRuleManagerRef()->GetRulesRef();
    wxASSERT(myRulesArray);
    for (unsigned int s = 0; s < myRulesArray->GetCount(); s++) {
        tmSymbolRule * myRule = myRulesArray->Item(s);
        wxASSERT(myRule);
        if (myRule->GetAttributFilter() == wxEmptyString || myRule->IsActive() == false) {
            continue;
        }
        
        if(pVectPoint->SetAttributFilter(myRule->GetAttributFilter())==false){
            continue;
        }
        
        wxPen myRulePen = myRule->GetPen();
        wxPen mySelectPen = myRulePen;
        mySelectPen.SetColour(m_SelMem->GetSelectionColour());
        mySelectPen.SetWidth(mySelectPen.GetWidth() + 1);
        pgdc->SetPen(myRulePen);
        
        while (1){
            long myOid = wxNOT_FOUND;
            wxRealPoint * pptsReal = pVectPoint->GetNextDataPoint(myOid);
            
            if (pptsReal == NULL){
                break;
            }
            
            // set brush
            pgdc->SetPen(myRulePen);
            if (m_ActuallayerID == m_SelMem->GetSelectedLayer()){
                if (m_SelMem->IsSelected(myOid)){
                    pgdc->SetPen(mySelectPen);
                }
            }
            
            wxPoint Intpts = m_scale->RealToPixel(*pptsReal);
#ifdef __WXMSW__
            pgdc->StrokeLine (Intpts.x, Intpts.y, Intpts.x + 0.1, Intpts.y + 0.1);
#else
            pgdc->StrokeLine (Intpts.x, Intpts.y, Intpts.x, Intpts.y);
#endif
             wxDELETE(pptsReal);
            iLoop++;
        }
    }
    
    if (IsLoggingEnabled()){
		wxLogDebug(_T("%d Lines drawn"), iLoop);
    }
	
	dc.SelectObject(wxNullBitmap);
	wxDELETE(pgdc);
	return true;
}





bool tmDrawer::DrawPolygons (tmLayerProperties * itemProp, tmGISData * pdata)
{
    if (itemProp->GetSymbolRuleManagerRef()->GetRulesRef()->GetCount() > 0 &&
        itemProp->GetSymbolRuleManagerRef()->IsUsingRules()==true) {
        return DrawPolygonsRules(itemProp, pdata);
    }
    
	// variables
	wxMemoryDC dc;
	bool bReturn = true;
	int iNbVertex = 0;
	int iLoop = 0;
	int i=0;
	
	wxStopWatch sv;
	// device context for drawing
	dc.SelectObject(*m_bmp);
	wxGraphicsContext* pgdc = wxGraphicsContext::Create( dc); 
	
	
	// create pen and brush based on symbology
	tmSymbolVectorPolygon * pSymbol = (tmSymbolVectorPolygon*) itemProp->GetSymbolRef();
	wxPen myPen (pSymbol->GetBorderColour(), pSymbol->GetBorderWidth());
	wxBrush myBrush (pSymbol->GetFillColour(),pSymbol->GetFillStyle());
	
	wxPen mySPen (m_SelMem->GetSelectionColour(), pSymbol->GetBorderWidth());
	wxBrush mySBrush (m_SelMem->GetSelectionColour(), pSymbol->GetFillStyle());
	
	pgdc->SetBrush(myBrush);
	pgdc->SetPen(myPen);
		
	// define spatial filter
	tmGISDataVector * pVectPoly = (tmGISDataVector*) pdata;
	if(!pVectPoly->SetSpatialFilter(m_spatFilter,itemProp->GetType()))
	{
		if (IsLoggingEnabled()){
			wxLogDebug(_T("Error setting spatial filter"));
		}
		dc.SelectObject(wxNullBitmap);
		wxDELETE(pgdc);
		return false;
	}
	
	long myOid = -1;
	bool bAsSelection = false;
	 //loop all features 
	while (1)
	{
		// get polygons info
		int iPolyRings = pVectPoly->GetNextDataPolygonInfo(myOid);
		if (iPolyRings <= 0)
		{
			if (IsLoggingEnabled()){
				wxLogDebug(_T("Error getting info about polygons, return value is : %d"), iPolyRings);
            }
			break;
		}
		
		/* Temp code, for debuging remove after
		if (iPolyRings > 1)
		{
			if (IsLoggingEnabled()){
				wxLogDebug(_T("Polygon : %d contain : %d rings"),iLoop, iPolyRings);
            }
		}*/
			
		wxGraphicsPath myPolygonPath = pgdc->CreatePath();
		// get polygons data, loop all rings into polygons
		for (i = 0; i<iPolyRings; i++)
		{
			wxRealPoint * pptsReal = pVectPoly->GetNextDataPolygon(i, iNbVertex);
			
			if(pptsReal == NULL)
			{
				if (IsLoggingEnabled()){
					wxLogDebug(_T("No point returned @polygon: %d @loop : %d"), iLoop, i);
                }
				bReturn = FALSE;
				break;
			}
			
			// changing pen for selected data
			if (m_ActuallayerID == m_SelMem->GetSelectedLayer())
				if (m_SelMem->IsSelected(myOid))
				{
					pgdc->SetPen(mySPen);
					pgdc->SetBrush(mySBrush);
					bAsSelection = true;
				}
			
			
			// creating path based on ring data and putting this path
			// into main path
			wxGraphicsPath myPath = pgdc->CreatePath();
			myPath.MoveToPoint(m_scale->RealToPixel(pptsReal[0]));
			for (int i = 1; i< iNbVertex; i++)
				myPath.AddLineToPoint(m_scale->RealToPixel(pptsReal[i]));
			
			myPath.CloseSubpath();
			myPolygonPath.AddPath(myPath);
			delete [] pptsReal;
		}
		
		
		pgdc->DrawPath(myPolygonPath);
	
		
		iLoop ++;
		
		// returning to basic pen
		if (bAsSelection)
		{
			pgdc->SetPen(myPen);
			pgdc->SetBrush(myBrush);
			bAsSelection = false;
		}
		
	}

	if (IsLoggingEnabled()){
		wxLogMessage(_T("%d Polygons drawn in %ld [ms]"), iLoop, sv.Time());
    }
	
	dc.SelectObject(wxNullBitmap);
	wxDELETE(pgdc);
	return bReturn;
}


bool tmDrawer::DrawPolygonsRules (tmLayerProperties * itemProp, tmGISData * pdata){
    wxASSERT(itemProp->GetSymbolRuleManagerRef()->GetRulesRef()->GetCount() > 0);
	wxMemoryDC dc;
    dc.SelectObject(*m_bmp);
	wxGraphicsContext* pgdc = wxGraphicsContext::Create( dc);    
    
    // define spatial filter
	tmGISDataVector * pVectPoly = (tmGISDataVector*) pdata;
	if(!pVectPoly->SetSpatialFilter(m_spatFilter,itemProp->GetType()))
	{
		if (IsLoggingEnabled()){
			wxLogError(_T("Error setting spatial filter"));
		}
		dc.SelectObject(wxNullBitmap);
		wxDELETE(pgdc);
		return false;
	}
    
    wxLogDebug(_("Number of Polygon to process : %d "), pVectPoly->GetCount());
    wxStopWatch sv;
    
    // process rules
    int iLoop = 0;
    tmSymbolRuleArray * myRulesArray = itemProp->GetSymbolRuleManagerRef()->GetRulesRef();
    wxASSERT(myRulesArray);
    for (unsigned int s = 0; s < myRulesArray->GetCount(); s++) {
        tmSymbolRule * myRule = myRulesArray->Item(s);
        wxASSERT(myRule);
        if (myRule->GetAttributFilter() == wxEmptyString || myRule->IsActive() == false) {
            continue;
        }
        
        if(pVectPoly->SetAttributFilter(myRule->GetAttributFilter())==false){
            continue;
        }
        
        wxBrush myRuleBrush = myRule->GetBrush();
        wxPen myRulePen = myRule->GetPen();
        wxBrush mySelectBrush = myRuleBrush;
        mySelectBrush.SetColour(m_SelMem->GetSelectionColour());
        mySelectBrush.SetStyle(wxBRUSHSTYLE_BDIAGONAL_HATCH);
        wxPen mySelectPen = myRulePen;
        mySelectPen.SetColour(m_SelMem->GetSelectionColour());
        mySelectPen.SetWidth(mySelectPen.GetWidth() + 1);
      
        pgdc->SetBrush(myRuleBrush);
        pgdc->SetPen(myRulePen);
        
        while (1){
            long myOid = wxNOT_FOUND;
            int iPolyRings = pVectPoly->GetNextDataPolygonInfo(myOid);
            if (iPolyRings <= 0){
                break;
            }
            wxGraphicsPath myPolygonPath = pgdc->CreatePath();
            for (int i = 0; i<iPolyRings; i++){
                int iNbVertex = 0;
                wxRealPoint * pptsReal = pVectPoly->GetNextDataPolygon(i, iNbVertex);
                
                if(pptsReal == NULL){
                    if (IsLoggingEnabled()){
                        wxLogError(_T("No point returned @polygon: %d @loop : %d"), iLoop, i);
                    }
                    continue;
                }
                
                // set brush
                pgdc->SetPen(myRulePen);
                pgdc->SetBrush(myRuleBrush);
                if (m_ActuallayerID == m_SelMem->GetSelectedLayer()){
                    if (m_SelMem->IsSelected(myOid)){
                        pgdc->SetPen(mySelectPen);
                        pgdc->SetBrush(mySelectBrush);
                    }
                }
 
                wxGraphicsPath myPath = pgdc->CreatePath();
                myPath.MoveToPoint(m_scale->RealToPixel(pptsReal[0]));
                for (int i = 1; i< iNbVertex; i++){
                    myPath.AddLineToPoint(m_scale->RealToPixel(pptsReal[i]));
                }
                
                myPath.CloseSubpath();
                myPolygonPath.AddPath(myPath);
                wxDELETEA(pptsReal);
            }
            pgdc->DrawPath(myPolygonPath);
            iLoop ++;
        }
    }
    
    if (IsLoggingEnabled()){
		wxLogMessage(_T("%d Polygons drawn in %ld [ms]"), iLoop, sv.Time());
    }
	
	dc.SelectObject(wxNullBitmap);
	wxDELETE(pgdc);
	return true;
}




bool tmDrawer::DrawRaster (tmLayerProperties * itemProp, tmGISData * pdata)
{
	
	tmGISDataRaster * pRaster = (tmGISDataRaster*) pdata;
	if(!pRaster->SetSpatialFilter(m_spatFilter, itemProp->GetType()))
	{
		return false;
	}
	
	// check if we need to draw raster (inside visible area)
	if (!pRaster->IsImageInsideVisibleArea())
	{
		return false;
	}
	
	imgbuf = NULL;
	maskbuf = NULL;
	
	// converting image coordinate & clipping 
	tmRealRect myClippedCoordReal = pRaster->GetImageClipedCoordinates();
	wxPoint topleftpx = m_scale->RealToPixel(wxRealPoint(myClippedCoordReal.x_min,
														myClippedCoordReal.y_min));
	wxPoint bottomright = m_scale->RealToPixel(wxRealPoint(myClippedCoordReal.x_max,
														  myClippedCoordReal.y_max));
	wxRect myClippedCoordPx (topleftpx, bottomright);
	wxImage * myRaster = new wxImage (myClippedCoordPx.GetWidth(), myClippedCoordPx.GetHeight(), true);

		
	if (pRaster->GetImageData(&imgbuf, &imglen, &maskbuf, &masklen, 
							  wxSize(myClippedCoordPx.GetWidth(),myClippedCoordPx.GetHeight())) != CE_None)
	{
		fprintf(stderr, "%s line %d : Error loading image data \n ", __FUNCTION__, __LINE__);
		// need cleanup
		// in all case, clean data
		if (imgbuf)
		{
			CPLFree(imgbuf);
			imgbuf = NULL;
		}

		if (maskbuf)
		{
			CPLFree(maskbuf);
			maskbuf = NULL;
		}
		return false;
	}
	
	unsigned char * myAlphaBuffer;
	if (pRaster->GetImageTranslucency(wxSize(myClippedCoordPx.GetWidth(),myClippedCoordPx.GetHeight()),
									  itemProp->GetSymbolRef()->GetTransparency(),
									  &myAlphaBuffer)==false)
		
	{
		if (myAlphaBuffer)
			CPLFree(myAlphaBuffer);
	}
	
	// data loaded successfully, draw image on display now
	// device context for drawing
	wxMemoryDC dc;
	dc.SelectObject(*m_bmp);
	//wxGraphicsContext* pgdc = wxGraphicsContext::Create( dc); 
	
	
	
	myRaster->SetData(imgbuf, true);
	myRaster->SetAlpha(myAlphaBuffer, true);

	dc.DrawBitmap(*myRaster, wxPoint(myClippedCoordPx.GetX(), myClippedCoordPx.GetY()),true);
	dc.SelectObject(wxNullBitmap);


	myRaster->Destroy();
	if (imgbuf)
	{
		CPLFree(imgbuf);
		imgbuf = NULL;
	}

	if (maskbuf)
	{
		CPLFree(maskbuf);
		maskbuf = NULL;
	}
	
	if (myAlphaBuffer)
	{
		CPLFree(myAlphaBuffer);
	}
	
	return TRUE;	
}



/***************************************************************************//**
 @brief Draw the vertex for lines and polygons
 @details If required by options, this function draw a dot for each vertex of
 the line or polygons
 @param pgdc pointer to a valid wxGraphicsContext (no check done)
 @param pts pointer to a list of nb_pts points. This function dosen't take owner
 of the list. Il must be destroyed elsewhere
 @param nb_pts number of points stored in pts
 @param pen Adress of a pen or pen array see also nb_pen
 @param nb_pen Number of pen stored into pen. May be used for drawing vertex in
 with different pen (not implemented now)
 @return true if no exception encoutered
 @param itemProp Properties of item.
 @author Lucien Schreiber (c) CREALP 2008
 @date 20 October 2008
 *******************************************************************************/
bool tmDrawer::DrawVertexLine (wxGraphicsContext* pgdc, wxPoint * pts, int nb_pts,
						   tmLayerProperties * itemProp, wxPen * pen, int nb_pen)
{
	int i = 0;	
	if (nb_pen == 1){ // only one pen
		pgdc->SetPen(*pen);
    }	

	switch (itemProp->GetVertexFlags())
	{
		case tmDRAW_VERTEX_ALL: // ALL VERTEX
			for (i = 0;i<nb_pts; i++){
                if (pts[i] == wxDefaultPosition) {
                    continue;
                }
                
			#ifdef __WXMSW__
				pgdc->StrokeLine (pts[i].x , pts[i].y, pts[i].x + 0.1, pts[i].y + 0.1);
			#else
				pgdc->StrokeLine (pts[i].x, pts[i].y, pts[i].x, pts[i].y);
			#endif
			}
			break;
			
		case tmDRAW_VERTEX_BEGIN_END: // ONLY BEGIN / END
			for (i = 0; i< nb_pts; i = i+nb_pts-1){
                if (pts[i] == wxDefaultPosition) {
                    continue;
                }
                
				#ifdef __WXMSW__
				pgdc->StrokeLine (pts[i].x, pts[i].y, pts[i].x + 0.1, pts[i].y + 0.1);
				#else
				pgdc->StrokeLine (pts[i].x, pts[i].y, pts[i].x, pts[i].y);
				#endif
			}
			break;
			
		default:
			break;
	}
	return true;
}



/***************************************************************************//**
 @brief Draw vertex for polygons
 @details Because polygons may hide points if we draw them in the same loop, we
 must draw vertex in a second loop to ensure they are displayed properly
 @param itemProp Item's properties
 @param pdata Item's data
 @return  true if drawing was successfull, false otherwise
 @author Lucien Schreiber (c) CREALP 2008
 @date 21 October 2008
 *******************************************************************************/
bool tmDrawer::DrawVertexPoly (tmLayerProperties * itemProp, tmGISData * pdata)
{
	// variables
	wxMemoryDC dc;
	bool bReturn = true;
	int iNbVertex = 0;
	int iLoop = 0;
	int i=0;
	
	// check if this function must run
	if (itemProp->GetVertexFlags() == tmDRAW_VERTEX_NONE){
		return false;
	}
	
	// device context for drawing
	dc.SelectObject(*m_bmp);
	wxGraphicsContext* pgdc = wxGraphicsContext::Create( dc); 
	
	
	// create pen for vertex
	tmSymbolVectorPolygon * pSymbol = (tmSymbolVectorPolygon*) itemProp->GetSymbolRef();
	wxPen * myVPen = CreateVertexUniquePen(itemProp, pSymbol->GetBorderWidth());
	
	// define spatial filter
	tmGISDataVector * pVectPoly = (tmGISDataVector*) pdata;
	if(!pVectPoly->SetSpatialFilter(m_spatFilter,itemProp->GetType()))
	{
		if (IsLoggingEnabled()){
			wxLogDebug(_T("Error setting spatial filter"));
		}
		wxDELETE(myVPen);
		return false;
	}
	
	long myOid = -1;
	//loop all features 
	while (1)
	{
		// get polygons info
		int iPolyRings = pVectPoly->GetNextDataPolygonInfo(myOid);
		if (iPolyRings <= 0)
		{
			if (IsLoggingEnabled()){
				wxLogDebug(_T("Error getting info about polygons, return value is : %d"), iPolyRings);
            }
			break;
		}
		
		if (iPolyRings > 1)
		{
			if (IsLoggingEnabled()){
				wxLogDebug(_T("Polygon : %d contain : %d rings"),iLoop, iPolyRings);
            }
		}
		
		// get polygons data, loop all rings into polygons
		for (i = 0; i<iPolyRings; i++)
		{
			wxRealPoint * pptsReal = pVectPoly->GetNextDataPolygon(i, iNbVertex);
			if(pptsReal == NULL)
			{
				if (IsLoggingEnabled()){
					wxLogDebug(_T("No point returned @polygon: %d @loop : %d"), iLoop, i);
                }
				bReturn = FALSE;
				break;
			}
            // convert realpts to pxpts
            wxPoint * pptspx = new wxPoint[iNbVertex];
            for (int i = 0; i< iNbVertex; i++) {
                pptspx[i] = m_scale->RealToPixel(pptsReal[i]);
            }
            wxDELETEA(pptsReal);

			// drawing vertex
			DrawVertexLine(pgdc, pptspx, iNbVertex, itemProp, myVPen);
			wxDELETEA(pptspx);
			
		}
		iLoop ++;
		
	}
	wxDELETE(myVPen);
	dc.SelectObject(wxNullBitmap);
	return bReturn;
	
}



/***************************************************************************//**
 @brief Create a pen for drawin vertex
 @details This create an unique pen of color wxBLACK and where size is two time
 the line's size.
 @param itemProp Properties of item
 @param size This is half the size of the vertex (vertex radius)
 @return  Pointer to a wxPen. User must take care of destroying the pen after
 usage
 @author Lucien Schreiber (c) CREALP 2008
 @date 21 October 2008
 *******************************************************************************/
wxPen * tmDrawer::CreateVertexUniquePen (tmLayerProperties * itemProp, int size)
{
	wxPen * myPen = new wxPen (*wxBLACK, size*2);
	return myPen;
}



/***************************************************************************//**
 @brief Create a pen for drawing editing vertex
 @author Lucien Schreiber (c) CREALP 2009
 @date 03 February 2009
 *******************************************************************************/
wxPen * tmDrawer::CreateEditUniqueVertexPen (int size)
{
	wxPen * myPen = new wxPen (*wxBLACK, size);
	return myPen;
}



/***************************************************************************//**
 @brief Create a pen for drawing editing segment
 @author Lucien Schreiber (c) CREALP 2009
 @date 03 February 2009
 *******************************************************************************/
wxPen * tmDrawer::CreateEditUniqueSegmentPen (int size)
{
	wxPen * myPen = new wxPen (*wxRED, size);
	return myPen;
}


/***************************************************************************//**
 @brief Draw a vertex (used during editing)
 @param pt a point (real coordinates)
 @param size the size of the vertex to draw
 @param colour the wxColour to use for drawing vertex 
 @author Lucien Schreiber (c) CREALP 2009
 @date 03 February 2009
 *******************************************************************************/
void tmDrawer::DrawEditVertex (const wxRealPoint & pt,int size, wxColour colour)
{
	wxPoint myScreenPt = m_scale->RealToPixel(pt);
	wxMemoryDC mdc;
	mdc.SelectObject(*m_bmp);
	wxPen * myVPen = new wxPen(colour, size);
	mdc.SetPen(*myVPen);
	DrawPoint(myScreenPt, &mdc);	
	delete myVPen;
	mdc.SelectObject(wxNullBitmap);
}




/***************************************************************************//**
 @brief Draw the segment between two points
 @param pt1 first point (Real)
 @param pt2 second point (Real)
 @param size size of the segment
 @author Lucien Schreiber (c) CREALP 2009
 @date 03 February 2009
 *******************************************************************************/
void tmDrawer::DrawEditSegment (const wxRealPoint & pt1,
								const wxRealPoint & pt2,
								int size)
{
	wxPoint myPt1(0,0), myPt2(0,0);
	wxPen * mySegmentPen = CreateEditUniqueSegmentPen(size);
	wxPen * myVertexPen = CreateEditUniqueVertexPen(size);
	
	wxMemoryDC mdc;
	mdc.SelectObject(*m_bmp);

	myPt1 = m_scale->RealToPixel(pt1);
	
	// draw segment first, then vertex
	if (pt2 != wxRealPoint(0,0))
	{
		myPt2 = m_scale->RealToPixel(pt2);
		
		mdc.SetPen(*mySegmentPen);
		mdc.DrawLine(myPt1, myPt2);
		
		mdc.SetPen(*myVertexPen);
		DrawPoint(myPt2, &mdc);
	}
	
	
	if (pt1 != wxRealPoint(0,0))
	{
		DrawPoint(myPt1, &mdc);
	}
	
	
	// cleaning
	mdc.SelectObject(wxNullBitmap);
	delete myVertexPen;
	delete mySegmentPen;
}


/***************************************************************************//**
 @brief Draw a point
 @param pt a point value in screen coordinates
 @param pMdc a valid wxMemory object
 @author Lucien Schreiber (c) CREALP 2009
 @date 03 February 2009
 *******************************************************************************/
void tmDrawer::DrawPoint (const wxPoint & pt, wxMemoryDC * pMdc)
{
#ifdef __WXMSW__
	pMdc->DrawLine(pt.x, pt.y, pt.x + 0.1, pt.y + 0.1);
#else
	pMdc->DrawLine(pt.x, pt.y, pt.x, pt.y);
#endif
}


/***************************************************************************//**
 @brief Draw a point
 @param pt a point value in screen coordinates
 @param pcdc a valid wxClientDC object
 @author Lucien Schreiber (c) CREALP 2009
 @date 04 February 2009
 *******************************************************************************/
void tmDrawer::DrawPoint (const wxPoint & pt, wxClientDC * pcdc)
{
#ifdef __WXMSW__
	pcdc->DrawLine(pt.x, pt.y, pt.x + 0.1, pt.y + 0.1);
#else
	pcdc->DrawLine(pt.x, pt.y, pt.x, pt.y);
#endif
}



/***************************************************************************//**
 @brief Draw all edited lines
 @param pts Adress of a wxArrayRealPoints
 @param size Size of the line
 @param colour the Colour of the line to draw, default is RED
 @author Lucien Schreiber (c) CREALP 2009
 @date 04 February 2009
 *******************************************************************************/
void tmDrawer::DrawEditLine (const wxArrayRealPoints & pts, int size, wxColour colour)
{
	wxMemoryDC mdc;
	mdc.SelectObject(*m_bmp);
	
	wxPen * mySegPen = new wxPen(colour, size);
	wxPen * myVertexPen = CreateEditUniqueVertexPen(size);
	
	// convert real to screen
	int nb_pts = pts.GetCount();
	wxPoint * myPts = new wxPoint[nb_pts];
	for (int i = 0; i<nb_pts; i++)
	{
		myPts[i] = m_scale->RealToPixel(pts.Item(i));
	}
	
	// draw segments
	mdc.SetPen(*mySegPen);
	mdc.DrawLines(nb_pts, myPts);
	
	// Draw Vertex
	mdc.SetPen(*myVertexPen);
	for (int j = 0; j< nb_pts; j++)
	{
		DrawPoint(myPts[j], &mdc);		
	}
	
	delete [] myPts;
	delete mySegPen;
	delete myVertexPen;
	
}


/*
void tmDrawer::DrawMemoryData (tmGISData * data,
							   tmLayerProperties * layerprop,
							   wxClientDC * dc)
{
	wxASSERT (layerprop);
	wxASSERT (data);
	//m_ActuallayerID = layerprop->m_LayerID;
	switch (layerprop->GetSpatialType())
	{
		case LAYER_SPATIAL_LINE:
			DrawMemoryDataLine(data, layerprop, dc);
			break;
			
		default:
			wxFAIL_MSG(_T("No drawer found"));
			break;
	}
	
}*/


/*
void tmDrawer::DrawMemoryDataLine (tmGISData * data, 
								   tmLayerProperties * layerprop,
								   wxClientDC * dc)
{
	
	tmGISDataVectorMemory * memdata = (tmGISDataVectorMemory*) data;
	if (memdata->GetVertexCount() <= 0)
		return;
	
	
	//wxClientDC dc(this);
	//wxGraphicsContext* pgdc = wxGraphicsContext::Create(dc);
	
	// create pen based on symbology
	tmSymbolVectorLine * pSymbol = (tmSymbolVectorLine*) layerprop->GetSymbolRef();
	wxPen myPen (pSymbol->GetColour(),pSymbol->GetWidth());//, pSymbol->GetShape());
	dc->SetPen(myPen);
		
	int iVertexNumber = 0;
	wxRealPoint * pptsReal = memdata->GetVertexAll(iVertexNumber);
	wxASSERT(pptsReal != NULL);
	wxASSERT(iVertexNumber > 0);
	
	wxPoint * myPxPts = new wxPoint[iVertexNumber];
	for (int i = 0; i< iVertexNumber; i++)
		myPxPts[i] = m_scale->RealToPixel(pptsReal[i]);
	
	// creating path
	dc->DrawLines(iVertexNumber, myPxPts);
	
	// draw vertex
	wxPen * myVPen = CreateVertexUniquePen(layerprop, pSymbol->GetWidth());
	dc->SetPen(*myVPen);
	for (int i = 0; i< iVertexNumber; i++) {
#ifdef __WXMSW__
		dc->DrawLine(myPxPts[i].x, myPxPts[i].y, myPxPts[i].x + 0.1, myPxPts[i].y + 0.1);
#else
		dc->DrawLine (myPxPts[i].x, myPxPts[i].y, myPxPts[i].x, myPxPts[i].y);
#endif
	}
	wxDELETE(myVPen);
	delete [] myPxPts;
	delete [] pptsReal;
}
*/

