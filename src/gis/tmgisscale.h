/***************************************************************************
 tmgisscale.h
 Deals with GIS scale and conversion
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

#ifndef _TM_GIS_SCALE_H_
#define _TM_GIS_SCALE_H_

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include <wx/graphics.h>
#include <algorithm>
#include "ogrsf_frmts.h"		// OGR accessing
#include "../core/vrrealrect.h"
#include "../core/projectdefmemory.h"
#include "../core/tmcoordconvert.h"


const int tmSCALE_MARGIN = 10; // margin between image and border when zooming to full extent
bool tmIsSameDouble(double left, double right, double epsilon);


/***************************************************************************//**
 @brief Class representing real rectangle
 @details Used for storing bounding box values
 @author Lucien Schreiber (c) CREALP 2008
 @date 14 July 2008
 *******************************************************************************/
class tmRealRect
{
private:
    double GetDifferences (const double & min, const double & max);
    
public:
    double x_min;
    double y_min;
    double x_max;
    double y_max;
    
    tmRealRect() : x_min(0.0), y_min(0.0), x_max(0.0), y_max(0.0) { }
    tmRealRect(double xmin, double ymin, double xmax, double ymax) :
    x_min(xmin), y_min(ymin), x_max(xmax), y_max(ymax) { }
    
    bool Clip (const tmRealRect & src, tmRealRect & result);
    
    bool operator==(const tmRealRect& pt) const
    {
        return wxIsSameDouble(x_min, pt.x_min) && wxIsSameDouble(y_min, pt.y_min)
        && wxIsSameDouble(x_max, pt.x_max) && wxIsSameDouble(y_max, pt.y_max);
    }
    
    bool operator!=(const tmRealRect& pt) const
    {
        if (wxIsSameDouble(x_min, pt.x_min)==false){
            return true;
        }
        if (wxIsSameDouble(y_min, pt.y_min)==false) {
            return true;
        }
        if (wxIsSameDouble(x_max, pt.x_max)==false) {
            return true;
        }
        if (wxIsSameDouble(y_max, pt.y_max)==false) {
            return true;
        }
        return false;
    }
    
    double GetWidth () {return GetDifferences(x_min, x_max);}
    double GetHeight () {return GetDifferences(y_min, y_max);}
	
    OGRGeometry * GetPolygonGeometry ();
};




class tmGISScale : public wxObject
{
private:
    tmRealRect m_ExtentMaxLayers;
    wxRect m_ExtentWnd;
    tmRealRect m_ExtentWndReal;
    double m_PixelSize;
    wxSize m_ExtentWndMM;
    long m_UnitScale;
    double m_WidthDistanceInM;
    wxSize m_PPI;
    PRJDEF_UNIT_TYPE m_ProjectUnit;
    PRJDEF_PROJ_TYPE m_ProjectProjection;
    
    void InitMemberValues();
    void _ComputeUnitScale ();

protected:
public:
    tmGISScale();
    ~tmGISScale();
    
    // static functions
    static tmRealRect ComputeMaxCoord (const tmRealRect & r1, const tmRealRect & r2);
    
    void SetPPI (const wxSize & ppi){m_PPI = ppi;}
    void SetUnitAndProj (PRJDEF_PROJ_TYPE proj, PRJDEF_UNIT_TYPE units);

    // setter, mostly used for unit testing
    void SetExtentWndReal(const tmRealRect &ExtentWndReal){m_ExtentWndReal = ExtentWndReal;}
    void SetWidthDistanceInM(double WidthDistanceInM) {m_WidthDistanceInM = WidthDistanceInM;}

    // setter and getter for layers
    void SetMaxLayersExtentAsExisting (const tmRealRect & r);
    void SetMaxLayersExtent (const tmRealRect & r){ m_ExtentMaxLayers = r;}
    tmRealRect GetMaxLayersExtent(){ return m_ExtentMaxLayers;}
    double GetLayersExtentWidth();
    double GetLayersExtentHeight();
    double GetPixelSize(){return m_PixelSize;}
    wxPoint2DDouble GetTopLeftValue(){return wxPoint2DDouble(m_ExtentWndReal.x_min, m_ExtentWndReal.y_max);}
    
    // setter and getters for windows
    void SetWindowExtent (const wxRect & extent){m_ExtentWnd = extent;}
    wxRect GetWindowExtent () {return m_ExtentWnd;}
    tmRealRect GetWindowExtentReal() {return m_ExtentWndReal;}
    void SetWindowExtentMM (const wxSize & size)
    {
        m_ExtentWndMM = size;
        _ComputeUnitScale();
    }
    long GetActualScale (){return m_UnitScale;}
    wxString GetVisibleWidthText ();
    double GetWindowRealWidth();
    double GetwindowRealHeight();
    
    // computing scale and reduction factor
    double ComputeDivFactor (wxSize wnd_extent = wxDefaultSize);
    bool ComputeMaxExtent ();
    wxSize ComputeCenterPxWnd (double divratio, wxSize wnd_extent = wxDefaultSize);
    bool ComputeMaxExtentReal (wxSize wnd_offset = wxDefaultSize);
    
    // size windows changed -> px size change too
    bool ComptuteNewWindowSize (const wxSize & oldsize, const wxSize & newsize);
    
    // scale function
    void ComputeNewScaleExtent (const long & scale);
    
    // zoom functions
    double GetBestDivFactor (const wxRect & selected_rect);
    void ComputeNewRealZoomExtent (const wxRect & calc_wnd_extent, const wxPoint & top_left);
    void ComputePrevZoomExtent (double pixelsize,const wxPoint2DDouble & topleft);
    
    // pan functions
    void ComputeNewRealPanExtent (const wxPoint & offsetxtop);
    
    // moving, zooming display
    bool MoveViewTo (const vrRealRect & rect);
    bool ZoomViewTo (const vrRealRect & rect);
    
    // mostly used for snapping
    double MetersToPixels(int meters);
    double MetersToRealUnits (int meters);
    
    // converting pixels - real (with inverting y axis)
    inline wxRealPoint PixelToReal (wxPoint pt)
    {
        return (wxRealPoint( m_ExtentWndReal.x_min + (((double) pt.x) * m_PixelSize),
                            m_ExtentWndReal.y_max - (((double) pt.y)* m_PixelSize)));
    }
    
    wxPoint RealToPixel (wxRealPoint realpt);
    tmRealRect PixelsToReal (const wxRect & rectpx);
    
    
    static inline double DifferenceDouble (const double & d1, const double &d2)
    {
        if (wxIsSameDouble(d1, d2))
            return 0;
        if (d1 > d2)
            return d1 - d2;
        else
            return d2-d1;
    }
    
    
    static inline double DifferenceCoord (const double & coordmax, const double & coordmin)
    {
        if (wxIsSameDouble(coordmax, coordmin))
            return 0;
        if (coordmax <= 0 && coordmin >= 0)
            return coordmax + coordmin;
        if (coordmax >= 0 )//&& coordmin < coordmax)
            return coordmax - coordmin;
        
        // if case isn't taken into account
        wxString sFunction = wxString::FromAscii(__FUNCTION__);
        wxString sFunctionLineError = wxString::Format( _T("%s line %d : "),
                                                       sFunction.c_str(), __LINE__);
        wxString sErrMsg = wxString::Format(_T("%s values are coord min-max : %.*f - %.*f "),
                                            sFunctionLineError.c_str(), 2, coordmin, 2 , coordmax);
        wxASSERT_MSG(0,sErrMsg);
        
        return 0;
    }
    
    static inline double RemoveFromCoord (const double & coord1, const double & value)
    {
        if (coord1 > 0)
            return coord1 - value;
        else
            return coord1 + value;
    }
    
    static inline double AppendToCoord (const double & coord1, const double & value)
    {
        //if (coord1 > 0)
        return coord1 + value;
        //else
        //	return coord1 - value;
    }
    
    // extent validity
    bool IsLayerExtentValid();
};


#endif
